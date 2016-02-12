// object1.js

//  Program to draw a green square on a red background
//  in the center of the canvas
//  Adapted from Angel & Shreiner 2D Sierpinski Gasket program
//  and from Whitford Holey's Swiss Flag program
//  by J. Andrew Whitford Holey, January 11, 2016


var gl;
var points;
var k = 1;
var program;
var inputForm;

window.onload = function init()
{
    var canvas = document.getElementById( "gl-canvas" );
    inputForm = document.getElementById("nsinput");
    inputForm.onchange =
    function () {
      k = parseInt(inputForm.value);
      initPoints();
      setBuffer();
      render();
      console.log(k);
    };
    gl = WebGLUtils.setupWebGL( canvas );
    if ( !gl ) { alert( "WebGL isn't available" ); }

    //
    //  Configure WebGL
    //
    gl.viewport( 0, 0, canvas.width, canvas.height );
    gl.clearColor( 0.0, 0.2, 0.0, 0.5 ); // red background

    
    //  Load shaders and initialize attribute buffers
    program = initShaders( gl, "vertex-shader", "fragment-shader" );
    gl.useProgram( program );

    initPoints();
    setBuffer();
    render();
};

function initPoints(){
    points = [];
    var vertices = [

        //shape1
        vec2( -0.2, -0.2 ), //  0
        vec2(  0.2, -0.2 ), //  1
        vec2(  0.2,  0.2 ), //  2

        vec2(  0.0,  0.0 ), //3
        vec2(  0.0,  0.4 ), //4
        vec2(  0.4,  0.4 ),  //5

        //shape2
        vec2( -0.7, -0.2 ), 
        vec2( -0.3, -0.2 ), 
        vec2( -0.3,  0.2 ), 

        vec2( -0.5,  0.0 ), 
        vec2( -0.5,  0.4 ), 
        vec2( -0.1,  0.4 ),

        //shape3
        vec2(  0.3, -0.2 ), 
        vec2(  0.7, -0.2 ),
        vec2(  0.7,  0.2 ), 

        vec2(  0.5,  0.0 ), 
        vec2(  0.5,  0.4 ), 
        vec2(  0.9,  0.4 ),

        //shape4
        vec2( -0.45, -0.3 ), 
        vec2( -0.05, -0.3 ), 
        vec2( -0.45, -0.7 ), 

        vec2( -0.65, -0.9 ), 
        vec2( -0.25, -0.9 ), 
        vec2( -0.25, -0.5 ),

        //shape5
        vec2(  0.05, -0.3 ), 
        vec2(  0.45, -0.3 ), 
        vec2(  0.05, -0.7 ), 

        vec2( -0.15, -0.9 ), 
        vec2(  0.25, -0.9 ), 
        vec2(  0.25, -0.5 )
 
    ];
    var total = k*6;
    for(j=0; j<total; j++){
        points.push( vertices[j] );
    }
}

function setBuffer() {
    // Load the data into the GPU
    var bufferId = gl.createBuffer();
    gl.bindBuffer( gl.ARRAY_BUFFER, bufferId );
    gl.bufferData( gl.ARRAY_BUFFER, flatten(points), gl.STATIC_DRAW );

    // Associate our shader variables with our data buffer
    
    var vPosition = gl.getAttribLocation( program, "vPosition" );
    gl.vertexAttribPointer( vPosition, 2, gl.FLOAT, false, 0, 0 );
    gl.enableVertexAttribArray( vPosition );
}


function render() {
    gl.clear( gl.COLOR_BUFFER_BIT );
    gl.drawArrays( gl.TRIANGLES, 0, points.length );
}
