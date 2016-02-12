// File: movingOvoid.cpp

// Program to draw an ovoid globerevolving around the line z = x;
// each triangle in the ovoid is a different randomly-generated color.
// Adapted from Angel & Shreiner 2D Sierpinski Gasket, Color Cube programs
// and recursive sphere.

#include "/usr/people/classes/CS321/include/Angel.h"
#include "/usr/people/classes/CS321/include/holeyShapes.h"

// window parameters
const int defaultWindowSize = 512;

// parameters for creating the globe
const int latDivs  = 18;
const int longDivs = 36;
const int numGlobePoints = 6 * longDivs * (latDivs - 1);

// parameters for the globe transformation matrices
const GLfloat sx = 0.4, sy = 0.2, sz = 0.2; // scale factors
const GLfloat dx = 0.5, dy = 0.0, dz = 0.0; // translation factors

const int xRotateDivs = 180; // number of positions around the revolution
int       xRotatePos  =   0; // current position around the revolution
                             // (0 ... rotateDivs - 1)

const int revolveDivs = 540; // number of positions around the revolution
int       revolvePos  =   0; // current position around the revolution
                             // (0 ... revolveDivs - 1)

const GLfloat obliqueAngle = -45.0;  // degrees

// constant globe matrices
const mat4 zRotateScaleAndTranslate =
               Translate( dx, dy, dz ) * Scale( sx, sy, sz ) * RotateZ( 90.0 );
const mat4 obliqueRotate     = RotateY( obliqueAngle );

// parameters for the pyramids (quad based)
const int pyrBaseVerts = 4;
const int pyrStart     = numGlobePoints;
const int numPyrPoints = 6 * pyrBaseVerts;
const GLfloat psx = 0.4, psy =  0.5, psz = 0.4; // scale factors
const GLfloat pdx = 0.7, pdy = -0.8, pdz = 0.7; // translation factors
const mat4 pyrScale = Scale( psx, psy, psz );

// parameters for viewer position
const GLfloat initViewerDist  =  4.0;
const GLfloat minViewerDist   =  2.0;
const GLfloat maxViewerDist   = 10.0;
const GLfloat maxOffsetRatio  =  1.0;
const GLfloat deltaViewerDist =  0.25;
const GLfloat deltaOffset     =  0.1;
point4 eye( 0.0, 0.0, initViewerDist, 1.0 );
const point4 at ( 0.0, 0.0, 0.0, 1.0 );
const vec4   up ( 0.0, 1.0, 0.0, 0.0 );

int numPoints = numGlobePoints + numPyrPoints;

GLuint model_view;  // uniform location of the model_view matrix

// Projection transformation parameters
const GLfloat dimScale = 0.1;
GLfloat left   = -dimScale, right =  dimScale,
        bottom = -dimScale, top   =  dimScale,
        zNear  =  0.4, zFar  = 20.0;

GLuint  projection;  // uniform location of the projection matrix


//----------------------------------------------------------------------------

void
init( void )
{
    // Create the point and color arrays
    point4 *points = new point4[numPoints];
    color4 *colors = new color4[numPoints];

    // Set up the ovoid globe
    globe( longDivs, latDivs, points, 0 );
    globeColors( longDivs, latDivs, colors, 0 );

    // Set up the pyramid
    pyramid( pyrBaseVerts, points, pyrStart );
    randomColors( numPyrPoints, colors, pyrStart );

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
    glBufferSubData( GL_ARRAY_BUFFER, 0, numPoints * sizeof(point4), points );
    glBufferSubData( GL_ARRAY_BUFFER, numPoints * sizeof(point4),
                     numPoints * sizeof(color4), colors );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "movingGlobe_vs.glsl", "movingGlobe_fs.glsl" );
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
    projection = glGetUniformLocation( program, "projection" );

    glEnable( GL_DEPTH_TEST ); 
    glClearColor( 1.0, 0.9, 0.75, 1.0 ); // light yellow background
}

//----------------------------------------------------------------------------

void
display( void )
{
    // clear the window
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // set up projection matrix
    mat4 p = Frustum( left, right, bottom, top, zNear, zFar );
    glUniformMatrix4fv( projection, 1, GL_TRUE, p );

    // set up view position
    mat4 lookAt = LookAt( eye, at, up );

  // Set up globe
    // set up the rotation matrix
    GLfloat xRotationAngle = xRotatePos * 360.0 / xRotateDivs;
    mat4 xRotation = RotateX( xRotationAngle );

    // set up the revolution matrix
    GLfloat revolveAngle = revolvePos * 360.0 / revolveDivs;
    mat4 revolutionRotate = RotateZ( revolveAngle );

    // set up the model_view matrix
    mat4 mv = lookAt * obliqueRotate * revolutionRotate *
              xRotation * zRotateScaleAndTranslate;

    glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );
    glDrawArrays( GL_TRIANGLES, 0, numGlobePoints );

  // Set up pyramids
    mat4 pyrTranslate = Translate( pdx, pdy, pdz );  // right front pyramid
    mv = lookAt * pyrTranslate * pyrScale * RotateY(  45.0 );
    glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );
    glDrawArrays( GL_TRIANGLES, pyrStart, numPyrPoints );

    pyrTranslate = Translate( pdx, pdy, -pdz );      // right rear pyramid
    mv = lookAt * pyrTranslate * pyrScale * RotateY( 135.0 );
    glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );
    glDrawArrays( GL_TRIANGLES, pyrStart, numPyrPoints );

    pyrTranslate = Translate( -pdx, pdy, pdz );      // left front pyramid
    mv = lookAt * pyrTranslate * pyrScale * RotateY( 225.0 );
    glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );
    glDrawArrays( GL_TRIANGLES, pyrStart, numPyrPoints );

    pyrTranslate = Translate( -pdx, pdy, -pdz );     // left rear pyramid
    mv = lookAt * pyrTranslate * pyrScale * RotateY( 315.0 );
    glUniformMatrix4fv( model_view, 1, GL_TRUE, mv );
    glDrawArrays( GL_TRIANGLES, pyrStart, numPyrPoints );

    glutSwapBuffers( );
}

