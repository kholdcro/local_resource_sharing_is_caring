#include "Defs_GLB.h"
#include "Defs_MOD.h"
#include "Mnge_DAC.h"
#include "Sens_ENC.h"
#include "Acts_LIN.h"
#include "Acts_ROT.h"
#include "Coms_ESP.h"
#include "Coms_123.h"
#include "Sens_MOD.h"
#include "math.h"
#include "dsp.h"
#include "Mnge_ERR.h"

uint16_t ANG_Desired[3] = {1800, 1800, 1800}; // -180.0 to 180.0 deg = 0 to 3600
float VEL_Desired[3] = {0.f, 0.f, 0.f};
volatile float ANG_Deadband = MotRot_PID_Deadband;
volatile float ANG_DeadbandInverse = 1.0f / MotRot_PID_Deadband;
uint8_t Trq_Limit[3] = {0, 0, 0}; // save torque limit during wiggle
uint8_t Speed_100[3] = {MotRot_SpeedInit, MotRot_SpeedInit, MotRot_SpeedInit};
float Speed_DEG[3] = {(((float) MotRot_SpeedInit) * 0.01f) * MotRot_SpeedMax * MotRot_PID_period,
    (((float) MotRot_SpeedInit) * 0.01f) * MotRot_SpeedMax * MotRot_PID_period,
    (((float) MotRot_SpeedInit) * 0.01f) * MotRot_SpeedMax * MotRot_PID_period};
uint8_t DrvInterval[3] = {0, 0, 0};

float PID_GainP = MotRot_PID_kP;
float PID_GainI = MotRot_PID_kI;
float PID_GainD = MotRot_PID_kD;
float SPD_Gain = MotRot_SPD_k;

volatile float Vel_I_SPD[3] = {0.f, 0.f, 0.f};
volatile float Vel_tgt_unsanitized[3] = {0.f, 0.f, 0.f};

uint8_t ANG_OkRange = MotRot_OkRange;

volatile bool SPD_AvgFlag[3] = {false, false, false};
volatile int16_t SPD_AvgOut[3] = {0, 0, 0};
volatile int16_t SPD_AvgIn[3] = {0, 0, 0};
volatile float Vel_TgtIn[3] = {0.f, 0.f, 0.f};

#define WHEEL 68.15f // wheel distance from vertex
#define SxOUT 0.9f // output speed factor for non-primary wheels (tune curve)
#define INV_THREE 0.333333f // division avoidance
#define INV255_x1800 7.058824f // factor for 255 to 180pwm conversion
#define SMALL_PI 3.141592f

/* ******************** ROTARY MOTOR OUTPUTS ******************************** */
void Acts_ROT_Out(uint8_t edge, int16_t duty) {
    if (!MODE_MotRot_Active || !FLG_MotRot_Active) duty = 0; // rotary motors off
    switch (edge) {
        case 0:
            if (duty > 0) ROT_DIR_1 = 1; // direction output
            else ROT_DIR_1 = 0;
            PWM_Set(ROT_PWM_1, abs(duty)); // pwm output
            break;

        case 1:
            if (duty > 0) ROT_DIR_2 = 1; // direction output
            else ROT_DIR_2 = 0;
            PWM_Set(ROT_PWM_2, abs(duty)); // pwm output
            break;

        case 2:
            if (duty > 0) ROT_DIR_3 = 1; // direction output
            else ROT_DIR_3 = 0;
            PWM_Set(ROT_PWM_3, abs(duty)); // pwm output
            break;

        default:
            break;
    }
}

