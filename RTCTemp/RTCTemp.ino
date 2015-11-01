#include "DHT.h"
#include "DS1302RTC.h"
#include "Time.h"

#define DHTPIN 22					// what digital pin we're connected to
#define DHTTYPE DHT22				// DHT 22  (AM2302), AM2321
#define DS1302_GND_PIN 37			// DS1302 GND pin
#define DS1302_VCC_PIN 39			// DS1302 VCC pin

unsigned long time = 1;				//sampling period in min
int num = 10;                       //# of samples to average
void TempHum(int num);
void SetupRTC();
void PrintTime();
DHT dht(DHTPIN, DHTTYPE);
DS1302RTC RTC(31, 33, 35);				// set RTC pins (RST,DAT,CLK)
time_t s = 0;


void setup() {
	Serial.begin(9600);
	dht.begin();
	SetupRTC();
}

void loop() {
	//TempHum(num);
	PrintTime();
}

void TempHum(int num) {
	float temp = 0;
	float hum = 0;
	for (int i = 1; i <= num; i++) {
		float h = dht.readHumidity();
		float t = dht.readTemperature();
		hum = hum + h;
		temp = temp + t;
		delay(time*60*1000/num); //delay between samples
		
	}
	Serial.print("Temp is: ");
	Serial.print(temp /num);
	Serial.print(" degrees Celsius\t");
	Serial.print("Hum is: ");
	Serial.print(hum /num);
	Serial.println(" %");
}

void SetupRTC() {
	int h = 12; int m = 22; int s = 0; int d = 30; int mo = 10; int y = 15;
	digitalWrite(DS1302_GND_PIN, LOW);			//enable DS1302
	pinMode(DS1302_GND_PIN, OUTPUT);
	digitalWrite(DS1302_VCC_PIN, HIGH);
	pinMode(DS1302_VCC_PIN, OUTPUT);
	if (RTC.haltRTC())
		Serial.println("RTC stopped");
	else
		Serial.println("RTC working");
	if (RTC.writeEN())
		Serial.println("Write allowed");
	else
		Serial.println("Write protected");
	Serial.println("Time sync");
	setSyncProvider(RTC.get);					//get time from the RTC
	if (timeStatus() == timeSet)
		Serial.println("Time is set");
	else
		Serial.println("Failed to set time");
	delay(2000);
	setTime(h, m, s, d, mo, y);
	RTC.set(now());
	
	
}

void PrintTime(){
	if (now() != s) {
		Serial.print(hour());
		Serial.print(":");
		Serial.print(minute());
		Serial.print(":");
		Serial.println(second());
		s = now();
	}

}