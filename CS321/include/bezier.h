/*
 * File: bezier.h
 */

#ifndef BEZIER_H
#define BEZIER_H

/*
 * Functions to process bezier patches,
 * adapted from Angel & Schreiner 6th edition
 */

#include "/usr/people/classes/CS321/include/Angel.h"

#ifndef point4
typedef Angel::vec4 point4;
#endif

#define BACK_TO_FRONT -1
#define FRONT_TO_BACK  1

//----------------------------------------------------------------------------

/**
 * Returns the number of quadrilaterals in a Bezier patch
 * subdivided subdivisions times.
 *
 * @param  subdivisions the number of times to subdivide,
 *                      meaningless for negative values
 * @return the number of quadrilaterals in a Bezier patch
 *         subdivided subdivisions times
 */
inline int
numQuadsPerPatch( int subdivisions ) {
    return (int) pow( 4, subdivisions );
}

//----------------------------------------------------------------------------

inline void
draw_patch( point4 p[4][4], int orientation,
            point4 points[], vec3 normals[], vec2 texCoords[], int start,
            GLfloat texSstart, GLfloat texSend, GLfloat texTstart, GLfloat texTend )
{
    // Compute the normal vectors
    vec3 normal00 = orientation * normalize( cross ( p[0][1] - p[0][0], p[1][0] - p[0][0] ));
    vec3 normal30 = orientation * normalize( cross ( p[2][0] - p[3][0], p[3][1] - p[3][0] ));
    vec3 normal33 = orientation * normalize( cross ( p[3][2] - p[3][3], p[2][3] - p[3][3] ));
    vec3 normal03 = orientation * normalize( cross ( p[1][3] - p[0][3], p[0][2] - p[0][3] ));

    // Compute the texture coordinates
    vec2 tex00 = vec2( texSstart, texTstart );
    vec2 tex30 = vec2( texSend, texTstart );
    vec2 tex33 = vec2( texSend, texTend );
    vec2 tex03 = vec2( texSstart, texTend );

    // Draw the quad (as two triangles) bounded by the corners of the
    //   Bezier patch.
  if (orientation == BACK_TO_FRONT) {
    points[start]      = p[0][0];
    normals[start]     = normal00;
    texCoords[start++] = tex00;
    points[start]      = p[3][0];
    normals[start]     = normal30;
    texCoords[start++] = tex30;
    points[start]      = p[3][3];
    normals[start]     = normal33;
    texCoords[start++] = tex33;
    points[start]      = p[0][0];
    normals[start]     = normal00;
    texCoords[start++] = tex00;
    points[start]      = p[3][3];
    normals[start]     = normal33;
    texCoords[start++] = tex33;
    points[start]      = p[0][3];
    normals[start]     = normal03;
    texCoords[start++] = tex03;
  } else { // orientation == FRONT_TO_BACK
    points[start]      = p[0][0];
    normals[start]     = normal00;
    texCoords[start++] = tex00;
    points[start]      = p[3][3];
    normals[start]     = normal33;
    texCoords[start++] = tex33;
    points[start]      = p[3][0];
    normals[start]     = normal30;
    texCoords[start++] = tex30;
    points[start]      = p[0][0];
    normals[start]     = normal00;
    texCoords[start++] = tex00;
    points[start]      = p[0][3];
    normals[start]     = normal03;
    texCoords[start++] = tex03;
    points[start]      = p[3][3];
    normals[start]     = normal33;
    texCoords[start++] = tex33;
  }
}

//----------------------------------------------------------------------------

inline void
divide_rows( point4 p[4][4], point4 q[4][4], point4 r[4][4] )
{
  // Subdivide a Bezier patch into two equivalent Bezier patches:
  //   left (q) and right (r) sharing edge curves in the middle of the original
  for (int i = 0; i < 4; i++) {
    point4 mid = ( p[i][1] + p[i][2] ) / 2;

    q[i][0] = p[i][0];
    q[i][1] = ( p[i][0] + p[i][1] ) / 2;
    q[i][2] = ( q[i][1] + mid ) / 2;

    r[i][3] = p[i][3];
    r[i][2] = ( p[i][2] + p[i][3] ) / 2;
    r[i][1] = ( mid + r[i][2] ) / 2;

    q[i][3] = r[i][0] = ( q[i][2] + r[i][1] ) / 2;
    
    for ( int j = 0; j < 4; ++j ) {
	q[i][j].w = 1.0;
	r[i][j].w = 1.0;
    }
  }
}