/* ******************** ROTARY MOTOR PID ************************************ */
void Acts_ROT_PID(uint8_t edge, float current, uint16_t target) {
    float OUT; // pwm output variable
    static float PID_I[3] = {0, 0, 0}; // integral error variable
    static bool SPD_Flag[3] = {false, false, false}; // speed controller on flag
    static bool SPD_Used[3] = {false, false, false}; // speed output used flag
    static float errorOld[3] = {0, 0, 0}; // previous error (derivative gain)
    static uint16_t targetOld1[3] = {0, 0, 0};
    
    // avoid bad control inputs
    target = clamp_ui16(target, MotRot_AngleIntMIN, MotRot_AngleIntMAX);

    const float error = ((float) target) * 0.1f - 180.0f - current;
    
    // stop speed controller when target reached
    if (SPD_Flag[edge] && (fabsf(error) < 0.1f)){
        SPD_Flag[edge] = false;
        if ((CMD_ID != 254) && SPD_Used[edge]) PID_I[edge] = 0.0f;
        /* if it gets to target without position controller used
         * (ie SPD output was always smaller than position), reset
         * position PID to avoid overshoot */
    }

    // reset integral component when target new 
    if (target != targetOld1[edge]){
        if ((CMD_ID != 254) ||
            ((CMD_ID == 254) && (sgn(PID_I[edge]) != sgn(error)))) {
            PID_I[edge] = 0.0f;
        }
        targetOld1[edge] = target;
    }
    
    // integral component
    if (fabsf(error) < Acts_ROT_GetAngleDeadband()) {
        // only allow wind-down within deadband
        if ((sgn(PID_I[edge]) != sgn(error)) && (fabsf(PID_I[edge]) < epsilon))
            PID_I[edge] += error * MotRot_PID_period;
        // clamp to max output value in deadband
        if (error < 0) PID_I[edge] = clamp_f(PID_I[edge], 
                -MotRot_PID_Imax * fabsf(error) * Acts_ROT_GetAngleDeadbandInverse(), 0.0f);
        else if (error > 0) PID_I[edge] = clamp_f(PID_I[edge], 
                0.0f, MotRot_PID_Imax * fabsf(error) * Acts_ROT_GetAngleDeadbandInverse());
    } else if (fabsf(error) < 7.0f){ 
        // avoid integral build up when far away (if error >6.69, kp maxes PWM)
        PID_I[edge] += error * MotRot_PID_period;
        PID_I[edge] = clamp_f(PID_I[edge], -MotRot_PID_Imax, MotRot_PID_Imax);
    } else PID_I[edge] = 0.0f;

    // derivative component
    float PID_D = (error - errorOld[edge]) * MotRot_PID_freq;
    errorOld[edge] = error;

    // limit derivative
    PID_D = clamp_f(PID_D, -MotRot_PID_Dmax, MotRot_PID_Dmax);

    // PID position output
    float outP = PID_GainP * error + PID_GainI * PID_I[edge] + PID_GainD * PID_D;
    outP = clamp_f(outP, -MotRot_PID_Max, MotRot_PID_Max); // limit output
    
    // speed control
    if (Speed_100[edge] != 100) {
        static uint16_t targetOld2[3] = {0, 0, 0};
        static uint8_t newTargetCount[3] = {0, 0, 0};
        static float SPD[3] = {0, 0, 0}; // speed control integral term

        // when new target, set speed direction and feed forward
        if ((target != targetOld2[edge]) && (CMD_ID != 254)){
            SPD[edge] = copysgn(MotRot_SPD_Max, error) * (((float) Acts_ROT_GetSpeedLimit(edge)) * 0.01f);
            targetOld2[edge] = target;
            newTargetCount[edge] = 0;
            SPD_Flag[edge] = true;
        } else {
            if((CMD_ID == 254) && (target != targetOld2[edge])){
                targetOld2[edge] = target;
                SPD_Flag[edge] = true;
            }    
            // speed control (integral)
            if (!Flg_SensFus)
                SPD[edge] += SPD_Gain * (copysgn(Speed_DEG[edge], error) - Sens_ENC_GetDelta(edge));
            else
                SPD[edge] += SPD_Gain * (copysgn(Speed_DEG[edge], error) - Sens_MOD_GetDelta(edge));
            if (sgn(error) != sgn(SPD[edge])) SPD[edge] = 0;
        }
        
        // ignore other direction (no additional breaking under load)
        if (error > 0.0f) SPD[edge] = clamp_f(SPD[edge], 0.0f, MotRot_SPD_Max);
        else if (error < -0.0f) SPD[edge] = clamp_f(SPD[edge], -MotRot_SPD_Max, -0.0f);
        else SPD[edge] = 0.0f;
        Acts_ROT_SetSPDAvgOut(edge, (int16_t)(SPD[edge]));

        // average speed integral 
        if (SPD_AvgFlag[edge]){
            if (newTargetCount[edge] >= 16){ // ignore first loops before averaging
                SPD_AvgFlag[edge] = false;
                SPD[edge] = (SPD[edge] + ((float)Acts_ROT_GetSPDAvgNeighbour(edge))) * 0.5f;
            } else newTargetCount[edge]++;
        }
        
        if (error > 0.0f) SPD[edge] = clamp_f(SPD[edge], 0.0f, MotRot_SPD_Max);
        else if (error < -0.0f) SPD[edge] = clamp_f(SPD[edge], -MotRot_SPD_Max, -0.0f);
        else SPD[edge] = 0.0f;
        
        // whichever is smallest in direction of target
        if (SPD_Flag[edge] && // if target has not been reached yet
                (fabsf(SPD[edge]) < fabsf(outP))){
            OUT = SPD[edge];
            SPD_Used[edge] = true;
        } else {
            OUT = outP;
            SPD_Used[edge] = false;
            
            // if target reached, scale down PWM by half to lightly hold position 
            if (FLG_ReducePWMatTarget && (!SPD_Flag[edge])) OUT *= 0.6;
        }
    } else {
        OUT = outP; // full speed position output
    }
    
    // scale down PWM towards zero within deadband
    if (fabsf(error) < Acts_ROT_GetAngleDeadband())
        OUT *= fabsf(error) * Acts_ROT_GetAngleDeadbandInverse();
    
    // scale down pwm at large offset with neighbour
    if (!Flg_SensFus)
        OUT *= Sens_ENC_GetLrgOffsetMult(edge);
    else
        OUT *= Sens_MOD_GetLrgOffsetMult(edge);
    
    Acts_ROT_Out(edge, (int16_t) OUT); // output pwm
}

