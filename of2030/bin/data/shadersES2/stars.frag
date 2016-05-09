
precision highp float;

uniform float iProgress; // 0.0 means start, 1.0 means end

// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.

// The input, n, should have a magnitude in the approximate range [0, 100].
// The output is pseudo-random, in the range [0,1].
float Hash( float n )
{
  return fract( (1.0 + cos(n)) * 415.92653);
}

float Noise2d( in vec2 x )
{
    float xhash = Hash( x.x * 37.0 );
    float yhash = Hash( x.y * 57.0 );
    return fract( xhash + yhash );
}

void main(void)
{
  // Note: Choose threshold in the range [0.96, 0.9999].
  // Higher values (i.e., closer to one) yield a sparser starfield.
  // float fThreshold = 0.97;
  float threshold = 0.9999 + (0.96 - 0.9999) * iProgress;

  // Add a camera offset in "FragCoord-space".
  // vertical movement; from 250.0 to -50.0
  vec2 vCameraOffset = vec2(0.0, 0.0 - 400.0 * iProgress);

  vec2 resolution = vec2(100.0, 400.0);
  vec2 vSamplePos = ( gl_FragCoord.xy + floor( vCameraOffset ) ) / resolution.xy;

  // Sky Background Color
  vec3 vColor = vec3( 0.1, 0.2, 0.4 ) * vSamplePos.y;

  // Stars
  float StarVal = Noise2d( vSamplePos );
  if ( StarVal >= threshold )
  {
      StarVal = pow( (StarVal - threshold)/(1.0 - threshold), 6.0 );
      vColor += vec3( StarVal );
  }

  gl_FragColor = vec4(vColor, 1.0);
}
