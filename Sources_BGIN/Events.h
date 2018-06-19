/** ###################################################################
**     Filename  : Events.h
**     Project   : ProcessorExpert
**     Processor : MCF5213CAF80
**     Component : Events
**     Compiler  : CodeWarrior MCF C Compiler
**     Date/Time : 2015-01-08, 16:54, # CodeGen: 710
**     Abstract  :
**         This is user's event module.
**         Put your event handler code here.
**     Settings  :
**     Contents  :
**         ACTim_OnInterrupt           - void ACTim_OnInterrupt(void);
**         Rep_OnError               - void Rep_OnError(void);
**         Rep_OnRxChar              - void Rep_OnRxChar(void);
**         Rep_OnTxChar              - void Rep_OnTxChar(void);
**         Rep_OnFullRxBuf           - void Rep_OnFullRxBuf(void);
**         Rep_OnFreeTxBuf           - void Rep_OnFreeTxBuf(void);
**         RefreshScreen_OnInterrupt - void RefreshScreen_OnInterrupt(void);
**         SM1_OnRxChar              - void SM1_OnRxChar(void);
**         SM1_OnTxChar              - void SM1_OnTxChar(void);
**         SM1_OnFullRxBuf           - void SM1_OnFullRxBuf(void);
**         SM1_OnFreeTxBuf           - void SM1_OnFreeTxBuf(void);
**         SM1_OnError               - void SM1_OnError(void);
**         AS1_OnError               - void AS1_OnError(void);
**         AS1_OnRxChar              - void AS1_OnRxChar(void);
**         AS1_OnTxChar              - void AS1_OnTxChar(void);
**         AS1_OnFullRxBuf           - void AS1_OnFullRxBuf(void);
**         AS1_OnFreeTxBuf           - void AS1_OnFreeTxBuf(void);
**         Cpu_OnAddrRelatedEXP      - void Cpu_OnAddrRelatedEXP(byte ExceptionType);
**         Cpu_OnInstrRelatedEXP     - void Cpu_OnInstrRelatedEXP(byte ExceptionType);
**         Cpu_OnTrapEXP             - void Cpu_OnTrapEXP(byte ExceptionType);
**         Cpu_OnIntRelatedEXP       - void Cpu_OnIntRelatedEXP(byte ExceptionType);
**         Cpu_OnDivideByZeroEXP     - void Cpu_OnDivideByZeroEXP(void);
**         Cpu_OnLvdINT              - void Cpu_OnLvdINT(void);
**         Cpu_OnCoreWatchdogINT     - void Cpu_OnCoreWatchdogINT(void);
**
** ###################################################################*/

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PE_Timer.h"
#include "AS1.h"
#include "SCLK.h"
#include "Sync.h"
#include "DataReady0.h"
#include "ADCData.h"
#include "OE0.h"
#include "OE1.h"
#include "CSIC1.h"
#include "CSIC2.h"
#include "SM1.h"
#include "RefreshScreen.h"
#include "CS0.h"
#include "Rep.h"
#include "ACTim.h"
#include "CSMem.h"
#include "Bus.h"
#include "BusEn.h"
#include "Lktim.h"
#include "ADCData2.h"
#include "DataReady1.h"
#include "InitTestTim.h"
#include "ACSync.h"
#include "ACInt.h"
#include "ACErrTim.h"
#include "BusErrTime.h"
#include "ADSel.h"
#include "SenEn.h"
#include "FC1.h"
#include "WD_Freq.h"
#include "WD_En.h"

#include "SPIdev.h"
#include "Protocolo.h"

