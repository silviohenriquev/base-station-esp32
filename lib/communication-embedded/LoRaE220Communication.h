#ifndef LoRa_E220_COMMUNICATION_H
#define LoRa_E220_COMMUNICATION_H

#include <Arduino.h>
#include <commConfig.h>
#include <commTypes.h>
#include <LoRa_E220.h>
#include <RTClib.h>
#include <SPI.h>
#include <Wire.h>

#if (!defined(__STM32F1__) && !defined(ESP32) && !defined(ARDUINO_ARCH_SAMD))
	#define ACTIVATE_SOFTWARE_SERIAL
#endif
#if defined(ESP32)
	#define HARDWARE_SERIAL_SELECTABLE_PIN
#endif

class LoRaE220Communication
{
public:
    // Class constructor
    #ifdef ACTIVATE_SOFTWARE_SERIAL
        LoRaE220Communication(byte txE220pin, byte rxE220pin, byte auxPin, byte m0Pin, byte m1Pin, DeviceFunction function);
    #endif
    #ifdef HARDWARE_SERIAL_SELECTABLE_PIN
		//LoRaE220Communication(byte txE220pin, byte rxE220pin, HardwareSerial* serial, byte auxPin, byte m0Pin, byte m1Pin, DeviceFunction function);
		LoRaE220Communication(HardwareSerial *serial,byte auxPin, byte m0Pin, byte m1Pin, DeviceFunction function);
    #endif
    int setup();
    void printParameters();

    bool sendSensorsDataPacket(SensorData data);

    SensorData getSensorsData();
    void printSensorsData(SensorData data);
    msgType updatePacket();
    void clearSensorData();
    // Class destructor
    // ~LoRaE220Communication();

private:
    LoRa_E220 _lora;    
    RTC_DS3231 _rtc;

private:
    void _configure();

    const float _calc = 100.0;

    packetSensorsData _mSensorData;
    packetGeneric _rx;

    DateTime _time;

    ResponseStructContainer c;
    ResponseStructContainer _rsc;
    msgType _typeMsg;
    Configuration configuration;
    DeviceConfig _config;
    ResponseStatus _rs;
    
    SensorData _sensorData;
};

#endif // LoRa_COMM