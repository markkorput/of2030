uniform vec2 iResolution;
uniform float iProgress;
uniform float iDuration;
uniform float iIterations;
uniform float iLocalPanoStart;
uniform float iLocalPanoEnd;
uniform float iGain;

void main(void){
    float iterationDuration = iDuration / iIterations;
    // effectTime
    float f = (iProgress*iDuration);
    // iterationTime
    f = f - floor(f / iterationDuration) * iterationDuration;
    // iterationProgress
    // f = f / iterationDuration;
    f = (f - iLocalPanoStart) / (iLocalPanoEnd - iLocalPanoStart);

    //float cursorX = localProgress * iResolution.x;
    //float pixelDistance = abs(cursorX - gl_FragCoord.x);
    //float val = (300.0 - pixelDistance)/300.0;
    gl_FragColor = vec4(vec3(f), 1.0);
}
