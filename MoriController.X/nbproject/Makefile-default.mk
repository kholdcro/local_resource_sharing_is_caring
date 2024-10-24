#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/MoriController.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/MoriController.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS
SUB_IMAGE_ADDRESS_COMMAND=--image-address $(SUB_IMAGE_ADDRESS)
else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=mcc_generated_files/i2c1.c mcc_generated_files/pin_manager.c mcc_generated_files/interrupt_manager.c mcc_generated_files/traps.c mcc_generated_files/tmr1.c mcc_generated_files/uart4.c mcc_generated_files/adc1.c mcc_generated_files/tmr3.c mcc_generated_files/pwm.c mcc_generated_files/reset.c mcc_generated_files/clock.c mcc_generated_files/system.c mcc_generated_files/tmr5.c mcc_generated_files/ext_int.c mcc_generated_files/uart1.c mcc_generated_files/uart2.c mcc_generated_files/uart3.c mcc_generated_files/mcc.c main.c Coms_123.c Coms_ESP.c Coms_REL.c Coms_CMD.c Acts_CPL.c Acts_LIN.c Acts_ROT.c Mnge_BAT.c Mnge_BTN.c Mnge_DAC.c Mnge_PWM.c Sens_ACC.c Sens_ENC.c Mnge_RGB.c Tmrs_CBK.c Mnge_ERR.c Sens_EDG.c Sens_FAC.c Sens_GRD.c Sens_MOD.c Util_MAT.c Util_MTH.c Util_ORI.c Util_TIM.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/mcc_generated_files/i2c1.o ${OBJECTDIR}/mcc_generated_files/pin_manager.o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o ${OBJECTDIR}/mcc_generated_files/traps.o ${OBJECTDIR}/mcc_generated_files/tmr1.o ${OBJECTDIR}/mcc_generated_files/uart4.o ${OBJECTDIR}/mcc_generated_files/adc1.o ${OBJECTDIR}/mcc_generated_files/tmr3.o ${OBJECTDIR}/mcc_generated_files/pwm.o ${OBJECTDIR}/mcc_generated_files/reset.o ${OBJECTDIR}/mcc_generated_files/clock.o ${OBJECTDIR}/mcc_generated_files/system.o ${OBJECTDIR}/mcc_generated_files/tmr5.o ${OBJECTDIR}/mcc_generated_files/ext_int.o ${OBJECTDIR}/mcc_generated_files/uart1.o ${OBJECTDIR}/mcc_generated_files/uart2.o ${OBJECTDIR}/mcc_generated_files/uart3.o ${OBJECTDIR}/mcc_generated_files/mcc.o ${OBJECTDIR}/main.o ${OBJECTDIR}/Coms_123.o ${OBJECTDIR}/Coms_ESP.o ${OBJECTDIR}/Coms_REL.o ${OBJECTDIR}/Coms_CMD.o ${OBJECTDIR}/Acts_CPL.o ${OBJECTDIR}/Acts_LIN.o ${OBJECTDIR}/Acts_ROT.o ${OBJECTDIR}/Mnge_BAT.o ${OBJECTDIR}/Mnge_BTN.o ${OBJECTDIR}/Mnge_DAC.o ${OBJECTDIR}/Mnge_PWM.o ${OBJECTDIR}/Sens_ACC.o ${OBJECTDIR}/Sens_ENC.o ${OBJECTDIR}/Mnge_RGB.o ${OBJECTDIR}/Tmrs_CBK.o ${OBJECTDIR}/Mnge_ERR.o ${OBJECTDIR}/Sens_EDG.o ${OBJECTDIR}/Sens_FAC.o ${OBJECTDIR}/Sens_GRD.o ${OBJECTDIR}/Sens_MOD.o ${OBJECTDIR}/Util_MAT.o ${OBJECTDIR}/Util_MTH.o ${OBJECTDIR}/Util_ORI.o ${OBJECTDIR}/Util_TIM.o
POSSIBLE_DEPFILES=${OBJECTDIR}/mcc_generated_files/i2c1.o.d ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d ${OBJECTDIR}/mcc_generated_files/traps.o.d ${OBJECTDIR}/mcc_generated_files/tmr1.o.d ${OBJECTDIR}/mcc_generated_files/uart4.o.d ${OBJECTDIR}/mcc_generated_files/adc1.o.d ${OBJECTDIR}/mcc_generated_files/tmr3.o.d ${OBJECTDIR}/mcc_generated_files/pwm.o.d ${OBJECTDIR}/mcc_generated_files/reset.o.d ${OBJECTDIR}/mcc_generated_files/clock.o.d ${OBJECTDIR}/mcc_generated_files/system.o.d ${OBJECTDIR}/mcc_generated_files/tmr5.o.d ${OBJECTDIR}/mcc_generated_files/ext_int.o.d ${OBJECTDIR}/mcc_generated_files/uart1.o.d ${OBJECTDIR}/mcc_generated_files/uart2.o.d ${OBJECTDIR}/mcc_generated_files/uart3.o.d ${OBJECTDIR}/mcc_generated_files/mcc.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/Coms_123.o.d ${OBJECTDIR}/Coms_ESP.o.d ${OBJECTDIR}/Coms_REL.o.d ${OBJECTDIR}/Coms_CMD.o.d ${OBJECTDIR}/Acts_CPL.o.d ${OBJECTDIR}/Acts_LIN.o.d ${OBJECTDIR}/Acts_ROT.o.d ${OBJECTDIR}/Mnge_BAT.o.d ${OBJECTDIR}/Mnge_BTN.o.d ${OBJECTDIR}/Mnge_DAC.o.d ${OBJECTDIR}/Mnge_PWM.o.d ${OBJECTDIR}/Sens_ACC.o.d ${OBJECTDIR}/Sens_ENC.o.d ${OBJECTDIR}/Mnge_RGB.o.d ${OBJECTDIR}/Tmrs_CBK.o.d ${OBJECTDIR}/Mnge_ERR.o.d ${OBJECTDIR}/Sens_EDG.o.d ${OBJECTDIR}/Sens_FAC.o.d ${OBJECTDIR}/Sens_GRD.o.d ${OBJECTDIR}/Sens_MOD.o.d ${OBJECTDIR}/Util_MAT.o.d ${OBJECTDIR}/Util_MTH.o.d ${OBJECTDIR}/Util_ORI.o.d ${OBJECTDIR}/Util_TIM.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/mcc_generated_files/i2c1.o ${OBJECTDIR}/mcc_generated_files/pin_manager.o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o ${OBJECTDIR}/mcc_generated_files/traps.o ${OBJECTDIR}/mcc_generated_files/tmr1.o ${OBJECTDIR}/mcc_generated_files/uart4.o ${OBJECTDIR}/mcc_generated_files/adc1.o ${OBJECTDIR}/mcc_generated_files/tmr3.o ${OBJECTDIR}/mcc_generated_files/pwm.o ${OBJECTDIR}/mcc_generated_files/reset.o ${OBJECTDIR}/mcc_generated_files/clock.o ${OBJECTDIR}/mcc_generated_files/system.o ${OBJECTDIR}/mcc_generated_files/tmr5.o ${OBJECTDIR}/mcc_generated_files/ext_int.o ${OBJECTDIR}/mcc_generated_files/uart1.o ${OBJECTDIR}/mcc_generated_files/uart2.o ${OBJECTDIR}/mcc_generated_files/uart3.o ${OBJECTDIR}/mcc_generated_files/mcc.o ${OBJECTDIR}/main.o ${OBJECTDIR}/Coms_123.o ${OBJECTDIR}/Coms_ESP.o ${OBJECTDIR}/Coms_REL.o ${OBJECTDIR}/Coms_CMD.o ${OBJECTDIR}/Acts_CPL.o ${OBJECTDIR}/Acts_LIN.o ${OBJECTDIR}/Acts_ROT.o ${OBJECTDIR}/Mnge_BAT.o ${OBJECTDIR}/Mnge_BTN.o ${OBJECTDIR}/Mnge_DAC.o ${OBJECTDIR}/Mnge_PWM.o ${OBJECTDIR}/Sens_ACC.o ${OBJECTDIR}/Sens_ENC.o ${OBJECTDIR}/Mnge_RGB.o ${OBJECTDIR}/Tmrs_CBK.o ${OBJECTDIR}/Mnge_ERR.o ${OBJECTDIR}/Sens_EDG.o ${OBJECTDIR}/Sens_FAC.o ${OBJECTDIR}/Sens_GRD.o ${OBJECTDIR}/Sens_MOD.o ${OBJECTDIR}/Util_MAT.o ${OBJECTDIR}/Util_MTH.o ${OBJECTDIR}/Util_ORI.o ${OBJECTDIR}/Util_TIM.o

