#include <Arduino.h>
#include "LoRa_E220.h"
#include <LoRaE220Communication.h>
#include <FirebaseESP32.h>
#include "api.h"

FirebaseData firebaseData;
FirebaseJson json;

LoRaE220Communication lora(&Serial2, 15, 21, 19, DeviceFunction::base_station);

void wifiConnect();
void firebaseInit();
void postData(SensorData);

void setup(){
    Serial.begin(9600);
    delay(1000);

    wifiConnect();
    firebaseInit();

    lora.setup();
    lora.printParameters();
}

void loop(){
    if(lora.updatePacket() == msgType::SENSORS_DATA){
        //lora.printSensorsData(lora.getSensorsData());
        postData(lora.getSensorsData());
    }
    
}

void wifiConnect(){
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with Ip: ");
    Serial.print(WiFi.localIP());
    Serial.println();
}

void firebaseInit(){
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);

    Firebase.setReadTimeout(firebaseData, 1000 * 60);
    Firebase.setwriteSizeLimit(firebaseData, "tiny");
    Serial.println("--------------------------------");
    Serial.println("Connected...");
}

void postData(SensorData data){
    String path = "/database/";
    path += String(data.id);
    String send = "{\"date\": ";
    send += data.date;
    send += ", \"irradiance\": ";
    send += data.irradiance;
    send += ", \"tempExternal\": ";
    send += data.tempExternal;
    send += ", \"tempInternal\": ";
    send += data.tempInternal;
    send += ", \"tempCell\": ";
    send += data.tempCell;
    send += "}";
    Serial.println(send);
    json.setJsonData(send);
    Firebase.pushJSON(firebaseData, path, json);
}