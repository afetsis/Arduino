#include "EthernetUdp.h"
#include "Ethernet.h"
#include "DS1302RTC.h"
#include "Time.h"
#include "SPI.h"

#define DS1302_GND_PIN 37			// DS1302 GND pin
#define DS1302_VCC_PIN 39			// DS1302 VCC pin

void PrintTime();
void SetupRTC();
DS1302RTC RTC(31, 33, 35);				// set RTC pins (RST,DAT,CLK)
time_t s = 0;
EthernetUDP Udp;


byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
byte ip[] = {192,168,1,20};
unsigned int localPort = 8888;
char timeServer[] = "time.windows.com";
const int NTP_PACKET_SIZE = 48;
char packetBuffer[NTP_PACKET_SIZE];



void setup() {
	Serial.begin(9600);
	while(!Serial){;}
	Ethernet.begin(mac, ip);
	Udp.begin(localPort);
	Serial.println("waiting for sync");
	SetupRTC();
}

void loop() {
	//PrintTime();	
}



void SetupRTC(){
	digitalWrite(DS1302_GND_PIN, LOW);			//enable DS1302
	pinMode(DS1302_GND_PIN, OUTPUT);
	digitalWrite(DS1302_VCC_PIN, HIGH);
	pinMode(DS1302_VCC_PIN, OUTPUT);
	Serial.println("Time sync");
	setSyncInterval(3600);
	setSyncProvider(getNtpTime);		//get time from server
	
}

unsigned long getNtpTime(){
	sendNTPpacket(timeServer);		// send an NTP packet to a time server
	delay(2000);
	if (Udp.parsePacket()){
			Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
														   //the timestamp starts at byte 40 of the received packet and is four bytes,
														   // or two words, long. First, esxtract the two words:
			unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
			unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
			// combine the four bytes (two words) into a long integer // this is NTP time (seconds since Jan 1 1900):
			unsigned long secsSince1900 = highWord << 16 | lowWord;
			// now convert NTP time into Arduino Time format: // Time starts on Jan 1 1970. In seconds, that's 2208988800:
			const unsigned long seventyYears = 2208988800UL;     // subtract seventy years:
			unsigned long epoch = secsSince1900 - seventyYears;
			time_t t = epoch;
			setTime(t);
			adjustTime(7200); // UTC+2 Athens time zone
	}
	Ethernet.maintain();
	return 0; // return 0 if unable to get the time
}

unsigned long sendNTPpacket(char *address){
	// set all bytes in the buffer to 0
	memset(packetBuffer, 0, NTP_PACKET_SIZE);
	// Initialize values needed to form NTP request
	// (see URL above for details on the packets)
	packetBuffer[0] = 0b11100011; // LI, Version, Mode
	packetBuffer[1] = 0; // Stratum, or type of clock
	packetBuffer[2] = 6; // Polling Interval
	packetBuffer[3] = 0xEC; // Peer Clock Precision
							// 8 bytes of zero for Root Delay & Root Dispersion
	packetBuffer[12] = 49;
	packetBuffer[13] = 0x4E;
	packetBuffer[14] = 49;
	packetBuffer[15] = 52;
	// all NTP fields have been given values, now
	// you can send a packet requesting a timestamp:
	Udp.beginPacket(address, 123);
	Udp.write(packetBuffer, NTP_PACKET_SIZE); //NTP requests are to port 123
	Udp.endPacket();
	
}

void PrintTime() {

	if (now() != s) {
		if (hour() < 10) {
			Serial.print("0");
			Serial.print(hour());
		}
		else
			Serial.print(hour());
		
		Serial.print(":");
		if (minute() < 10) {
			Serial.print("0");
			Serial.print(minute());
		}
		else
			Serial.print(minute());
		Serial.print(":");
		if (second() < 10) {
			Serial.print("0");
			Serial.println(second());
		}
		else
			Serial.println(second());
		s = now();
	}

}