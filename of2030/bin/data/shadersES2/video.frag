precision highp float;

uniform sampler2D tex0;

varying vec2 texCoordVarying;

void main()
{
    vec4 texel0 = texture2D(tex0, texCoordVarying);

    float a = texel0.a;
    if(texel0.r == 0.0 && texel0.g == 0.0 && texel0.b == 0.0){
      a = 0.0;
    }
    gl_FragColor = vec4(texel0.rgb, a);
}
