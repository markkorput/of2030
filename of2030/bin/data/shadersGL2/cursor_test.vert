// uniform mat4 modelViewProjectionMatrix;
// in vec4 position;

//uniform mat4 iScreenCamMatrix;
//uniform vec2 iScreenWorldSize;
uniform vec3 iPos;
uniform vec2 iResolution;
// uniform float iProgress;
// uniform float iDuration;
// uniform float iIterations;
// uniform float iLocalPanoStart;
// uniform float iLocalPanoEnd;
uniform float iGain;

void main(void)
{
  // iGain gives the fatness of the cursor bar in (world) meters
  // scale vertex positions horizontally
  //vec4 vPos = gl_Vertex * vec4(1.0/iResolution.x*iScreenWorldSize.x*iGain, 5000.0, 0.0, 1.0);

  // move cursor to its current 3D position
  //vPos = vPos + vec4(iPos, 0.0);

  // gl_Position = gl_ModelViewProjectionMatrix * iScreenCamMatrix * vPos;
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
