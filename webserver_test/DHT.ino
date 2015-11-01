#include <dht.h>

dht DHT;
#define DHT22_PIN 53


void setup(){

	Serial.begin(9600);
	pinMode(49,OUTPUT); //DHT22 GND on pin 49
	digitalWrite(49,LOW);

}

void loop(){
	DHT22.read22(DHT22_PIN);
	Serial.print(DHT.temperature,1);
	delay(2000);

}