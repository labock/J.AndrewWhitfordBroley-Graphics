/*
 * File: holeyShapes.h
 */

#ifndef HOLEY_SHAPES_H
#define HOLEY_SHAPES_H

/**
 * File with functions to generate a variety of useful 3D shapes for use in OpenGL,
 * and a few functions to generate colors.
 * Each shape is generated as a sequence of triangles in an array of 4D points.
 * It uses several basic definitions from the file "Angel.h", supplied with the
 * Angel & Shriener textbook, Interactive Computer Graphics, 6th Edition.
 * Each function takes a pointer to an array of point4 (vec4) points and a starting
 * index start; it returns the index of the next unused position in the array.
 * For every function, the number of available array elements needed is specified.
 */

#include "/usr/people/classes/CS321/include/Angel.h"

#ifndef point4
typedef Angel::vec4 point4;
#endif

#ifndef color4
typedef Angel::vec4 color4;
#endif


/*****************************************************************************
/*
/* Functions that general shapes as triangles of points
/*
/*****************************************************************************/

/**
 * Generate a cube with unit coordinates.
 * This code is adapted from code by Angel & Shriener from their book
 * Interactive Computer Graphics, 6th Edition.
 * This cube requires 36 vertices in the array points,
 * beginning at position start.
 */
int cube( point4 points[], int start) {

  const int NumVertices  = 8;
  const int NumFaces     = 6;
  const int VertsPerFace = 4;

  point4 vertices[NumVertices] = {
    point4( -1.0, -1.0,  1.0,  1.0 ), // 0
    point4( -1.0,  1.0,  1.0,  1.0 ), // 1
    point4(  1.0,  1.0,  1.0,  1.0 ), // 2
    point4(  1.0, -1.0,  1.0,  1.0 ), // 3
    point4( -1.0, -1.0, -1.0,  1.0 ), // 4
    point4( -1.0,  1.0, -1.0,  1.0 ), // 5
    point4(  1.0,  1.0, -1.0,  1.0 ), // 6
    point4(  1.0, -1.0, -1.0,  1.0 )  // 7
  };

  int faceIndices[NumFaces][VertsPerFace] = {
    { 1, 0, 3, 2 }, // 0  front face
    { 2, 3, 7, 6 }, // 1  right face
    { 3, 0, 4, 7 }, // 2  bottom face
    { 6, 5, 1, 2 }, // 3  top face
    { 4, 5, 6, 7 }, // 4  back face
    { 5, 4, 0, 1 }  // 5  left face
  };

  for (int i = 0; i < NumFaces; i++ ) {
    points[start++] = vertices[faceIndices[i][0]];
    points[start++] = vertices[faceIndices[i][1]];
    points[start++] = vertices[faceIndices[i][2]];
    points[start++] = vertices[faceIndices[i][0]];
    points[start++] = vertices[faceIndices[i][2]];
    points[start++] = vertices[faceIndices[i][3]];
  }
  return start;
}

/**
 * Generate a pyramid with a unit-radius k-gon base,
 * centered at the origin in the xz-plane, and an
 * apex at (0, 1, 0, 1).
 * A pyramid requires 6*k vertices in the array points,
 * beginning at position start.
 */
int pyramid( int k, point4 points[], int start) {

  point4 apex       = point4( 0.0, 1.0, 0.0, 1.0);
  point4 baseCenter = point4( 0.0, 0.0, 0.0, 1.0);
  point4 *baseVertices = new point4[k];

  double theta = 2 * M_PI / k;
  for (int i = 0; i < k; i++) {
    double angle = i * theta;
    baseVertices[i] = point4( cos(angle), 0.0, sin(angle), 1.0);
  }

  for (int i = 0; i < k-1; i++ ) {
    points[start++] = baseCenter;
    points[start++] = baseVertices[i];
    points[start++] = baseVertices[i+1];
    points[start++] = apex;
    points[start++] = baseVertices[i+1];
    points[start++] = baseVertices[i];
  }
  points[start++] = baseCenter;
  points[start++] = baseVertices[k-1];
  points[start++] = baseVertices[0];
  points[start++] = apex;
  points[start++] = baseVertices[0];
  points[start++] = baseVertices[k-1];

  return start;
}