//----------------------------------------------------------------------------

void
idle( void )
{
    xRotatePos = (xRotatePos + 1) % xRotateDivs;
    revolvePos = (revolvePos + 1) % revolveDivs;

    glutPostRedisplay( );
}

//----------------------------------------------------------------------------

void
specialKey( int key, int x, int y )
{
    switch ( key ) {
        GLfloat offsetRatio;
    case GLUT_KEY_PAGE_UP:    // Moves viewer in
        if (eye.z > minViewerDist) {
          eye.z -= deltaViewerDist;
          offsetRatio = eye.z / (eye.z + deltaViewerDist);
          eye.x *= offsetRatio;
          eye.y *= offsetRatio;
        }
        break;
    case GLUT_KEY_PAGE_DOWN:  // Moves viewer out
        if (eye.z < maxViewerDist) {
          eye.z += deltaViewerDist;
          offsetRatio = eye.z / (eye.z - deltaViewerDist);
          eye.x *= offsetRatio;
          eye.y *= offsetRatio;
        }
        break;
    case GLUT_KEY_LEFT:       // Moves viewer left
        offsetRatio = eye.x / eye.z;
        if (offsetRatio > -maxOffsetRatio) {
          offsetRatio -= deltaOffset;
          eye.x = eye.z * offsetRatio;
        }
        break;
    case GLUT_KEY_RIGHT:      // Moves viewer right
        offsetRatio = eye.x / eye.z;
        if (offsetRatio < maxOffsetRatio) {
          offsetRatio += deltaOffset;
          eye.x = eye.z * offsetRatio;
        }
        break;
    case GLUT_KEY_UP:         // Moves viewer up
        offsetRatio = eye.y / eye.z;
        if (offsetRatio < maxOffsetRatio) {
          offsetRatio += deltaOffset;
          eye.y = eye.z * offsetRatio;
        }
        break;
    case GLUT_KEY_DOWN:       // Moves viewer down
        offsetRatio = eye.y / eye.z;
        if (offsetRatio > -maxOffsetRatio) {
          offsetRatio -= deltaOffset;
          eye.y = eye.z * offsetRatio;
        }
        break;

    }

    glutPostRedisplay( );
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    GLfloat offsetRatio;
    switch ( key ) {
    case 033:                 // Escape key exits program
        exit( EXIT_SUCCESS );
        break;
    case 'a': case 'A':       // Moves viewer left
        offsetRatio = eye.x / eye.z;
        if (offsetRatio > -maxOffsetRatio) {
          offsetRatio -= deltaOffset;
          eye.x = eye.z * offsetRatio;
        }
        break;
    case 'd': case 'D':       // Moves viewer right
        offsetRatio = eye.x / eye.z;
        if (offsetRatio < maxOffsetRatio) {
          offsetRatio += deltaOffset;
          eye.x = eye.z * offsetRatio;
        }
        break;
    case 'w': case 'W':       // Moves viewer up
        offsetRatio = eye.y / eye.z;
        if (offsetRatio < maxOffsetRatio) {
          offsetRatio += deltaOffset;
          eye.y = eye.z * offsetRatio;
        }
        break;
    case 's': case 'S':       // Moves viewer down
        offsetRatio = eye.y / eye.z;
        if (offsetRatio > -maxOffsetRatio) {
          offsetRatio -= deltaOffset;
          eye.y = eye.z * offsetRatio;
        }
        break;
    case ' ':                 // Space stops the ball
        glutIdleFunc    ( NULL );
        break;
    default:                  // Any key not specified above restarts animation
        glutIdleFunc    ( idle );
        break;

    }

    glutPostRedisplay( );
}

//----------------------------------------------------------------------------

void
reshape( int width, int height )
{
    GLfloat aspect = ((GLfloat) width) / height;
    if ( aspect >= 1.0 ) {
      right = dimScale * aspect;
      top   = dimScale;
    } else {
      right = dimScale;
      top   = dimScale / aspect;
    }
    left   = -right;
    bottom = - top;
    glViewport( 0, 0, width, height );

}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( defaultWindowSize, defaultWindowSize );

    // If you are using freeglut, the next two lines will check if 
    // the code is truly 3.2. Otherwise, comment them out
    
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );

    glutCreateWindow( "Moving Globe" );

    glewExperimental = GL_TRUE;
    glewInit();

    init();

    glutDisplayFunc ( display    );
    glutKeyboardFunc( keyboard   );
    glutSpecialFunc ( specialKey );
    glutIdleFunc    ( idle       );
    glutReshapeFunc ( reshape    );

    glutMainLoop();
    return EXIT_SUCCESS;
}
