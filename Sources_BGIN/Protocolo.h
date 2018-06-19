/*
 * Funciones de protocolo
 * */

#ifndef __Protocolo_H
#define __Protocolo_H
/* MODULE Events */

//#include "Bit1.h"
#include "AS1.h"
#include "Adq.h"
#include "SPIdev.h"
#include "Potencia.h"

#include "Events.h"
#include "Cpu.h"
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
//#include "Bit1.h"
#include "RefreshScreen.h"
#include "CS0.h"
//#include "CS1.h"
//#include "CS2.h"
#include "Control.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "Incubadora.h"
#include "Potencia.h"


#define MY_NAME 'A'

#define STX 0x02
#define ETX 0x03
#define ACK 0x06

#define RXSTD 1

#define DEST  1
#define CMND1 2
#define CMND2 3

#define DECIMALES 2

/* declaración de funciones*/

void Prot_Recibir(void);
void Vis_SendNext(void);
void Vis_SendBlock(byte *buf, byte qty);
void Prot_ChecarDir(void);
void Prot_Refresh(void);
void Prot_CorTrama(byte tosd);
void Prot_Report(void);
void Prot_Date(void);
void Prot_SetAlarmsPot(byte edo);
void Prot_BusRec(void);
void Bus_WaitAnsw(byte from);
void Prot_BusNoAnsw(void);
void Prot_mot(byte datos[]);
void Prot_LedsBus(byte);
void Prot_bots(byte datos[]);
void Prot_LedInit(void);
void Prot_BufEmpty(void);
void Prot_TxChar(void);
void Prot_lock(void);
void IncOff(void);
void IncOn(void);
void IncTestPassed(void);
void Prot_Alarm(byte tipo, word valor);
void Prot_MotSetVel(byte vel,byte lock);
void Prot_MotAct();
void Prot_Motor(byte vel);
void Prot_Puertas(byte edopts);
void Prot_SilAlarm(void);
void Prot_Pte(byte cfg);
void Prot_DispOn(void);
void Prot_DispOff(void);
void Prot_LcdForm(byte form, byte type);
void Prot_LVD(void);
#endif
