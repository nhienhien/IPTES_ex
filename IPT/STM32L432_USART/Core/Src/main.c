/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *z
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "stdbool.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAX_ARRAY 100
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

UART_HandleTypeDef hlpuart1;
UART_HandleTypeDef huart1;

RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN PV */
RTC_TimeTypeDef sTime = {0};
RTC_DateTypeDef sDate = {0};
RTC_AlarmTypeDef sAlarm = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_ADC1_Init(void);
static void MX_LPUART1_UART_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void convert_Mode(uint8_t mode);
void ON_OFF_Pulse(void);
//------------------------------Set_Time/Alarm--------------------------------//
uint8_t DecimalToBCD(int number);
void setTime(uint8_t weekday, uint8_t date, uint8_t month, uint8_t year, uint8_t hour, uint8_t minute, uint8_t second);
void getTime(void);
void set_Alarm(uint8_t set_hour, uint8_t set_minute, uint8_t set_second, uint8_t set_day, uint8_t step_alarm);
//-----------------------Checksum-------------------------//
void str2hex(char* input, char* output);
uint8_t NMEA_Checksum(char *Rx_Buffer);
//-----------------------Take Position-------------------------//
uint8_t check_Valid_Header(char *Rx_Buffer, uint8_t case_return);
uint8_t take_Coordinate(char *Rx_Buffer);
uint8_t send_Coordinate_to_Serve(char *lat_Array, char *lng_Array);
float Haversine(float Lat_Center, float Long_Center, float Lat_Location, float Long_Location);
//--------------------------------- Module SIM Function ---------------------------------//
void 	deleteBuffer(char ch[], int sophantu);
void 	resetHardwareSARAU201(void);
void 	contcatenateString(char str1[],char str2[],char str3[],char str4[],char str5[]);
void 	checkMessage(char phoneNumber[]);
void 	nextDay(uint8_t* date, uint8_t* day, uint8_t* month, uint8_t* year);
void 	send_getDataServer(void);
void 	Detaching(void);
void 	outSafeArea(void);
void 	allowDetach(void);
void 	enableDetach(void);
void 	enableSpeaker(void);
void 	CutString(int stringpos, int startpos, char parentstring[], char childstring[], char startchar[], char endchar[]);
uint8_t SaraU201Command(char ch[], char str[], int timeout);
uint8_t resetSoftwareSARAU201(void);
uint8_t deleteSMS(void);
uint8_t initSIM(void);
uint8_t sendMsg(int Case, char PhoneNumber[], char Latitude[], char Longtitude[], char Battery[]);
uint8_t makeCall(char phoneNumber[]);
uint8_t checkPhoneNumber(char phoneNumber[]);
uint8_t DataServer(char Latitude[], char Longtitude[], char Battery[]);
uint8_t checkDataServer(char stat[], char lat_center[], char lng_center[], char rad[]);
uint8_t ConverDay(char Day[]);
uint8_t ConverMonth(char Month[]);
uint8_t getTimeServer(uint8_t* day, uint8_t* date, uint8_t* month, uint8_t* year, uint8_t* hour, uint8_t* minute, uint8_t* second);
uint8_t CheckValidSMS(char Data[]);
uint8_t CheckValidDataServer(char Data[]);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char Rx_Buffer[MAX_ARRAY] = {0};
char Rx_Data[2] = {0};
char lat_Array[10] = {0};
char lng_Array[10] = {0};
char Latitude_ARR[12] 	= "16.078700";
char Longtitude_ARR[12] = "108.161539";
char Rx_data_sim[10];
char Rx_buffer_sim[350];
char Status[1];
char LatCenter[10];
char LngCenter[10];
char Radius[3];
char Thu[3], Ngay[2], Thang[3], Nam[4];
char Gio[2], Phut[2], Giay[2];
char tempARR[100];
char teleNumber[10] 	= "935087813";
char teleNumber2[10] 	= "936055147";
char Battery_ARR[5] 	= "50.2";
char Rssi1[5] = {0};
char Rssi2[5] = {0};
char SaveMes[100] = {0};//"+CMTI: \"ME\",2\r"
char locateMes[5] = {0};
float lat_Location = 16.123456;
float lng_Location = 107.123456;
float distance1;
uint16_t Rx_index = 0;
uint8_t Valid_Coordinate = 5;
uint8_t start_Header_Buffer = 0;
uint8_t valid_GPS = 0;
uint8_t end_Buffer = 0;
uint8_t flag_Alarm = 0;
uint8_t start_Receive = 0;
uint8_t num_error_Coordinate = 0;
uint8_t step_Alarm = 25;
uint8_t Weekday, Date, Month, Year, Hour, Minute, Second;
//uint8_t Weekday = 2, Date = 14, Month = 10, Year = 20, Hour = 5, Minute = 0, Second = 0;
uint16_t Rx_index_sim = 0;
uint8_t checkMes, checkCall, checkSer = 0;
/*Bien thong bao*/
uint8_t Ring = 0;
uint8_t Detach = 0;
uint8_t SafeArea = 0;
uint8_t AllowDetach = 0;
uint8_t status;
uint8_t thu, ngay, thang, nam, gio, phut , giay = 0;
uint8_t Rssi1_num, Rssi2_num = 0;
uint8_t NewSms = 0;
uint8_t checkNewMes = 0;
uint8_t Mes_index = 0;
float latCen, lngCen = 0;
//-----------------------------------------------------------------//
__STATIC_INLINE void DelayMicro(__IO uint32_t micros)
{
micros *= (SystemCoreClock / 1000000) / 9;
/* Wait till done */
while (micros--) ;
}
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
 /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
 set to 'Yes') calls __io_putchar() */
 #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
 #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
//------------------------------Set_Time/Alarm--------------------------------//
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	if (step_Alarm == 25)
		flag_Alarm = 1;
	if (step_Alarm == 35)
		flag_Alarm = 0;
	HAL_ResumeTick();
//	HAL_UART_Receive_IT(&huart1, (uint8_t*)Rx_Data, 1);
}

uint8_t DecimalToBCD(int number)
{
	return (((number / 10) << 4) | (number % 10));
}

void setTime(uint8_t weekday, uint8_t date, uint8_t month, uint8_t year, uint8_t hour, uint8_t minute, uint8_t second)
{
	sDate.WeekDay 	= DecimalToBCD(weekday);
	sDate.Date 		= DecimalToBCD(date);
	sDate.Month 	= DecimalToBCD(month);
	sDate.Year 		= DecimalToBCD(year);

	sTime.Hours		= DecimalToBCD(hour);
	sTime.Minutes	= DecimalToBCD(minute);
	sTime.Seconds	= DecimalToBCD(second);

	HAL_RTC_SetTime(&hrtc, &sTime, FORMAT_BCD);
	HAL_RTC_SetDate(&hrtc, &sDate, FORMAT_BCD);
}

