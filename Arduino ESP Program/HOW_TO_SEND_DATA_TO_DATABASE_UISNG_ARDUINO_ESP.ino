#include "SoftwareSerial.h"
String ssid ="ORL";
String password="orl_india";
SoftwareSerial monitor(10, 11);// RX, TX
String data;
String server = "192.168.1.8"; // www.example.com
String uri = "/test.php";// our example is /esppost.php
byte dat [5];
String temp ,hum;
void setup() 
{
  monitor.begin(9600);
  Serial.begin(115200);
  monitor.println("DATA BASE TEST");
  delay(500);
  reset();
  delay(500);
  connectWifi();
  delay(500);
}
//reset the esp8266 module
void reset() 
{
  Serial.println("AT+RST");
  delay(1000);
  if(Serial.find("OK") ) monitor.println("Module Reset");
}
//connect to your wifi network
void connectWifi() 
{
  Serial.println("AT+CWMODE=3");
  delay(2000);
  String cmd = "AT+CWJAP=\"" +ssid+"\",\"" + password + "\"";
  Serial.println(cmd);
  delay(4000);
  if(Serial.find("OK"))
  {
    monitor.println("Connected!");
  }
  else 
  {
      connectWifi();
      monitor.println("Cannot connect to wifi"); 
  }
}

void loop () 
{
  int n=analogRead(A1);
 
  hum =String(n);
  data = "sensor_value=" + hum;
  httppost();
  delay(1000);
}
void httppost ()
{
  Serial.println("AT+CIPSTART=\"TCP\",\"" + server + "\",80");//start a TCP connection.
  if( Serial.find("OK")) 
  {
    monitor.println("TCP connection ready");
  }
  delay(1000);
  String postRequest ="POST " + uri + " HTTP/1.0\r\n" +"Host: " + server + "\r\n" +"Accept: *" + "/" + "*\r\n" +
  "Content-Length: " + data.length() + "\r\n" +
  "Content-Type: application/x-www-form-urlencoded\r\n" +
  "\r\n" + data;

  String sendCmd = "AT+CIPSEND=";//determine the number of caracters to be sent.

  Serial.print(sendCmd);

  Serial.println(postRequest.length() );

  delay(500);

  if(Serial.find(">"))
  {
    monitor.print(">");
    monitor.print(postRequest);
    Serial.print(postRequest);
  }
  else
  {
    Serial.println("AT+CIPCLOSE");
  }
  if(Serial.find("OK"))
  {
    monitor.println("RECEIVED: OK");
  }
  else
  {
    monitor.println("RECEIVED: Error");
  }
}
