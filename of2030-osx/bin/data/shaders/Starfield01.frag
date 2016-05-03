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

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    // Add a camera offset in "FragCoord-space".
    vec2 vCameraOffset = iMouse.xy;
    vec2 vSamplePos = ( fragCoord.xy + floor( vCameraOffset ) ) / iResolution.xy;

    vec3 vColor  = vec3(0.0, 0.0, 0.0);

	// Sky Background Color
	vColor += vec3( 0.1, 0.2, 0.4 ) * vSamplePos.y;

    // Stars
    // Note: Choose fThreshhold in the range [0.99, 0.9999].
    // Higher values (i.e., closer to one) yield a sparser starfield.
    float fThreshhold = 0.97;
    float StarVal = Noise2d( vSamplePos );
    if ( StarVal >= fThreshhold )
    {
        StarVal = pow( (StarVal - fThreshhold)/(1.0 - fThreshhold), 6.0 );
		vColor += vec3( StarVal );
    }

	fragColor = vec4(vColor, 1.0);
}
