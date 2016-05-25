uniform vec3 iPos;
uniform vec2 iResolution;
uniform float iProgress;
uniform float iDuration;
uniform float iIterations;
uniform float iScreenPanoStart;
uniform float iScreenPanoEnd;
uniform float iGain;

void main(void){
    float iterationDuration = iDuration / iIterations;
    // effectTime
    float f = (iProgress*iDuration);
    // iterationTime
    f = f - floor(f / iterationDuration) * iterationDuration;
    // iterationProgress
    f = f / iterationDuration;
    // localProgress
    f = (f - iScreenPanoStart) / (iScreenPanoEnd - iScreenPanoStart);

    // float pixPerPano = iResolution.x / (iScreenPanoEnd - iScreenPanoStart);

    float alpha = 0.0;
    if(abs(gl_FragCoord.x - f*iResolution.x) < iGain){
      alpha = 1.0;
    }

    gl_FragColor = vec4(vec3(1.0), alpha);
}