void getTime(void)
{
	HAL_RTC_GetTime(&hrtc, &sTime, FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, FORMAT_BIN);

	Weekday = sDate.WeekDay;
	Date 	= sDate.Date;
	Month 	= sDate.Month;
	Year 	= sDate.Year;

	Hour 	= sTime.Hours;
	Minute 	= sTime.Minutes;
	Second	= sTime.Seconds;
}

void set_Alarm(uint8_t set_hour, uint8_t set_minute, uint8_t set_second, uint8_t set_day, uint8_t step_alarm)
{
//	  uint8_t set_minute_Alarm = (set_minute + step_alarm)%60;
//	  if (set_minute_Alarm < set_minute) set_hour++;
	  uint8_t set_second_Alarm = (set_second + step_alarm)%60;
	  if (set_second_Alarm < set_second) set_minute++;
	  if (set_minute == 60)
	  {
		  set_minute = 0;
		  set_hour++;
	  }

	  sAlarm.AlarmTime.Hours = DecimalToBCD(set_hour);
	  sAlarm.AlarmTime.Minutes = DecimalToBCD(set_minute);
	  sAlarm.AlarmTime.Seconds = DecimalToBCD(set_second_Alarm);
	  sAlarm.AlarmTime.SubSeconds = 0x0;
	  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	  sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY|RTC_ALARMMASK_HOURS;
	  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	  sAlarm.AlarmDateWeekDay = 0x1;
	  sAlarm.Alarm = RTC_ALARM_A;
	  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

//-----------------------Tranceive_UART-------------------------//
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)
	{
		if (Rx_Data[0] == '$')
		{
			start_Receive = 1;
			Rx_index = 0;
		} else
		if ((Rx_Data[0] == '\r')||(Rx_Data[0] == '\n'))
		{
			start_Receive = 0;
//			HAL_UART_Receive_IT(&huart1, (uint8_t*)Rx_Data, 1);
			HAL_UART_Abort_IT(&huart1);
		} else
			HAL_UART_Receive_IT(&huart1, (uint8_t*)Rx_Data, 1);

		if ((start_Receive)&&(Rx_index < MAX_ARRAY))
		{
			Rx_Buffer[Rx_index++] = Rx_Data[0];
			HAL_UART_Receive_IT(&huart1, (uint8_t*)Rx_Data, 1);
		}
		else
		{
			end_Buffer = 1;
//			HAL_UART_Abort_IT(&huart1);
		}
	}
	if(huart -> Instance == LPUART1)
	  {
	    HAL_UART_Receive_IT(&hlpuart1, (uint8_t *)Rx_data_sim, 1);
	    Rx_buffer_sim[Rx_index_sim++] = Rx_data_sim[0];
	    if (Rx_data_sim[0] == 'C') checkNewMes = 1;
	    if (Rx_data_sim[0] == 'M' && checkNewMes == 1) checkNewMes = 2;
	    if (Rx_data_sim[0] == 'T' && checkNewMes == 2) checkNewMes = 3;
	    if (Rx_data_sim[0] == 'I' && checkNewMes == 3) checkNewMes = 4;
	    if (checkNewMes == 4)
	    {
	      SaveMes[Mes_index++] = Rx_data_sim[0];
	      if (Rx_data_sim[0] == '\r')
	      {
	        NewSms = 1;
	        checkNewMes = 0;
	      }
	    }
	  }
}

//-----------------------Checksum-------------------------//
void str2hex(char* Rx_Buffer, char* Rx_Buffer_hex)
{
    int i = 0;
    while(Rx_Buffer[i] != '\0')
    {
        sprintf((char*)(Rx_Buffer_hex+i*2),"%02X", Rx_Buffer[i]);
        i++;
    }
    Rx_Buffer_hex[2*i++] = '\0';
}

uint8_t NMEA_Checksum(char *Rx_Buffer)
{
	char Rx_Buffer_hex[150] = {0};
	str2hex(Rx_Buffer, Rx_Buffer_hex);
	uint8_t sum = 0, i = 1, Check_num = 0;
	char couple[3] = {0}, Checksum_line[3] = {0};
	while (Rx_Buffer[i] != '*')
	{
		strncpy(couple, Rx_Buffer_hex + i*2, 2);
		sum ^= strtol(couple,NULL,16);
		i+=1;
		__NOP();
	}
	strncpy(Checksum_line,Rx_Buffer + i + 1, 2);
	Check_num = strtol(Checksum_line,NULL,16);

	if (sum == Check_num)
	{
		return 1;
	} else return 0;
}
//-----------------------Take Position-------------------------//
uint8_t check_Valid_Header(char *Rx_Buffer, uint8_t case_return)
{
//	while (end_Buffer != 1)
//	{
//		HAL_UART_Receive_IT(&huart1, (uint8_t*)Rx_Data, 1);
//	}
	char header[6] = {0};
	end_Buffer = 0;
	for (uint16_t i = 0; i < MAX_ARRAY; i++)
	{
		if (Rx_Buffer[i] == '$')
		{
			strncpy(header, Rx_Buffer + i + 1, 5);
			if (strstr(header, "GPGGA"))
			{
				if (case_return == 0) return 1;
				else return (i+1);
			} else
			if (strstr(header, "GPRMC"))
			{
				if (case_return == 0) return 2;
				else return (i+1);
			} else

//			HAL_UART_Receive_IT(&huart1, (uint8_t*)Rx_Data, 1);
		    flag_Alarm = 1;
//			return 0;
		}
	}
	return 0;
}