/**
 * Generate a cylinder with a unit-radius k-gon base;
 * the cylinder is vertical, with the bases in the
 * y = -1 and y = 1 planes.
 * A cylinder requires 12*k vertices in the array points.
 */
int cylinder( int k, point4 points[], int start) {

  point4 topCenter       = point4( 0.0,  1.0, 0.0, 1.0);
  point4 bottomCenter    = point4( 0.0, -1.0, 0.0, 1.0);
  point4 *topVertices    = new point4[k];
  point4 *bottomVertices = new point4[k];

  double theta = 2 * M_PI / k;
  for (int i = 0; i < k; i++) {
    double angle = i * theta;
    topVertices[i]    = point4( cos(angle),  1.0, sin(angle), 1.0);
    bottomVertices[i] = point4( cos(angle), -1.0, sin(angle), 1.0);
  }

  for (int i = 0; i < k-1; i++ ) {
    // triangle for bottom base
    points[start++] = bottomCenter;
    points[start++] = bottomVertices[i];
    points[start++] = bottomVertices[i+1];
    // triangle for top base
    points[start++] = topCenter;
    points[start++] = topVertices[i+1];
    points[start++] = topVertices[i];
    // triangles for side rectangle
    points[start++] = bottomVertices[i];
    points[start++] = topVertices[i];
    points[start++] = topVertices[i+1];
    points[start++] = bottomVertices[i];
    points[start++] = topVertices[i+1];
    points[start++] = bottomVertices[i+1];
  }
  // last bottom triangle
  points[start++] = bottomCenter;
  points[start++] = bottomVertices[k-1];
  points[start++] = bottomVertices[0];
  // last top triangle
  points[start++] = topCenter;
  points[start++] = topVertices[0];
  points[start++] = topVertices[k-1];
  // last side triangles
  points[start++] = bottomVertices[k-1];
  points[start++] = topVertices[k-1];
  points[start++] = topVertices[0];
  points[start++] = bottomVertices[k-1];
  points[start++] = topVertices[0];
  points[start++] = bottomVertices[0];

  return start;
}


/**
 * Create a point 1.0 units from the origin
 * on the same line as the line between p and the origin.
 * Returns the new point unless p is too close to the
 * origin, in which case it returns p.
 */
point4 unit( const point4& p ) {

  GLfloat lenSqrd = p.x*p.x + p.y*p.y + p.z*p.z;
  point4 result = p;
  if (lenSqrd > DivideByZeroTolerance) {
    GLfloat len = sqrt(lenSqrd);
    result /= len;
    result.w = 1.0;
  }
  return result;
}

/**
 * Recursively divides the triangle specified by
 * vertices a, b, c into 4 triangles divs times.
 * Returns 3 * 4^divs
 */
int divideTriangle( int divs,
                    const point4& a,
                    const point4& b,
                    const point4& c,
                    point4 points[],
                    int start ) {
  if (divs > 0) {
    point4 v1 = unit( a + b );
    point4 v2 = unit( a + c );
    point4 v3 = unit( b + c );
    start = divideTriangle( divs-1,  a, v1, v2, points, start );
    start = divideTriangle( divs-1,  c, v2, v3, points, start );
    start = divideTriangle( divs-1,  b, v3, v1, points, start );
    start = divideTriangle( divs-1, v1, v3, v2, points, start );
  } else {
    points[start++] = a;
    points[start++] = b;
    points[start++] = c;
  }
  return start;
}


/**
 * Generate a spherical polyhedron with vertices at
 * unit radius, centered at the origin.
 * This function generates the polyhedron by recursively
 * subdividing the faces of an octohedron; if
 * divs is 0, it generates the original octohedron.
 * The number of points required is as follows:
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
 * returns start + 24 * 4^divs
 */
