#ifndef _PMS5003_H_
#define _PMS5003_H_

#include "stm32F7xx_hal.h"
/**
 *  Dinh nghia gia tri bit bat dau cua PMS5003
 */
#define PMS_START_BYTE_1 0x42
#define PMS_START_BYTE_2 0x4d

/**
 *  Cac lenh dieu khien PMS5003 
 */
static  uint8_t wakeup[7] = {0x42, 0x4D, 0xE4, 0x00, 0x01, 0x01, 0x74};
static  uint8_t active[7] = {0x42, 0x4d, 0xe1, 0x00, 0x01, 0x01, 0x71};
static  uint8_t sleep[7] =  {0x42, 0x4d, 0xe4, 0x00, 0x00, 0x01, 0x73};
static uint8_t passive[] = { 0x42, 0x4D, 0xE1, 0x00, 0x00, 0x01, 0x70 };
static uint8_t rpassive[] = { 0x42, 0x4D, 0xE2, 0x00, 0x00, 0x01, 0x71 };

/**
 * Mo ta trang thai hien tai cua Sensor
 */
typedef enum{
	PMS5003_RECEIVED,		/*< Trang thai da nhan duoc du lieu nhung chua xu ly */
	PMS5003_RECEIVING,	/*< Trang thai du lieu cam bien da duoc nhan va xu ly xong*/
	PMS5003_OFF,
	PMS5003_ON,
	PMS5003_CRC_ERROR, /*< Trang thai nhan du lieu bi loi*/
	PMS5003_ERROR,
} PMS5003_STATE;

/**
 * Struct chua tat ca cac bien can de giao tiep voi cam bien
 */
struct PMS5003_Data{
	volatile uint16_t 			frame;
	volatile uint16_t 			checksum;
	volatile uint16_t 			pm1p0;
	volatile uint16_t 			pm2p5;
	volatile uint16_t 			pm10;
	volatile uint16_t 			pm1p0_ATM;
	volatile uint16_t			pm2p5_ATM;
	volatile uint16_t 			pm10_ATM;
	uint8_t 			Buf;
	uint8_t 			uartBuf[32];
	uint8_t				countDataToReceive;
	PMS5003_STATE 		state;
};

void PMS5003_ReceiveStart_IT(UART_HandleTypeDef* handle);
void PMS5003_TransferData(UART_HandleTypeDef* handle);
void PMS5003_ReceiveData_IT(UART_HandleTypeDef* handle);
int PMS5003_ReceiveData(UART_HandleTypeDef* handle);
void PMS5003_sleep(void);
void PMS5003_wakeup(void);

#endif /* _PMS5003_H_ */
