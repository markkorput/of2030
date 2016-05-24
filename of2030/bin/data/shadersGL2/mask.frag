uniform sampler2DRect iMask;

// varying vec2 texCoordVarying;

void main()
{
    //vec4 texel0 = texture2DRect(tex0, texCoordVarying);
    vec4 texel1 = texture2DRect(iMask, gl_FragCoord.xy);
    gl_FragColor = vec4(vec3(1.0), texel1.r);
}
