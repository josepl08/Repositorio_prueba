
// DEFINICIONES ASOCIADAS AL FORMATO DE LA RECEPCION/ENVIO DE TRAMAS EN UI
#define IN 									0x02 
#define FIN 								0x03
#define ACK 								0x06
// DEFINICIONES ASOCIADAS A TIPOS DE ERRORES EN PROCESAMIENTO DE CRC Y ACK
#define ERR_OK								0x00
#define ERR_NO_ACK							0x10
#define ERR_NO_CRC							0x02
// DEFINICIONES ASOCIADAS A LOS COMANDOS EN EL ENVIO Y RECEPCION DE TRAMAS DE UI
#define	COMM_SEND_PARAM						0x01
#define COMM_SCALE_INIT						0x02
#define COMM_SCALE_CONFIRM1					0x03
#define COMM_SCALE_CONFIRM2					0x04
#define COMM_SCALE_SEND_PATIENT_W			0x05
#define COMM_ALARM_SYSTEM_F					0x06
#define COMM_ALARM_CURRENT_F				0x07
#define COMM_ALARM_AIR_TEMP_L				0x08
#define COMM_ALARM_AIR_TEMP_H				0x09
#define COMM_ALARM_PATIENT_TEMP_L			0x0A
#define COMM_ALARM_PATIENT_TEMP_H			0x0B
#define	COMM_ALARM_PATIENT_SENSOR_F			0x0C
#define COMM_ALARM_AIR_FLOW_F				0x0D
#define COMM_ALARM_DOOR_OPEN				0x0E
#define COMM_ALARM_AIRFILTER_OPEN			0x0F
#define COMM_ALARM_HEATER_F					0x10 
#define COMM_ALARM_WATERLEVEL_L				0x11
#define COMM_ALARM_HR_H 					0x12
#define COMM_ALARM_HR_L 					0x13
#define COMM_ALARM_OXY_H 					0x14
#define COMM_ALARM_OXY_L 					0x15	
#define COMM_ALARM_TEMP_H_HEATER_DISABLE 	0x16
#define COMM_ALARM_CHANGE_OXY_CELL 			0x17
#define COMM_ALARM_HEATER_OVERHEATED 		0x18 
#define COMM_ALARM_AIRTEMP_SENSOR_F			0x19
#define COMM_ALARM_HR_HEATER_F 				0x1A
#define COMM_CONTROL_AIR_TEMP				0x1B
#define COMM_CONTROL_PATIENT_TEMP1			0x1C
#define COMM_CONTROL_PATIENT_TEMP2			0x1D
#define COMM_CONTROL_HR						0x1E
#define COMM_CONTROL_OXY					0x1F
#define COMM_REPORT_TREND					0x20
#define COMM_REPORT_ALARM 					0x21 
#define COMM_REPORT_STOP					0x22
#define COMM_COLUMN_UP						0x23
#define COMM_COLUMN_DOWN					0x24
#define COMM_TRDLMBRG_UP					0x25
#define COMM_TRDLMBRG_DOWN					0x26
#define COMM_ANTITRDLMBRG_UP 				0x27
#define COMM_ANTITRDLMBRG_DOWN				0x28
#define COMM_TRDLMBRG_LEVEL0				0x29
// Definiciones asociadas al estado en que se encuentra la solicitud de reporte de tendencias
#define REP_IDLE							0x00
#define REP_START							0x02
#define REP_IN_PROGRESS						0x04
// Definiciones asociadas al estado en que se encuentra la solicitud de reporte de alarmas
#define ALRM_IDLE 							0x00
#define ALRM_START 							0x02
#define ALRM_IN_PROGRESS					0x04
//	VARIABLES GLOBALES 
byte string_ACK = 0;						// Variable donde se guarda valor de ACK para envío/recepción de tramas
byte RX_buffer[64], TX_buffer[64];			// Variables buffer para almacenar caracteres 
byte RX_cont = 0, RX_string_strt, RX_size;	// Variable para contador de buff Rx, inicio de trama y tamaño de trama
byte weight_stat;							// Variable para guardar estado de la medicion de peso.
bye  report_trend_stat, report_alarm_stat;	// Variables para resguardo de estado de reporte y alarmas
//=======================================METODOS==========================================================
void protocolo_ACKset(bool en){
	string_ACK = (byte)en;
}

