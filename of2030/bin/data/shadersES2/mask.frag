precision highp float;
uniform sampler2D tex0;
uniform sampler2D iMask;
uniform vec4 iColor;
//uniform ivec2 iResolution;
//varying vec2 texCoordVarying;

void main()
{
		//vec2 coord = mod(texCoordVarying*iTexCoordMultiply, iResolution);
    vec4 clr = texture2D(tex0, texCoordVarying);
    float mask = texture2D(iMask, texCoordVarying).r;
    gl_FragColor = vec4(clr.rgb, clr.a*mask) * iColor;
}
