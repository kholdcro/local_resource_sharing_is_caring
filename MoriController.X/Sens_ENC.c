#include <string.h>
#include "Defs_GLB.h"
#include "Defs_MOD.h"
#include "Defs_OFF.h"
#include "Sens_ENC.h"
#include "Coms_123.h"
#include "Acts_ROT.h"
#include "mcc_generated_files/i2c1.h"
#include "Mnge_ERR.h"
#include "math.h"

// Encoder AS5048B
volatile float ENC_Data[3] = {0, 0, 0};
volatile float ENC_DataOld[3] = {0, 0, 0};
volatile float ENC_DataAvg[3][5] = {{0, 0, 0, 0, 0},{0, 0, 0, 0, 0},{0, 0, 0, 0, 0}};
volatile float ENC_GlobOffset[3] = {0, 0, 0};
volatile float ENC_LiveOffset[3] = {0, 0, 0};

volatile float ENC_LrgOffsetMult[3] = {1, 1, 1};

//State understanding for Kalman filter
volatile float ENC_State_Pos[3] = {0, 0, 0};
volatile float ENC_State_Vel[3] = {0, 0, 0};

volatile bool Flg_KalInit = false;

volatile float ENC_Noise[3] = {Kal_Q1, Kal_Q2, Kal_R};

const int8_t ModuleOffsetData[3][NUM_MODS*3] = {
    {Off1_A1, Off1_A2, Off1_A3, Off1_B1, Off1_B2, Off1_B3, Off1_C1, Off1_C2, Off1_C3, Off1_D1, Off1_D2, Off1_D3, Off1_E1, Off1_E2, Off1_E3, Off1_F1, Off1_F2, Off1_F3, Off1_G1, Off1_G2, Off1_G3, Off1_H1, Off1_H2, Off1_H3, Off1_I1, Off1_I2, Off1_I3, Off1_J1, Off1_J2, Off1_J3, Off1_K1, Off1_K2, Off1_K3, Off1_L1, Off1_L2, Off1_L3, Off1_M1, Off1_M2, Off1_M3, Off1_N1, Off1_N2, Off1_N3},
    {Off2_A1, Off2_A2, Off2_A3, Off2_B1, Off2_B2, Off2_B3, Off2_C1, Off2_C2, Off2_C3, Off2_D1, Off2_D2, Off2_D3, Off2_E1, Off2_E2, Off2_E3, Off2_F1, Off2_F2, Off2_F3, Off2_G1, Off2_G2, Off2_G3, Off2_H1, Off2_H2, Off2_H3, Off2_I1, Off2_I2, Off2_I3, Off2_J1, Off2_J2, Off2_J3, Off2_K1, Off2_K2, Off2_K3, Off2_L1, Off2_L2, Off2_L3, Off2_M1, Off2_M2, Off2_M3, Off2_N1, Off2_N2, Off2_N3},
    {Off3_A1, Off3_A2, Off3_A3, Off3_B1, Off3_B2, Off3_B3, Off3_C1, Off3_C2, Off3_C3, Off3_D1, Off3_D2, Off3_D3, Off3_E1, Off3_E2, Off3_E3, Off3_F1, Off3_F2, Off3_F3, Off3_G1, Off3_G2, Off3_G3, Off3_H1, Off3_H2, Off3_H3, Off3_I1, Off3_I2, Off3_I3, Off3_J1, Off3_J2, Off3_J3, Off3_K1, Off3_K2, Off3_K3, Off3_L1, Off3_L2, Off3_L3, Off3_M1, Off3_M2, Off3_M3, Off3_N1, Off3_N2, Off3_N3}
};
const uint8_t ModuleIDs[NUM_MODS][6] = {
    {ID_A1, ID_A2, ID_A3, ID_A4, ID_A5, ID_A6},
    {ID_B1, ID_B2, ID_B3, ID_B4, ID_B5, ID_B6},
    {ID_C1, ID_C2, ID_C3, ID_C4, ID_C5, ID_C6},
    {ID_D1, ID_D2, ID_D3, ID_D4, ID_D5, ID_D6},
    {ID_E1, ID_E2, ID_E3, ID_E4, ID_E5, ID_E6},
    {ID_F1, ID_F2, ID_F3, ID_F4, ID_F5, ID_F6},
    {ID_G1, ID_G2, ID_G3, ID_G4, ID_G5, ID_G6},
    {ID_H1, ID_H2, ID_H3, ID_H4, ID_H5, ID_H6},
    {ID_I1, ID_I2, ID_I3, ID_I4, ID_I5, ID_I6},
    {ID_J1, ID_J2, ID_J3, ID_J4, ID_J5, ID_J6},
    {ID_K1, ID_K2, ID_K3, ID_K4, ID_K5, ID_K6},
    {ID_L1, ID_L2, ID_L3, ID_L4, ID_L5, ID_L6},
    {ID_M1, ID_M2, ID_M3, ID_M4, ID_M5, ID_M6},
    {ID_N1, ID_N2, ID_N3, ID_N4, ID_N5, ID_N6}
};