inline void
divide_cols( point4 p[4][4], point4 q[4][4], point4 r[4][4] )
{
  // Subdivide a Bezier patch into two equivalent Bezier patches:
  //   front (q) and back (r) sharing edge curves in the middle of the original
  for (int i = 0; i < 4; i++) {
    point4 mid = ( p[1][i] + p[2][i] ) / 2;

    q[0][i] = p[0][i];
    q[1][i] = ( p[0][i] + p[1][i] ) / 2;
    q[2][i] = ( q[1][i] + mid ) / 2;

    r[3][i] = p[3][i];
    r[2][i] = ( p[2][i] + p[3][i] ) / 2;
    r[1][i] = ( mid + r[2][i] ) / 2;

    q[3][i] = r[0][i] = ( q[2][i] + r[1][i] ) / 2;
    
    for ( int j = 0; j < 4; ++j ) {
	q[i][j].w = 1.0;
	r[i][j].w = 1.0;
    }
  }
}

//----------------------------------------------------------------------------

inline void
transpose( point4 a[4][4] )
{
    for ( int i = 0; i < 3; i++ ) {
        for ( int j = i+1; j < 4; j++ ) {
	    point4 t = a[i][j];
	    a[i][j] = a[j][i];
	    a[j][i] = t;
	}
    }
}

int
divide_patch_rec( point4 p[4][4], int subdivisions, int orientation,
                  point4 points[], vec3 normals[], vec2 texCoords[], int start,
                  GLfloat texSstart, GLfloat texSend, GLfloat texTstart, GLfloat texTend )
{
  if ( subdivisions > 0 ) {
    point4 q[4][4], r[4][4], s[4][4], t[4][4];
    point4 a[4][4], b[4][4];

    // subdivide patche into right and left,
    // then into front and back
    divide_rows( p, a, b );
    divide_cols( a, q, s );
    divide_cols( b, r, t );

    GLfloat texSmid = (texSstart + texSend) / 2;
    GLfloat texTmid = (texTstart + texTend) / 2;

    // recursive division of 4 resulting patches
    start = divide_patch_rec( q, subdivisions - 1, orientation,
                              points, normals, texCoords, start,
                              texSstart, texSmid, texTstart, texTmid );
    start = divide_patch_rec( r, subdivisions - 1, orientation,
                              points, normals, texCoords, start,
                              texSstart, texSmid, texTmid, texTend );
    start = divide_patch_rec( s, subdivisions - 1, orientation,
                              points, normals, texCoords, start,
                              texSmid, texSend, texTstart, texTmid );
    start = divide_patch_rec( t, subdivisions - 1, orientation,
                              points, normals, texCoords, start,
                              texSmid, texSend, texTmid, texTend );
  } else {
    draw_patch( p, orientation, points, normals, texCoords, start,
                texSstart, texSend, texTstart, texTend );
    start += 6;
  }
  return start;
}

/**
 * Divides a Bezier patch subdivisions times and places point values into
 * points, along with normals and texture coordinates.
 *
 * @param p            the 4 X 4 patch of Bezier control points, corners interpolated
 * @param subdivisions the number of times to subdivide the patch
 * @param orientation  BACK_TO_FRONT if row 0 is at the back of the patch
 *                     FRONT_TO_BACK if row 0 is at the front of the patch
 * @param points       the array of points to put the points to draw into,
 *                     must contain at least start + 6 * numQuadsPerPatch( subdivisions )
 * @param normals      the array of vectors to put the normal vectors into,
 *                     should be NULL if not needed
 *                     must contain at least start + 6 * numQuadsPerPatch( subdivisions )
 * @param texCoords    the array of (s, t) pairs to put texture coordinates into,
 *                     should be NULL if not needed
 *                     must contain at least start + 6 * numQuadsPerPatch( subdivisions )
 * @param texSstart    the starting s-coordinate for the texture coordinates
 * @param texSend      the ending s-coordinate for the texture coordinates
 * @param texTstart    the starting t-coordinate for the texture coordinates
 * @param texTend      the ending s-coordinate for the texture coordinates
 * @return             start + 6 * numQuadsPerPatch(subdivisions) on success
 *                     -1 if orientation is not BACK_TO_FRONT or FRONT_TO_BACK,
 *                        or if points is NULL
 */
int
divide_patch( point4 p[4][4], int subdivisions, int orientation,
              point4 points[], vec3 normals[], vec2 texCoords[], int start,
              GLfloat texSstart, GLfloat texSend, GLfloat texTstart, GLfloat texTend )
{
  int numpoints = 6 * numQuadsPerPatch( subdivisions );
  int normalsNULL   = (normals   == NULL);
  int texCoordsNULL = (texCoords == NULL);
  if (points    == NULL) return -1;
  if (normalsNULL)   normals   = new vec3[numpoints];
  if (texCoordsNULL) texCoords = new vec2[numpoints];
  int result = divide_patch_rec( p, subdivisions, orientation,
                                 points, normals, texCoords, start,
                                 texSstart, texSend, texTstart, texTend );
  if (normalsNULL)   delete [] normals;
  if (texCoordsNULL) delete [] texCoords;
  return result;
}


#endif
