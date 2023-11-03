import * as THREE from '/three/three.module.js';

var camera, scene, renderer;
var geometry, material, mesh, edges, line;
var x, y, z;
const socket = io()

init();
render();

function init() {
    x = 0;
    y = 0;
    z = 0;

    // Inicializa la camara de la escena
    camera = new THREE.PerspectiveCamera( 70, window.innerWidth / window.innerHeight, 0.01, 10 );
    
    // Crea la escena
    scene = new THREE.Scene();

    // Crea la figura 3D y le agrega una "malla" con la textura de la figura
    geometry = new THREE.BoxGeometry( 0.2, 0.2, 0.2 );
    material = new THREE.MeshNormalMaterial();
    mesh = new THREE.Mesh( geometry, material );
    scene.add( mesh );

    // Agrega lineas blancas para delimitar los bordes de las caras del cubo
    edges = new THREE.EdgesGeometry( geometry );
    line = new THREE.LineSegments(edges, new THREE.LineBasicMaterial( { color: 0xffffff } ) ); 
    scene.add( line );  
    
    // Inicializa el renderizador, habilitando el antialiasing
    renderer = new THREE.WebGLRenderer( { antialias: true } );
    renderer.setSize( window.innerWidth, window.innerHeight );
    document.body.appendChild( renderer.domElement );

    // Agrega un listener para ver si la ventana cambia de tamaño
    window.addEventListener( 'resize', resize, false );

    // Posiciona la camara en la escena
    camera.position.x = 0;
    camera.position.y = 0;
    camera.position.z = 0.5;

    document.getElementById('datos').innerHTML = "Cuaternión:  X: 0; Y: 0; Z: 0; W: 0";
}

function render(){
    socket.on('data', (data) => {
        //console.log(data.x);
        // x = data.x;
        // y = data.y;
        // z = data.z;

        document.getElementById('datos').innerHTML = "Cuaternión  X: " + data.SEq2 + "; Y: " + data.SEq3 + "; Z: " + data.SEq1 + "; W: " + data.SEq4;

        const quaternion = new THREE.Quaternion(data.SEq1, data.SEq2, data.SEq3, data.SEq4);
        // Rota la figura un cierto angulo en radianes, determinado por los valores (x, y, z)
        mesh.rotation.setFromQuaternion(quaternion)
        line.rotation.setFromQuaternion(quaternion)
    });
    
    renderer.render( scene, camera );
    requestAnimationFrame(render);
}

function resize(){
    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();
    renderer.setSize( window.innerWidth, window.innerHeight );
}
