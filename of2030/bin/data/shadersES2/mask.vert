attribute vec4 position;
uniform mat4 modelViewProjectionMatrix;

attribute vec2 texcoord;
varying vec2 texCoordVarying;

void main()
{
  texCoordVarying = texcoord;
  gl_Position = modelViewProjectionMatrix * position;
}
