#ifndef _plugin_matio_matio_h_
#define _plugin_matio_matio_h_

#include "./lib/matio.h"

namespace Upp {

template<class T> void GetTypeCode 				(enum matio_classes &class_type, enum matio_types &data_type) {
	NEVER_("Class unsupported");
}

template<> void inline GetTypeCode<double>		(enum matio_classes &class_type, enum matio_types &data_type) {
	class_type = MAT_C_DOUBLE;		
	data_type  = MAT_T_DOUBLE;
}

template<> void inline GetTypeCode<float>		(enum matio_classes &class_type, enum matio_types &data_type) {
	class_type = MAT_C_SINGLE;		
	data_type  = MAT_T_SINGLE;
}

template<> void inline GetTypeCode<int>			(enum matio_classes &class_type, enum matio_types &data_type) {
	class_type = MAT_C_INT64;		
	data_type  = MAT_T_INT64;
}

template<> void inline GetTypeCode<String>		(enum matio_classes &class_type, enum matio_types &data_type) {
	class_type = MAT_C_CHAR;		
	data_type  = MAT_T_UTF8;
}

template<> void inline GetTypeCode<const char *> (enum matio_classes &class_type, enum matio_types &data_type) {
	class_type = MAT_C_CHAR;		
	data_type  = MAT_T_UTF8;
}

template <class T>
class MatMatrix {
public:
	MatMatrix() : rows(0), cols(0) 			{}
	MatMatrix(int rows, int cols)  			{Alloc(rows, cols);}
	MatMatrix(const Nuller&)               	{rows = cols = 0;}
	bool IsNullInstance() const    			{return rows == 0 && cols == 0; }
	
	void Alloc(int size) {
		data.Alloc(size); 
		this->rows = size;
		this->cols = 1;
	}
	void Alloc(int rows, int cols) {
		data.Alloc(rows*cols); 
		this->rows = rows;
		this->cols = cols;	
	}
	void Clear() 							{data.Clear();}
	
	T &operator()(int r)					{return data[r];}
	T &operator()(int r, int c)				{return data[c*rows + r];}
	const T &operator()(int r) const		{return data[r];}
	const T &operator()(int r, int c) const	{return data[c*rows + r];}
		
	operator T*()                    		{ return data; }
	operator const T*() const        		{ return data; }
		
	int GetCount() 							{return rows*cols;}
	int GetRows() 							{return rows;}
	int GetCols() 							{return cols;}
			
	void Print() {
		for(int r = 0; r < GetRows(); r++) {
			Cout() << "\n";
			for(int c = 0; c < GetCols(); c++)
				Cout() << operator()(r, c) << "  ";
		}	
		Cout() << "\n";
	}
	
//private:
	Buffer<T> data;
	int rows, cols;
};

class MatVar {
public:
	MatVar() : var(NULL), del(true) {}
	MatVar(const Nuller&)			{var = 0;} 
	bool IsNullInstance() const    	{return var == 0;}
	
	MatVar(String name, int rows, int cols, Vector<String> &elements) {
		Buffer<size_t> dim(2);
		dim[0] = rows;
		dim[1] = cols;
		
		Buffer<const char *> names(elements.GetCount());
		for (int i = 0; i < elements.GetCount(); ++i)
			names[i] = ~(elements[i]);
		var = Mat_VarCreateStruct(name, 2, dim, names, elements.GetCount());
		this->del = false;
	}
	
	~MatVar();
	
	const char *GetName()		{ASSERT(var != NULL); return var->name;}
	
	enum matio_classes GetType() 	{ASSERT(var != NULL); return var->class_type;}
	
	const char* GetTypeString();

	int GetDimCount() const			{ASSERT(var != NULL); return var->rank;}
	
	int GetDimCount(int dim) const	{ASSERT(var != NULL); return (int)var->dims[dim];}
	
	int GetCount();
	
	int GetFieldCount() {
		ASSERT(var != NULL); 
		ASSERT(var->class_type == MAT_C_STRUCT);
		
		return Mat_VarGetNumberOfFields(var);
	}
	const char *GetFieldName(int id) {	
		ASSERT(var != NULL); 
		ASSERT(var->class_type == MAT_C_STRUCT);
		
		return Mat_VarGetStructFieldnames(var)[id];
	}
	
	MatVar GetVar(String name) {
		ASSERT(var != NULL); 
		ASSERT(var->class_type == MAT_C_STRUCT);
		
		return MatVar(Mat_VarGetStructFieldByName(var, name, 0), false);
	}
	
	bool IsLoaded() {return var != 0;}
	
	
	void VarWriteStruct(String name, MatVar &val) {
		Mat_VarSetStructFieldByName(var, name, 0, val.var); 
	}
	
