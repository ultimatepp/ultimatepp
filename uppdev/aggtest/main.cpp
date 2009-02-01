#include "aggtest.h"

// this function is defined in lion.cpp, where's also the description of the lion
unsigned parse_lion(agg::path_storage& ps, agg::rgba8* colors, unsigned* path_idx);

void AggTestCtrl::parse_lion() {
	npaths = ::parse_lion(path, colors, path_idx);
	agg::pod_array_adaptor<unsigned> pathidx(path_idx, 100);
	agg::bounding_rect(path, pathidx, 0, npaths, &x1, &y1, &x2, &y2);
	base_dx=(x2-x1);
	base_dy=(y2-y1);
};
AggTestCtrl::AggTestCtrl():
pixf(rbuf),rb(pixf),r(rb),
x1(0),y1(0),x2(0),y2(0),
npaths(0),base_dx(0),base_dy(0),scale(1.0){
	SetPixFmt(agg::pix_format_bgr24);
	parse_lion();
};
void AggTestCtrl::onDraw(Draw& dest){
	scale = 1;
	rb.clear(agg::rgba8(255, 255, 255));
	for(int i = 0; i < 200; i++) {
		RTIMING("Lion");
		agg::trans_affine mtx;
		mtx *= agg::trans_affine_translation(-base_dx/2.0, -base_dy/2.0);
		mtx *= agg::trans_affine_scaling(scale, scale);
		mtx *= agg::trans_affine_translation(rbuf.width()/2, rbuf.height()/2);
		agg::conv_transform<agg::path_storage, agg::trans_affine> trans(path, mtx);
		agg::render_all_paths(rasterizer, sl, r, trans, colors, path_idx, npaths);
	}
};
void AggTestCtrl::onInit(){
};
void AggTestCtrl::onResize(int width,int height){
	scale=Upp::min(width/base_dx,height/base_dy);
	rb.reset_clipping(true);
};
aggtest::aggtest(){
	CtrlLayout(*this, "AggCtrl");
	SetMinSize(Size(16,16));
	Sizeable();
};

GUI_APP_MAIN {
	aggtest().Run();
}
