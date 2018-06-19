/** ###################################################################
**     Filename  : ProcessorExpert.c
**     Project   : ProcessorExpert
**     Processor : MCF5213CAF80
**     Version   : Driver 01.00
**     Compiler  : CodeWarrior MCF C Compiler
**     Date/Time : 2015-01-08, 16:54, # CodeGen: 710
**     Abstract  :
**         Main module.
**         This module contains user's application code.
**     Settings  :
**     Contents  :
**         No public methods
**
** ###################################################################*/
/* MODULE ProcessorExpert */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
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
#include "Potencia.h"
#include "SPIdev.h"
#include "Protocolo.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */


 void main(void){
  /* Write your local variable definition here */
byte dect,ret;
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/
  Bus_Enable();
  AS1_Enable();
  Rep_Enable();
  BusEn_ClrVal();
  
  Cpu_Delay100US(0xfff);
  Prot_LedInit();
  SPI_InitDev();
  //Pot_MotorOff(); 
  //setcont(350,15);
  //IncOff();
  SPI_ReadMem(0,1);
  Pot_HeaterEn(1);
  //IncTestPassed();
  WD_En_ClrVal();
  
  Lecturas_fin();
  for(;;){
    	  SCLK_SetVal();
    	  dect++;
    	  SCLK_ClrVal();
    	  Sync_SetVal();
    	  while(DataReady0_GetVal() || DataReady1_GetVal())
    		  ;
    	  Lecturas_clk();
      }

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END ProcessorExpert */
/*
** ###################################################################
**
**     This file was created by Processor Expert 5.00 [04.48]
**     for the Freescale MCF series of microcontrollers.
**
** ###################################################################
*/
