//MicroSD reads from microsd a command file
#include "SPI.h"
#include "SD.h"

const int chipSelect = 4;
float refresh_rate = 0.0;

void setup() {
	Serial.begin(9600);
	if (!SD.begin(chipSelect)) {							//initialize sd card
		Serial.println("microSD did not initialize...");
		return;
	}
	Serial.println("microSD ready!");
	File commandFile = SD.open("commands.txt");
	if (commandFile) {
		Serial.println("Reading command.txt");
		float decade = pow(10, commandFile.available() - 1);
		while (commandFile.available()) {
			float temp = (commandFile.read() - '0');			//reads byte per byte
			refresh_rate = temp*decade + refresh_rate;
			decade = decade / 10;
		}
		Serial.print("Refresh rate = ");
		Serial.print(refresh_rate);
		Serial.println("ms");
	}
	else {
		Serial.println("Could not read command file");
	}

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
	delay(refresh_rate);
}