uint8_t take_Coordinate(char *Rx_Buffer)
{
	uint8_t start_pos = check_Valid_Header(Rx_Buffer, 1);
	uint8_t valid_Header = check_Valid_Header(Rx_Buffer, 0);
	while (valid_Header == 0)
	{
		HAL_UART_Receive_IT(&huart1, (uint8_t*)Rx_Data, 1);
		valid_Header = check_Valid_Header(Rx_Buffer, 0);
	}
	uint8_t lat_domain, lng_domain;
	if (check_Valid_Header(Rx_Buffer, 0) == 1)
	{
		lat_domain = 3;
		lng_domain = 5;
	} else
	if (check_Valid_Header(Rx_Buffer, 0) == 2)
	{
		lat_domain = 4;
		lng_domain = 6;
	} else return 0;

	uint8_t domain = 1; //mien trong cau truc NMEA
	for (uint16_t i = start_pos; i < MAX_ARRAY; i++)
	{
		if (Rx_Buffer[i] == ',')
		{
			domain++;
			if (domain == lat_domain)
			{
				for (uint8_t  j = i+1; j < i+9; j++)
					if (Rx_Buffer[j] == ',' )
						return 0;
				strncpy(lat_Array, Rx_Buffer + i + 1, 9);
				__NOP();
			} else
			if (domain == lng_domain)
			{
				for (uint8_t  j = i+1; j < i+10; j++)
					if (Rx_Buffer[j] == ',' )
						return 0;
				strncpy(lng_Array, Rx_Buffer + i + 1, 10);
				__NOP();
				return 1;
			}
		}
	} flag_Alarm = 1;
	return 0;
}
uint8_t send_Coordinate_to_Serve(char *lat_Array, char *lng_Array)
{
	char location_str_1[5] = {0};
	char location_str_2[5] = {0};
	volatile float lat_Location_Update, lng_Location_Update;
	if (take_Coordinate(Rx_Buffer) == 1)
	{
		//-------------------------	Latitude ----------------------------//
		volatile float lat_1 = 0, lat_2 = 0;
		strncpy(location_str_1, lat_Array, 2);	lat_1 = strtof(location_str_1,NULL);
		strncpy(location_str_2, lat_Array + 2, 7);	lat_2 = strtof(location_str_2,NULL);
		lat_Location_Update = lat_1 + lat_2/60;
		gcvt(lat_Location_Update, 9, Latitude_ARR);
		//-------------------------- Longitude --------------------------//
		volatile float lng_1 = 0, lng_2 = 0;
		strncpy(location_str_1, lng_Array, 3);	lng_1 = strtof(location_str_1,NULL);
		strncpy(location_str_2, lng_Array + 3, 7);	lng_2 = strtof(location_str_2,NULL);
		lng_Location_Update = lng_1 + lng_2/60;
		gcvt(lng_Location_Update, 10, Longtitude_ARR);
		__NOP();
		distance1 = Haversine(lat_Location, lng_Location, lat_Location_Update, lng_Location_Update);
	//		if (distance1 > 0.01)
	//		{
				lat_Location = lat_Location_Update;
				lng_Location = lng_Location_Update;
				gcvt(lat_Location, 10, Latitude_ARR);
				gcvt(lng_Location, 10, Longtitude_ARR);
				return 1;
		//}
	} else
		return 0;
}

//uint8_t send_Coordinate_to_Serve(char *lat_Array, char *lng_Array)
//{
////	if (take_Coordinate(Rx_Buffer) == 1)
////	{
//		convert_Coordinate(lat_Array, lng_Array);
//		distance1 = Haversine(lat_Location, lng_Location, lat_Location_Update, lng_Location_Update);
////		if (distance1 > 0.01)
////		{
//			lat_Location = lat_Location_Update;
//			lng_Location = lng_Location_Update;
//			gcvt(lat_Location, 10, Latitude_ARR);
//			gcvt(lng_Location, 10, Longtitude_ARR);
//			return 1;
////		}
//		__NOP();
//	}
//	return 0;
//}

float Haversine(float Lat_Center, float Long_Center, float Lat_Location, float Long_Location)
{
	float distance, hav;
	float lat1, lat2, long1, long2, dlat, dlong;

	/*Chuyen do sang radian*/
	lat1 = Lat_Location / (180 / M_PI);
	long1 = Long_Location / (180 / M_PI);
	lat2 = Lat_Center / (180 / M_PI);
	long2 = Long_Center / (180 / M_PI);

	/*Khoang cach giua latitude va longtitude*/
	dlat = lat2 - lat1;
	dlong = long2 - long1;

	/*Cong thuc haversine*/
	hav = sqrt (pow (sin(dlat / 2), 2) + cos (lat1) * cos (lat2) * pow(sin(dlong/2), 2));
	distance = 2 * 6371 * asin(hav);

	return distance;
}
//-----------------------ON_OF_Pulse-------------------------//
void ON_OFF_Pulse(void)
{
	DelayMicro(300);
    HAL_GPIO_WritePin(PIN_PWR_GPIO_Port, PIN_PWR_Pin, GPIO_PIN_RESET);
    DelayMicro(300);
    HAL_GPIO_WritePin(PIN_PWR_GPIO_Port, PIN_PWR_Pin, GPIO_PIN_SET);
    DelayMicro(300);
    HAL_GPIO_WritePin(PIN_PWR_GPIO_Port, PIN_PWR_Pin, GPIO_PIN_RESET);
    HAL_Delay(800);
}
void convert_Mode(uint8_t mode)
{
	if (mode)
	{
	   while (HAL_GPIO_ReadPin(PIN_WAKEUP_GPIO_Port, PIN_WAKEUP_Pin) != 1)
		   ON_OFF_Pulse();
	} else
	   while (HAL_GPIO_ReadPin(PIN_WAKEUP_GPIO_Port, PIN_WAKEUP_Pin) == 1)
		   ON_OFF_Pulse();
}
//-----------------------Deteching-------------------------//
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == DETACH_PIN_Pin)
  {
    for(int i = 0; i < 100000; i++); //Delay
    if (HAL_GPIO_ReadPin(GPIOA, DETACH_PIN_Pin))
      Detach = 1;
    else Detach = 0;
    EXTI->PR1 |= DETACH_PIN_Pin;
    //Tắt treo Systick
    HAL_ResumeTick();

  }
  if (GPIO_Pin == ALLOW_DETACH_PIN_Pin)
  {
    AllowDetach = 1;
    for(int i = 0; i < 100000; i++); //Delay
    int timeout;
    while(!HAL_GPIO_ReadPin(GPIOA, ALLOW_DETACH_PIN_Pin) & (timeout < 50000)) // �?ợi đến khi thả nút nhấn trong khoảng th�?i gian nhất định
    {
      timeout++;
    }
    EXTI->PR1 |= ALLOW_DETACH_PIN_Pin;
    //Tắt treo Systick
    HAL_ResumeTick();
  }
  if(GPIO_Pin == RI_SIM_Pin)
  {
    /*Ngat danh thuc va thoat Low power mode trên STM32*/
    Ring = 1;
    __NOP();
    EXTI->PR1 |= RI_SIM_Pin; //Xóa pending ngắt
    //Tắt treo Systick
    HAL_ResumeTick();
  }
}