	template <class T>
	void VarWriteStruct(String name, void *data, int numRows, int numCols, int index) {
		int numDim = 2;
		if (IsNull(numCols)) 
			numDim = 1;
			
		Buffer<size_t> dims(numDim);
		dims[0] = numRows;
		if (!IsNull(numCols)) 
			dims[1] = numCols;
		
		enum matio_classes class_type;
		enum matio_types data_type;
		GetTypeCode<T>(class_type, data_type);
		
		matvar_t *variable = Mat_VarCreate(name, class_type, data_type, numDim, dims, data, 0);
		Mat_VarSetStructFieldByName(var, name, index, variable); 
	}

	template <class T>
	void VarWriteStruct(String name, MatMatrix<T> &data, int index = 0) {
		VarWriteStruct<T>(name, data, data.GetRows(), data.GetCols(), index);
	}
	
	void VarWriteStruct(String name, String data, int index = 0) {
		VarWriteStruct<String>(name, (void *)data.Begin(), 1, data.GetCount(), index);
	}

	void VarWriteStruct(String name, const char *data, int index = 0) {
		VarWriteStruct<String>(name, (void *)data, 1, (int)strlen(data), index);
	}

	template <class T>
	void VarWriteStruct(String name, T data, int index = 0) {
		VarWriteStruct<T>(name, &data, 1, Null, index);
	}
	
private:
	MatVar(mat_t *mat, String name);
	MatVar(matvar_t *var, bool del) {this->var = var; this->del = del;}
	
	matvar_t *var;
	bool del;
	
	friend class MatFile;
};
	
class MatFile {
public:
	MatFile() : mat(NULL), listVar(NULL), numVar(0) {}
	~MatFile();
	
	bool Create(String fileName, mat_ft version = MAT_FT_MAT5);
	void Close();
	
	bool OpenRead(String fileName) 	{return Open(fileName, MAT_ACC_RDONLY);}
	bool OpenWrite(String fileName) {return Open(fileName, MAT_ACC_RDWR);}
	bool IsOpen()					{return !!mat;}
	bool OpenCreate(String fileName, enum mat_ft mat_file_ver) {
		if (mat != NULL)
			Mat_Close(mat);
	
		mat = Mat_CreateVer(fileName, "BEMRosetta", mat_file_ver);
			    
    	return !!mat;
	}
	
	mat_ft GetVersion() {
		ASSERT(mat != NULL);
		
		return Mat_GetVersion(mat);
	}
	
	String GetVersionName() {
		mat_ft ver = GetVersion();
		switch (ver) {
		case MAT_FT_MAT4: 		return "4";
		case MAT_FT_MAT5: 		return "5";
		case MAT_FT_MAT73:		return "7.3";
		case MAT_FT_UNDEFINED:	return "unknown";
		}
		return "unknown";
	}
	
	int GetVarCount() {
		GetVarList();
		return (int)numVar;
	}
	
	String GetVarName(int id) {
		GetVarList();
		if (listVar == NULL)
			return Null;
		
		if (id >= (int)numVar)
			return Null;
		return listVar[id];
	}
	
		
	bool VarExists(String name) {
		GetVarList();
		if (listVar == NULL)
			return false;
		
		for (int i = 0; i < (int)numVar; ++i) {
			if (listVar[i] == name)
				return true;
		}
		return false;
	}
	
	bool VarDelete(String name) {
		ASSERT(mat != NULL);
		
		int ret = Mat_VarDelete(mat, name);
		if (ret == 0) {
			GetVarList();
			return 0;
		}
		return ret;
	}
	
	MatVar GetVar(String name) {return MatVar(mat, name);}

	template <class T> inline
	T VarRead(const MatVar &var) {
		ASSERT(mat != NULL);
	
		T ret = Null;
			
		int numDim = var.GetDimCount();	
		if (numDim > 2)
			return ret;
			
		Buffer<int> start(numDim, 0);
		Buffer<int> stride(numDim, 1);
		Buffer<int> edge(numDim);
	
		for (int i = 0; i < numDim; ++i)
			edge[i] = var.GetDimCount(i);
	
		if (0 != Mat_VarReadData(mat, var.var, &ret, start, stride, edge)) 
			return ret;
		
		return ret;	
	}
	

	template <class T> inline
	T VarRead(String name) {
		MatVar var = GetVar(name);
		if (IsNull(var))
			return Null;
		return VarRead<T>(var);
	}
	
	template <class T>
	MatMatrix<T> VarReadMat(MatVar &var) {
		ASSERT(mat != NULL);
	
		MatMatrix<T> ret;
			
		int numDim = var.GetDimCount();	
		if (numDim > 2)
			return ret;
			
		Buffer<int> start(numDim, 0);
		Buffer<int> stride(numDim, 1);
		Buffer<int> edge(numDim);
	
		for (int i = 0; i < numDim; ++i)
			edge[i] = var.GetDimCount(i);
	
		ret.Alloc((int)var.GetDimCount(0), (int)var.GetDimCount(1));
	
		if (0 != Mat_VarReadData(mat, var.var, ret, start, stride, edge)) {
			ret.Clear();
			return ret;
		}	
		return ret;	
	}	
	