/* ******************** READ ENCODER **************************************** */
void Sens_ENC_Read(uint8_t edge) {
    I2C1_MESSAGE_STATUS status;
    static I2C1_TRANSACTION_REQUEST_BLOCK TRB[3][2];
    static uint8_t writeBuffer[3], readBuffer[3][2], *pWrite[3], *pRead[3];
    uint8_t timeOut = 0, slaveTimeOut = 0;
    
    uint16_t angleINT = 0;
    float angleFLT = 0;
    
    // this initial value is important
    status = I2C1_MESSAGE_PENDING;

    // fill write buffer with register and declare pointers
    writeBuffer[edge] = AS5048B_Reg_AngleMSB;
    pWrite[edge] = &writeBuffer[edge];
    pRead[edge] = readBuffer[edge];
        
    // build TRB for writing and reading
    I2C1_MasterWriteTRBBuild(&TRB[edge][0], pWrite[edge], 1, AS5048B_Address | edge);
    I2C1_MasterReadTRBBuild(&TRB[edge][1], pRead[edge], 2, AS5048B_Address | edge);

    while(status != I2C1_MESSAGE_FAIL) {
        // now send the transactions
        I2C1_MasterTRBInsert(2, TRB[edge], &status);

        // wait for the message to be sent or status has changed.
        while(status == I2C1_MESSAGE_PENDING) {
            __delay_us(5); // add some delay here
            // timeout checking
            if (slaveTimeOut >= SLAVE_I2C_ENCODER_DEVICE_TIMEOUT){
                slaveTimeOut = 0;
                break;
            } else slaveTimeOut++;
        }

        if (status == I2C1_MESSAGE_COMPLETE) break;

        // check for max retry and skip this byte
        if (timeOut >= SLAVE_I2C_ENCODER_RETRY_MAX) break;
        else timeOut++;
    }
    
    if ((status != I2C1_MESSAGE_COMPLETE) && Flg_EdgeAct[edge])
        Mnge_ERR_ActivateStop(edge, ERR_I2CAngleFailed);
    
    // combine 14 bit result
    angleINT = ((((uint16_t) readBuffer[edge][0]) << 6) & 0x3FC0) | ((uint16_t)(readBuffer[edge][1] & 0x003F));
//    angleINT[edge] += (uint16_t)(readBuffer[edge][1] & 0x003F);
    angleINT = clamp_ui16(angleINT, 0, AS5048B_Res);
    
    // convert to float
    angleFLT = (float) (0x3FFF - angleINT);
    angleFLT = angleFLT * AS5048B_360ResInverse - 180.0f;
    angleFLT = clamp_f(angleFLT, -180.0f, 180.0f);

//    Sens_ENC_UpdateOld(edge); // update old value
    Sens_ENC_UpdateNew(edge, angleFLT); // rolling average
    if (!Flg_KalInit) Sens_ENC_KalmanInit();
    Sens_ENC_UpdateKalman(edge);
}

/* ******************** GET CURRENT ANGLE *********************************** */
float Sens_ENC_Get(uint8_t edge, bool WithLiveOffset) {
    if (WithLiveOffset)
        return (float) ENC_Data[edge] - ENC_LiveOffset[edge] - ENC_GlobOffset[edge];
    else
        return (float) ENC_Data[edge] - ENC_GlobOffset[edge];
}

/* ******************** GET ANGLE DELTA ************************************* */
float Sens_ENC_GetDelta(uint8_t edge) {
    return (ENC_Data[edge] - ENC_DataOld[edge]);
}

/* ******************** UPDATE ANGLE OFFSET BY NEIGHBOUR ******************** */
void Sens_ENC_SetGlobalOffset(uint8_t edge){
    uint8_t *neighbours;
    uint8_t i;
    neighbours = Coms_123_GetNeighbourIDs();
    for (i = 0; i < NUM_MODS; i++){
        if (!memcmp(neighbours+edge*7, ModuleIDs[i], 6)){
            ENC_GlobOffset[edge] = (float)(ModuleOffsetData[edge][i*3+(*(neighbours+6+7*edge)-48)]) * 0.1f;
            // 48 is ascii protection against 0 in Coms_123
            break;
        }
    }
}

void Sens_ENC_SetLiveOffset(uint8_t edge, uint16_t nbrAngVal) {
    ENC_LiveOffset[edge] = (((float)Acts_ROT_GetAngle(edge, false)) 
            - ((float)nbrAngVal)) * 0.05f; // *0.5 to divide by two, *0.1 to translate to degrees
    
    // if angle offset between neightbours bigger than 10 deg,
    // scale output PWM down (zero at 30 deg diff)
    if (fabsf(ENC_LiveOffset[edge]) > 5.0f){ 
        ENC_LrgOffsetMult[edge] = 1.0f - (fabsf(ENC_LiveOffset[edge]) - 5.0f) / 10.0f;
        ENC_LrgOffsetMult[edge] = clamp_f(ENC_LrgOffsetMult[edge], 0.0f, 1.0f);
    } else ENC_LrgOffsetMult[edge] = 1.0f;
}