//--------------------------------- Cac ham Module SIM ---------------------------------//
void deleteBuffer(char ch[], int sophantu)
/*Xoa bo dem*/
{
	for (int j = 0; j < sophantu; j++)
	{
		ch[j] = '\0';
 	}
	Rx_index_sim = 0;
}
uint8_t SaraU201Command(char ch[], char str[], int timeout)
{
	uint8_t CheckError = 0;
	do
	{
		HAL_UART_Transmit(&hlpuart1, (uint8_t *)ch, strlen(ch), 1000);

		/*Delay*/
		uint32_t tickstart = HAL_GetTick();
		uint32_t wait = timeout;
		if (wait < HAL_MAX_DELAY)
		{
			wait += (uint32_t)(uwTickFreq);
		}
		while((HAL_GetTick() - tickstart) < wait)
		{
			if (strstr(Rx_buffer_sim, str))
				return 1;
		}
		CheckError++;
		if (CheckError == 2)
		{
			return 0;
		}
	}
	while(strstr(Rx_buffer_sim, str)==NULL);
	return 1;
}
void resetHardwareSARAU201(void)
/*Reset bang chan PWR_ON*/
{
	//Tat module
	HAL_GPIO_WritePin(PWR_SIM_GPIO_Port, PWR_SIM_Pin, GPIO_PIN_RESET);
	HAL_Delay(2000);
	HAL_GPIO_WritePin(PWR_SIM_GPIO_Port, PWR_SIM_Pin, GPIO_PIN_SET);

	//Bat Module
	HAL_GPIO_WritePin(PWR_SIM_GPIO_Port, PWR_SIM_Pin, GPIO_PIN_RESET);
	DelayMicro(70);
	HAL_GPIO_WritePin(PWR_SIM_GPIO_Port, PWR_SIM_Pin, GPIO_PIN_SET);
}
uint8_t resetSoftwareSARAU201(void)
/*Reset bang lenh AT*/
{
	//Set the MT to minimum functionality
	deleteBuffer(Rx_buffer_sim, Rx_index_sim);
	if (!SaraU201Command("AT+CFUN=0\r\n", "OK", 5000)) return 0;
	//Set the MT to full functionality
	deleteBuffer(Rx_buffer_sim, Rx_index_sim);
	if (!SaraU201Command("AT+CFUN=1\r\n", "OK", 5000)) return 0;
	return 1;
}
void contcatenateString(char str1[],char str2[],char str3[],char str4[],char str5[])
/*Noi chuoi*/
{
	strcat(str1, str2);
	strcat(str1, str3);
	strcat(str1, str4);
	strcat(str1, str5);
}
uint8_t deleteSMS(void)
{
	//Xoa tin nhan
	deleteBuffer(Rx_buffer_sim, Rx_index_sim);
	if (!SaraU201Command("AT+CMGD=1,4\r\n", "OK", 5000)) return 0;
	return 1;
}
uint8_t checkPhoneNumber(char phoneNumber[])
{
	if (Ring == 1)
	{
		//List current calls
		deleteBuffer(Rx_buffer_sim, Rx_index_sim);
		if (!SaraU201Command("AT+CLCC\r\n", "OK", 5000)) return 0;
		if (strstr(Rx_buffer_sim, phoneNumber))
		{
			//Neu dung SDT thi lam gi do
		}
	}
	return 1;
}
uint8_t CheckValidSMS(char Data[])
{
	uint8_t i;
	for (i = 0; i < strlen(Data); i++)
	{
		if (Data[i] == 'C' && Data[i + 1] == 'M' && Data[i + 2] == 'T' && Data[i + 3] == 'I')
			return i + 4;
	}
	return 0;
}
uint8_t CheckValidDataTime(char Data[])
{
	uint8_t i;
	for (i = 0; i < strlen(Data); i++)
	{
		if (Data[i] == 'D' && Data[i + 1] == 'a' && Data[i + 2] == 't' && Data[i + 3] == 'e')
			return i + 4;
	}
	return 0;
}
uint8_t CheckValidDataServer(char Data[])
{
	uint8_t i;
	for (i = 0; i < strlen(Data); i++)
	{
		if (Data[i] == '|')
			return i + 1;
	}
	return 0;
}
uint8_t checkRSSI(void)
{
	deleteBuffer(Rssi1, 5);
	deleteBuffer(Rssi2, 5);
	CutString(1, 0, Rx_buffer_sim, Rssi1, ":", ",");
	CutString(1, 0, Rx_buffer_sim, Rssi2, ",", "\r");
	Rssi1_num = atoi(Rssi1);
	Rssi2_num = atoi(Rssi2);
	if (Rssi1_num != 99 && Rssi2_num != 99)
		return 1;
	return 0;
}

void checkMessage(char phoneNumber[])
{
  //Lay vị tri luu tru cua tin nhan
  uint8_t locate_sms;
  CutString(1, 0, SaveMes, locateMes, ",", "\r");
  locate_sms = atoi(locateMes);
  if (locate_sms > 0 && locate_sms <= 340)
  {
    deleteBuffer(SaveMes, 100);
    Mes_index = 0;

    //Doc tin nhan tu vi tri luu tru
    deleteBuffer(tempARR, 100);
    deleteBuffer(Rx_buffer_sim, Rx_index_sim);
    contcatenateString(tempARR, "AT+CMGR=", locateMes, "\r\n", "");
    SaraU201Command(tempARR, "OK\r\n", 5000);

    //Kiem tra co dung noi dung tin nhan
    if(strstr(Rx_buffer_sim, "TOADO"))
    {
      if (!sendMsg(3, teleNumber, Latitude_ARR, Longtitude_ARR, Battery_ARR)) checkMes = 1;
    }
    else if(strstr(Rx_buffer_sim, "BATLOA"))
    {
      enableSpeaker();
    }
    else if(strstr(Rx_buffer_sim, "THAO"))
    {
      enableDetach();
    }
  }
  deleteBuffer(Rx_buffer_sim, Rx_index_sim);
  SaraU201Command("AT+CMGD=1,4\r\n", "OK", 5000);
}

