precision highp float;
uniform sampler2D tex0;
uniform sampler2D iMask;

varying vec2 texCoordVarying;

void main()
{
    vec4 clr = texture2D(tex0, texCoordVarying);
    float mask = texture2D(iMask, texCoordVarying).r;
    gl_FragColor = vec4(clr.rgb, clr.a*mask);
}