float Sens_ENC_GetLiveOffset(uint8_t edge) {
    return ENC_LiveOffset[edge];
}

void Sens_ENC_UpdateOld(uint8_t edge){
    ENC_DataAvg[edge][4] = ENC_DataAvg[edge][3];
    ENC_DataAvg[edge][3] = ENC_DataAvg[edge][2];
    ENC_DataAvg[edge][2] = ENC_DataAvg[edge][1];
    ENC_DataAvg[edge][1] = ENC_DataAvg[edge][0];
}

void Sens_ENC_UpdateNew(uint8_t edge, float value){
    ENC_DataOld[edge] = ENC_Data[edge];
    ENC_Data[edge] = value;
    
//    ENC_DataAvg[edge][0] = value;
//    ENC_Data[edge] = (ENC_DataAvg[edge][4] + ENC_DataAvg[edge][3]
//            + ENC_DataAvg[edge][2] + ENC_DataAvg[edge][1]
//            + ENC_DataAvg[edge][0]) * 0.2f;
}

float Sens_ENC_GetLrgOffsetMult(uint8_t edge){
    return ENC_LrgOffsetMult[edge];
}


void Sens_ENC_KalmanInit(void) {
    uint8_t i;
    for (i=0; i<3; i++){
        ENC_State_Pos[i] = Sens_ENC_Get(i, false);
        ENC_State_Vel[i] = 0.0f;
    }
    Flg_KalInit = true;
}


float Sens_ENC_GetKalmanPos(uint8_t edge, bool WithLiveOffset) {
    if (WithLiveOffset)
        return (float) ENC_State_Pos[edge] - ENC_LiveOffset[edge] - ENC_GlobOffset[edge] + 180.0f;
    else
        return (float) ENC_State_Pos[edge] - ENC_GlobOffset[edge] + 180.0f;
}

/* ******************** GET ANGLE DELTA ************************************* */
volatile float Sens_ENC_GetKalmanVel(uint8_t edge) {
    return ENC_State_Vel[edge];
}


void Sens_ENC_UpdateKalmanNoise(float Q1, float Q2, float R) {
    ENC_Noise[0] = Q1;
    ENC_Noise[1] = Q2;
    ENC_Noise[2] = R;
}

// Kalman currently only works for Velocity Control (due to velocity used in prediction)
// We might have to add a check to see if it managed to initialize first
//ENC_Data is our measurement
//ENC_State_Pos is the position state estimate
//ENC_State_Vel is the velocity state estimate
//ENC_State_Cov is our covariance matrix
//Kal_dt is our timestep (0.01f)
void Sens_ENC_UpdateKalman(uint8_t edge) {
    //Kalman covariance matrix: ENC_State_Cov[edge][1, 2, 3, 4] = [[1, 2], [3, 4]]
//    static float ENC_State_Cov[3][4] = { 
//        {Kal_Cov_Init, Kal_Cov_Init, Kal_Cov_Init, Kal_Cov_Init},
//        {Kal_Cov_Init, Kal_Cov_Init, Kal_Cov_Init, Kal_Cov_Init},
//        {Kal_Cov_Init, Kal_Cov_Init, Kal_Cov_Init, Kal_Cov_Init}
//        };    
//    
//    //State estimates
    float pos_est = ENC_State_Pos[edge] + Kal_dt * ENC_State_Vel[edge];
    float vel_est = ENC_State_Vel[edge];
//    
//    //Covariance prediction
//    float P1 =   ENC_State_Cov[edge][0]
//                + ENC_State_Cov[edge][1] * Kal_dt
//                + ENC_State_Cov[edge][2] * Kal_dt
//                + ENC_State_Cov[edge][3] * Kal_dt * Kal_dt
//                + ENC_Noise[0];
//    float P2 = ENC_State_Cov[edge][1]
//                + ENC_State_Cov[edge][3] * Kal_dt;
//    float P3 = ENC_State_Cov[edge][2]
//                + ENC_State_Cov[edge][3] * Kal_dt;
//    
//    float P4 = ENC_State_Cov[edge][3] + ENC_Noise[1];
//    
//    //Kalman gain
//    float K_gain = P1 / (P1 + ENC_Noise[2]);
//    
//    //Kalman update
//    ENC_State_Cov[edge][0] = P1 * (1 - K_gain);
//    ENC_State_Cov[edge][1] = P2 * (1 - K_gain);
//    ENC_State_Cov[edge][2] = P3 * (1 - K_gain * Kal_dt);
//    ENC_State_Cov[edge][3] = P4 * (1 - K_gain * Kal_dt);
    
    const float K_gain = 0.1;
    ENC_State_Pos[edge] = pos_est + K_gain * (Sens_ENC_Get(edge, false) - pos_est);
    ENC_State_Vel[edge] = vel_est + K_gain * (Sens_ENC_GetDelta(edge) - vel_est);                
}