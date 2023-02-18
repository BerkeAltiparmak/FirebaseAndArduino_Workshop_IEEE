/*
  Rui Santos
  Complete project details at our blog.
    - ESP32: https://RandomNerdTutorials.com/esp32-firebase-realtime-database/
    - ESP8266: https://RandomNerdTutorials.com/esp8266-nodemcu-firebase-realtime-database/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  Based in the RTDB Basic Example by Firebase-ESP-Client library by mobizt
  https://github.com/mobizt/Firebase-ESP-Client/blob/main/examples/RTDB/Basic/Basic.ino

  Berke Altiparmak
  Modified the code here to make it appropriate for our IEEE Workshop
*/

#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "Berke"
#define WIFI_PASSWORD "makeuoft2023"

// Insert Firebase project API Key
#define API_KEY "AIzaSyBtvD5rXoJmCheNRv7BtSfw6i59ihz1D-I"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://esp-firebase-68fdb-default-rtdb.firebaseio.com/" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
unsigned long receiveDataPrevMillis = 10000;
int count = 0;
bool signupOK = false;

void setup(){
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop(){
  // Store data every 15 seconds starting from 0
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    // Write an Int number on the database path test/int
    if (Firebase.RTDB.setInt(&fbdo, "test/int", count)){
      Serial.println("Stored " + String(count) + " at path:" + fbdo.dataPath());
    }
    else {
      Serial.println("Failed to pass data, error message: " + fbdo.errorReason());
    }
    count++;
    
    // Write a Float number on the database path test/float
    float randomNumber = 0.01 + random(0,100);
    if (Firebase.RTDB.setFloat(&fbdo, "test/float", randomNumber)){
      Serial.println("Stored a random number at path:" + fbdo.dataPath());
    }
    else {
      Serial.println("FAILED");
      Serial.println("Failed to pass data, error message: " + fbdo.errorReason());
    }

    delay(1000);
    if (Firebase.RTDB.getFloat(&fbdo, "/test/float")) {
      if (fbdo.dataType() == "float") {
        float floatValue = fbdo.floatData();
        Serial.println("By reading from the database, found out the random number stored was: " + String(floatValue));
      }
    }
    else {
       Serial.println("Failed to pass data, error message: " + fbdo.errorReason());
    }
  }
}