# Source Files
SOURCEFILES=mcc_generated_files/i2c1.c mcc_generated_files/pin_manager.c mcc_generated_files/interrupt_manager.c mcc_generated_files/traps.c mcc_generated_files/tmr1.c mcc_generated_files/uart4.c mcc_generated_files/adc1.c mcc_generated_files/tmr3.c mcc_generated_files/pwm.c mcc_generated_files/reset.c mcc_generated_files/clock.c mcc_generated_files/system.c mcc_generated_files/tmr5.c mcc_generated_files/ext_int.c mcc_generated_files/uart1.c mcc_generated_files/uart2.c mcc_generated_files/uart3.c mcc_generated_files/mcc.c main.c Coms_123.c Coms_ESP.c Coms_REL.c Coms_CMD.c Acts_CPL.c Acts_LIN.c Acts_ROT.c Mnge_BAT.c Mnge_BTN.c Mnge_DAC.c Mnge_PWM.c Sens_ACC.c Sens_ENC.c Mnge_RGB.c Tmrs_CBK.c Mnge_ERR.c Sens_EDG.c Sens_FAC.c Sens_GRD.c Sens_MOD.c Util_MAT.c Util_MTH.c Util_ORI.c Util_TIM.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/MoriController.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33EP512GM604
MP_LINKER_FILE_OPTION=,--script=p33EP512GM604.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/mcc_generated_files/i2c1.o: mcc_generated_files/i2c1.c  .generated_files/flags/default/ea574262f8ab2c0c6105bca66931080242df8ea9 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/i2c1.c  -o ${OBJECTDIR}/mcc_generated_files/i2c1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/i2c1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/pin_manager.o: mcc_generated_files/pin_manager.c  .generated_files/flags/default/32ab184eea00a173ea556bd3c91cf8823adb928a .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/pin_manager.c  -o ${OBJECTDIR}/mcc_generated_files/pin_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/interrupt_manager.o: mcc_generated_files/interrupt_manager.c  .generated_files/flags/default/412f93c23400167ca3cba1b8c86adb0b896fca3c .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/interrupt_manager.c  -o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/traps.o: mcc_generated_files/traps.c  .generated_files/flags/default/af15065ab6d3f092ac25d7e3d870c97b15338c37 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/traps.c  -o ${OBJECTDIR}/mcc_generated_files/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/traps.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/tmr1.o: mcc_generated_files/tmr1.c  .generated_files/flags/default/84161f602173e92febc706d889716dd0aad73928 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr1.c  -o ${OBJECTDIR}/mcc_generated_files/tmr1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/uart4.o: mcc_generated_files/uart4.c  .generated_files/flags/default/fc19fcfc00b60ed5ed62979cd072d32ddf457067 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart4.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart4.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/uart4.c  -o ${OBJECTDIR}/mcc_generated_files/uart4.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart4.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/adc1.o: mcc_generated_files/adc1.c  .generated_files/flags/default/854176d9173a8fff662ee1b8460c54ba17683d1e .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/adc1.c  -o ${OBJECTDIR}/mcc_generated_files/adc1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/adc1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/tmr3.o: mcc_generated_files/tmr3.c  .generated_files/flags/default/a16e1784d78e47877e485e5cdcd66e56847b66a3 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr3.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr3.c  -o ${OBJECTDIR}/mcc_generated_files/tmr3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr3.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/pwm.o: mcc_generated_files/pwm.c  .generated_files/flags/default/cb499b387bded0b63a9b65fdec990448bc66017e .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pwm.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/pwm.c  -o ${OBJECTDIR}/mcc_generated_files/pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/pwm.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/reset.o: mcc_generated_files/reset.c  .generated_files/flags/default/3db0d33d87ffe8e2ba2caaea68fca5b50ed77c2b .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/reset.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/reset.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/reset.c  -o ${OBJECTDIR}/mcc_generated_files/reset.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/reset.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/clock.o: mcc_generated_files/clock.c  .generated_files/flags/default/7d785e47bcbece5b94d00fa0f7233e67a0c05800 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/clock.c  -o ${OBJECTDIR}/mcc_generated_files/clock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/clock.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/system.o: mcc_generated_files/system.c  .generated_files/flags/default/98c13d2793e8de78b830adb46808eab79861dde .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/system.c  -o ${OBJECTDIR}/mcc_generated_files/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/system.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/tmr5.o: mcc_generated_files/tmr5.c  .generated_files/flags/default/4d249c7a44f21e3474ee2e79725d0c16c09b2aae .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr5.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr5.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr5.c  -o ${OBJECTDIR}/mcc_generated_files/tmr5.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr5.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/ext_int.o: mcc_generated_files/ext_int.c  .generated_files/flags/default/c31e0e314eae059bbab7ec7ff2de52d263c19ddb .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/ext_int.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/ext_int.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/ext_int.c  -o ${OBJECTDIR}/mcc_generated_files/ext_int.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/ext_int.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/uart1.o: mcc_generated_files/uart1.c  .generated_files/flags/default/e398886dfbd7338cba3f0bd162f5f3aaf09c562c .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/uart1.c  -o ${OBJECTDIR}/mcc_generated_files/uart1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/uart2.o: mcc_generated_files/uart2.c  .generated_files/flags/default/99199bef89e0a9dd97eccb082973b97777eaa7da .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/uart2.c  -o ${OBJECTDIR}/mcc_generated_files/uart2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/uart3.o: mcc_generated_files/uart3.c  .generated_files/flags/default/548a77c4b77296d1e803fb10a1041c3630ff4861 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart3.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/uart3.c  -o ${OBJECTDIR}/mcc_generated_files/uart3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart3.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/mcc.o: mcc_generated_files/mcc.c  .generated_files/flags/default/44b9f5180a0569d8ec27fa404866c953eec9b819 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/mcc.c  -o ${OBJECTDIR}/mcc_generated_files/mcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/mcc.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/84e4fa04e305c4dccfcd087595a1453875d98805 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Coms_123.o: Coms_123.c  .generated_files/flags/default/af8163c501a193d110b87719f3f327b93590b3b5 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Coms_123.o.d 
	@${RM} ${OBJECTDIR}/Coms_123.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Coms_123.c  -o ${OBJECTDIR}/Coms_123.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Coms_123.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Coms_ESP.o: Coms_ESP.c  .generated_files/flags/default/40e2eb1c691251b50dcb3598fc60f1430237618 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Coms_ESP.o.d 
	@${RM} ${OBJECTDIR}/Coms_ESP.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Coms_ESP.c  -o ${OBJECTDIR}/Coms_ESP.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Coms_ESP.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Coms_REL.o: Coms_REL.c  .generated_files/flags/default/87e404d650a7a90e405fdeec5b9577156b29a953 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Coms_REL.o.d 
	@${RM} ${OBJECTDIR}/Coms_REL.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Coms_REL.c  -o ${OBJECTDIR}/Coms_REL.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Coms_REL.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Coms_CMD.o: Coms_CMD.c  .generated_files/flags/default/6464a52f90b1f7dd01dfb0f8e98d882c6ee8b98c .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Coms_CMD.o.d 
	@${RM} ${OBJECTDIR}/Coms_CMD.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Coms_CMD.c  -o ${OBJECTDIR}/Coms_CMD.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Coms_CMD.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Acts_CPL.o: Acts_CPL.c  .generated_files/flags/default/11782f807542b0aca8d6d6c9a1f5ecd143922e96 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Acts_CPL.o.d 
	@${RM} ${OBJECTDIR}/Acts_CPL.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Acts_CPL.c  -o ${OBJECTDIR}/Acts_CPL.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Acts_CPL.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Acts_LIN.o: Acts_LIN.c  .generated_files/flags/default/2f2e0da39eea534dcbc98540aa55f69c1ffa3a1d .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Acts_LIN.o.d 
	@${RM} ${OBJECTDIR}/Acts_LIN.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Acts_LIN.c  -o ${OBJECTDIR}/Acts_LIN.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Acts_LIN.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Acts_ROT.o: Acts_ROT.c  .generated_files/flags/default/9f6564921d1a7b0c6180c285ef69959cb1aae64f .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Acts_ROT.o.d 
	@${RM} ${OBJECTDIR}/Acts_ROT.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Acts_ROT.c  -o ${OBJECTDIR}/Acts_ROT.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Acts_ROT.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O3 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Mnge_BAT.o: Mnge_BAT.c  .generated_files/flags/default/4d4aa8aa349da6388e8ba1ff70c53d7524fc009d .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Mnge_BAT.o.d 
	@${RM} ${OBJECTDIR}/Mnge_BAT.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Mnge_BAT.c  -o ${OBJECTDIR}/Mnge_BAT.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Mnge_BAT.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Mnge_BTN.o: Mnge_BTN.c  .generated_files/flags/default/ff93020fd8a2b883d41ea02f98181c73663ecd30 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Mnge_BTN.o.d 
	@${RM} ${OBJECTDIR}/Mnge_BTN.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Mnge_BTN.c  -o ${OBJECTDIR}/Mnge_BTN.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Mnge_BTN.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Mnge_DAC.o: Mnge_DAC.c  .generated_files/flags/default/dc36a8728a85c9900343aed1dd366c936c1dbbc1 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Mnge_DAC.o.d 
	@${RM} ${OBJECTDIR}/Mnge_DAC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Mnge_DAC.c  -o ${OBJECTDIR}/Mnge_DAC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Mnge_DAC.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Mnge_PWM.o: Mnge_PWM.c  .generated_files/flags/default/fe6c1ec913f2a9419df36d4533c418eefd1da959 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Mnge_PWM.o.d 
	@${RM} ${OBJECTDIR}/Mnge_PWM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Mnge_PWM.c  -o ${OBJECTDIR}/Mnge_PWM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Mnge_PWM.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Sens_ACC.o: Sens_ACC.c  .generated_files/flags/default/f6bfba77c5dc9e8174db8417fa065859b24c4f8e .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Sens_ACC.o.d 
	@${RM} ${OBJECTDIR}/Sens_ACC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Sens_ACC.c  -o ${OBJECTDIR}/Sens_ACC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Sens_ACC.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O3 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Sens_ENC.o: Sens_ENC.c  .generated_files/flags/default/7db5485d8dee61665d5b2a9f3b866eac895c4160 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Sens_ENC.o.d 
	@${RM} ${OBJECTDIR}/Sens_ENC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Sens_ENC.c  -o ${OBJECTDIR}/Sens_ENC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Sens_ENC.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O3 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Mnge_RGB.o: Mnge_RGB.c  .generated_files/flags/default/96e03149657e2654b59dcbd2efa49573c8d93abb .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Mnge_RGB.o.d 
	@${RM} ${OBJECTDIR}/Mnge_RGB.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Mnge_RGB.c  -o ${OBJECTDIR}/Mnge_RGB.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Mnge_RGB.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Tmrs_CBK.o: Tmrs_CBK.c  .generated_files/flags/default/e88a668f2468fce1797f8370e53bb9411a710460 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Tmrs_CBK.o.d 
	@${RM} ${OBJECTDIR}/Tmrs_CBK.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Tmrs_CBK.c  -o ${OBJECTDIR}/Tmrs_CBK.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Tmrs_CBK.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Mnge_ERR.o: Mnge_ERR.c  .generated_files/flags/default/67bdadcc384a27ee53d2994ce610c12747ab28e4 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Mnge_ERR.o.d 
	@${RM} ${OBJECTDIR}/Mnge_ERR.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Mnge_ERR.c  -o ${OBJECTDIR}/Mnge_ERR.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Mnge_ERR.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Sens_EDG.o: Sens_EDG.c  .generated_files/flags/default/5da4930814f00054856fc47f6e54eaf38b2c62cb .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Sens_EDG.o.d 
	@${RM} ${OBJECTDIR}/Sens_EDG.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Sens_EDG.c  -o ${OBJECTDIR}/Sens_EDG.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Sens_EDG.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Sens_FAC.o: Sens_FAC.c  .generated_files/flags/default/31fa8ac8f636e1c74d7fba3e42130649a3982567 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Sens_FAC.o.d 
	@${RM} ${OBJECTDIR}/Sens_FAC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Sens_FAC.c  -o ${OBJECTDIR}/Sens_FAC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Sens_FAC.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Sens_GRD.o: Sens_GRD.c  .generated_files/flags/default/20f0c280e7a0e8178033f33cc12e4382b6bbbb5f .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Sens_GRD.o.d 
	@${RM} ${OBJECTDIR}/Sens_GRD.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Sens_GRD.c  -o ${OBJECTDIR}/Sens_GRD.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Sens_GRD.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Sens_MOD.o: Sens_MOD.c  .generated_files/flags/default/c4947ab4f2925d7d71f6aee89a821ec2d1d65eec .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Sens_MOD.o.d 
	@${RM} ${OBJECTDIR}/Sens_MOD.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Sens_MOD.c  -o ${OBJECTDIR}/Sens_MOD.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Sens_MOD.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Util_MAT.o: Util_MAT.c  .generated_files/flags/default/36b25ad0d1165f66ffca83321791e7c5462925ed .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Util_MAT.o.d 
	@${RM} ${OBJECTDIR}/Util_MAT.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Util_MAT.c  -o ${OBJECTDIR}/Util_MAT.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Util_MAT.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Util_MTH.o: Util_MTH.c  .generated_files/flags/default/a33892b3121b89fe413adeffc7a8afa72f03fc37 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Util_MTH.o.d 
	@${RM} ${OBJECTDIR}/Util_MTH.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Util_MTH.c  -o ${OBJECTDIR}/Util_MTH.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Util_MTH.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Util_ORI.o: Util_ORI.c  .generated_files/flags/default/ce74c1a4ef0089d2510f22b3fe5f2907c1978714 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Util_ORI.o.d 
	@${RM} ${OBJECTDIR}/Util_ORI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Util_ORI.c  -o ${OBJECTDIR}/Util_ORI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Util_ORI.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Util_TIM.o: Util_TIM.c  .generated_files/flags/default/66bc52d6551c2b034e3c5e27775403980315d309 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Util_TIM.o.d 
	@${RM} ${OBJECTDIR}/Util_TIM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Util_TIM.c  -o ${OBJECTDIR}/Util_TIM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Util_TIM.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/mcc_generated_files/i2c1.o: mcc_generated_files/i2c1.c  .generated_files/flags/default/ad17b64caff98d7337bb870224f5475a58457ec6 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/i2c1.c  -o ${OBJECTDIR}/mcc_generated_files/i2c1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/i2c1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/pin_manager.o: mcc_generated_files/pin_manager.c  .generated_files/flags/default/7395055fe933a7ee997d86cf5001c6c2e735345a .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/pin_manager.c  -o ${OBJECTDIR}/mcc_generated_files/pin_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/interrupt_manager.o: mcc_generated_files/interrupt_manager.c  .generated_files/flags/default/15c6206fea90acd01f3a624f9be7e1f90aaf692e .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/interrupt_manager.c  -o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/traps.o: mcc_generated_files/traps.c  .generated_files/flags/default/a72aabd14317e31b3e7afbd204214af22da51e01 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/traps.c  -o ${OBJECTDIR}/mcc_generated_files/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/traps.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/tmr1.o: mcc_generated_files/tmr1.c  .generated_files/flags/default/cb49d889e26d18d6331620173f02288c76d80916 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr1.c  -o ${OBJECTDIR}/mcc_generated_files/tmr1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr1.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/uart4.o: mcc_generated_files/uart4.c  .generated_files/flags/default/48df7f27e3aaa8fc214634c86937971e3e3df55e .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart4.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart4.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/uart4.c  -o ${OBJECTDIR}/mcc_generated_files/uart4.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart4.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/adc1.o: mcc_generated_files/adc1.c  .generated_files/flags/default/be95df07cddef0dfb57ecbd2c122dd77f082b976 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/adc1.c  -o ${OBJECTDIR}/mcc_generated_files/adc1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/adc1.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/tmr3.o: mcc_generated_files/tmr3.c  .generated_files/flags/default/a057c00d2c3be70573953b39f7d538f7eaa0b32 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr3.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr3.c  -o ${OBJECTDIR}/mcc_generated_files/tmr3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr3.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/pwm.o: mcc_generated_files/pwm.c  .generated_files/flags/default/35082aa216d870cee7524fd7ddc437e273dc10ff .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pwm.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pwm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/pwm.c  -o ${OBJECTDIR}/mcc_generated_files/pwm.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/pwm.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/reset.o: mcc_generated_files/reset.c  .generated_files/flags/default/4bfeba6a41367c8722d89d4310522f7b1fe8e943 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/reset.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/reset.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/reset.c  -o ${OBJECTDIR}/mcc_generated_files/reset.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/reset.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/clock.o: mcc_generated_files/clock.c  .generated_files/flags/default/448238bf88928359d897a0d8b79c5b7edb9a1f1a .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/clock.c  -o ${OBJECTDIR}/mcc_generated_files/clock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/clock.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/system.o: mcc_generated_files/system.c  .generated_files/flags/default/4900b8b2e5802a42da5fd69a8af6f6b00b8b38d4 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/system.c  -o ${OBJECTDIR}/mcc_generated_files/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/system.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/tmr5.o: mcc_generated_files/tmr5.c  .generated_files/flags/default/a59cc61ae3982be90cb395295ab8963d1d36df41 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr5.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr5.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr5.c  -o ${OBJECTDIR}/mcc_generated_files/tmr5.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr5.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/ext_int.o: mcc_generated_files/ext_int.c  .generated_files/flags/default/521ac371dfef3f71c02591361ce45fc65741c96d .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/ext_int.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/ext_int.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/ext_int.c  -o ${OBJECTDIR}/mcc_generated_files/ext_int.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/ext_int.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/uart1.o: mcc_generated_files/uart1.c  .generated_files/flags/default/a9d29c4fc005de2859171fe95c0bbab6786e59b5 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/uart1.c  -o ${OBJECTDIR}/mcc_generated_files/uart1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart1.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/uart2.o: mcc_generated_files/uart2.c  .generated_files/flags/default/9de7d53b6a83e5ee525fb14cf80b261c85a2205c .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/uart2.c  -o ${OBJECTDIR}/mcc_generated_files/uart2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart2.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/uart3.o: mcc_generated_files/uart3.c  .generated_files/flags/default/542c074542708ff5cb01d178b67b8646dece59d9 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart3.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/uart3.c  -o ${OBJECTDIR}/mcc_generated_files/uart3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart3.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/mcc.o: mcc_generated_files/mcc.c  .generated_files/flags/default/9bc13cc424bc44f13d18092e7f7b0eadb19be961 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/mcc.c  -o ${OBJECTDIR}/mcc_generated_files/mcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/mcc.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/64db92c6b99c6cb8cef808c882f870de7361d07e .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Coms_123.o: Coms_123.c  .generated_files/flags/default/8159c9543e6ff8a10a2c0398d903e7abd278df87 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Coms_123.o.d 
	@${RM} ${OBJECTDIR}/Coms_123.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Coms_123.c  -o ${OBJECTDIR}/Coms_123.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Coms_123.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O1 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Coms_ESP.o: Coms_ESP.c  .generated_files/flags/default/b91dab70f77ed4cbceea38cfc1b7b26173f77d40 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Coms_ESP.o.d 
	@${RM} ${OBJECTDIR}/Coms_ESP.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Coms_ESP.c  -o ${OBJECTDIR}/Coms_ESP.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Coms_ESP.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Coms_REL.o: Coms_REL.c  .generated_files/flags/default/96b2778ecd8256ef5ffa89c54e0e06beb98c6c1b .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Coms_REL.o.d 
	@${RM} ${OBJECTDIR}/Coms_REL.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Coms_REL.c  -o ${OBJECTDIR}/Coms_REL.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Coms_REL.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Coms_CMD.o: Coms_CMD.c  .generated_files/flags/default/b7b6b60151500d392a12dd97f07af0933a3eaac6 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Coms_CMD.o.d 
	@${RM} ${OBJECTDIR}/Coms_CMD.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Coms_CMD.c  -o ${OBJECTDIR}/Coms_CMD.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Coms_CMD.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Acts_CPL.o: Acts_CPL.c  .generated_files/flags/default/24988b235f7ea5bef7814c84bde2f5e1332b06b3 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Acts_CPL.o.d 
	@${RM} ${OBJECTDIR}/Acts_CPL.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Acts_CPL.c  -o ${OBJECTDIR}/Acts_CPL.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Acts_CPL.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Acts_LIN.o: Acts_LIN.c  .generated_files/flags/default/b7f519a1f8d39e52bd6e6f7b198ea93c62a47a6d .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Acts_LIN.o.d 
	@${RM} ${OBJECTDIR}/Acts_LIN.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Acts_LIN.c  -o ${OBJECTDIR}/Acts_LIN.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Acts_LIN.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Acts_ROT.o: Acts_ROT.c  .generated_files/flags/default/cb797498773fb2b405e0eb8da7b62d8303ce6df0 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Acts_ROT.o.d 
	@${RM} ${OBJECTDIR}/Acts_ROT.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Acts_ROT.c  -o ${OBJECTDIR}/Acts_ROT.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Acts_ROT.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O3 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Mnge_BAT.o: Mnge_BAT.c  .generated_files/flags/default/58ed2af41a550362dc8cface3d391d249051a4d8 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Mnge_BAT.o.d 
	@${RM} ${OBJECTDIR}/Mnge_BAT.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Mnge_BAT.c  -o ${OBJECTDIR}/Mnge_BAT.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Mnge_BAT.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Mnge_BTN.o: Mnge_BTN.c  .generated_files/flags/default/e0d63e1f6eedd8fb0382e55c4afe9463bcb27f49 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Mnge_BTN.o.d 
	@${RM} ${OBJECTDIR}/Mnge_BTN.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Mnge_BTN.c  -o ${OBJECTDIR}/Mnge_BTN.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Mnge_BTN.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Mnge_DAC.o: Mnge_DAC.c  .generated_files/flags/default/e71297afde2d50b8c99877b8cabe0c1aa1cc7207 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Mnge_DAC.o.d 
	@${RM} ${OBJECTDIR}/Mnge_DAC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Mnge_DAC.c  -o ${OBJECTDIR}/Mnge_DAC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Mnge_DAC.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Mnge_PWM.o: Mnge_PWM.c  .generated_files/flags/default/1caeec75743e8b830f1145028a81eac7adf5bf9 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Mnge_PWM.o.d 
	@${RM} ${OBJECTDIR}/Mnge_PWM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Mnge_PWM.c  -o ${OBJECTDIR}/Mnge_PWM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Mnge_PWM.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Sens_ACC.o: Sens_ACC.c  .generated_files/flags/default/ed95e528758a4cb5eab314f59b2613c950856440 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Sens_ACC.o.d 
	@${RM} ${OBJECTDIR}/Sens_ACC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Sens_ACC.c  -o ${OBJECTDIR}/Sens_ACC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Sens_ACC.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O3 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Sens_ENC.o: Sens_ENC.c  .generated_files/flags/default/eb6455e7e9bbfe9b8560f0d8a5c7f1dd9fab0b74 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Sens_ENC.o.d 
	@${RM} ${OBJECTDIR}/Sens_ENC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Sens_ENC.c  -o ${OBJECTDIR}/Sens_ENC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Sens_ENC.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O3 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Mnge_RGB.o: Mnge_RGB.c  .generated_files/flags/default/d6bf62d7d8858755db744479d32b87daa58ea1d8 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Mnge_RGB.o.d 
	@${RM} ${OBJECTDIR}/Mnge_RGB.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Mnge_RGB.c  -o ${OBJECTDIR}/Mnge_RGB.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Mnge_RGB.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Tmrs_CBK.o: Tmrs_CBK.c  .generated_files/flags/default/3a880c0309d522f71c8c919b3ec416ec9c0d3d9b .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Tmrs_CBK.o.d 
	@${RM} ${OBJECTDIR}/Tmrs_CBK.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Tmrs_CBK.c  -o ${OBJECTDIR}/Tmrs_CBK.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Tmrs_CBK.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Mnge_ERR.o: Mnge_ERR.c  .generated_files/flags/default/7aaa3e885bd51f5e40305ddfab305396df03acdc .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Mnge_ERR.o.d 
	@${RM} ${OBJECTDIR}/Mnge_ERR.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Mnge_ERR.c  -o ${OBJECTDIR}/Mnge_ERR.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Mnge_ERR.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Sens_EDG.o: Sens_EDG.c  .generated_files/flags/default/a0ffc40c02d9afcc40c04b0981e07ecd1107473f .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Sens_EDG.o.d 
	@${RM} ${OBJECTDIR}/Sens_EDG.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Sens_EDG.c  -o ${OBJECTDIR}/Sens_EDG.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Sens_EDG.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Sens_FAC.o: Sens_FAC.c  .generated_files/flags/default/776608db2b3c528bc907fd94d7c12ee6df5cfc9a .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Sens_FAC.o.d 
	@${RM} ${OBJECTDIR}/Sens_FAC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Sens_FAC.c  -o ${OBJECTDIR}/Sens_FAC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Sens_FAC.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Sens_GRD.o: Sens_GRD.c  .generated_files/flags/default/fefa6e5c729ef42bfd418c7a9814fe7d4bd2b36c .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Sens_GRD.o.d 
	@${RM} ${OBJECTDIR}/Sens_GRD.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Sens_GRD.c  -o ${OBJECTDIR}/Sens_GRD.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Sens_GRD.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Sens_MOD.o: Sens_MOD.c  .generated_files/flags/default/b0094e0be5f3ff0079a649ba3db755b7612b268a .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Sens_MOD.o.d 
	@${RM} ${OBJECTDIR}/Sens_MOD.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Sens_MOD.c  -o ${OBJECTDIR}/Sens_MOD.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Sens_MOD.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Util_MAT.o: Util_MAT.c  .generated_files/flags/default/9907df3f29a8086779a04bb540f464904453de8c .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Util_MAT.o.d 
	@${RM} ${OBJECTDIR}/Util_MAT.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Util_MAT.c  -o ${OBJECTDIR}/Util_MAT.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Util_MAT.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Util_MTH.o: Util_MTH.c  .generated_files/flags/default/39f633340df9d44e88d3fd2d992316f7cfb5112 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Util_MTH.o.d 
	@${RM} ${OBJECTDIR}/Util_MTH.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Util_MTH.c  -o ${OBJECTDIR}/Util_MTH.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Util_MTH.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Util_ORI.o: Util_ORI.c  .generated_files/flags/default/94ec90ca3036e0bfbec72322130bf5e610391735 .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Util_ORI.o.d 
	@${RM} ${OBJECTDIR}/Util_ORI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Util_ORI.c  -o ${OBJECTDIR}/Util_ORI.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Util_ORI.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/Util_TIM.o: Util_TIM.c  .generated_files/flags/default/6c54fa7e0600230af49948f53f0ed95c4bc99fe .generated_files/flags/default/63bb65385b9bcfd57ff63c9d0ae2a7c08640c179
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Util_TIM.o.d 
	@${RM} ${OBJECTDIR}/Util_TIM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  Util_TIM.c  -o ${OBJECTDIR}/Util_TIM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Util_TIM.o.d"      -mno-eds-warn  -g -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O2 -funroll-loops -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/MoriController.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/MoriController.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)   -mreserve=data@0x1000:0x101B -mreserve=data@0x101C:0x101D -mreserve=data@0x101E:0x101F -mreserve=data@0x1020:0x1021 -mreserve=data@0x1022:0x1023 -mreserve=data@0x1024:0x1027 -mreserve=data@0x1028:0x104F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/MoriController.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/MoriController.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -save-temps=obj -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}\\xc16-bin2hex ${DISTDIR}/MoriController.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
