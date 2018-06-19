/*
 * Incubadora.h
 *
 *  Created on: Sep 3, 2015
 *      Author: ATE
 */

#ifndef INCUBADORA_H_
#define INCUBADORA_H_
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

#include "Cpu.h"
#include "Events.h"
#include "AS1.h"

#include "OE0.h"
#include "OE1.h"
#include "DataReady0.h"
#include "Sync.h"
#include "Protocolo.h"
#include "SCLK.h"
#include "SPIdev.h"
#include "ACInt.h"

#define CALOK  		1
#define MOTREC 		2
#define CALCONEC	4

void Inc_Test(void);
void Inc_Rec(byte rc);
void Inc_Error(void);
void Inc_start(void);
void Inc_SetError(byte err);

#endif /* INCUBADORA_H_ */
