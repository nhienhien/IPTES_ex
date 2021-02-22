/*
 * ORG1411_GPS.h
 *
 *  Created on: 20 thg 1, 2021
 *      Author: Admin
 */

#ifndef ORG1411_GPS_H_
#define ORG1411_GPS_H_



#endif /* ORG1411_GPS_H_ */
//-----------------------ON_OF_Pulse---------------------------//
void ON_OFF_Pulse(void);
void convert_Mode(int mode);
//-------------------------Checksum----------------------------//
void str2hex(char* Rx_Buffer, char* Rx_Buffer_hex);
int NMEA_Checksum(char *Rx_Buffer);
//-----------------------Take_Position-------------------------//
int check_Valid_Header(char *Rx_Buffer, int case_return);
int take_Coordinate(char *Rx_Buffer);
int convert_Coordinate(char *lat_Array, char *lng_Array);
void ORG1411_GPS_Take_Coordinate(UART_HandleTypeDef *huart, USART_TypeDef *HUART);
