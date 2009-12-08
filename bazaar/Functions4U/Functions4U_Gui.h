#ifndef _Functions4U_Functions4U_Gui_h_
#define _Functions4U_Functions4U_Gui_h_

const RGBA *GetPixel(Image &img, int x, int y);
RGBA *GetPixel(ImageBuffer &img, int x, int y);

Image Rotate180(const Image& img);


#endif
