precision highp float;

uniform sampler2DRect tex0;
uniform sampler2DRect iMask;
uniform vec4 iColor;

varying vec2 texCoordVarying;

void main()
{
    vec4 texel0 = texture2DRect(tex0, texCoordVarying);
    vec4 texel1 = texture2DRect(iMask, texCoordVarying); //gl_FragCoord.xy);
    gl_FragColor = vec4(texel0.rgb, texel0.a * texel1.a);
}
