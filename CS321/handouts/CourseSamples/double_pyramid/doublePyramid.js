// doublePyramid.js

//  Program to draw a regular polygon with a specified number of vertices
//  Adapted from Angel & Shreiner 2D Sierpinski Gasket program
//  by J. Andrew Whitford Holey, January 14, 2016

var gl;
var points;
var colors;
var k = 4;
var program;
var inputForm;

//  Run this function when the window loads
window.onload = function init()
{
    var canvas = document.getElementById("gl-canvas");
    inputForm = document.getElementById("nsinput");
    inputForm.onchange =
      function () {
        k = parseInt(inputForm.value);
        initPoints();
        setBuffer();
        render();
      };

    
    gl = WebGLUtils.setupWebGL (canvas);
    if  (!gl) { alert ("WebGL isn't available"); }

    //  Configure WebGL
    gl.viewport (0, 0, canvas.width, canvas.height);
    gl.clearColor (0.4, 0.4, 0.4, 1.0); // medium gray background
    gl.enable (gl.DEPTH_TEST);

    
    //  Load shaders and initialize attribute buffers
    program = initShaders (gl, "vertex-shader", "fragment-shader");
    gl.useProgram (program);

    // Initialize points, buffer; render
    initPoints();
    setBuffer();
    render();
};


function initPoints() {
  var apex          = vec4 (0.0, 1.0, 0.0, 1.0);
  var bpex          = vec4 (0.0,-1.0, 0.0, 1.0);
  var baseVertices  = [];
/*  var apexColor     = vec4 (0.5, 1.0, 0.5, 1.0);
  var bpexColor     = vec4 (0.5, 0.0, 0.5, 1.0);
  var baseColors    = [];
*/  var vertsPerSlice = 6; // base triangle and side triangle

  var theta = 2.0 * Math.PI / k;
  points = [];
  colors = [];
  for  (i = 0; i < k; i++) {
    var angle = i * theta;
    var cosAngle = Math.cos(angle);
    var sinAngle = Math.sin(angle);
    baseVertices.push (vec4 (cosAngle, 0.0, sinAngle, 1.0));
//    baseColors.push (vec4 (0.5*(cosAngle+1), 0.5, 0.5*(sinAngle+1), 1.0));
  }

  for (i = 0; i < k-1; i++) {
    points.push (bpex);               //colors.push (bpexColor);
    points.push (baseVertices[i]);    //colors.push (baseColors[i]);
    points.push (baseVertices[i+1]);  //colors.push (baseColors[i]);
    points.push (apex);               //colors.push (apexColor);
    points.push (baseVertices[i+1]);  //colors.push (baseColors[i]);
    points.push (baseVertices[i]);    //colors.push (baseColors[i]);
  }
  points.push (bpex);               //colors.push (bpexColor);
  points.push (baseVertices[0]);    //colors.push (baseColors[k-1]);
  points.push (baseVertices[k-1]);  //colors.push (baseColors[k-1]);
  points.push (apex);               //colors.push (apexColor);
  points.push (baseVertices[k-1]);  //colors.push (baseColors[k-1]);
  points.push (baseVertices[0]);    //colors.push (baseColors[k-1]);
  
  // Generate random colors for each triangle
  for (i = 0; i < 3*2*k; i++) {
    var nextColor = vec4(0.0, 0.0, 0.0, 1.0);
    for (j = 0; j < 3; j++) {
      nextColor[j] = Math.random();
    }
    colors.push(nextColor);
//    colors.push(nextColor);
//    colors.push(nextColor);
  }
}

function setBuffer() {

    // Set up color buffer
    var cBuffer = gl.createBuffer ();
    gl.bindBuffer (gl.ARRAY_BUFFER, cBuffer);
    gl.bufferData (gl.ARRAY_BUFFER, flatten(colors), gl.STATIC_DRAW);

    var vColor = gl.getAttribLocation (program, "vColor");
    gl.vertexAttribPointer (vColor, 4, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray (vColor);

    // Set up vertex buffer
    var vBuffer = gl.createBuffer ();
    gl.bindBuffer (gl.ARRAY_BUFFER, vBuffer);
    gl.bufferData (gl.ARRAY_BUFFER, flatten(points), gl.STATIC_DRAW);
    
    var vPosition = gl.getAttribLocation (program, "vPosition");
    gl.vertexAttribPointer (vPosition, 4, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray (vPosition);

}


function render() {
    gl.clear (gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
    gl.drawArrays (gl.TRIANGLES, 0, points.length);
}
