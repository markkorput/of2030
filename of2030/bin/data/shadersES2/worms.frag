uniform vec3 iPos;
uniform float iGain;
uniform vec2 iResolution;
uniform float iScreenPanoStart;
uniform float iScreenPanoEnd;
uniform float iEffectPanoStart;
uniform float iEffectPanoEnd;

float worm(vec2 fragCoord, vec2 scroll, float tiltFactor){
    // sine-wave based baseline (causing the horizontal waviness)
    float xLine = sin(fragCoord.x*0.02+scroll.x);
    // add nother wave for some more randomness
    xLine += sin(fragCoord.x*0.01)*2.5;
    // tilt
    xLine += fragCoord.x*tiltFactor;


    // cursor used for the sine-based sinPos, offet by the above xLine for waviness
    float yCursor = (xLine+fragCoord.y*0.2);
    // multiply factor; high value means more/narrower horizontal bands
    yCursor *= 0.1;
    // vertical sine-pos; causing the vertically stacked bands
    float val = sin(yCursor+scroll.y);


    // hardness; 0.0 means all black, 1.0 means blurry edges, 10.0 means high-contrast edges
    val *= 26.0;
    // increase; higher value means more white
    val += -25.0 + iGain * 5.0;

    return val;
}

void main(void){
    // global scroll movement speed
    vec2 scroll = vec2(20, -1.0) * iProgress;
    float tilt = 0.0;

    float pixPerPano = iResolution.x / (iScreenPanoEnd - iScreenPanoStart);
    float startX = (iEffectPanoStart - iScreenPanoStart) * pixPerPano;
    float endX = (iEffectPanoEnd - iScreenPanoStart) * pixPerPano;

    float c = 0.0;

    if(gl_FragCoord.x >= startX && gl_FragCoord.x <= endX){
      c = clamp(worm(gl_FragCoord.xy, scroll, tilt), 0.0, 1.0);
      c += clamp(worm(gl_FragCoord.xy, scroll+vec2(5.0, 3.0), tilt+0.01), 0.0, 1.0);
      c += clamp(worm(gl_FragCoord.xy, scroll+vec2(50.0, 2.0), tilt+0.003), 0.0, 1.0);
    }
    gl_FragColor = vec4(vec3(c), 1.0);
}
