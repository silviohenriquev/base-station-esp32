#ifndef COMM_TYPES_H
#define COMM_TYPES_H

/************* AUXILIAR TYPES ************/
#include <stdint.h>
#include "commConfig.h"

enum class DeviceFunction
{
    sensor,
    base_station
};

typedef struct
{
    DeviceFunction function;
} DeviceConfig;

typedef struct
{
	int typeMsg = 0;
	int id = 1;
	float tempCell = 1;
	float tempExternal = 0;
	float tempInternal = 0;
	int irradiance = 0;
	uint32_t date = 0;
} SensorData;


#endif // COMM_TYPES_H