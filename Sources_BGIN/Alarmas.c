/*
 * Alarmas.c
 *
 *  Created on: Aug 23, 2015
 *      Author: ATE
 */
#include "Alarmas.h"
word av,er,de,al,fa; 
word avp,erp,dep,alp,fap;

void Ala_SetAviso(word alw)	{av |= alw;}
void Ala_SetError(word alw)	{er |= alw;}
void Ala_SetDesv(word alw)	{de |= alw;}
void Ala_SetAlarma(word alw){al |= alw;}
void Ala_SetFalla(word alw)	{fa |= alw;}
void Ala_ClrAviso(word alw)	{av &= ~alw;}
void Ala_ClrError(word alw)	{er &= ~alw;}
void Ala_ClrDesv(word alw)	{de &= ~alw;}
void Ala_ClrAlarma(word alw){al &= ~alw;}
void Ala_ClrFalla(word alw)	{fa &= ~alw;}
void Ala_SendAviso(void){
	//if (av != avp)
		Prot_Alarm('A',av);
	avp =av;
}
void Ala_SendError(void){
	//if (er != erp)
		Prot_Alarm('E',er);
	erp =er;
}
void Ala_SendDesv(void){
	//if (de != dep)
		Prot_Alarm('D',de);
	dep =de;
}
void Ala_SendAla(void){
	//if (al != alp)
		Prot_Alarm('L',al);
	alp =al;
}
void Ala_SendFalla(void){
	//if (fa != fap)
		//Prot_Alarm('F',fa); // Habilitar linea para habilitar falla
	fap =fa;
}
word Ala_GetFalla(char tg){
	return fa;
}
void Ala_Heater(byte stat){
	if (stat)
		Ala_SetFalla(1);
	else
		Ala_ClrFalla(1);
		
}
void Ala_Bus(byte type, byte stat){
	if (stat) Ala_SetFalla(1);
	else Ala_ClrFalla(1);
}
