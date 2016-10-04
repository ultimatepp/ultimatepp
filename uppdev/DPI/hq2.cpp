#include <Core/Core.h>

using namespace Upp;

enum {
  diff_offset = (0x440 << 21) + (0x207 << 11) + 0x407,
  diff_mask   = (0x380 << 21) + (0x1f0 << 11) + 0x3f0,
};
 
uint32  yuvTable[32768];
uint8   rotate[256];
 
const uint8 hqTable[256] = {
  4, 4, 6,  2, 4, 4, 6,  2, 5,  3, 15, 12, 5,  3, 17, 13,
  4, 4, 6, 18, 4, 4, 6, 18, 5,  3, 12, 12, 5,  3,  1, 12,
  4, 4, 6,  2, 4, 4, 6,  2, 5,  3, 17, 13, 5,  3, 16, 14,
  4, 4, 6, 18, 4, 4, 6, 18, 5,  3, 16, 12, 5,  3,  1, 14,
  4, 4, 6,  2, 4, 4, 6,  2, 5, 19, 12, 12, 5, 19, 16, 12,
  4, 4, 6,  2, 4, 4, 6,  2, 5,  3, 16, 12, 5,  3, 16, 12,
  4, 4, 6,  2, 4, 4, 6,  2, 5, 19,  1, 12, 5, 19,  1, 14,
  4, 4, 6,  2, 4, 4, 6, 18, 5,  3, 16, 12, 5, 19,  1, 14,
  4, 4, 6,  2, 4, 4, 6,  2, 5,  3, 15, 12, 5,  3, 17, 13,
  4, 4, 6,  2, 4, 4, 6,  2, 5,  3, 16, 12, 5,  3, 16, 12,
  4, 4, 6,  2, 4, 4, 6,  2, 5,  3, 17, 13, 5,  3, 16, 14,
  4, 4, 6,  2, 4, 4, 6,  2, 5,  3, 16, 13, 5,  3,  1, 14,
  4, 4, 6,  2, 4, 4, 6,  2, 5,  3, 16, 12, 5,  3, 16, 13,
  4, 4, 6,  2, 4, 4, 6,  2, 5,  3, 16, 12, 5,  3,  1, 12,
  4, 4, 6,  2, 4, 4, 6,  2, 5,  3, 16, 12, 5,  3,  1, 14,
  4, 4, 6,  2, 4, 4, 6,  2, 5,  3,  1, 12, 5,  3,  1, 14,
};
 
static void initialize() {
  static bool initialized = false;
  if(initialized == true) return;
  initialized = true;
 
  for(unsigned i = 0; i < 32768; i++) {
    uint8 R = (i >>  0) & 31;
    uint8 G = (i >>  5) & 31;
    uint8 B = (i >> 10) & 31;
 
    //bgr555->bgr888
    double r = (R << 3) | (R >> 2);
    double g = (G << 3) | (G >> 2);
    double b = (B << 3) | (B >> 2);
 
    //bgr888->yuv888
    double y = (r + g + b) * (0.25f * (63.5f / 48.0f));
    double u = ((r - b) * 0.25f + 128.0f) * (7.5f / 7.0f);
    double v = ((g * 2.0f - r - b) * 0.125f + 128.0f) * (7.5f / 6.0f);
 
    yuvTable[i] = ((unsigned)y << 21) + ((unsigned)u << 11) + ((unsigned)v);
  }
 
  for(unsigned n = 0; n < 256; n++) {
    rotate[n] = ((n >> 2) & 0x11) | ((n << 2) & 0x88)
              | ((n & 0x01) << 5) | ((n & 0x08) << 3)
              | ((n & 0x10) >> 3) | ((n & 0x80) >> 5);
  }
}
 
static bool same(uint16 x, uint16 y) {
  return !((yuvTable[x] - yuvTable[y] + diff_offset) & diff_mask);
}
 
static bool diff(uint32 x, uint16 y) {
  return ((x - yuvTable[y]) & diff_mask);
}
 
static void grow(uint32 &n) { n |= n << 16; n &= 0x03e07c1f; }
static uint16 pack(uint32 n) { n &= 0x03e07c1f; return uint16(n | (n >> 16)); }
 
static uint16 blend1(uint32 A, uint32 B) {
  grow(A); grow(B);
  A = (A * 3 + B) >> 2;
  return pack(A);
}
 
static uint16 blend2(uint32 A, uint32 B, uint32 C) {
  grow(A); grow(B); grow(C);
  return pack((A * 2 + B + C) >> 2);
}
 
