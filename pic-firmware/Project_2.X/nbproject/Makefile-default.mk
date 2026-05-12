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
FINAL_IMAGE=${DISTDIR}/Project_2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Project_2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Button/Button.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Buzzer/Buzzer.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Current/Current.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Fan/Fan.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Humidity/Humidity.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/LCD/LCD.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/LED/LED.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Motor/Motor.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Relay/Relay.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Soil/Soil.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Temperature/Temp.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Ultrasonic/Ultrasonic.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/ADC/ADC.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/GPIO/GPIO.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/Interrupt_Manager/Interrupt_Manager.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/TIMER0/Timer0.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/USART/USART.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/SERVICES/Delay.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/APP/Comms/Comms.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/APP/Irrigation/Irrigation.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/APP/MyProject.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/APP/Safety/Safety.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/2029219237/Button.p1 ${OBJECTDIR}/_ext/2029035031/Buzzer.p1 ${OBJECTDIR}/_ext/1890666640/Current.p1 ${OBJECTDIR}/_ext/1957042314/Fan.p1 ${OBJECTDIR}/_ext/1491501468/Humidity.p1 ${OBJECTDIR}/_ext/1957047108/LCD.p1 ${OBJECTDIR}/_ext/1957047170/LED.p1 ${OBJECTDIR}/_ext/471120852/Motor.p1 ${OBJECTDIR}/_ext/466809272/Relay.p1 ${OBJECTDIR}/_ext/539170280/Soil.p1 ${OBJECTDIR}/_ext/1086581739/Temp.p1 ${OBJECTDIR}/_ext/1723092823/Ultrasonic.p1 ${OBJECTDIR}/_ext/2145928993/ADC.p1 ${OBJECTDIR}/_ext/2099098800/GPIO.p1 ${OBJECTDIR}/_ext/1111239760/Interrupt_Manager.p1 ${OBJECTDIR}/_ext/1766508780/Timer0.p1 ${OBJECTDIR}/_ext/634542204/USART.p1 ${OBJECTDIR}/_ext/1992713099/Delay.p1 ${OBJECTDIR}/_ext/1085684820/Comms.p1 ${OBJECTDIR}/_ext/398482839/Irrigation.p1 ${OBJECTDIR}/_ext/860197556/MyProject.p1 ${OBJECTDIR}/_ext/1148429997/Safety.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/2029219237/Button.p1.d ${OBJECTDIR}/_ext/2029035031/Buzzer.p1.d ${OBJECTDIR}/_ext/1890666640/Current.p1.d ${OBJECTDIR}/_ext/1957042314/Fan.p1.d ${OBJECTDIR}/_ext/1491501468/Humidity.p1.d ${OBJECTDIR}/_ext/1957047108/LCD.p1.d ${OBJECTDIR}/_ext/1957047170/LED.p1.d ${OBJECTDIR}/_ext/471120852/Motor.p1.d ${OBJECTDIR}/_ext/466809272/Relay.p1.d ${OBJECTDIR}/_ext/539170280/Soil.p1.d ${OBJECTDIR}/_ext/1086581739/Temp.p1.d ${OBJECTDIR}/_ext/1723092823/Ultrasonic.p1.d ${OBJECTDIR}/_ext/2145928993/ADC.p1.d ${OBJECTDIR}/_ext/2099098800/GPIO.p1.d ${OBJECTDIR}/_ext/1111239760/Interrupt_Manager.p1.d ${OBJECTDIR}/_ext/1766508780/Timer0.p1.d ${OBJECTDIR}/_ext/634542204/USART.p1.d ${OBJECTDIR}/_ext/1992713099/Delay.p1.d ${OBJECTDIR}/_ext/1085684820/Comms.p1.d ${OBJECTDIR}/_ext/398482839/Irrigation.p1.d ${OBJECTDIR}/_ext/860197556/MyProject.p1.d ${OBJECTDIR}/_ext/1148429997/Safety.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/2029219237/Button.p1 ${OBJECTDIR}/_ext/2029035031/Buzzer.p1 ${OBJECTDIR}/_ext/1890666640/Current.p1 ${OBJECTDIR}/_ext/1957042314/Fan.p1 ${OBJECTDIR}/_ext/1491501468/Humidity.p1 ${OBJECTDIR}/_ext/1957047108/LCD.p1 ${OBJECTDIR}/_ext/1957047170/LED.p1 ${OBJECTDIR}/_ext/471120852/Motor.p1 ${OBJECTDIR}/_ext/466809272/Relay.p1 ${OBJECTDIR}/_ext/539170280/Soil.p1 ${OBJECTDIR}/_ext/1086581739/Temp.p1 ${OBJECTDIR}/_ext/1723092823/Ultrasonic.p1 ${OBJECTDIR}/_ext/2145928993/ADC.p1 ${OBJECTDIR}/_ext/2099098800/GPIO.p1 ${OBJECTDIR}/_ext/1111239760/Interrupt_Manager.p1 ${OBJECTDIR}/_ext/1766508780/Timer0.p1 ${OBJECTDIR}/_ext/634542204/USART.p1 ${OBJECTDIR}/_ext/1992713099/Delay.p1 ${OBJECTDIR}/_ext/1085684820/Comms.p1 ${OBJECTDIR}/_ext/398482839/Irrigation.p1 ${OBJECTDIR}/_ext/860197556/MyProject.p1 ${OBJECTDIR}/_ext/1148429997/Safety.p1

