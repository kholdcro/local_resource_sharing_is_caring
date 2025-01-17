#ifndef COMS_CMD_H
#define	COMS_CMD_H

bool Coms_CMD_Handle(uint8_t, uint8_t);
bool Coms_CMD_Reset(uint8_t*, bool*);
bool Coms_CMD_Verbose(uint8_t, uint8_t*);
bool Coms_CMD_WiggleEdge(uint8_t, uint8_t, uint8_t*);
bool Coms_CMD_Shape(uint8_t, uint8_t, uint8_t*);
bool Coms_CMD_SetVelocity(uint8_t, uint8_t*);
bool Coms_CMD_SetKalmanNoise(uint8_t, uint8_t*);
bool Coms_CMD_GetNeigbourAccel(uint8_t, uint8_t, uint8_t*);
bool Coms_CMD_GetNeigbourAngle(uint8_t, uint8_t, uint8_t*);
void Coms_CMD_SetEdge(uint8_t, uint8_t);
bool Coms_CMD_SetWiFiEdge(uint8_t, uint8_t, uint8_t*);
void Coms_CMD_OverflowError(uint8_t*);

bool Coms_CMD_Request_Edges(uint8_t, uint8_t*);
bool Coms_CMD_Request_Angles(uint8_t, uint8_t*);      
bool Coms_CMD_Request_Orient(uint8_t, uint8_t*);
bool Coms_CMD_Request_Neighbour(uint8_t, uint8_t*);
bool Coms_CMD_Request_WiFiEdge(uint8_t, uint8_t*);
bool Coms_CMD_No_WifiEdge(uint8_t, uint8_t*);
bool Coms_CMD_Stop_PARTYMODE(uint8_t, uint8_t*);
bool Coms_CMD_Set_PARTYMODE(uint8_t, uint8_t*);

bool Coms_CMD_Set_ID(uint8_t, uint8_t*);
bool Coms_CMD_SetMotRotOn(uint8_t, uint8_t*);
bool Coms_CMD_SetMotRotOff(uint8_t, uint8_t*);
bool Coms_CMD_SetRotCurrentLimit(uint8_t, uint8_t*);
bool Coms_CMD_SetAngleOkRange(uint8_t, uint8_t *);
bool Coms_CMD_SetPIDGains(uint8_t, uint8_t *);
bool Coms_CMD_SetSPDGain(uint8_t, uint8_t *);
bool Coms_CMD_SetRotDeadband(uint8_t, uint8_t*);
bool Coms_CMD_SetMotLinOn(uint8_t, uint8_t*);
bool Coms_CMD_SetMotLinOff(uint8_t, uint8_t*);
bool Coms_CMD_SetFlags(uint8_t, uint8_t, uint8_t*);
bool Coms_CMD_Restart_PIC(uint8_t, uint8_t*);
bool Coms_CMD_PermIdle_PIC(uint8_t, uint8_t*);

void Coms_123_Set_Sens_FUS_relay_distance(uint8_t);

#endif	/* COMS_CMD_H */

