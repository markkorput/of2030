uniform sampler2DRect tex0;
uniform sampler2DRect iMask;
uniform vec4 iColor;
uniform float iAlpha;
varying vec2 texCoordVarying;

void main()
{
    vec4 clr = texture2DRect(tex0, texCoordVarying);
    float mask = texture2DRect(iMask, texCoordVarying).r;
    gl_FragColor = vec4(clr.rgb, clr.a*mask*iAlpha) * iColor;
}
