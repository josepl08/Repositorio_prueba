/*
 * Control.h
 *
 *  Created on: Nov 2, 2014
 *      Author: AMG
 */

#ifndef CONTROL_H_
#define CONTROL_H_

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
//#include "Rep.h"

void control(void);
void setcont(int16_t setpoint, byte act, bool save);
void Cont_SetInt(byte *intdir);
void Cont_report(void);
void SP_HumOx(uint16_t setpoint, byte act, bool save);

#endif /* CONTROL_H_ */
