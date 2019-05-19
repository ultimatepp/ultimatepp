#ifndef _plugin_matio_matio_h_
#define _plugin_matio_matio_h_

#include "./lib/matio.h"

template <class T>
class MatMatrix {
public:
	MatMatrix() : rows(0), cols(0) {}
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
			
private:
	Buffer<T> data;
	int rows, cols;
};

class MatVar {
public:
	MatVar() : var(NULL) {}
	~MatVar();
	MatVar(mat_t *mat, String name);
	
	const char *GetName()		{ASSERT(var != NULL); return var->name;}
	
	enum matio_types GetType() 	{ASSERT(var != NULL); return var->data_type;}
	
	const char* GetTypeString();

	int GetDimCount()			{ASSERT(var != NULL); return var->rank;}
	
	int GetDimCount(int dim)	{ASSERT(var != NULL); return (int)var->dims[dim];}
	
	int GetCount();
	
private:
	matvar_t *var;
	
	friend class MatFile;
};
	
class MatFile {
public:
	MatFile() : mat(NULL), listVar(NULL), numVar(0) {}
	~MatFile();
	
	bool Create(String fileName, mat_ft version = MAT_FT_MAT5);
	
	bool OpenRead(String fileName) 	{return Open(fileName, MAT_ACC_RDONLY);}
	bool OpenWrite(String fileName) {return Open(fileName, MAT_ACC_RDWR);}
	bool IsOpen()					{return !!mat;}
	
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
		
		return 0 == Mat_VarDelete(mat, name);
	}
	
	MatVar GetVar(String name) {return MatVar(mat, name);}
				
	template <class T>
	MatMatrix<T> VarRead(MatVar &var) {
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
	MatMatrix<T> VarRead(String name) {
		MatVar var = GetVar(name);
		return VarRead<T>(var);
	}
	
	template<class T> void inline GetTypeCode(enum matio_classes &class_type, enum matio_types &data_type) const {
		NEVER_("Unsupported type in matio");		
	}
	
	template <class T>
	bool VarWrite(String name, MatMatrix<T> &data, bool compression = true) {
		Buffer<size_t> dims(2);
		dims[0] = data.GetRows();
		dims[1] = data.GetCols();
		
		enum matio_classes class_type;
		enum matio_types data_type;
		GetTypeCode<T>(class_type, data_type);
		
		if (VarExists(name))
			if (0 != Mat_VarDelete(mat, name))
				return false;
		matvar_t *var = Mat_VarCreate(name, class_type, data_type, 2, dims, data, MAT_F_DONT_COPY_DATA);
		if (var == NULL)
			return false;
		if (0 != Mat_VarWrite(mat, var, MAT_COMPRESSION_NONE))
			return false;
		return true;
	}
	
	template <class T>
	bool VarWrite(String name, T data, bool compression = true) {
		Buffer<size_t> dims(1);
		dims[0] = 1;
		
		enum matio_classes class_type;
		enum matio_types data_type;
		GetTypeCode<T>(class_type, data_type);
		
		if (VarExists(name))
			Mat_VarDelete(mat, name);
		matvar_t *var = Mat_VarCreate(name, class_type, data_type, 1, dims, &data, MAT_F_DONT_COPY_DATA);
		if (var == NULL)
			return false;
		if (0 != Mat_VarWrite(mat, var, MAT_COMPRESSION_NONE))
			return false;
		return true;
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
		
		if (listVar == NULL) {
			numVar = 0; 
			listVar = Mat_GetDir(mat, &numVar);
		}
	}
	
	static MatStatic cons;
	
	static void LogFunc(int log_level, char *message) {
		cons.logLevel = log_level;
		cons.lastError = message;
	}
};

template<> void inline MatFile::GetTypeCode<double>	(enum matio_classes &class_type, enum matio_types &data_type) const {
	class_type = MAT_C_DOUBLE;		
	data_type  = MAT_T_DOUBLE;
}

template<> void inline MatFile::GetTypeCode<float>	(enum matio_classes &class_type, enum matio_types &data_type) const {
	class_type = MAT_C_SINGLE;		
	data_type  = MAT_T_SINGLE;
}

template<> void inline MatFile::GetTypeCode<int>	(enum matio_classes &class_type, enum matio_types &data_type) const {
	class_type = MAT_C_INT64;		
	data_type  = MAT_T_INT64;
}

#endif
