#include "pms5003.h"  //include library for dust sensor
struct PMS5003_Data PMS5003; //initialize PMS5003 struct to store the values of the sensor
extern UART_HandleTypeDef huart1; // uart communication
uint8_t count = 0;
/**
  * @brief  Start interrupting to receive 1 order
  * @param  handle : UART handle
  * @retval No
  */
void PMS5003_ReceiveStart_IT(UART_HandleTypeDef* handle){
	HAL_UART_Receive_IT(handle, PMS5003.uartBuf, 32);
}
/**
  * @brief  Stop receiving PMS5003 data
  * @param  handle : UART handle
  * @retval No
  */
void PMS5003_ReceiveData_IT(UART_HandleTypeDef* handle){
	//First character received: 0x42 
	if (PMS5003.uartBuf[0] == PMS_START_BYTE_1 && PMS5003.uartBuf[1] == PMS_START_BYTE_2){
			PMS5003.frame = ((PMS5003.uartBuf[2] << 8) + PMS5003.uartBuf[3]); //get frame
			PMS5003.pm1p0 = ((PMS5003.uartBuf[4] << 8) + PMS5003.uartBuf[5]); //get pms1.0 data
			PMS5003.pm2p5 = ((PMS5003.uartBuf[6] << 8) + PMS5003.uartBuf[7]); //get pms2.5 data
			PMS5003.pm10 = ((PMS5003.uartBuf[8] << 8) + PMS5003.uartBuf[9]); //get pms10 data
			PMS5003.pm1p0_ATM = ((PMS5003.uartBuf[10] << 8) + PMS5003.uartBuf[11]); //get pms1.0 data in atmosphere
			PMS5003.pm2p5_ATM = ((PMS5003.uartBuf[12] << 8) + PMS5003.uartBuf[13]); //get pms2.5 data in atmosphere
			PMS5003.pm10_ATM = ((PMS5003.uartBuf[14] << 8) + PMS5003.uartBuf[15]); //get pms10 data in atmosphere
			PMS5003.checksum = ((PMS5003.uartBuf[30] << 8) + PMS5003.uartBuf[31]); //get checksum
			HAL_UART_Receive_IT(handle, PMS5003.uartBuf,32); //waiting for incoming new data
			PMS5003.state = PMS5003_RECEIVED; //state of process = RECEIVED
		}
		else{ 
			HAL_UART_Receive_IT(handle, PMS5003.uartBuf, 32); //waiting for incoming new data
			PMS5003.state = PMS5003_CRC_ERROR; //state of process = ERROR
		}
}
/**
  * @brief  Receiving PMS5003 data without interrupt //same as above
  * @param  handle : UART handle
  * @retval No
  */
int PMS5003_ReceiveData(UART_HandleTypeDef* handle){
		HAL_UART_Receive(handle, PMS5003.uartBuf, 1, 1000);
		if(PMS5003.uartBuf[0] == PMS_START_BYTE_1){
			PMS5003.countDataToReceive = 31;
			HAL_UART_Receive(handle, PMS5003.uartBuf, PMS5003.countDataToReceive, 1000);
			PMS5003.state = PMS5003_RECEIVING;
	 }
		if(PMS5003.uartBuf[0] == PMS_START_BYTE_2 && PMS5003.state == PMS5003_RECEIVING){
			PMS5003.frame = ((PMS5003.uartBuf[1] << 8) + PMS5003.uartBuf[2]); 
			PMS5003.pm1p0 = ((PMS5003.uartBuf[3] << 8) + PMS5003.uartBuf[4]);
			PMS5003.pm2p5 = ((PMS5003.uartBuf[5] << 8) + PMS5003.uartBuf[6]);
			PMS5003.pm10 = ((PMS5003.uartBuf[7] << 8) + PMS5003.uartBuf[8]);
			PMS5003.pm1p0_ATM = ((PMS5003.uartBuf[9] << 8) + PMS5003.uartBuf[10]);
			PMS5003.pm2p5_ATM = ((PMS5003.uartBuf[11] << 8) + PMS5003.uartBuf[12]);
			PMS5003.pm10_ATM = ((PMS5003.uartBuf[13] << 8) + PMS5003.uartBuf[14]);
			PMS5003.checksum = ((PMS5003.uartBuf[29] << 8) + PMS5003.uartBuf[30]);
			for(int i = 0; i < 31; i++){
				PMS5003.uartBuf[i] = 0;
			}
		PMS5003.state = PMS5003_RECEIVED;
		}
		else{ 
			PMS5003.state = PMS5003_CRC_ERROR;
		}
		return PMS5003.state;
}

void PMS5003_sleep(void) //send the script code to PMS5003 to make it in sleep mode
{
	uint8_t command[] = { 0x42, 0x4D, 0xE4, 0x00, 0x00, 0x01, 0x73 };
	HAL_UART_Transmit(&huart1,command,7,1000);
}

void PMS5003_wakeup(void) //send the script code to PMS5003 to make it in run mode
{
	uint8_t command[] = { 0x42, 0x4D, 0xE4, 0x00, 0x00, 0x01, 0x73 };
	HAL_UART_Transmit(&huart1,command,7,1000);
}
