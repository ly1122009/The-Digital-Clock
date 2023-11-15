

/* MOCK - (UART + SPI) */
/*[FINAL MOCK] _ NGUYEN CONG LY _ EMB07 */


/* Setting date and time : */
	/* time */ /* Using Timer interrupt delay 1s */
	 /* Hour : 0 <= x <= 23 ( only integer ) */
	 /* Minutes: 0 <= x <= 59 (only integer ) */
	 /* Second: 0 <= x <= 59 (only integer) */
	
	





#include "Prototype.h"
#include "MAX7219.h"

/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/		
/*-----------------------------------------MACRO---------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

#define Current_Mode_Time 1
#define Current_Mode_Date 2
#define Current_Mode_InitialState 0

#define Button_Off 1
#define Button_On 2

#define Alarm_ON 1
#define Alarm_OFF 0

#define Mode_Intensity_Blank_OFF 0
#define Mode_Intensity_Blank_ON 1




/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/		
/*-----------------------------------------VARIABLE UART-------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/


typedef struct {
	volatile uint32 Ngay ;
	volatile uint32 Thang ;
	volatile uint32 Nam ;
}Date_Type;

typedef struct {
	volatile uint32 Gio;
	volatile uint32 Phut;
	volatile uint32 Giay;
}Time_Type;


/* Date */
	/* Thang có 31 ngay : 1, 3, 5, 7, 8, 10, 12 */
	/* Thang có 30 ngay: 4, 6, 9 ,11 */
	/* Thang 2 có 28 hoac 29 ngay */
		/* Thang 2 co 29 ngay neu la nam nhuan : (nam % 400) == 0 || (nam % 4 == 0 && nam % 100 != 0) */
		/* Neu ko thì thang 2 co 28 ngay . */

volatile static uint8 String_Intro[] = "[MOCK FINAL] _ Nguyen Cong Ly ";
volatile static uint8 String_Rule_Date_1[] = "Thang co 31 ngay : 1, 3, 5, 7, 8, 10, 12";
volatile static uint8 String_Rule_Date_2[] = "Thang co 30 ngay : 4, 6, 9 ,11";
volatile static uint8 String_Rule_Date_3[] = "Thang 2 co 29 ngay neu la nam nhuan : (nam % 400) == 0 || (nam % 4 == 0 && nam % 100 != 0)";


volatile static uint8 String_Requirement_Time[] = "Time current : ";
volatile static uint8 String_String_Requirement_Time_Gio[] = "Hour current is (Ex: 05) (00<= Hour <= 24): ";
volatile static uint8 String_String_Requirement_Time_Phut[] = "Minutes current is (Ex: 32) (00 <= Minutes <= 59) : ";
volatile static uint8 String_String_Requirement_Time_Giay[] = "Second current is (Ex: 30) (00 <= Second <= 59):";
volatile static uint8 String_String_Requirement_Time_Syntax[] = "Ex: If the time setting is: 05-32-30";
volatile static uint8 String_String_Requirement_Time_Gio_Syntax[] = "Set Hour current is: 05 ";
volatile static uint8 String_String_Requirement_Time_Phut_Syntax[] = "Set Minutes current is: 32 ";
volatile static uint8 String_String_Requirement_Time_Giay_Syntax[] = "Set Second current is: 30 ";


volatile static uint8 String_Requirement_Alarm[] = "Set Time Alarm : ";
volatile static uint8 String_String_Requirement_Alarm_Gio[] = "Hour Alarm is (Ex: 05) (00<= Hour <= 24): ";
volatile static uint8 String_String_Requirement_Alarm_Phut[] = "Minutes Alarm is (Ex: 32) (00 <= Minutes <= 59) : ";
volatile static uint8 String_String_Requirement_Alarm_Giay[] = "Second Alarm is (Ex: 30) (00 <= Second <= 59): ";
volatile static uint8 String_String_Requirement_Alarm_Syntax[] = "Ex: If the alarm setting is: 05-32-30";
volatile static uint8 String_String_Requirement_Alarm_Gio_Syntax[] = "Set Hour Alarm is: 05 ";
volatile static uint8 String_String_Requirement_Alarm_Phut_Syntax[] = "Set Minutes Alarm is: 32 ";
volatile static uint8 String_String_Requirement_Alarm_Giay_Syntax[] = "Set Second Alarm is: 30 ";


volatile static uint8 String_Requirement_Date[] = "Set date current : ";
volatile static uint8 String_String_Requirement_Date_Ngay[] = "Day current is (Ex: 14) (00 <= Day <= 31): ";
volatile static uint8 String_String_Requirement_Date_Thang[] = "Month current is (Ex: 11) (00 <= Month <= 12): ";
volatile static uint8 String_String_Requirement_Date_Nam[] = "Year current is (Ex: 2023) ( Year > 0 ) : ";
volatile static uint8 String_String_Requirement_Date_Syntax[] = "Ex: If the date setting is: 14.11.2023";
volatile static uint8 String_String_Requirement_Date_Ngay_Syntax[] = "Set Day current is: 14 ";
volatile static uint8 String_String_Requirement_Date_Thang_Syntax[] = "Set Month current is: 11 ";
volatile static uint8 String_String_Requirement_Date_Nam_Syntax[] = "Set Year current is: 2023 ";

volatile static uint8 String_Error[] = "Da nhap sai, vui long nhap lai";

volatile static uint8 String_Receiver[30];

