#ifndef _CtrlLibTest_PerlinNoise_h_
#define _CtrlLibTest_PerlinNoise_h_

#include <Core/Core.h>
using namespace Upp;

struct PerlinNoise
{
public:
	PerlinNoise() : octaves(2), persistence(0.5f)  {}

	PerlinNoise &Octaves(int _octaves) 		{ octaves = minmax(_octaves, 2, 31); return *this; }
	PerlinNoise &Persistence(float _pers) 	{ persistence = _pers; return *this; }

	virtual float Noise(int x, int y) const;
	float SmoothNoise1(int x, int y) const;
	float InterpolatedNoise1(float x, float y) const;
	float CosineInterpolate(float a, float b, float x) const;
	
	float PerlinNoise2D(int x, int y) const;
	float PerlinNoise2D(float x, float y) const;	
	float PerlinNoise3D(int x, int y, int z, int offset = 0x151) const
		{ return PerlinNoise2D(x + offset*z, y + offset*z); }
	float PerlinNoise3D(float x, float y, int z, int offset = 0x151) const
		{ return PerlinNoise2D(x + offset*z, y + offset*z); }		
protected:
	int octaves;
	float persistence;
};

class PerlinNoiseCache : private PerlinNoise, public Moveable<PerlinNoiseCache>
{
public:
	PerlinNoiseCache() 			{ Clear(); }
	void Cache(int cx, int cy);

	PerlinNoiseCache &Octaves(int _octaves) 		{ octaves = minmax(_octaves, 2, 31); return *this; }
	PerlinNoiseCache &Persistence(float _pers) 		{ persistence = max(0.0f, _pers); return *this; }

	Image		AsImage()	const;
	void 		Clear();

	Size		GetXY() const						{ return sz; }
	
	float		Get(int x, int y) const							{ ASSERT(~cache); return cache[(x % sz.cx) + sz.cx * (y % sz.cy)]; }
	float		Get(int x, int y, int z) const;
	float		Get(float x, float y, int z) const;
	virtual float operator()(int x, int y, int z) const 		{ return Get(x, y, z); }
	virtual float operator()(float x, float y, int z) const		{ return Get(x, y, z); }
	
private:
	Size 					sz;
	Buffer<float> 			cache;
	Buffer<float>			noise;
	
	void		  InitNoise();
	void		  DeinitNoise()									{ noise.Clear(); }
	virtual float Noise(int x, int y) const;	
};

#endif
