const THREE = require('three');
var OBJLoader = require('three-obj-loader');
OBJLoader(THREE);

function webgl(el) {
    
    var scene = new THREE.Scene();

    var VIEW_ANGLE = 45;
    var WIDTH = 640;
    var HEIGHT = 480;
    var ASPECT = WIDTH / HEIGHT;
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

    var loader = new THREE.OBJLoader();

    var texLoader = new THREE.TextureLoader();

    this.loadTextureMaterial = function (tex, load, err){
        texLoader.load(
            // resource URL
            tex,
        
            // onLoad callback
            function ( texture ) {
                // in this example we create the material when the texture is loaded
                var material = new THREE.MeshBasicMaterial( {
                    map: texture,
                    flatShading: true
                });
                if(load != null)
                    load(material);
            },
        
            // onProgress callback currently not supported
            undefined,
        
            // onError callback
            function ( error ) {
                if(err != null)
                    err(error);
                console.log("Failed to load "+ tex);
            }
        );
    }

    this.loadObj = function (obj, load, err){
        loader.load(
            // resource URL
            obj,
            // called when resource is loaded
            function ( object ) {
                if(load != null)
                    load(object);
                scene.add( object );
            },
            // called when loading is in progresses
            function ( xhr ) {},
            // called when loading has errors
            function ( error ) {
                if(err != null)
                    err(error);
                console.log("Failed to load "+ obj);
            }
        );
    }

    this.success = false;
    try{
        var renderer = new THREE.WebGLRenderer();
        this.renderer = renderer;

        renderer.setSize(WIDTH, HEIGHT);
        el.appendChild(renderer.domElement);
        el.addEventListener('resize', function(){
            renderer.setSize(WIDTH, HEIGHT);
        });

        this.render = function (){
            renderer.render(scene, camera);
        }
        this.success = true;
    }catch(e){}

    return this;
}

module.exports = { THREE: THREE, webgl: webgl };