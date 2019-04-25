var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var sensor = {state:false};

app.get('/', function(req, res) {
  res.sendFile(__dirname + '/index.html');
});

io.on('connection', function(socket) {
  console.log('User connected: ' + socket.id);
  socket.emit('sensor', sensor);
  socket.on('disconnect', function(){
    console.log('User disconnected: ' + socket.id);
  });
  
  socket.on('dht-temp', function(message) { 
    console.log('id: ' + socket.id + "Temperature: " + message);
    io.sockets.emit('dht-temp', message);
  });
  socket.on('dht-humid', function(message) { 
    console.log('id: ' + socket.id + "Humidity:  " + message);
    io.sockets.emit('dht-humid', message);
  });
});

http.listen(3000, function() {
  console.log('listening on *:3000');
});