/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/		
/*-----------------------------------------VARIALBE STRUCT--------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/


volatile static Time_Type Time;
volatile static Date_Type Date;
volatile static Time_Type Alarm;


/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/		
/*-----------------------------------------VARIALBE SPI--------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
volatile static uint32 Reg_Temp_TCR_1 = 0;






/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/		
/*-----------------------------------------VARIALBE MAX7219----------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

/* TIME */
volatile static uint32 Time_Gio_Chuc = 0;
volatile static uint32 Time_Gio_Donvi = 0;

volatile static uint32 Time_Phut_Chuc = 0;
volatile static uint32 Time_Phut_Donvi = 0;

volatile static uint32 Time_Giay_Chuc = 0;
volatile static uint32 Time_Giay_Donvi = 0;


/* ALARM */
volatile static uint32 Alarm_Gio_Chuc = 0;
volatile static uint32 Alarm_Gio_Donvi = 0;

volatile static uint32 Alarm_Phut_Chuc = 0;
volatile static uint32 Alarm_Phut_Donvi = 0;

volatile static uint32 Alarm_Giay_Chuc = 0;
volatile static uint32 Alarm_Giay_Donvi = 0;

/* DATE */
volatile static uint32 Date_Ngay_Chuc = 0;
volatile static uint32 Date_Ngay_Donvi = 0;

volatile static uint32 Date_Thang_Chuc = 0;
volatile static uint32 Date_Thang_Donvi = 0;

volatile static uint32 Date_Nam_Ngan = 0;
volatile static uint32 Date_Nam_Tram = 0;
volatile static uint32 Date_Nam_Chuc = 0;
volatile static uint32 Date_Nam_Donvi = 0;


/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/		
/*-----------------------------------------VARIALBE MODE-------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

volatile static uint32 Mode_Current = 0; /* Chi trang thai hien thi cua led 7 doan */
volatile static uint32 Count_SW2 = 0;  /* Bien CountSW2 dem so lan nhan button SW2 */
volatile static uint32 CountTick = 0 ;  /* Dem timer systick */
volatile static uint32 Alarm_Status = 0;  /* Chi trang thai Alarm co kich hoat hay chua */
volatile static uint32 Alarm_Count_Led = 0;  /* Dem so lan sang tat cua led */  
volatile static uint32 Mode_Intensity_Blank = Mode_Intensity_Blank_ON; /* Chi trang thai Mode_Intensity_Blank co Min hay ko. Tuc la turn off led */


/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/		
/*----------------------------------------------------FUNCTIONS------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

 /* Systick Interrupt */
void SysTick_Handler(void);
 /* Function IRQ */
void PORTC_IRQHandler(void);

	/* Functions in main */
uint32 String_to_Int(volatile uint8 String_Source[]);  /* Convert String to int */
void CheckAndTurnOnAlarm(void);  /* Check and turn on Alarm if TIME == ALARM */
void Alarm_Operation(void);  /* Khi alarm kich hoat thi chop tat led 10s roi tat, moi lan 0,5s */



void SysTick_Handler(void){
	
	CountTick++;
}
  /* Function IRQ */