byte protocolo_ACKget(){
	return	string_ACK;
}
/*	===============bool 	protocolo_CRCCheck(	byte *string, 	byte size)=============
	- byte 	*string 	apuntador a inicio de trama
	- byte	size 		tamaño de la trama
	Método para realizar la comprobación del crc de las trama ingresada. Para la correcta implementación
	del método se asumen las siguientes condiciones:
		*	Se debe incluir la libreria "crc.h"
		*	La trama está compuesta de elementos de 1 byte.
		* 	La trama tiene el siguiente formato: |In|Comm|Data_1|Data_2|...|Data_n|CRC|Fin|
				+	In: 	Inicio de trama (0x03h)
				+	Comm: 	Comando de la trama 
				+	Data[]:	Datos de la trama (número de datos variable según comando)
				+	CRC:	Cyclic Redundancy Check de la trama
				+	Fin:	Fin de trama (0x02h)
		*	El CRC está compuesto a partir del procesamiento de los componentes Comm y Data[]
======================================================================================= 
*/
bool protocolo_CRCCheck(byte *string, byte size){
	bool		crc_ok 
	uint16_t 	crc_chksum,crc_string;
	crc_string = 0x0000 + ((uint16_t)string[size-3]<<0x0008) + ((uint16_t)string[size-2]);
	crc_chksum = gen_crc16(&string[0], size);
	if (crc_chksum == crc_string){
		crc_ok = TRUE;
	}
	else{
		crc_ok = FALSE;
	}

	return crc_ok;
}

/*	===============byte		protocolo_UISendString(byte *string, byte size)=============
	- byte *string 		apuntador a inicio de trama a enviar
	- byte size 		tamaño de la trama
	Método para el envío de tramas al subsistema de Interfaz de Usuario. De manera general se envía 
	la trama al UI y se queda en espera de una señal de ACK (0x06h). la señal ACk se envía solo si
	la UI ha comprobado el CRC de manera correcta. Si no se recibe ACK se regresa un ERROR. (Manejo
	de error aun no implementado)
======================================================================================= 
*/
byte protocolo_UISendString(byte *string, byte size){
	byte cont = 0, try = 0, ACK_stat = 0, ERR_stat, ACK_received = 0;

	//Agregar método para envío de trama
	while ( ACK_stat == FALSE){
		if (protocolo_ACKget()){
			ACK_stat = TRUE;
			ERR_stat = ERR_OK;
			ACK_received = TRUE;
		}

		// Metodo de delay de 10 ms
		cont++;
		if (cont > 5 && ACK_received == FALSE){
			// Agregar método de envío de trama
			try++;
			if(try > 2){
				ACK_stat = TRUE
				ERR_stat = ERR_NO_ACK;
			}
		}
	}
	return ERR_stat;
}
/*	===============void 	protocolo_UICharReceived()	=============
	- Sin argumentos
	Método para la recepción de caracteres del periferico UART del UI. El método esta pensado para 
	que sea implementado en el ISR del uart cada que reciba un caracter. El caracter recibido será
	resguardado en un buffer y se examinará para identificar los caracteres de inicio y fin de 
	trama, una vez identificada una trama, se llama al método correspondiente para procesarlo
======================================================================================= 
*/
void protocolo_UICharReceived(){
	// método para recibir caractér en RX_buffer[RX_cont]
	if (RX_buffer[RX_cont] == IN){
		RX_string_strt = RX_cont;
	}
	if (RX_buffer[RX_cont] == OUT){
		if (RX_string_strt < RX_cont){
			RX_size = RX_cont - RX_string_strt;
		} 
		else{
			RX_size = RX_cont + 63 - RX_string_strt;
		}
		(void)protocolo_UIReceiveString(&RX_buffer[RX_string_strt], RX_size);
	}
	RX_cont = (RX_cont + 1) & 0x3F; // Se asume un buffer para caracteres de entrada de 64 elementos
}
/*	===============byte 	protocolo_UIReceiveString(byte *string, 	byte size)=============
	- Sin argumentos
	Método para procesamiento de trama recibida, en este método se realiza la comprobación de CRC, 
	si se comprueba el CRC, se envía comando ACK hacia UI y se procesa los datos obtenidos según el
	comando. 
===========================================================================================
*/
byte protocolo_UIReceiveString(byte *string, byte size){
	byte err, comm;
	if(protocolo_CRCCheck(&string[0]), size){
		comm = string[1];
		switch (comm){
			case COMM_CONTROL_AIR_TEMP:
				//
			break;
			case COMM_CONTROL_PATIENT_TEMP1:

			break;
			case COMM_CONTROL_PATIENT_TEMP2:

			break;
			case COMM_CONTROL_HR:

			break;
			case COMM_CONTROL_OXY:

			break;
			case COMM_SCALE_INIT:
				protocolo_ScaleStatSet(COMM_SCALE_INIT);
				protocolo_ScaleMeasurement();
			break;
			case COMM_SCALE_CONFIRM1:
				protocolo_ScaleStatSet(COMM_SCALE_CONFIRM1);
				protocolo_ScaleMeasurement();
			break;
			case COMM_SCALE_CONFIRM2:
				protocolo_ScaleStatSet(COMM_SCALE_CONFIRM2);
				protocolo_ScaleMeasurement();
			break;
			case COMM_REPORT_TREND:
				protocolo_UITrendReport_Request();
			break;
			case COMM_REPORT_ALARM:
				protocolo_UIAlarmReportRequest();
			break;
			case COMM_REPORT_STOP:
				protocolo_ReportStatGet(REP_IDLE);
				protocolo_UITrendReport_Request();
				protolo_UI
			break;
			case COMM_TRDLMBRG_UP:

			break;
			case COMM_TRDLMBRG_DOWN:

			break;
			case COMM_TRDLMBRG_LEVEL0:

			break;
		}
		err = ERR_OK;
	}
	else{
		err = ERR_NO_CRC;
	}
}

