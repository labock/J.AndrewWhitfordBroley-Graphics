function skewedCubeX(points) {
  const numFaces     = 6;
  const vertsPerFace = 6;

  var vertices = [
    vec4( -1.2, -1.0,  1.0,  1.0 ), // 0
    vec4( -8.0,  1.0,  1.0,  1.0 ), // 1
    vec4(  1.2,  1.0,  1.0,  1.0 ), // 2
    vec4(  0.8, -1.0,  1.0,  1.0 ), // 3
    vec4( -1.0, -1.0, -1.0,  1.0 ), // 4
    vec4( -1.0,  1.0, -1.0,  1.0 ), // 5
    vec4(  1.0,  1.0, -1.0,  1.0 ), // 6
    vec4(  1.0, -1.0, -1.0,  1.0 )  // 7
  ];

  var faceIndices = [
    [ 1, 0, 3, 2 ], // 0  front face
    [ 2, 3, 7, 6 ], // 1  right face
    [ 3, 0, 4, 7 ], // 2  bottom face
    [ 6, 5, 1, 2 ], // 3  top face
    [ 4, 5, 6, 7 ], // 4  back face
    [ 5, 4, 0, 1 ]  // 5  left face
  ];

  for ( i = 0; i < numFaces; i++ ) {
    points.push( vertices[faceIndices[i][0]] );
    points.push( vertices[faceIndices[i][1]] );
    points.push( vertices[faceIndices[i][2]] );
    points.push( vertices[faceIndices[i][0]] );
    points.push( vertices[faceIndices[i][2]] );
    points.push( vertices[faceIndices[i][3]] );
  }
  return vertsPerFace * numFaces;
}