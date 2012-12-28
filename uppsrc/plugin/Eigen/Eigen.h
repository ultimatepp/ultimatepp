#ifndef _Eigen_Eigen_h
#define _Eigen_Eigen_h

#include <Core/Core.h>

#define EIGEN_MATRIX_PLUGIN <plugin/Eigen/ToStringPlugin.h>
#define EIGEN_DENSEBASE_PLUGIN <plugin/Eigen/ToStringPlugin.h>

#ifndef flagSSE2
 #define EIGEN_DONT_VECTORIZE
 #define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT
#endif

#ifndef _DEBUG
#define EIGEN_NO_DEBUG
#endif

#define EIGEN_EXCEPTIONS

//#undef Success
#include "Eigen/Eigen"


NAMESPACE_UPP

template <class T>
void Xmlize(XmlIO &xml, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> &mat) {
	if(xml.IsStoring()) {
		Size sz(mat.cols(), mat.rows());
		xml ("Size", sz);
		for(int r = 0; r < mat.rows(); r++)
			for(int c = 0; c < mat.cols(); c++) {
				T data = mat(r, c);
				xml.Attr("item", data);
			}
	} else {
		Size sz;
		xml ("Size", sz);
		mat.resize(sz.cy, sz.cx);
		int r = 0; 
		int c = 0;
		for(int i = 1; i < xml->GetCount(); i++) {
			T data;
			xml.Attr("item", data);	
			mat(r, c) = data;
			++c;
			if (c == sz.cx) {
				c = 0;
				r++;
			}
			if (r == sz.cy)
				break;
		}	
	}
}

template <class T>
void Xmlize(XmlIO &xml, Eigen::Matrix<T, Eigen::Dynamic, 1> &vec) {
	if(xml.IsStoring()) {
		int sz = vec.size();
		xml ("Size", sz);
		for(int r = 0; r < vec.size(); r++) {
			T data = vec(r);
			xml.Attr("item", data);	
		}
	} else {
		int sz;
		xml ("Size", sz);
		vec.resize(sz);
		int r = 0; 
		for(int i = 1; i < xml->GetCount(); i++) {
			T data;
			xml.Attr("item", data);	
			vec(r) = data;
			++r;
			if (r == sz)
				break;
		}	
	}
}

template <class T>
void Jsonize(JsonIO &xml, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> &mat) {
	if(xml.IsStoring()) {
		Size sz(mat.cols(), mat.rows());
		xml.Set(sz);
		for(int r = 0; r < mat.rows(); r++)
			for(int c = 0; c < mat.cols(); c++) {
				T data = mat(r, c);
				xml.Set(data);	
			}
	} else {
		Size sz = xml.Get();
		mat.resize(sz.cy, sz.cx);
		int r = 0; 
		int c = 0;
		for(int i = 0; i < sz.cy*sz.cx; i++) {
			T data = xml.Get();
			mat(r, c) = data;
			++c;
			if (c == sz.cx) {
				c = 0;
				r++;
			}
			if (r == sz.cy)
				break;
		}	
	}
}

template <class T>
void Jsonize(JsonIO &xml, Eigen::Matrix<T, Eigen::Dynamic, 1> &vec) {
	Vector<T> vector;
	if(xml.IsStoring()) {
		vector.SetCount(vec.size());
		for (int i = 0; i < vec.size(); ++i)
			vector[i] = vec(i);
		xml("vector", vector);
	} else {
		xml("vector", vector);
		vec.resize(vector.GetCount());
		for (int i = 0; i < vec.size(); ++i)
			vec(i) = vector[i];
	}
}

END_UPP_NAMESPACE

#endif
