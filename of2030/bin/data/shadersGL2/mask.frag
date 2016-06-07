uniform sampler2DRect tex0;
uniform sampler2DRect iMask;
uniform vec4 iColor;
uniform float iAlpha;
//uniform vec2 iTexCoordMultiply;
//uniform vec2 iResolution;
varying vec2 texCoordVarying;

void main()
{
		// vec2 coord = mod(texCoordVarying*iTexCoordMultiply, iResolution);
    vec4 clr = texture2DRect(tex0, texCoordVarying);
    float mask = texture2DRect(iMask, texCoordVarying).r;
    gl_FragColor = vec4(clr.rgb, clr.a*mask*iAlpha) * iColor;
}
