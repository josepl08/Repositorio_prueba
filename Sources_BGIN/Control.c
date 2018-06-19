/*
 * Adq.c
 *
 *  Created on: Nov 2, 2014
 *      Author: ATE03
 */
#include "Control.h"

int32_t 	kp=350, ki=30, kd=100;
int32_t 	di, intct;
int32_t 	error,error_ant;
int32_t 	ref = 3500;
int32_t 	pid;
int32_t		act_val;
byte 		active;
byte		pot,potsw;
byte		repc_buf[17];
word		csent;

int32_t		kph=1, kih=1;
int32_t		error_h;
int32_t		refh;
int32_t		heat_t;
int32_t		intcth=50000;

byte 		cheat;
uint16_t	ValAct;
byte 		TempC[5];
uint16_t	sp[8] = {250,250,250,300,350,500,350,350};

void control(void){
byte bfin[4];	
	pid 	 = 0;
	act_val  = Adq_Val(active,1);
	error 	 = ref-act_val;
	pid 	+= kp * error;
	if (error < 320 )	{
		intct += error;
		bfin[0]=(byte)(intct>>24);
		bfin[1]=(byte)(intct>>16);
		bfin[2]=(byte)(intct>>8);
		bfin[3]=(byte)(intct);
		//SPI_SendEnAd(6,2);
		//SPI_WriteMem(9,bfin,4);
	}
	//else intct=0;
	error_ant = error;
	pid	+= (intct / ki);
	
	if(error < -100){
		if (active ==15)
			Ala_SetDesv(16);
		else if (active ==6)
			Ala_SetDesv(1);
		else if (active ==7)
			Ala_SetDesv(4);
		if (error > 250)
			Ala_SetAlarma(0x40);
	}
	else{
		Ala_ClrDesv(16);
		Ala_ClrDesv(1);
		Ala_ClrDesv(4);
		Ala_ClrAlarma(0x40);
	}
	if(error > 100){
		if (active ==15)
			Ala_SetDesv(32);
		else if (active ==6)
			Ala_SetDesv(2);
		else if (active ==7)
			Ala_SetDesv(8);
		if (error > 250)
			Ala_SetAlarma(0x40);
	}
	else{
		Ala_ClrDesv(32);
		Ala_ClrDesv(2);
		Ala_ClrDesv(8);
		Ala_ClrAlarma(0x40);
	}
	
	if (error >320)
		Prot_MotSetVel(32,0);
	else if (error > 0 )
		Prot_MotSetVel(error/10+1,0);
	else
		Prot_MotSetVel(1,0);
	
	/*if(intct>0x7ffff)
		intct=0x7ffff;
	/*else if (in < 0xffff8000)
		in=0xffff8000;*/
	
	if (pid > 0xffff){
		//refh = 3500;
		if (!cheat) intcth = 50000;
		pot=100;
	}
	else if (pid < 0){
		//refh = 0;
		if (!cheat) intcth=0;
		pot=0;
	}
	else{
		//refh = (pid / 187)*10;
		if (!cheat) intcth=pid;
		pot=pid/655;
	}
	potsw=pot;
	pid = 0;
	
	refh=6500;
	heat_t = Adq_Val(11,1);
	if(error > 100 && heat_t<1500) 
		Pot_ChkHeat();
	else 
		Ala_Heater(0);
	if(heat_t>8000){	
		cheat=1;
		error = refh - heat_t;	
		//pid 	+= kph * error;
		intcth 		+= error;
		if(intcth>0xffff)
			intcth=0xffff;
		if(intcth<-8000)
			intcth=-8000;
		pid	+= (intcth / kih);
		if (pid > 0xffff)
			pot = 100;
		else if (pid < 0)
			pot = 0;
		else
			pot = (byte)(pid / 658);
	}
	else cheat=0;
	//pot=pot-25;
	Pot_HeaterPow(pot,potsw);
	
	/* Control de humedad */
	Pedir_Valor_Adq(&TempC[1], 5); //Pedir Humedad
	ValAct = (TempC[1]-0x30) * 100 + (TempC[2]- 0x30) * 10 + (TempC[4]-0x30);
	
	if (ValAct < sp[5])
		Pot_Hum(ON);
	else
		Pot_Hum(OFF);
	
	if (ValAct > sp[5]+100){ // Alarma desviación Humedad
		Ala_SetDesv(0x0040);
		if(ValAct > sp[5]+200)
			Ala_SetFalla(0x20);
		else
			Ala_ClrFalla(0x20);
	}
	else{
		Ala_ClrDesv(0x0040);
		Ala_ClrFalla(0x20);
	}
	
	if (ValAct < sp[5]-100){
		Ala_SetDesv(0x0080);
	}
	else{
		Ala_ClrDesv(0x0080);
	}
	
	/* Control de oxigeno */
	Pedir_Valor_Adq(&TempC[1], 3); //Pedir Oxigeno
	ValAct = (TempC[1]-0x30) * 100 + (TempC[2]- 0x30) * 10 + (TempC[4]-0x30);
	
	if (ValAct < sp[3])
		Pot_Oxy(ON);
	else
		Pot_Oxy(OFF);
	
	if (ValAct > sp[3]+100)
		Ala_SetDesv(0x1000);
	else
		Ala_ClrDesv(0x1000);
	
	if (ValAct < sp[3]-100)
		Ala_SetDesv(0x2000);
	else
		Ala_ClrDesv(0x2000);
}
/* Función para cambiar control */
void setcont(int16_t setpoint, byte act, bool save){
byte tmp[2];
	ref = setpoint*10;
	active = act;	
	intct=0;
	intcth=0;
	tmp[1]=act;
	tmp[0]=(byte)setpoint-150;
	if (save){
		SPI_SendEnAd(6,2);
		SPI_WriteMem(1,&tmp[0],2);
	}
	
}
/* Función para reporte de variables */
void Cont_SetInt(byte *intdir){
	//intct= (int32_t)(*intdir)<<24 + (int32_t)(*(intdir+1))<<16 + (int32_t)(*(intdir+2))<<8 + (int32_t)(*(intdir+3));
}

void Cont_report(void){
	
	repc_buf[0]=active;
	
	repc_buf[1] =(byte)(ref>>24);
	repc_buf[2] =(byte)(ref>>16);
	repc_buf[3] =(byte)(ref>>8);
	repc_buf[4] =(byte)(ref);
	
	repc_buf[5] =(byte)(act_val>>24);
	repc_buf[6] =(byte)(act_val>>16);
	repc_buf[7] =(byte)(act_val>>8);
	repc_buf[8] =(byte)(act_val);
	
	repc_buf[9] =(byte)(error>>24);
	repc_buf[10]=(byte)(error>>16);
	repc_buf[11]=(byte)(error>>8);
	repc_buf[12]=(byte)(error);
	
	repc_buf[13]=(byte)(pid>>24);
	repc_buf[14]=(byte)(pid>>16);
	repc_buf[15]=(byte)(pid>>8);
	repc_buf[16]=(byte)(pid);
	
	Rep_SendBlock(&repc_buf[0],17,&csent);
	
}

void SP_HumOx(uint16_t setpoint, byte act, bool save){
byte tm;
	if(act){
		sp[5]=setpoint;
		tm=(byte)(setpoint/10);
		if (save){
			SPI_SendEnAd(6,2);
			SPI_WriteMem(3,&tm,1);
		}
	}
	else{
		sp[3]=setpoint;
		tm=(byte)(setpoint/10);
		if (save){
			SPI_SendEnAd(6,2);
			SPI_WriteMem(4,&tm,1);
		}
	}
}
