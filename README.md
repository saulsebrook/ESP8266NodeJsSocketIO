# ESP8266NodeJsSocketIO

Code to allow an ESP8266 talk to a locally hosted Node.js server implementing socket.io.


ESP8266DHTSocket => Using a DHT11 sensor to send live readings to a website, readings are updated asynchronously

ESP8266NodeServerLEDToggle => Using socket.io open up persistent communication between a webpage and ESP8266, the webpage has a button that can turn on the LED remotely and also updates site to indicate LED status, LED can also be toggled from ESP8266 and LED status is emitted through to site

ESP8266ReadSensorInput => Proof of concept to get the status of a switch on a live website

ESP8266ReadMultipleSensors => A collection of all of the above all collated together on the one site





#TODO increase the delay of the DHT sensor, work out a way to run a function asynchronously using c++
