// SwissFlag.js

// Program to draw a Swiss Flag (a white greek cross on a red background)
// Adapted from Angel & Shreiner 2D Sierpinski Gasket program
// by J. Andrew Whitford Holey, December 22, 2015

var gl;
var points = [];

window.onload = function init()
{
    var canvas = document.getElementById( "gl-canvas" );
    
    gl = WebGLUtils.setupWebGL( canvas );
    if ( !gl ) { alert( "WebGL isn't available" ); }

    //
    //  Initialize our data for the Greek cross
    //

    // First, initialize the corners of the cross
    
    var vertices = [
        vec2( -0.2, -0.2 ), //  0
        vec2( -0.6, -0.2 ), //  1
        vec2( -0.6,  0.2 ), //  2
        vec2( -0.2,  0.2 ), //  3
        vec2( -0.2,  0.6 ), //  4
        vec2(  0.2,  0.6 ), //  5
        vec2(  0.2,  0.2 ), //  6
        vec2(  0.6,  0.2 ), //  7
        vec2(  0.6, -0.2 ), //  8
        vec2(  0.2, -0.2 ), //  9
        vec2(  0.2, -0.6 ), // 10
        vec2( -0.2, -0.6 )  // 11
    ];

    // Set up the cross as six triangles
    points.push( vertices[ 1] );
    points.push( vertices[ 3] );
    points.push( vertices[ 2] );

    points.push( vertices[ 1] );
    points.push( vertices[ 0] );
    points.push( vertices[ 3] );

    points.push( vertices[11] );
    points.push( vertices[ 5] );
    points.push( vertices[ 4] );

    points.push( vertices[11] );
    points.push( vertices[10] );
    points.push( vertices[ 5] );

    points.push( vertices[ 9] );
    points.push( vertices[ 7] );
    points.push( vertices[ 6] );

    points.push( vertices[ 9] );
    points.push( vertices[ 8] );
    points.push( vertices[ 7] );


    //
    //  Configure WebGL
    //
    gl.viewport( 0, 0, canvas.width, canvas.height );
    gl.clearColor( 1.0, 0.0, 0.0, 1.0 ); // red background

    
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