/* ******************** EXECUTE WIGGLE ************************************** */
void Acts_ROT_Wiggle(uint8_t edge) {
    static uint16_t Wgl_Count[3] = {0, 0, 0};
    Wgl_Count[edge]++;
    if (Wgl_Count[edge] <= (MotRot_WiggleTime * 100)) {
        if (Wgl_Count[edge] <= MotRot_WiggleTime * 60) {
            Acts_ROT_Out(edge, 300);
            if (Flg_EdgeSyn[edge]) {
                Wgl_Count[edge] = MotRot_WiggleTime * 61;
                if (Flg_DriveAndCouple[edge]) {
                    uint8_t i;
                    for (i = 0; i < 3; i++) if (i != edge) Acts_ROT_Out(i, 0);
                }
            }
        } else if (Wgl_Count[edge] <= MotRot_WiggleTime * 90)
            Acts_ROT_Out(edge, -300);
        else if (Wgl_Count[edge] <= MotRot_WiggleTime * 98)
            Acts_ROT_Out(edge, 300);
        else
            Acts_ROT_Out(edge, -300);
    } else {
        Acts_ROT_Out(edge, 0);
        Mnge_DAC_Set(edge, Trq_Limit[edge]);
        Flg_EdgeWig[edge] = false;
        Wgl_Count[edge] = 0;
    }
}

