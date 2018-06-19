/*
 * Incubadora.c
 *
 *  Created on: Sep 3, 2015
 *      Author: ATE
 */
#include "Incubadora.h"
byte tct=0;
byte tatpt;
word heatini;
word inctst;
word heattst;

void Inc_Test(void){
word res;
	tct=10; // QUITAR PARA HABILITAR AUTOTEST
	switch(tct){
	case 0:
		tatpt++;
		if (tatpt > 5){
			tct++;
			tatpt=0;
			inctst=0;
			Prot_LcdForm(0x10,0x31);
		}
	break;
	case 1:
		Prot_Motor(10);
		tct++;
		//inctst |= MOTREC; // quitar para prueba motor
	break;
	case 2:
		if(inctst & MOTREC)
			tct++;
		else{
			tatpt++;
			if (tatpt > 25){
				Prot_LcdForm(0x10,0x81);
				tct=0xff;
				tatpt=0;
			}
		}
	break;
	case 3:
		Pot_Test(1);
		ACInt_Enable();
		tct++;
	break;
	case 4:
		if (inctst & 0x10){
			Prot_LcdForm(0x10,0x84);
			tct=0xff;
			tatpt=0;
		}
		else {
			tct++;
			Pot_Test(0);
		}
	break;
	case 5:
		Pot_GetACStat();
		tct++;
	break;
	case 6:
		if(inctst & CALCONEC){
			tct++;
			tatpt=0;
		}
		else{
			tatpt++;
			Pot_GetACStat();
			if (tatpt > 5){
				Prot_LcdForm(0x10,0x83);
				tct=0xff;
				tatpt=0;
			}
		}
	break;
	case 7:
		heatini=Adq_Val(11,0);
		Pot_HeaterOn();
		tct++;
	break;
	case 8:
		heattst=Adq_Val(11,0);
		if (heatini<heattst-15 || heattst > 300 ){
			Pot_HeaterOff();
			tatpt=0;
			tct++;
		}			
		else{
			tatpt++;
			if (tatpt > 25){
				Prot_LcdForm(0x10,0x80);
				tct=0xff;
				tatpt=0;
			}	
		}
	break;
	case 9:
		res=Adq_Test();
		if (res==20)
			tct++;
		else if (res==1){
			Prot_LcdForm(0x10,0x82);
			tct=0xff;
			tatpt=0;
		}
			
	break;
	case 10:
		IncTestPassed();
		Prot_LcdForm(0x30,0x30); // 0x10,0x41); //Para fin de autotest
	break;
	case 0xff:
		if (tatpt++ > 20){
			tct=0;
			Inc_Error();
		}
	break;
	}
}

void Inc_Rec(byte rc){
	inctst |= rc;
}
void Inc_Error(void){
	IncOff();
	InitTestTim_Disable();	
	tatpt=0;
	tct=0;
} 
void Inc_start(void){
	tct=0;
}
void Inc_SetError(byte err){
	inctst |= err<<4;
}