	template <class T>
	MatMatrix<std::complex<T>> VarReadMatComplex(MatVar &var) {
		ASSERT(mat != NULL);
	
		MatMatrix<std::complex<T>> ret;
			
		int numDim = var.GetDimCount();	
		if (numDim > 2)
			return ret;
			
		Buffer<int> start(numDim, 0);
		Buffer<int> stride(numDim, 1);
		Buffer<int> edge(numDim);
	
		for (int i = 0; i < numDim; ++i)
			edge[i] = var.GetDimCount(i);
	
		ret.Alloc((int)var.GetDimCount(0), (int)var.GetDimCount(1));
	
		Buffer<T> real(var.GetCount()), imag(var.GetCount());
		
		struct mat_complex_split_t data = {real, imag};
		
		if (0 != Mat_VarReadData(mat, var.var, &data, start, stride, edge)) {
			ret.Clear();
			return ret;
		}	
		
		for (int i = 0; i < var.GetCount(); ++i) {
			ret(i).real(real[i]);
			ret(i).imag(imag[i]);
		}
		return ret;	
	}
	
	template <class T>
	MatMatrix<T> VarReadMat(String name) {
		MatVar var = GetVar(name);
		if (IsNull(var))
			return Null;
		return VarReadMat<T>(var);
	}
	
	bool VarWrite(MatVar &var, bool compression = true) {
		if (0 != Mat_VarWrite(mat, var.var, compression ? MAT_COMPRESSION_NONE : MAT_COMPRESSION_ZLIB))
			return false;
		return true;
	}
		
	template <class T>
	bool VarWrite(String name, void *data, int numRows, int numCols, bool compression = true) {
		int numDim = 2;
		if (IsNull(numCols)) 
			numDim = 1;
			
		Buffer<size_t> dims(numDim);
		dims[0] = numRows;
		if (!IsNull(numCols)) 
			dims[1] = numCols;
		
		enum matio_classes class_type;
		enum matio_types data_type;
		GetTypeCode<T>(class_type, data_type);
		
		if (VarExists(name))
			if (0 != Mat_VarDelete(mat, name))
				return false;
		matvar_t *var = Mat_VarCreate(name, class_type, data_type, numDim, dims, data, MAT_F_DONT_COPY_DATA);
		if (var == NULL)
			return false;
		if (0 != Mat_VarWrite(mat, var, compression ? MAT_COMPRESSION_NONE : MAT_COMPRESSION_ZLIB))
			return false;
		return true;
	}
	
	template <class T>
	bool VarWrite(String name, MatMatrix<T> &data, bool compression = true) {
		return VarWrite<T>(name, data, data.GetRows(), data.GetCols(), compression);
	}
	
	template <class T>
	bool VarWrite(String name, T data, bool compression = true) {
		return VarWrite<T>(name, &data, 1, Null, compression);
	}
	
	bool VarWrite(String name, String data, bool compression = true) {
		return VarWrite<String>(name, (void *)data.Begin(), 1, data.GetCount(), compression);
	}

	static String GetLastError()	{return cons.lastError;}
	
	struct MatStatic {
		MatStatic() {
			Mat_LogInitFunc("Matio", LogFunc);
		}
		int logLevel;
		String lastError;
	};
	
private:
	mat_t *mat;
	char **listVar;
	size_t numVar;
	
	bool Open(String fileName, int mode) {
		if (mat != NULL)
			Mat_Close(mat);
	
		mat = Mat_Open(fileName, mode);
			    
    	return !!mat;
	}
	
	void GetVarList() {
		ASSERT(mat != NULL);
		
		numVar = 0; 
		listVar = Mat_GetDir(mat, &numVar);
	}
	
	static MatStatic cons;
	
	static void LogFunc(int log_level, char *message) {
		cons.logLevel = log_level;
		cons.lastError = message;
	}
};

template <> inline
String MatFile::VarRead<String>(const MatVar &var) {
	ASSERT(mat != NULL);
		
	int numDim = var.GetDimCount();	
	if (numDim > 2)
		return String();
		
	Buffer<int> start(numDim, 0);
	Buffer<int> stride(numDim, 1);
	Buffer<int> edge(numDim);

	for (int i = 0; i < numDim; ++i)
		edge[i] = var.GetDimCount(i);

	String ret(0, 1 + var.GetDimCount(0)*var.GetDimCount(1));

	var.var->class_type = MAT_C_INT8;
	var.var->data_type  = MAT_T_INT8;

	if (0 != Mat_VarReadData(mat, var.var, (void *)ret.Begin(), start, stride, edge)) 
		return String();
	
	return ret;	
}

template <> inline
MatMatrix<std::complex<float>> MatFile::VarReadMat(MatVar &var) {return VarReadMatComplex<float>(var);}

template <> inline
MatMatrix<std::complex<double>> MatFile::VarReadMat(MatVar &var) {return VarReadMatComplex<double>(var);}

template <> inline
MatMatrix<std::complex<long double>> MatFile::VarReadMat(MatVar &var) {return VarReadMatComplex<long double>(var);}

}
	
#endif

