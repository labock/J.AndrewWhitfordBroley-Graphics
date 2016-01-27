// object1.js

//  Program to draw a green square on a red background
//  in the center of the canvas
//  Adapted from Angel & Shreiner 2D Sierpinski Gasket program
//  and from Whitford Holey's Swiss Flag program
//  by J. Andrew Whitford Holey, January 11, 2016


var gl;
var points = [];

window.onload = function init()
{
    var canvas = document.getElementById( "gl-canvas" );
    
    gl = WebGLUtils.setupWebGL( canvas );
    if ( !gl ) { alert( "WebGL isn't available" ); }

    //
    //  Initialize our data for the square
    //

    // First, initialize the corners of the square
    
    var vertices = [
        vec2( -1, 1 ), //  0
        vec2(  0.0, 1 ), //  1
        vec2(  -1,  0.0 ), //  2
        
        vec2(-.5,.5),
        vec2(-.5,-.5),
        vec2(.5,.5),

        vec2(0,0),
        vec2(1,0),
        vec2(0,-1)

    ];

    // Set up the quad as two triangles
    points.push( vertices[ 0] );
    points.push( vertices[ 1] );
    points.push( vertices[ 2] );

    points.push( vertices[ 3] );
    points.push( vertices[ 4] );
    points.push( vertices[ 5] );

    points.push( vertices[ 6] );
    points.push( vertices[ 7] );
    points.push( vertices[ 8] );


    //
    //  Configure WebGL
    //
    gl.viewport( 0, 0, canvas.width, canvas.height );
    gl.clearColor( 0.3, 0.7, 0.6, 1.0 ); // red background

    
    //  Load shaders and initialize attribute buffers
    var program = initShaders( gl, "vertex-shader", "fragment-shader" );
    gl.useProgram( program );

    
    // Load the data into the GPU
    var bufferId = gl.createBuffer();
    gl.bindBuffer( gl.ARRAY_BUFFER, bufferId );
    gl.bufferData( gl.ARRAY_BUFFER, flatten(points), gl.STATIC_DRAW );

    // Associate our shader variables with our data buffer
    
    var vPosition = gl.getAttribLocation( program, "vPosition" );
    gl.vertexAttribPointer( vPosition, 2, gl.FLOAT, false, 0, 0 );
    gl.enableVertexAttribArray( vPosition );

    render();
};


function render() {
    gl.clear( gl.COLOR_BUFFER_BIT );
    gl.drawArrays( gl.TRIANGLES, 0, points.length );
}
