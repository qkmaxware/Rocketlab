const THREE = require('three');

function webgl(el) {
    
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

    var loader = new THREE.OBJLoader();

    var texLoader = new THREE.TextureLoader();

    this.loadTextureMaterial = function (tex, load, err){
        texLoader.load(
            // resource URL
            'textures/land_ocean_ice_cloud_2048.jpg',
        
            // onLoad callback
            function ( texture ) {
                // in this example we create the material when the texture is loaded
                var material = new THREE.MeshBasicMaterial( {
                    map: texture
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
            }
        );
    }

    this.success = false;
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
        this.success = true;
    }catch(e){}

    return this;
}

module.exports = { THREE: THREE, webgl: webgl };