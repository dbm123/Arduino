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
char ssid[] = "splash"; //  your network SSID (name) 
char pass[] = "a1b1c1d1e1";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "ec2-54-218-35-199.us-west-2.compute.amazonaws.com";    // name address for Google (using DNS)



// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  } 

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  } 
  Serial.println("Connected to wifi");
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

  String tempString;

  for (i = 0; i < 10000; i++) {
    tempString = String(i, DEC); // sin() in radians

    // String PostData = "deviceId=1963&temperature=451";
    String PostData = "deviceId=" + MACString + "&temperature=" + tempString;
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
      delay(60000); // wait 60 seconds
    }
    else {
      Serial.println( "fail: " + tempString);  
    } 
  }
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
}

