//MicroSD writes to micro sd card connected to ethernet shield
#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;

void setup() {
	Serial.begin(9600);
	if (!SD.begin(chipSelect)) {							//initialize sd card
		Serial.println("microSD did not initialize...");
		return;
	}
	Serial.println("microSD ready!");


}

void loop() {
	String  dataString = "Hello world!";
	File dataFile = SD.open("log.txt", FILE_WRITE);
	if (dataFile) {
		dataFile.println(dataString);
		dataFile.close();
		Serial.println(dataString);
	}
	else {
		Serial.println("Could not open file");
	}
	delay(5000);
}