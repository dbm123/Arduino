/*
  Web client
 
 This sketch connects to a website (http://www.google.com)
 using a WiFi shield.
 
 This example is written for a network using WPA encryption. For 
 WEP or WPA, change the Wifi.begin() call accordingly.
 
 This example is written for a network using WPA encryption. For 
 WEP or WPA, change the Wifi.begin() call accordingly.
 
 Circuit:
 * WiFi shield attached
 
 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe
 */


#include <SPI.h>
#include <WiFi.h>

char ssid[] = "lab-i5";
//char ssid[] = "splash"; //  your network SSID (name) 
char pass[] = "a1b1c1d1e1";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)
// http://arduino.cc/en/Reference/WiFi


int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "ec2-54-218-35-199.us-west-2.compute.amazonaws.com";    // name address for Google (using DNS)

const int led9=9;
boolean LED9On = false;  

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

void setup() {
   pinMode(led9,OUTPUT);
   digitalWrite(led9,HIGH);
    
  
  //Initialize serial and wait for port to open:
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true) {  // fast blink of led9
      blinkLED(led9, 10, 30);
    }
  } 

  // print a list of available networks
  //scanNetworks();

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    Serial.print("Password: ");
    Serial.println(pass);
    Serial.println("...waiting for WiFi.begin to return");
    blinkLED(led9, 10, 30);

    /* Connect to WPA/WPA2 network with two parameters. WEP requires three    
       WiFi.begin(ssid, pass); // for WPA
         pass: WPA encrypted networks use a password in the form of a string for security.
       WiFi.begin(ssid, keyIndex, key); 
         keyIndex: WEP encrypted networks can hold up to 4 different keys. This identifies which key you are going to use.
         key: a hexadecimal string used as a security code for WEP encrypted networks.
         
       http://arduino.cc/en/Reference/WiFiBegin

      DBM 10/20/13 Seems to me that you have to know you are going for a WEP connection before
      you try to make the connection. See examples ConnectWithWEP or ConnectWithWPA

    */
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    //delay(10000);
    if (status != WL_CONNECTED) {
      Serial.println("...failed to connect");
      delay(2000);
      Serial.println("...going to try again");
    }
  } // while
  Serial.println("Success - connected ...");
  printWifiStatus();

  byte mac[6]; // the MAC address of your Wifi shield
  String MACString;  
  WiFi.macAddress(mac);

  String MAC2Digits; 
  int i;
  for (i = 5; i > -1; i--) {
    //Serial.print( i );
    MAC2Digits = String(mac[i], HEX);
    if (MAC2Digits.length() == 1)
      MAC2Digits = "0" + MAC2Digits;
    //Serial.print(" ");
    //Serial.print(MAC2Digits);
    //Serial.print(" ");
    //Serial.println(MAC2Digits.length());
    MACString = MACString + MAC2Digits;
    if (i > 0) MACString = MACString + ":"; // only add the colon between digits
    //Serial.println(MACString);
  }
  MACString.toUpperCase();
  Serial.println(MACString);

  /*
  MACString = String(mac[5], HEX) + ":" + 
   String(mac[4], HEX) + ":" +
   String(mac[3], HEX) + ":" +
   String(mac[2], HEX) + ":" +
   String(mac[1], HEX) + ":" +
   String(mac[0], HEX);
   MACString.toUpperCase();
   */

  /*
  Serial.println(MACString);
   Serial.print("MAC: ");
   Serial.print(mac[5],HEX);
   Serial.print(":");
   Serial.print(mac[4],HEX);
   Serial.print(":");
   Serial.print(mac[3],HEX);
   Serial.print(":");
   Serial.print(mac[2],HEX);
   Serial.print(":");
   Serial.print(mac[1],HEX);
   Serial.print(":");
   Serial.println(mac[0],HEX);
   */

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:

  blinkLED(led9, 50, 30); // all good - give it a rapid flash
  
  String tempString;
  String probenumString;

  for (i = 0; i < 10000; i++) {
    toggleL9();
    
    tempString = String(i, DEC); // sin() in radians

    // a stupid routine to alternate probenum between 0 and 1 based on i
    probenumString = String(i, BIN); // convert to binary string
    char lsbchar;
    lsbchar = probenumString.charAt(probenumString.length()-1);  // grab the LSB

    String PostData = "deviceId=" + MACString + "&temperature=" + tempString + "&probenum=" + lsbchar;
    if (client.connect(server, 80)) {
      client.println( "POST /post-temp.php HTTP/1.1" );
      client.println( "Host: ec2-54-218-35-199.us-west-2.compute.amazonaws.com" );
      client.println( "Content-Type: application/x-www-form-urlencoded" );
      client.println( "Connection: close" );
      client.print( "Content-Length: " );
      client.println( PostData.length() );
      client.println();
      client.print( PostData );
      client.println();
      Serial.println( PostData ); // print it out
      delay(30000); // wait xx seconds (push interval) (1500 = 1.5s)
    }
    else { // returned false -- just try again
      Serial.println( "fail: " + tempString);  
      // flash LED9 rapidly x times
      blinkLED(led9, 100, 30);
    } 
  } // for
  /*
    client.println("POST /insert-test.php HTTP/1.1");
   client.println("Host: 54.218.35.199");
   client.println("User-Agent: Arduino/1.0");
   //client.println("Connection: close");
   client.print("Content-Length: ");
   client.println(PostData.length());
   client.println();
   client.println(PostData);
   */

  /*
  if (client.connect("ec2-54-218-35-199.us-west-2.compute.amazonaws.com", 80)) {
   Serial.println("connected to server");
   // Make a HTTP request:
   client.println("POST /insert-test.php?deviceIdName=1963&temperatureName=91413 HTTP/1.1");
   client.println("Host: www.google.com");
   client.println("Connection: close");
   client.println();
   }
   else {
     Serial.println("\nFailed to connect to server...");
   }
   */
}

void blinkLED( int nLED, int nTimes, int nDelay) {
  for (int i = 1; i <= nTimes; i++) {
    //Serial.print(".");
    digitalWrite(nLED,HIGH);
    delay(nDelay);
    digitalWrite(nLED,LOW);
    delay(nDelay);
  }
  //Serial.println(".");
}      

void toggleL9() {
  //if ( (i % 2) == 0) { do_something(); }
  if (LED9On == true) {
    // turn LED9 off
    digitalWrite(led9,LOW);
    LED9On = false;
  }
  else {
    // turn LED9 on 
    digitalWrite(led9,HIGH);
    LED9On = true;
  }
  
}

void loop() {
  // if there are incoming bytes available 
  // from the server, read them and print them:
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();

    // do nothing forevermore:
    while(true);
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  
  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption,HEX);
  Serial.println();
}

void scanNetworks() {
  // scan for nearby networks:
  Serial.println("** Scanning networks **");
  byte numSsid = WiFi.scanNetworks();

  // print the list of networks seen:
  Serial.print("SSID List:");
  Serial.println(numSsid);
  // print the network number and name for each network found:
  for (int thisNet = 0; thisNet<numSsid; thisNet++) {
    Serial.print(thisNet);
    Serial.print(") Network: ");
    Serial.println(WiFi.SSID(thisNet));
  }
}