static uint16 blend3(uint32 A, uint32 B, uint32 C) {
  grow(A); grow(B); grow(C);
  return pack((A * 5 + B * 2 + C) >> 3);
}
 
static uint16 blend4(uint32 A, uint32 B, uint32 C) {
  grow(A); grow(B); grow(C);
  return pack((A * 6 + B + C) >> 3);
}
 
static uint16 blend5(uint32 A, uint32 B, uint32 C) {
  grow(A); grow(B); grow(C);
  return pack((A * 2 + (B + C) * 3) >> 3);
}
 
static uint16 blend6(uint32 A, uint32 B, uint32 C) {
  grow(A); grow(B); grow(C);
  return pack((A * 14 + B + C) >> 4);
}
 
static uint16 blend(unsigned rule, uint16 E, uint16 A, uint16 B, uint16 D, uint16 F, uint16 H) {
  switch(rule) { default:
    case  0: return E;
    case  1: return blend1(E, A);
    case  2: return blend1(E, D);
    case  3: return blend1(E, B);
    case  4: return blend2(E, D, B);
    case  5: return blend2(E, A, B);
    case  6: return blend2(E, A, D);
    case  7: return blend3(E, B, D);
    case  8: return blend3(E, D, B);
    case  9: return blend4(E, D, B);
    case 10: return blend5(E, D, B);
    case 11: return blend6(E, D, B);
    case 12: return same(B, D) ? blend2(E, D, B) : E;
    case 13: return same(B, D) ? blend5(E, D, B) : E;
    case 14: return same(B, D) ? blend6(E, D, B) : E;
    case 15: return same(B, D) ? blend2(E, D, B) : blend1(E, A);
    case 16: return same(B, D) ? blend4(E, D, B) : blend1(E, A);
    case 17: return same(B, D) ? blend5(E, D, B) : blend1(E, A);
    case 18: return same(B, F) ? blend3(E, B, D) : blend1(E, D);
    case 19: return same(D, H) ? blend3(E, D, B) : blend1(E, B);
  }
}
 
void filter_size(unsigned &width, unsigned &height) {
  initialize();
  width  *= 2;
  height *= 2;
}
 
void filter_render(
  uint32 *colortable, uint32 *output, unsigned outpitch,
  const uint16 *input, unsigned pitch, unsigned width, unsigned height
) {
  initialize();
  pitch >>= 1;
  outpitch >>= 2;

  for(unsigned y = 0; y < height; y++) {
    const uint16 *in = input + y * pitch;
    uint32 *out0 = output + y * outpitch * 2;
    uint32 *out1 = output + y * outpitch * 2 + outpitch;
 
    int prevline = (y == 0 ? 0 : pitch);
    int nextline = (y == height - 1 ? 0 : pitch);
 
    in++;
    *out0++ = 0; *out0++ = 0;
    *out1++ = 0; *out1++ = 0;
 
    for(unsigned x = 1; x < width - 1; x++) {
      uint16 A = *(in - prevline - 1);
      uint16 B = *(in - prevline + 0);
      uint16 C = *(in - prevline + 1);
      uint16 D = *(in - 1);
      uint16 E = *(in + 0);
      uint16 F = *(in + 1);
      uint16 G = *(in + nextline - 1);
      uint16 H = *(in + nextline + 0);
      uint16 I = *(in + nextline + 1);
      uint32 e = yuvTable[E] + diff_offset;
 
      uint8 pattern;
      pattern  = diff(e, A) << 0;
      pattern |= diff(e, B) << 1;
      pattern |= diff(e, C) << 2;
      pattern |= diff(e, D) << 3;
      pattern |= diff(e, F) << 4;
      pattern |= diff(e, G) << 5;
      pattern |= diff(e, H) << 6;
      pattern |= diff(e, I) << 7;
 
      *(out0 + 0) = colortable[blend(hqTable[pattern], E, A, B, D, F, H)]; pattern = rotate[pattern];
      *(out0 + 1) = colortable[blend(hqTable[pattern], E, C, F, B, H, D)]; pattern = rotate[pattern];
      *(out1 + 1) = colortable[blend(hqTable[pattern], E, I, H, F, D, B)]; pattern = rotate[pattern];
      *(out1 + 0) = colortable[blend(hqTable[pattern], E, G, D, H, B, F)];
 
      in++;
      out0 += 2;
      out1 += 2;
    }
 
    in++;
    *out0++ = 0; *out0++ = 0;
    *out1++ = 0; *out1++ = 0;
  }
}

