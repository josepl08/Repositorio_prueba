/* MODULE Protocolo */

#include "Protocolo.h"
#include <string.h>

byte InSerBuf[40], OutSerBuf[20], TempBuf[20];//,TempRep[100];
byte TempRep[200] = {"Air1,Air2,Air3,Oxi,THum,Hum,P1,P2,-,Peso,-,Cal*10,-,-,-,AirProm,pot,potr"};

word 		sent, rsent, bsent;
byte 		repcont;
byte 		pos;
byte 		serstat;
byte 		tosend;

byte 		recib;

uint32_t	palarms;


byte		control_activo=4;


byte		busbuff[6], bonrx, bpos = 0;
uint16_t	comando;

byte 		free=0;
byte		lock, leds, bell, stby;
byte 		busbuffout[7];
byte		colonmov;

byte 		pte;
byte		motvel, motlock, velant=1;
byte 		corr;

byte		buswait;

byte		visbuff[20][8];
byte		vidx, vlast, vstat;

byte		disppos;

extern 		uint16_t	sp[8] ;
extern 		byte 		active;
extern		int32_t 	ref;

/*Función para recibir tramas byte por byte*/
void Prot_Recibir(void){
	if(!pte){//serstat && RXSTD){
		AS1_RecvChar(&InSerBuf[pos]);
		if ((InSerBuf[pos] == ETX)){
			if(InSerBuf[1]=='A')
				if(InSerBuf[2]>=0x80 && InSerBuf[2]<=0x82){
					if (InSerBuf[2]!=0x80) InSerBuf[2]+=5;
					else InSerBuf[2]=0x8f;
					setcont(InSerBuf[3]*10 + (InSerBuf[4]-0x80), InSerBuf[2]-0x80,1);
				}	
				else if (InSerBuf[3]== 'O'){
					SP_HumOx(InSerBuf[4]*10,0,1);
				}
				else if (InSerBuf[3]== 'H'){  
					SP_HumOx(InSerBuf[4]*10,1,1);
				}	
				else if ((InSerBuf[2]== 'B') && (InSerBuf[3]== 'z')){
					zerobasc();
				}
				else if ((InSerBuf[2]== 'B') && (InSerBuf[3]== 'c')){
					calibbasc();
				}
				else if (InSerBuf[2] == 's'){
					bell = 0x00;
					leds &= 0xf7;
					leds |= (bell << 3) & 0x08;
					Prot_LedsBus(leds);
				}
				else if (InSerBuf[2] == 'p'){
					Rep_SendBlock(&InSerBuf[3],pos-3,&sent);
					//Rep_SendChar(0x0d);
					//Rep_SendChar(0x0a);
				}
			Vis_SendNext();
		}
		else if (InSerBuf[pos] == STX){
			InSerBuf[0] = InSerBuf[pos];
			pos = 1;
		}
		else if (InSerBuf[pos] == 6){
			Vis_SendNext();
		}
		else if ((InSerBuf[pos] == 5) ||(InSerBuf[pos] == 4)){
			vidx--;
			Vis_SendNext();
		}	
		else pos++;
		if (pos>25) pos=0;
	}
	else{
		AS1_RecvChar(&InSerBuf[0]);
		Rep_SendChar(InSerBuf[0]);
	}
}
void Vis_SendNext(void){
	if((vidx >= vlast) || (vlast == 0)){ 
		vlast=0;
		vidx=0;
		vstat=0;
	}
	else{
		AS1_SendBlock(&visbuff[vidx][0],visbuff[vidx][7],&sent);
		if(sent<visbuff[vidx][7])
			Vis_SendBlock(&visbuff[vidx][0],visbuff[vidx][7]);
		vidx++;
	}
}
void Vis_SendBlock(byte *buf, byte qty){
byte iv;	
	if (vlast >=19){
		vstat=0;
		vlast=0;
	}
	for(iv=0;iv<qty;iv++)
		visbuff[vlast][iv]=*(buf+iv);
	visbuff[vlast][7]=qty;
	vlast++;

	if (vstat==0){
		AS1_SendBlock(&visbuff[vidx][0],visbuff[vidx][7],&sent);
		if(sent<visbuff[vidx][7])
			Vis_SendBlock(&visbuff[vidx][0],visbuff[vidx][7]);
		vidx++;
		vstat=1;
	}
		
}
void Prot_ChecarDir(void){				/* Función para revisar si el comando es para esta dirección */
	if (InSerBuf[DEST] == MY_NAME){
		switch (InSerBuf[CMND1]){
			case 'P' : // Sensor Piel 1
				OutSerBuf[0] = 'P';
				Pedir_Valor_Adq(&OutSerBuf[1], 1);
				AS1_SendChar(0x02);
				AS1_SendBlock(&OutSerBuf[0],DECIMALES+4,&sent);
				AS1_SendChar(0x03);
			break;
			case 'Q' : // Sensor Piel 2 
				OutSerBuf[0] = 'Q';
				Pedir_Valor_Adq(&OutSerBuf[1], 0);
				AS1_SendChar(0x02);
				AS1_SendBlock(&OutSerBuf[0],DECIMALES+4,&sent);
				AS1_SendChar(0x03);			
			break;
			case 'A' : // Sensor Temperatura Aire 1	
				OutSerBuf[0] = 'A';
				Pedir_Valor_Adq(&OutSerBuf[1], 2);
				AS1_SendChar(0x02);
				AS1_SendBlock(&OutSerBuf[0],DECIMALES+4,&sent);
				AS1_SendChar(0x03);			
			break;
			case 'B' : // Sensor Temperatura Aire 2
				OutSerBuf[0] = 'B';
				Pedir_Valor_Adq(&OutSerBuf[1], 3);
				AS1_SendChar(0x02);
				AS1_SendBlock(&OutSerBuf[0],DECIMALES+4,&sent);
				AS1_SendChar(0x03);
			break;
			case 'C' : // Sensor Temperatura Aire 3	
				OutSerBuf[0] = 'C';
				Pedir_Valor_Adq(&OutSerBuf[1], 4);
				AS1_SendChar(0x02);
				AS1_SendBlock(&OutSerBuf[0],DECIMALES+4,&sent);
				AS1_SendChar(0x03);			
			break;
			case 'D' : // Sensor Temperatura Aire 4	
				OutSerBuf[0] = 'D';
				Pedir_Valor_Adq(&OutSerBuf[1], 5);
				AS1_SendChar(0x02);
				AS1_SendBlock(&OutSerBuf[0],DECIMALES+4,&sent);
				AS1_SendChar(0x03);			
			break;
			case 'O' : // Sensor Oxígeno
				OutSerBuf[0] = 'O';
				Pedir_Valor_Adq(&OutSerBuf[1], 6);
				AS1_SendChar(0x02);
				AS1_SendBlock(&OutSerBuf[0],DECIMALES+4,&sent);
				AS1_SendChar(0x03);			
			break;
			case 'H' : // Sensor Humedad
				OutSerBuf[0] = 'H';
				Pedir_Valor_Adq(&OutSerBuf[1], 7);
				AS1_SendChar(0x02);
				AS1_SendBlock(&OutSerBuf[0],DECIMALES+4,&sent);
				AS1_SendChar(0x03);
			break;
			case 'G' : // Sensor Temperatura en Humedad	
				OutSerBuf[0] = 'G';
				Pedir_Valor_Adq(&OutSerBuf[1], 2);
				AS1_SendChar(0x02);
				AS1_SendBlock(&OutSerBuf[0],DECIMALES+4,&sent);
				AS1_SendChar(0x03);			
			break;
			case 'F' : // Sensor Temperatura Calefactor
				OutSerBuf[0] = 'F';
				Pedir_Valor_Adq(&OutSerBuf[1], 2);
				AS1_SendChar(0x02);
				AS1_SendBlock(&OutSerBuf[0],DECIMALES+4,&sent);
				AS1_SendChar(0x03);			
			break;
			case 'I' : // Sensor Temperatura Humidificador
				OutSerBuf[0] = 'I';
				Pedir_Valor_Adq(&OutSerBuf[1], 2);
				AS1_SendChar(0x02);
				AS1_SendBlock(&OutSerBuf[0],DECIMALES+4,&sent);
				AS1_SendChar(0x03);			
			break;
			default : 
				AS1_SendChar(0x02);
				AS1_SendChar(0x15);
				AS1_SendChar(0x03);	
		}
	}

}
void Prot_Refresh(void){
	uint16_t altmp;

	if(!pte){
		switch(tosend){
			case 0x00:
				OutSerBuf[2]=0x2E; // Aire
			break;
			case 0x02:
				OutSerBuf[2]='L'; // Piel X
			break;
			case 0x03:
				OutSerBuf[2]= 0x09; // Oxígeno
			break;
			case 0x04:
				OutSerBuf[2]= 0x25; // Piel 1
			break;
			case 0x05:
				OutSerBuf[2]= 0x0B; // Humedad
			break;
			case 0x06:
				OutSerBuf[2]= 0x2C; // Piel 1
			break;		
			case 0x07:
				OutSerBuf[2]= 0x2D; // Piel 2
			break;
			case 0x08:
				SPI_GetSen();
				OutSerBuf[2]=0xff;
			break;
			case 9:
				//Ala_SendAviso();
				OutSerBuf[2]=0xff;
			break;
			case 10:
				//Ala_SendError();
				OutSerBuf[2]=0xff;
			break;
			case 11:	
				//Ala_SendDesv();
				OutSerBuf[2]=0xff;
			break;
			case 12:
				//Ala_SendAla();
				OutSerBuf[2]=0xff;
			break;
			case 14:
				//Ala_SendFalla();
				OutSerBuf[2]=0xff;
			break;	
			case 0x0d:
				OutSerBuf[2]=0x07;
			break;
			default:
				OutSerBuf[2]=0xff;
			break;
			
		}
		if(tosend == 0x02){  // Potencia
			Pot_GetPot(&TempBuf[1],0);
			OutSerBuf[3] = TempBuf[1];
			OutSerBuf[4] = 0;
		}
		else if (tosend == 0x0d){ // Báscula
				if (0){//Ala_GetFalla('f') && 1<<tosend){
					OutSerBuf[3] = 0x3a;
				}
				else{
					Pedir_Valor_Adq(&TempBuf[1], 9);
					OutSerBuf[3] = (TempBuf[1]-0x30);	
					OutSerBuf[4] = (TempBuf[3]-0x30) * 10 + (TempBuf[4]- 0x30);
				}
			}
		else if (tosend == 0){  // Aire
			if (0){//Ala_GetFalla('f') && 1<<tosend){
				OutSerBuf[4] = 0x8a;
			}
			else{			
				Pedir_Valor_Adq(&TempBuf[1], 15);
				OutSerBuf[3] = (TempBuf[1]-0x30) * 10 + (TempBuf[2]- 0x30);	
				OutSerBuf[4] = TempBuf[4]-0x30 + 0x80;
			}
		}
		else{
			if (0){//Ala_GetFalla('f') && 1<<tosend){
				OutSerBuf[4] = 0x8a;
			}
			else{
				Pedir_Valor_Adq(&TempBuf[1], tosend);
				OutSerBuf[3] = (TempBuf[1]-0x30) * 10 + (TempBuf[2]- 0x30);	
				OutSerBuf[4] = TempBuf[4]-0x30 + 0x80;
			}
		}	
		OutSerBuf[0] = 0x02;
		OutSerBuf[1] = 0x56;
		OutSerBuf[5] = 0x03;
		
		if(OutSerBuf[2]!=0xff){ 
			Prot_CorTrama(6);
			//vstat|=2;
			Vis_SendBlock(&OutSerBuf[0],6+corr);		
		}
		switch(tosend%9){ // Control de Pantalla auxiliar 
			case 0:
				OutSerBuf[0] = 0x02;
				OutSerBuf[1] = 0x56;
				OutSerBuf[2] = 0x2c;
				if (disppos <2)
					OutSerBuf[3] = 101+disppos;
				else 
					OutSerBuf[3] = 100+disppos;
				if (disppos == 2 )
					OutSerBuf[4] = 0x82;
				else 
					OutSerBuf[4] = 0x81;
				OutSerBuf[5] = 0x03;
				free = 0;
				BusEn_SetVal();
				Bus_DisableEvent();
				Bus_SendBlock(&OutSerBuf[0],7+corr,&bsent);
				Bus_EnableEvent();
			break;
			case 3:
				OutSerBuf[0] = 0x02;
				OutSerBuf[1] = 0x56;
				OutSerBuf[2] = 0x2d;
				OutSerBuf[5] = 0x03;
				switch (disppos){
					case 0:
						if (active ==15){
							OutSerBuf[3] = ref/100;
							OutSerBuf[4] = (ref%100)/10+0x80;
						}
						else{
							OutSerBuf[3] = 00;
							OutSerBuf[4] = 0x80;
						}
					break;	
					case 1:
						if (active ==6){
							OutSerBuf[3] = ref/100;
							OutSerBuf[4] = (ref%100)/10+0x80;
						}
						else{
							OutSerBuf[3] = 00;
							OutSerBuf[4] = 0x80;
						}
					break;	
					case 2:
						if (active ==7){
							OutSerBuf[3] = ref/100;
							OutSerBuf[4] = (ref%100)/10+0x80;
						}
						else{
							OutSerBuf[3] = 00;
							OutSerBuf[4] = 0x80;
						}
					break;	
					case 3:
						OutSerBuf[3] = sp[3]/10;
						OutSerBuf[4] = sp[3]%10+0x80;
					break;	
					case 4:
						OutSerBuf[3] = sp[5]/10;
						OutSerBuf[4] = sp[5]%10+0x80;
					break;	
					default:
					break;
				}
				free = 0;
				BusEn_SetVal();
				Bus_DisableEvent();
				Bus_SendBlock(&OutSerBuf[0],7+corr,&bsent);
				Bus_EnableEvent();
			break;
			case 6:
				OutSerBuf[0] = 0x02;
				OutSerBuf[1] = 0x56;
				OutSerBuf[2] = 0x2e;
				OutSerBuf[5] = 0x03;
				switch (disppos){
					case 0:
						Pedir_Valor_Adq(&TempBuf[1], 15);
						OutSerBuf[3] = (TempBuf[1]-0x30) * 10 + (TempBuf[2]- 0x30);	
						OutSerBuf[4] = TempBuf[4]-0x30 + 0x80;
						OutSerBuf[5] = 0x03;
					break;	
					case 1:
						Pedir_Valor_Adq(&TempBuf[1], 6);
						OutSerBuf[3] = (TempBuf[1]-0x30) * 10 + (TempBuf[2]- 0x30);	
						OutSerBuf[4] = TempBuf[4]-0x30 + 0x80;
						OutSerBuf[5] = 0x03;
					break;	
					case 2:
						Pedir_Valor_Adq(&TempBuf[1], 7);
						OutSerBuf[3] = (TempBuf[1]-0x30) * 10 + (TempBuf[2]- 0x30);	
						OutSerBuf[4] = TempBuf[4]-0x30 + 0x80;
						OutSerBuf[5] = 0x03;
					break;	
					case 3:
						Pedir_Valor_Adq(&TempBuf[1], 3);
						OutSerBuf[3] = (TempBuf[1]-0x30) * 10 + (TempBuf[2]- 0x30);	
						OutSerBuf[4] = TempBuf[4]-0x30 + 0x80;	
						OutSerBuf[5] = 0x03;
					break;	
					case 4:
						Pedir_Valor_Adq(&TempBuf[1], 5);
						OutSerBuf[3] = (TempBuf[1]-0x30) * 10 + (TempBuf[2]- 0x30);	
						OutSerBuf[4] = TempBuf[4]-0x30 + 0x80;	
						OutSerBuf[5] = 0x03;
					break;	
					default:
					break;
				}
				free = 0;
				BusEn_SetVal();
				Bus_DisableEvent();
				Bus_SendBlock(&OutSerBuf[0],7+corr,&bsent);
				Bus_EnableEvent();
			default:
			break;
		} // Fin Control De pantalla auxiliar
		
		
		/*if(tosend == 0 || tosend == 6 || tosend == 7 ){
			free = 0;
			BusEn_SetVal();
			Bus_DisableEvent();
			Bus_SendBlock(&OutSerBuf[0],7+corr,&bsent);
			Bus_EnableEvent();
		}*/
		tosend++;
		tosend &= 0x0F;
	}
	//*  Funciones de Control*
	//* 1diag deshab, 2 diag hab


}
void Prot_CorTrama(byte tosd){
	byte ct,tbf[10];
	corr=0;
	for (ct=1;ct<tosd-1;ct++){
		if (OutSerBuf[ct] >= 1 && OutSerBuf[ct]<=3 ){
			tbf[ct+corr]=1;
			tbf[ct+1+corr]=OutSerBuf[ct]+0x80;
			corr++;
		}
		else
			tbf[ct+corr]=OutSerBuf[ct];
	}
	for (ct=1;ct<tosd+corr-1;ct++){
		OutSerBuf[ct]=tbf[ct];
	}
	OutSerBuf[tosd-1+corr]=3;
}
void Prot_Report(void){					/* Función de Reporte */
	byte ci, repopt;
	Rep_RecvChar(&repopt);
	if (pte){
		if (repopt == 0x0d)
			Prot_Pte(0);
		else 
			AS1_SendChar(repopt);
	}
	if(repopt == 'e'){
		IncOn();
		//IncTestPassed();
	}
	if(repopt == 'h'){
		asm(HALT);
	}
	else if(repopt == 'a')
		IncOff();
	if(repopt == 'E'){
		IncOn();
		IncTestPassed();
	}
	else if(repopt == 'p'){
		Prot_Pte(1);
		OutSerBuf[0] = 0x02;
		OutSerBuf[1] = 0x56;
		OutSerBuf[2] = 'p';
		OutSerBuf[3] = 0x03;
		AS1_SendBlock(&OutSerBuf[0],4,&sent);
	}
	else if(repopt == 'z')
		zerobasc();
	else{
		if (!pte) {
			TempRep[0]=0x0d;
			TempRep[1]=0x0a;
			Rep_SendBlock(&TempRep[0],2,&rsent);
			for(ci=0;ci<16;ci++){
				TempRep[ci*10] = 0x1b;
				TempRep[ci*10+1] = 0x5b;
				TempRep[ci*10+2] = '3';
				TempRep[ci*10+3] = '1'+(ci%6);
				TempRep[ci*10+4] = 'm';
				Pedir_Valor_Adq(&TempRep[ci*10+5],ci);
				TempRep[ci*10-1] = ',';
			}
			Pot_GetPot(&TempRep[160],1);
			//SPI_Stat(&TempRep[86]);
			TempRep[166] = ',';
			Rep_SendBlock(&TempRep[0],167,&rsent);
			Prot_Date();
		}
	}
}
void Prot_Date(void){
byte td[4];
	td[0]=2;
	td[1]='V';
	td[2]='f';
	td[3]=3;
	Vis_SendBlock(td,4);
}
void Prot_SetAlarmsPot(byte edo){
	palarms &= 0xffffff00;
	palarms += edo;
}
void Prot_BusRec(void){
	(void)Bus_RecvChar(&busbuff[pos]);
	if(busbuff[pos] == 0x02){
		busbuff[0] = 0x02;
		pos = 1;
		bonrx = 1;
		if(buswait){
			Ala_Bus(buswait,1);
			buswait=0;
		}
	}
	else if(bonrx){
		if(busbuff[pos] == 0x03){
			if(busbuff[1] == 'M') 
				Prot_mot(busbuff);
			else if (busbuff[1] == 'B')
				Prot_bots(busbuff);
			else if(busbuff[1] == 0x05){
				Prot_mot(busbuff);
				Ala_Bus(buswait,0);
				buswait=0;
			}
			pos=0;
			bonrx=0;
		}
		else {
			pos++;
			if(pos==7){pos=0; bonrx=0; }
		}
	}
}
void Bus_WaitAnsw(byte from){
	buswait=from;
}
void Prot_BusNoAnsw(void){
	Ala_Bus(buswait,1);
}
void Prot_mot(byte datos[]){
	if(datos[2]!=0xff){
		Inc_Rec(0x02);
	}
}
void Prot_LedsBus(byte led){
	busbuffout[0] = 0x02;
	busbuffout[1] = 'B';
	busbuffout[2] = 'E';
	busbuffout[4] = 0x03;
	if (led ==0)
		busbuffout[3] = 0;
	else	
		busbuffout[3] = ~led;
	BusEn_SetVal();
	Bus_DisableEvent();
	Bus_SendBlock(&busbuffout[0],6,&bsent);
	Bus_EnableEvent();
}
void Prot_bots(byte datos[]){
uint16_t botones;
	
	Lktim_Init();
	botones = ((datos[3] << 8 ) | datos[2]) & 0x3f7f;
	if(botones){
		comando |= botones;
	}
	else{
		switch (comando){
			case 8:
				if (--disppos==0xff)
					disppos=4;
			break;
			case 64:
				if (++disppos>=5)
					disppos=0;
			break;
			case 2048 :
				if(!lock && !stby ) 
					Pot_Column(1);
				break;
			case 512 :
				if(!lock && !stby) 
					Pot_Tbg(TBG12N);
				break;
			case 4096 :
				if (!lock && !stby )
					Pot_Column(4);
				break;
			case 16 :
				if(!stby){
					lock = lock ^ 0x01;
					leds &= 0xbf;
					leds |= (lock << 6) & 0x40;
					Prot_LedsBus(leds);
					Lktim_Enable();
				}
				break;
			case 256 :
				if(!lock && !stby){ 
					Pot_Tbg(TBG0);
					Pot_Column(0);
				}
				break;
			case 1024 :
				if(!lock && !stby) 
					Pot_Tbg(TBG12P);
				break;
			case 4 :
				if(!lock && !stby){
					bell = 0x01;
					leds &= 0xf7;
					leds |= (bell << 3) & 0x08;
					Prot_LedsBus(leds);
					Prot_SilAlarm();
				}
				break;
			case 1:
				if(!lock){
					if(stby){
						IncOn();
						IncTestPassed();
					}
					else{
						IncOff();
					}	
				}	
				break;
			default :
				break;
		}
		comando=0;
	}	
}
void Prot_LedInit(void){
	leds = 0x00;
	Prot_LedsBus(0x00);
}
void Prot_BufEmpty(void){
	BusEn_ClrVal();
	//free = 1;
}
void Prot_TxChar(void){
	free++;
	if(free >= 7){
		
		free=0;
	}
}
void Prot_lock(void){
	lock = 1;
	leds &= 0xbf;
	leds |= (lock << 6) & 0x40;
	Prot_LedsBus(leds);
}
void Prot_Alarm(byte tipo, word valor){
	byte endpos=0;
	OutSerBuf[0] = 0x02;
	OutSerBuf[1] = 0x56;
	OutSerBuf[2] = 'a';
	OutSerBuf[3] = tipo;
	OutSerBuf[4] = (byte)(valor>>8);
	if (OutSerBuf[4]>= 0x01 && OutSerBuf[4]<= 0x03){
		endpos++;
		OutSerBuf[5]= OutSerBuf[4]+0x80;
		OutSerBuf[4]= 0x01;
	}
	OutSerBuf[5+endpos] = (byte)valor;
	if (OutSerBuf[5+endpos]>= 0x01 && OutSerBuf[4]<= 0x03){
		OutSerBuf[6+endpos]= OutSerBuf[5+endpos]+0x80;
		OutSerBuf[5+endpos]= 0x01;
		endpos++;
	}
	OutSerBuf[6+endpos] = 0x03;
	
		
	AS1_SendBlock(&OutSerBuf[0],7+endpos,&sent);
}
void IncOff(void){
byte tmp;
	stby = 1;
	Prot_LedsBus(0x00);	
	Prot_DispOff();
	
	Lktim_Disable();
	ACInt_Disable();
	ACTim_Disable();
	RefreshScreen_Disable();
	InitTestTim_Disable();
	
	SPI_SendEnAd(0x0,0);
	SPI_SendEnAd(0x0,1);
	//SPI_SendEnAd(0x0,2);
	
	Pot_Oxy(OFF);
	Pot_Hum(OFF);
	Pot_HeaterOff();
	
	Cpu_Delay100US(500);
	Prot_Motor(0);
	Sync_ClrVal();
	
	SenEn_PutVal(0x07);
	SPI_SendEnAd(6,2);
	tmp=0x80;
	SPI_WriteMem(0,&tmp,1);
}
void IncOn(void) {
	leds = 0x04;
	stby = 0;
	ACInt_Disable();
	Prot_LedsBus(leds);
	Prot_DispOn();
	//Inc_start();
	//InitTestTim_Enable();
	SPI_SendEnAd(0xff, 0);
	SPI_SendEnAd(0xff, 1);
	//SPI_SendEnAd(0xff, 2);
	Pot_HeaterInit();
	SenEn_PutVal(0x08);
	//IncTestPassed();
}
void IncTestPassed(void){
byte tmp;	
	stby = 0;
	Lktim_Enable();
	//TI1_Enable();
	ACInt_Enable();
	RefreshScreen_Enable();
	InitTestTim_Disable();
	Pot_MotorOn();
	Prot_Motor(0x55);
	
	SPI_SendEnAd(6,2);
	tmp=0x81;
	SPI_WriteMem(0,&tmp,1);
}
void Prot_MotSetVel(byte vel,byte lock){
	if (lock)
		motlock=1;
	else if (vel == 0xff)
		motlock=0;
	
	if (motlock)
		Prot_Motor(0xff);
	else
		motvel=vel;
}
void Prot_MotAct(){
	Prot_Motor(motvel);
}
void Prot_Motor(byte vel){
	if (1){//(vel != velant) /*&& !motlock*/){
		OutSerBuf[0]=2;
		OutSerBuf[1]='M';
		OutSerBuf[2]='V';
		OutSerBuf[5]=3;
		if (vel == 0){
			OutSerBuf[1]=0x50;
			OutSerBuf[2]=0x51;
			OutSerBuf[3]=3;
			BusEn_SetVal();
			Bus_DisableEvent();
			Bus_SendBlock(&OutSerBuf[0],5,&bsent);
			Bus_EnableEvent();
		}
		else if (vel < 32){
			OutSerBuf[3]=0xe9-vel;
			OutSerBuf[4]=OutSerBuf[3]^0x1A;
			BusEn_SetVal();
			Bus_DisableEvent();
			Bus_SendBlock(&OutSerBuf[0],7,&bsent);
			Bus_EnableEvent();
		}
		else{
			OutSerBuf[3]=0xc9;
			OutSerBuf[4]=0xd3;
			BusEn_SetVal();
			Bus_DisableEvent();
			Bus_SendBlock(&OutSerBuf[0],7,&bsent);
			Bus_EnableEvent();
		}
		BusErrTime_Enable();	
	}
	velant=vel;
}
void Prot_Puertas(byte edopts){
	if (edopts)
		Prot_Motor(0xff);
	else
		Prot_Motor(0x55);
		
}
void Prot_SilAlarm(void){
	OutSerBuf[0]=2;
	OutSerBuf[1]='V';
	OutSerBuf[2]='a';
	OutSerBuf[3]='S';
	OutSerBuf[4]=3;
	AS1_SendBlock(&OutSerBuf[0],5,&sent);
}
void Prot_Pte(byte cfg){
	pte=cfg;
}
void Prot_DispOn(void){
	OutSerBuf[0]=2;
	OutSerBuf[1]='V';
	OutSerBuf[2]='e';
	OutSerBuf[3]=3;
	Vis_SendBlock(&OutSerBuf[0],4);
	OutSerBuf[2]='F';
	OutSerBuf[3]='0';
	OutSerBuf[4]='0';
	OutSerBuf[5]=0x03;
	Vis_SendBlock(&OutSerBuf[0],6);
	//AS1_SendBlock(&OutSerBuf[0],4,&sent);
	Rep_SendChar(0x1b);
	Rep_SendChar(0x5b);
	Rep_SendChar('2');
	Rep_SendChar('J');
	Rep_SendChar(0x1b);
	Rep_SendChar(0x5b);
	Rep_SendChar('1');
	Rep_SendChar(';');
	Rep_SendChar('1');
	Rep_SendChar('H');	
	Rep_SendChar(0x1b);
	Rep_SendChar(0x5b);
	Rep_SendChar('3');
	Rep_SendChar('7');
	Rep_SendChar('m');
	//TempRep = {"\n\rAir1,Air2,Air3,Oxi,THum,Hum,P1,P2,-,Peso,-,Cal*10,-,-,-,AirProm,pot,potr\n"};
	Rep_SendBlock(&TempRep[0],74,&sent);
	Rep_SendChar(',');
	Prot_Date();
}
void Prot_DispOff(void){
	OutSerBuf[0]=2;
	OutSerBuf[1]='V';
	OutSerBuf[2]='g';
	OutSerBuf[3]=3;
	Vis_SendBlock(&OutSerBuf[0],4);
}
void Prot_LcdForm(byte form, byte type){
	OutSerBuf[0]=2;
	OutSerBuf[1]='V';
	OutSerBuf[2]='F';
	OutSerBuf[3]=form;
	OutSerBuf[4]=type;
	OutSerBuf[5]=3;
	AS1_SendBlock(&OutSerBuf[0],6,&sent);
}
void Prot_LVD(void){
	Rep_SendChar('V');
}
