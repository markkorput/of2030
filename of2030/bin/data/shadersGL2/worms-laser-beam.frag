uniform vec3 iPos;
uniform float iGain;
uniform vec2 iResolution;
uniform float iScreenPanoStart;
uniform float iScreenPanoEnd;
uniform float iEffectPanoStart;
uniform float iEffectPanoEnd;

float Hash( float n )
{
  return fract( (1.0 + cos(n)) * 415.92653);
}

float Noise2d( in vec2 x )
{
    float xhash = Hash( x.x * 37.0 );
    float yhash = Hash( x.y * 57.0 );
    return fract( xhash + yhash );
}


float worm(vec2 fragCoord, vec2 scroll, float tiltFactor){
    // sine-wave based baseline (causing the horizontal waviness)
    float xLine = Noise2d(fragCoord*0.02+scroll); //sin(fragCoord.x*0.02+scroll.x);
    // add nother wave for some more randomness
    xLine += Noise2d((fragCoord+scroll) * 0.01) * 5.5; // sin(fragCoord.x*0.01)*5.5;
    xLine += Noise2d((fragCoord+scroll) * 0.028); //sin(fragCoord.x*0.028) * 7.0;
    // tilt
    xLine += fragCoord.x*tiltFactor;


    // cursor used for the sine-based sinPos, offet by the above xLine for waviness
    float yCursor = (xLine+fragCoord.y*0.1);
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
    vec2 scroll = iPos.xy;
    float tilt = 0.0;

    float pixPerPano = iResolution.x / (iScreenPanoEnd - iScreenPanoStart);
    float startX = (iEffectPanoStart - iScreenPanoStart) * pixPerPano;
    float endX = (iEffectPanoEnd - iScreenPanoStart) * pixPerPano;

    float c = 0.0;

    if(gl_FragCoord.x >= startX && gl_FragCoord.x <= endX){
      // worm 1
      c += clamp(worm(gl_FragCoord.xy, scroll, tilt), 0.0, 1.0);
      // worm 2
      //c += clamp(worm(gl_FragCoord.xy, scroll+vec2(5.0, 3.0), tilt+0.01), 0.0, 1.0);
      // worm 3
      //c += clamp(worm(gl_FragCoord.xy, scroll+vec2(50.0, 2.0), tilt+0.003), 0.0, 1.0);
    }

    gl_FragColor = vec4(vec3(1.0), c);
}
