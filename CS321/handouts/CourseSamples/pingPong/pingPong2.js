// File: polygon.js

// Program to draw a ball bouncing between two walls;
// each triangle in the ball is a different randomly-generated color.
// Adapted from Angel & Shreiner 2D Sierpinski Gasket, Color Cube programs
// and recursive sphere.
// by J. Andrew Whitford Holey, February 4, 2016

var canvas;
var gl;

// parameters for the walls (stretched cubes)
const numWallPoints = 36;      // 6 faces * 2 triangles/face * 3 vertices/triangle
const wallWidth     =  0.125;
const wallSX        =  0.0625; // 1/16 scale factor to get 1/8 width
const wallDX        =  0.9375; // move wall +|-15/16

// parameters for creating the ball
const divs          =  3; // number of recursive divisions
var   numBallPoints = 24; // actual value computed in init

// parameters for the ball transformation matrices
const radius = 0.25;
const sx = radius, sy = radius, sz = radius; // ball scale factors
var   theta = 0.0;                           // rotation
var   deltaTheta = 1.5;                      // rotation change
var   deltaDX = 1.0 / 128.0;                 // change in dx
var   dx = 0.0, dy = 0.0, dz = 0.0;          // translation factors
const compressLimit = radius / 16.0;         // compression limit
var   compressFactor = 1.0;                  // compression factor
var   phase = 0;                             // phase of compression cycle

// constant matrices
const scaleBall = scalem( sx, sy, sz );
const scaleWall = scalem( wallSX, 1.0, 1.0 );
const leftWall  = translate( -wallDX, 0.0, 0.0 ) * scaleWall;
const rightWall = translate(  wallDX, 0.0, 0.0 ) * scaleWall;

var numPoints;

var modelViewLoc;  // uniform location of the modelView matrix


/**
 * Recursively divides the triangle specified by
 * vertices a, b, c into 4 triangles divs times,
 * appends the vertices of the resulting triangles to points, and.
 * returns the number of vertices appended to points ( 3 * 4^divs ).
 *
 * @author  J. Andrew Whitford Holey
 * @version January 18, 2016
 * @param   a, b, c three vec4 vertices defining a triangle
 *                  (i.e., they are not collinear)
 * @param   divs    the number of (recursive) divisions to make
 * @param   points  the array of points to append the divided triangles to
 * @return  the number of vertices appended to points (
 */
function divideTriangle( a, b, c, divs, points ) {
  var result;
  if (divs > 0) { // subdivide the triangle into four
    // find the unit distance points midway between the vertices a, b, and c
    var v1 = unit(a + b);
    var v2 = unit(a + c);
    var v3 = unit(b + c);

    // subdivide the four resulting triangles
    divideTriangle(a, v1, v2, divs-1, points);
    divideTriangle(c, v2, v3, divs-1, points);
    divideTriangle(b, v3, v1, divs-1, points);
    result = 4 * divideTriangle(v1, v3, v2, divs-1, points);

  } else { // divs == 0; append this triangle to points
    points.push(a);
    points.push(b);
    points.push(c);
    result = 3;
  }
  return result;
}

/**
 * Generate a spherical polyhedron with vertices at
 * unit radius, centered at the origin.
 * This function generates the polyhedron by recursively
 * subdividing the faces of an octohedron; if
 * divs is 0, it generates the original octohedron.
 * The generated vertices are appended to points as triangles,
 * and the number of vertices generated is returned.
 * The number of vertices appended to points and
 * the number of edges around the circumference are as follows:
 *
 *   divs     points       edges around circumference
 *     0         24            4
 *     1         96            8
 *     2        384           16
 *     3       1536           32
 *     4       6144           64
 *     k         24 * 4^k      4 * 2^k
 *
 * Note that with smooth shading, divs = 4 is probably enough
 * to approximate a sphere for most purposes.
 *
 * @author  J. Andrew Whitford Holey
 * @version January 18, 2016
 * @param   divs    the number of (recursive) divisions to make
 * @param   points  the array of points to append the divided triangles to
 * @return  the number of vertices appended to points ( 24 * 4^divs )
 */
