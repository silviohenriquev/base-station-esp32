#include <Arduino.h>
#include "LoRa_E220.h"
#include <LoRaE220Communication.h>
#include <FirebaseESP32.h>
#include "api.h"
#include "time.h"

FirebaseData firebaseData;
FirebaseJson json;

LoRaE220Communication lora(&Serial2, 15, 21, 19, DeviceFunction::base_station);

void restart();
void wifiConnect();
void firebaseInit();
void postData(SensorData);
void postLastData(SensorData);

int day;
int month;

void setup(){
    Serial.begin(9600);
    delay(1000);

    wifiConnect();
    firebaseInit();

    lora.setup();
    lora.printParameters();
}

void loop(){

    restart();
    if(lora.updatePacket() == msgType::SENSORS_DATA){
        if(lora.getSensorsData().id==1){
            lora.printSensorsData(lora.getSensorsData());
            postData(lora.getSensorsData());
            lora.blink(100);
        }
        

    }    
    
}

void restart(){
    //Firebase.getInt(firebaseData,"restart/");
    //restart=firebaseData.intData();
    //Serial.print("Restart:  ");
    //Serial.println(restart);
    if (!Firebase.readStream(firebaseData)){
        Serial.println(firebaseData.errorReason());
    }

    if (firebaseData.streamTimeout()){
        Serial.println("Stream timeout, resume streaming...");
        Serial.println();
    }

    if(firebaseData.streamAvailable()){
        Serial.println("Stream disponivel");
        if(firebaseData.to<int>()==1){
            Firebase.setInt(firebaseData, "restart/", 0);
            ESP.restart();
        }
    }
}

void wifiConnect(){
    pinMode(2, OUTPUT);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        digitalWrite(2, HIGH);
        delay(300);
        digitalWrite(2, LOW);
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

    if (!Firebase.beginStream(firebaseData, "restart")){
        Serial.println(firebaseData.errorReason());
    }
}

void postData(SensorData data){
    
    time_t timestamp = data.date;
    struct tm* timeinfo = localtime(&timestamp);
    day = timeinfo->tm_mday;
    month = timeinfo->tm_mon + 1;
    String path = "/database-novo/"+String(data.id)+"/"+String(month)+"/"+String(day)+"/";
    String send = "{\"id\": ";
    send += data.id;
    send += ", \"mes\": ";
    send += month;
    send += ", \"dia\": ";
    send += day;
    send += ", \"date\": ";
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
    if(data.id == 1){
        Firebase.pushJSON(firebaseData, path, json);
        postLastData(data);
    } 
}

void postLastData(SensorData data){
    String path = "/database-novo/";
    path += "lastData";
    String send = "{\"id\": ";
    send += data.id;
    send += ", \"mes\": ";
    send += month;
    send += ", \"dia\": ";
    send += day;
    send += ", \"date\": ";
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
    //Serial.println(send);
    json.setJsonData(send);
    if(data.id == 1){
        Firebase.setJSON(firebaseData, path, json);
    } 
}
