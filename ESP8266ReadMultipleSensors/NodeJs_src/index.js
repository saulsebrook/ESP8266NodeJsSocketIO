var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var sensor = {state:false};
var light = {state:false};

app.get('/', function(req, res) {
  res.sendFile(__dirname + '/index.html');
});

io.on('connection', function(socket) {
  console.log('User connected: ' + socket.id);
  socket.emit('sensor', sensor);
  socket.emit('light', light);
  socket.on('disconnect', function(){
  console.log('User disconnected: ' + socket.id);
  });

// Listens for 'sensor' change in state suppose => sensor = false
// Line 1 changes state to true
// Line 2 console.logs that state is true
// Line 3 emits that state is true
    socket.on('sensor', function(state) { 
      sensor.state = !sensor.state;
      console.log('id: ' + socket.id + ' sensor: ' + sensor.state); // Not crucial to operation
      io.sockets.emit('sensor', sensor);
  });

// Same method as above
    socket.on('toggle', (state) => {
      light.state = !light.state;
      console.log('id: ' + socket.id + ' light: ' + light.state);
      io.sockets.emit('light', light);
    });

// DHT Sensor 
  socket.on('dht-temp', function(message) { 
    //sensor.state = !sensor.state;
    console.log('id: ' + socket.id + "Temperature: " + message);
    io.sockets.emit('dht-temp', message);
  });
  socket.on('dht-humid', function(message) { 
    //sensor.state = !sensor.state;
    console.log('id: ' + socket.id + "Humidity:  " + message);
    io.sockets.emit('dht-humid', message);
  });
});

http.listen(3000, function() {
  console.log('listening on *:3000');
});
