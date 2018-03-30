const THREE = require('three');

function stage(el) {
    
    var scene = new THREE.Scene();

    var VIEW_ANGLE = 45;
    var ASPECT = 640 / 480;
    var NEAR = 0.1;
    var FAR = 10000;
    var camera = new THREE.PerspectiveCamera(
        VIEW_ANGLE,
        ASPECT,
        NEAR,
        FAR
    );
    scene.add(camera);

    this.scene = scene;
    this.camera = camera;

    try{
        var renderer = new THREE.WebGLRenderer();
        this.renderer = renderer;

        renderer.setSize(el.style.width, el.style.height);
        el.appendChild(renderer.domElement);
        el.addEventListener('resize', function(){
            renderer.setSize(el.style.width, el.style.height);
        });

        this.render = function (){
            renderer.render(scene, camera);
        }
    }catch{}

    return this;
}

module.exports = { THREE: THREE, stage: stage };