#include "DHT.h"
#define DHTTYPE DHT11
uint8_t DHTPin = D8; 

DHT dht(DHTPin, DHTTYPE);  
float temperature;
float humidity;

void setup() {
  pinMode(DHTPin, INPUT);
  dht.begin(); 
  Serial.begin(115200);     
}
void loop() {
  temperature = dht.readTemperature(); // Gets the values of the temperature
  humidity = dht.readHumidity(); // Gets the values of the humidity
  if (isnan(temperature)) {
Serial.print("it is nan");
  }
  else{
Serial.print("Temperature = ");
  Serial.println(temperature);
  }
  
  //int temp = analogRead(pinTemp);    //Read the analog pin
  //temp = temp * 0.48828125;   // convert output (mv) to readable celcius
  //Serial.print("Temperature: ");
  //Serial.print(temp);
  //Serial.println("C");  //print the temperature status
  delay(1000);  
}