/* ******************** DRIVE FUNCTION ************************************** */
void Acts_ROT_Drive(uint8_t speed, int8_t curve, uint8_t edge, uint8_t direc) {
    float a, b, c; // extension values from 180
    float Mo = curve * 242.4;//137.9f;
    float Sa = speed * INV255_x1800; // 255 to 180 max
    if (!direc) Sa = -1.0f * Sa; // inwards or outwards
    
    // max Da = 31.725

    switch (edge) {
        case 0:
            a = 180 + (MotLin_MAX_1 - Acts_LIN_GetTarget(0))*12 / (MotLin_MAX_1 - MotLin_MIN_1);
            b = 180 + (MotLin_MAX_2 - Acts_LIN_GetTarget(1))*12 / (MotLin_MAX_2 - MotLin_MIN_2);
            c = 180 + (MotLin_MAX_3 - Acts_LIN_GetTarget(2))*12 / (MotLin_MAX_3 - MotLin_MIN_3);
            break;
        case 1:
            a = 180 + (MotLin_MAX_2 - Acts_LIN_GetTarget(1))*12 / (MotLin_MAX_2 - MotLin_MIN_2);
            b = 180 + (MotLin_MAX_3 - Acts_LIN_GetTarget(2))*12 / (MotLin_MAX_3 - MotLin_MIN_3);
            c = 180 + (MotLin_MAX_1 - Acts_LIN_GetTarget(0))*12 / (MotLin_MAX_1 - MotLin_MIN_1);
            break;
        case 2:
            a = 180 + (MotLin_MAX_3 - Acts_LIN_GetTarget(2))*12 / (MotLin_MAX_3 - MotLin_MIN_3);
            b = 180 + (MotLin_MAX_1 - Acts_LIN_GetTarget(0))*12 / (MotLin_MAX_1 - MotLin_MIN_1);
            c = 180 + (MotLin_MAX_2 - Acts_LIN_GetTarget(1))*12 / (MotLin_MAX_2 - MotLin_MIN_2);
            break;
        default:
            a = 180;
            b = 180;
            c = 180;
            break;
    }

    // vertex angles (float alpha = acosf((b*b + c*c - a*a)/(2*b*c));)
    float beta = acosf((a * a + c * c - b * b) / (2 * a * c));
    float gamm = acosf((a * a + b * b - c * c) / (2 * a * b));

    // wheel coordinates (for a: [WHEEL, 0])
    float Wb[2] = {(b - WHEEL) * cosf(gamm), (b - WHEEL) * sinf(gamm)};
    float Wc[2] = {a - WHEEL * cosf(beta), WHEEL * sinf(beta)};

    // second point in wheel direction
    float Wb2[2] = {Wb[0] - cosf(SMALL_PI * 0.5f - gamm), Wb[1] + sinf(SMALL_PI * 0.5f - gamm)};
    float Wc2[2] = {Wc[0] + cosf(SMALL_PI * 0.5f - beta), Wc[1] + sinf(SMALL_PI * 0.5f  - beta)};

    // centroid coordinates
    float D[2] = {(b * cosf(gamm) + a) * INV_THREE, b * sinf(gamm) * INV_THREE};

    // moment arm of wheel force to centroid
    float Da = fabsf(D[0] - WHEEL);
    float Db = fabsf((Wb2[1] - Wb[1]) * D[0]
            - (Wb2[0] - Wb[0]) * D[1] + Wb2[0] * Wb[1] - Wb2[1] * Wb[0])
            / sqrtf(powf(Wb2[1] - Wb[1], 2) + powf(Wb2[0] - Wb[0], 2));
    float Dc = fabsf((Wc2[1] - Wc[1]) * D[0]
            - (Wc2[0] - Wc[0]) * D[1] + Wc2[0] * Wc[1] - Wc2[1] * Wc[0])
            / sqrtf(powf(Wc2[1] - Wc[1], 2) + powf(Wc2[0] - Wc[0], 2));

    // wheel speeds
    float Sc = SxOUT*((Mo - Sa * Da) / (Db * cosf(SMALL_PI * 0.5f - beta) / cosf(SMALL_PI * 0.5f - gamm) + Dc));
    float Sb = SxOUT*(Sc * cosf(SMALL_PI * 0.5f - beta) / cosf(SMALL_PI * 0.5f - gamm));

    uint8_t i;
    for (i = 0; i < 3; i++) Acts_ROT_SetDrvInterval(i, 5); // lasts for 1 sec

    // output depending on driving edge
    switch (edge) {
        case 0:
            Acts_ROT_Out(0, (int16_t)(Sa));
            Acts_ROT_Out(1, (int16_t)(Sb));
            Acts_ROT_Out(2, (int16_t)(Sc));
            break;
        case 1:
            Acts_ROT_Out(1, (int16_t)(Sa));
            Acts_ROT_Out(2, (int16_t)(Sb));
            Acts_ROT_Out(0, (int16_t)(Sc));
            break;
        case 2:
            Acts_ROT_Out(2, (int16_t)(Sa));
            Acts_ROT_Out(0, (int16_t)(Sb));
            Acts_ROT_Out(1, (int16_t)(Sc));
            break;
        default:
            break;
    }
}