uint8_t checkDataServer(char stat[], char lat_center[], char lng_center[], char rad[])
{
  uint8_t vitri = 0;
  if (strstr(Rx_buffer_sim, "+URDFILE" ) && strstr(Rx_buffer_sim, "\r\nOK"))
  {
    vitri = CheckValidDataServer(Rx_buffer_sim);
    if (vitri != 0)
    {
      /*Xoa mang*/
      deleteBuffer(stat, strlen(stat));
      deleteBuffer(lat_center, strlen(lat_center));
      deleteBuffer(lng_center, strlen(lng_center));
      deleteBuffer(rad, strlen(rad));
      /*Tach chuoi du lieu*/
//      CutString(2, vitri, Rx_buffer_sim, lat_center, ":", "-");
//      CutString(3, vitri, Rx_buffer_sim, lng_center, ":", "-");
//      CutString(4, vitri, Rx_buffer_sim, rad, ":", "-");
//      CutString(5, vitri, Rx_buffer_sim, stat, ":", "\"");
      CutString(4, vitri, Rx_buffer_sim, lat_center, " ", " ");
      CutString(7, vitri, Rx_buffer_sim, lng_center, " ", " ");
      CutString(10, vitri, Rx_buffer_sim, rad, " ", " ");
      CutString(13, vitri, Rx_buffer_sim, stat, " ", "\"");
      //Chuyen chuoi thanh so
      status = atoi(Status);
      return 1;
    }
    return 0;
  }
  return 0;
}

uint8_t initSIM(void)
// Khoi tao module SIM, cau hinh mang,...
{
	while(HAL_GPIO_ReadPin(STAT_SIM_GPIO_Port, STAT_SIM_Pin)==0)
	{
	}
	/*Kiem tra ket noi voi module sim*/
	deleteBuffer(Rx_buffer_sim, Rx_index_sim);
	if (!SaraU201Command("AT\r\n", "OK", 500)) return 0;
	/*tat chuc nang phan hoi, Echo*/
	deleteBuffer(Rx_buffer_sim, Rx_index_sim);
	if (!SaraU201Command("ATE0\r\n", "OK", 500)) return 0;
	/*Kiem tra Sim co hoat dong khong*/
	deleteBuffer(Rx_buffer_sim, Rx_index_sim);
	if (!SaraU201Command("AT+CPIN?\r\n", "+CPIN: READY", 500)) return 0;
	/*Cau hinh dinh dang tin nhan*/ /*Set msg message format (0:PDU mode/ 1: Text mode)*/
	deleteBuffer(Rx_buffer_sim, Rx_index_sim);
	if (!SaraU201Command("AT+CMGF=1\r\n", "OK", 500)) return 0;
	/*Thong bao khi co tin nhan moi */
	deleteBuffer(Rx_buffer_sim, Rx_index_sim);
	if (!SaraU201Command("AT+CNMI=1,1\r\n", "OK", 500)) return 0;
	/*Disable flow control*/
	deleteBuffer(Rx_buffer_sim, Rx_index_sim);
	if (!SaraU201Command("AT&K0\r\n", "OK", 500)) return 0;
	/*Enable Power Saving is controlled by RTS*/
	deleteBuffer(Rx_buffer_sim, Rx_index_sim);
	if (!SaraU201Command("AT+UPSV=2\r\n", "OK", 500)) return 0;
	/*Network registration status*/
	deleteBuffer(Rx_buffer_sim, Rx_index_sim);
	if (!SaraU201Command("AT+UREG?\r\n", "+UREG: 0,6\r\n\r\nOK", 5000)) return 0;
	deleteBuffer(Rx_buffer_sim, Rx_index_sim);
	if (!SaraU201Command("AT&W\r\n", "OK\r\n", 500)) return 0;
	deleteBuffer(Rx_buffer_sim, Rx_index_sim);
	return 1;
}
uint8_t sendMsg(int Case, char PhoneNumber[], char Latitude[], char Longtitude[], char Battery[])
{
	char Phone[12] = {0};
	deleteBuffer(Rx_buffer_sim, Rx_index_sim);
	deleteBuffer(tempARR, 100); //Xóa mảng tempARR
	//Noi chuoi so dien thoai
	strcat(Phone, "+84");
	strcat(Phone, PhoneNumber);
	//Noi chuoi de gui lenh AT
	strcat(tempARR, "AT+CMGS=\"");
	strcat(tempARR, Phone);
	strcat(tempARR, "\"\r");
	if (!SaraU201Command(tempARR, ">", 1000)) return 0;
	if (Case == 1)
	{
		/*Thông báo ra khoi vùng an toàn + Link toa do + Phan tram pin*/
		deleteBuffer(tempARR, 100);
		strcat(tempARR, "OutSafeArea\n");
		strcat(tempARR, "http://maps.google.com/maps?q=");
		strcat(tempARR, Latitude);
		strcat(tempARR, ",");
		strcat(tempARR, Longtitude);
		strcat(tempARR, "\n");
//		strcat(tempARR, "Battery:");
//		strcat(tempARR, "\x1A");
		deleteBuffer(Rx_buffer_sim, Rx_index_sim);
		if (!SaraU201Command(tempARR, "OK", 5000)) return 0;
	}
	else if (Case == 2)
	{
		/*Thong bao thao thiet bi+ Link toa do + Phan tram pin*/
		deleteBuffer(tempARR, 100);
		strcat(tempARR, "Detaching\n");
		strcat(tempARR, "http://maps.google.com/maps?q=");
		strcat(tempARR, Latitude);
		strcat(tempARR, ",");
		strcat(tempARR, Longtitude);
		strcat(tempARR, "\n");
//		strcat(tempARR, "Battery:");
//		strcat(tempARR, "\x1A");
		deleteBuffer(Rx_buffer_sim, Rx_index_sim);
		if (!SaraU201Command(tempARR, "OK", 5000)) return 0;
	}
	else if (Case == 3)
	{
		/*Gửi link đuong dan toa do + Phan tram pin*/
		deleteBuffer(tempARR, 100);
		strcat(tempARR, "http://maps.google.com/maps?q=");
		strcat(tempARR, Latitude);
		strcat(tempARR, ",");
		strcat(tempARR, Longtitude);
		strcat(tempARR, "\n");
		strcat(tempARR, "Battery:");
		strcat(tempARR, "\x1A");
		deleteBuffer(Rx_buffer_sim, Rx_index_sim);
		if (!SaraU201Command(tempARR, "OK", 5000)) return 0;
	}
	return 1;
}
uint8_t makeCall(char phoneNumber[])
{
	char Phone[12] = {0};
	uint32_t count = 0;
	uint8_t Call = 0; //Bien dem so cuoc goi
	//Cai dat thong bao tinh trang cuoc goi(Set reporting call status)
	deleteBuffer(Rx_buffer_sim, 300);
	if (!SaraU201Command("AT+UCALLSTAT=1\r\n", "OK", 1000)) return 0;
	do
	{
		//Xoa bo dem
		deleteBuffer(Rx_buffer_sim, 256);
		deleteBuffer(tempARR, 100);
		deleteBuffer(Phone, 12);
		//Noi chuoi
		strcat(Phone, "+84");
		strcat(Phone, phoneNumber);
		strcat(tempARR, "ATD");
		strcat(tempARR, Phone);
		strcat(tempARR, ";\r\n");
//		contcatenateString(Phone,"+84",phoneNumber,"","");
//		contcatenateString(tempARR, "ATD",Phone, ";\r\n", "");
		//Dem thoi gian
		count = HAL_GetTick();
		//Goi dien den khi nhac may
		if (!SaraU201Command(tempARR, "OK", 2000)) return 0;
		while (HAL_GetTick() - count < 40000)//HAL_GetTick() - count < 43000
		{
			if (strstr(Rx_buffer_sim, "0"))
				break;
		}
		//Tat cuoc goi
		if (!SaraU201Command("ATH\r\n", "OK", 500)) return 0;
		Call++;
		HAL_Delay(3000);
		if (Call == 5) break;
	}
	while(HAL_GetTick() - count > 40000);
	return 1;
}

