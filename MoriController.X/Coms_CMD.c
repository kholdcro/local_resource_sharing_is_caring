#include "Defs_GLB.h"
#include "Coms_CMD.h"
#include "Coms_ESP.h"
#include "Coms_REL.h"
#include "Acts_LIN.h"
#include "Acts_CPL.h"
#include "Acts_ROT.h"
#include "Mnge_RGB.h"
#include "Sens_ENC.h"
#include "Sens_MOD.h"
#include "Coms_123.h"
#include "mcc_generated_files/reset.h"
#include <stdbool.h>
#include "math.h"



uint8_t CmdExtTemp[4][3]; // incoming extension value (temporary)
uint16_t CmdAngTemp[4][3]; // incoming angle value (temporary)
uint8_t CmdIDnTemp = 0;

uint8_t Coms_CMD_max_dist_acc_relay = 0; //Min value for relaying

/* ******************** COMMAND HANDLER************************************** */
bool Coms_CMD_Handle(uint8_t edge, uint8_t byte) {
    static bool alloc[4] = {true, true, true, true};
    static uint8_t state[4];

    if (alloc[edge]) {
        state[edge] = (byte & 0b00011111);
        alloc[edge] = false;
//        Coms_ESP_Verbose_One_Byte(state[edge]);
        return false;
    }

    switch (state[edge]) {
        case 0:
            if (Coms_CMD_Verbose(byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;
            
        case 1:
            if (Coms_CMD_SetVelocity(byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;

        case 2:
            if (Coms_CMD_SetKalmanNoise(byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;


        case 3:
            if (Coms_CMD_GetNeigbourAccel(edge, byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;       

        case 4:
            if (Coms_CMD_PermIdle_PIC(byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;     

        case 5:
            if (Coms_CMD_GetNeigbourAngle(edge, byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;             

        case 8:
            if (Coms_CMD_SetAngleOkRange(byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;
            
        case 9:
            if (Coms_CMD_SetPIDGains(byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;
            
        case 10:
            if (Coms_CMD_SetSPDGain(byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;
            
        case 11:
            if (Coms_CMD_SetRotDeadband(byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;
            
        case 12:
            if (Coms_CMD_SetRotCurrentLimit(byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;

        case 13:
            if (Coms_CMD_Shape(edge, byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;

        case 15:
            if (Coms_CMD_Stop_PARTYMODE(byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;

        case 16:
            if (Coms_CMD_WiggleEdge(edge, byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;

        case 17:
            if (Coms_CMD_SetFlags(edge, byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;

        case 18:
            if(Coms_CMD_Set_PARTYMODE(byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;

        case 19:
            if(Coms_CMD_Set_ID(byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;

        case 20:
            if (Coms_CMD_Request_Edges(byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;

        case 21:
            if (Coms_CMD_Request_Angles(byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;

        case 22:
            if (Coms_CMD_Request_Orient(byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;

        case 23:
            if (Coms_CMD_Request_Neighbour(byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;

        case 24:        
            if (Coms_CMD_Request_WiFiEdge(byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;

        case 25:
            if (Coms_CMD_No_WifiEdge(byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;

        case 26:
            if (Coms_CMD_SetWiFiEdge(edge, byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;

        case 27:    //Depricated
            if (Coms_CMD_SetMotRotOn(byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;

        case 28:    //Depricated
            if (Coms_CMD_SetMotRotOff(byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;

        case 29:    //Depricated
            if (Coms_CMD_SetMotLinOn(byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;

        case 30:    //Depricated
            if (Coms_CMD_SetMotLinOff(byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;

        case 31:
            if (Coms_CMD_Restart_PIC(byte, &state[edge]))
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;

        case 50: // END BYTE NOT RECEIVED **************************************
            if ((byte == EDG_End) || (byte == ESP_End)) // wait for next end byte
                Coms_ESP_Verbose_Write("CMD_OVL");
                return Coms_CMD_Reset(&state[edge], &alloc[edge]);
            break;

        default:
            return Coms_CMD_Reset(&state[edge], &alloc[edge]);
    }
    return false;
}

bool Coms_CMD_Reset(uint8_t *state, bool *alloc) {
    *alloc = true;
    *state = 0;
    return true;
}

void Coms_CMD_OverflowError(uint8_t *state) {
    *state = 50;
}


//------------------------- Misc. -------------------------//
bool Coms_CMD_Verbose(uint8_t byte, uint8_t *state) {
    if (byte == ESP_End) {
        FLG_Verbose = !FLG_Verbose;
    } else {
        Coms_CMD_OverflowError(state);
    }
    return true;
}

bool Coms_CMD_Restart_PIC(uint8_t byte, uint8_t *state) {
    if (byte == ESP_End) {
        __asm__ volatile ("reset");
    } else {
        Coms_CMD_OverflowError(state);
    }
    return true;
}


bool Coms_CMD_PermIdle_PIC(uint8_t byte, uint8_t *state) {
    if (byte == ESP_End) {
        uint8_t e;
        for (e = 0; e < 3; e++) {
            Acts_ROT_Out(e, 0);
            Coms_123_Disconnected(e);
        }
        
        FLG_MotLin_Active = false;
        FLG_MotRot_Active = false;
        
        // switch off LED flags and turn red
        MODE_LED_ANGLE = false;
        MODE_LED_EDGES = false;
        MODE_LED_RNBOW = false;
        MODE_LED_PARTY = false;
        LED_R = LED_On; //Keep centre LED on for indiciation
        Mnge_RGB_SetAll(0,0,0);
        while(true) {
            if (Flg_i2c_PWM == false) //Managed to write all i2c commands
                __builtin_pwrsav(0);
        }
    } else {
        Coms_CMD_OverflowError(state);
    }
    return true;
}


//-------------------- Functional Commands --------------------//
bool Coms_CMD_WiggleEdge(uint8_t edge, uint8_t byte, uint8_t *state) {
    static uint8_t count = 0;
    static uint8_t side;
    if (count >= 1) {
        if (byte == ESP_End) {
            Acts_ROT_SetWiggle(side);
        } else {
            Coms_CMD_OverflowError(state);
        }
        count = 0;
        return true;
    } else {
        side = byte;
        count++;
    }
    return false;
}

//------------------------- Requests -------------------------//
bool Coms_CMD_Request_Edges(uint8_t byte, uint8_t *state) {
    if (byte == ESP_End) {
        Coms_ESP_Request_Edges();
    } else {
        Coms_CMD_OverflowError(state);
    }
    return true;
}

bool Coms_CMD_Request_Angles(uint8_t byte, uint8_t *state) {
    if (byte == ESP_End) {
        Coms_ESP_Request_Angles();
    } else {
        Coms_CMD_OverflowError(state);
    }
    return true;
}

bool Coms_CMD_Request_Orient(uint8_t byte, uint8_t *state) {
    if (byte == ESP_End) {
        Coms_ESP_Request_Orient();
    } else {
        Coms_CMD_OverflowError(state);
    }
    return true;
}

bool Coms_CMD_Request_Neighbour(uint8_t byte, uint8_t *state) {
    static uint8_t count = 0;
    static uint8_t side;
    if (count >= 1) {
        if (byte == ESP_End) {
            Coms_ESP_Request_Neighbour(side);
        } else {
            Coms_CMD_OverflowError(state);
        }
        count = 0;
        return true;
    } else {
        side = byte;
        count++;
    }
    return false;
}

bool Coms_CMD_Request_WiFiEdge(uint8_t byte, uint8_t *state) {
    if (byte == ESP_End) {
        Coms_ESP_Request_WiFiEdge();
    } else {
        Coms_CMD_OverflowError(state);
    }
    return true;
}

bool Coms_CMD_No_WifiEdge(uint8_t byte, uint8_t *state) {
    if (byte == ESP_End) {
        Coms_ESP_Request_WiFiEdge(byte);
    } else {
        Coms_CMD_OverflowError(state);
    }
    return true;
}

bool Coms_CMD_Stop_PARTYMODE(uint8_t byte, uint8_t *state)
{
    if (byte == ESP_End) {
        MODE_LED_PARTY = false; // XXX to be replaced by separate off routine
    } else {
        Coms_CMD_OverflowError(state);
    }
    return true;
}

bool Coms_CMD_Set_PARTYMODE(uint8_t byte, uint8_t *state)
{
    if (byte == ESP_End) {
        MODE_LED_PARTY = true; // XXX to be replaced by separate off routine
    } else {
        Coms_CMD_OverflowError(state);
    }
    return true;
}


//------------------------- Setters -------------------------//
bool Coms_CMD_Set_ID(uint8_t byte, uint8_t *state)
{
    static uint8_t count=0;
    static uint8_t tmpID[6];
    if (count >= 6) {
        if (byte == ESP_End) {
            uint8_t i;
            for (i = 0; i < 6; i++) {
                ESP_ID[i] = tmpID[i];
            }
            Flg_ID_check = true;
        } else {
            Coms_CMD_OverflowError(state);
        }
        count = 0;
        return true;
    } else {
        tmpID[count] = byte;
        count++;
    }
    return false;
}


bool Coms_CMD_GetNeigbourAccel(uint8_t edge, uint8_t byte, uint8_t *state) {
    static uint8_t count[3] = {0};

    static uint8_t float_cnt[3] = {0};
    static uint8_t variab_cnt[3] = {0};
    static float acc_data[3][6] = {{0.}, {0.}, {0.}};
            
    if (count[edge] == 0) {
        float_cnt[edge] = 0;
        variab_cnt[edge] = 0;
    }
    if (count[edge] < 24) {    //Receives floats; every 4 bytes updates to new pointer
        uint8_t *float_ptr = (uint8_t *)&acc_data[edge][variab_cnt[edge]];
        *(float_ptr+3-float_cnt[edge]) = byte;
//        *(float_ptr+float_cnt[edge]) = byte;
        
        float_cnt[edge]++;
        if (float_cnt[edge] > 3){
            float_cnt[edge] = 0;
            variab_cnt[edge]++;
        } 
        count[edge]++;
    }
    else {  
        if (byte == EDG_End) {              
            uint8_t i;
            bool update = true;
            for(i=0; i<6; i++) {
                update &= isnormal(acc_data[edge][i]);
            }
//            uint8_t j;
//            Coms_ESP_Verbose_One_Byte(4);
//            for(j=0;j<6;j++)
//                Coms_ESP_Verbose_Float(acc_data[edge][j]);
            if (update) {
                const struct ground_state neighbour_acc = { 
                    {acc_data[edge][0], acc_data[edge][1], acc_data[edge][2]},
                    {acc_data[edge][3], acc_data[edge][4], acc_data[edge][5]},
                    Util_TIM_get_time()
                };  
                Sens_MOD_update_neighbor_state(edge, neighbour_acc);
            }
        } else {
            count[edge] = 0;
            Coms_CMD_OverflowError(state);
        }
        count[edge] = 0;
        float_cnt[edge] = 0;
        variab_cnt[edge] = 0;
        return true;
    } 
    return false;
}


bool Coms_CMD_GetNeigbourAngle(uint8_t edge, uint8_t byte, uint8_t *state) {
    static uint8_t count[3] = {0};

    static float angle[3] = {0.};
    uint8_t *speed_ptr[3];
    speed_ptr[0] = (uint8_t *)&angle[0];
    speed_ptr[1] = (uint8_t *)&angle[1];
    speed_ptr[2] = (uint8_t *)&angle[2];

//    if (count[edge] == 0) 
        // Coms_ESP_Verbose_One_Byte(88);
    if (count[edge] < 4){
        *(speed_ptr[edge]+3-count[edge]) = byte;
        count[edge]++;
        // Coms_ESP_Verbose_One_Byte(byte);

    } else {
        if (byte == EDG_End) {
            Sens_MOD_update_neighbor_angle(edge, angle[edge]);
        } else {
            count[edge] = 0;
            Coms_CMD_OverflowError(state);
        }
        count[edge] = 0;
        return true;
    }
    return false;
}


// Depricated
bool Coms_CMD_SetMotRotOn(uint8_t byte, uint8_t *state)
{
    if (byte == ESP_End) {
        FLG_MotRot_Active = true;
    } else {
        Coms_CMD_OverflowError(state);
    }
    return true;
}

// Depricated
bool Coms_CMD_SetMotRotOff(uint8_t byte, uint8_t *state) {
    if (byte == ESP_End) {
        FLG_MotRot_Active = false;
    } else {
        Coms_CMD_OverflowError(state);
    }
    return true;
}

bool Coms_CMD_SetAngleOkRange(uint8_t byte, uint8_t *state) {
    static bool byteReceived = false;
    static uint8_t ANG_OkRange;
    if (!byteReceived) {
        ANG_OkRange = byte;
        byteReceived = true;
    } else {
        if (byte == ESP_End) {
            Acts_ROT_SetOkRange(ANG_OkRange);
            byteReceived = false;
            return true;
        } else {
            Coms_CMD_OverflowError(state);
        }
        byteReceived = false;
    }
    return false;
}

bool Coms_CMD_SetPIDGains(uint8_t byte, uint8_t *state) {
    static uint8_t count = 0;
    static uint8_t flags;
    static uint8_t storage[3] = {0, 0, 0};
    static uint8_t len;
    if (count == 0) {
        flags = byte;
        len = 0;
        len += (flags & 0b00000001);
        len += ((flags & 0b00000010) >> 1);
        len += ((flags & 0b00000100) >> 2);
    }
    else if (count < (len+1)){
        storage[count - 1] = byte;
    } else {
        if (byte == ESP_End) {
            uint8_t i=0;
            if (flags & 0b00000001){
                Acts_ROT_SetPIDGains(0, storage[i]);
                i++;
            }
            if (flags & 0b00000010) {
                Acts_ROT_SetPIDGains(1, storage[i]);
                i++;                    
            }
            if (flags & 0b00000100) {
                Acts_ROT_SetPIDGains(2, storage[i]);
                i++;                    
            }
            count = 0;
            return true;
        } else {
            Coms_CMD_OverflowError(state);
        }
    }
    count++;
    return false;    
}

bool Coms_CMD_SetSPDGain(uint8_t byte, uint8_t *state) {
    static bool byteReceived = false;
    static uint8_t SPD_Gain;
    if (!byteReceived) {
        SPD_Gain = byte;
        byteReceived = true;
    } else {
        if (byte == ESP_End) {
            Acts_ROT_SetSPDGain(SPD_Gain);
            byteReceived = false;
            return true;
        } else {
            Coms_CMD_OverflowError(state);
        }
        byteReceived = false;
    }
    return false;
}

bool Coms_CMD_SetRotDeadband(uint8_t byte, uint8_t *state) {
    static bool byteReceived = false;
    static uint8_t deadbandValue;
    if (!byteReceived) {
        deadbandValue = byte;
        byteReceived = true;
    } else {
        if (byte == ESP_End) {
            Acts_ROT_SetAngleDeadband(deadbandValue);
            byteReceived = false;
            return true;
        } else {
            Coms_CMD_OverflowError(state);
        }
        byteReceived = false;
    }
    return false;
}

bool Coms_CMD_SetRotCurrentLimit(uint8_t byte, uint8_t *state) {
    static uint8_t count = 0;
    static uint8_t flags;
    static uint8_t storage[3] = {0, 0, 0};
    static uint8_t len;
    if (count == 0) {
        flags = byte;
        len = 0;
        len += (flags & 0b00000001);
        len += ((flags & 0b00000010) >> 1);
        len += ((flags & 0b00000100) >> 2);
    }
    else if (count < (len+1)){
        storage[count - 1] = byte;
    } else {
        if (byte == ESP_End) {
            uint8_t i=0;
            if (flags & 0b00000001){
                Acts_ROT_SetCurrentLimit(2, storage[i]);
                i++;
            }
            if (flags & 0b00000010) {
                Acts_ROT_SetCurrentLimit(1, storage[i]);
                i++;                    
            }
            if (flags & 0b00000100) {
                Acts_ROT_SetCurrentLimit(0, storage[i]);
                i++;                    
            }
            count = 0;
            return true;
        } else {
            Coms_CMD_OverflowError(state);
        }
    }
    count++;
    return false;    
}


// Depricated
bool Coms_CMD_SetMotLinOn(uint8_t byte, uint8_t *state) {
    if (byte == ESP_End) {
        FLG_MotLin_Active = true;
    } else {
        Coms_CMD_OverflowError(state);
    }
    return true;
}

// Depricated
bool Coms_CMD_SetMotLinOff(uint8_t byte, uint8_t *state) {
    if (byte == ESP_End) {
        FLG_MotLin_Active = false;
    } else {
        Coms_CMD_OverflowError(state);
    }
    return true;
}

bool Coms_CMD_SetWiFiEdge(uint8_t edge, uint8_t byte, uint8_t *state) {
    static uint8_t tmp_wifi_edge[4] = {255, 255, 255, 255};
    static bool databyte = true; //Former count - but only one byte sent
    if (byte == ESP_End) {
        if (tmp_wifi_edge[edge] < 4) {
            Coms_REL_SetWiFiEdge(tmp_wifi_edge[edge]);
            if (edge != ESP_URT_NUM)
                Coms_ESP_Return_WiFi_Edge(tmp_wifi_edge[edge]);
        }
        databyte = true;
        return true;
    } else if (databyte) {
        tmp_wifi_edge[edge] = byte;
        databyte = false;
    } else {
        Coms_CMD_OverflowError(state);
        databyte = true;
        return true;
    }
    return false;
}

bool Coms_CMD_SetFlags(uint8_t edge, uint8_t byte, uint8_t *state) {
    static bool set_flg_alloc[4] = {true, true, true, true};
    static uint8_t flag[4];
    static bool flag_set[4];

    if (set_flg_alloc[edge]) {
        flag[edge] = byte & 0b01111111; //first bit determines on or off
        flag_set[edge] = ((byte >> 7) & 0x01) != 0;
        set_flg_alloc[edge] = false;
        return false;
    }

    if (byte != ESP_End) {
        Coms_CMD_OverflowError(state);
        set_flg_alloc[edge] = true;
        return true;
    }

    switch (flag[edge]) {
        case 0:
            FLG_MotRot_Active = flag_set[edge];
            if (!flag_set[edge]) {
                uint8_t i;
                for (i = 0; i < 3; i++) Flg_EdgeReq_Ang[i] = false;
            }
            break;

        case 1:
            FLG_MotLin_Active = flag_set[edge];
            if (!flag_set[edge]) {
                uint8_t i;
                for (i = 0; i < 3; i++) Flg_EdgeReq_Ext[i] = false;
            }
            break;

        case 2: // f1
            // FLG_WaitAllEdges = flag_set[edge];
            FLG_Disable_Hall_sensor = flag_set[edge];
            break;            
            
        case 3: // f2
//            MODE_LED_EDGES = flag_set[edge];
            FLG_Disable_ACC_sensor = flag_set[edge];
            break;            

        case 4: // f3
            FLG_ReducePWMatTarget = flag_set[edge];
            break;            

        case 5: // f4 //HERE
            Flg_VelCtrl = flag_set[edge];
            CMD_ID = VEL_CMDID;
            Acts_ROT_SetSPDGain(2);
//            Acts_ROT_SetPIDGains(2, 50);
            break;            

        case 6: // f5
            // Flag5 = flag_set[edge];
//            Flg_Develop = flag_set[edge];
            Flg_SensFus = flag_set[edge];
            break;

        default:
            break;
    }

    set_flg_alloc[edge] = true;
    return true;
}

//------------------------- Setters -------------------------//
bool Coms_CMD_SetVelocity(uint8_t byte, uint8_t *state)
{
    static uint8_t alloc;
    static uint8_t count=0;
    static uint8_t len=0;
    static uint8_t float_cnt;
    static uint8_t storage_loc=0;
    static float speed_tmp[3] = {0, 0, 0};
    uint8_t *speed_ptr[3];
    speed_ptr[0] = (uint8_t *)&speed_tmp[0];
    speed_ptr[1] = (uint8_t *)&speed_tmp[1];
    speed_ptr[2] = (uint8_t *)&speed_tmp[2];
    if (count == 0) {
        alloc = byte;
        len = 0;
        len += (byte & 0b00000001) * 4;
        len += ((byte & 0b00000010) >> 1) * 4;
        len += ((byte & 0b00000100) >> 2) * 4;
        storage_loc = 0;
        float_cnt = 0;
    }
    else if (count < (len+1)){
        *(speed_ptr[storage_loc]+float_cnt) = byte;
        float_cnt++;
        if (float_cnt >= 4) {
            storage_loc++;
            float_cnt = 0;
        }
    } else {
        if (byte == ESP_End) {
            uint8_t i=0;
            if (alloc & 0b00000001){
                Acts_ROT_SetVelTarget(0, speed_tmp[i]);
                i++;
            }
            if (alloc & 0b00000010) {
                Acts_ROT_SetVelTarget(1, speed_tmp[i]);
                i++;                    
            }
            if (alloc & 0b00000100) {
                Acts_ROT_SetVelTarget(2, speed_tmp[i]);
                i++;                    
            }
            count = 0;
            return true;
        } else {
            count = 0;
            Coms_CMD_OverflowError(state);
        }
    }
    count++;
    return false;    
}

bool Coms_CMD_SetKalmanNoise(uint8_t byte, uint8_t *state)
{
    static uint8_t alloc;
    static uint8_t count=0;
    static uint8_t len=0;
    static uint8_t float_cnt;
    static uint8_t storage_loc=0;
    static float noise_tmp[3] = {0, 0, 0};
    uint8_t *noise_ptr[3];
    noise_ptr[0] = (uint8_t *)&noise_tmp[0];
    noise_ptr[1] = (uint8_t *)&noise_tmp[1];
    noise_ptr[2] = (uint8_t *)&noise_tmp[2];
    if (count == 0) {
        alloc = byte;
        len = 12;
        storage_loc = 0;
        float_cnt = 0;
    }
    else if (count < (len+1)){
        *(noise_ptr[storage_loc]+float_cnt) = byte;
        float_cnt++;
        if (float_cnt >= 4) {
            storage_loc++;
            float_cnt = 0;
        }
    } else {
        if (byte == ESP_End) {
            Sens_ENC_UpdateKalmanNoise(noise_tmp[0],noise_tmp[1],noise_tmp[2]);
            count = 0;
            return true;
        } else {
            count = 0;
            Coms_CMD_OverflowError(state);
        }
    }
    count++;
    return false;    
}


//----------- Fukkin massive-ass shape command -----------//
/* EspInAloc: 
 * 0bxx000000, where xx = indicator
 * 00 = extension and angular values
 * - 0b00xxxxxx linear & rotary indicators 0,1,2 (1 = value follows in order)
 * 01 = drive input
 * - 0b01xooooo 1 = automatic (modules evaluates), 0 = manual (direct to PWM)
 * - 0b010ooxxx manual indicator followed by 3 pwm values (1 signed byte each)
 * - 0b011xxooo automatic indicator followed by reference edge 0,1,2
 * - 0b011ooxoo direction (0 = inwards, 1 = outwards)
 * - 0b011xxxxx if xxx = 111, last xx initicates drive&coupling sequence
 * 10 = coupling & led input
 * - 0b10xxxooo retract couplings 0,1,2 (if already open, interval prolonged)
 * - 0b10oooxxx rgb led values follow in order
 * 11 = parameter update
 * - 0b11xxxooo speed setting rotary motors 0,1,2 (1 = value follows in order)
 * - 0b11oooxxx tbd
 */
bool Coms_CMD_Shape(uint8_t edge, uint8_t byte, uint8_t *state) {
    static uint8_t alloc[4];
    static uint8_t EspInCase[4];
    static uint8_t EspInBits[4];
    static uint8_t EspInBits2[4];
    static uint8_t EspInByts[4];
    static uint16_t EspInLost[4]; // counting lost byte instances since start
    static uint16_t EspIn0End[4]; // counting instances of no end byte since start
    static int8_t DrivePWM[4][3]; // manual drive mode PWM values by edge
    static uint8_t DriveSpd[3];
    static uint8_t DriveCrv[3]; // automatic drive mode speed and curv
    static uint8_t RgbPWM[4][3]; // rgb led values
    static uint8_t MotRotSpd[4][3];

    switch (EspInCase[edge]) {
        case 0: // CHECK START BYTE
            EspInCase[edge] = 1;
            EspInByts[edge] = 1;
            alloc[edge] = byte;

        case 1: // INPUT ALLOCATION ********************************************
            // if xx == 00, count bits         
            if (((byte >> 6) & 0x03) == 0) { // ******** ANGLE & EXTENSION INPUT
                EspInCase[edge] = 2;
                // Brian Kernighan: http://graphics.stanford.edu ...
                //  ... /~seander/bithacks.html#CountBitsSetNaive
                uint8_t EspInAlocTmp = byte;
                for (EspInBits[edge] = 0; EspInAlocTmp; EspInBits[edge]++) {
                    EspInAlocTmp &= EspInAlocTmp - 1; // clear the LSB set
                }
                EspInAlocTmp = byte & 0b00000111; // angle input is 2 bytes
                for (EspInBits2[edge] = 0; EspInAlocTmp; EspInBits2[edge]++) {
                    EspInAlocTmp &= EspInAlocTmp - 1;
                }

            } else if (((byte >> 6) & 0x03) == 1) { // ************* DRIVE INPUT
                if (byte & 0b00100000) { // automatic drive mode
                    if ((byte & 0b00011100) == 28) { // drive&couple sequence
                        EspInCase[edge] = 22;
                    } else {
                        EspInCase[edge] = 19;
                    }
                } else { // manual drive mode
                    EspInCase[edge] = 15;
                    // only last three bits relevant when counting rec. bytes
                    uint8_t EspInAlocTmp = (byte & 0b00000111);
                    for (EspInBits[edge] = 0; EspInAlocTmp; EspInBits[edge]++) {
                        EspInAlocTmp &= EspInAlocTmp - 1; // clear LSB set
                    }
                }

            } else if (((byte >> 6) & 0x03) == 2) { // **** COUPLING & LED INPUT
                //            Coms_ESP_Verbose_Write(coms_message);
                if (byte & 0b00000111) {
                    EspInCase[edge] = 32;
                    // only last three bits relevant when counting rec. bytes
                    uint8_t EspInAlocTmp = (byte & 0b00000111);
                    for (EspInBits[edge] = 0; EspInAlocTmp; EspInBits[edge]++) {
                        EspInAlocTmp &= EspInAlocTmp - 1; // clear LSB set
                    }
                    //                Coms_ESP_Verbose_Write(coms_1);
                } else {
                    EspInCase[edge] = 35;
                    EspInBits[edge] = 0;
                    EspInBits2[edge] = 0;
                }

            } else if (((byte >> 6) & 0x03) == 3) { // ******** PARAMETER UPDATE
                if (byte & 0b00111000) { // rotary motor speed setting
                    EspInCase[edge] = 40;
                    uint8_t EspInAlocTmp = (byte & 0b00111000);
                    for (EspInBits[edge] = 0; EspInAlocTmp; EspInBits[edge]++) {
                        EspInAlocTmp &= EspInAlocTmp - 1; // clear the LSB set
                    }            
                } else {
                    EspInCase[edge] = 43;
                    EspInBits[edge] = 0;
                    EspInBits2[edge] = 0;
                }
            }
            EspInByts[edge] = EspInByts[edge] + 1;
            break;

        case 2: // EXTENSION INPUTS ********************************************
            /* cases 2 to 6, one byte per motor input */
            if (alloc[edge] & 0b00100000) {
                CmdExtTemp[edge][0] = byte;
                EspInByts[edge] = EspInByts[edge] + 1;
                EspInCase[edge] = 3;
                break;
            }
        case 3:
            if (alloc[edge] & 0b00010000) {
                CmdExtTemp[edge][1] = byte;
                EspInByts[edge] = EspInByts[edge] + 1;
                EspInCase[edge] = 4;
                break;
            }
        case 4:
            if (alloc[edge] & 0b00001000) {
                CmdExtTemp[edge][2] = byte;
                EspInByts[edge] = EspInByts[edge] + 1;
                EspInCase[edge] = 5;
                break;
            }

        case 5: // ANGLE INPUTS ************************************************
            /* cases 8 to 13, two bytes per motor input */
            if (alloc[edge] & 0b00000100) {
                CmdAngTemp[edge][0] = 0xFF00 & ((uint16_t) (byte) << 8);
                EspInByts[edge] = EspInByts[edge] + 1;
                EspInCase[edge] = 6;
                break;
            }
        case 6:
            if (alloc[edge] & 0b00000100) {
                CmdAngTemp[edge][0] = CmdAngTemp[edge][0] | (uint16_t) byte;
                EspInByts[edge] = EspInByts[edge] + 1;
                EspInCase[edge] = 7;
                break;
            }
        case 7:
            if (alloc[edge] & 0b00000010) {
                CmdAngTemp[edge][1] = 0xFF00 & ((uint16_t) (byte) << 8);
                EspInByts[edge] = EspInByts[edge] + 1;
                EspInCase[edge] = 8;
                break;
            }
        case 8:
            if (alloc[edge] & 0b00000010) {
                CmdAngTemp[edge][1] = CmdAngTemp[edge][1] | (uint16_t) byte;
                EspInByts[edge] = EspInByts[edge] + 1;
                EspInCase[edge] = 9;
                break;
            }
        case 9:
            if (alloc[edge] & 0b00000001) {
                CmdAngTemp[edge][2] = 0xFF00 & ((uint16_t) (byte) << 8);
                EspInByts[edge] = EspInByts[edge] + 1;
                EspInCase[edge] = 10;
                break;
            }
        case 10:
            if (alloc[edge] & 0b00000001) {
                CmdAngTemp[edge][2] = CmdAngTemp[edge][2] | (uint16_t) byte;
                EspInByts[edge] = EspInByts[edge] + 1;
                EspInCase[edge] = 11;
                break;
            }
        case 11:
            CmdIDnTemp = byte;
            EspInCase[edge] = 12;
            EspInByts[edge] = EspInByts[edge] + 1;
            break;

        case 12: // verify motor inputs
            if (byte == ESP_End) {
                if ((EspInByts[edge] == (3 + EspInBits[edge] + EspInBits2[edge]))
                        && (CmdIDnTemp != 0)){
                    CMD_ID = CmdIDnTemp; // update command ID
                    Coms_CMD_SetEdge(edge, alloc[edge]); // implement command
                } else {
                    EspInLost[edge] = EspInLost[edge] + 1; // data lost
                }
            } else {
                EspIn0End[edge] = EspIn0End[edge] + 1; // no end byte
            }
            EspInCase[edge] = 0;
            return true;
            break;

        case 15: // MANUAL DRIVE MODE ******************************************
            if (alloc[edge] & 0b00000100) {
                DrivePWM[edge][0] = byte;
                EspInByts[edge] = EspInByts[edge] + 1;
                EspInCase[edge] = 16;
                break;
            }
        case 16:
            if (alloc[edge] & 0b00000010) {
                DrivePWM[edge][1] = byte;
                EspInByts[edge] = EspInByts[edge] + 1;
                EspInCase[edge] = 17;
                break;
            }
        case 17:
            if (alloc[edge] & 0b00000001) {
                DrivePWM[edge][2] = byte;
                EspInByts[edge] = EspInByts[edge] + 1;
                EspInCase[edge] = 18;
                break;
            }
        case 18: // verify drive inputs
            if (byte == ESP_End) {
                if (EspInByts[edge] == (2 + EspInBits[edge])) {
                    uint8_t m;
                    for (m = 0; m <= 2; m++) {
                        if ((alloc[edge] >> (2 - m)) & 0b00000001) {
                            if (!Flg_EdgeCon[m]) {
                                Acts_ROT_Out(m, DrivePWM[edge][m]*8);
                                Acts_ROT_SetDrvInterval(m, 5);
                            }
                        }
                    }
                } else {
                    EspInLost[edge] = EspInLost[edge] + 1; // data lost
                }
            } else {
                EspIn0End[edge] = EspIn0End[edge] + 1; // no end byte
            }
            EspInCase[edge] = 0;
            return true;
            break;

        case 19: // AUTOMATIC DRIVE MODE ***************************************
            DriveSpd[edge] = byte;
            EspInByts[edge] = EspInByts[edge] + 1;
            EspInCase[edge] = 20;
            break;

        case 20:
            DriveCrv[edge] = byte;
            EspInByts[edge] = EspInByts[edge] + 1;
            EspInCase[edge] = 21;
            break;

        case 21: // verify drive inputs
            if (byte == ESP_End) {
                if (EspInByts[edge] == 4) {
                    if (!Flg_EdgeCon[0] && !Flg_EdgeCon[1] && !Flg_EdgeCon[2]) {
                        uint8_t i;
                        for (i = 0; i < 3; i++) Flg_Drive[edge] = true;
                        Acts_ROT_Drive(DriveSpd[edge], DriveCrv[edge],
                                ((alloc[edge] & 0x18) >> 3),
                                ((alloc[edge] & 0x04) >> 2));
                    }
                } else {
                    EspInLost[edge] = EspInLost[edge] + 1; // data lost
                }
            } else {
                EspIn0End[edge] = EspIn0End[edge] + 1; // no end byte
            }
            EspInCase[edge] = 0;
            return true;
            break;

        case 22:; // DRIVE AND COUPLING SEQUENCE
            uint8_t OUTedge = alloc[edge] & 0b00000011;
            if (byte == ESP_End) {
                Flg_EdgeReq_Cpl[OUTedge] = true;
                Flg_DriveAndCouple[OUTedge] = true;
            } else {
                EspIn0End[edge] = EspIn0End[edge] + 1; // no end byte
            }
            EspInCase[edge] = 0;
            return true;
            break;

        case 32: // COUPLING & LED INPUT ***************************************
            if (alloc[edge] & 0b00000100) {
                RgbPWM[edge][0] = byte;
                EspInByts[edge] = EspInByts[edge] + 1;
                EspInCase[edge] = 33;
                break;
            }
        case 33:
            if (alloc[edge] & 0b00000010) {
                RgbPWM[edge][1] = byte;
                EspInByts[edge] = EspInByts[edge] + 1;
                EspInCase[edge] = 34;
                break;
            }
        case 34:
            if (alloc[edge] & 0b00000001) {
                RgbPWM[edge][2] = byte;
                EspInByts[edge] = EspInByts[edge] + 1;
                EspInCase[edge] = 35;
                break;
            }

        case 35: // verify coupling inputs
            if (byte == ESP_End) {
                if (EspInByts[edge] == (2 + EspInBits[edge])) {
                    // set smas
                    uint8_t m;
                    for (m = 0; m <= 2; m++) {
                        if (alloc[edge] & (0b00100000 >> m)) {
                            Flg_EdgeReq_Cpl[m] = true;
                        }
                    }
                    // update leds
                    for (m = 0; m <= 2; m++) {
                        if ((alloc[edge] >> (2 - m)) & 0b00000001) {
                            Mnge_RGB_Set(m, RgbPWM[edge][m]);
                        }
                    }
                } else {
                    EspInLost[edge] = EspInLost[edge] + 1; // data lost
                }
            } else {
                EspIn0End[edge] = EspIn0End[edge] + 1; // no end byte
            }
            EspInCase[edge] = 0;
            return true;
            break;

        case 40: // ROTARY SPEED UPDATE ****************************************
            if (alloc[edge] & 0b00100000) {
                MotRotSpd[edge][0] = byte;
                EspInByts[edge] = EspInByts[edge] + 1;
                EspInCase[edge] = 41;
                break;
            }

        case 41:
            if (alloc[edge] & 0b00010000) {
                MotRotSpd[edge][1] = byte;
                EspInByts[edge] = EspInByts[edge] + 1;
                EspInCase[edge] = 42;
                break;
            }

        case 42:
            if (alloc[edge] & 0b00001000) {
                MotRotSpd[edge][2] = byte;
                EspInByts[edge] = EspInByts[edge] + 1;
                EspInCase[edge] = 43;
                break;
            }

        case 43: // verify motor speed inputs
            if (byte == ESP_End) {
                if (EspInByts[edge] == (2 + EspInBits[edge])) {
                    uint8_t m;
                    for (m = 0; m <= 2; m++) {
                        if (alloc[edge] & (0b00100000 >> m)) {
                            Acts_ROT_SetSpeedLimit(m, MotRotSpd[edge][m]);
                        }
                    }
                }
            }
            EspInCase[edge] = 0;
            return true;
            break;
          
        default:
            EspInCase[edge] = 0;
            break;
    }
    return false;
}

/* ******************** ESP COMMAND TO MOTORS ******************************* */
void Coms_CMD_SetEdge(uint8_t inEdge, uint8_t byte) {
    uint8_t j, k;

    // Set extensions
    for (j = 1; j <= 3; j++) {
        if ((byte >> (5 - (j - 1))) & 0x01) {
            Acts_LIN_SetTarget(j - 1, CmdExtTemp[inEdge][j - 1]);
        }
    }

    // Set angles
    for (k = 1; k <= 3; k++) {
        if (((byte >> (2 - (k - 1))) & 0x01) && (Flg_EdgeCon[k - 1])) {
            Acts_ROT_SetTarget(k - 1, CmdAngTemp[inEdge][k - 1]);
        }
    }
}


void Coms_123_Set_Sens_FUS_relay_distance(uint8_t dist) {
    Coms_CMD_max_dist_acc_relay = dist;
}