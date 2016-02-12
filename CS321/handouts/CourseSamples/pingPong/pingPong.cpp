// File: pingPong.cpp

// Program to draw a ball bouncing between two walls;
// each triangle in the ball is a different randomly-generated color.
// Adapted from Angel & Shreiner 2D Sierpinski Gasket, Color Cube programs
// and recursive sphere.

#include "/usr/people/classes/CS321/include/Angel.h"
#include "holeyShapes.h"

// parameters for the walls (stretched cubes)
const int numWallPoints = 36;  // 6 faces * 2 triangles * 3 vertices/triangle
const GLfloat wallWidth = 0.125;
const GLfloat wallSX = 0.0625; // 1/16 scale factor to get 1/8 width
const GLfloat wallDX = 0.9375; // move wall +|-15/16

// parameters for creating the ball
const int divs = 3;     // number of recursive divisions
int numBallPoints = 24; // actual value computed in init

// parameters for the ball transformation matrices
const GLfloat radius = 0.25;
const GLfloat sx = radius, sy = radius, sz = radius; // scale factors
GLfloat theta = 0.0;                                 // rotation
GLfloat deltaTheta = 1.5;                            // rotation change
GLfloat deltaDX = 1.0 / 128.0;                       // change in dx
GLfloat dx = 0.0, dy = 0.0, dz = 0.0;                // translation factors
const GLfloat compressLimit = radius / 16.0;         // compression limit
GLfloat compressFactor = 1.0;                        // compression factor
int phase = 0;                                       // phase of compression cycle

// constant matrices
const mat4 scaleBall = Scale( sx, sy, sz );
const mat4 scaleWall = Scale( wallSX, 1.0, 1.0 );
const mat4 leftWall  = Translate( -wallDX, 0.0, 0.0 ) * scaleWall;
const mat4 rightWall = Translate(  wallDX, 0.0, 0.0 ) * scaleWall;

int numPoints;

GLuint model_view;  // uniform location of the model_view matrix


//----------------------------------------------------------------------------

void
init( void )
{
    // Compute number of points in the ball and the total number of points
    for (int i = 0; i < divs; i++) numBallPoints *= 4;
    numPoints = numWallPoints + numBallPoints;

    // Allocate the arrays for the points and the colors
    point4 *points = new point4[numPoints];
    color4 *colors = new color4[numPoints];

    // Set up the wall
    cube( points, 0 );
    randomColors( numWallPoints, colors, 0,             // blue-black
                  color4( 0.0, 0.0, 0.0, 1.0 ),
                  color4( 0.1, 0.1, 0.3, 1.0 ) );

    // Set up the ball
    spherichedron( divs, points, numWallPoints );
    randomColors( numBallPoints, colors, numWallPoints, // bright red
                  color4( 0.8, 0.0, 0.0, 1.0 ),
                  color4( 1.0, 0.2, 0.1, 1.0 ) );

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, numPoints * (sizeof(point4) + sizeof(color4)),
                  NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0,
                     numPoints * sizeof(point4), points );
    glBufferSubData( GL_ARRAY_BUFFER, numPoints * sizeof(point4),
                     numPoints * sizeof(color4), colors );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "pingPong_vs.glsl", "pingPong_fs.glsl" );
    glUseProgram( program );

    // Initialize the vertex position attribute from the vertex shader
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );

    GLuint vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(numPoints * sizeof(point4)) );

    model_view = glGetUniformLocation( program, "model_view" );

    glEnable( GL_DEPTH_TEST );
    glClearColor( 1.0, 0.9, 0.75, 1.0 ); // light yellow background
}

//----------------------------------------------------------------------------

void
display( void )
{
    // clear the window
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

/****** Note how both walls are drawn with the same points, ******
 ****** but with different model_view matrices.             ******/
    // draw the left wall
    glUniformMatrix4fv( model_view, 1, GL_TRUE, leftWall );
    glDrawArrays( GL_TRIANGLES, 0, numWallPoints );

    // draw the right wall
    glUniformMatrix4fv( model_view, 1, GL_TRUE, rightWall );
    glDrawArrays( GL_TRIANGLES, 0, numWallPoints );

    // draw the ball
    mat4 mv = Translate( dx, dy, dz ) *
              RotateY( theta ) *
              Scale( compressFactor, 1 / compressFactor, 1 / compressFactor ) *
              scaleBall;
    glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );
    glDrawArrays( GL_TRIANGLES, numWallPoints, numBallPoints );

    glutSwapBuffers( );
}

//----------------------------------------------------------------------------

void
idle( void )
{
    dx += deltaDX;
    GLfloat dist2wall = (1.0 - wallWidth) - (radius + fabs(dx));
    switch (phase) {
      case 0: // moving between walls, uncompressed
        if (dist2wall < 0.0) { // ball has reached wall
          compressFactor = (radius + dist2wall) / radius;
          phase = 1;
        }
        break;
      case 1: // moving into wall, compressing
        if (dist2wall >= -compressLimit) { // can compress further
          compressFactor = (radius + dist2wall) / radius;
        } else { // reached limit of compressing
          deltaDX = -deltaDX; // reverse direction
          dx += deltaDX;      // undo move past compression limit
          phase = 2;
        }
        break;
      case 2: // uncompressing, center moving away from wall, stretching
        if (dist2wall <= compressLimit) { // still touching wall
          compressFactor = (radius + dist2wall) / radius;
        } else { // released from wall
          compressFactor = (radius + 2 * compressLimit- dist2wall) / radius;
          phase = 3;
        }
        break;
      case 3: // away from wall, unstretching
        if (dist2wall < 2 * compressLimit) {
          compressFactor = (radius + 2 * compressLimit- dist2wall) / radius;
        } else {
          compressFactor = 1.0;
          phase = 0;
        }
        break;
    }

    theta -= deltaTheta;
    if (theta < 0.0) theta += 360.0;

    glutPostRedisplay( );
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
    case 033:                 // Escape key exits program
        exit( EXIT_SUCCESS );
        break;
    case ' ':                 // Space stops the ball
        glutIdleFunc    ( NULL );
        break;
    default:
        glutIdleFunc    ( idle );
        break;

    }
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );

    // If you are using freeglut, the next two lines will check if 
    // the code is truly 3.2. Otherwise, comment them out
    
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );

    glutCreateWindow( "Ball Bouncing between Two Walls" );

    glewExperimental = GL_TRUE;
    glewInit();

    init();

    glutDisplayFunc ( display  );
    glutKeyboardFunc( keyboard );
    glutIdleFunc    ( idle     );

    glutMainLoop();
    return EXIT_SUCCESS;
}
