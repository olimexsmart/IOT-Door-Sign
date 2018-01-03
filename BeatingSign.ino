/*
    This sketch sends data via HTTP GET requests to data.sparkfun.com service.

    You need to get streamId and privateKey at data.sparkfun.com and paste them
    below. Or just customize this script to talk to other HTTP servers.

*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define SPEED 500
#define RED D8
#define GREEN D9
#define BLUE D10

const char* ssid     = "Penthouse";
const char* password = "ollinthebuilding";
//const char* ssid     = "GRAAL-WLAN";
//const char* password = "p1n0c4s4l1n0";

//const char* url = "http://olimexsmart.it/embe/getCurrentHR.php";
const char* url = "http://192.168.1.44/embe/getCurrentHR.php";

bool flag = false;
byte HR;
unsigned long minute, lastBeat;
unsigned int pause;
int r, g, b;
int  randR, randG, randB;
bool dirR, dirG, dirB;


void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, HIGH);

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

  // Initialize vars
  randomSeed(analogRead(A0));
  minute = millis();
  lastBeat = millis();
  r = random(1023);
  g = random(1023);
  b = random(1023);
  randR = random(SPEED);
  randG = random(SPEED);
  randB = random(SPEED);
  dirR = random(1);
  dirG = random(1);
  dirB = random(1);
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

  pause = 60000 / HR;


  while (millis() - minute < 60000) { // Cycle inside for a minute

    backgroundUpdate();
    applyRGB(0);

    if (millis() - lastBeat > pause) {
      heartBeat(1023);
      lastBeat = millis();
    }

    delay(10);
  }

  minute = millis();
}


/* #########################################################################################
   FUNCTIONS
*/

void heartBeat(int intensity) {
  int midway = 2 * (intensity / 3);
  int lowend = intensity / 10;

  // Increasing
  for (int i = 0; i < intensity; i++) {
    applyRGB(i);
    delayMicroseconds(100);
  }

  // Decreasing
  for (int i = intensity; i > lowend; i--) {
    applyRGB(i);
    delayMicroseconds(100);
  }

  delay(25);
  // Increasing
  for (int i = lowend; i < midway; i++) {
    applyRGB(i);
    delayMicroseconds(150);
  }

  // Decreasing
  for (int i = midway; i > lowend; i--) {
    applyRGB(i);
    delayMicroseconds(750);
  }
  for (int i = lowend; i > 0; i--) {
    applyRGB(i);
    delayMicroseconds(2000);
  }

}


int deLog(int x) {
  return ((511500.0) / ((x * 0.671) - 1023.0)) + 1523.0;
}

void backgroundUpdate() {
  if (randR == random(SPEED))
    dirR = !dirR;
  if (randG == random(SPEED))
    dirG = !dirG;
  if (randB == random(SPEED))
    dirB = !dirB;

  r = dirR ? (r + 1) : (r - 1);
  g = dirG ? (g + 1) : (g - 1);
  b = dirB ? (b + 1) : (b - 1);

  r = constrain(r, 0, 1023);
  g = constrain(g, 0, 1023);
  b = constrain(b, 0, 1023);
}

void applyRGB(int bias) {
  analogWrite(RED, deLog(constrain(r + bias, 0, 1023)));
  analogWrite(GREEN, deLog(constrain(g + bias, 0, 1023)));
  analogWrite(BLUE, deLog(constrain(b + bias, 0, 1023)));
}

