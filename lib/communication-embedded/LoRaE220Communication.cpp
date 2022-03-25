
#include "LoRaE220Communication.h"

#ifdef ACTIVATE_SOFTWARE_SERIAL
LoRaE220Communication::LoRaE220Communication(byte txE220pin, byte rxE220pin, byte auxPin, byte m0Pin, byte m1Pin, DeviceFunction function)
    :_lora(txE220pin, rxE220pin, auxPin, m0Pin, m1Pin)
{
	this->_config.function = function;
}
#endif
#ifdef HARDWARE_SERIAL_SELECTABLE_PIN
LoRaE220Communication::LoRaE220Communication(HardwareSerial *serial, byte auxPin, byte m0Pin, byte m1Pin, DeviceFunction function)
    :_lora(serial, auxPin, m0Pin, m1Pin, UART_BPS_RATE_9600)
{
	this->_config.function = function;
}
#endif

int LoRaE220Communication::setup(){
    this->_configure();
	return 0;
}

void LoRaE220Communication::_configure()
{
    this->_lora.begin(); //Start the LoRaE220 module

	if(_config.function == DeviceFunction::sensor)
	{
		this->c = this->_lora.getConfiguration();
    	this-> configuration = *(Configuration*) c.data;

    	this->configuration.ADDL = SENSOR_ADDL;
		this->configuration.ADDH = SENSOR_ADDH;

    	this->configuration.CHAN = CHANNEL;

		this->configuration.SPED.uartBaudRate = UART_BPS_9600; // Serial baud rate
		this->configuration.SPED.airDataRate = AIR_DATA_RATE_010_24; // Air baud rate
		this->configuration.SPED.uartParity = MODE_00_8N1; // Parity bit

		this->configuration.OPTION.subPacketSetting = SPS_200_00; // Packet size
		this->configuration.OPTION.RSSIAmbientNoise = RSSI_AMBIENT_NOISE_DISABLED; // Need to send special command
		this->configuration.OPTION.transmissionPower = POWER_22; // Device power

    	this->configuration.TRANSMISSION_MODE.enableRSSI = RSSI_DISABLED; // Enable RSSI info
    	this->configuration.TRANSMISSION_MODE.fixedTransmission = FT_FIXED_TRANSMISSION; // Enable repeater mode
    	this->configuration.TRANSMISSION_MODE.enableLBT = LBT_DISABLED; // Check interference
    	this->configuration.TRANSMISSION_MODE.WORPeriod = WOR_2000_011; // WOR timing

    	this->_lora.setConfiguration(this->configuration, WRITE_CFG_PWR_DWN_SAVE);
	}
	if(_config.function == DeviceFunction::base_station)
	{
		this->c = this->_lora.getConfiguration();
    	this-> configuration = *(Configuration*) c.data;

    	this->configuration.ADDL = BASE_ADDL;
    	this->configuration.ADDH = BASE_ADDH;

    	this->configuration.CHAN = CHANNEL;
    	
		this->configuration.SPED.uartBaudRate = UART_BPS_9600; // Serial baud rate
		this->configuration.SPED.airDataRate = AIR_DATA_RATE_010_24; // Air baud rate
		this->configuration.SPED.uartParity = MODE_00_8N1; // Parity bit

		this->configuration.OPTION.subPacketSetting = SPS_200_00; // Packet size
		this->configuration.OPTION.RSSIAmbientNoise = RSSI_AMBIENT_NOISE_DISABLED; // Need to send special command
		this->configuration.OPTION.transmissionPower = POWER_22; // Device power

    	this->configuration.TRANSMISSION_MODE.enableRSSI = RSSI_DISABLED; // Enable RSSI info
    	this->configuration.TRANSMISSION_MODE.fixedTransmission = FT_FIXED_TRANSMISSION; // Enable repeater mode
    	this->configuration.TRANSMISSION_MODE.enableLBT = LBT_DISABLED; // Check interference
    	this->configuration.TRANSMISSION_MODE.WORPeriod = WOR_2000_011; // WOR timing

    	this->_lora.setConfiguration(this->configuration, WRITE_CFG_PWR_DWN_SAVE);
	}
    
}

