/*
 * ORG1411_GPS.c
 *
 *  Created on: 20 thg 1, 2021
 *      Author: Admin
 */

#include "main.h"
#include "ORG1411_GPS.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_hal.h"

#define PIN_PWR_GPIO_Port GPIOA
#define PIN_PWR_Pin GPIO_PIN_8
#define PIN_WAKEUP_GPIO_Port GPIOC
#define PIN_WAKEUP_Pin GPIO_PIN_8
#define MAX_ARRAY 100

char Rx_Buffer[MAX_ARRAY] = {0};
char Rx_Data[2] = {0};
char lat_Array[10] = {0};
char lng_Array[10] = {0};
char Latitude_ARR[12] 	= {0};
char Longtitude_ARR[12] = {0};
float lat_Location;
float lng_Location;
int end_Buffer = 0;
uint8_t start_Receive = 0;
uint16_t Rx_index = 0;

//--------------------------DelayMicro-----------------------------//
__STATIC_INLINE void DelayMicro(__IO uint32_t micros)
{
micros *= (SystemCoreClock / 1000000) / 9;
/* Wait till done */
while (micros--) ;
}
#ifdef __GNUC__
 /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
 set to 'Yes') calls __io_putchar() */
 #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
 #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

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
void convert_Mode(int mode)
{
	if (mode)
	{
	   while (HAL_GPIO_ReadPin(PIN_WAKEUP_GPIO_Port, PIN_WAKEUP_Pin) != 1)
		   ON_OFF_Pulse();
	} else
	   while (HAL_GPIO_ReadPin(PIN_WAKEUP_GPIO_Port, PIN_WAKEUP_Pin) == 1)
		   ON_OFF_Pulse();
}

//-----------------------------Checksum----------------------------//
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

int NMEA_Checksum(char *Rx_Buffer)
{
	char Rx_Buffer_hex[150] = {0};
	str2hex(Rx_Buffer, Rx_Buffer_hex);
	int sum = 0, i = 1, Check_num = 0;
	char couple[3] = {0}, Checksum_line[3] = {0};
	while (Rx_Buffer[i] != '*')
	{
		strncpy(couple, Rx_Buffer_hex + i*2, 2);
		sum ^= strtol(couple,NULL,16);
		i+=1;
	}
	strncpy(Checksum_line,Rx_Buffer + i + 1, 2);
	Check_num = strtol(Checksum_line,NULL,16);

	if (sum == Check_num)
	{
		return 1;
	} else return 0;
}
//-----------------------Take_Position-------------------------//
int check_Valid_Header(char *Rx_Buffer, int case_return)
{
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
			}
		}
	}
	return 0;
}

int take_Coordinate(UART_HandleTypeDef *huart, char *Rx_Buffer)
{
	int start_pos = check_Valid_Header(Rx_Buffer, 1);
	int valid_Header = check_Valid_Header(Rx_Buffer, 0);
	while (valid_Header == 0)
	{
		HAL_UART_Receive_IT(&huart, (int*)Rx_Data, 1);
		valid_Header = check_Valid_Header(Rx_Buffer, 0);
	}

	int lat_domain, lng_domain;
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

	int domain = 1; //mien trong cau truc NMEA
	for (uint16_t i = start_pos; i < MAX_ARRAY; i++)
	{
		if (Rx_Buffer[i] == ',')
		{
			domain++;
			if (domain == lat_domain)
			{
				for (int  j = i+1; j < i+9; j++)
					if (Rx_Buffer[j] == ',' )
						return 0;
				strncpy(lat_Array, Rx_Buffer + i + 1, 9);
				__NOP();
			} else
			if (domain == lng_domain)
			{
				for (int  j = i+1; j < i+10; j++)
					if (Rx_Buffer[j] == ',' )
						return 0;
				strncpy(lng_Array, Rx_Buffer + i + 1, 10);
				__NOP();
				return 1;
			}
		}
	}
	return 0;
}

int convert_Coordinate(UART_HandleTypeDef *huart, char *lat_Array, char *lng_Array)
{
	char location_str_1[5] = {0};
	char location_str_2[5] = {0};
	if (take_Coordinate(&huart, Rx_Buffer) == 1)
	{
		//-------------------------	Latitude ----------------------------//
		volatile float lat_1 = 0, lat_2 = 0;
		strncpy(location_str_1, lat_Array, 2);	lat_1 = strtof(location_str_1,NULL);
		strncpy(location_str_2, lat_Array + 2, 7);	lat_2 = strtof(location_str_2,NULL);
		lat_Location = lat_1 + lat_2/60;
		gcvt(lat_Location, 10, Latitude_ARR);
		//-------------------------- Longitude --------------------------//
		volatile float lng_1 = 0, lng_2 = 0;
		strncpy(location_str_1, lng_Array, 3);	lng_1 = strtof(location_str_1,NULL);
		strncpy(location_str_2, lng_Array + 3, 7);	lng_2 = strtof(location_str_2,NULL);
		lng_Location = lng_1 + lng_2/60;
		gcvt(lng_Location, 10, Longtitude_ARR);
		return 1;
	}
	return 0;
}

void ORG1411_GPS_Take_Coordinate(UART_HandleTypeDef *huart, USART_TypeDef *HUART)
{
	int valid_Coordinate_GPS;
	if (huart->Instance == huart)
	{
		if (Rx_Data[0] == '$')
		{
			start_Receive = 1;
			Rx_index = 0;
		} else
		if ((Rx_Data[0] == '\r')||(Rx_Data[0] == '\n'))
		{
			start_Receive = 0;
//			HAL_UART_Abort_IT(&huart);
		} else
			HAL_UART_Receive_IT(&huart, (int*)Rx_Data, 1);
		if ((start_Receive)&&(Rx_index < MAX_ARRAY))
		{
			Rx_Buffer[Rx_index++] = Rx_Data[0];
			HAL_UART_Receive_IT(&huart, (int*)Rx_Data, 1);
		}
		else
		{
			end_Buffer = 1;
		}
	}
	while (end_Buffer != 1)
	{
	  HAL_UART_Receive_IT(&huart, (int*)Rx_Data, 1);
	}
	valid_Coordinate_GPS = convert_Coordinate(&huart, lat_Array, lng_Array);
}
