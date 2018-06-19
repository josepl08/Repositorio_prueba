/*
 * Modulo Potencia y Alarmas en potencia
 * */

#include "Potencia.h"
byte potstal;
bool acthum=0;
bool actheat=0;
bool heattimerun=0;
byte heatpow,powtoshow,powreal;
byte heattime, heattimeset, heattosync;
byte acstat;

bool locksync, syncrond, onsync, heatstat, onsctest, heatonsc, onoctest;
byte testcnt,sccnt;
word testfree;
void Pot_GetAlarms(void){
	SPI_SendExp(EXPREAD1, 0x55, POTAC, 0);
	SPI_SendExp(EXPREAD1, 0x55, POTAC, RECIEVE);			// Doble Necesario
	SPI_SendExp(EXPREAD2, 0x55, POTAC, 0);
	SPI_SendExp(EXPREAD2, 0x55, POTAC, RECIEVE);
}
void Pot_HeaterEn(byte stheat){
	//SPI_SendExp(ENHEAT, stheat, POTAC,0);
	/*SPI_SendExp(0x2c, 1, 0,0);
	SPI_SendExp(0x2d, 1, 0,0);
	SPI_SendExp(0x2b, 1, 0,0);
	SPI_SendExp(0x2e, 1, 0,0);
	SPI_SendExp(0x2f, 1, 0,0);
	SPI_SendExp(0x30, 1, 0,0);
	SPI_SendExp(0x31, 1, 0,0);
	SPI_SendExp(0x32, 1, 0,0);*/
	SPI_SendExp(0x25, 1, 0,0);
}
void Pot_HeaterPow(byte setheat,byte powshow){
	//setheat=50;//<-Set fixed
	heatpow = 100-setheat;
	powreal = setheat;
	powtoshow=powshow;
}
void Pot_SetTime(void){
	if (powreal > 95){
		Pot_HeaterPulse();
	}
	else if(powreal > 4){
		ACTim_SetPeriodUS(78*heatpow);
		ACTim_Enable();
	}
	if(!onoctest) ACErrTim_Enable();
}
void Pot_HeaterSync(void){
	Pot_HeaterPulse();
	ACTim_Disable();
}
void Pot_HeaterPulse(void){
	//if (!heatonsc && !onoctest){
		Pot_HeaterOn();
		Pot_HeaterOff();
		//Pot_GetACStat();
		//Pot_Test(1);
	//}
}
void Pot_sync(void){
	acstat = ACSync_GetVal();
	if (acstat){
		onsync=0;
	}
}
void Pot_HeatStart(void){
	Pot_HeaterOff();
	heatstat=0;
	//heattimeset = heatpow;
	ACInt_Enable();
}
void Pot_HeaterOn(void){
	SPI_SendExp(0x24, 01, 0,0);
	//SPI_SendExp(0x28, 01, 0,0); // 0 <- POTAC para prueba Exp
	//SPI_SendExp(HEATER, ON, POTAC,0);
}
void Pot_HeaterOff(void){
	SPI_SendExp(0x24, 00, 0,0);
	//SPI_SendExp(0x28, 00, 0,0); // 0 <- POTAC para prueba Exp
	//SPI_SendExp(HEATER, OFF, POTAC,0);
}
void Pot_MotorOn(void){
	SPI_SendExp(MOTOR, ON, POTAC,0);
}
void Pot_MotorOff(void){
	SPI_SendExp(MOTOR, OFF, POTAC,0);
}
void Pot_Hum(byte sthum){
	//if (sthum != acthum){
		SPI_SendExp(0x27, sthum, 0,0);
		acthum = sthum;
	//}
}
void Pot_Column(byte bots){
	if (bots == 1){
		SPI_SendExp(COLDW1, ON, 0,0);
		SPI_SendExp(COLUP1, OFF, 0,0);
	}
	else if ( bots == 4 ){
		SPI_SendExp(COLUP1, ON, 0,0);
		SPI_SendExp(COLDW1, OFF, 0,0);
	}
	else{
		SPI_SendExp(COLUP1, OFF, 0,0);
		SPI_SendExp(COLDW1, OFF, 0,0);

	}
}
void Pot_ColumnComand(byte com){
	
}
void Pot_Oxy(byte stoxi){
	SPI_SendExp(OXIGEN, stoxi, POTCOX,0);
}
void Pot_GetPot(byte *pos,byte type){
	if (type){
		*pos = ',';
		*(pos+1) = powtoshow/10+'0';
		*(pos+2) = powtoshow%10+'0';
		*(pos+3) = ',';
		*(pos+4) = powreal/10+'0';
		*(pos+5) = powreal%10+'0';
		
	}
	else *pos = powtoshow;
	
}
void Pot_Tbg(byte tbg){
	switch(tbg){
		case TBG12N:
			SPI_SendExp(TBGDW1, OFF, POTTDPT,0);
			SPI_SendExp(TBGUP2, OFF, POTTDPT,0);
			SPI_SendExp(TBGUP1, ON, POTTDPT,0);
			SPI_SendExp(TBGDW2, ON, POTTDPT,0);
			break;
		case TBG12P:
			SPI_SendExp(TBGDW2, OFF, POTTDPT,0);
			SPI_SendExp(TBGUP1, OFF, POTTDPT,0);
			SPI_SendExp(TBGUP2, ON, POTTDPT,0);
			SPI_SendExp(TBGDW1, ON, POTTDPT,0);
			break;
		case TBG0:
			SPI_SendExp(TBGUP1, OFF, POTTDPT,0);
			SPI_SendExp(TBGUP2, OFF, POTTDPT,0);
			SPI_SendExp(TBGDW1, ON, POTTDPT,0);
			SPI_SendExp(TBGDW2, ON, POTTDPT,0);
			break;
		default:
			SPI_SendExp(TBGUP1, OFF, POTTDPT,0);
			SPI_SendExp(TBGUP2, OFF, POTTDPT,0);
			SPI_SendExp(TBGDW1, ON, POTTDPT,0);
			SPI_SendExp(TBGDW2, ON, POTTDPT,0);
			break;
	}
	
}
void Pot_ACIntH(void){
	if (onsctest){
		Pot_HeaterOn();
		Pot_HeaterOff();
		Pot_GetACStat();
		ACInt_Disable();
	}
}
void Pot_GetACStat(void){
	SPI_SendExp(0x8E, 0x55, POTAC, 0);
	SPI_SendExp(0x8E, 0x55, POTAC, RECIEVE);
}
void Pot_Test(byte stat){
	onsctest=stat;
}
/*Función para recibir datos de la tarjeta de potencia*/
void Pot_Rec(byte rec){ //
	// Estas funciones pasan al ADC DEL uC
	/*if (onsctest){
		if(rec & 0x30){
			Inc_SetError(CALEFSC);
			if (sccnt++>3)	
				Pot_HeaterSC();
			else
				Pot_GetACStat();
			Ala_Heater(1);
		}
		else{
			onsctest=0;
			sccnt=0;
		}
	}
	else if (onoctest){
		onoctest=0;
		if(rec & 0x08)
			Ala_SetFalla(1);
		else
			Ala_ClrFalla(1);
	}
	else */if (!(rec & 0x08))
		Inc_Rec(CALCONEC);
	else if (rec & 0x30)
		Ala_Heater(1);
}
void Pot_HeaterSC(void){
	heatonsc=1;
	SPI_SendExp(ENHEAT, OFF, POTAC,0);
}
void Pot_HeaterInit(void){
	heatonsc=0;
	SPI_SendExp(ENHEAT, ON, POTAC,0);
}
void Pot_ChkHeat(void){
	if (!testfree){
		onoctest=1;
		Pot_HeaterOff();
	}
}
void Pot_Time(void){
	if (onoctest && !testfree){
		if(++testcnt >50){
			testcnt=0;
			testfree=1;
			Pot_GetACStat();
		}			
	}
	if(testfree)
		if (++testfree>600)
			testfree=0;
}