/* ******************** DRIVE HANDLE ************************************* */
void Acts_ROT_DrvHandle() {
    uint8_t edge;
    for (edge = 0; edge < 3; edge++) {
        if (DrvInterval[edge]) {
            DrvInterval[edge]--;
        } else {
            Flg_Drive[edge] = false;
        }
    }
}

void Acts_ROT_SetDrvInterval(uint8_t edge, uint8_t interval) {
    DrvInterval[edge] = interval;
    Flg_Drive[edge] = true;
}


/* ******************** ROTARY MOTOR PID ************************************ */
void Acts_ROT_VEL(uint8_t edge) {
    float OUT; // pwm output variable
    static float targetOld[3] = {0, 0, 0};
    static float errorOld[3] = {0, 0, 0};
    static float SPD[3] = {0, 0, 0}; // speed control integral term
    static float SPD_old[3] = {0, 0, 0}; // speed control integral term
    
    float currnt_spd = Sens_ENC_GetKalmanVel(edge) * MotRot_PID_freq;
    if (Flg_SensFus)
        currnt_spd = Sens_MOD_GetDelta(edge) * MotRot_PID_freq;
    
    float target_spd = Acts_ROT_GetVelTarget(edge);  
    
    //still need to stop motion if speed at its limit (Done at source))
//    target_spd = clamp_f(target_spd, -MotRot_SpeedMax, MotRot_SpeedMax);
    

    const float error = (target_spd - currnt_spd); 
//    float PID_D = (error - errorOld[edge]);
    errorOld[edge] = error;

    // limit derivative
//    PID_D = clamp_f(PID_D, -MotRot_PID_Dmax, MotRot_PID_Dmax);

    //reset if SPD integral not in the same direction as the target (remove, not useful))
//    if ((SPD[edge] != 0) && (sgn(target_spd) != sgn(SPD[edge])))
//        SPD[edge] = 0;
    
    // Only increase integral component when target in same direction
    if (sgn(target_spd) == sgn(targetOld[edge])){
        SPD[edge] += SPD_Gain * error;
    }
    targetOld[edge] = target_spd;
    
    // clamps speed to max values
    SPD[edge] = clamp_f(SPD[edge], -MotRot_PID_Max, MotRot_PID_Max);
    Acts_ROT_SetSPDAvgOut(edge, (int16_t)(SPD[edge]));
        
    //prevents mori from exceeding angle bounds
    uint16_t currnt_ang;
    if (!Flg_SensFus)
        currnt_ang = (uint16_t)(Sens_ENC_Get(edge, true)*10.+1800.);
    else
        currnt_ang = (uint16_t)(Sens_MOD_GetAngle(edge, true)*10.+1800.);
    if ((currnt_ang <= MotRot_AngleIntMIN) && SPD[edge] < 0) SPD[edge] = -0.f;
    if ((currnt_ang >= MotRot_AngleIntMAX) && SPD[edge] > 0) SPD[edge] = 0.f;

    // Need check if neighbour has same sign as spd.
    if(sgn(Acts_ROT_GetVelNeighbourTarget(edge)) != sgn(Acts_ROT_GetVelTarget(edge))){
        SPD[edge] = SPD_old[edge];
//        PID_D = 0;
    }    
    
    // Average speed integral with neighbour
    if (SPD_AvgFlag[edge]){
        SPD_AvgFlag[edge] = false;
        SPD[edge] = (SPD[edge] + ((float)Acts_ROT_GetSPDAvgNeighbour(edge))) * 0.5f;
    }
    
    Acts_ROT_SaveSPD(edge, SPD[edge]);
    SPD_old[edge] = SPD[edge];
        
//    OUT = SPD[edge] + 0.2*PID_D; 
    OUT = SPD[edge]; 
    
    // scale down pwm at large offset with neighbour
    if (!Flg_SensFus)
        OUT *= Sens_ENC_GetLrgOffsetMult(edge);
    else
        OUT *= Sens_MOD_GetLrgOffsetMult(edge);
    
    Acts_ROT_Out(edge, (int16_t) OUT); // output pwm
}


