#include <string.h>
#include "Sens_ACC.h"
#include "Defs_GLB.h"
#include "Mnge_RGB.h"
#include "Mnge_ERR.h"
#include "math.h"
#include "dsp.h"
#include "Util_MTH.h"

// Accelerometer MMA8452Q
volatile int16_t ACC_Data[3] = {0, 0, 0};
#define INV_PI_x1800 572.957795f

void Sens_ACC_Setup(void) {
    uint8_t MMAinitReg1[2] = {MMA8452Q_CTRL_REG1_ADDR, MMA8452Q_CTRL_REG1_STBY};
    uint8_t MMAinitReg2[2] = {MMA8452Q_CTRL_REG1_ADDR, MMA8452Q_CTRL_REG1_ACTV};
    uint8_t MMAinitReg3[2] = {MMA8452Q_XYZ_DATA_CFG, MMA8452Q_CTRL_REG2_RNGE};
    uint8_t MMAinitReg4[2] = {MMA8452Q_CTRL_REG2_ADDR, MMA8452Q_CTRL_REG2_ACTV};
    
    static I2C1_MESSAGE_STATUS status;
//    static I2C1_TRANSACTION_REQUEST_BLOCK TRB[3];
    static I2C1_TRANSACTION_REQUEST_BLOCK TRB[4];
    static uint8_t *pWrite, writeBuffer[2], nCount, iCount;
    uint8_t timeOut = 0, slaveTimeOut = 0;

    // this initial value is important
    status = I2C1_MESSAGE_PENDING;

    // fill write buffer with first set of instructions
    nCount = 2;
    pWrite = MMAinitReg1;
    for (iCount = 0; iCount < nCount; iCount++) writeBuffer[iCount] = *pWrite++;
    // build first TRB for writing
    I2C1_MasterWriteTRBBuild(&TRB[0], pWrite, nCount, MMA8452Q_Address);

    // fill write buffer with second set of instructions
    nCount = 2;
    pWrite = MMAinitReg2;
    for (iCount = 0; iCount < nCount; iCount++) writeBuffer[iCount] = *pWrite++;
    // build second TRB for writing
    I2C1_MasterWriteTRBBuild(&TRB[1], pWrite, nCount, MMA8452Q_Address);

    // fill write buffer with second set of instructions
    nCount = 2;
    pWrite = MMAinitReg3;
    for (iCount = 0; iCount < nCount; iCount++) writeBuffer[iCount] = *pWrite++;
    // build second TRB for writing
    I2C1_MasterWriteTRBBuild(&TRB[2], pWrite, nCount, MMA8452Q_Address);

    // fill write buffer with second set of instructions
    nCount = 2;
    pWrite = MMAinitReg4;
    for (iCount = 0; iCount < nCount; iCount++) writeBuffer[iCount] = *pWrite++;
    // build second TRB for writing
    I2C1_MasterWriteTRBBuild(&TRB[3], pWrite, nCount, MMA8452Q_Address);
    
    while (status != I2C1_MESSAGE_FAIL) {
        // now send the transactions
//        I2C1_MasterTRBInsert(3, TRB, &status);
        I2C1_MasterTRBInsert(4, TRB, &status);

        // wait for the message to be sent or status has changed.
        while (status == I2C1_MESSAGE_PENDING) {
            __delay_us(5); // add some delay here
            // timeout checking
            if (slaveTimeOut >= SLAVE_I2C_GENERIC_DEVICE_TIMEOUT){
                slaveTimeOut = 0;
                break;
            } else slaveTimeOut++;
        }

        if (status == I2C1_MESSAGE_COMPLETE) break;

        // check for max retry and skip this byte
        if (timeOut >= SLAVE_I2C_GENERIC_RETRY_MAX) break;
        else timeOut++;
    }
    
    if (status != I2C1_MESSAGE_COMPLETE)
        Mnge_ERR_ActivateStop(0, ERR_I2CAccelerometerFailed);
}

void Sens_ACC_Read(void) {
    static I2C1_MESSAGE_STATUS status;
    static I2C1_TRANSACTION_REQUEST_BLOCK TRB[2];
    static uint8_t writeBuffer, readBuffer[6], *pWrite, *pRead;
    uint8_t timeOut = 0, slaveTimeOut = 0;

    // this initial value is important
    status = I2C1_MESSAGE_PENDING;

    // fill write buffer with register and declare pointers
    writeBuffer = MMA8452Q_OUT_X_MSB_ADDR;
    pWrite = &writeBuffer;
    pRead = readBuffer;

    // build TRB for writing and reading
    I2C1_MasterWriteTRBBuild(&TRB[0], pWrite, 1, MMA8452Q_Address);
    I2C1_MasterReadTRBBuild(&TRB[1], pRead, 6, MMA8452Q_Address);

    while (status != I2C1_MESSAGE_FAIL) {
        // now send the transactions
        I2C1_MasterTRBInsert(2, TRB, &status);

        // wait for the message to be sent or status has changed.
        while (status == I2C1_MESSAGE_PENDING) {
            __delay_us(5); // add some delay here
            // timeout checking
            if (slaveTimeOut >= SLAVE_I2C_GENERIC_DEVICE_TIMEOUT){
                slaveTimeOut = 0;
                break;
            } else slaveTimeOut++;
        }

        if (status == I2C1_MESSAGE_COMPLETE) break;

        // check for max retry and skip this byte
        if (timeOut >= SLAVE_I2C_GENERIC_RETRY_MAX) break;
        else timeOut++;
    }
    
    if (status != I2C1_MESSAGE_COMPLETE)
        Mnge_ERR_ActivateStop(0, ERR_I2CAccelerometerFailed);

    uint8_t i;
    for (i = 0; i < 5; i += 2) {
        ACC_Data[i / 2] = ((readBuffer[i] * 256) + readBuffer[i + 1]) / 16;
    }
}

// acceleration values between -2047 and 2047 (2g)
uint16_t Sens_ACC_GetRaw(uint8_t axis) { //axis 0 corresponds to x, 1 to y, 2 to z
    return (uint16_t)(ACC_Data[axis] + 2047);
}

float Sens_ACC_Get_Low(uint8_t axis) {
    return ACC_Data[axis];
}

uint16_t Sens_ACC_GetAngle(uint8_t angle){ 
    const float accX = (float)ACC_Data[0];
    const float accY = (float)ACC_Data[1];
    const float accZ = (float)ACC_Data[2];
         
    // 0 = alpha (about x), 1 = beta (about y), 2 = gamma (about z); right hand rule
    if (angle == 0){
        const float sqrt_denom = fast_sqrt(accX*accX + accZ*accZ);
        return (uint16_t) ((int16_t)(atan2_approximation1(accY, sqrt_denom)*INV_PI_x1800) + 1800);
    } else if (angle == 1) {
        const float sqrt_denom = fast_sqrt(accY*accY + accZ*accZ);
        return (uint16_t) ((int16_t)-(atan2_approximation1(accX, sqrt_denom)*INV_PI_x1800) + 1800);
    } else if (angle == 2) {
        return (uint16_t) ((int16_t)-(atan2_approximation1(-accY, accZ)*INV_PI_x1800) + 1800);
    } else {
        return 999;
    }
}