void LoRaE220Communication::printParameters(){
    Serial.println("----------------------------------------");

	Serial.print(F("HEAD : "));  Serial.print(this->configuration.COMMAND, HEX);Serial.print(" ");Serial.print(this->configuration.STARTING_ADDRESS, HEX);Serial.print(" ");Serial.println(this->configuration.LENGHT, HEX);
	Serial.println(F(" "));
	Serial.print(F("AddH : "));  Serial.println(this->configuration.ADDH, HEX);
	Serial.print(F("AddL : "));  Serial.println(this->configuration.ADDL, HEX);
	Serial.println(F(" "));
	Serial.print(F("Chan : "));  Serial.print(this->configuration.CHAN, DEC); Serial.print(" -> "); Serial.println(this->configuration.getChannelDescription());
	Serial.println(F(" "));
	Serial.print(F("SpeedParityBit     : "));  Serial.print(this->configuration.SPED.uartParity, BIN);Serial.print(" -> "); Serial.println(this->configuration.SPED.getUARTParityDescription());
	Serial.print(F("SpeedUARTDatte     : "));  Serial.print(this->configuration.SPED.uartBaudRate, BIN);Serial.print(" -> "); Serial.println(this->configuration.SPED.getUARTBaudRateDescription());
	Serial.print(F("SpeedAirDataRate   : "));  Serial.print(this->configuration.SPED.airDataRate, BIN);Serial.print(" -> "); Serial.println(this->configuration.SPED.getAirDataRateDescription());
	Serial.println(F(" "));
	Serial.print(F("OptionSubPacketSett: "));  Serial.print(this->configuration.OPTION.subPacketSetting, BIN);Serial.print(" -> "); Serial.println(this->configuration.OPTION.getSubPacketSetting());
	Serial.print(F("OptionTranPower    : "));  Serial.print(this->configuration.OPTION.transmissionPower, BIN);Serial.print(" -> "); Serial.println(this->configuration.OPTION.getTransmissionPowerDescription());
	Serial.print(F("OptionRSSIAmbientNo: "));  Serial.print(this->configuration.OPTION.RSSIAmbientNoise, BIN);Serial.print(" -> "); Serial.println(this->configuration.OPTION.getRSSIAmbientNoiseEnable());
	Serial.println(F(" "));
	Serial.print(F("TransModeWORPeriod : "));  Serial.print(this->configuration.TRANSMISSION_MODE.WORPeriod, BIN);Serial.print(" -> "); Serial.println(this->configuration.TRANSMISSION_MODE.getWORPeriodByParamsDescription());
	Serial.print(F("TransModeEnableLBT : "));  Serial.print(this->configuration.TRANSMISSION_MODE.enableLBT, BIN);Serial.print(" -> "); Serial.println(this->configuration.TRANSMISSION_MODE.getLBTEnableByteDescription());
	Serial.print(F("TransModeEnableRSSI: "));  Serial.print(this->configuration.TRANSMISSION_MODE.enableRSSI, BIN);Serial.print(" -> "); Serial.println(this->configuration.TRANSMISSION_MODE.getRSSIEnableByteDescription());
	Serial.print(F("TransModeFixedTrans: "));  Serial.print(this->configuration.TRANSMISSION_MODE.fixedTransmission, BIN);Serial.print(" -> "); Serial.println(this->configuration.TRANSMISSION_MODE.getFixedTransmissionDescription());


	Serial.println("----------------------------------------");
}

