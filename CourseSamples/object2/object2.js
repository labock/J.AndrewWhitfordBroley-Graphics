// object2.js

//  Program to draw a green square and a blue triangle
//  Adapted from Angel & Shreiner 2D Sierpinski Gasket program
//  and from Whitford Holey's object2 program
//  by J. Andrew Whitford Holey, January 11, 2016


var gl;
var points = [];
var currentColorLoc;

window.onload = function init()
{
    var canvas = document.getElementById( "gl-canvas" );
    
    gl = WebGLUtils.setupWebGL( canvas );
    if ( !gl ) { alert( "WebGL isn't available" ); }

    //
    //  Initialize our data for the square and the triangle
    //

    // First, initialize the corners of the quad
    
    var quadVertices = [
        vec2( -0.75, -0.75 ), //  0
        vec2(  0.25, -0.75 ), //  1
        vec2(  0.25,  0.25 ), //  2
        vec2( -0.75,  0.25 ), //  3
    ];

    // Set up the quad as two triangles
    points.push( quadVertices[ 0] );
    points.push( quadVertices[ 1] );
    points.push( quadVertices[ 2] );

    points.push( quadVertices[ 0] );
    points.push( quadVertices[ 2] );
    points.push( quadVertices[ 3] );
    
    // Now, initialize the corners of the triangle

    var triaVertices = [
        vec2( 0.00, 0.00 ), //  0
        vec2( 0.70, 0.00 ), //  1
        vec2( 0.35, 0.80 ), //  2
    ];

    // Set up the triangle
    points.push( triaVertices[ 0] );
    points.push( triaVertices[ 1] );
    points.push( triaVertices[ 2] );

    //
    //  Configure WebGL
    //
    gl.viewport( 0, 0, canvas.width, canvas.height );
    gl.clearColor( 1.0, 1.0, 0.0, 1.0 ); // yellow background

    
    //  Load shaders and initialize attribute buffers
    var program = initShaders( gl, "vertex-shader", "fragment-shader" );
    gl.useProgram( program );

    
    // Load the data into the GPU
    var bufferId = gl.createBuffer();
    gl.bindBuffer( gl.ARRAY_BUFFER, bufferId );
    gl.bufferData( gl.ARRAY_BUFFER, flatten(points), gl.STATIC_DRAW );

    // Associate our shader variables with our data buffer
    
    var vPosition   = gl.getAttribLocation( program, "vPosition" );
    currentColorLoc = gl.getUniformLocation(program, "currentColor")
    gl.vertexAttribPointer( vPosition, 2, gl.FLOAT, false, 0, 0 );
    gl.enableVertexAttribArray( vPosition );

    render();
};


function render() {
    var startQuad     = 0;
    var numQuadPoints = 6;
    var startTriangle = numQuadPoints;
    var numTriaPoints = 3;

    var green = vec4( 0.0, 0.0, 1.0, 1.0 );
    var blue  = vec4( 0.0, 1.0, 0.0, 1.0 );

    // clear the canvas
    gl.clear( gl.COLOR_BUFFER_BIT );
    
    // draw the square
    gl.uniform4fv( currentColorLoc, green );
    gl.drawArrays( gl.TRIANGLES, startQuad, numQuadPoints );
    
    // draw the triangle
    gl.uniform4fv( currentColorLoc, blue );
    gl.drawArrays( gl.TRIANGLES, startTriangle, numTriaPoints );
}
