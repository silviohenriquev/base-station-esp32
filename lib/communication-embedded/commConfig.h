#ifndef COMM_CONFIG_H
#define COMM_CONFIG_H

// v3.1

#include <stdint.h>


// NRF Config Defitions
#define CHANNEL 42

#define SENSOR_ADDL 0x51
#define SENSOR_ADDH 0x47

#define BASE_ADDL 0x47
#define BASE_ADDH 0x51


#define LORA_MAX_PAYLOAD 32
#define SENSORS_DATA_LENGTH 16

#pragma pack (push, 1)

enum class msgType
{
    NONE = -1,
    SENSORS_DATA
};



typedef struct
{
  	uint8_t typeMsg : 4;
  	uint8_t id : 4;
  	int64_t rest_a : 64;
  	int64_t rest_b : 64;
  	int64_t rest_c : 64;
  	int64_t rest_d : 56;
} packetTypeGeneric;

typedef union {
  	unsigned char encoded[LORA_MAX_PAYLOAD];
  	packetTypeGeneric decoded;
} packetGeneric;


/**
 * Structure for sending sensors data,
 * This type sends:
 *  - Message type
 *  - Sensor Id
 *  - Irradiance
 *  - Temperature
 *  - Date
 */
typedef struct
{
  	uint16_t typeMsg : 16;
  	uint16_t id : 16;
  	uint16_t irradiance : 16;
  	uint16_t tempCell : 16;
  	uint16_t tempExternal : 16;
  	uint16_t tempInternal : 16;
  	uint32_t date : 32;
} packetTypeSensorsData;

typedef union packetSensorsData {
  	unsigned char encoded[SENSORS_DATA_LENGTH];
  	packetTypeSensorsData decoded;
} packetSensorsData;
//restoring the standard alignment
#pragma pack(pop)

#endif // COMM_CONFIG_H