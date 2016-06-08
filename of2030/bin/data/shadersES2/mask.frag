precision highp float;
uniform sampler2D tex0;
uniform sampler2D iMask;
uniform vec4 iColor;
uniform float iAlpha;
uniform vec2 iTexCoordMultiply;
uniform ivec2 iResolution;
varying vec2 texCoordVarying;

void main()
{
		vec2 coord = mod(texCoordVarying*iTexCoordMultiply, iResolution);
    vec4 clr = texture2D(tex0, coord);
    float mask = texture2D(iMask, coord).r;
    gl_FragColor = vec4(clr.rgb, clr.a*mask*iAlpha) * iColor;
}