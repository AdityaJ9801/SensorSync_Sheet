#include <WiFi.h>

#include <HTTPClient.h>

#include "DHT.h"



#define DHTPIN    5



//our sensor is DHT11 type

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);



const char * ssid = "abc"; // write your wifi name

const char * password = "123"; //write wifi password



String server = "http://"...".com"; // put your server url

String eventName = "Sensor_data"; // name you have put for your project

String IFTTT_Key = "keys";// write your key 

String IFTTTUrl="https://maker.ifttt.com/trigger/Sensor_data/with/key/<keys>";



int value1;

int value2;

String value3;



void setup() {

  Serial.begin(115200);

  dht.begin();

  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);



  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");

  }

  Serial.println("Viola, Connected !!!");

}







void sendDataToSheet(void)

{

  String url = server + "/trigger/" + eventName + "/with/key/" + IFTTT_Key + "?value1=" + String((int)value1) + "&value2="+String((int)value2) +"&value3=" + String(value3);  

  Serial.println(url);

  //Start to send data to IFTTT

  HTTPClient http;

  Serial.print("[HTTP] begin...\n");

  http.begin(url); //HTTP



  Serial.print("[HTTP] GET...\n");

  // start connection and send HTTP header

  int httpCode = http.GET();

  // httpCode will be negative on error

  if(httpCode > 0) {

    // HTTP header has been send and Server response header has been handled

    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server

    if(httpCode == HTTP_CODE_OK) {

      String payload = http.getString();

      Serial.println(payload);

    }

  } else {

    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());

  }

  http.end();



}

String get_temp_humid_index(int humidity,int temperature)
{

  int temp_threshold = 30;
  int humid_threshold = 60;

  if (temperature > temp_threshold && humidity > humid_threshold){
        return("hot_and_humid");
  }    
  else if (temperature > temp_threshold && humidity <= humid_threshold){
        return("hot_and_dry");
  }
  else if (temperature <= temp_threshold && humidity > humid_threshold){
        return("cool_and_humid");
  }
  else{
    return ("cool_and_dry");
  }

}

void loop() {

  value1 = dht.readHumidity();

  value2 = dht.readTemperature();

  value3 = get_temp_humid_index(value1,value2); 



  Serial.print("Values are ");

  Serial.print(value1);

  Serial.print(' ');

  Serial.print(value2);

  Serial.print(' ');

  Serial.println(value3);

  Serial.print(' '); 



  sendDataToSheet();

  delay(900000);


}