function spherichedron(divs, points) {

  var numFaces = 8;
  var pointsPerFace;

  var vertices = [
    vec4(0.0,  1.0,  0.0,  1.0), // 0 top
    vec4(0.0,  0.0,  1.0,  1.0), // 1 front
    vec4(1.0,  0.0,  0.0,  1.0), // 2 right
    vec4(0.0,  0.0, -1.0,  1.0), // 3 back
    vec4(-1.0, 0.0,  0.0,  1.0), // 4 left
    vec4(0.0, -1.0,  0.0,  1.0), // 5 bottom
  ];

  var faceIndices = [
    [ 0, 1, 2 ], // 0  upper right front face
    [ 0, 2, 3 ], // 1  upper right  rear face
    [ 0, 3, 4 ], // 2  upper  left  rear face
    [ 0, 4, 1 ], // 3  upper  left front face
    [ 5, 2, 1 ], // 4  lower right front face
    [ 5, 3, 2 ], // 5  lower right  rear face
    [ 5, 4, 3 ], // 6  lower  left  rear face
    [ 5, 1, 4 ], // 7  lower  left front face
  ];

  for (i = 0; i < numFaces; i++) {
    pointsPerFace =
      divideTriangle(vertices[faceIndices[i][0]],
                     vertices[faceIndices[i][1]],
                     vertices[faceIndices[i][2]],
                     divs,
                     points);
  }
  return numFaces * pointsPerFace;
}

window.onload = function init()
{
    canvas = document.getElementById("gl-canvas");
    
    gl = WebGLUtils.setupWebGL( canvas );
    if (!gl) { alert("WebGL isn't available"); }


    gl.viewport(0, 0, canvas.width, canvas.height);
    gl.clearColor(1.0, 1.0, 1.0, 1.0);
    
    gl.enable(gl.DEPTH_TEST);

    //
    //  Load shaders and initialize attribute buffers
    //
    var program = initShaders(gl, "vertex-shader", "fragment-shader");
    gl.useProgram(program);
    
    var cBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, cBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, flatten(colors), gl.STATIC_DRAW);

    var vColor = gl.getAttribLocation(program, "vColor");
    gl.vertexAttribPointer(vColor, 4, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(vColor);

    var vBuffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, flatten(points), gl.STATIC_DRAW);
    

    var vPosition = gl.getAttribLocation(program, "vPosition");
    gl.vertexAttribPointer(vPosition, 4, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(vPosition);

    modelViewLoc = gl.getUniformLocation(program, "model_view"); 
    
    //event listeners for buttons
    
    document.getElementById( "xButton" ).onclick = function () {
        axis = xAxis;
    };
    document.getElementById( "yButton" ).onclick = function () {
        axis = yAxis;
    };
    document.getElementById( "zButton" ).onclick = function () {
        axis = zAxis;
    };
        
    render();
}

function colorCube()
{
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
}

function quad(a, b, c, d) 
{
    var vertices = [
        vec4( -0.5, -0.5,  0.5, 1.0 ),
        vec4( -0.5,  0.5,  0.5, 1.0 ),
        vec4(  0.5,  0.5,  0.5, 1.0 ),
        vec4(  0.5, -0.5,  0.5, 1.0 ),
        vec4( -0.5, -0.5, -0.5, 1.0 ),
        vec4( -0.5,  0.5, -0.5, 1.0 ),
        vec4(  0.5,  0.5, -0.5, 1.0 ),
        vec4(  0.5, -0.5, -0.5, 1.0 )
    ];

    var vertexColors = [
        [ 0.0, 0.0, 0.0, 1.0 ],  // black
        [ 1.0, 0.0, 0.0, 1.0 ],  // red
        [ 1.0, 1.0, 0.0, 1.0 ],  // yellow
        [ 0.0, 1.0, 0.0, 1.0 ],  // green
        [ 0.0, 0.0, 1.0, 1.0 ],  // blue
        [ 1.0, 0.0, 1.0, 1.0 ],  // magenta
        [ 0.0, 1.0, 1.0, 1.0 ],  // cyan
        [ 1.0, 1.0, 1.0, 1.0 ]   // white
    ];

    // We need to parition the quad into two triangles in order for
    // WebGL to be able to render it.  In this case, we create two
    // triangles from the quad indices
    
    //vertex color assigned by the index of the vertex
    
    var indices = [ a, b, c, a, c, d ];

    for (var i = 0; i < indices.length; ++i) {
        points.push( vertices[indices[i]] );
        //colors.push( vertexColors[indices[i]] );
    
        // for solid colored faces use 
        colors.push(vertexColors[a]);
        
    }
}

function render()
{
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    theta[axis] += 2.0;
    gl.uniform3fv(thetaLoc, theta);

    gl.drawArrays( gl.TRIANGLES, 0, NumVertices );

    requestAnimFrame( render );
}

