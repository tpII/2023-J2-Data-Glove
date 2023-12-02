import * as THREE from '/three/three.module.js';

var camera, scene, renderer;
var geometry, material, mesh;
var container;
var canvasWidth, canvasHeight;
var quaternion;
const socket = io();
var potAnterior = 0;
var progressBar;

init();
render();

function init() {
    progressBar = document.getElementById('progress-bar');
    quaternion = new THREE.Quaternion();
    const axesHelper = new THREE.AxesHelper(5);
    container = document.getElementById('canvas');
    document.body.appendChild(container);
    canvasWidth = container.offsetWidth;
    canvasHeight = container.offsetHeight;
    // Inicializa la camara de la escena
    camera = new THREE.PerspectiveCamera( 75, canvasWidth / canvasHeight, 0.01, 1000 );
    
    // Crea la escena
    scene = new THREE.Scene();
    scene.background = new THREE.Color(0xffffff);

    // Crea la figura 3D y le agrega una "malla" con la textura de la figura
    geometry = new THREE.BoxGeometry( 18, 18, 18 );
    material = new THREE.MeshNormalMaterial();
    mesh = new THREE.Mesh( geometry, material );
    mesh.name = "mesh";
    scene.add( mesh );
    
    axesHelper.setColors(new THREE.Color('rgb(0,0,255)'), new THREE.Color('rgb(0,255,0)'), new THREE.Color('rgb(255,0,0)'));
    scene.add(axesHelper);
    
    // Inicializa el renderizador, habilitando el antialiasing
    renderer = new THREE.WebGLRenderer( { antialias: true } );
    renderer.setSize( canvasWidth, canvasHeight );
    container.appendChild( renderer.domElement );

    // Agrega un listener para ver si la ventana cambia de tamaño
    window.addEventListener( 'resize', resize, false );

    // Posiciona la camara en la escena
    camera.position.x = 0;
    camera.position.y = 0;
    camera.position.z = 30;
    camera.rotation.set(0, 0, -90);

    document.getElementById('cuaternionX').innerHTML = "1";
    document.getElementById('cuaternionY').innerHTML = "0";
    document.getElementById('cuaternionZ').innerHTML = "0";
    document.getElementById('cuaternionW').innerHTML = "0";
}

function render(){
    socket.removeAllListeners('data');
    socket.on('data', (data) => {
        if(Math.abs(data.POT - potAnterior) > 2){
            potAnterior = data.POT;
            updateProgressBar(data.POT);
            scene.getObjectByName('mesh').scale.x = (data.POT/100);
        }
        document.getElementById('cuaternionX').innerHTML = parseFloat(data.SEq_1).toFixed(4);
        document.getElementById('cuaternionY').innerHTML = parseFloat(data.SEq_2).toFixed(4);
        document.getElementById('cuaternionZ').innerHTML = parseFloat(data.SEq_3).toFixed(4);
        document.getElementById('cuaternionW').innerHTML = parseFloat(data.SEq_4).toFixed(4);

        quaternion.set(parseFloat(data.SEq_1), parseFloat(data.SEq_2), parseFloat(data.SEq_3), parseFloat(data.SEq_4));
        // Rota la figura un cierto angulo en radianes, determinado por los valores (x, y, z)
        quaternion.y *= -1;
        scene.getObjectByName('mesh').rotation.setFromQuaternion(quaternion);
        socket.sendBuffer = [];
    });
    renderer.render( scene, camera );
    requestAnimationFrame(render);
}

function resize(){
    camera.aspect = container.offsetWidth / container.offsetHeight;
    camera.updateProjectionMatrix();
    renderer.setSize( container.offsetWidth, container.offsetHeight );
}

function updateProgressBar(progress) {
    progressBar.style.width = `${progress}%`;
    progressBar.innerHTML = `${progress}%`;
}