void ACTim_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  ACTim_OnInterrupt (module Events)
**
**     Component   :  ACTim [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void Rep_OnError(void);
/*
** ===================================================================
**     Event       :  Rep_OnError (module Events)
**
**     Component   :  Rep [AsynchroSerial]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void Rep_OnRxChar(void);
/*
** ===================================================================
**     Event       :  Rep_OnRxChar (module Events)
**
**     Component   :  Rep [AsynchroSerial]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the <Receiver>
**         property is enabled or the <SCI output mode> property (if
**         supported) is set to Single-wire mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void Rep_OnTxChar(void);
/*
** ===================================================================
**     Event       :  Rep_OnTxChar (module Events)
**
**     Component   :  Rep [AsynchroSerial]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void Rep_OnFullRxBuf(void);
/*
** ===================================================================
**     Event       :  Rep_OnFullRxBuf (module Events)
**
**     Component   :  Rep [AsynchroSerial]
**     Description :
**         This event is called when the input buffer is full;
**         i.e. after reception of the last character 
**         that was successfully placed into input buffer.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void Rep_OnFreeTxBuf(void);
/*
** ===================================================================
**     Event       :  Rep_OnFreeTxBuf (module Events)
**
**     Component   :  Rep [AsynchroSerial]
**     Description :
**         This event is called after the last character in output
**         buffer is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void RefreshScreen_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  RefreshScreen_OnInterrupt (module Events)
**
**     Component   :  RefreshScreen [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void SM1_OnRxChar(void);
/*
** ===================================================================
**     Event       :  SM1_OnRxChar (module Events)
**
**     Component   :  SM1 [SynchroMaster]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void SM1_OnTxChar(void);
/*
** ===================================================================
**     Event       :  SM1_OnTxChar (module Events)
**
**     Component   :  SM1 [SynchroMaster]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void SM1_OnFullRxBuf(void);
/*
** ===================================================================
**     Event       :  SM1_OnFullRxBuf (module Events)
**
**     Component   :  SM1 [SynchroMaster]
**     Description :
**         This event is called when the input buffer is full, i.e.
**         after reception of the last character that was successfully
**         placed into input buffer.
**         This event is available only when the <Interrupt
**         service/event> property is enabled and the <Input buffer
**         size> property is set to non-zero value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void SM1_OnFreeTxBuf(void);
/*
** ===================================================================
**     Event       :  SM1_OnFreeTxBuf (module Events)
**
**     Component   :  SM1 [SynchroMaster]
**     Description :
**         This event is called after the last character in output
**         buffer is transmitted.
**         This event is available only when the <Interrupt
**         service/event> property is enabled and the <Output buffer
**         size> property is set to non-zero value.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void SM1_OnError(void);
/*
** ===================================================================
**     Event       :  SM1_OnError (module Events)
**
**     Component   :  SM1 [SynchroMaster]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void AS1_OnError(void);
/*
** ===================================================================
**     Event       :  AS1_OnError (module Events)
**
**     Component   :  AS1 [AsynchroSerial]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void AS1_OnRxChar(void);
/*
** ===================================================================
**     Event       :  AS1_OnRxChar (module Events)
**
**     Component   :  AS1 [AsynchroSerial]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the <Receiver>
**         property is enabled or the <SCI output mode> property (if
**         supported) is set to Single-wire mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void AS1_OnTxChar(void);
/*
** ===================================================================
**     Event       :  AS1_OnTxChar (module Events)
**
**     Component   :  AS1 [AsynchroSerial]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void AS1_OnFullRxBuf(void);
/*
** ===================================================================
**     Event       :  AS1_OnFullRxBuf (module Events)
**
**     Component   :  AS1 [AsynchroSerial]
**     Description :
**         This event is called when the input buffer is full;
**         i.e. after reception of the last character 
**         that was successfully placed into input buffer.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void AS1_OnFreeTxBuf(void);
/*
** ===================================================================
**     Event       :  AS1_OnFreeTxBuf (module Events)
**
**     Component   :  AS1 [AsynchroSerial]
**     Description :
**         This event is called after the last character in output
**         buffer is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void Cpu_OnAddrRelatedEXP(byte ExceptionType);
/*
** ===================================================================
**     Event       :  Cpu_OnAddrRelatedEXP (module Events)
**
**     Component   :  Cpu [MCF5212_64_LQFP]
**     Description :
**         This event is called when one of the address related
**         exceptions occurs. This event is automatically enabled when
**         the <Address related reset> property is disabled and at
**         least one of the <Address related exceptions> is enabled.
**         Disabling this event also disables all the <Address related
**         exceptions>.
**     Parameters  :
**         NAME            - DESCRIPTION
**         ExceptionType   - This parameter
**                           specifies which exception caused this event.
**                           Possible values are:
**                           - ACCESS_ERROR - Access Error Exception 
**                           - ADDRESS_ERROR - Address Error Exception 
**                           - RTE_FORMAT_ERROR - RTE Format Error
**                           Exception
**     Returns     : Nothing
** ===================================================================
*/

void Cpu_OnInstrRelatedEXP(byte ExceptionType);
/*
** ===================================================================
**     Event       :  Cpu_OnInstrRelatedEXP (module Events)
**
**     Component   :  Cpu [MCF5212_64_LQFP]
**     Description :
**         This event is called when one of the instruction related
**         exceptions occurs. This event is automatically enabled when
**         the <Instruction related reset> property is disabled and at
**         least one of the <Instruction related exceptions> is enabled.
**         Disabling this event also disables all the <Access related
**         exceptions>.
**     Parameters  :
**         NAME            - DESCRIPTION
**         ExceptionType   - This parameter
**                           specifies which exception caused this event.
**                           Possible values are:
**                           - ILLEGAL_INSTRUCTION - Illegal Instruction
**                           Exception 
**                           - UNIMPL_LINEA - Unimplemented Line-A
**                           Opcode Exception 
**                           - UNIMPL_LINEF - Unimplemented Line-F
**                           Opcode Exception 
**                           - PRIVILEDGE_VIOL - Privilege Violation
**                           Exception
**     Returns     : Nothing
** ===================================================================
*/

void Cpu_OnTrapEXP(byte ExceptionType);
/*
** ===================================================================
**     Event       :  Cpu_OnTrapEXP (module Events)
**
**     Component   :  Cpu [MCF5212_64_LQFP]
**     Description :
**         This event is called when one of the trap exceptions occurs.
**         This event is automatically enabled when at least one of the
**         <Trap exceptions> is enabled. Disabling this event also
**         disables all the <Trap exceptions>.
**     Parameters  :
**         NAME            - DESCRIPTION
**         ExceptionType   - This parameter
**                           specifies which trap exception caused this
**                           event. Possible values are:
**                           - TRAP_0 - Trap exception #0 
**                           - TRAP_1 - Trap exception #1 
**                           - ... 
**                           - TRAP_15 - Trap exception #15
**     Returns     : Nothing
** ===================================================================
*/

void Cpu_OnIntRelatedEXP(byte ExceptionType);
/*
** ===================================================================
**     Event       :  Cpu_OnIntRelatedEXP (module Events)
**
**     Component   :  Cpu [MCF5212_64_LQFP]
**     Description :
**         This event is called when one of the interrupt related
**         exceptions occurs. This event is automatically enabled when
**         the <Interrupt related reset> property is disabled and at
**         least one of the <Interrupt related exceptions> is enabled.
**         Disabling this event also disables all the <Interrupt
**         related exceptions>.
**     Parameters  :
**         NAME            - DESCRIPTION
**         ExceptionType   - This parameter
**                           specifies which exception caused this event.
**                           Possible values are:
**                           - SPURIOUS_INTERRUPT - Spurious Interrupt
**                           Exception 
**                           - UNINITIALIZED_INTERRUPT - Uninitialized
**                           Interrupt Exception
**     Returns     : Nothing
** ===================================================================
*/

void Cpu_OnDivideByZeroEXP(void);
/*
** ===================================================================
**     Event       :  Cpu_OnDivideByZeroEXP (module Events)
**
**     Component   :  Cpu [MCF5212_64_LQFP]
**     Description :
**         This event is called when the divide by zero exception
**         occurs. This event can be enabled only if the
**         <Divide-By-Zero> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void Cpu_OnLvdINT(void);
/*
** ===================================================================
**     Event       :  Cpu_OnLvdINT (module Events)
**
**     Component   :  Cpu [MCF5212_64_LQFP]
**     Description :
**         This event is called when the Low voltage interrupt had
**         occurred. This event is automatically enabled when the <LVD
**         module> and <LVD interrupt> properties are set to 'Enabled'.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void Cpu_OnCoreWatchdogINT(void);
/*
** ===================================================================
**     Event       :  Cpu_OnCoreWatchdogINT (module Events)
**
**     Component   :  Cpu [MCF5212_64_LQFP]
**     Description :
**         This event is called when the OnCoreWatchdog interrupt had
**         occurred. This event is automatically enabled when the <Mode>
**         is set to 'Interrupt'.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/


void Bus_OnError(void);
/*
** ===================================================================
**     Event       :  Bus_OnError (module Events)
**
**     Component   :  Bus [AsynchroSerial]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void Bus_OnRxChar(void);
/*
** ===================================================================
**     Event       :  Bus_OnRxChar (module Events)
**
**     Component   :  Bus [AsynchroSerial]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the <Receiver>
**         property is enabled or the <SCI output mode> property (if
**         supported) is set to Single-wire mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void Bus_OnTxChar(void);
/*
** ===================================================================
**     Event       :  Bus_OnTxChar (module Events)
**
**     Component   :  Bus [AsynchroSerial]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void Bus_OnFullRxBuf(void);
/*
** ===================================================================
**     Event       :  Bus_OnFullRxBuf (module Events)
**
**     Component   :  Bus [AsynchroSerial]
**     Description :
**         This event is called when the input buffer is full;
**         i.e. after reception of the last character 
**         that was successfully placed into input buffer.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void Bus_OnFreeTxBuf(void);
/*
** ===================================================================
**     Event       :  Bus_OnFreeTxBuf (module Events)
**
**     Component   :  Bus [AsynchroSerial]
**     Description :
**         This event is called after the last character in output
**         buffer is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void Lktim_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  Lktim_OnInterrupt (module Events)
**
**     Component   :  Lktim [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void InitTestTim_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  InitTestTim_OnInterrupt (module Events)
**
**     Component   :  InitTestTim [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void ACInt_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  ACInt_OnInterrupt (module Events)
**
**     Component   :  ACInt [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void ACErrTim_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  ACErrTim_OnInterrupt (module Events)
**
**     Component   :  ACErrTim [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void BusErrTime_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  BusErrTime_OnInterrupt (module Events)
**
**     Component   :  BusErrTime [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void FC1_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  FC1_OnInterrupt (module Events)
**
**     Component   :  FC1 [FreeCntr]
*/
/*!
**     @brief
**         This event is called when a compare matches the counter
**         value (if compare or reload is selected as a interrupt
**         source) or a counter overflows (for free-running devices).
**         It is valid only when the component is enabled - <"Enable">
**         and the events are enabled - <"EnableEvent">. The event is
**         available only if <Interrupt service/event> is enabled.
*/
/* ===================================================================*/

/* END Events */
#endif /* __Events_H*/

/*
** ###################################################################
**
**     This file was created by Processor Expert 5.00 [04.48]
**     for the Freescale MCF series of microcontrollers.
**
** ###################################################################
*/
