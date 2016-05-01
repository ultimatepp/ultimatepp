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

#define eigen_assert(x) ASSERT(x)

//#undef Success
#include "Eigen/Eigen"
#include <plugin/Eigen/unsupported/Eigen/NonLinearOptimization>

NAMESPACE_UPP

using namespace Eigen;

template<typename _Scalar, ptrdiff_t nx = Dynamic, ptrdiff_t ny = Dynamic>
struct NonLinearOptimizationFunctor {
	typedef _Scalar Scalar;
	enum {
		InputsAtCompileTime = nx,
		ValuesAtCompileTime = ny
	};
	typedef Matrix<double, InputsAtCompileTime, 1> InputType;
	typedef Matrix<double, ValuesAtCompileTime, 1> ValueType;
	typedef Matrix<double, ValuesAtCompileTime, InputsAtCompileTime> JacobianType;
	
	int64 unknowns, datasetLen;
	
	NonLinearOptimizationFunctor() : unknowns(InputsAtCompileTime), datasetLen(ValuesAtCompileTime) {}
	NonLinearOptimizationFunctor(int unknowns, int datasetLen) : unknowns(unknowns), datasetLen(datasetLen) {}
	
	ptrdiff_t inputs() const {return ptrdiff_t(unknowns);}
	ptrdiff_t values() const {return ptrdiff_t(datasetLen);}
	virtual void operator() (const InputType& x, ValueType* v, JacobianType* j = 0) const {};
};

template <class T>
void Xmlize(XmlIO &xml, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> &mat) {
	Size_<int64> sz(mat.cols(), mat.rows());
	xml ("size", sz);
	if(xml.IsStoring()) {
		for(int r = 0; r < mat.rows(); r++)
			for(int c = 0; c < mat.cols(); c++) {
				XmlIO io = xml.Add("item");
				T data = mat(r, c);
				Xmlize(io, data);
			}
	} else {
		mat.resize(ptrdiff_t(sz.cy), ptrdiff_t(sz.cx));
		int r = 0, c = 0;
		for(int i = 0; i < xml->GetCount(); i++) 
			if(xml->Node(i).IsTag("item")) {
				XmlIO io = xml.At(i);
				T data;
				Xmlize(io, data);
				mat(r, c) = data;
				++c;
				if (c == sz.cx) {
					c = 0;
					r++;
				}
			}
	}
}

template <class T>
void Xmlize(XmlIO &xml, Eigen::Matrix<T, Eigen::Dynamic, 1> &vec) {
	int64 sz = vec.size();
	xml ("size", sz);
	if(xml.IsStoring()) {
		for(int r = 0; r < sz; r++) {
			XmlIO io = xml.Add("item");
			T data = vec(r);
			Xmlize(io, data);
		}
	} else {
		vec.resize(ptrdiff_t(sz));
		int r = 0;
		for(int i = 0; i < xml->GetCount(); i++)
			if(xml->Node(i).IsTag("item")) {
				XmlIO io = xml.At(i);
				T data;
				Xmlize(io, data);
				vec(r++) = data;
			}
	}
}

template <class T>
void Jsonize(JsonIO &io, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> &mat) {
	Vector<T> vector;
	Size_<int64> sz(mat.cols(), mat.rows());
	io("size", sz);
	if(io.IsStoring()) {
		vector.SetCount(int(sz.cx)*int(sz.cy));
		int i = 0;
		for(int r = 0; r < mat.rows(); r++)
			for(int c = 0; c < mat.cols(); c++) 
				vector[i++] = mat(r, c);
		io("data", vector);
	} else {
		io("data", vector);
		mat.resize(ptrdiff_t(sz.cy), ptrdiff_t(sz.cx));
		int r = 0, c = 0;
		for (int i = 0; i < vector.GetCount(); ++i) {
			mat(r, c) = vector[i];
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
void Jsonize(JsonIO &io, Eigen::Matrix<T, Eigen::Dynamic, 1> &vec) {
	Vector<T> vector;
	int64 sz = vec.size();
	io("size", sz);
	if(io.IsStoring()) {
		vector.SetCount(int(sz));
		for (int i = 0; i < sz; ++i)
			vector[i] = vec(i);
		io("data", vector);
	} else {
		io("data", vector);
		vec.resize(ptrdiff_t(sz));
		for (int i = 0; i < vec.size(); ++i)
			vec(i) = vector[i];
	}
}

template <class T>
void Serialize(Stream& stream, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> &mat) {
	Size_<int64> sz(mat.cols(), mat.rows());
	stream % sz;
	if(stream.IsStoring()) {
		for(int r = 0; r < mat.rows(); r++)
			for(int c = 0; c < mat.cols(); c++) {
				T data = mat(r, c);
				stream % data;	
			}
	} else {
		mat.resize(ptrdiff_t(sz.cy), ptrdiff_t(sz.cx));
		int r = 0, c = 0;
		for(int i = 0; i < sz.cy*sz.cx; i++) {
			T data;
			stream % data;
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
void Serialize(Stream& stream, Eigen::Matrix<T, Eigen::Dynamic, 1> &vec) {
	int64 sz = vec.size();
	stream % sz;
	if(stream.IsStoring()) {
		for (int i = 0; i < sz; ++i) {
			T data = vec(i);
			stream % data;
		}
	} else {
		vec.resize(ptrdiff_t(sz));
		for (int i = 0; i < sz; ++i) {
			T data;
			stream % data;
			vec(i) = data;
		}
	}
}

END_UPP_NAMESPACE

#endif