bool LoRaE220Communication::sendSensorsDataPacket(SensorData data){
	this->_mSensorData.decoded.typeMsg      = static_cast<uint16_t>(msgType::SENSORS_DATA  		   );
	this->_mSensorData.decoded.id           = static_cast<uint16_t>(data.id                		   );
	this->_mSensorData.decoded.irradiance   = static_cast<uint16_t>(data.irradiance        		   );
	this->_mSensorData.decoded.tempCell     = static_cast<uint16_t>(data.tempCell     *this->_calc );
	this->_mSensorData.decoded.tempExternal = static_cast<uint16_t>(data.tempExternal *this->_calc );
	this->_mSensorData.decoded.tempInternal = static_cast<uint16_t>(data.tempInternal *this->_calc );
	this->_mSensorData.decoded.date         = static_cast<uint32_t>(data.date                      );
	
	Serial.println("Sending Packet..");

	this->_rs=this->_lora.sendFixedMessage(BASE_ADDH, BASE_ADDL, CHANNEL, &this->_mSensorData.encoded, LORA_MAX_PAYLOAD);
	Serial.println(this->_rs.getResponseDescription());

	return true;
}

msgType LoRaE220Communication::updatePacket(){
	if(this->_lora.available()>1){
		this->_rsc = this->_lora.receiveMessage(LORA_MAX_PAYLOAD);
		this->_rx = *(packetGeneric*) this->_rsc.data;
		// Save the message type
		this->_typeMsg = static_cast<msgType>(this->_rx.decoded.typeMsg);
		Serial.println("Chegou Algo...");
		if(this->_typeMsg == msgType::SENSORS_DATA){
			this->clearSensorData();
			memcpy(this->_mSensorData.encoded, this->_rx.encoded, SENSORS_DATA_LENGTH);
			this->_sensorData.typeMsg      = static_cast<int>     (this->_mSensorData.decoded.typeMsg   				   	     );
			this->_sensorData.id           = static_cast<int>     (this->_mSensorData.decoded.id        				   	     );
			this->_sensorData.irradiance   = static_cast<int>     (this->_mSensorData.decoded.irradiance			   		     );
			this->_sensorData.tempCell     = static_cast<float>   ((float)(this->_mSensorData.decoded.tempCell)     /this->_calc );
			this->_sensorData.tempExternal = static_cast<float>   ((float)(this->_mSensorData.decoded.tempExternal) /this->_calc );
			this->_sensorData.tempInternal = static_cast<float>   ((float)(this->_mSensorData.decoded.tempInternal) /this->_calc );
			this->_sensorData.date         = static_cast<uint32_t>(this->_mSensorData.decoded.date                               );
		}

		return this->_typeMsg;
	}
}

void LoRaE220Communication::clearSensorData(){
	this->_sensorData.typeMsg = -1;
	this->_sensorData.id = -1;
	this->_sensorData.irradiance = -1;
	this->_sensorData.tempCell = -1;
	this->_sensorData.tempExternal = -1;
	this->_sensorData.tempInternal = -1;
	this->_sensorData.date = -1;
}

SensorData LoRaE220Communication::getSensorsData(){
	return this->_sensorData;
}

void LoRaE220Communication::printSensorsData(SensorData data){
	Serial.print("Id: ");
	Serial.println(data.id);
	Serial.print("Irradiancia: ");
	Serial.print(data.irradiance);
	Serial.println(" W/M2");
	Serial.print("Temperatura da celula: ");
	Serial.print(data.tempCell);
	Serial.println(" °C");
	Serial.print("Temperatura externa: ");
	Serial.print(data.tempExternal);
	Serial.println(" °C");
	Serial.print("Temperatura interna: ");
	Serial.print(data.tempInternal);
	Serial.println(" °C");
	Serial.print("Data: ");
	this->_time = data.date;
	Serial.print(this->_time.year(), DEC);
    Serial.print('/');
    Serial.print(this->_time.month(), DEC);
    Serial.print('/');
    Serial.print(this->_time.day(), DEC);
    Serial.print(" ");
    Serial.print(this->_time.hour(), DEC);
    Serial.print(':');
    Serial.print(this->_time.minute(), DEC);
    Serial.print(':');
    Serial.print(this->_time.second(), DEC);
    Serial.println();
}