/*
 * Modulo SPI
 * */

#include "SPIdev.h"
struct SBlock{
	SM1_TComData data[19];
	word qty;
	byte device;
	byte config;
};
struct SBlock scomds[256];
struct SBlock sync;
byte recbuf[19];
byte stosend, last;
byte sstat, ssrec;
word ssnt, srecd;
byte data[4];
byte addcom;
byte rectmp;
byte visdat[6];


void SPI_InitDev(void){
	
	//SPI_ReadMem(0x0155, &data[0],1);// Prueba Memoria
	//* <<<--- Configuración EXP MAX7301 OK 19-01-15
	SPI_SendExp(0x04, 0x00, 0, 0);
	SPI_SendExp(0x06, 0x00, 0, 0);
	SPI_SendExp(0x09, 0x55, 0, 0);
	SPI_SendExp(0x0a, 0x55, 0, 0);
	SPI_SendExp(0x0b, 0x55, 0, 0);
	SPI_SendExp(0x0c, 0x55, 0, 0);
	SPI_SendExp(0x0d, 0xa5, 0, 0);
	SPI_SendExp(0x0e, 0xaa, 0, 0);
	SPI_SendExp(0x0f, 0x6a, 0, 0);
	SPI_SendExp(0x04, 0x01, 0, 0);
	//*/		
	
	/*//SM1_ClearRxBuf();
	//Configura los habilitadores del ADC encendidos
	SPI_SendEnAd(0xff,0);
	SPI_SendEnAd(0xff,1);
	SPI_SendEnAd(0xff,2);
	// Configura Exp AC como entradas
	SPI_SendExp(0x0A, 0x00, POTAC, 0);
	SPI_SendExp(0x00, 0x01, POTAC, 0);
	SPI_SendExp(0x03, 0x01, POTAC, 0);
	SPI_SendExp(0x04, 0x01, POTAC, 0);
	SPI_SendExp(0x05, 0x01, POTAC, 0);
	SPI_SendExp(0x07, 0x01, POTAC, 0);
	SPI_SendExp(0x09, 0x01, POTAC, 0);
	SPI_SendExp(0x10, 0x01, POTAC, 0);	// Encender Exp 1
	// Configura Exp 2 como salidas y deja en cero lógico
	SPI_SendExp(0x0A, 0x00, POTCOX, 0);
	SPI_SendExp(0x09, 0x01, POTCOX, 0);
	SPI_SendExp(0x10, 0x01, POTCOX, 0);	// Encender Exp 2
	// Configura Exp 3 como salidas y deja en cero lógico
	SPI_SendExp(0x0A, 0x00, POTTDPT, 0);
	SPI_SendExp(0x06, 0x01, POTTDPT, 0);
	SPI_SendExp(0x07, 0x01, POTTDPT, 0);
	SPI_SendExp(0x08, 0x01, POTTDPT, 0);
	SPI_SendExp(0x09, 0x01, POTTDPT, 0);
	SPI_SendExp(0x10, 0x01, POTTDPT, 0); // Encender Exp 3
	// Apaga y habilita el calefactor*/
}
void SPI_SendExp(SM1_TComData com, SM1_TComData dat, byte dev, byte config){
	
	scomds[last].data[0] = com;
	scomds[last].data[1] = dat;
	scomds[last].qty = 2;
	scomds[last].device = dev;
	scomds[last].config = config;
	last++;
	
	if (sstat == 0){
		SPI_SendNext();
		sstat = 1;
	}
	else if (last>60){
		last=0;
		sstat=0;
		stosend=0;
	}
}
void SPI_SendEnAd(SM1_TComData dat, byte dev){
	scomds[last].data[0] = dat;
	scomds[last].qty = 1;
	scomds[last].device = dev+3;
	last++;

	if (sstat == 0){
		SPI_SendNext();
		sstat = 1;
	}
	else if (last>50){
		last=0;
		sstat=0;
		stosend=0;
	}
}
void SPI_ReadMem(word address, byte qty){
byte im;	
	scomds[last].data[0] = 0x03;
	scomds[last].data[1] = 0;
	scomds[last].data[2] = (byte)(address>>8);
	scomds[last].data[3] = (byte)address;
	for (im = 0; im<qty ;im++) scomds[last].data[4+im] = 0x55;
	scomds[last].device = 5;
	scomds[last].config = 2;
	scomds[last].qty = qty+4;
	last++;
	
	if (sstat == 0){
		SPI_SendNext();
		sstat = 1;
	}
	else if (last>60){
		last=0;
		sstat=0;
		stosend=0;
	}
}
void SPI_RecMem(byte *datrec){
	switch (scomds[stosend-1].data[3]){
	case 0:
		if(*datrec == 0x80){
			IncOff();
			setcont(350,15,0);
		}
		else if (*datrec == 0x81){
			IncOn();
			SPI_ReadMem(1,13);
			IncTestPassed();
		}
		break;
	case 2:
		setcont(((word)*(datrec-1))+ 150,*(datrec),0);
		visdat[0]=0x02;
		visdat[1]=0x56;
		visdat[5]=0x03;
		if		(*datrec==15) visdat[2]=0x50;
		else if	(*datrec==6)  visdat[2]=0x51;
		else if	(*datrec==7)  visdat[2]=0x52;
		visdat[4]= (*(datrec-1)%10)+0x80; 
		visdat[3]= *(datrec-1)/10+15; 
		Vis_SendBlock(visdat,6);
		break;
	case 3:
		SP_HumOx(((word)(*datrec)*10),1,0);
		visdat[0]=0x02;
		visdat[1]=0x56;
		visdat[2]=0x54;
		visdat[3]= *(datrec);
		visdat[4]=0x03;
		Vis_SendBlock(visdat,5);
		break;
	case 4:
		SP_HumOx(((word)(*datrec)*10),0,0);
		visdat[0]=0x02;
		visdat[1]=0x56;
		visdat[4]=0x03;
		visdat[2]=0x55;
		visdat[3]= *(datrec);
		Vis_SendBlock(visdat,5);
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		break;
	case 8:
		break;
	case 9:
		Cont_SetInt(datrec);
		break;
	default:
		break;
	}
	if (--scomds[stosend-1].qty > 4){
		scomds[stosend-1].data[3]++;
		SPI_RecMem(++datrec);	
	}
}
void SPI_WriteMem(word address, byte *dat, byte qty){
byte im;	
	scomds[last].data[0] = 0x02;
	scomds[last].data[1] = 0;
	scomds[last].data[2] = (byte)(address>>8);
	scomds[last].data[3] = (byte)address;
	for (im = 0; im<qty ;im++) scomds[last].data[4+im] = *(dat+im);
	scomds[last].device = 5;
	scomds[last].config = 0;
	scomds[last].qty = 4+qty;
	last++;

	if (sstat == 0){
		SPI_SendNext();
		sstat = 1;
	}
	else if (last>60){
		last=0;
		sstat=0;
		stosend=0;
	}
}
void SPI_Recieve(void) {
	SM1_ClearTxBuf();
	CS0_SetVal();
	//CS1_SetVal();
	//CS2_SetVal();
	CSIC1_ClrVal();
	CSIC2_ClrVal();
	CSMem_SetVal();
	if(scomds[stosend-1].config & RECIEVE){
		SM1_RecvBlock(&recbuf[0],scomds[stosend-1].qty,&srecd);
		if(scomds[stosend-1].device == 0){ 
			Adq_SensPot(recbuf);
		}
		else if(scomds[stosend-1].device == POTAC){ 
			Pot_Rec(recbuf[1]);
		}
		else if(scomds[stosend-1].device == 5){ 
			SPI_RecMem(&recbuf[4]);
		}
	}
	SM1_ClearRxBuf();	
}
void SPI_SendNext(void){
	if (stosend<last){
		switch(scomds[stosend].device){
			case 0:	CS0_ClrVal(); break;	
			//case 1:	CS1_ClrVal(); break;			
			//case 2: CS2_ClrVal(); break;
			case 3: CSIC1_SetVal(); break;
			case 4: CSIC2_SetVal(); break;
			case 5: CSMem_ClrVal(); break;
			default: break;
		}
		//SM1_ClearTxBuf();
		while (SM1_SendBlock(&scomds[stosend].data[0], scomds[stosend].qty, &ssnt))
			SM1_ClearTxBuf();
		//if ( ssnt!=scomds[stosend].qty)
			
		stosend++;
	}
	else{
		stosend = 0;
		last = 0;
		sstat = 0;
	}
}
void SPI_LockSync(void){
	sync.data[0] = EXPREAD1;
	sync.data[1] = 0x55;
	sync.qty = 2;
	sync.device = POTAC;
	sync.config = RECIEVE; 
	
	if (sstat == 0){
		SPI_SendNext();
		sstat = 1;
	}
}
void SPI_Stat(byte *srep){
	*srep     = stosend;
	*(srep+1) = last;
	*(srep+2) = sstat;
}
void SPI_GetSen(void){
	SPI_SendExp(0xd8, 0x55, 0, 0);
	SPI_SendExp(0xd8, 0x55, 0, RECIEVE);
}
