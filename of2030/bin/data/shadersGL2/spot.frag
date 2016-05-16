uniform vec3 iPos;
uniform vec2 iResolution;
// uniform float iProgress;
// uniform float iDuration;
// uniform float iIterations;
// uniform vec2 iScreenWorldSize;
uniform float iScreenPanoStart;
uniform float iScreenPanoEnd;
// uniform float iEffectPanoStart;
// uniform float iEffectPanoEnd;
uniform float iGain;

// varying vec2 texCoordVarying;

void main(void){
  float pixPerPano = iResolution.x / (iScreenPanoEnd - iScreenPanoStart);

  vec2 pixPos = (iPos.xy - vec2(iScreenPanoStart, 0.0)) * pixPerPano;
  float gainDist = min(iResolution.x, iResolution.y) * 0.5 * iGain;
  float curPixDist = length(gl_FragCoord.xy - pixPos);
  float alpha = 1.0 - curPixDist / gainDist;

  gl_FragColor = vec4(vec3(1.0), alpha);
}