void PORTC_IRQHandler(void){
	if(PORT_C -> PORT_PCR12 & (1u<<24)) /* Interrupt PTC12 (SW_Mode) */
	{
		PORT_C -> PORT_PCR12 |= (1u<<24); /* Write 1 to clear */
	
	  /* Content */
		if( Mode_Current == 3)
		{
			Mode_Current = Current_Mode_Time;
		}
		else 
		{
			Mode_Current++;
		}
	
		
		if(Alarm_Status == Alarm_ON)
		{
			Alarm_Status = Alarm_OFF;
			GPIO_SetHighPIN(GPIO_D,16);
		}
		else
		{
			
		}
		
	if( Mode_Current == Current_Mode_Date && Mode_Intensity_Blank == Mode_Intensity_Blank_ON )
	{

				MAX7219_SetValueForDigits(NumberOfDigit_0,Date_Nam_Donvi,NoUsingDP);  
				MAX7219_SetValueForDigits(NumberOfDigit_1,Date_Nam_Chuc,NoUsingDP); 
				MAX7219_SetValueForDigits(NumberOfDigit_2,Date_Nam_Tram,NoUsingDP);  
				MAX7219_SetValueForDigits(NumberOfDigit_3,Date_Nam_Ngan,NoUsingDP); 
				
				MAX7219_SetValueForDigits(NumberOfDigit_4,Date_Thang_Donvi,UsingDP);  
				MAX7219_SetValueForDigits(NumberOfDigit_5,Date_Thang_Chuc,NoUsingDP); 
				
				MAX7219_SetValueForDigits(NumberOfDigit_6,Date_Ngay_Donvi,UsingDP);  
				MAX7219_SetValueForDigits(NumberOfDigit_7,Date_Ngay_Chuc,NoUsingDP);
				
	}
	
	
	}
	
	if(PORT_C -> PORT_PCR13 & (1u<<24)) /* Interrupt PTC13 (Reduce brightness) */
	{
		PORT_C -> PORT_PCR13 |= (1u<<24); /* Write 1 to clear */
		 
		/* Content */
	
		
		Count_SW2++;
		if(Count_SW2 == 5)  /* If Bam button SW2 5 lan thi tat het led */
		{
			MAX7219_SetValueForDigits(NumberOfDigit_0,ValueOfDigits_blank,NoUsingDP);
			MAX7219_SetValueForDigits(NumberOfDigit_1,ValueOfDigits_blank,NoUsingDP);
			MAX7219_SetValueForDigits(NumberOfDigit_2,ValueOfDigits_blank,NoUsingDP);
			MAX7219_SetValueForDigits(NumberOfDigit_3,ValueOfDigits_blank,NoUsingDP);
			MAX7219_SetValueForDigits(NumberOfDigit_4,ValueOfDigits_blank,NoUsingDP);
			MAX7219_SetValueForDigits(NumberOfDigit_5,ValueOfDigits_blank,NoUsingDP);
			MAX7219_SetValueForDigits(NumberOfDigit_6,ValueOfDigits_blank,NoUsingDP);
			MAX7219_SetValueForDigits(NumberOfDigit_7,ValueOfDigits_blank,NoUsingDP);
			Mode_Intensity_Blank = Mode_Intensity_Blank_OFF;
			Count_SW2 = 0;
		}
		
		else
		{
			MAX7219_SetIntensity((Intensity_Brightness_Max - (Count_SW2 * 3)));  /* Giam do sang theo 4 muc */
			Mode_Intensity_Blank = Mode_Intensity_Blank_ON;  
		}
		

		
		if(Alarm_Status == Alarm_ON)
		{
			Alarm_Status = Alarm_OFF;
			GPIO_SetHighPIN(GPIO_D,16);
			Mode_Intensity_Blank = Mode_Intensity_Blank_ON;
		}
		else
		{
			
		}
		
		
	}
		
	
	
	
}
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/		
/*-----------------------------------------FUNCTIONS UART------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
uint32 String_to_Int(volatile uint8 String_Source[]){
	
	volatile uint32 Temp = 0;
	volatile uint32 Int_result = 0; 
	volatile uint32 Count_while = 0;
	while(String_Source[Count_while] != '\0' )  
	{
		Temp = String_Source[Count_while] - '0';
		Int_result = Int_result * 10 + Temp;
		Count_while++;
	}
	return Int_result;
	
	
}
void CheckAndTurnOnAlarm(void){

	/* Bao thuc (ALARM )*/
	if( Time_Gio_Chuc == Alarm_Gio_Chuc )
	{
		if(Time_Gio_Donvi == Alarm_Gio_Donvi)
		{
			if( Time_Phut_Chuc == Alarm_Phut_Chuc)
			{
				if( Time_Phut_Donvi == Alarm_Phut_Donvi)
				{
					if( Time_Giay_Chuc == Alarm_Giay_Chuc )
					{
						if(Time_Giay_Donvi == Alarm_Giay_Donvi)
						{
							Alarm_Status = Alarm_ON;
						}
					}
					else
					{
					}
				}
				else
				{
				}
			}
			else
			{
			}
			
		}
		else
		{
		}
	}
	else
	{
		
	}
	
	

}
void Alarm_Operation(void){
	
	if(Alarm_Status == Alarm_ON)
	{
	while(CountTick < 500){}
		GPIO_TogglePIN(GPIO_D,16);  /* Toggle led green after 500ms */
		Alarm_Count_Led++;
		if(Alarm_Count_Led == 20)
		{
			Alarm_Status = Alarm_OFF;
			Alarm_Count_Led = 0;
		}
		else
		{
		}
	}
	else
	{
	}
}






/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/		
/*-----------------------------------------FUNCTIONS SPI-------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/


void PhanTich_Int_Donvi(volatile uint32 Input, volatile uint32 *Output_Donvi);
void PhanTich_Int_Chuc(volatile uint32 Input, volatile uint32 *Output_Chuc);
void PhanTich_Int_Tram(volatile uint32 Input, volatile uint32 *Output_Tram);
void PhanTich_Int_Ngan(volatile uint32 Input, volatile uint32 *Output_Ngan);
uint32 CheckNgay_1(volatile uint32 Ngay,volatile uint32 Thang,volatile uint32 Nam);
uint32 CheckNgay_2(volatile uint32 Ngay,volatile uint32 Thang,volatile uint32 Nam);
uint32 CheckTime_1(volatile uint32 Gio_p,volatile uint32 Phut_p,volatile uint32 Giay_p);
uint32 CheckAlarm_1(volatile uint32 Gio_p,volatile uint32 Phut_p,volatile uint32 Giay_p);

void PhanTich_Int_Donvi(volatile uint32 Input, volatile uint32 *Output_Donvi){
	*Output_Donvi = Input % 10;
}

void PhanTich_Int_Chuc(volatile uint32 Input, volatile uint32 *Output_Chuc){
	volatile uint32 Temp = 0;
	Temp = Input / 10; 
	*Output_Chuc = Temp % 10;
}

void PhanTich_Int_Tram(volatile uint32 Input, volatile uint32 *Output_Tram){
	volatile uint32 Temp = 0;
	Temp = Input / 100; 
	*Output_Tram = Temp % 10;
}

void PhanTich_Int_Ngan(volatile uint32 Input, volatile uint32 *Output_Ngan)
{
	volatile uint32 Temp = 0;
	Temp = Input / 1000; 
	*Output_Ngan = Temp % 10;
}

uint32 CheckNgay_1(volatile uint32 Ngay,volatile uint32 Thang,volatile uint32 Nam){
	
	if ( Ngay > 0 && Ngay <= 31 )
	{
		return 0 ; 
	}
	else if ( Thang > 0 && Thang <= 12)
	{
		return 0; 
	}
	else if ( Nam > 0) 
	{
		return 0; 
	}
	else 
	{
		return 1;
	}
}
	
uint32 CheckNgay_2(volatile uint32 Ngay,volatile uint32 Thang,volatile uint32 Nam){
	
	if(Thang == 1 || Thang == 3 || Thang == 5 || Thang == 7 || Thang == 8 || Thang == 10 || Thang == 12 )
	{
		if( Ngay > 0 && Ngay <= 31)
		{
			return 0;
		}
		else 
		{
			return 1;
		}
		
	}
	else if (  Thang == 4 ||  Thang == 6 || Thang == 9 || Thang == 11 )
	{
		if( Ngay > 0 && Ngay <= 30)
		{
			return 0;
		}
		else 
		{
			return 1;
		}
	}
	else if (Thang == 2)
	{
		if( ((Nam % 400 == 0) || ((Nam % 4 == 0) && (Nam % 100 != 0 ))))
		{
			if( Ngay > 0 && Ngay <= 29 )
			{
				return 0;
			}
			else 
			{
				return 1;
			}
		}
		else 
		{
			if( Ngay > 0 && Ngay <= 28)
			{
				return 0;
			}
			else 
			{
				return 1;
			}
		}
		
	}
	else 
	{
		return 1;
	}
	
}


uint32 CheckTime_1(volatile uint32 Gio_p,volatile uint32 Phut_p,volatile uint32 Giay_p){
	
	if (Gio_p <= 23)  /* Do unsigned int nen Gio_p luc nao cung >= 0, khong can them dieu kien Gio_p >= 0 */
	{
		if ( Phut_p <= 59 )
		{
			if(( Giay_p <= 59 ))
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else
		{
				return 1;
		}
	}
	else 
	{
		return 1;
	}
}

