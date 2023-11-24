import * as THREE from '/three/three.module.js';

var camera, scene, scene1, scene2, renderer;
var geometry, material, mesh, edges, line;
var geometry2, material2, mesh2
var container;
var canvasWidth, canvasHeight;
var quaternion;
const socket = io();
var changeSceneButton;

init();
render();

function init() {
    const axesHelper = new THREE.AxesHelper(5);
    container = document.getElementById('canvas');
    document.body.appendChild(container);
    canvasWidth = container.offsetWidth;
    canvasHeight = container.offsetHeight;
    // Inicializa la camara de la escena
    camera = new THREE.PerspectiveCamera( 75, canvasWidth / canvasHeight, 0.01, 1000 );
    
    // Crea la escena
    scene1 = new THREE.Scene();
    scene1.background = new THREE.Color(0xffffff);

    // Crea la figura 3D y le agrega una "malla" con la textura de la figura
    geometry = new THREE.BoxGeometry( 18, 18, 18 );
    material = new THREE.MeshNormalMaterial();
    mesh = new THREE.Mesh( geometry, material );
    scene1.add( mesh );
    axesHelper.setColors(new THREE.Color('rgb(0,0,255)'), new THREE.Color('rgb(0,255,0)'), new THREE.Color('rgb(255,0,0)'));
    scene1.add(axesHelper);

    // Agrega lineas blancas para delimitar los bordes de las caras del cubo
    edges = new THREE.EdgesGeometry( geometry );
    line = new THREE.LineSegments(edges, new THREE.LineBasicMaterial( { color: 0x222021 } ) ); 
    scene1.add( line );
    
    // Crea la escena
    scene2 = new THREE.Scene();
    scene2.background = new THREE.Color(0xffffff);

    // Crea la figura 3D y le agrega una "malla" con la textura de la figura
    geometry2 = new THREE.SphereGeometry( 15, 32, 16 );
    material2 = new THREE.MeshNormalMaterial();
    mesh2 = new THREE.Mesh( geometry2, material2 );
    scene2.add( mesh2 );
    
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

    document.getElementById('cuaternionX').innerHTML = "1";
    document.getElementById('cuaternionY').innerHTML = "0";
    document.getElementById('cuaternionZ').innerHTML = "0";
    document.getElementById('cuaternionW').innerHTML = "0";

    scene = scene1;
    changeSceneButton = document.getElementById('scene1');
}

function render(){
    socket.on('data', (data) => {
        // document.getElementById('cuaternionX').innerHTML = parseFloat(data.SEq_1).toFixed(4);
        // document.getElementById('cuaternionY').innerHTML = parseFloat(data.SEq_2).toFixed(4);
        // document.getElementById('cuaternionZ').innerHTML = parseFloat(data.SEq_3).toFixed(4);
        // document.getElementById('cuaternionW').innerHTML = parseFloat(data.SEq_4).toFixed(4);

        quaternion = new THREE.Quaternion(parseFloat(data.SEq_1), parseFloat(data.SEq_2), parseFloat(data.SEq_3), parseFloat(data.SEq_4));
        // Rota la figura un cierto angulo en radianes, determinado por los valores (x, y, z)
        mesh.rotation.setFromQuaternion(quaternion)
        if(scene == scene1)
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

changeSceneButton.onclick = function(elem){
    console.log("Entra a la funcion")
    if(scene == scene1)
        scene = scene2;
    else scene = scene1;
}
