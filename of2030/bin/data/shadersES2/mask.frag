precision highp float;

uniform sampler2D tex0;
uniform sampler2D iMask;

varying vec2 texCoordVarying;

void main()
{
    vec3 clr = texture2D(tex0, texCoordVarying).rgb;
    float mask = texture2D(iMask, texCoordVarying).r;
    gl_FragColor = vec4(clr, mask);
}
