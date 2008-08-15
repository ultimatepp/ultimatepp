#ifndef _agg_upp_bind_test_agg_upp_bind_test_h_
#define _agg_upp_bind_test_agg_upp_bind_test_h_


//#include <CtrlLib/CtrlLib.h>
//#define AGG_RGBA32
//#include <agg_aris_main/agg_aris_main.h>

//#include <vector>
//namespace agg
//{
//	UPP::NTL_MOVEABLE(vertex_d);
//	typedef path_base<vertex_stl_storage<std::vector<vertex_f> > > stl_path_storage; 
//	typedef path_base<vertex_upp_storage<Upp::Vector<vertex_d> > > upp_path_storage;
	
//}

//#define path_storage stl_path_storage
//#define path_storage upp_path_storage



#include <agg_upp_bind/agg_upp_bind.h>

using namespace Upp;

#if 0
void TrianglePath(agg::path_storage& path, double *_m_x, double *_m_y)
{
	//aris: test ...
//	agg::path_storage path;
		path.move_to(_m_x[0], _m_y[0]);
		path.line_to(_m_x[1], _m_y[1]);
		path.line_to(_m_x[2], _m_y[2]);
	path.close_polygon();
//	return path;
};
#endif

#endif