/*	===============void 	protocolo_UISendParam(		)==================================
	- Sin argumentos
	Método para envío de parámetros a desplegarse en subsistema UI. Se enviaran los parámetros de
	temperatura de aire (TA), temperatura de piel 1 (P1), temperatura de piel 2 (P2), porcentaje de
	humedad relativa (HR), porcentaje de oxigeno (O2), y porcentaje de potencia de calefactor (POT).
	La trama tiene el siguiente formato: |In|Comm|TA|P1|P2|HR|O2|POT|CRC|Fin|
		+	In: 	Inicio de trama (0x03h)
		+	Comm: 	Comando de la trama 
		+	TA:		Valor de temperatura de aire con formato |D|U|d|
		+ 	P1:	 	Valor de temperatura de piel 1 con formato |D|U|d|
		+ 	P2: 	Valor de temperatura de piel 2 con formato |D|U|d|
		+ 	HR: 	Valor de porcentaje de humedad relativa con formato |C|D|U|
		+ 	O2: 	Valor de pocentaje de oxigeno en aire con formato |C|D|U|
		+ 	POT: 	porcentaje de potencia del calefactor |C|D|U|
		+	CRC:	Cyclic Redundancy Check de la trama
		+	Fin:	Fin de trama (0x02h)
===========================================================================================
*/
void protocolo_UISendParam(){
	uint16_t str_crc;
	TX_buffer[0] = IN;
	TX_buffer[1] = COMM_SEND_PARAM;
	//agregar método para asignar valores de parametros a variable TX_buffer[2-7]
	str_crc = gen_crc16(&TX_buffer[0], 8);
	TX_buffer[8] = byte((str_crc >> 8)&0xFF);
	TX_buffer[9] = byte(str_crc&0xFF);
	TX_buffer[10] = FIN; 
	// agregar método para enviar string TX_buffer[0-10]
}