int spherichedron( int divs, point4 points[], int start) {

  const int NumVertices  = 6;
  const int NumFaces     = 8;
  const int VertsPerFace = 3;

  point4 vertices[NumVertices] = {
    point4(  0.0,  1.0,  0.0,  1.0 ), // 0 top
    point4(  0.0,  0.0,  1.0,  1.0 ), // 1 front
    point4(  1.0,  0.0,  0.0,  1.0 ), // 2 right
    point4(  0.0,  0.0, -1.0,  1.0 ), // 3 back
    point4( -1.0,  0.0,  0.0,  1.0 ), // 4 left
    point4(  0.0, -1.0,  0.0,  1.0 ), // 5 bottom
  };

  int faceIndices[NumFaces][VertsPerFace] = {
    { 0, 1, 2 }, // 0  upper right front face
    { 0, 2, 3 }, // 1  upper right  rear face
    { 0, 3, 4 }, // 2  upper  left  rear face
    { 0, 4, 1 }, // 3  upper  left front face
    { 5, 2, 1 }, // 4  lower right front face
    { 5, 3, 2 }, // 5  lower right  rear face
    { 5, 4, 3 }, // 6  lower  left  rear face
    { 5, 1, 4 }, // 7  lower  left front face
  };

  for (int i = 0; i < NumFaces; i++ ) {
    start = divideTriangle( divs,
                            vertices[faceIndices[i][0]],
                            vertices[faceIndices[i][1]],
                            vertices[faceIndices[i][2]],
                            points, start );
  }
  return start;
}

/**
 * Generates triangles representing a globe divided into latitude
 * and longitude segments;
 * the globe is centered at the origin and its vertices are unit distance
 * from the origin.
 *
 * @param longDivs number of divisions around the circumferences (xz-plane).
 *                 must be at least 3
 * @param latDivs  number of divisions from pole to pole,
 *                 must be at least 2
 * @param points   an array of at least 6 * longDivs * (latDivs - 1) points
 *                 beginning at position start
 * @param start    the position in points to beginning storing vertices
 * @return -1 if longDivs < 3 or latDivs < 2,
 *         start + 6 * longDivs * (latDivs - 1) otherwise
 */
int globe( int longDivs, int latDivs, point4 points[], int start ) {
  if (longDivs < 3 || latDivs < 2) return -1;
  const int numVertices = longDivs * (latDivs + 1);
  const GLfloat longAngleDiv = 2 * M_PI / longDivs;
  const GLfloat latAngleDiv  = M_PI / latDivs;

  const point4 northPole = point4( 0.0,  1.0, 0.0, 1.0 );
  const point4 southPole = point4( 0.0, -1.0, 0.0, 1.0 );

  point4 *vertices = new point4[numVertices];

  // put poles in first and last rows
  const int lastRow = numVertices - longDivs;
  for (int i = 0; i < longDivs; i++) {
    vertices[i]           = northPole;
    vertices[lastRow + i] = southPole;
  }
  // generate vertices in remaining rows
  for (int row = 1; row < latDivs; row++) {
    GLfloat latAngle = row * latAngleDiv;
    GLfloat latCos = cos(latAngle);
    GLfloat latSin = sin(latAngle);
    for (int i = 0; i < longDivs; i++) {
      GLfloat longAngle = i * longAngleDiv;
      GLfloat longCos = cos(longAngle);
      GLfloat longSin = sin(longAngle);
      GLfloat x = latSin * longCos;
      GLfloat y = latCos;
      GLfloat z = latSin * longSin;
      vertices[row * longDivs + i] = point4( x, y, z, 1.0 );
    }
  }

  // generate triangles in points
  for (int row = 1; row < latDivs; row++) {
    for (int i = 0; i < longDivs - 1; i++) {
      points[start++] = vertices[row * longDivs + i];
      points[start++] = vertices[(row-1) * longDivs + i];
      points[start++] = vertices[row * longDivs + i+1];
      points[start++] = vertices[row * longDivs + i];
      points[start++] = vertices[row * longDivs + i+1];
      points[start++] = vertices[(row+1) * longDivs + i+1];
    }
    points[start++] = vertices[(row+1) * longDivs - 1];
    points[start++] = vertices[row * longDivs - 1];
    points[start++] = vertices[row * longDivs];
    points[start++] = vertices[(row+1) * longDivs - 1];
    points[start++] = vertices[row * longDivs];
    points[start++] = vertices[(row+1) * longDivs];
  }

  delete [] vertices;
  return start;
}


/*****************************************************************************
/*
/* Functions that general colors
/*
/*****************************************************************************/

