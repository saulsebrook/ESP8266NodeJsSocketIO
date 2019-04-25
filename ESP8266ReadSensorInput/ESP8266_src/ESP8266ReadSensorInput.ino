#include <SocketIOClient.h>

#define LedPin 4

String host = "192.168.1.117";
int port = 3000;
bool clicked = false;
int tracker = 0;

SocketIOClient socket;

void setupNetwork() {
// Connect to local WiFi network
  WiFi.begin("WIFINAME", "PASSWORD"); 
  Serial.print("Waiting to connect...");
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }

}

// Digital pin is pulled LOW and goes HIGH when switch is closed
void sensor(String state) {
  digitalRead(LedPin);
  if(digitalRead(LedPin) == HIGH){
  Serial.println("[sensor] " + state);
  if (state == "\"state\":true") {
    Serial.println("[sensor] ON");
    
  }
  else {
    Serial.println("[sensor] off");
    digitalWrite(LedPin, LOW);
  }
  }
}

void setup() {

  pinMode(LedPin, INPUT);
  Serial.begin(115200);
  setupNetwork();
  socket.on("sensor", sensor);
  socket.connect(host, port);

}

void loop() {
  socket.monitor();
  if(digitalRead(LedPin) == HIGH && tracker == 0){
    socket.emit("sensor", "{\"state\":true}");
    tracker = 1;
  }
  if(digitalRead(LedPin) == LOW && tracker == 1){
    socket.emit("sensor", "{\"state\":false}");
    tracker = 0;
  }
  delay(1000);
}
