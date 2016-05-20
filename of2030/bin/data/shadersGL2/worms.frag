uniform vec3 iPos;
uniform vec3 iSize;
uniform float iGain;
uniform vec2 iResolution;
uniform float iScreenPanoStart;
uniform float iScreenPanoEnd;
uniform float iEffectPanoStart;
uniform float iEffectPanoEnd;
uniform vec2 iScreenWorldSize;

float worm(vec2 pos){ //}, float tiltFactor){
    // sine-wave based baseline (causing the horizontal waviness)
    float xLine = sin(pos.x*0.1)*0.1;
    // add nother wave for some more randomness
    xLine += sin(pos.x+0.13)*0.21;
    xLine += sin(pos.x+1.521)*0.31;
    // tilt
    //xLine += pos.x*tiltFactor;

    // cursor used for the sine-based sinPos, offet by the above xLine for waviness
    float yCursor = (xLine+pos.y);
    // multiply factor; high value means more/narrower horizontal bands
    // yCursor *= 0.1;
    // vertical sine-pos; causing the vertically stacked bands
    float val = sin(yCursor); //+pos.y);

    float factor = 100.0;
    // hardness; 0.0 means all black, 1.0 means blurry edges, 10.0 means high-contrast edges
    val *= factor; // TODO make a uniform param for this
    // increase; higher value means more white
    val += iGain - factor;

    return val;
}

void main(void){
    // global scroll movement speed
    //float tilt = 0.0;

    float pixPerPano = iResolution.x / (iScreenPanoEnd - iScreenPanoStart);
    float startX = (iEffectPanoStart - iScreenPanoStart) * pixPerPano;
    float endX = (iEffectPanoEnd - iScreenPanoStart) * pixPerPano;

    vec2 fragWorldPos = gl_FragCoord.xy / iResolution * iScreenWorldSize;

    float c = 0.0;

    if(gl_FragCoord.x >= startX && gl_FragCoord.x <= endX){
      // worm 1
      c += clamp(worm(iPos.xy + fragWorldPos), 0.0, 1.0);
      // worm 2
      c += clamp(worm(iPos.xy + fragWorldPos + iSize.xy), 0.0, 1.0);
      // worm 3
      c += clamp(worm(iPos.xy + fragWorldPos - iSize.xy), 0.0, 1.0);
    }

    gl_FragColor = vec4(vec3(1.0), c);
}
