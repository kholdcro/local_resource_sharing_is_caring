/* ************************************************************************** */
/* ************************************************************************** */
/* **********   File:    Defines.h                                 ********** */
/* **********   Author:  Christoph H. Belke                        ********** */
/* **********   Date:    09.12.2020                                ********** */
/* ************************************************************************** */
/* ************************************************************************** */

#ifndef DEFS_GLB_H
#define	DEFS_GLB_H

#define FCY 36864000UL               // cycle frequency

#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <libpic30.h>


/* ******************** MODULE ********************************************** */
#define MODULE 'H' // module name by letter


/* ******************** NOTES *********************************************** */
// Timer info
/* Timer 1: 100 Hz - angle feedback
 * Timer 3: 20 Hz - extension feedback, coupling controller
 * Timer 5: 5 Hz - updating LEDs 
 * TMRx_f NOT used to define frequency */
#define TMR1_f 100
#define TMR3_f 20
#define TMR5_f 5
#define epsilon 1e-10f

// I2C MCC modification // not updated since fosc boost
/* I2C1BRG changed from MCC calculated 0x08 to 0x07, as FRM calculation
 * is as follows: ((1/0.4 - 0.120)*3.6864)-2 = 6.77
 * FRM: http://ww1.microchip.com/downloads/en/DeviceDoc/70000195g.pdf*/


/* ******************** MODE SELECTION ************************************** */
//#define MODE_DEBUG false
//#define MODE_ENC_CON true
#define MODE_ACC_CON true

#define MODE_MotLin_Active true
#define MODE_MotRot_Active true
#define MODE_Cplngs_Active true

// RGB LED Default values
#define RGB_Default_Red 7
#define RGB_Default_Green 1
#define RGB_Default_Blue 12

/* ******************** ERROR CODES ***************************************** */
#define ERR_NeighbourLost 1
#define ERR_NeighbourToldMe 2
#define ERR_NeighbourConAfterAct 3
#define ERR_NeighbourAckAfterAct 4
#define ERR_I2CAngleFailed 20
#define ERR_I2CLedDriverFailed 21
#define ERR_I2CAccelerometerFailed 22
#define ERR_ESPToldMe 90
#define ERR_ButtonPushed 32

/* ******************** BATTERY ********************************************* */
#define BatCountMax 10 // seconds of continuos low bat before flag is triggered

/* ******************** LIVE MODE VARS ************************************** */
extern volatile bool MODE_LED_ANGLE;
extern volatile bool MODE_LED_EDGES;
extern volatile bool MODE_LED_RNBOW;
extern volatile bool MODE_LED_PARTY;

/* ********************  FLAGS ********************************************** */
// FLG can be set externally, Flg is only used by program
extern volatile bool FLG_WaitAllEdges;
extern volatile bool FLG_Disable_Hall_sensor;
extern volatile bool FLG_Disable_ACC_sensor;
extern volatile bool FLG_MotLin_Active;
extern volatile bool FLG_MotRot_Active;
extern volatile bool FLG_Verbose;
extern volatile bool FLG_Emergency;
extern volatile bool FLG_ReducePWMatTarget;

extern volatile bool Flg_BatLow;
extern volatile bool Flg_Button;
extern volatile bool Flg_Develop;
extern volatile bool Flg_SensFus;

extern volatile bool Flg_Drive[3];
extern volatile bool Flg_DriveAndCouple[3];

extern volatile bool Flg_EdgeCon[3];
extern volatile bool Flg_EdgeSyn[3];
extern volatile bool Flg_EdgeAct[3];
extern volatile bool Flg_EdgeWig[3];
extern volatile bool Flg_EdgeReq_Ang[3];
extern volatile bool Flg_EdgeReq_Ext[3];
extern volatile bool Flg_EdgeReq_Cpl[3];
extern volatile bool Flg_EdgeReq_CplNbrWait[3];
extern volatile bool Flg_EdgeNbr_Offset[3];
extern volatile bool Flg_AllEdgRdy[3];
extern volatile bool Flg_NbrEdgRdy[3];
extern volatile bool Flg_ByteReadOverran[3];
extern volatile bool Flg_VelCtrl;


extern volatile bool Flg_ID_check;

extern volatile bool Flg_i2c_PWM;
extern volatile bool Flg_i2c_ACC;
extern volatile bool Flg_i2c_DAC;

extern volatile bool Flg_Tmr1;
extern volatile bool Flg_Tmr3;
extern volatile bool Flg_Tmr5;

/* ******************** STATE INFO ****************************************** */
extern volatile uint8_t ESP_DataLog_Time_Elapsed;
extern volatile uint8_t ESP_ID[6];
extern volatile uint8_t CMD_ID;


