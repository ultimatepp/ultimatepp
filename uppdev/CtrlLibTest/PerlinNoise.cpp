#include "CtrlLibTest.h"

void PerlinNoiseCache::Cache(int cx, int cy)
{
	ASSERT(cx >= 1 && cy >= 1);
	Clear();
	sz = Size(cx, cy);
	InitNoise();
	int xycnt = cx *cy;
	cache.Alloc(xycnt);	
	float *q = ~cache;
	for (int y = 0; y < sz.cy; y++)
		for (int x = 0; x < sz.cx; x++, q++)
			*q = PerlinNoise2D(x, y);
	DeinitNoise();		
}

Image PerlinNoiseCache::AsImage() const
{
	ASSERT(~cache);
	ImageBuffer ib(sz.cx, sz.cy);
	RGBA *q = ~ib;
	RGBA *eoi = q + ib.GetLength();
	const float *v = ~cache;
	while (q != eoi) {
		q->r = q->g = q->b = 128 + int(128 * (*v));
		q->a = 255;	
		q++;
		v++;
	}
	return ib;
}

float PerlinNoiseCache::Get(int x, int y, int z) const
{
	int offset = z * 151;
	return Get(x+offset, y+offset);
}

float PerlinNoiseCache::Get(float x, float y, int z) const
{
	int integer_X   	 	= int(x);
	float fractional_X 		= x - integer_X;

	int integer_Y    		= int(y);
	float fractional_Y 		= y - integer_Y;	
	
	float v1 = Get(integer_X,     integer_Y, z);
	float v2 = Get(integer_X + 1, integer_Y, z);
	float v3 = Get(integer_X,     integer_Y + 1, z);
	float v4 = Get(integer_X + 1, integer_Y + 1, z);
	
	float i1 = CosineInterpolate(v1, v2, fractional_X);
	float i2 = CosineInterpolate(v3, v4, fractional_X);

	return CosineInterpolate(i1, i2, fractional_Y);
}

void PerlinNoiseCache::InitNoise()
{
	int xycnt = sz.cx * sz.cy;
	noise.Alloc(xycnt);
	float *q = ~noise;
	for (int y = 0; y < sz.cy; y++)
		for (int x = 0; x < sz.cx; x++, q++)
			*q = ((rand() % 10000) - 5000) / 10000.0f;
			//*q = PerlinNoise::Noise(x, y);	
}

inline int Abs(int v) { return (v < 0) ? -v : v; }

float PerlinNoiseCache::Noise(int x, int y) const
{
	ASSERT(~noise);
	return noise[Abs((x % sz.cx) + sz.cx * (y % sz.cy))];
}

void PerlinNoiseCache::Clear()
{
	cache.Clear();
	noise.Clear();
	sz = Null;
}

/*
** Original author: Hugo Elias
** http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
** Modified and corrected by James Thomas
*/
float PerlinNoise::Noise(int x, int y) const
{
	int n = x + y * 22283;
	n = (n<<13) ^ n;
	return ( 1.0f - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);    
}

float PerlinNoise::SmoothNoise1(int x, int y) const 
{
	float corners = ( Noise(x-1, y-1)+Noise(x+1, y-1)+Noise(x-1, y+1)+Noise(x+1, y+1) ) / 16;
	float sides   = ( Noise(x-1, y)  +Noise(x+1, y)  +Noise(x, y-1)  +Noise(x, y+1) ) /  8;
	float center  =  Noise(x, y) / 4;
	return corners + sides + center;
}

float PerlinNoise::CosineInterpolate(float a, float b, float x) const
{
	float ft = x * 3.1415927f;
	float f = (1 - (float)cos(ft)) * 0.5f;
	return  a*(1-f) + b*f;
}

float PerlinNoise::InterpolatedNoise1(float x, float y) const
{
	int integer_X   	 	= int(x);
	float fractional_X 		= x - integer_X;

	int integer_Y    		= int(y);
	float fractional_Y 		= y - integer_Y;

	float v1 = SmoothNoise1(integer_X,     integer_Y);
	float v2 = SmoothNoise1(integer_X + 1, integer_Y);
	float v3 = SmoothNoise1(integer_X,     integer_Y + 1);
	float v4 = SmoothNoise1(integer_X + 1, integer_Y + 1);

	float i1 = CosineInterpolate(v1, v2, fractional_X);
	float i2 = CosineInterpolate(v3, v4, fractional_X);

	return CosineInterpolate(i1, i2, fractional_Y);
}

float PerlinNoise::PerlinNoise2D(int x, int y) const
{
	float total = 0;
	float amplitude = 1.0f;
	float max = 0.0f;
	int frequency = 1;
	float fx = float(x);
	float fy = float(y);
	for (int i = 0; i < octaves; i++) {
		total = total + InterpolatedNoise1(fx / frequency, fy / frequency) * amplitude;
		max += amplitude;
		amplitude *= persistence;
		frequency <<= 1;
	}
	return total / max;
}

float PerlinNoise::PerlinNoise2D(float x, float y) const
{
	float total = 0;
	float amplitude = 1.0f;
	float max = 0.0f;
	int frequency = 1;
	float fx = float(x);
	float fy = float(y);
	for (int i = 0; i < octaves; i++) {
		total = total + InterpolatedNoise1(fx / frequency, fy / frequency) * amplitude;
		max += amplitude;
		amplitude *= persistence;
		frequency <<= 1;
	}
	return total / max;
}
/*
** End of Hugo Elias code
*/