void CutString(int stringpos, int startpos, char parentstring[], char childstring[], char startchar[], char endchar[])
{
	//Du lieu bat dau tu day
	int count = 0;
	for (int i = startpos; i < strlen(parentstring); i++)
	{
		if (parentstring[i] == startchar[0])
			count++;
		if (count == stringpos)
		{
			int t = 0;
			for (int j = i + 1; j < strlen(parentstring); j++)
			{
				if (parentstring[j] == endchar[0]) break;
				childstring[t] = parentstring[j];
				t++;
			}
			break;
		}
	}
}
uint8_t DataServer(char Latitude[], char Longtitude[], char Battery[])
{
	/*Check the GPRS attach status*/
	while(HAL_GPIO_ReadPin(STAT_SIM_GPIO_Port, STAT_SIM_Pin)==0)
	{
	}
	deleteBuffer(Rx_buffer_sim, Rx_index_sim);
	if (!SaraU201Command("AT+CSQ\r\n", "OK\r\n", 10000)) return 0;
	if (!checkRSSI()) return 0;
	deleteBuffer(Rx_buffer_sim, Rx_index_sim);
	if (!SaraU201Command("AT+CGATT?\r\n", "+CGATT: 1\r\n\r\nOK\r\n", 10000)) return 0;
	/*Activate the GPRS connection.*/
	deleteBuffer(Rx_buffer_sim, Rx_index_sim);
	if (!SaraU201Command("AT+UPSDA=0,3\r\n", "OK\r\n", 30000))
	{
		SaraU201Command("AT+UPSDA=0,4\r\n", "OK\r\n", 5000);
		return 0;
	}
	/*Set the server domain name*/
	deleteBuffer(Rx_buffer_sim, Rx_index_sim);
	if (!SaraU201Command("AT+UHTTP=0,1,\"ipteks.tapit.vn\"\r\n", "OK\r\n", 2000)) return 0;

	/*Xoa bo dem va noi chuoi*/
	deleteBuffer(tempARR, 100);

	strcat(tempARR, "AT+UHTTPC=0,1,\"/insert.php?device=1&lat=");
	strcat(tempARR, Latitude);
	strcat(tempARR, "&lng=");
	strcat(tempARR, Longtitude);

	strcat(tempARR, "&pin=");
	strcat(tempARR, Battery);
	strcat(tempARR, "\",\"get.ffs\"");
	strcat(tempARR, "\r\n");
//	contcatenateString(tempARR, "AT+UHTTPC=0,1,\"/insert.php?device=1&lat=", Latitude, "&lng=", Longtitude);
//	contcatenateString(tempARR, "&pin=",Battery, "\",\"get.ffs\"", "\r\n");
	/*GET request to HTTP server*/
	deleteBuffer(Rx_buffer_sim, 100);
	if (!SaraU201Command(tempARR, "+UUHTTPCR: 0,1,1\r\n", 10000))
	{
		SaraU201Command("AT+UPSDA=0,4\r\n", "OK", 5000);
		return 0;
	}
	/*Context deactivation*/
	deleteBuffer(Rx_buffer_sim, Rx_index_sim);
	if (!SaraU201Command("AT+UPSDA=0,4\r\n", "OK", 5000)) return 0;
	/*Read file*/
	deleteBuffer(Rx_buffer_sim, 300);
	if (!SaraU201Command("AT+URDFILE=\"get.ffs\"\r\n", "\r\nOK", 30000)) return 0; //
	return 1;
}
uint8_t getTimeServer(uint8_t* day, uint8_t* date, uint8_t* month, uint8_t* year, uint8_t* hour, uint8_t* minute, uint8_t* second)
{
	uint8_t vitri = 0;
	vitri = CheckValidDataTime(Rx_buffer_sim);
	if (vitri != 0)
	{
		//Xoa cac buffer
		deleteBuffer(Thu, 3);
		deleteBuffer(Ngay, 2);
		deleteBuffer(Thang, 2);
		deleteBuffer(Nam, 4);
		deleteBuffer(Gio, 2);
		deleteBuffer(Phut, 2);
		deleteBuffer(Giay, 2);

		CutString(1, vitri, Rx_buffer_sim, Thu, " ", ",");
		CutString(2, vitri, Rx_buffer_sim, Ngay, " ", " ");
		CutString(3, vitri, Rx_buffer_sim, Thang, " ", " ");
		CutString(4, vitri, Rx_buffer_sim, Nam, " ", " ");
		CutString(5, vitri, Rx_buffer_sim, Gio, " ", ":");
		CutString(2, vitri, Rx_buffer_sim, Phut, ":", ":");
		CutString(3, vitri, Rx_buffer_sim, Giay, ":", " ");

		//Chuyen chuoi thanh so
		*day	= ConverDay(Thu);
		*date	= atoi(Ngay);
		*month	= ConverMonth(Thang);
		*year	= atoi(Nam) % 100;
		*hour	= (atoi(Gio) + 7) % 24;
		*minute = atoi(Phut);
		*second = atoi(Giay);
		return 1;
	}
	return 0;
}
uint8_t ConverDay(char Day[])
{
	if (strstr(Day, "Mon")) return 1;
	else if (strstr(Day, "Tue")) return 2;
	else if (strstr(Day, "Wed")) return 3;
	else if (strstr(Day, "Thu")) return 4;
	else if (strstr(Day, "Fri")) return 5;
	else if (strstr(Day, "Sat")) return 6;
	else return 7; //if (strstr(Day, "Sun"))
}
uint8_t ConverMonth(char Month[])
{
	if (strstr(Month, "Jan")) return 1;
	else if (strstr(Month, "Feb")) return 2;
	else if (strstr(Month, "Mar")) return 3;
	else if (strstr(Month, "Apr")) return 4;
	else if (strstr(Month, "May")) return 5;
	else if (strstr(Month, "Jun")) return 6;
	else if (strstr(Month, "Jul")) return 7;
	else if (strstr(Month, "Aug")) return 8;
	else if (strstr(Month, "Sep")) return 9;
	else if (strstr(Month, "Oct")) return 10;
	else if (strstr(Month, "Nov")) return 11;
	else  return 12;
}
void nextDay(uint8_t* date, uint8_t* day, uint8_t* month, uint8_t* year)
{
	/*Thu*/
	if (*date == 7) *date = 1;
	else *date += 1;
	/*Ngay thang nam*/
	if (*day == 31)
	{
		*day = 1;
		if (*month != 12) *month += 1;
		else
		{
			*month = 1;
			*year += 1;
		}
	}
	else if (*day == 30 && (*month == 4 || *month == 6 || *month == 9 || *month == 11))
	{
		*day = 1;
		*month += 1;
	}
	else if ((*day == 29 || *day == 28) && (*month == 2))
	{
		if (*day == 29 || (*day == 28 && ((*year % 400 != 0) && (*year % 4 != 0 || *year % 100 == 0))))
		{
			*day = 1;
			*month += 1;
		}
		else
			*day += 1;
	}
	else
		*day += 1;
}
uint8_t checkSafeArea(char lat_Center[], char lng_Center[], char Rad[], char lat_Current[], char lng_Current[])
{
	uint16_t d;
	float lat_Cen, lng_Cen, lat_Cur, lng_Cur;
	int rad = atoi(Rad);
	lat_Cen = strtof(lat_Center, NULL);
	lng_Cen = strtof(lng_Center, NULL);
	lat_Cur = strtof(lat_Current, NULL);
	lng_Cur = strtof(lng_Current, NULL);

	d = Haversine(lat_Cen, lng_Cen, lat_Cur , lng_Cur);

	if (d > (float)rad)
		return 0;
	return 1;
}
void send_getDataServer(void) //bo comment khi hoan thien
{
	/*Gui va nhan du lieu tu Server*/
	if (!DataServer(Latitude_ARR, Longtitude_ARR, Battery_ARR)) checkSer = 1;
	/*lay thoi gian tu Server*/
	if (getTimeServer(&thu, &ngay, &thang, &nam, &gio, &phut, &giay))
	{
		if (atoi(Gio) + 7 >= 24)
			nextDay(&thu, &ngay, &thang, &nam);
		//Lay TG tu RTC
		getTime();
		if (Hour != gio || Minute != phut || Weekday != thu || Date != ngay || Month != thang || Year != nam)
		{
			setTime(thu, ngay, thang, nam, gio, phut, giay);
		}
	}
	if (checkDataServer(Status, LatCenter, LngCenter, Radius))
	{
		/*Kiem tra vi tri hien tai co nam trong vung an toan*/
		if (!checkSafeArea(LatCenter, LngCenter, Radius, Latitude_ARR, Longtitude_ARR))
			SafeArea = 1;
		else SafeArea = 0;
	}
	deleteBuffer(Rx_buffer_sim, 300);
}
void Detaching(void)
{
	//Gưi tin nhan thong bao bi thao
	if (!sendMsg(2, teleNumber, Latitude_ARR, Longtitude_ARR, Battery_ARR)) checkMes = 1;
	//Goi dien kiem tra co duoc nhac may
	if (!makeCall(teleNumber)) checkCall = 1;
	//Bat Loa bao dong
	//-------------------------------------
	//Doi tg cap nhap thanh 2ph
}
void outSafeArea(void)
{
	//Gui tin nhan thong bao ra khoi vung an toan
	if (!sendMsg(1, teleNumber, Latitude_ARR, Longtitude_ARR, Battery_ARR)) checkMes = 1;
	HAL_Delay(3000);
	//Goi dien kiem tra co duoc nhac may
	if (!makeCall(teleNumber)) checkCall = 1;
}
void allowDetach(void)
{
	// Gui va lay du lieu tu Server
	if (!DataServer(Latitude_ARR, Longtitude_ARR, Battery_ARR)) checkSer = 1;

	//Tach chuoi lay gia tri can thiet
	checkDataServer(Status, LatCenter, LngCenter, Radius);
}
void enableSpeaker(void)
{
	uint32_t start = HAL_GetTick();
	HAL_GPIO_WritePin(SPEAKER_PIN_GPIO_Port, SPEAKER_PIN_Pin, GPIO_PIN_SET);

	while ((HAL_GetTick() - start) < 60000)
	{
	}
	HAL_GPIO_WritePin(SPEAKER_PIN_GPIO_Port, SPEAKER_PIN_Pin, GPIO_PIN_RESET);
}
void enableDetach(void)
{
	uint32_t start = HAL_GetTick();
	while ((HAL_GetTick() - start) < 60000)
	{
		if (HAL_GPIO_ReadPin(DETACH_PIN_GPIO_Port, DETACH_PIN_Pin)==0)
			break;
	}
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_ADC1_Init();
  MX_LPUART1_UART_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&hlpuart1, (uint8_t *)Rx_data_sim, 1);
  //Tat power saving
  HAL_GPIO_WritePin(RTS_SIM_GPIO_Port, RTS_SIM_Pin, RESET);
  //Bat module (set chan PWR)
  HAL_GPIO_WritePin(PIN_PWR_GPIO_Port, PIN_PWR_Pin, SET);
  //Cho khoi dong module
  HAL_Delay(4000);
  //Initial Module Sim
