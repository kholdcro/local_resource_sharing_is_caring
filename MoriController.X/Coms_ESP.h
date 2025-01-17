/* 
 * File:   Coms_ESP.h
 * Author: belke
 *
 * Created on April 4, 2019, 5:34 PM
 */

#ifndef COMS_ESP_H
#define	COMS_ESP_H

#include "mcc_generated_files/system.h"

void Coms_ESP_Eval (void);
void Coms_ESP_Boot (void);
bool Coms_ESP_Handle(uint8_t);

uint8_t Coms_ESP_ReturnID (uint8_t);
void Coms_ESP_Verbose (void);

void Coms_ESP_SetLEDs (uint8_t, uint8_t);
void Coms_ESP_Verbose_Write(const char*);
void Coms_ESP_Verbose_One_Byte(uint8_t);
void Coms_ESP_Verbose_Float(float);
void Coms_ESP_SendStable(bool);
void Coms_ESP_TurnOnWifi(void);

void Coms_ESP_Request_Edges(void);
void Coms_ESP_Write_Edges(void);
void Coms_ESP_Request_Angles(void);
void Coms_ESP_Write_Angles(void);
void Coms_ESP_Request_Orient(void);
void Coms_ESP_Write_Orient(void);
void Coms_ESP_Write_Velocity(void);
void Coms_ESP_Write_AngleFlt(void);
void Coms_ESP_SendSerialOverflow(uint8_t);
void Coms_ESP_Request_Neighbour(uint8_t);
void Coms_ESP_Request_WiFiEdge();
void Coms_ESP_Neighbour_Disconnected(uint8_t);

void Coms_ESP_LED_State(uint8_t, uint8_t);
void Coms_ESP_LED_On(uint8_t, bool);
void Coms_ESP_LED_Tgl(uint8_t);
void Coms_ESP_LED_Blk(uint8_t, uint8_t);
void Coms_ESP_LED_Set_Blink_Freq(uint8_t, uint8_t);
void Coms_ESP_Interpret(void);
void Coms_ESP_No_WiFi_Edge(void);
void Coms_ESP_Return_WiFi_Edge(uint8_t);
void Coms_ESP_Request_ID(void);
bool Coms_ESP_VerifyID(void);

void Coms_ESP_StateUpdate(void);
bool Coms_ESP_SetDatalogFlags(uint8_t);
bool Coms_ESP_SetDatalogPeriod(uint8_t);
bool Coms_ESP_SetClientLetter(uint8_t);
bool Coms_ESP_SendErrorCode(uint8_t);
bool Coms_ESP_Set_Sens_FUS_dist(uint8_t);

#endif	/* COMS_ESP_H */