/* ******************** PERIPHERALS ***************************************** */
// Output latches for LEDs
#define LED_R LATBbits.LATB0        // blue LED - 1 is off
#define LED_Y LATBbits.LATB1        // orange LED - 1 is off
#define BTN_Stat PORTAbits.RA1      // button port
#define WIFI_EN LATBbits.LATB4      // wifi enable
#define BAT_LBO PORTBbits.RB7       // low battery indicator

// On-off confusion prevention
#define LED_On 0                    // drained by pin, low V = led on
#define LED_Off 1
#define WIFI_On 1                   // high enables ESP
#define WIFI_Off 0
#define WIFI_LED_ON 1
#define WIFI_LED_OFF 0


/* ******************** ESP COMMUNICATION *********************************** */
#define ESP_End 14                  // end byte
#define ESP_Relay 0b00000101
#define ESP_URT_NUM 3
#define UART_BUFF_SIZE 1023


/* ******************** EDGE COMMUNICATION *********************************** */
#define EDG_End 42                  // end byte
#define EDG_IdlIntrvl 5             // idle check at 5Hz, 5 = 1 sec = con lost
#define EDG_ConIntrvl 10            // con check at 5Hz, 10 = 2 sec = con lost
#define EDG_ActIntrvl 5             // act check at 20Hz, 3 = 
#define EDG_ExtNbrRng 6             // current neighbour ext. must be (own +-)
#define EDG_ExtSlwRng 2             // if nbr < this > EDG_ExtNbrRng, slw down
#define EDG_ExtSlwVal 512           // slow down to this 
#define VEL_CMDID 253

/* ******************** PWM GENERATOR *************************************** */
// Duty cycle register
#define ROT_PWM_DutyReg_1 SDC5      // generator 5, secondary
#define ROT_PWM_DutyReg_2 PDC5      // generator 5, primary
#define ROT_PWM_DutyReg_3 SDC4      // generator 4, secondary
#define LIN_PWM_DutyReg_1 SDC3      // generator 3, secondary
#define LIN_PWM_DutyReg_2 SDC2      // generator 2, secondary
#define LIN_PWM_DutyReg_3 SDC1      // generator 1, secondary
#define ROT_PWM_MAXRANGE 1024
// ROT PWM full range 1024 (SPHASEx, PHASEx)
// UPDATED ROT PWM RANGE TO 180 FOR CURRENT CHOPPING FREQUENCY MATCH
// LIN PWM limited to 1024 (SPHASEx)

// Duty cycle selector
#define ROT_PWM_1 1
#define ROT_PWM_2 2
#define ROT_PWM_3 3
#define LIN_PWM_1 4
#define LIN_PWM_2 5
#define LIN_PWM_3 6

// Ensure the following are set in the PWM Register selector
//#define PWM_Perd_RotA SPHASE5
//#define PWM_Perd_RotB PHASE5
//#define PWM_Perd_RotC SPHASE4
//#define PWM_Perd_LinA SPHASE3
//#define PWM_Perd_LinB SPHASE2    
//#define PWM_Perd_LinC SPHASE1    


/* ******************** LINEAR MOTORS *************************************** */
#define LIN_DIR_1 LATBbits.LATB10
#define LIN_DIR_2 LATBbits.LATB12
#define LIN_DIR_3 LATBbits.LATB14

#define MotLin_MinInput 1           // linear motor controlled with 0.1mm input
#define MotLin_MaxInput 120         // from 0 to 12 mm -> min: 0, max:120

#define MotLin_SlowRegion 50        // slow region near min and max
#define MotLin_SlowFactor 1.2       // linear slow down factor in slow region
#define MotLin_OkRange 2            // +- (*0.1mm) (automatic CMD update)

#define MotLin_PID_erband 4         // acceptable error band ~ *0.01mm
#define MotLin_PID_stable 3         // stable time in sec
#define MotLin_PID_period 0.05f     // timer period
#define MotLin_PID_kP 162.0f        // proportional component
#define MotLin_PID_kI 101.3f        // integral component
#define MotLin_PID_kD 8.4f          // integral component
#define MotLin_PID_DMax 1024        // derivative limit
#define MotLin_PID_Imax 1024        // integral limit
#define MotLin_PID_Max 1024         // duty cycle limit


/* ******************** ROTARY MOTORS *************************************** */
#define ROT_DIR_1 LATCbits.LATC6
#define ROT_DIR_2 LATCbits.LATC7
#define ROT_DIR_3 LATAbits.LATA10

#define MotRot_AngleIntMIN 600      // minimum input range in degrees *10 (uint16_t)
#define MotRot_AngleIntMAX 3000     // maximum input range in degrees *10 (uint16_t)
#define MotRot_OkRange 10           // +- (0.1*degrees) (automatic CMD update)

