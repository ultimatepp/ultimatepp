#include "bicubic.h"

#if 0

double BiCubicR(double x)
{
   double xp2,xp1,xm1;
   double r = 0;

   xp2 = x + 2;
   xp1 = x + 1;
   xm1 = x - 1;

   if (xp2 > 0)
      r += xp2 * xp2 * xp2;
   if (xp1 > 0)
      r -= 4 * xp1 * xp1 * xp1;
   if (x > 0)
      r += 6 * x * x * x;
   if (xm1 > 0)
      r -= 4 * xm1 * xm1 * xm1;

   return(r / 6.0);
}

void BiCubicScale(const RGBA *bm_in, int nx, int ny,
                  RGBA *bm_out, int nnx, int nny)
{
   int i_out,j_out,i_in,j_in,ii,jj;
   int n,m;
	long index;
   double cx,cy,dx,dy,weight;
   double red,green,blue,alpha;
   RGBA col;
 
   for (i_out=0;i_out<nnx;i_out++) {
      for (j_out=0;j_out<nny;j_out++) {
         i_in = (i_out * nx) / nnx;
         j_in = (j_out * ny) / nny;
         cx = i_out * nx / (double)nnx;
         cy = j_out * ny / (double)nny;
         dx = cx - i_in;
         dy = cy - j_in;
         red   = 0;
         green = 0;
         blue  = 0;
			alpha = 0;
         for (m=-1;m<=2;m++) {
            for (n=-1;n<=2;n++) {
				ii = i_in + m;
				jj = j_in + n;
				if (ii < 0)   ii = 0;
				if (ii >= nx) ii = nx-1;
				if (jj < 0)   jj = 0;
				if (jj >= ny) jj = ny-1;
				index = jj * nx + ii;
				weight = BiCubicR(m-dx) * BiCubicR(n-dy);
				red   += weight * bm_in[index].r;
				green += weight * bm_in[index].g;
				blue  += weight * bm_in[index].b;
				alpha += weight * bm_in[index].a;
            }
         }
         col.r = (int)red;
         col.g = (int)green;
         col.b = (int)blue;
		 col.a = (int)alpha;
         bm_out[j_out * nnx + i_out] = col;
      }
   }
}

Image RescaleBicubic(Image& img, int cx, int cy)
{
	ImageBuffer ib(cx, cy);
	BiCubicScale(~img, img.GetWidth(), img.GetHeight(), ~ib, cx, cy);
	return ib;
}
#endif