void Acts_ROT_SaveSPD(uint8_t edge, float SPD_edge) {
    Vel_I_SPD[edge] = SPD_edge;
}


float Acts_ROT_GetSPD(uint8_t edge) {
    return Vel_I_SPD[edge];
}


/* ******************** ACTIVATE WIGGLE ************************************* */
void Acts_ROT_SetWiggle(uint8_t edge) {
    Mnge_DAC_Set(edge, MotRot_WiggleTorque);
    Flg_EdgeWig[edge] = true;
}

/* ******************** ROTARY MOTOR CURRENT LIMIT ************************** */
void Acts_ROT_SetCurrentLimit(uint8_t edge, uint8_t limit) {
    Trq_Limit[edge] = limit; // save limit so wiggle remembers
    Mnge_DAC_Set(edge, limit);
}

/* ******************** SPEED LIMIT ***************************************** */
void Acts_ROT_SetSpeedLimit(uint8_t edge, uint8_t limit) {
    if (limit > 100) limit = 100;
    else if (limit < 1) limit = 1;
    // May need to set limit to 0
    Speed_DEG[edge] = ((float) limit) * 0.01f * MotRot_SpeedMax * MotRot_PID_period;
    Speed_100[edge] = limit;
    if(CMD_ID != 254)
        Coms_123_ResetCmdMatch(edge);
}

uint8_t Acts_ROT_GetSpeedLimit(uint8_t edge) {
    return Speed_100[edge];
}

/* ******************** DESIRED ANGLE *************************************** */
void Acts_ROT_SetTarget(uint8_t edge, uint16_t desired) {
    ANG_Desired[edge] = desired;
    Flg_EdgeAct[edge] = false; // reset act flag until cmd verified with neighbour
    Flg_EdgeReq_Ang[edge] = true;
    Flg_AllEdgRdy[edge] = false;
    Flg_NbrEdgRdy[edge] = false;
    Coms_123_ResetCmdMatch(edge);
}

uint16_t Acts_ROT_GetTarget(uint8_t edge) {
    return ANG_Desired[edge];
}


float Acts_ROT_GetVelTarget(uint8_t edge) {
    return VEL_Desired[edge];
}


volatile uint16_t Acts_ROT_SendVelTarget(uint8_t edge) {
    if((VEL_Desired[edge] > 0.1) && (VEL_Desired[edge] < 1.f)) 
        return 1801;
    if((VEL_Desired[edge] < -0.1) && (VEL_Desired[edge] > -1.f)) 
        return 1799;    
    return (uint16_t)(VEL_Desired[edge]+1800.f);
}

void Acts_ROT_SaveUnsaniTarget(uint8_t edge, float tgt_raw) {
    Vel_tgt_unsanitized[edge] = tgt_raw;
}


float Acts_ROT_GetUnsaniTarget(uint8_t edge) {
    return Vel_tgt_unsanitized[edge];
}


//THIS IS THE ISSUE AGAIN
void Acts_ROT_SetVelTarget(uint8_t edge, float vel_target) {
    Acts_ROT_SaveUnsaniTarget(edge, vel_target);
    static float vel_target_old[3] = {0.f, 0.f, 0.f};
    
    //Prevent "Spikes" to large numbers
    if (fabs(vel_target) >= MotRot_SpeedMax) //Max commanded speed
        return; // No need to set VEL_Desired[edge] = vel_target_old[edge]; VEL_Desired already at prev value 
    
    //Prevent sharp drops to 0
    if ((fabs(vel_target_old[edge]) > 5.f) && (fabs(vel_target) < 0.001))
        return;  
    
    vel_target_old[edge] = VEL_Desired[edge];
    
    VEL_Desired[edge] = vel_target; //negative = hax
}

