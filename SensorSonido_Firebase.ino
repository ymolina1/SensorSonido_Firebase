/*
  Complete details at https://RandomNerdTutorials.com/esp32-useful-wi-fi-functions-arduino/
*/

#include <WiFi.h>
#include "FirebaseESP32.h"

#define FIREBASE_HOST "https://proyectofisica-8a564-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "taDNxQVE4uSNmorgUZ3qCxIKggNnxe5TeDYoMdPB"




// Replace with your network credentials (STATION)
const char* ssid = "HAM";
const char* password = "*********";

FirebaseData firebaseData;
String ruta = "/PFisica";

//---------Time
unsigned long time1 = 0;
unsigned long time2 = 0;
unsigned long ElapsedTime = 0;


//--------------------
const int out = 5;
const int SAMPLE_TIME = 10;
unsigned long millisCurrent;
unsigned long millisLast = 0;
unsigned long millisElapsed = 0;
int sampleBufferValue = 0;
//--------------------

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.println("FISICA");
  Serial.begin(9600);
  initWiFi();
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());

  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  //Firebase.reconnectWiFi(true);

  Firebase.pushInt(firebaseData, ruta + "/Nivel/",22);

  //Firebase.setInt(firebaseData, ruta + "/logico",283);

  //String JSONdata = "{\"logico\":" + String(123)+" "/"}";
  //Firebase.pushJSON(firebaseData, ruta + "/Push/JSON", JSONdata);

  time1 = millis();

  
  
}

void loop() {
  // put your main code here, to run repeatedly:

  

  Firebase.readStream(firebaseData);
  if(firebaseData.streamAvailable())
  {
    Serial.println(firebaseData.stringData());
    delay(500);
  }

  //----------------------------------------- Dectectar sonido------------------------------
  millisCurrent = millis();
  millisElapsed = millisCurrent - millisLast;

  if(digitalRead(out) == HIGH)
  {
    sampleBufferValue++;
    // si milli elapsaed es igual a 30 seg guardar info
    
  }

  if(millisElapsed > SAMPLE_TIME)
  {
    Serial.println(sampleBufferValue);

    //----
    //-----Time
    time2 = millis();
    if(time2 > (time1+500))
    {
      if(sampleBufferValue > 0)
      {
        Firebase.pushInt(firebaseData, ruta + "/Nivel/",sampleBufferValue);
      }
      time1 = millis();
      
      
    }
    //----


    
    sampleBufferValue = 0;
    millisLast = millisCurrent;
  }
  
}