void protocolo_ScaleStatSet(byte val){
	weight_stat = val;
}
byte protocolo_ScaleStatGet(){
	return weight_stat
}
// Método para envio de valor de peso.
/*	===================void 	scale_measurement(		)==================================
	- Sin argumentos
	Método para el manejo de la medición de peso en la incubadora. Se propone que el método sea
	llamado cada que se reciba un comando relacionado a la incubadora, durante cada estado de la 
	medición se modificará una variable global que guarde el estado de la medición. los estados 
	posibles de la medición son:

		+ IDLE: 	Estado en suspensión de la medición. Durante este estado no se realiza medición 
					peso y se está en espera de comando de activación desde el subsistema UI.
		+ MEAS1_EN:	Estado de activación de la medición 1: Cuando se ingresa a este estado, se inicia
					la medición del peso (se asume que el neonato se está en la cama). Los valores 
					obtenidos de peso se guardan en un vector para su promediación. Se deberá deter-
					minar que las lecturas recibidas son estables para la determinación del peso.
		+ MEAS1_OK:	Estado de finalización de medición 1. En este estado se manda comando al subsis-
					tema UI para que se indique al usuario que debe levantar al neonato. Se permane-
					cerá en este estado hasta que se reciba el comando de inicio de medicion 2 o 
					hasta que haya un timeout y se deermine error.
		+ MEAS2_EN:	Estado de activación de medición 2. En este estado se vuelven a guardar mediciones 
					de peso en vector, se determina estabilidad en la señal y se obtiene medición de 
					peso. Una vez obtenida se calcula la diferencia de peso y se envía a UI.
	Cada caso de la construcción switch se habilitaran flags para que método ajenos a este archvo 
	puedan llamar a los flags para determinar las aaciones a tomar segun el estado de la medicion de 
	peso en que se encuentren.
===========================================================================================
*/
void protocolo_ScaleMeasurement(){
	switch(weight_stat){
		case COMM_SCALE_INIT:
			// hacer cosas
		break;
		case COMM_SCALE_CONFIRM1:
			// hacer cosas
		break;
		case COMM_SCALE_CONFIRM2:
			// hacer cosas
		break;
		case COMM_SCALE_SEND_PATIENT_W:
			// hacer cosas
			weight_stat & 0x00;
		break;
	}

}
// Método para enviar alarmas
/*	===================void protocolo_UISendAlarm( byte alarm)=============================
	- byte alarm 	comando asociado a alarma que se enviará a subsistema UI.
	- byte en 		habilitar/deshabilitar alarma.
	Método para enviar alarma al subsistema UI para su posterior almacenamiento y visualización.
	Las tramas para el envío de comando se enviarín con el siguiente formato: |In|Comm|En|CRC|Fin|
		+ In: 	Inicio de trama (0x03h)
		+ Comm:	Comando asociado a un tipo de alarma
		+ En:	0x01h para habilitar alarma y 0x00h para deshabilitar alarma
		+ CRC:	Chequeo de redundancia cíclica (2 bits)
		+ Fin:	Dinal de trama (0x02h)
===========================================================================================
*/
void protocolo_UISendAlarm(byte alarm, byte en){
	byte str_crc;

	TX_buffer[0] = IN;
	TX_buffer[1] = alarm;
	TX_buffer[2] = en;
	str_crc = gen_crc16(&TX_buffer[0], 3);
	TX_buffer[3] = byte((str_crc >> 8)&0xFF);
	TX_buffer[4] = byte(str_crc&0xFF);
	TX_buffer[5] = FIN;
	protocolo_UISendString(&TX_buffer[0], 6);
}
// Envio de parametro de Referencia
void protocolo_UIControlRef(){
	
}
// Manejo de trendelemburg
void protocolo_ReportStatSet(byte reportStatVal){
 	report_trend_stat = reportStatVal;
}

byte protocolo_ReportStatGet(){
	return = report_trend_stat;
}
// Reporte de Tendencias
void protocolo_UITrendReport_Request(){
	if (report_trend_stat == REP_START){
		// agregar métodos para deshabilitar funciones de incubadora
		report_trend_stat= REP_IN_PROGRESS;
	}
	if (report_trend_stat == REP_IN_PROGRESS){
		// se ingresa a este método cuando se envian tramas por UART. 
		// enviar trama de todos los datos desde #medicion hasta ID paciente
		// 	para parar de enviar los datos se deberá cambiar report_trend_stat desde el método que recibe tramas de UI
		// 
	}
	if (report_trend_stat == REP_IDLE){
		// agregar metodos para habilitar funciones de incubadora.
	}
}

void protocolo_AlarmStatSet(byte reportAlarmVal){
	report_alarm_stat = reportAlarmVal;
}
byte protocolo_AlarmStatGet(){
	return report_alarm_stat;
}
// Reporte de Alarmas
void protocolo_UIAlarmReportRequest(){
	if (report_alarm_stat == ALRM_START){
		// agregar métodos para deshabilitar funciones de incubadora.
		report_alarm_stat =  ALRM_IN_PROGRESS;
	}
	if (report_alarm_stat == ALRM_IN_PROGRESS){
		byte i,j,k;
		byte ala_cont = 0;
		byte ala_stat;
		// se ingresa a este método cuando se envian tramas por UART. 
		// enviar trama de todos los datos desde #medicion hasta hora
		// procesar los bytes de alarmas para enviar los byte correspondientes
		for (i=0; i<6;i++){
			for (j=0; j<4; j++){
				if (){
					ala_stat = (RX_buffer[19 + i] >> (2*j)) & 0x03;
					switch (ala_stat){
						case 0:
							// Método en caso de que alarma se reporte inhabilitada
						break;
						case 1;
							// Método para enviar alarma
						break;
						case 2:
						case 3:
							// Error 
						break;
					}
					ala_cont++;	// Contador para asociarse con los comandos de alarmas e
								// identificar la alarma habilitada/deshabilitada 
				}
			}
		}
		// 	para parar de enviar los datos se deberá cambiar report_trend_stat desde el método que recibe tramas de UI
		// 
	}
}
