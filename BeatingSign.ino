/*
    This sketch sends data via HTTP GET requests to data.sparkfun.com service.

    You need to get streamId and privateKey at data.sparkfun.com and paste them
    below. Or just customize this script to talk to other HTTP servers.

*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid     = "Penthouse";
const char* password = "ollinthebuilding";
//const char* ssid     = "GRAAL-WLAN";
//const char* password = "p1n0c4s4l1n0";

//const char* url = "http://olimexsmart.it/embe/getCurrentHR.php";
const char* url = "http://192.168.1.44/embe/getCurrentHR.php";

bool flag = false;
byte HR;
unsigned long epoch;
int r, g, b;
//int  randR, randG, 


void setup() {
  pinMode(D8, OUTPUT);
  pinMode(D9, OUTPUT);
  pinMode(D10, OUTPUT);

  digitalWrite(D8, HIGH);
  digitalWrite(D9, HIGH);
  digitalWrite(D10, HIGH);

  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  epoch = millis();
}



void loop() {
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http;  //Declare an object of class HTTPClient
    http.begin(url);  //Specify request destination
    int httpCode = http.GET(); //Send the request
    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      HR = (byte) payload.toInt();
      Serial.println("HR: " + payload);  //Print the response payload
    }
    http.end();   //Close connection
  }

  if (HR <= 0) {
    HR = 60;
    Serial.println("Manage wrong data in DB");
  }


  while (millis() - epoch < 60000) { // Cycle inside for a minute

    heartBeat(255, 255, 255, 1023);

    delay(60000 / HR);
  }

  epoch = millis();
}


/*
   FUNCTIONS
*/

void heartBeat(byte r, byte g, byte b, int intensity) {
  int midway = 2 * (intensity / 3);
  int lowend = intensity / 10;
  int agg;


  // Increasing
  for (int i = 0; i < intensity; i++) {
    agg = deLog(i);
    analogWrite(D8, agg);
    analogWrite(D9, agg);
    analogWrite(D10, agg);
    delayMicroseconds(100);
  }

  // Decreasing
  for (int i = intensity; i > lowend; i--) {
    agg = deLog(i);
    analogWrite(D8, agg);
    analogWrite(D9, agg);
    analogWrite(D10, agg);
    delayMicroseconds(100);
  }

  delay(25);
  // Increasing
  for (int i = lowend; i < midway; i++) {
    agg = deLog(i);
    analogWrite(D8, agg);
    analogWrite(D9, agg);
    analogWrite(D10, agg);
    delayMicroseconds(150);
  }

  // Decreasing
  for (int i = midway; i > lowend; i--) {
    agg = deLog(i);
    analogWrite(D8, agg);
    analogWrite(D9, agg);
    analogWrite(D10, agg);
    delayMicroseconds(750);
  }
  for (int i = lowend; i > 0; i--) {
    agg = deLog(i);
    analogWrite(D8, agg);
    analogWrite(D9, agg);
    analogWrite(D10, agg);
    delayMicroseconds(2000);
  }

}


int deLog(int x) {
  return ((511500.0) / ((x * 0.671) - 1023.0)) + 1523.0;
}

