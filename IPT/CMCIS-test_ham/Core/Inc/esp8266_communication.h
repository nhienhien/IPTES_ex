#ifndef INCLUDE_ESP8266_COMMUNICATION_H_
#define INCLUDE_ESP8266_COMMUNICATION_H_

#include <stdint.h>
#include <stddef.h>

/**
 * Uses tag-length-value format
 *
 *    1       2      <length>   Bytes
 * +--------------------------+
 * | Tag  | Length | Value    |
 * +--------------------------+
 */

/**
 *
 */
#define ESP8266_COM_TAG_TEMP_SENSOR 0x01

/**
 * Sends a temperature and humidity value to the ESP8266
 */
//void esp8266_send_value(uint8_t temp, uint8_t hum);
void esp8266_send_value(uint8_t temp, uint8_t hum, uint16_t lux, uint16_t pm1, uint16_t pm25, uint16_t pm10);
#endif /* INCLUDE_ESP8266_COMMUNICATION_H_ */
