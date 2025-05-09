
/**
  TMR1 Generated Driver API Source File 

  @Company
    Microchip Technology Inc.

  @File Name
    tmr1.c

  @Summary
    This is the generated source file for the TMR1 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides APIs for driver for TMR1. 
    Generation Information : 
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.169.0
        Device            :  dsPIC33EP512GM604
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.50
        MPLAB             :  MPLAB X v5.40
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/

#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <libpic30.h>
#include "adc1.h"
#include "tmr1.h"
#include "../Defs_GLB.h"
#include "../Util_TIM.h"
#include "../Acts_ROT.h"
#include "../Sens_ENC.h"
#include "../Mnge_PWM.h"
#include "../Sens_ACC.h"
#include "../Sens_MOD.h"
#include "../Sens_GRD.h"
#include "../Mnge_BTN.h"
#include "../Mnge_DAC.h"


/**
 Section: File specific functions
*/
void (*TMR1_InterruptHandler)(void) = NULL;
void TMR1_CallBack(void);

/**
  Section: Data Type Definitions
*/

/** TMR Driver Hardware Instance Object

  @Summary
    Defines the object required for the maintenance of the hardware instance.

  @Description
    This defines the object required for the maintenance of the hardware
    instance. This object exists once per hardware instance of the peripheral.

  Remarks:
    None.
*/

typedef struct _TMR_OBJ_STRUCT
{
    /* Timer Elapsed */
    volatile bool           timerElapsed;
    /*Software Counter value*/
    volatile uint8_t        count;

} TMR_OBJ;

static TMR_OBJ tmr1_obj;

/**
  Section: Driver Interface
*/

void TMR1_Initialize (void)
{
    //TMR1 0; 
    TMR1 = 0x00;
    //Period = 0.01 s; Frequency = 36864000 Hz; PR1 1439; 
    PR1 = 0x59F;
    //TCKPS 1:256; TON enabled; TSIDL disabled; TCS FOSC/2; TSYNC disabled; TGATE disabled; 
    T1CON = 0x8030;

    if(TMR1_InterruptHandler == NULL)
    {
        TMR1_SetInterruptHandler(&TMR1_CallBack);
    }
    
    IFS0bits.T1IF = false;
    IEC0bits.T1IE = true;
	
    tmr1_obj.timerElapsed = false;

}


void __attribute__ ( ( interrupt, no_auto_psv ) ) _T1Interrupt (  )
{
    /* Check if the Timer Interrupt/Status is set */

    //***User Area Begin

    // ticker function call;
    // ticker is 1 -> Callback function gets called everytime this ISR executes
    if(TMR1_InterruptHandler) 
    { 
           TMR1_InterruptHandler(); 
    }

    //***User Area End

    tmr1_obj.count++;
    tmr1_obj.timerElapsed = true;
    IFS0bits.T1IF = false;
}

void TMR1_Period16BitSet( uint16_t value )
{
    /* Update the counter values */
    PR1 = value;
    /* Reset the status information */
    tmr1_obj.timerElapsed = false;
}

uint16_t TMR1_Period16BitGet( void )
{
    return( PR1 );
}

void TMR1_Counter16BitSet ( uint16_t value )
{
    /* Update the counter values */
    TMR1 = value;
    /* Reset the status information */
    tmr1_obj.timerElapsed = false;
}

uint16_t TMR1_Counter16BitGet( void )
{
    return( TMR1 );
}


void __attribute__ ((weak)) TMR1_CallBack(void)
{
    // Add your custom callback code here
    static uint8_t k = 0;
    static bool light = false;
    k++;
    if (k >= 10){
        k = 0;
        LED_Y = light;
        light = !light;
    }
    
    for (uint8_t edge = 0; edge < 3; edge++) {
        Sens_ENC_Read(edge); // always update encoder reading
        if(!FLG_Disable_Hall_sensor)
//            Sens_MOD_update_local_angle(edge, (Sens_ENC_GetKalmanPos(edge, false)));     
            Sens_MOD_update_local_angle(edge, (Sens_ENC_Get(edge, false)+180.f));     
    }

    Sens_ACC_Read();    
    if(!FLG_Disable_ACC_sensor)
        Sens_GRD_add_measurement(Sens_ACC_Get_Low(0), Sens_ACC_Get_Low(1), Sens_ACC_Get_Low(2), Util_TIM_get_time());
    Util_TIM_increment_timers();

    
    for (uint8_t edge = 0; edge < 3; edge++) { // angle control loops 
        if (Flg_EdgeWig[edge]) // wiggle is always ok, check first
            Acts_ROT_Wiggle(edge);
        else if (Flg_EdgeReq_Ang[edge] && Flg_EdgeAct[edge] && !Flg_VelCtrl && !Flg_SensFus)
            Acts_ROT_PID(edge, Sens_ENC_Get(edge, true), Acts_ROT_GetTarget(edge));
        else if (Flg_EdgeReq_Ang[edge] && Flg_EdgeAct[edge] && !Flg_VelCtrl && Flg_SensFus)
            Acts_ROT_PID(edge, Sens_MOD_GetAngle(edge, true), Acts_ROT_GetTarget(edge));        
        else if (Flg_EdgeAct[edge] && Flg_VelCtrl)
            Acts_ROT_VEL(edge);
        else
            if (!MODE_LED_PARTY && !Flg_Drive[edge])
                Acts_ROT_Out(edge, 0);// make sure motors are off
    }
    
    // Manage remaining i2c communication (must be tmr1)
    if (Flg_i2c_PWM){
        Mnge_PWM_Write();
        Flg_i2c_PWM = false;
    }
//    if (Flg_i2c_ACC && MODE_ACC_CON){
//        Flg_i2c_ACC = false;
//    }
    if (Flg_i2c_DAC){
        Mnge_DAC_Ctrl(); 
        Flg_i2c_DAC = false;
    }
    ESP_DataLog_Time_Elapsed++;
    Flg_Tmr1 = true;
}

void  TMR1_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC0bits.T1IE = false;
    TMR1_InterruptHandler = InterruptHandler; 
    IEC0bits.T1IE = true;
}

void TMR1_Start( void )
{
    /* Reset the status information */
    tmr1_obj.timerElapsed = false;

    /*Enable the interrupt*/
    IEC0bits.T1IE = true;

    /* Start the Timer */
    T1CONbits.TON = 1;
}

void TMR1_Stop( void )
{
    /* Stop the Timer */
    T1CONbits.TON = false;

    /*Disable the interrupt*/
    IEC0bits.T1IE = false;
}

bool TMR1_GetElapsedThenClear(void)
{
    bool status;
    
    status = tmr1_obj.timerElapsed;

    if(status == true)
    {
        tmr1_obj.timerElapsed = false;
    }
    return status;
}

int TMR1_SoftwareCounterGet(void)
{
    return tmr1_obj.count;
}

void TMR1_SoftwareCounterClear(void)
{
    tmr1_obj.count = 0; 
}

/**
 End of File
*/
