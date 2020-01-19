#include <ESP8266WiFi.h>            
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_MCP4725.h>
  
Adafruit_MCP4725 MCP4725;

ESP8266WebServer server(80);   //Web server object. Will be listening in port 80 (default for HTTP)

// Assign output variables to GPIO pins

int speedFactor = 15;
uint32_t  voltage_value = 250;
void handleRecuperator() { 

if (server.arg("speedFactor")!= ""){     //Parameter found
speedFactor = server.arg("speedFactor").toInt();
voltage_value = 600 + abs(speedFactor %= 101)*15.3;
MCP4725.setVoltage(voltage_value, true);
}
server.send(200, "text/plain", String(speedFactor));          //Returns the HTTP response

}
void setup() {

 Serial.begin(9600);
 MCP4725.begin(0x60); // The I2C Address of my module 
 
WiFi.begin("ssid", "passwd"); //Connect to the WiFi network
while (WiFi.status() != WL_CONNECTED) { //Wait for connection
delay(500);
Serial.println("Waiting to connect…");
}
MCP4725.setVoltage(voltage_value, true); //About 10% of recuperator power
server.on("/recuperator", handleRecuperator); //Associate the handler function to the path
server.begin();                                       //Start the server
Serial.println("Server listening");   

}

void loop() {

server.handleClient();    //Handling of incoming requests

}
