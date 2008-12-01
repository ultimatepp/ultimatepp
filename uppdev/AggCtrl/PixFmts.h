#ifndef _AggCtrl_PixFmts_h_
#define _AggCtrl_PixFmts_h_

// Using a particular pixel format doesn't obligatory mean the necessity
// of software conversion. For example, win32 API can natively display 
// gray8, 15-bit RGB, 24-bit BGR, and 32-bit BGRA formats. 
// This list can be (and will be!) extended in future.
namespace agg{
	enum pix_format_e{
		pix_format_undefined = 0,  // By default. No conversions are applied 
		pix_format_bw,     // 1 bit per color B/W
		pix_format_gray8,  // Simple 256 level grayscale
		pix_format_gray16, // Simple 65535 level grayscale
		pix_format_rgb555, // 15 bit rgb. Depends on the byte ordering!
		pix_format_rgb565, // 16 bit rgb. Depends on the byte ordering!
		pix_format_rgbAAA, // 30 bit rgb. Depends on the byte ordering!
		pix_format_rgbBBA, // 32 bit rgb. Depends on the byte ordering!
		pix_format_bgrAAA, // 30 bit bgr. Depends on the byte ordering!
		pix_format_bgrABB, // 32 bit bgr. Depends on the byte ordering!
		pix_format_rgb24,  // R-G-B, one byte per color component
		pix_format_bgr24,  // B-G-R, native win32 BMP format.
		pix_format_rgba32, // R-G-B-A, one byte per color component
		pix_format_argb32, // A-R-G-B, native MAC format
		pix_format_abgr32, // A-B-G-R, one byte per color component
		pix_format_bgra32, // B-G-R-A, native win32 BMP format
		pix_format_rgb48,  // R-G-B, 16 bits per color component
		pix_format_bgr48,  // B-G-R, native win32 BMP format.
		pix_format_rgba64, // R-G-B-A, 16 bits byte per color component
		pix_format_argb64, // A-R-G-B, native MAC format
		pix_format_abgr64, // A-B-G-R, one byte per color component
		pix_format_bgra64, // B-G-R-A, native win32 BMP format
		  
		end_of_pix_formats
	};
};

#endif
