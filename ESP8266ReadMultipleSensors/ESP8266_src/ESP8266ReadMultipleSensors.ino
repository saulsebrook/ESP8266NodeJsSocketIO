#include <SocketIOClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE    DHT11
#define Sensor 4
#define LED 5
#define ButtonPin 0

DHT dht(DHTPIN, DHTTYPE);
// Variable so that the If Else statement in the loop will only run once
int tracker = 0;
unsigned long currentTime;
long timeDelay = 30000;
unsigned long previousTime = 0;

String host = "192.168.1.2";
int port = 8080;
bool clicked = false;

SocketIOClient socket;

void setupNetwork() {
    WiFi.begin("It Burns When IP", "lucy1816647"); 
    Serial.println("Waiting to connect...");
    while (WiFi.status() != WL_CONNECTED) { 
        delay(500);
        Serial.print(".");
    }
}

void click() {
  clicked = true;
}

void sendTemp() {
  currentTime = millis();
  if(currentTime - previousTime >= timeDelay){
    previousTime = currentTime;
    if (isnan(dht.readTemperature())) {
      Serial.println(F("Failed to read from DHT sensor!"));
    }
    else{
      Serial.print(F("Temperature: ")); 
      Serial.println(dht.readTemperature());
      socket.emit("dht-temp", String(dht.readTemperature()));
    }
    if (isnan(dht.readHumidity())){ 
      socket.emit("dht", "Error gettiing humidity");
    }
    else{
      Serial.print(F("Humidity: "));
      Serial.println(String(dht.readHumidity()));
      socket.emit("dht-humid", String(dht.readHumidity())); 
    }
  }
}

void light(String state) {
    Serial.println("[light] " + state);
    if (state == "\"state\":true") {
        Serial.println("[light] ON");
        digitalWrite(LED, HIGH);
    }
    else {
        Serial.println("[light] off");
        digitalWrite(LED, LOW);
    }
}

void setup() {

    dht.begin();
    pinMode(Sensor, OUTPUT);
    pinMode(ButtonPin, INPUT);
    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);
    digitalWrite(Sensor, LOW);

    Serial.begin(115200);
    setupNetwork();

    attachInterrupt(digitalPinToInterrupt(ButtonPin), click, FALLING);    
    // .on listens for an event, in this case "light" then runs the function light()
    socket.on("light", light);
    socket.connect(host, port);
}

void clickCheck() {
  if (clicked) {
    Serial.println("[click]");
    socket.emit("toggle", "{\"state\":true}");
    clicked = false;
  }  
}

void loop() {
    socket.monitor();
    
    if(digitalRead(Sensor) == HIGH && tracker == 0){
        socket.emit("sensor", "{\"state\":true}");
        tracker = 1;
    }
    if(digitalRead(Sensor) == LOW && tracker == 1){
        socket.emit("sensor", "{\"state\":false}");
        tracker = 0;
    }
    clickCheck();
    sendTemp();
}
