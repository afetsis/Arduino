//webserver_test.ino
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; // define mac address
IPAddress ip(192,168,1,20);						   // define ip address
EthernetServer server(80);						   // define http server port

void setup() {
		Serial.begin(9600);
		Ethernet.begin(mac,ip);
		Serial.print("server is at: ");
		Serial.println(Ethernet.localIP());
}

void loop() {
		EthernetClient client = server.available(); // listen for clients
		
		if (client){
			bool currentLineIsBlank = true;
			Serial.print("new client");

			while(client.connected()){
				if(client.available()){				// client data available to read
					char c = client.read();			// read 1 byte (character) from client            // last line of client request is blank and ends with \n                // respond to client only after last line received

					if((c =='\n') && (currentLineIsBlank)){
						//client.println("HTTP/1.1 200 OK");				//http standard response
                    	//client.println("Content-Type: text/html");
                    	//client.println("Connection: close");
                    	//client.println();
                    	//sent web page
                    	client.println("<!DOCTYPE html>");
                    	client.println("<html>");
                    	client.println("<head>");
                    	client.println("<title>Arduino Web Page</title>");
                    	client.println("</head>");
                    	client.println("<body>");
                    	client.println("<h1>Hello from Arduino!</h1>");
                    	client.println("<p>A web page from the Arduino server</p>");
                    	client.println("</body>");
                    	client.println("</html>");
                    	break;
                    	if (c == '\n') {		// last character on line of received text		// starting new line with next character read
                    		currentLineIsBlank = true;
                		} 
                		else if (c != '\r') {	// a text character was received from client
                    		currentLineIsBlank = false;
                		}
					}
				}
			}

		}
}
