#include <Core/Core.h>

#include "matio.h"
#include "lib/matioConfig.h"

namespace Upp {
	
MatFile::MatStatic MatFile::cons;


MatVar::~MatVar() {
	if (var != NULL && del)
		Mat_VarFree(var);
}

MatVar::MatVar(mat_t *mat, String name) {
	ASSERT(mat != NULL);
	
	del = true;
	var = Mat_VarReadInfo(mat, name);
	if (var == NULL) 
		return;
}

const char* MatVar::GetTypeString()	{
	ASSERT(var != NULL); 
	
    const char *class_type_desc[18] = {"Undefined","Cell Array","Structure",
       "Object","Character Array","Sparse Array","Double Precision Array",
       "Single Precision Array", "8-bit, signed integer array",
       "8-bit, unsigned integer array","16-bit, signed integer array",
       "16-bit, unsigned integer array","32-bit, signed integer array",
       "32-bit, unsigned integer array","64-bit, signed integer array",
       "64-bit, unsigned integer array","Function","Opaque"};

	return class_type_desc[var->class_type];
}

int MatVar::GetCount() {
	ASSERT(var != NULL); 
	
	int ret = GetDimCount(0);
	for (int i = 1; i < GetDimCount(); ++i)
		ret *= GetDimCount(i);
	return ret;
}

MatFile::~MatFile() {
	Close();
}

void MatFile::Close() {
	if (mat != NULL) {
		Mat_Close(mat);
		mat = NULL;
	}
}

bool MatFile::Create(String fileName, mat_ft version) {
	if (mat != NULL)
		Mat_Close(mat);
	
	time_t t = time(NULL);
	String header = Format("MATLAB 5.0 MAT-file, Platform: %s, "
                "Created by: libmatio v%d.%d.%d on %s. U++ wrapper Bazaar/plugin/matio <https://www.ultimatepp.org/>", MATIO_PLATFORM,
                MATIO_MAJOR_VERSION, MATIO_MINOR_VERSION, MATIO_RELEASE_LEVEL,
                ctime(&t));
	mat = Mat_CreateVer(fileName, header, version);
	
	return !!mat;
}

}