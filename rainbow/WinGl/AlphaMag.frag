uniform sampler2D textMap;
uniform vec4 textColor;

float alphaThreshold = 0.5;
float distanceScale = 25.0;

float outlineOuter = 0.1;
float outlineInner = 0.5;
vec4 outlineColor = vec4(1, 0, 0, 1);

float glowOffsetX = 0.005;
float glowOffsetY = 0.005;
float glowStart = 0.3;
float glowEnd = 1.0;
vec4 glowColor = vec4(1.0, 1.0, 1.0, 1.0);

void main()
{
	vec4 color = texture2D(textMap, gl_TexCoord[0].xy);
	float distanceFactor = color.a;
	
	float width = fwidth(gl_TexCoord[0].xy) * distanceScale;
	
	float outlineMin1 = outlineOuter + width * 2.0;
	float outlineMax1 = outlineInner + width * 2.0;
	
	if (distanceFactor > outlineOuter && distanceFactor < outlineMax1)
	{
		float outlineAlpha;
		
		if (distanceFactor <= outlineMin1)
			outlineAlpha = smoothstep(outlineOuter, outlineMin1, distanceFactor);
		else
			outlineAlpha = smoothstep(outlineMax1, outlineInner, distanceFactor);
			
		color = mix(color, outlineColor, outlineAlpha);
	}
	
	color.a = smoothstep(alphaThreshold - width, alphaThreshold + width, distanceFactor);
	
	vec2 glowOffset = vec2(-glowOffsetX, -glowOffsetY);
	float glowDistance = texture2D(textMap, gl_TexCoord[0].xy + glowOffset).a;
	float glowFactor = smoothstep(glowStart, glowEnd, glowDistance);
	
	color = mix(vec4(glowColor.rgb, glowFactor), color, color.a);
	
	gl_FragColor = color;
}