void Acts_ROT_SetVelNeighbourTarget(uint8_t edge, int16_t neigh_vel_target) {
    Vel_TgtIn[edge] = ((float)neigh_vel_target) - 1800.f;
}

float Acts_ROT_GetVelNeighbourTarget(uint8_t edge) {
    return Vel_TgtIn[edge];
}

/* ******************** RETURN FORMATTED ANGLE ****************************** */
uint16_t Acts_ROT_GetAngle(uint8_t edge, bool withLiveOffset) {
    return (uint16_t)(10.0f * Sens_ENC_Get(edge, withLiveOffset) + 1800.0f);
}

/* ******************** RETURN WHETHER ALL IN DESIRED RANGE ***************** */
bool Acts_ROT_InRange(uint8_t edge, uint16_t angle) {
    const uint16_t diff = abs(angle - Acts_ROT_GetTarget(edge));
    if (diff <= ANG_OkRange) return true;
    return false;
}

/* ******************** MATH FUNCTIONS ************************************** */
int8_t sgn(float value){
    return (value > 0) - (value < 0);
}

float copysgn(float value, float check){
    if (check > 0.0f) return value;
    if (check < 0.0f) return -value;
    return 0.0f;
}

/* https://stackoverflow.com/questions/427477/fastest-way-to-clamp-a-real-fixed-floating-point-value */
float clamp_f(float d, float min, float max) {
  const float t = d < min ? min : d;
  return t > max ? max : t;
}

uint16_t clamp_ui16(uint16_t d, uint16_t min, uint16_t max) {
  const uint16_t t = d < min ? min : d;
  return t > max ? max : t;
}

/* ******************** SPEED AVERAGING ************************************* */
void Acts_ROT_SetSPDAvgOut(uint8_t edge, int16_t value){
    SPD_AvgOut[edge] = value;
}

uint16_t Acts_ROT_GetSPDAvgOut(uint8_t edge){
    return (uint16_t)(SPD_AvgOut[edge] + 1800);
}

//Used in speed PID
void Acts_ROT_SetSPDAvgNeighbour(uint8_t edge, uint16_t value){
    int16_t tmp_spd = ((int16_t)value) - 1800;
    if(abs(tmp_spd) > MotRot_SPD_Max) //spd should be clamped to 1024
        return;
    SPD_AvgFlag[edge] = true;
    SPD_AvgIn[edge] = tmp_spd;
}

int16_t Acts_ROT_GetSPDAvgNeighbour(uint8_t edge){
    return SPD_AvgIn[edge];
}

/* ******************** DEADBAND ******************************************** */
// https://deltamotion.com/support/webhelp/rmctools/Registers/Register_Descriptions/Axis_Parameter_Registers/Output/Param_Deadband_Tolerance.htm
void Acts_ROT_SetAngleDeadband(uint8_t value){
    ANG_Deadband = ((float) value) * 0.1f;
    ANG_DeadbandInverse = 1.0f / ANG_Deadband;
}

float Acts_ROT_GetAngleDeadband(){
    return ANG_Deadband;
}

float Acts_ROT_GetAngleDeadbandInverse(){
    return ANG_DeadbandInverse;
}

void Acts_ROT_SetOkRange(uint8_t value){
    ANG_OkRange = value;
}

/* ******************** ADJUST GAINS **************************************** */
void Acts_ROT_SetPIDGains(uint8_t pid, uint8_t value){
    switch (pid){
        case 0:
            PID_GainP = (float)value;
            break;
        case 1:
            PID_GainI = (float)value;
            break;
        case 2:
            PID_GainD = (float)value;
            break;
        default:
            break;
    }
}

void Acts_ROT_SetSPDGain(uint8_t value){
    SPD_Gain = (float)value;
}