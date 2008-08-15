Use automake to build the library. 

If automake is not available you still can use the old make. 
There is a very simple Makefile that can be used. Note that
if you use automake it will overwrite Makefile.
=====================================================

this file was changed by aris002@yahoo.co.uk for upp package
I also changed the stucture of agg directories and includes (to have less headaches :)) ...

!!! You don't need to use makefiles with upp!!!

Install like normal upp package
Any questions - http://www.arilect.com/upp/forum/

15/07/2006 Added upp Vector support to agg::path_storage

Important from antigrain web:
  It can seem it's always better to use the packed version, but in practice it's not so. 
The scanline_p works faster when rendering large solid polygons, 
that is when the area of the polygon is much larger than the perimeter 
in the meaning of the number of pixels. 
  But when renering text it's definitely better to use the scanline_u 
because of much less number of processed spans.
The difference is about three times
and the number of spans is also important for the overall performance.
  Besides, in most of the span generators, such as gradients, 
Gouraud shader, or image transformers,
the number of spans becomes even more critical, and so,
the scanline_p is not used there.

==
Aris: scanline_p8 p-olygons large.
==
my svg xml parsing is an old version and also maybe it should be a separate upp package
==