/**
 * Generate a random GLfloat in the range fMin ... fMax.
 * From code given at stackoverflow.com
 */
GLfloat fRandom( GLfloat fMin, GLfloat fMax ) {
  GLfloat f = ((GLfloat)random()) / RAND_MAX;
  return fMin + f * (fMax - fMin);
}

/**
 * Generate a random color, with alpha = 1.0.
 */
color4 randomColor( ) {
  return color4( fRandom( 0.0, 1.0 ), fRandom( 0.0, 1.0 ), fRandom( 0.0, 1.0 ), 1.0 );
}

/**
 * Generate a random color, with specified minimum and maximum color values.
 */
color4 randomColor( color4 minColor, color4 maxColor ) {
  return color4( fRandom( minColor[0], maxColor[0] ),
                 fRandom( minColor[1], maxColor[1] ),
                 fRandom( minColor[2], maxColor[2] ),
                 fRandom( minColor[3], maxColor[3] ) );
}

/**
 * Generate k random colors in the array colors,
 * starting at position start.
 * Returns (start + k).
 * Requires colors to be at least start + k in size.
 */
int randomColors( int k, color4 colors[], int start ) {
  for (int j = 0; j < k; j++) {
    for (int i = 0; i < 4; i++) {
      colors[start+j][i] = fRandom( 0.0, 1.0 );
    }
  }
  return start + k;
}  

/**
 * Generate k random colors in the array colors,
 * starting at position start;
 * the colors are constrained to be between
 * minColor and maxColor.
 * minColor and maxColor must have component values
 * between 0.0 and 1.0, and each component of minColor
 * must be less than or equal to the corresponding
 * component of maxColor.
 * Returns -1 (failure) if these constraints are not
 * met;
 * returns (start + k) on successful completion
 * Requires colors to be at least start + k in size.
 */
int randomColors( int k, color4 colors[], int start,
                  color4 minColor, color4 maxColor ) {

  // test for valid values of minColor and maxColor
  for (int i = 0; i < 4; i++) {
    if ( minColor[i] < 0.0 ||
         minColor[i] > maxColor[i] ||
         maxColor[i] > 1.0 ) {
      return -1;
    }
  }

  // generate the colors
  for (int j = 0; j < k; j++) {
    for (int i = 0; i < 4; i++) {
      colors[start+j][i] = fRandom( minColor[i], maxColor[i] );
    }
  }
  return start + k;
}

/**
 * Generate k random colors in triplicate in the array colors,
 * starting at position start.
 * That is, it fills colors with 3*k element, every three a random color.
 * Returns (start + 3*k).
 * Requires colors to be at least start + 3*k in size.
 */
int randomTriangleColors( int k, color4 colors[], int start ) {
  for (int j = 0; j < k; j++) {
    color4 newColor = color4(fRandom( 0.0, 1.0 ), fRandom( 0.0, 1.0 ),
                             fRandom( 0.0, 1.0 ), fRandom( 0.0, 1.0 ));
    colors[start++] = newColor;
    colors[start++] = newColor;
    colors[start++] = newColor;
  }
  return start;
}  

/**
 * Generate k random colors in triplicate in the array colors,
 * starting at position start;
 * the colors are constrained to be between
 * minColor and maxColor.
 * minColor and maxColor must have component values
 * between 0.0 and 1.0, and each component of minColor
 * must be less than or equal to the corresponding
 * component of maxColor.
 * That is, it fills colors with 3*k element, every three a random color.
 * Returns -1 (failure) if these constraints are not
 * met;
 * returns (start + 3*k) on successful completion
 * Requires colors to be at least start + k in size.
 */
int randomTriangleColors( int k, color4 colors[], int start,
                          color4 minColor, color4 maxColor ) {

  // test for valid values of minColor and maxColor
  for (int i = 0; i < 4; i++) {
    if ( minColor[i] < 0.0 ||
         minColor[i] > maxColor[i] ||
         maxColor[i] > 1.0 ) {
      return -1;
    }
  }

  // generate the colors
  for (int j = 0; j < k; j++) {
    color4 newColor = color4(fRandom( minColor[0], maxColor[0] ),
                             fRandom( minColor[1], maxColor[1] ),
                             fRandom( minColor[2], maxColor[2] ),
                             fRandom( minColor[3], maxColor[3] ));
    colors[start++] = newColor;
    colors[start++] = newColor;
    colors[start++] = newColor;
  }
  return start;
}

