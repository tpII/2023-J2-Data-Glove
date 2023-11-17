import * as THREE from '/three/three.module.js';

var camera, scene, renderer;
var geometry, material, mesh, edges, line;
var container;
var canvasWidth, canvasHeight;
const socket = io()

init();
render();

function init() {
    container = document.getElementById('canvas');
    document.body.appendChild(container);
    canvasWidth = container.offsetWidth;
    canvasHeight = container.offsetHeight;
    // Inicializa la camara de la escena
    camera = new THREE.PerspectiveCamera( 70, canvasWidth / canvasHeight, 0.01, 10 );
    
    // Crea la escena
    scene = new THREE.Scene();
    scene.background = new THREE.Color(0xffffff);

    // Crea la figura 3D y le agrega una "malla" con la textura de la figura
    geometry = new THREE.BoxGeometry( 0.3, 0.3, 0.3 );
    material = new THREE.MeshNormalMaterial();
    mesh = new THREE.Mesh( geometry, material );
    scene.add( mesh );

    // Agrega lineas blancas para delimitar los bordes de las caras del cubo
    edges = new THREE.EdgesGeometry( geometry );
    line = new THREE.LineSegments(edges, new THREE.LineBasicMaterial( { color: 0x222021 } ) ); 
    scene.add( line );  
    
    // Inicializa el renderizador, habilitando el antialiasing
    renderer = new THREE.WebGLRenderer( { antialias: true } );
    renderer.setSize( canvasWidth, canvasHeight );
    container.appendChild( renderer.domElement );

    // Agrega un listener para ver si la ventana cambia de tamaño
    window.addEventListener( 'resize', resize, false );

    // Posiciona la camara en la escena
    camera.position.x = 0;
    camera.position.y = 0;
    camera.position.z = 0.5;

    document.getElementById('cuaternionX').innerHTML = "1";
    document.getElementById('cuaternionY').innerHTML = "0";
    document.getElementById('cuaternionZ').innerHTML = "0";
    document.getElementById('cuaternionW').innerHTML = "0";
}

function render(){
    socket.on('data', (data) => {
        document.getElementById('cuaternionX').innerHTML = data.SEq1.toFixed(4);
        document.getElementById('cuaternionY').innerHTML = data.SEq2.toFixed(4);
        document.getElementById('cuaternionZ').innerHTML = data.SEq3.toFixed(4);
        document.getElementById('cuaternionW').innerHTML = data.SEq4.toFixed(4);

        const quaternion = new THREE.Quaternion(data.SEq1, data.SEq2, data.SEq3, data.SEq4);
        // Rota la figura un cierto angulo en radianes, determinado por los valores (x, y, z)
        mesh.rotation.setFromQuaternion(quaternion)
        line.rotation.setFromQuaternion(quaternion)
    });
    
    renderer.render( scene, camera );
    requestAnimationFrame(render);
}

function resize(){
    camera.aspect = container.offsetWidth / container.offsetHeight;
    camera.updateProjectionMatrix();
    renderer.setSize( container.offsetWidth, container.offsetHeight );
}
