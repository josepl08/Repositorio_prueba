

#include "Adq.h"
/****************************************************************************************
Este archivo contiene los métodos necesrios para el manejo de las actividades de adquisición
de datos que requeridos para el proyecto BGIN. De manera general, los métodos contenidos se
encargan de:
	+ Adquirir y acondicionar los datos
*/
byte 		clkstat;
//byte 		clkcount = 0;		//Se migra como variable local dentro de Lecturas_clk() 
byte 		medic = 0;				// medic: variable global para guardar numero de mediciones de adc
int32_t 	Valores[16], Valores_r[16];
uint32_t	Valores_[16],Valores_r_[16];
uint32_t 	tempval, tempval2, dieces;
uint16_t 	Data[16][16];
uint8_t		Array[16][32];
int16_t 	filtro[16][FILTER_DEPTH];
word 		senta;
uint8_t 	i,val,fil;		// Se migra variable 'par' como variable local dentro de Lecturas_fin()
int16_t 	factor[16] = {3125,3125,3125,162,3125,1250,3125,3125,3125,477,477,1120,1120,938,625,312};

uint32_t	offset_decneg[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint32_t	offset_decpos[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

uint32_t	offsetn[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint32_t	offsetp[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

uint8_t		puntos[16] = {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3};
byte 		ValAsc[16][8];
//byte 		sigfilt;			//Se migra como variable local dentro de Lecturas_fin() 
//int32_t		pyftmp;			//Se migra como variable local dentro de Lecturas_fin() 

int64_t		rht;
int64_t		facteqh[5] = {-19206,143700000,34210000000,-124000000000000,240000000};
int64_t		facteqpi[7]= {91400, 137600, -85000, 1000700, 1101600, 18248000, 24989000};
int64_t		facteqth[7]= {246800, -4069300, 26646000, -89507000, 165980000, -186230000, 125720000};


byte		ch;
/* variables de alarma */
uint16		alarmh[16] = {390,390,390,1000,390,1000,380,380,0,0,0,4000,4000};
uint16    	alarml[16] = {250,250,250,180,250,300,340,340};
int32_t    	valdes[16] = {200000,200000,200000,180000,10000,10000,3800000,3800000,0,0,10000,80000000,80000000};
word		alsth,alstl, alen=0x10ef,senen=0x14ff,sencon,sestdes=0x18c0;

byte		tstcnt;

/*******************************	void Lecturas_clk()	**************************************
	Método para la adquisición de lecturas de sensores de IC ADS1178, las lecturas se realizan 
	desde el puerto PORTAN del microcontrolador. el bus de datos que entra al puerto PORTAN es
	un bus paralelo multiplezado de 8 canales. (Componente ADC_Data).
	Se acumulan 16 mediciones de sensores em arreglo Array[][] y se pasa al método de post 
	procesamiento Lecturas_fin().
*/
void Lecturas_clk(){
	byte		nbtd, clkcount = 0;
	do{
		SCLK_SetVal();
		nbtd++;
		SCLK_ClrVal();
		
		Array[medic][clkcount] = ADCData_GetVal();
		ADSel_SetVal();
		Array[medic][clkcount + 16] = ADCData_GetVal();
		ADSel_ClrVal();
		clkcount++;
	}while(clkcount<16);
	if(++medic >=16){
		medic=0;
		Lecturas_fin();
	}
}
/********************************	void Lecturas_fin()	**************************************
	Método para post procesamiento de datos obtenidos se realiza acomodo de lecturas de sensores
	se convierten las lecturas de adc al valor de cada sensor por medio de factores de conversion
	se convierten los valores numericos a formato ASCII para su posterior envio y se verifican
	las alarmas asociadas a valores de sensores.
*/
void Lecturas_fin(void){
	// VARIABLES LOCALES 
	volatile uint8_t 	buff[2];
	volatile uint16_t 	res[2];
	volatile bool 		cbit[2];
	volatile byte med, row, col; 	// contadores para acomodo de datos
	volatile byte sigfilt;		 	// contador para arreglo de valores de filtro promedio
	volatile int32_t pyftmp;	 	// acumulador para valores de filtro promediador
	volatile byte par 				// contador utilizado en etapa de filtrado
	// SECCION DE ACOMODO DE VALORES DE SENSORES
	// Limpia última conversión
	for( med=0 ; med < 16; med++ ){
		for( col=0 ; col < 16; col++ ){
			Data[col][med]=0;
		}
	}
	// Acomodar los datos recibidos
	for( med=0 ; med < 16; med++ ){
		for( col=0 ; col < 16; col++ ){
			buff[0] = Array[med][col];
			buff[1] = Array[med][col + 16];
			for( row = 0; row < 8; row++){
				// ACOMODO 1ER ADC
				cbit[0] = buff[0] & 0x01;
				Data[med][row] <<= 1;
				Data[med][row] = Data[med][row] + cbit[0];
				buff[0]>>=1;
				// ACOMODO 2DO ADC
				cbit[1] = buff[1] & 0x01;
				Data[med][8+row] <<= 1;
				Data[med][8+row] = Data[med][8+row] + cbit[1];
				buff[1]>>=1;
			}
		}
	}
	// SECCION DE FILTRADO DIGITAL DE SEÑALES DE SENSORES 
	// limpiar filtro siguiente
	for( col=0 ; col < 16; col++ ){
		filtro[col][sigfilt] = 0;
	}
	/* Generar valor real (Promedio 16) */
	for( par=0 ; par < 16; par++ ){
		pyftmp=0;
		for( med=0 ; med < 16; med++ ){
			pyftmp += Data[med][par];
		}
		if (par==7 || par == 6 ) {
			filtro[par][sigfilt] = (int16_t)(-pyftmp / 16 );
		}
		else{
			filtro[par][sigfilt] = (int16_t)(pyftmp / 16 );
		}
	}
	sigfilt++;
	sigfilt &= FILTMASK;  
	// Función de filtrado digital 
	for( par=0 ; par < 16; par++ ){
		pyftmp=0;
		for( fil=0 ; fil < FILTER_DEPTH; fil++ ){
			pyftmp += (int32_t)filtro[par][fil];	
		}
		pyftmp =  (pyftmp/FILTER_DEPTH);
		pyftmp -= offsetn[par];
		//pyftmp += offsetp[par];
		Valores[par] = 	((int32_t)pyftmp * factor[par]);// - offset_decneg[par];
		//Valores[par] += offset_decpos[par] ; 	//Tranforma valores reales
	}
	par=4;
	pyftmp=0;
	for( fil=0 ; fil < 16; fil++ ){
		pyftmp += (uint16_t)filtro[par][fil];
	}
	pyftmp =  (pyftmp/16);
	Valores[par] = 	((uint32_t)pyftmp) * factor[par];
	par=3; // Oxigeno arriba de 50% ok
	pyftmp=0;
	for( fil=0 ; fil < 16; fil++ ){
		pyftmp += (uint16_t)filtro[par][fil];
	}
	pyftmp =  (pyftmp/16);
	Valores[par] = 	((uint32_t)pyftmp) * factor[par];
	par=11;
	for( fil=0 ; fil < 16; fil++ ){
		pyftmp += (uint16_t)filtro[par][fil];
	}
	pyftmp =  (pyftmp/16);
	pyftmp -= offsetn[par];
	Valores[par] = 	((uint32_t)pyftmp * factor[par]);
	
	Valores[0] = Valores[0]/10;
	Valores[1] = Valores[1]/10;
	Valores[2] = Valores[2]/10;
	Valores[4] = Valores[4]/100;
	Valores[5] = Valores[5]/10;
	Valores[6] = Valores[6]/100;
	Valores[7] = Valores[7]/100;	
	Valores[9] = Valores[9]/10;
	Valores[10] = Valores[10]/10;
	Valores[11] = Valores[11]/10;
	Valores[13] = Valores[13]/1000;
	Valores[14] = Valores[14]/1000;
	Valores[15] = Valores[15]/10;
	if (Valores[9] & 0x80000000){
		Valores[9]=0;
	}
	if ((Valores[3] <2100000) && (Valores[3]>1900000)){
		Valores[3] = 2100000;
	}
	Valores[15] = (Valores[0]+Valores[1]+Valores[2])/3;
	
	/* Función Para Linealizar y compensar humedad*/
	rht=facteqh[0];
	for(ch=0;ch<3;ch++){
		rht = ((Valores[5] * rht) / 1000) + facteqh[ch+1];
	}
	Valores[5] = rht / (100000000 + ((Valores[15] - 2300000)/10 * 24)) ;
	//Valores[5] = 3500000 + Valores[5]/10;
	/* Función para obtener temperatura de humedad */
	rht = facteqth[0];
	for(ch=0;ch<6;ch++){
		rht = ((Valores[4] * rht) / 1000000) + facteqth[ch+1];
	}
	Valores[4] = rht/10;
	/* Función para compensar piel1 */
	rht = facteqpi[0];
	for(ch=0;ch<6;ch++){
		rht = ((Valores[6] * rht) / 1000000) + facteqpi[ch+1];
	}
	Valores[6] = rht/10;
	/* Función para compensar piel2 */
	rht = facteqpi[0];
	for(ch=0;ch<6;ch++){
		rht = ((Valores[7] * rht) / 1000000) + facteqpi[ch+1];
	}
	Valores[7] = rht/10;
	
	/* Pasar a ASCII */
	for(val=0;val<16;val++){
		tempval = Valores[val];
		Valores_r[val] = Valores[val];
		dieces = 10;
		for(i=0;i<8;i++){
			tempval2 = (tempval % dieces);
			ValAsc[val][7-i] = ((byte)( tempval2 / (dieces/10) )) + 0x30 ;
			tempval = tempval - tempval2; 
			dieces *= 10;
		}
	}
	Adq_alarmas();
	Adq_ChkSenAmb();
}
/*  Función para pedir dato */
void Pedir_Valor_Adq(byte *pos, byte req){
	byte dts;
	if(req == 9){
	for(dts=0;dts<DECIMALES+2;dts++)
		*(pos+dts) = ValAsc[req][puntos[req]-1+dts];
	*(pos+4) = *(pos+3);
	*(pos+3) = *(pos+2);
	*(pos+2) = *(pos+1);
	*(pos+1) = '.';
	}
	else{
		for(dts=0;dts<DECIMALES+2;dts++)
		*(pos+dts) = ValAsc[req][puntos[req]-2+dts];
		*(pos+4) = *(pos+3);
		*(pos+3) = *(pos+2);
		*(pos+2) = '.';
	}
}
/* Funcion valores */
int16_t Adq_Val(byte selec,byte esp){
	uint32_t res;
	byte ia;
	
	res = Valores_r[selec];
	for( ia=7 ; ia > puntos[selec]+esp ; ia--)
		res /= 10;
	
	return (int16_t)res;
}
/* Función de Alarmas */
void Adq_alarmas(void){
	byte al;
	for (al=0;al<16;al++){
		if (senen & 1<<al){
			if (sestdes & 1<<al){
				if (Valores[al]>valdes[al])
					Adq_SetFalla(al);
				else
					Adq_ClrFalla(al);
			}
			else {
				if (Valores[al]<valdes[al])
					Adq_SetFalla(al);
				else
					Adq_ClrFalla(al);
			}
		}
		if (alen & 1<<al){
			if (Valores[al]/10000 < alarml[al])
				switch(al){
					case 0:
						Ala_SetAlarma(TMP_AIRE_BAJA);
					break;
					case 1:
						Ala_SetAlarma(TMP_AIRE_BAJA);
					break;
					case 2:
						Ala_SetAlarma(TMP_AIRE_BAJA);
					break;
					case 3:
						Ala_SetAlarma(OX_BAJO);
					break;
					case 4:
					break;
					case 5:
						Ala_SetAlarma(HUM_BAJA);
					break;
					case 6:
					case 7:
						Ala_SetAlarma(PIEL_BAJA);
					break;
					case 11:
						Ala_SetError(2);			//Báscula
					break;
					default:
					break; //
				}
			else
				switch(al){
					case 0:
						Ala_ClrAlarma(TMP_AIRE_BAJA);
					break;
					case 1:
						Ala_ClrAlarma(TMP_AIRE_BAJA);
					break;
					case 2:
						Ala_ClrAlarma(TMP_AIRE_BAJA);
					break;
					case 3:
						Ala_ClrAlarma(OX_BAJO);
					break;
					case 4:
					break;
					case 5:
						Ala_ClrAlarma(HUM_BAJA);
					break;
					case 6:
					case 7:
						Ala_ClrAlarma(PIEL_BAJA);
					break;
					case 11:
						Ala_ClrError(2);//Báscula
					break;
					default:
					break;
				}
			if (Valores[al]/10000 > alarmh[al])
				switch(al){
					case 0:
						Ala_SetAlarma(TMP_AIRE_ALTA);
					break;
					case 1:
						Ala_SetAlarma(TMP_AIRE_ALTA);
					break;
					case 2:
						Ala_SetAlarma(TMP_AIRE_ALTA);
					break;
					case 3:
						Ala_SetAlarma(OX_ALTO);
					break;
					case 4:
					break;
					case 5:
						Ala_SetAlarma(HUM_ALTA);
					break;
					case 6:
					case 7:
						Ala_SetAlarma(PIEL_ALTA);
					break;
					case 12:
						Ala_SetAlarma(CALEF_ALTA);
					break;
					default:
					break;
				}
			else
				switch(al){
					case 0:
						Ala_ClrAlarma(TMP_AIRE_ALTA);
					break;
					case 1:
						Ala_ClrAlarma(TMP_AIRE_ALTA);
					break;
					case 2:
						Ala_ClrAlarma(TMP_AIRE_ALTA);
					break;
					case 3:
						Ala_ClrAlarma(OX_ALTO);
					break;
					case 4:
					break;
					case 5:
						Ala_ClrAlarma(HUM_ALTA);
					break;
					case 6:
					case 7:
						Ala_ClrAlarma(PIEL_ALTA);
					break;
					case 12:
						Ala_ClrAlarma(CALEF_ALTA);
					break;
					default:
					break;
				}
		}
	}
	if (factor[3]>200 || factor[3]<150)
		Ala_SetError(1);
	else
		Ala_ClrError(1);
	
}
void Adq_SetFalla(byte set){
	switch(set){
		case 0:
			Ala_SetFalla(F_SNAIR);
		break;
		case 1:
			Ala_SetFalla(F_SNAIR);
		break;
		case 2:
			Ala_SetFalla(F_SNAIR);
		break;
		case 3:
			Ala_SetFalla(F_SNOXI);
		break;
		case 4:
		break;
		case 5:
			Ala_SetFalla(F_SNHUM);
		break;
		case 6:
			Ala_SetFalla(F_SNPIE);
		break;
		case 7:
			Ala_SetFalla(F_SNPIE);
		break;
		default:
		break;
	}
}
void Adq_ClrFalla(byte set){
	switch(set){
		case 0:
		case 1:
		case 2:
			Ala_ClrFalla(F_SNAIR);
		break;
		case 3:
			Ala_ClrFalla(F_SNOXI);
		break;
		case 4:
		break;
		case 5:
			Ala_ClrFalla(F_SNHUM);
		break;
		case 6:
			Ala_ClrFalla(F_SNPIE);
		break;
		case 7:
			Ala_ClrFalla(F_SNPIE);
		break;
		default:
		break;	
	}
}
uint16_t Adq_Alarms(void){
	return (uint16_t)(alstl) + (uint16_t)alsth << 8;
}
void zerobasc(void){
	offsetn[9] = filtro[9][sigfilt]-30;
}
void calibbasc(void){
	factor[9] = 5000000/(filtro[9][sigfilt] - offsetn[9]);
}
byte Adq_Test(void){
byte it;
	if (tstcnt==0)
	;	
	tstcnt++;
	if (tstcnt>6 && tstcnt<20){
		if (Ala_GetFalla('F')==0)
			return 20;		
		else 
			return 0;
	}
	if (tstcnt>=20)
		return 1;
		
}
void Adq_SensPot(byte *rec) {
	rec++;
	//*rec=~(*rec);
	if (*rec & 0x02){
		Prot_MotSetVel(0xff, 1);
		Ala_SetAviso(1);
	}
	else{
		Prot_MotSetVel(0xff, 0);
		Ala_ClrAviso(1);
	}
	if (*rec & 0x01)
		Ala_SetAviso(2);
	else
		Ala_ClrAviso(2);
}
void Adq_ChkSenAmb(void){
byte is;
	for(is=0;is<3;is++){
		if((Valores[is] < (Valores[(is+1)%3] - 300000)) || (Valores[is] > (Valores[(is+1)%3] + 300000)))
			Ala_SetFalla(4);
		else
			Ala_ClrFalla(4);
	}
}