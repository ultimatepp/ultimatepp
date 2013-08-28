uniform sampler2D Texture;
uniform vec4 GlyphColor;
uniform vec4 GlowColor;
uniform vec4 OutlineColor;

uniform bool Outline;
uniform bool Glow;
uniform bool Shadow;

const float AlphaCenter = 0.5;
const float DistanceScale = 25.0;

uniform float OutlineCenter;
uniform float GlowCenter;

void main()
{
	vec4 color = texture2D(Texture, gl_TexCoord[0].xy);
	float alpha = color.a;
	float width = min(length(fwidth(gl_TexCoord[0])), 1.0) * DistanceScale;

	vec4 finalColor = GlyphColor;

	float ma = smoothstep(AlphaCenter - width, AlphaCenter + width, alpha);
	finalColor.a = ma;
	
	if(Outline)
	{
		float mo = smoothstep(OutlineCenter - width, OutlineCenter + width, alpha);
		finalColor = mix(OutlineColor, finalColor, ma);
		finalColor.a = mo;
	}
	
	if(Glow && alpha >= finalColor.a - 0.5)
	{
		float mg = smoothstep(AlphaCenter, GlowCenter, sqrt(alpha));
		finalColor = mix(GlowColor, finalColor, ma);
		finalColor.a = mg;
	}
	
	gl_FragColor = finalColor * GlyphColor.a;
}
