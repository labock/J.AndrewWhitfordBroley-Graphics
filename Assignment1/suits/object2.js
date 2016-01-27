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

    // First, initialize the corners of the diamond
    
    var quadVertices = [
        vec2( -0.5, 0.9 ), //  0
        vec2( -0.8, 0.5 ), //  1
        vec2( -0.5,  0.1 ), //  2
        vec2( -0.2,  0.5 ), //  3
    ];

    // Set up the diamond as two triangles
    points.push( quadVertices[ 0] );
    points.push( quadVertices[ 1] );
    points.push( quadVertices[ 2] );

    points.push( quadVertices[ 0] );
    points.push( quadVertices[ 2] );
    points.push( quadVertices[ 3] );
    
    // Now, initialize the corners of the Spade (2triangles)

    var triaVertices = [
        vec2( 0.4, 0.1 ), //  0
        vec2( 0.6, 0.1 ), //  1
        vec2( 0.5, 0.2 ), //  2

        vec2( 0.2, 0.2 ), //  0
        vec2( 0.8, 0.2 ), //  1
        vec2( 0.5, 0.8 ), //  2
    ];

    // Set up the Spade
    points.push( triaVertices[ 0] );
    points.push( triaVertices[ 1] );
    points.push( triaVertices[ 2] );

    points.push(triaVertices[3 ]);
    points.push(triaVertices[4 ]);
    points.push(triaVertices[5 ]);


    var heartVertices = [
        vec2(.5,-0.9),
        vec2(.2,-0.5),
        vec2(.8,-0.5),

        vec2(.5,-.5),
        vec2(.35,-.2),
        vec2(.2,-.5),

        vec2(.5,-.5),
        vec2(.65,-.2),
        vec2(.8,-.5)
    ];

    //set up heart
    points.push( heartVertices[ 0] );
    points.push( heartVertices[ 1] );
    points.push( heartVertices[ 2] );

    points.push( heartVertices[ 5]);
    points.push( heartVertices[ 3] );
    points.push( heartVertices[ 4] );

    points.push( heartVertices[ 6]);
    points.push( heartVertices[ 7] );
    points.push( heartVertices[ 8] );

    //Initialize Club

    var clubVertices = [
        vec2( -0.4, -0.9 ), //  0
        vec2( -0.6, -0.9 ), //  1
        vec2( -0.5, -0.8 ), //  2

        vec2( -0.4, -0.8 ), 
        vec2( -0.4, -0.6 ), 
        vec2( -0.6, -0.8 ),

        vec2( -0.6, -0.6 ), 
        vec2( -0.4, -0.6 ), 
        vec2( -0.6, -0.8 ),

        vec2( -0.6, -0.6 ), 
        vec2( -0.8, -0.7 ), 
        vec2( -0.6, -0.8 ),

        vec2( -0.4, -0.8 ), 
        vec2( -0.4, -0.6 ), 
        vec2( -0.2, -0.7 ),

        vec2( -0.4, -0.6 ), 
        vec2( -0.6, -0.6 ), 
        vec2( -0.5, -0.4 )
    ];

    //Set up the club
    points.push( clubVertices[ 0] );
    points.push( clubVertices[ 1] );
    points.push( clubVertices[ 2] );

    points.push( clubVertices[ 3] );
    points.push( clubVertices[ 4] );
    points.push( clubVertices[ 5] );

    points.push( clubVertices[ 6] );
    points.push( clubVertices[ 7] );
    points.push( clubVertices[ 8] );

    points.push( clubVertices[ 9] );
    points.push( clubVertices[ 10] );
    points.push( clubVertices[ 11] );

    points.push( clubVertices[ 12] );
    points.push( clubVertices[ 13] );
    points.push( clubVertices[ 14] );

    points.push( clubVertices[ 15] );
    points.push( clubVertices[ 16] );
    points.push( clubVertices[ 17] );
    //
    //  Configure WebGL
    //
    gl.viewport( 0, 0, canvas.width, canvas.height );
    gl.clearColor( 0.95, 0.95, 0.95, 1.0 ); // grey background

    
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
    var numTriaPoints = 6;
    var startHeart    = numQuadPoints+numTriaPoints;
    var numHeartPoints= 9;
    var startClub     = numTriaPoints+numQuadPoints+numHeartPoints;
    var numClubPoints = 18;

    var red = vec4( 2.0, 0.0, 0.0, 1.0 );
    var black  = vec4( 0.0, 0.0, 0.0, 1.0 );

    // clear the canvas
    gl.clear( gl.COLOR_BUFFER_BIT );
    
    // draw the square
    gl.uniform4fv( currentColorLoc, red );
    gl.drawArrays( gl.TRIANGLES, startQuad, numQuadPoints );
    
    // draw the triangle
    gl.uniform4fv( currentColorLoc, black );
    gl.drawArrays( gl.TRIANGLES, startTriangle, numTriaPoints );

    gl.uniform4fv( currentColorLoc, red);
    gl.drawArrays(gl.TRIANGLES, startHeart, numHeartPoints);

    //draw the club
    gl.uniform4fv( currentColorLoc, black );
    gl.drawArrays( gl.TRIANGLES, startClub, numClubPoints);
}