/**
 * Generates colors fo a globe generated by the function above;
 * each vertex has a unique random color.
 *
 * @param longDivs number of divisions around the circumferences (xz-plane).
 *                 must be at least 3
 * @param latDivs  number of divisions from pole to pole,
 *                 must be at least 2
 * @param colors   an array of at least 6 * longDivs * (latDivs - 1) colors
 *                 beginning at position start
 * @param start    the position in points to beginning storing colors
 * @return -1 if longDivs < 3 or latDivs < 2,
 *         start + 6 * longDivs * (latDivs - 1) otherwise
 */
int globeColors( int longDivs, int latDivs, color4 colors[], int start ) {
  if (longDivs < 3 || latDivs < 2) return -1;

  const int numVertices = longDivs * (latDivs + 1);

  const color4 northPole = randomColor( );
  const color4 southPole = randomColor( );

  point4 *vertexColors = new point4[numVertices];

  // put poles in first and last rows
  const int lastRow = numVertices - longDivs;
  for (int i = 0; i < longDivs; i++) {
    vertexColors[i]           = northPole;
    vertexColors[lastRow + i] = southPole;
  }
  // generate colors in remaining rows
  for (int row = 1; row < latDivs; row++) {
    for (int i = 0; i < longDivs; i++) {
      vertexColors[row * longDivs + i] = randomColor( );
    }
  }

  // assign colors for triangles
  for (int row = 1; row < latDivs; row++) {
    for (int i = 0; i < longDivs - 1; i++) {
      colors[start++] = vertexColors[row * longDivs + i];
      colors[start++] = vertexColors[(row-1) * longDivs + i];
      colors[start++] = vertexColors[row * longDivs + i+1];
      colors[start++] = vertexColors[row * longDivs + i];
      colors[start++] = vertexColors[row * longDivs + i+1];
      colors[start++] = vertexColors[(row+1) * longDivs + i+1];
    }
    colors[start++] = vertexColors[(row+1) * longDivs - 1];
    colors[start++] = vertexColors[row * longDivs - 1];
    colors[start++] = vertexColors[row * longDivs];
    colors[start++] = vertexColors[(row+1) * longDivs - 1];
    colors[start++] = vertexColors[row * longDivs];
    colors[start++] = vertexColors[(row+1) * longDivs];
  }

  delete [] vertexColors;
  return start;

}


/*****************************************************************************
/*
/* Functions that generate normals
/*
/*****************************************************************************/

/**
 * Generate the vector normal to a triangle represented by three points
 *
 * @param  a, b, c  The three points of the triangle
 * @return NULL if the vector cannot be computed (colinear or same points);
 *         otherwise, the normal vector to the triangle the three points make
 */
vec3 triangleNormal( const point4 & a, const point4 & b, const point4 & c ) {
  vec3 result = normalize( cross( b - a, c - b ) );
  return result;
}

/**
 * Generate the flat normals for an object, represented as 2*k triangles.
 * This cube requires 3*numTriangles vectors in the array normals,
 * beginning at position start.
 */
int flatNormals( const int numTriangles, const point4 points[],
                 vec3 normals[], const int start) {

  const int numPoints = 3 * numTriangles;

  for (int face = 0; face < numTriangles; face++ ) {
    int offset = start + 3 * face;
    vec3 normal = triangleNormal( points[offset],
                                  points[offset+1],
                                  points[offset+2] );
    for (int i = 0; i < 3; i++) {
      normals[offset+i] = normal;
    }
  }
  return start + numPoints;
}

/**
 * Generate the normals for a unit-radius spherical object,
 * where each normal vector is the first three coordinates of each point.
 * This cube requires numPoints vectors in the array normals,
 * beginning at position start.
 */
int sphericalNormals( const int numPoints, const point4 points[],
                      vec3 normals[], const int start) {
  for (int i = 0; i < numPoints; i++ ) {
    point4 point = points[start+i];
    normals[start+i] = vec3( point.x, point.y, point.z );
  }
  return start + numPoints;
}


#endif
