uniform sampler2DRect tex0;
uniform sampler2DRect iMask;
uniform vec4 iColor;
uniform float iAlpha;
uniform vec2 iTexCoordMultiply;
uniform vec2 iTexCoordOffset;
uniform vec2 iResolution;
varying vec2 texCoordVarying;

void main()
{
		vec2 coord = mod(iTexCoordOffset*iResolution+texCoordVarying*iTexCoordMultiply, iResolution);
    vec4 clr = texture2DRect(tex0, coord);
    float mask = texture2DRect(iMask, coord).r;
    gl_FragColor = vec4(clr.rgb, clr.a*mask*iAlpha) * iColor;
}
