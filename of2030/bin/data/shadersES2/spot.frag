precision highp float;

uniform vec2 iSpotPos;
uniform vec2 iSpotSize;
uniform float iGain;
uniform int iQuarterOn;
uniform int iQuarterOff;

void main(void){
  float intensityX = 1.0 - clamp(abs(gl_FragCoord.x - iSpotPos.x) / iSpotSize.x, 0.0, 1.0);
  float intensityY = 1.0 - clamp(abs(gl_FragCoord.y - iSpotPos.y) / iSpotSize.y, 0.0, 1.0);
  float intensity = intensityX*intensityY*iGain;

  // quarters on
  if(iQuarterOn == 1 && (gl_FragCoord.x < iSpotPos.x || gl_FragCoord.y > iSpotPos.y)){
    intensity = 0.0;
  }

  if(iQuarterOn == 2 && (gl_FragCoord.x < iSpotPos.x || gl_FragCoord.y < iSpotPos.y)){
    intensity = 0.0;
  }

  if(iQuarterOn == 3 && (gl_FragCoord.x > iSpotPos.x || gl_FragCoord.y < iSpotPos.y)){
    intensity = 0.0;
  }

  if(iQuarterOn == 4 && (gl_FragCoord.x > iSpotPos.x || gl_FragCoord.y > iSpotPos.y)){
    intensity = 0.0;
  }

  // quarters off
  if(iQuarterOff == 1 && gl_FragCoord.x > iSpotPos.x && gl_FragCoord.y < iSpotPos.y){
    intensity = 0.0;
  }

  if(iQuarterOff == 2 && gl_FragCoord.x > iSpotPos.x && gl_FragCoord.y > iSpotPos.y){
    intensity = 0.0;
  }

  if(iQuarterOff == 3 && gl_FragCoord.x < iSpotPos.x && gl_FragCoord.y > iSpotPos.y){
    intensity = 0.0;
  }

  if(iQuarterOff == 4 && gl_FragCoord.x < iSpotPos.x && gl_FragCoord.y < iSpotPos.y){
    intensity = 0.0;
  }

  gl_FragColor = vec4(vec3(1.0), intensity);
}
