uniform float sigma;     // The sigma value for the gaussian function: higher value means more blur
                         // A good value for 9x9 is around 3 to 5
                         // A good value for 7x7 is around 2.5 to 4
                         // A good value for 5x5 is around 2 to 3.5
                         // ... play around with this based on what you need :)

uniform float blurSize;  // This should usually be equal to
                         // 1.0f / texture_pixel_width for a horizontal blur, and
                         // 1.0f / texture_pixel_height for a vertical blur.

uniform sampler2D blurSampler;  // Texture that will be blurred by this shader

const float numBlurPixelsPerSide = 3.0f;
// 9x9 - 4
// 7x7 - 3
// 5x5 - 2
uniform vec2 blurMultiplyVec;
// 1, 0 - vertical
// 0, 1 - horizontal
uniform vec3 gaussian;
uniform float grayStrength;
uniform float blurStrength;

void main() {

	vec4 v = texture2D(blurSampler, gl_TexCoord[0].xy);
	if(blurStrength > 0.0f)
	{
		vec4 avgValue = vec4(0.0f, 0.0f, 0.0f, 0.0f);
		float coefficientSum = 0.0f;
		
		vec3 incGaussian = gaussian;
		avgValue += v * incGaussian.x;
		coefficientSum += incGaussian.x;
		incGaussian.xy *= incGaussian.yz;
		
		for (float i = 1.0f; i <= numBlurPixelsPerSide; i++) {
			vec2 pos = i * blurSize * blurMultiplyVec;
			avgValue += texture2D(blurSampler, gl_TexCoord[0].xy - pos) * incGaussian.x;         
			avgValue += texture2D(blurSampler, gl_TexCoord[0].xy + pos) * incGaussian.x;         
			coefficientSum += 2.0f * incGaussian.x;
			incGaussian.xy *= incGaussian.yz;
		}
			
		v = avgValue / coefficientSum;
	}
	float gray = dot(v.rgb, vec3(0.299, 0.587, 0.114));
	vec4 grayColor = vec4(gray, gray, gray, v.a);
	
	gl_FragColor = mix(v, grayColor, grayStrength);
}