#define MotRot_PID_Deadband 1.0f    // +- deadband tolerance in deg.
#define MotRot_PID_period 0.01f     // timer period
#define MotRot_PID_freq 100.0f      // timer period
#define MotRot_PID_kP 255.0f        // proportional gain (was 153 for OutMax 1024)
#define MotRot_PID_kI 95.0f         // integral gain (was 53.9 for OutMax 1024)
#define MotRot_PID_kD 6.0f          // derivative gain (was 3.4 for OutMax 1024)
#define MotRot_PID_Dmax 1800.0f     // derivative limit (was 1024)
#define MotRot_PID_Imax 1800.0f     // integral limit (was 1024)
#define MotRot_PID_Max 1800.0f      // duty cycle limit (was 1024)

//Note: speed gain used to be 150.f, but was changed for Velocity control. 
//The speed should be set anyway when using angle + position, and the gain should be set to 150.
#define MotRot_SPD_k 150.f           // speed control proportional gain
#define MotRot_SPD_Max 1024.0f      // speed duty cycle limit (Same as mot rot pwm range)
#define MotRot_SPD_OneOverMax 0.0005556f
#define MotRot_SpeedInit 100        // limit speed at start-up (/100)
#define MotRot_SpeedMax 58.3f       // max speed (degrees/second) (@tau=149)

// Maxon motor torque limit - 237 stall, 149 GPX safe, 63 backdrive safe
#define MotRot_TorqueLimit 149      // (/255)
#define MotRot_WiggleTime 15        // seconds
#define MotRot_WiggleTorque 40      // wiggle torque limit (/255)
#define MotRot_DefaultDrvInterval 5 // drv commands hold for 1 second by default
#define MotRot_DrvCplPushInterval 100 // push interval at 20Hz (< SMA_Period_2)


/* ******************** Kalman ********************************************** */
#define Kal_dt 0.01f                // Same as MotRot_PID_period
#define Kal_dt_inv 100.f            // Inverse of kal_dt
#define Kal_Cov_Init 0.02f
#define Kal_Q1 0.00001f
#define Kal_Q2 0.00001f
#define Kal_R 0.1f


/* ******************** I2C ************************************************* */
#define SLAVE_I2C_GENERIC_RETRY_MAX 3
#define SLAVE_I2C_GENERIC_DEVICE_TIMEOUT 50
#define SLAVE_I2C_ENCODER_RETRY_MAX 3
#define SLAVE_I2C_ENCODER_DEVICE_TIMEOUT 50
// IF > 255 must change timeouts to uint16_t in all i2c

/* ******************** ENCODERS AS5048B ************************************ */
#define AS5048B_Address 0x40
#define AS5048B_Reg_AngleMSB 0xFE
#define AS5048B_Res 16383
#define AS5048B_360ResInverse 0.021973f


/* ******************** ACCELEROMETER MMA8452Q ****************************** */
#define MMA8452Q_Address 0x1C //i2c address
#define MMA8452Q_CTRL_REG1_ADDR 0x2A //Ctrl reg address to be modified at Setup
#define MMA8452Q_CTRL_REG1_STBY 0x04 //Value ctrl reg must be modified at Setup
#define MMA8452Q_CTRL_REG1_ACTV 0x05 //Value ctrl reg must be modified at Setup
#define MMA8452Q_XYZ_DATA_CFG 0x0E //XYZ_DATA_CFG register (range and filter)
#define MMA8452Q_CTRL_REG2_RNGE 0x00 //2g range and high-pass filter off
#define MMA8452Q_OUT_X_MSB_ADDR 0x01 //Address of first data register to be read
#define MMA8452Q_CTRL_REG2_ADDR 0x2B //XYZ_DATA_CFG register (range and filter)
#define MMA8452Q_CTRL_REG2_ACTV 0x02 //XYZ_DATA_CFG register (range and filter)



/* ******************** LED DRIVER TLC59208 ****************************** */
#define TLC59208_ADDRESS 0x20   //Addresss pins A0-A2 tied to GND (not in DS?)
//#define TLC59208_ALL_ADDRESS 0x48 // not needed for now
#define TLC59208_CtrlReg 0xA2
#define TLC59208_MODE1Add 0x80 // address MODE1, auto increment enabled
#define TLC59208_MODE1 0x01 // respond to LED All Call, auto increment disabled
#define TLC59208_LEDOUT0Add 0x8C // address LEDOUT0, auto increment enabled
#define TLC59208_LEDOUT0 0xAA // LEDOUT0 all outputs PWM controlled
#define TLC59208_LEDOUT1Add 0x8D // address LEDOUT0, auto increment enabled
#define TLC59208_LEDOUT1 0xAA // LEDOUT0 all outputs PWM controlled

#define SMA_Period_1 140 // SMA on-time (updated in 20 Hz loop) -> 100 = 5 sec.
#define SMA_Period_2 200 // split into high I open (1) and low I stay phases (2)
#define SMA_Duty_1 130 // 150 // 8-bit PWM value for first phase
#define SMA_Duty_2 65 // 8-bit PWM value for second phase

#endif	/* DEFS_GLBH */