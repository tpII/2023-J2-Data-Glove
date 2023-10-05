const express = require('express');
const app = express();
const port = 8000;
const { Server } = require('socket.io');
const http = require('http');
const server = http.createServer(app);
const io = new Server(server);

app.use('/socket.io', express.static(__dirname + '/node_modules/socket.io/client-dist'));
app.use('/main.js', express.static(__dirname + '/views/main.js'));
app.use('/three/three.module.js', express.static(__dirname + '/node_modules/three/build/three.module.js'));
app.set('PORT', port)

// Configurar middleware para analizar JSON y URL-encoded
app.use(express.json());
app.use(express.urlencoded({ extended: false }));

// Landing
app.get('/', (req, res) => {
  res.sendFile(__dirname + "/views/index.html");
});

server.on('connection', (socket) => {
  console.log('WebSocket connection established');  
});

// POST route
app.post('/data', (req, res) => {
  const postData = req.body;
  // Conversion de grados a radianes
  postData.x = - postData.x * Math.PI / 180;
  postData.y = - postData.y * Math.PI / 180;
  postData.z = - postData.z * Math.PI / 180;
  io.emit('data', postData);

  res.json({ message: 'POST request received', data: postData });
});

server.listen(port, () => {
  console.log(`Server is running on port ${port}`);
});
