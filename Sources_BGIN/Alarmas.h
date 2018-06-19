/*
 * Alarmas.h
 *
 *  Created on: Aug 23, 2015
 *      Author: ATE
 */

#ifndef ALARMAS_H_
#define ALARMAS_H_

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

#define	TMP_AIRE_BAJA	0x0002
#define	OX_BAJO			0x0400
#define	HUM_BAJA		0x1000
#define	PIEL_BAJA		0x0008

#define	TMP_AIRE_ALTA	0x0001
#define	OX_ALTO			0x0200
#define	HUM_ALTA		0x0800
#define	PIEL_ALTA		0x0004
#define	CALEF_ALTA		0x2000

#define F_CALEF			0x0001
#define F_HUMID			0x0002
#define F_MOTOR			0x0004
#define F_VOXIG			0x0008
#define F_SNAIR			0x0010
#define F_SNPIE			0x0020
#define F_SNOXI			0x0040
#define F_SNHUM			0x0080
#define F_SNTEH			0x0100
#define F_SUMEL			0x0200
#define F_CIRAI			0x0400
#define F_SYSTEM		0x0800

void Ala_SetAviso(word alw);
void Ala_SetError(word alw);
void Ala_SetDesv(word alw);
void Ala_SetAlarma(word alw);
void Ala_SetFalla(word alw);

void Ala_ClrAviso(word alw);
void Ala_ClrError(word alw);
void Ala_ClrDesv(word alw);
void Ala_ClrAlarma(word alw);
void Ala_ClrFalla(word alw);

void Ala_SendAviso(void);
void Ala_SendError(void);
void Ala_SendDesv(void);
void Ala_SendAla(void);
void Ala_SendFalla(void);
word Ala_GetFalla(char tg);
void Ala_Heater(byte stat);
void Ala_Bus(byte type, byte stat);

#endif /* ALARMAS_H_ */
