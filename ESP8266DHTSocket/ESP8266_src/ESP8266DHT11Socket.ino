#include <SocketIOClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE    DHT11

DHT dht(DHTPIN, DHTTYPE);

int tracker = 0;

String host = "192.168.1.117";
int port = 3000;
bool clicked = false;

SocketIOClient socket;

void setupNetwork() {

  WiFi.begin("WIFINAME", "PASSWORD"); 
  Serial.print("Waiting to connect...");
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
}

void setup() {

  dht.begin();
  Serial.begin(115200);

  setupNetwork();

  socket.connect(host, port);
}

void sendTemp() {
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
  delay(5000);
}

void loop() {
  sendTemp();
}