uint32 CheckAlarm_1(uint32 Gio_p, uint32 Phut_p, uint32 Giay_p){
	
	if (Gio_p <= 23)  /* Do unsigned int nen Gio_p luc nao cung >= 0, khong can them dieu kien Gio_p >= 0 */
	{
		if ( Phut_p <= 59 )
		{
			if(( Giay_p <= 59 ))
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else
		{
				return 1;
		}
	}
	else 
	{
		return 1;
	}
}




/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/		
/*-----------------------------------------MAIN FUNCTION-------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/


int main(){
	
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/		
/*-----------------------------------------INITIAL CLOCK AND PROTOCOL------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/	
	
		/* Save value of TCR register */
	Reg_Temp_TCR_1  = 0x1F;  /* LPSPI0 -> SCR */ /* 0x1F is value when Reset state */
	
	/* PCC */
		/* UART */
			/* Congfig PCC Source UART - PLL */
			PCC_EnableClock_LPUART(LPUART1,SPLLDIV2);		/* Using PLL clock for UART (36 Mhz)*/
			/* Congfig PCC Source Port */
			PCC_EnableClock_Port(PORT_C);								/* Enable PCC Port C */
			
		
		
		/* SPI */
			/* Congfig PCC Source SPI - OSC */
			PCC_EnableClock_LPSPI(LPSPI0,SOSCDIV2);  		/* Using OSC clock for SPI (8Mhz)*/
			
			/* Congfig PCC Source Port */
			PCC_EnableClock_Port(PORT_E); 							/* Enable Clock Port E */

		/* SCG */
	SCG_Enable_SourceCLock(OSC,0);		/* Select Source clock OSC (8Mhz)*/
	SCG_EnableAndCongfig_PLL(2,36);  /* Using PLL, and Congfig PLL DIV = 2, MUL = 36 => 72 Mhz  */
	SCG_Switch_RunMode_SystemClockSource(PLL,1,2,3);  /* Congfig CORECLK = 72Mhz, BUSCLK = 36Mhz, SLOWCLK = 24Mhz */
	SCG_CongfigDIV_PCC(SPLLDIV2,DIV_0,DIV_2); /* SPLLDIV2 = DIV2 => 36 Mhz */  /* UART */
	SCG_CongfigDIV_PCC(SOSCDIV2,DIV_0,DIV_1);  /* SOSCDIV2 is = 1 */					 /* SPI */
	
		/* Congfig */
			/* LPUART1 */
				/* Congfig and Enable Interrupt for LPUART1*/
				LPUART_Init(LPUART1,PORT_C,7,6,38400,36000,9,Even_Parity);  /* Congfig LPUART1 */	
				
				/* Enable Receiver and Transmitter for LPUART1 */
				LPUART_EnableTransmitter(LPUART1);  /* Enable Transmitter LPUART1 */
				LPUART_EnableReceiver(LPUART1); /* Enable Receiver LPUART1 */
								
			/* SPI */
					/* LPSPI0 (Master) */
						/* 1. Setting SCK/PCS/SOUT/SIN pin */
						/* PTE0: LPSPI0_SCK */
						/* PTE1: LPSPI0_SIN */
						/* PTE2: LPSPI0_SOUT */
						/* PTE6: LPSPI0_PCS2 */

							PORT_E -> PORT_PCR6 |= (2u << 8); /* Peripheral Chip Select 2 */
							PORT_E -> PORT_PCR2 |= (2u << 8); /* LPSPI Serial Data Output (MOSI) */
							PORT_E -> PORT_PCR0 |= (2u << 8); /* LPSPI Serial Clock I/O */
							PORT_E -> PORT_PCR1 |= (2u << 8); /* LPSPI Serial Data Input (MISO) */
							
							
						/* 2. Set TCR  */
							/* Prescaler Value */
							Reg_Temp_TCR_1 &= ~(7u << 27);  /* Prescaler Value = Div 1 */
							/* Congfig CPOL, CPHA */
							Reg_Temp_TCR_1 &= ~(3u << 30);  /* CPOL = 0, CPHA = 0 */
							/* Congfig Framesize + 1 */
							Reg_Temp_TCR_1 = (15u << 0);  /* Set Framesize + 1 = 16 */
							/* Congfig MSB first */
							Reg_Temp_TCR_1 &= ~(1u << 23);  /* Congfig MSB first */
							/* Congfig Chip Select PCS = 1 */
							Reg_Temp_TCR_1 |= (2u << 24);  /* Congfig Chip Select (PCS) = 2 */
							
							LPSPI0 -> TCR = Reg_Temp_TCR_1; /* Send to TCR Register */
							
						/* 3. Set FIFO underrun: */	
							LPSPI0 -> FCR |= (3u << 16);  /* RXWATER */
							LPSPI0 -> FCR |= (3u << 0);  /* TXWATER */
							LPSPI0 -> CFGR1 |= (1u << 3); /* Set no stall */
							
						/* 4. Congfig LPSPI mode */
							LPSPI0 -> CFGR1 |= (1u << 0);  /* Congfig Master mode */
							LPSPI0 -> CR |= (1u << 3);  /* Debug enable */

						/* 5. Enable Module */
							LPSPI0 -> CR |= (1u << 0);  /* Enable Module  */
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/		
/*-----------------------------------------GPIO----------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/

GPIO_Output_Init(GPIO_D,16);  /* Output led PTD16 (Green) */
GPIO_SetHighPIN(GPIO_D,16);  /* Turn off led */

/* Setting Button */
		/* BT1 (PTC12) */
	GPIO_Input_Init(GPIO_C,12);
  
		/* BT2 (PTC13) */
	GPIO_Input_Init(GPIO_C,13);
	
	/* Interrupt */
		/* BT1 - failing edge */
	NVIC_Interrupt_Init(GPIO_C,12,Falling);
	  /* BT2 - falling edge */
	NVIC_Interrupt_Init(GPIO_C,13,Falling);	  
	
	NVIC_Systick_Init(71999u);  /* Using systick timer */
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/		
/*-----------------------------------------SEND DATA MAX7219---------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/						
						
			/* Initial State */	
	/* Nomrmal Operation */
	MAX7219_DisplayTest(DisplayTestMode_NormalOperation);

	/* Decode mode : Using code B */
	MAX7219_DecodeMode(DecodeMode_DecodeB_ForDigit0to7);
	
	/* Display test: Normal Operation */
	MAX7219_Shutdown(Shutdown_NormalOperation);
	
/* Set digit */
	/* Scan limit : Display digit 0 - 7 */
	MAX7219_ScanLimit(ScanLimit_NumberOfDigits_0to7);
	
	/* Display value Time */  /* default value of time is 18:18:00 */
	MAX7219_SetValueForDigits(NumberOfDigit_0,0,NoUsingDP);  
	MAX7219_SetValueForDigits(NumberOfDigit_1,0,NoUsingDP); 
	MAX7219_SetValueForDigits(NumberOfDigit_2,ValueOfDigits_Subtraction,NoUsingDP);  
	MAX7219_SetValueForDigits(NumberOfDigit_3,0,NoUsingDP); 
	MAX7219_SetValueForDigits(NumberOfDigit_4,0,NoUsingDP);  
	MAX7219_SetValueForDigits(NumberOfDigit_5,ValueOfDigits_Subtraction,NoUsingDP); 
	MAX7219_SetValueForDigits(NumberOfDigit_6,8,NoUsingDP);  
	MAX7219_SetValueForDigits(NumberOfDigit_7,1,NoUsingDP); 	
	
	/* Set value Date */  /* default value of date is 01.01.1991 */
	Date_Ngay_Chuc = 0;
	Date_Ngay_Donvi = 1;
	
	Date_Thang_Chuc = 0;
	Date_Thang_Donvi = 1;
	
	Date_Nam_Ngan = 1;
	Date_Nam_Tram = 9;
	Date_Nam_Chuc = 9;
	Date_Nam_Donvi = 1;
	
				
	
	/* Syntax: */
	/* Time: 18-00-00 */
	/* Date: 01.01.1991 */

/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/		
/*--------------------------------------------------UART-------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/		
/*-----------------------------------------SEND SYNTAX-------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
		/* Initial State */
		/* Transmitt Requirement */

			/* Introduce */
		LPUART_SendString(String_Intro,LPUART1);																		 				/* Send [MOCK FINAL] _ Nguyen Cong Ly */
		LPUART_SendChar('\n',LPUART1); 																										  /* Send \n */
	
			/* Rule */
		LPUART_SendString(String_Rule_Date_1,LPUART1);																		 	/* Send Thang co 31 ngay : 1, 3, 5, 7, 8, 10, 12 */
		LPUART_SendChar('\n',LPUART1); 																										  /* Send \n */
		LPUART_SendString(String_Rule_Date_2,LPUART1);																		 	/* Send Thang co 30 ngay : 4, 6, 9 ,11 */
		LPUART_SendChar('\n',LPUART1); 																										  /* Send \n */
		LPUART_SendString(String_Rule_Date_3,LPUART1);																		 	/* Send Thang 2 co 29 ngay neu la nam nhuan : (nam % 400) == 0 || (nam % 4 == 0 && nam % 100 != 0)*/
		LPUART_SendChar('\n',LPUART1); 																										  /* Send \n */
				
			/* Send syntax */
					/* TIME */
		LPUART_SendString(String_String_Requirement_Time_Syntax,LPUART1);										/* Send If the time setting is: 12-32-30 */
		LPUART_SendChar('\n',LPUART1);  																										/* Send \n */
		LPUART_SendString(String_String_Requirement_Time_Gio_Syntax,LPUART1);								/* Send Set Hour current is: 12 */
		LPUART_SendChar('\n',LPUART1);  																										/* Send \n */
		LPUART_SendString(String_String_Requirement_Time_Phut_Syntax,LPUART1);							/* Send Set Minutes current is: 32 */
		LPUART_SendChar('\n',LPUART1);																											/* Send \n */
		LPUART_SendString(String_String_Requirement_Time_Giay_Syntax,LPUART1);							/* Set Second current is: 30 */
	  LPUART_SendChar('\n',LPUART1);																											/* Send \n */
		
					/* ALARM */
		LPUART_SendString(String_String_Requirement_Alarm_Syntax,LPUART1);									/* Send Ex: If the alarm setting is: 15-32-30 */											
	  LPUART_SendChar('\n',LPUART1);																											/* Send \n */ 
		LPUART_SendString(String_String_Requirement_Alarm_Gio_Syntax,LPUART1);							/* Send Set Hour Alarm is: 15 */										
	  LPUART_SendChar('\n',LPUART1);																											/* Send \n */ 
		LPUART_SendString(String_String_Requirement_Alarm_Phut_Syntax,LPUART1);							/* Send Set Minutes Alarm is: 32 */										
	  LPUART_SendChar('\n',LPUART1);																											/* Send \n */
		LPUART_SendString(String_String_Requirement_Alarm_Giay_Syntax,LPUART1);							/* Send Set Second Alarm is: 30 */										
	  LPUART_SendChar('\n',LPUART1);																											/* Send \n */	
		
					/* DATE */
		LPUART_SendString(String_String_Requirement_Date_Syntax,LPUART1);										/* Send Ex: If the date setting is: 14.11.2023 */							
	  LPUART_SendChar('\n',LPUART1);																											/* Send \n */
		LPUART_SendString(String_String_Requirement_Date_Ngay_Syntax,LPUART1);							/* Send Set Day current is: 14 */										
	  LPUART_SendChar('\n',LPUART1);																											/* Send \n */ 
		LPUART_SendString(String_String_Requirement_Date_Thang_Syntax,LPUART1);							/* Send Set Month current is: 11 */										
	  LPUART_SendChar('\n',LPUART1);																											/* Send \n */
		LPUART_SendString(String_String_Requirement_Date_Nam_Syntax,LPUART1);								/* Send Set Year current is: 2023 */									
	  LPUART_SendChar('\n',LPUART1);																											/* Send \n */

		
		
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/		
/*----------------------------------------------TIME-----------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/		
		do{
				/* kiem tra dieu kien neu co loi thi bao Nhap sai, vui long nhap lai */
						if(CheckTime_1(Time.Gio,Time.Phut,Time.Giay))
						{
							LPUART_SendString(String_Error,LPUART1);																	/* Send nhap sai vui long nhap lai */
							LPUART_SendChar('\n',LPUART1);  
						}
			
				/* Feature */
						LPUART_SendString(String_Requirement_Time,LPUART1);																	/* Send Hour time is : " */
						LPUART_SendChar('\n',LPUART1);  																										/* Send \n */

						
				/* Receiver string from UART */
					/* Receive Time (Hour, Minutes, Second) */
						
						/* Receive Hour */
						LPUART_SendString(String_String_Requirement_Time_Gio,LPUART1);											/* Send Hour current is : " */
									for(volatile uint32 Count_for = 0; Count_for < 2 ; Count_for++)
						{
							LPUART_Receiver_Char(LPUART1, (String_Receiver + Count_for) );						/* Receiver String hour */
						}
						LPUART_SendChar('\n',LPUART1);  																						/* Send \n */
						Time.Gio = String_to_Int(String_Receiver);  																/* Convert string hour to int hour */
						
						PhanTich_Int_Chuc(Time.Gio,&Time_Gio_Chuc);  		/* Luu gia tri hang chuc cua gio */
						PhanTich_Int_Donvi(Time.Gio,&Time_Gio_Donvi);   /* Luu gia tri hang don vi cua gio */
						
						/* Receive Minutes */
						LPUART_SendString(String_String_Requirement_Time_Phut,LPUART1);											/* Send Minutes current is : " */
									for(volatile uint32 Count_for = 0; Count_for < 2 ; Count_for++)
						{
							LPUART_Receiver_Char(LPUART1, (String_Receiver + Count_for) );						/* Receiver String Minutes */
						}
						LPUART_SendChar('\n',LPUART1);  																						/* Send \n */
						Time.Phut = String_to_Int(String_Receiver);  																/* Convert string Minutes to int Minutes */
				
						PhanTich_Int_Chuc(Time.Phut,&Time_Phut_Chuc);  		/* Luu gia tri hang chuc cua phut */
						PhanTich_Int_Donvi(Time.Phut,&Time_Phut_Donvi);   /* Luu gia tri hang don vi cua phut */
						
						/* Receive Second */
						LPUART_SendString(String_String_Requirement_Time_Giay,LPUART1);											/* Send Second current is : " */
									for(volatile uint32 Count_for = 0; Count_for < 2 ; Count_for++)
						{
							LPUART_Receiver_Char(LPUART1, (String_Receiver + Count_for) );						/* Receiver String Minutes */
						}
						LPUART_SendChar('\n',LPUART1);  																						/* Send \n */
						Time.Giay = String_to_Int(String_Receiver);  																/* Convert string Second to int Second */
						
						PhanTich_Int_Chuc(Time.Giay,&Time_Giay_Chuc);  		/* Luu gia tri hang chuc cua giay */
						PhanTich_Int_Donvi(Time.Giay,&Time_Giay_Donvi);   /* Luu gia tri hang don vi cua giay */
					}
		while ( CheckTime_1(Time.Gio,Time.Phut,Time.Giay) );   /* Dieu kien cua Time */  /* Neu nhap khong dung se tao vong lap */
	
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/		
/*----------------------------------------------ALARM----------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/		
			do
			{
					/* kiem tra dieu kien neu co loi thi bao Nhap sai, vui long nhap lai */
					if(CheckAlarm_1(Time.Gio,Time.Phut,Time.Giay))
						{
							LPUART_SendString(String_Error,LPUART1);  																				/* Send Nhap sai vui long nhap lai */
							LPUART_SendChar('\n',LPUART1);  																									
						}
					
					/* Feature */
						LPUART_SendString(String_Requirement_Alarm,LPUART1);																/* Send Hour alarm is : " */
						LPUART_SendChar('\n',LPUART1);  																										/* Send \n */
				
				
				/* Receiver string from UART */
					/* Receive Alarm (Hour, Minutes, Second) */
						
						/* Receive Hour */
						LPUART_SendString(String_String_Requirement_Alarm_Gio,LPUART1);											/* Send Hour alarm is : " */
									for(volatile uint32 Count_for = 0; Count_for < 2 ; Count_for++)
						{
							LPUART_Receiver_Char(LPUART1, (String_Receiver + Count_for) );						/* Receiver String hour */
						}
						LPUART_SendChar('\n',LPUART1);  																						/* Send \n */
						Alarm.Gio = String_to_Int(String_Receiver);  																/* Convert string hour to int hour */
						
						PhanTich_Int_Chuc(Alarm.Gio,&Alarm_Gio_Chuc);  		/* Luu gia tri hang chuc cua gio */
						PhanTich_Int_Donvi(Alarm.Gio,&Alarm_Gio_Donvi);   /* Luu gia tri hang don vi cua gio */						
						
						/* Receive Minutes */
						LPUART_SendString(String_String_Requirement_Alarm_Phut,LPUART1);											/* Send Minutes Alarm is : " */
									for(volatile uint32 Count_for = 0; Count_for < 2 ; Count_for++)
						{
							LPUART_Receiver_Char(LPUART1, (String_Receiver + Count_for) );						/* Receiver String Minutes */
						}
						LPUART_SendChar('\n',LPUART1);  																						/* Send \n */
						Alarm.Phut = String_to_Int(String_Receiver);  																/* Convert string Minutes to int Minutes */
				
						PhanTich_Int_Chuc(Alarm.Phut,&Alarm_Phut_Chuc);  		/* Luu gia tri hang chuc cua phut */
						PhanTich_Int_Donvi(Alarm.Phut,&Alarm_Phut_Donvi);   /* Luu gia tri hang don vi cua phut */						
						
						/* Receive Second */
						LPUART_SendString(String_String_Requirement_Alarm_Giay,LPUART1);											/* Send Second Alarm is : " */
									for(volatile uint32 Count_for = 0; Count_for < 2 ; Count_for++)
						{
							LPUART_Receiver_Char(LPUART1, (String_Receiver + Count_for) );						/* Receiver String Minutes */
						}
						LPUART_SendChar('\n',LPUART1);  																						/* Send \n */
						Alarm.Giay = String_to_Int(String_Receiver);  																/* Convert string Second to int Second */					
					
						PhanTich_Int_Chuc(Alarm.Giay,&Alarm_Giay_Chuc);  		/* Luu gia tri hang chuc cua giay */
						PhanTich_Int_Donvi(Alarm.Giay,&Alarm_Giay_Donvi);   /* Luu gia tri hang don vi cua giay */
					}
			while ( CheckAlarm_1(Time.Gio,Time.Phut,Time.Giay));   /* Dieu kien cua alarm */  /* Neu nhap khong dung se tao vong lap */
							
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/		
/*----------------------------------------------DATE-----------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/		
			do{
					/* kiem tra dieu kien neu co loi thi bao Nhap sai, vui long nhap lai */
						if((CheckNgay_1(Date.Ngay,Date.Thang,Date.Nam)) || (CheckNgay_2(Date.Ngay,Date.Thang,Date.Nam)))
						{
							LPUART_SendString(String_Error,LPUART1);  /* Send Nhap sai vui long nhap lai */
							LPUART_SendChar('\n',LPUART1);  
						}
				
					/* Feature */
						LPUART_SendString(String_Requirement_Date,LPUART1);																	/* Send Date current is : " */
						LPUART_SendChar('\n',LPUART1);  																										/* Send \n */
				
				
				/* Receiver string from UART */
					/* Receive Alarm (Hour, Minutes, Second) */
						
						/* Receive day */
						LPUART_SendString(String_String_Requirement_Date_Ngay,LPUART1);											/* Send Day current is : " */
									for(volatile uint32 Count_for = 0; Count_for < 2 ; Count_for++)
						{
							LPUART_Receiver_Char(LPUART1, (String_Receiver + Count_for) );						/* Receiver String Day */
						}
						LPUART_SendChar('\n',LPUART1);  																						/* Send \n */
						Date.Ngay = String_to_Int(String_Receiver);  																/* Convert string Day to int Day */
						
						PhanTich_Int_Chuc(Date.Ngay,&Date_Ngay_Chuc);  		/* Luu gia tri hang chuc cua ngay */
						PhanTich_Int_Donvi(Date.Ngay,&Date_Ngay_Donvi);   /* Luu gia tri hang don vi cua ngay */
						
						/* Receive month */
						LPUART_SendString(String_String_Requirement_Date_Thang,LPUART1);											/* Send month current is : " */
									for(volatile uint32 Count_for = 0; Count_for < 2 ; Count_for++)
						{
							LPUART_Receiver_Char(LPUART1, (String_Receiver + Count_for) );						/* Receiver String month */
						}
						LPUART_SendChar('\n',LPUART1);  																						/* Send \n */
						Date.Thang = String_to_Int(String_Receiver);  																/* Convert string month to int month */
				
						PhanTich_Int_Chuc(Date.Thang,&Date_Thang_Chuc);  		/* Luu gia tri hang chuc cua thang */
						PhanTich_Int_Donvi(Date.Thang,&Date_Thang_Donvi);   /* Luu gia tri hang don vi cua thang */
						
						/* Receive year */
						LPUART_SendString(String_String_Requirement_Date_Nam,LPUART1);											/* Send Year current is : " */
									for(volatile uint32 Count_for = 0; Count_for < 4 ; Count_for++)
						{
							LPUART_Receiver_Char(LPUART1, (String_Receiver + Count_for) );						/* Receiver String Year */
						}
						LPUART_SendChar('\n',LPUART1);  																						/* Send \n */
						Date.Nam = String_to_Int(String_Receiver);  																/* Convert string Year to int Year */								
	
						PhanTich_Int_Ngan(Date.Nam,&Date_Nam_Ngan);  		/* Luu gia tri hang ngan cua nam */
						PhanTich_Int_Tram(Date.Nam,&Date_Nam_Tram);     /* Luu gia tri hang tram cua nam */	
						PhanTich_Int_Chuc(Date.Nam,&Date_Nam_Chuc);  		/* Luu gia tri hang chuc cua nam */
						PhanTich_Int_Donvi(Date.Nam,&Date_Nam_Donvi);   /* Luu gia tri hang don vi cua nam */
						
									for(volatile uint32 Count_for = 0; Count_for < 4 ; Count_for++)
						{
							String_Receiver[Count_for] = '\0';					/* Clear String Year */
						}
					}
				while( (CheckNgay_1(Date.Ngay,Date.Thang,Date.Nam)) || (CheckNgay_2(Date.Ngay,Date.Thang,Date.Nam)) );  /* Dieu kien cua ngay */  /* Neu nhap khong dung se tao vong lap */
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/		
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/			
						
						
						
						
						
						
						

						
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/		
/*---------------------------------------------------SPI-------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/	

						
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/		
/*---------------------------------------------------MODE TIME-------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------------*/						
							
			
		/* Su dung systick timer de dem thoi gian */		
		NVIC_Systick_Enable();	/* Enable Systick lam timer counter */			
					
					
	while(1)
	{
		
			if(Time_Giay_Donvi == 10)  /* Hang chuc cua giay se tang len 1 sau 10s */
		{
			Time_Giay_Chuc++;
			Time_Giay_Donvi = 0;
			 
					if ( Time_Giay_Chuc == 6   )  /* Hang don vi cua phut se tang len 1 sau moi~ 60 s */
					{
						Time_Phut_Donvi++;
						Time_Giay_Donvi = 0;
						Time_Giay_Chuc = 0;
						
						if ( Time_Phut_Donvi == 10 )  /* Hang chuc cua phut se tang len 1 sau moi 10 phut */
							{
								Time_Phut_Chuc++;
								Time_Phut_Donvi = 0;
								
								if (Time_Phut_Chuc == 6 )  /* Hang don vi cua gio se tang len 1 sau moi 60 phut */
								{
									Time_Phut_Chuc = 0;
									Time_Phut_Donvi = 0;
									Time_Gio_Donvi++;
									
									if (Time_Gio_Donvi == 10)  /* Hang chuc cua gio se tang len 1 sau moi 10 gio*/
									{
										Time_Gio_Chuc++;
										Time_Gio_Donvi = 0;
										
												
									}
									else
									{
									}
								}
								else 
								{
								}	
							}
						else
						{
							
						}
												
					}
					else
					{
					}

		}
		else
		{		
		}
		
		if ( Time_Gio_Chuc == 2 )  /* Thoi gian chay den 24h se reset ve 00:00:00 va sang 1 ngay moi */
		{
			if(Time_Gio_Donvi == 4)
			{
				Time_Gio_Chuc = 0;
				Time_Gio_Donvi = 0;
				Time_Phut_Chuc = 0;
				Time_Phut_Donvi = 0;
				Time_Giay_Chuc = 0;
				Time_Giay_Donvi = 0; 
				Date_Ngay_Donvi++;
			}
			else
			{
			}

		}
		else
		{
		}
		
		
		
		if ( Mode_Current == Current_Mode_Time && Mode_Intensity_Blank == Mode_Intensity_Blank_ON)  /* Hien thi time tren led 7 doan */
	{
			MAX7219_SetValueForDigits(NumberOfDigit_0,Time_Giay_Donvi,NoUsingDP);  
			MAX7219_SetValueForDigits(NumberOfDigit_1,Time_Giay_Chuc,NoUsingDP); 
			MAX7219_SetValueForDigits(NumberOfDigit_2,ValueOfDigits_Subtraction,NoUsingDP);  
			MAX7219_SetValueForDigits(NumberOfDigit_3,Time_Phut_Donvi,NoUsingDP); 
			MAX7219_SetValueForDigits(NumberOfDigit_4,Time_Phut_Chuc,NoUsingDP);  
			MAX7219_SetValueForDigits(NumberOfDigit_5,ValueOfDigits_Subtraction,NoUsingDP); 
			MAX7219_SetValueForDigits(NumberOfDigit_6,Time_Gio_Donvi,NoUsingDP);  
			MAX7219_SetValueForDigits(NumberOfDigit_7,Time_Gio_Chuc,NoUsingDP); 	
	}
	
		else
		{
		}
	
	
	CheckAndTurnOnAlarm();
	Alarm_Operation();
	
		/* Kim giay */
	/* hang don vi */
	while (CountTick < 1000);	/* Wait timer in 1000ms */
	Time_Giay_Donvi++;  /* Sau moi 1s thi tang Giay len 1 */
	
	 
	CountTick = 0;	/* Reset Count Timer */
		
	}
	
}