//  SaraU201Command("AT+URDFILE=\"get.ffs\"\r\n", "\r\nOK", 30000);


  while (!initSIM())
  {
	  resetHardwareSARAU201();
  }
  //Turn on Power Saving module SIM
  HAL_GPIO_WritePin(RTS_SIM_GPIO_Port, RTS_SIM_Pin, SET);
  HAL_UART_Receive_IT(&huart1, (uint8_t*)Rx_Data, 1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {//nhien
	  if (flag_Alarm == 0)
	      {
	        flag_Alarm = 2;
	        convert_Mode(1);
	        getTime();
	        step_Alarm = 25;
	        set_Alarm(Hour, Minute, Second, Date, step_Alarm);
	        HAL_UART_Abort_IT(&huart1);
	        HAL_GPIO_WritePin(RTS_SIM_GPIO_Port, RTS_SIM_Pin, SET);
	      }
	      if (checkCall == 1 || checkMes == 1 || checkSer == 1)
	        //Kiem tra loi
	      {
	        HAL_GPIO_WritePin(RTS_SIM_GPIO_Port, RTS_SIM_Pin, RESET);
	        HAL_Delay(30);
	        resetSoftwareSARAU201();
	        initSIM();
	        if (checkSer == 1) flag_Alarm = 1;
	        if (checkCall == 1 || checkMes == 1) Detach = 1;
	        checkCall = checkMes = checkSer = 0;
	      }
	      else if (NewSms == 1)
	        //Khi co tin nhan moi
	      {
	        HAL_GPIO_WritePin(RTS_SIM_GPIO_Port, RTS_SIM_Pin, RESET);
	        HAL_Delay(20);
	        checkMessage(teleNumber);
	        Ring = 0;
	        NewSms = 0;
	        HAL_GPIO_WritePin(RTS_SIM_GPIO_Port, RTS_SIM_Pin, SET);
	      }
	      else if (Detach == 1)
	        //Kiểm tra nếu tháo thiết bị mà không cho phép
	      {
	        HAL_GPIO_WritePin(RTS_SIM_GPIO_Port, RTS_SIM_Pin, RESET);
	        HAL_Delay(20);
	        send_getDataServer();
	        if (status == 0)
	        {
	          if (!sendMsg(2, teleNumber, Latitude_ARR, Longtitude_ARR, Battery_ARR)) checkMes = 1;
	          HAL_Delay(5000);
	          if (!makeCall(teleNumber)) checkCall = 1;
	          enableSpeaker();
	          flag_Alarm = 2;
	        }
	        Detach = 0;
	        HAL_GPIO_WritePin(RTS_SIM_GPIO_Port, RTS_SIM_Pin, SET);
	      }
	      else if (AllowDetach == 1)//Cho pheo thao thiet bi
	      {
	        HAL_GPIO_WritePin(RTS_SIM_GPIO_Port, RTS_SIM_Pin, RESET);
	        HAL_Delay(20);
	        send_getDataServer();
	        flag_Alarm = 2;
	        if (status == 1)
	        {
	          //Kiem tra thiết bị đã được tháo => Kiem tra chan tren VDK
	          enableDetach();
	          Detach = 0;
	        }
	        AllowDetach = 0;
	        HAL_GPIO_WritePin(RTS_SIM_GPIO_Port, RTS_SIM_Pin, SET);
	      }
	      else if (flag_Alarm == 1)
	        //Khi den tg danh thuc
	      {
	        //------------------------GPS--------F--------------------
	        HAL_Delay(3000);
	        while (end_Buffer != 1)
	        {
	          HAL_UART_Receive_IT(&huart1, (uint8_t*)Rx_Data, 1);
	        }
	        valid_GPS = send_Coordinate_to_Serve(lat_Array, lng_Array);
	  //      if (valid_GPS)
	  //      {
	          HAL_GPIO_WritePin(RTS_SIM_GPIO_Port, RTS_SIM_Pin, RESET);
	          HAL_Delay(20);
	          send_getDataServer();
	  //      }
//	        if (check_Valid_Header(Rx_Buffer, 0) != 0)
//	        {
//	          convert_Mode(0);
//	          flag_Alarm = 2;
//	        }
	        flag_Alarm = 2;
	        convert_Mode(0);
	        getTime();
	        step_Alarm = 35;
	        set_Alarm(Hour, Minute, Second, Date, step_Alarm);
	        if (SafeArea == 1)
	          //Khi ra khoi vung an toàn
	        {
	          //Doi tg cap nhap len 2ph
	          if (!sendMsg(1, teleNumber, Latitude_ARR, Longtitude_ARR, Battery_ARR)) checkMes = 1;
	          HAL_Delay(5000);
//	          if (!makeCall(teleNumber)) checkCall = 1;
//	          enableSpeaker();
	          SafeArea = 0;
	        }
	        HAL_GPIO_WritePin(RTS_SIM_GPIO_Port, RTS_SIM_Pin, SET);
	      }
	      else if (flag_Alarm == 2) //&& NewSms == 0 && Detach == 0 && AllowDetach == 0 && checkCall == 0 && checkMes == 0 && checkSer == 0
	      {
	        HAL_SuspendTick();
	        HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
	      }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_LPUART1|RCC_PERIPHCLK_ADC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 16;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage 
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config 
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.NbrOfDiscConversion = 1;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_VREFINT;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief LPUART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPUART1_UART_Init(void)
{

  /* USER CODE BEGIN LPUART1_Init 0 */

  /* USER CODE END LPUART1_Init 0 */

  /* USER CODE BEGIN LPUART1_Init 1 */

  /* USER CODE END LPUART1_Init 1 */
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 4800;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPUART1_Init 2 */

  /* USER CODE END LPUART1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 4800;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only 
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
    
  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date 
  */
  sTime.Hours = 0x5;
  sTime.Minutes = 0x10;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable the Alarm A 
  */
  sAlarm.AlarmTime.Hours = 0x5;
  sAlarm.AlarmTime.Minutes = 0x10;
  sAlarm.AlarmTime.Seconds = 0x30;
  sAlarm.AlarmTime.SubSeconds = 0x0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY|RTC_ALARMMASK_HOURS;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 0x1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(PWR_SIM_GPIO_Port, PWR_SIM_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, RTS_SIM_Pin|SPEAKER_PIN_Pin|PIN_PWR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PWR_SIM_Pin RTS_SIM_Pin PIN_PWR_Pin */
  GPIO_InitStruct.Pin = PWR_SIM_Pin|RTS_SIM_Pin|PIN_PWR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : RI_SIM_Pin */
  GPIO_InitStruct.Pin = RI_SIM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(RI_SIM_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SPEAKER_PIN_Pin */
  GPIO_InitStruct.Pin = SPEAKER_PIN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SPEAKER_PIN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ALLOW_DETACH_PIN_Pin DETACH_PIN_Pin */
  GPIO_InitStruct.Pin = ALLOW_DETACH_PIN_Pin|DETACH_PIN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : STAT_SIM_Pin */
  GPIO_InitStruct.Pin = STAT_SIM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(STAT_SIM_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PIN_WAKEUP_Pin */
  GPIO_InitStruct.Pin = PIN_WAKEUP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(PIN_WAKEUP_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
