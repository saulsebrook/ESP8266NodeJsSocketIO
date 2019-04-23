#include <SocketIOClient.h>

#define LedPin 2
#define ButtonPin 0

String host = "192.168.1.117";
int port = 3000;
bool clicked = false;

SocketIOClient socket;

void setupNetwork() {

  WiFi.begin("It Burns When IP", "lucy1816647"); 
  Serial.print("Waiting to connect...");
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
}

void click() {
  clicked = true;
}

void light(String state) {
  Serial.println("[light] " + state);
  if (state == "\"state\":true") {
    Serial.println("[light] ON");
    digitalWrite(LedPin, HIGH);
  }
  else {
    Serial.println("[light] off");
    digitalWrite(LedPin, LOW);
  }
}

void setup() {

  pinMode(LedPin, OUTPUT);
  pinMode(ButtonPin, INPUT);
  digitalWrite(LedPin, LOW);

  Serial.begin(115200);
  setupNetwork();
  attachInterrupt(digitalPinToInterrupt(ButtonPin), click, FALLING);    

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
  clickCheck();
}