# Source Files
SOURCEFILES=C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Button/Button.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Buzzer/Buzzer.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Current/Current.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Fan/Fan.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Humidity/Humidity.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/LCD/LCD.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/LED/LED.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Motor/Motor.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Relay/Relay.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Soil/Soil.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Temperature/Temp.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Ultrasonic/Ultrasonic.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/ADC/ADC.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/GPIO/GPIO.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/Interrupt_Manager/Interrupt_Manager.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/TIMER0/Timer0.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/USART/USART.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/SERVICES/Delay.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/APP/Comms/Comms.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/APP/Irrigation/Irrigation.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/APP/MyProject.c C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/APP/Safety/Safety.c



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
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/Project_2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=16F877A
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/2029219237/Button.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Button/Button.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/2029219237" 
	@${RM} ${OBJECTDIR}/_ext/2029219237/Button.p1.d 
	@${RM} ${OBJECTDIR}/_ext/2029219237/Button.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/2029219237/Button.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Button/Button.c 
	@-${MV} ${OBJECTDIR}/_ext/2029219237/Button.d ${OBJECTDIR}/_ext/2029219237/Button.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/2029219237/Button.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/2029035031/Buzzer.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Buzzer/Buzzer.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/2029035031" 
	@${RM} ${OBJECTDIR}/_ext/2029035031/Buzzer.p1.d 
	@${RM} ${OBJECTDIR}/_ext/2029035031/Buzzer.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/2029035031/Buzzer.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Buzzer/Buzzer.c 
	@-${MV} ${OBJECTDIR}/_ext/2029035031/Buzzer.d ${OBJECTDIR}/_ext/2029035031/Buzzer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/2029035031/Buzzer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1890666640/Current.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Current/Current.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1890666640" 
	@${RM} ${OBJECTDIR}/_ext/1890666640/Current.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1890666640/Current.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1890666640/Current.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Current/Current.c 
	@-${MV} ${OBJECTDIR}/_ext/1890666640/Current.d ${OBJECTDIR}/_ext/1890666640/Current.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1890666640/Current.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1957042314/Fan.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Fan/Fan.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1957042314" 
	@${RM} ${OBJECTDIR}/_ext/1957042314/Fan.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1957042314/Fan.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1957042314/Fan.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Fan/Fan.c 
	@-${MV} ${OBJECTDIR}/_ext/1957042314/Fan.d ${OBJECTDIR}/_ext/1957042314/Fan.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1957042314/Fan.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1491501468/Humidity.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Humidity/Humidity.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1491501468" 
	@${RM} ${OBJECTDIR}/_ext/1491501468/Humidity.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1491501468/Humidity.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1491501468/Humidity.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Humidity/Humidity.c 
	@-${MV} ${OBJECTDIR}/_ext/1491501468/Humidity.d ${OBJECTDIR}/_ext/1491501468/Humidity.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1491501468/Humidity.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1957047108/LCD.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/LCD/LCD.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1957047108" 
	@${RM} ${OBJECTDIR}/_ext/1957047108/LCD.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1957047108/LCD.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1957047108/LCD.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/LCD/LCD.c 
	@-${MV} ${OBJECTDIR}/_ext/1957047108/LCD.d ${OBJECTDIR}/_ext/1957047108/LCD.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1957047108/LCD.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1957047170/LED.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/LED/LED.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1957047170" 
	@${RM} ${OBJECTDIR}/_ext/1957047170/LED.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1957047170/LED.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1957047170/LED.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/LED/LED.c 
	@-${MV} ${OBJECTDIR}/_ext/1957047170/LED.d ${OBJECTDIR}/_ext/1957047170/LED.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1957047170/LED.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/471120852/Motor.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Motor/Motor.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/471120852" 
	@${RM} ${OBJECTDIR}/_ext/471120852/Motor.p1.d 
	@${RM} ${OBJECTDIR}/_ext/471120852/Motor.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/471120852/Motor.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Motor/Motor.c 
	@-${MV} ${OBJECTDIR}/_ext/471120852/Motor.d ${OBJECTDIR}/_ext/471120852/Motor.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/471120852/Motor.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/466809272/Relay.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Relay/Relay.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/466809272" 
	@${RM} ${OBJECTDIR}/_ext/466809272/Relay.p1.d 
	@${RM} ${OBJECTDIR}/_ext/466809272/Relay.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/466809272/Relay.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Relay/Relay.c 
	@-${MV} ${OBJECTDIR}/_ext/466809272/Relay.d ${OBJECTDIR}/_ext/466809272/Relay.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/466809272/Relay.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/539170280/Soil.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Soil/Soil.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/539170280" 
	@${RM} ${OBJECTDIR}/_ext/539170280/Soil.p1.d 
	@${RM} ${OBJECTDIR}/_ext/539170280/Soil.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/539170280/Soil.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Soil/Soil.c 
	@-${MV} ${OBJECTDIR}/_ext/539170280/Soil.d ${OBJECTDIR}/_ext/539170280/Soil.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/539170280/Soil.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1086581739/Temp.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Temperature/Temp.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1086581739" 
	@${RM} ${OBJECTDIR}/_ext/1086581739/Temp.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1086581739/Temp.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1086581739/Temp.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Temperature/Temp.c 
	@-${MV} ${OBJECTDIR}/_ext/1086581739/Temp.d ${OBJECTDIR}/_ext/1086581739/Temp.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1086581739/Temp.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1723092823/Ultrasonic.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Ultrasonic/Ultrasonic.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1723092823" 
	@${RM} ${OBJECTDIR}/_ext/1723092823/Ultrasonic.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1723092823/Ultrasonic.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1723092823/Ultrasonic.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Ultrasonic/Ultrasonic.c 
	@-${MV} ${OBJECTDIR}/_ext/1723092823/Ultrasonic.d ${OBJECTDIR}/_ext/1723092823/Ultrasonic.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1723092823/Ultrasonic.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/2145928993/ADC.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/ADC/ADC.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/2145928993" 
	@${RM} ${OBJECTDIR}/_ext/2145928993/ADC.p1.d 
	@${RM} ${OBJECTDIR}/_ext/2145928993/ADC.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/2145928993/ADC.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/ADC/ADC.c 
	@-${MV} ${OBJECTDIR}/_ext/2145928993/ADC.d ${OBJECTDIR}/_ext/2145928993/ADC.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/2145928993/ADC.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/2099098800/GPIO.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/GPIO/GPIO.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/2099098800" 
	@${RM} ${OBJECTDIR}/_ext/2099098800/GPIO.p1.d 
	@${RM} ${OBJECTDIR}/_ext/2099098800/GPIO.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/2099098800/GPIO.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/GPIO/GPIO.c 
	@-${MV} ${OBJECTDIR}/_ext/2099098800/GPIO.d ${OBJECTDIR}/_ext/2099098800/GPIO.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/2099098800/GPIO.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1111239760/Interrupt_Manager.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/Interrupt_Manager/Interrupt_Manager.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1111239760" 
	@${RM} ${OBJECTDIR}/_ext/1111239760/Interrupt_Manager.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1111239760/Interrupt_Manager.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1111239760/Interrupt_Manager.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/Interrupt_Manager/Interrupt_Manager.c 
	@-${MV} ${OBJECTDIR}/_ext/1111239760/Interrupt_Manager.d ${OBJECTDIR}/_ext/1111239760/Interrupt_Manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1111239760/Interrupt_Manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1766508780/Timer0.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/TIMER0/Timer0.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1766508780" 
	@${RM} ${OBJECTDIR}/_ext/1766508780/Timer0.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1766508780/Timer0.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1766508780/Timer0.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/TIMER0/Timer0.c 
	@-${MV} ${OBJECTDIR}/_ext/1766508780/Timer0.d ${OBJECTDIR}/_ext/1766508780/Timer0.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1766508780/Timer0.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/634542204/USART.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/USART/USART.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/634542204" 
	@${RM} ${OBJECTDIR}/_ext/634542204/USART.p1.d 
	@${RM} ${OBJECTDIR}/_ext/634542204/USART.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/634542204/USART.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/USART/USART.c 
	@-${MV} ${OBJECTDIR}/_ext/634542204/USART.d ${OBJECTDIR}/_ext/634542204/USART.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/634542204/USART.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1992713099/Delay.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/SERVICES/Delay.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1992713099" 
	@${RM} ${OBJECTDIR}/_ext/1992713099/Delay.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1992713099/Delay.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1992713099/Delay.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/SERVICES/Delay.c 
	@-${MV} ${OBJECTDIR}/_ext/1992713099/Delay.d ${OBJECTDIR}/_ext/1992713099/Delay.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1992713099/Delay.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1085684820/Comms.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/APP/Comms/Comms.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1085684820" 
	@${RM} ${OBJECTDIR}/_ext/1085684820/Comms.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1085684820/Comms.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1085684820/Comms.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/APP/Comms/Comms.c 
	@-${MV} ${OBJECTDIR}/_ext/1085684820/Comms.d ${OBJECTDIR}/_ext/1085684820/Comms.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1085684820/Comms.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/398482839/Irrigation.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/APP/Irrigation/Irrigation.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/398482839" 
	@${RM} ${OBJECTDIR}/_ext/398482839/Irrigation.p1.d 
	@${RM} ${OBJECTDIR}/_ext/398482839/Irrigation.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/398482839/Irrigation.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/APP/Irrigation/Irrigation.c 
	@-${MV} ${OBJECTDIR}/_ext/398482839/Irrigation.d ${OBJECTDIR}/_ext/398482839/Irrigation.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/398482839/Irrigation.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/860197556/MyProject.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/APP/MyProject.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/860197556" 
	@${RM} ${OBJECTDIR}/_ext/860197556/MyProject.p1.d 
	@${RM} ${OBJECTDIR}/_ext/860197556/MyProject.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/860197556/MyProject.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/APP/MyProject.c 
	@-${MV} ${OBJECTDIR}/_ext/860197556/MyProject.d ${OBJECTDIR}/_ext/860197556/MyProject.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/860197556/MyProject.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1148429997/Safety.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/APP/Safety/Safety.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1148429997" 
	@${RM} ${OBJECTDIR}/_ext/1148429997/Safety.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1148429997/Safety.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1148429997/Safety.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/APP/Safety/Safety.c 
	@-${MV} ${OBJECTDIR}/_ext/1148429997/Safety.d ${OBJECTDIR}/_ext/1148429997/Safety.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1148429997/Safety.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/_ext/2029219237/Button.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Button/Button.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/2029219237" 
	@${RM} ${OBJECTDIR}/_ext/2029219237/Button.p1.d 
	@${RM} ${OBJECTDIR}/_ext/2029219237/Button.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/2029219237/Button.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Button/Button.c 
	@-${MV} ${OBJECTDIR}/_ext/2029219237/Button.d ${OBJECTDIR}/_ext/2029219237/Button.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/2029219237/Button.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/2029035031/Buzzer.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Buzzer/Buzzer.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/2029035031" 
	@${RM} ${OBJECTDIR}/_ext/2029035031/Buzzer.p1.d 
	@${RM} ${OBJECTDIR}/_ext/2029035031/Buzzer.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/2029035031/Buzzer.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Buzzer/Buzzer.c 
	@-${MV} ${OBJECTDIR}/_ext/2029035031/Buzzer.d ${OBJECTDIR}/_ext/2029035031/Buzzer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/2029035031/Buzzer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1890666640/Current.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Current/Current.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1890666640" 
	@${RM} ${OBJECTDIR}/_ext/1890666640/Current.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1890666640/Current.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1890666640/Current.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Current/Current.c 
	@-${MV} ${OBJECTDIR}/_ext/1890666640/Current.d ${OBJECTDIR}/_ext/1890666640/Current.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1890666640/Current.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1957042314/Fan.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Fan/Fan.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1957042314" 
	@${RM} ${OBJECTDIR}/_ext/1957042314/Fan.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1957042314/Fan.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1957042314/Fan.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Fan/Fan.c 
	@-${MV} ${OBJECTDIR}/_ext/1957042314/Fan.d ${OBJECTDIR}/_ext/1957042314/Fan.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1957042314/Fan.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1491501468/Humidity.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Humidity/Humidity.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1491501468" 
	@${RM} ${OBJECTDIR}/_ext/1491501468/Humidity.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1491501468/Humidity.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1491501468/Humidity.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Humidity/Humidity.c 
	@-${MV} ${OBJECTDIR}/_ext/1491501468/Humidity.d ${OBJECTDIR}/_ext/1491501468/Humidity.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1491501468/Humidity.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1957047108/LCD.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/LCD/LCD.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1957047108" 
	@${RM} ${OBJECTDIR}/_ext/1957047108/LCD.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1957047108/LCD.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1957047108/LCD.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/LCD/LCD.c 
	@-${MV} ${OBJECTDIR}/_ext/1957047108/LCD.d ${OBJECTDIR}/_ext/1957047108/LCD.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1957047108/LCD.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1957047170/LED.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/LED/LED.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1957047170" 
	@${RM} ${OBJECTDIR}/_ext/1957047170/LED.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1957047170/LED.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1957047170/LED.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/LED/LED.c 
	@-${MV} ${OBJECTDIR}/_ext/1957047170/LED.d ${OBJECTDIR}/_ext/1957047170/LED.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1957047170/LED.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/471120852/Motor.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Motor/Motor.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/471120852" 
	@${RM} ${OBJECTDIR}/_ext/471120852/Motor.p1.d 
	@${RM} ${OBJECTDIR}/_ext/471120852/Motor.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/471120852/Motor.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Motor/Motor.c 
	@-${MV} ${OBJECTDIR}/_ext/471120852/Motor.d ${OBJECTDIR}/_ext/471120852/Motor.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/471120852/Motor.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/466809272/Relay.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Relay/Relay.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/466809272" 
	@${RM} ${OBJECTDIR}/_ext/466809272/Relay.p1.d 
	@${RM} ${OBJECTDIR}/_ext/466809272/Relay.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/466809272/Relay.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Relay/Relay.c 
	@-${MV} ${OBJECTDIR}/_ext/466809272/Relay.d ${OBJECTDIR}/_ext/466809272/Relay.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/466809272/Relay.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/539170280/Soil.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Soil/Soil.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/539170280" 
	@${RM} ${OBJECTDIR}/_ext/539170280/Soil.p1.d 
	@${RM} ${OBJECTDIR}/_ext/539170280/Soil.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/539170280/Soil.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Soil/Soil.c 
	@-${MV} ${OBJECTDIR}/_ext/539170280/Soil.d ${OBJECTDIR}/_ext/539170280/Soil.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/539170280/Soil.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1086581739/Temp.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Temperature/Temp.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1086581739" 
	@${RM} ${OBJECTDIR}/_ext/1086581739/Temp.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1086581739/Temp.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1086581739/Temp.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Temperature/Temp.c 
	@-${MV} ${OBJECTDIR}/_ext/1086581739/Temp.d ${OBJECTDIR}/_ext/1086581739/Temp.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1086581739/Temp.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1723092823/Ultrasonic.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Ultrasonic/Ultrasonic.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1723092823" 
	@${RM} ${OBJECTDIR}/_ext/1723092823/Ultrasonic.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1723092823/Ultrasonic.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1723092823/Ultrasonic.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/HAL/Ultrasonic/Ultrasonic.c 
	@-${MV} ${OBJECTDIR}/_ext/1723092823/Ultrasonic.d ${OBJECTDIR}/_ext/1723092823/Ultrasonic.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1723092823/Ultrasonic.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/2145928993/ADC.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/ADC/ADC.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/2145928993" 
	@${RM} ${OBJECTDIR}/_ext/2145928993/ADC.p1.d 
	@${RM} ${OBJECTDIR}/_ext/2145928993/ADC.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/2145928993/ADC.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/ADC/ADC.c 
	@-${MV} ${OBJECTDIR}/_ext/2145928993/ADC.d ${OBJECTDIR}/_ext/2145928993/ADC.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/2145928993/ADC.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/2099098800/GPIO.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/GPIO/GPIO.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/2099098800" 
	@${RM} ${OBJECTDIR}/_ext/2099098800/GPIO.p1.d 
	@${RM} ${OBJECTDIR}/_ext/2099098800/GPIO.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/2099098800/GPIO.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/GPIO/GPIO.c 
	@-${MV} ${OBJECTDIR}/_ext/2099098800/GPIO.d ${OBJECTDIR}/_ext/2099098800/GPIO.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/2099098800/GPIO.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1111239760/Interrupt_Manager.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/Interrupt_Manager/Interrupt_Manager.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1111239760" 
	@${RM} ${OBJECTDIR}/_ext/1111239760/Interrupt_Manager.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1111239760/Interrupt_Manager.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1111239760/Interrupt_Manager.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/Interrupt_Manager/Interrupt_Manager.c 
	@-${MV} ${OBJECTDIR}/_ext/1111239760/Interrupt_Manager.d ${OBJECTDIR}/_ext/1111239760/Interrupt_Manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1111239760/Interrupt_Manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1766508780/Timer0.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/TIMER0/Timer0.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1766508780" 
	@${RM} ${OBJECTDIR}/_ext/1766508780/Timer0.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1766508780/Timer0.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1766508780/Timer0.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/TIMER0/Timer0.c 
	@-${MV} ${OBJECTDIR}/_ext/1766508780/Timer0.d ${OBJECTDIR}/_ext/1766508780/Timer0.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1766508780/Timer0.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/634542204/USART.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/USART/USART.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/634542204" 
	@${RM} ${OBJECTDIR}/_ext/634542204/USART.p1.d 
	@${RM} ${OBJECTDIR}/_ext/634542204/USART.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/634542204/USART.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/MCAL/USART/USART.c 
	@-${MV} ${OBJECTDIR}/_ext/634542204/USART.d ${OBJECTDIR}/_ext/634542204/USART.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/634542204/USART.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1992713099/Delay.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/SERVICES/Delay.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1992713099" 
	@${RM} ${OBJECTDIR}/_ext/1992713099/Delay.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1992713099/Delay.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1992713099/Delay.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/SERVICES/Delay.c 
	@-${MV} ${OBJECTDIR}/_ext/1992713099/Delay.d ${OBJECTDIR}/_ext/1992713099/Delay.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1992713099/Delay.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1085684820/Comms.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/APP/Comms/Comms.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1085684820" 
	@${RM} ${OBJECTDIR}/_ext/1085684820/Comms.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1085684820/Comms.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1085684820/Comms.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/APP/Comms/Comms.c 
	@-${MV} ${OBJECTDIR}/_ext/1085684820/Comms.d ${OBJECTDIR}/_ext/1085684820/Comms.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1085684820/Comms.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/398482839/Irrigation.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/APP/Irrigation/Irrigation.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/398482839" 
	@${RM} ${OBJECTDIR}/_ext/398482839/Irrigation.p1.d 
	@${RM} ${OBJECTDIR}/_ext/398482839/Irrigation.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/398482839/Irrigation.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/APP/Irrigation/Irrigation.c 
	@-${MV} ${OBJECTDIR}/_ext/398482839/Irrigation.d ${OBJECTDIR}/_ext/398482839/Irrigation.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/398482839/Irrigation.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/860197556/MyProject.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/APP/MyProject.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/860197556" 
	@${RM} ${OBJECTDIR}/_ext/860197556/MyProject.p1.d 
	@${RM} ${OBJECTDIR}/_ext/860197556/MyProject.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/860197556/MyProject.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/APP/MyProject.c 
	@-${MV} ${OBJECTDIR}/_ext/860197556/MyProject.d ${OBJECTDIR}/_ext/860197556/MyProject.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/860197556/MyProject.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1148429997/Safety.p1: C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/APP/Safety/Safety.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/1148429997" 
	@${RM} ${OBJECTDIR}/_ext/1148429997/Safety.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1148429997/Safety.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/_ext/1148429997/Safety.p1 C:/Users/Abdullah/OneDrive/Documents/Embedded_Systems/Project_2/smart-irrigation-pic-ana-w-body/pic-firmware/APP/Safety/Safety.c 
	@-${MV} ${OBJECTDIR}/_ext/1148429997/Safety.d ${OBJECTDIR}/_ext/1148429997/Safety.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1148429997/Safety.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/Project_2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/Project_2.X.${IMAGE_TYPE}.map  -D__DEBUG=1  -mdebugger=none  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto        $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/Project_2.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} ${DISTDIR}/Project_2.X.${IMAGE_TYPE}.hex 
	
	
else
${DISTDIR}/Project_2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/Project_2.X.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -O1 -fasmfile -maddrqual=ignore -xassembler-with-cpp -I".." -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -mno-default-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/Project_2.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
	
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

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
