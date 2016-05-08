// uniform mat4 modelViewProjectionMatrix;
// in vec4 position;

uniform vec3 iPos;
uniform vec2 iResolution;
// uniform float iProgress;
// uniform float iDuration;
// uniform float iIterations;
// uniform float iLocalPanoStart;
// uniform float iLocalPanoEnd;
// uniform float iGain;

void main(void)
{
  vec4 translation = vec4(iPos*vec3(iResolution, 1.0), 0.0);
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex + translation;
}
