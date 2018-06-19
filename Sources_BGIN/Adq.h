/*
 * Adq.h
 *
 *  Created on: Jul 28, 2014
 *      Author: ATE03
 */

#ifndef ADQ_H_
#define ADQ_H_

#include "Events.h"
#include "AS1.h"
#include "OE0.h"
#include "OE1.h"
#include "DataReady0.h"
#include "Sync.h"
#include "Protocolo.h"
//#include "Bit1.h"
#include "SCLK.h"
#include "Sync.h"
#include "DataReady0.h"
#include "ADCData.h"
#include "OE0.h"
#include "OE1.h"
#include "CSIC1.h"
#include "CSIC2.h"
#include "SM1.h"
//#include "Bit1.h"
#include "RefreshScreen.h"
#include "CS0.h"
//#include "CS1.h"
//#include "CS2.h"
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Events.h"
#include "Cpu.h"
#include "IO_Map.h"
#include "Alarmas.h"

//#include "TimeSCLK.h"
//#include "SCLK.h"
#include "SCLK.h"

#define AIRE1	0
#define AIRE2	1
#define AIRE3	2
#define OXIGENO	3
#define TEMPH	4
#define HUMEDAD	5
#define PIEL1	6
#define PIEL2	7

#define TEMP1 0
#define TEMP2 1
#define FILTER_DEPTH 128
#define FILTMASK FILTER_DEPTH-1

#define FACTOR_LM35 625
#define FACTOR_PIEL 500

void Lecturas_clk(void);
void Lecturas_fin(void);
void Pedir_Valor_Adq(byte *pos, byte req);
int16_t Adq_Val(byte selec,byte esp);
void Adq_alarmas(void);
uint16_t Adq_Alarms(void);
void Adq_SetFalla(byte set);
void Adq_ClrFalla(byte set);
void zerobasc(void);
void calibbasc(void);
byte Adq_Test(void);
void Adq_SensPot(byte *rec);
void Adq_ChkSenAmb(void);
#endif /* ADQ_H_ */




