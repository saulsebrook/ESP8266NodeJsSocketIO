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
  
  socket.on('sensor', function(state) { 
    sensor.state = !sensor.state;
    console.log('id: ' + socket.id + ' sensor: ' + sensor.state);
    io.sockets.emit('sensor', sensor);
  });
});

http.listen(3000, function() {
  console.log('listening on *:3000');
});