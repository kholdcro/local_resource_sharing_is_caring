#include "Mnge_BTN.h"
#include "Defs_GLB.h"
#include "mcc_generated_files/uart4.h"
#include "Coms_ESP.h"
#include "Acts_ROT.h"
#include "Acts_CPL.h"
#include "Mnge_PWM.h"
#include "Mnge_ERR.h"

//volatile int k = 0;
void Button_Eval() {
    Mnge_ERR_ActivateStop(0, ERR_ButtonPushed);
    Flg_Button = false;
//    k = k + 1;
//    switch (k) {
//        case 1:
//            Coms_ESP_Drive(255, -127, 0, 1);
//            break;
//        case 2:
//            Coms_ESP_Drive(255, 0, 0, 1);
//            break;
//        case 3:
//            Coms_ESP_Drive(255, 127, 0, 1);
//            break;
//        case 4:
//            Coms_ESP_Drive(0, 0, 0, 1);
//            Flg_EdgeDemo = true;
//            break;
//        default:
//            Coms_ESP_Drive(0, 0, 0, 1);
//            Flg_EdgeDemo = false;
//            k = 0;
//            break;
//    }
}

//int Button_ReturnState() {
////    return k;
//}