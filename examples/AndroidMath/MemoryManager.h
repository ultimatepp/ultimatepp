#ifndef _JniMath_MemoryManager_h_
#define _JniMath_MemoryManager_h_

#include <jni.h>
#include <string>
#include <sstream>
#include <vector>

/**
 * Java "HashCode()" object memory manager.
 */
template <class T>
class MemoryManager {
public:
	void Insert(JNIEnv *env, jobject jobj, const T& obj)
	{
		std::string hashCode = GetJobjectHasCode(env, jobj);
		if(!hashCode.empty()) {
			hashCodes.push_back(hashCode);
			values.push_back(obj);
		}
	}
	
	void Erase(JNIEnv *env, jobject jobj)
	{
		int idx = FindIdx(env, jobj);
		if(idx >= 0) {
			hashCodes.erase(hashCodes.begin() + idx);
			values.erase(values.begin() + idx);
		}
	}
	
	T* Get(JNIEnv *env, jobject jobj)
	{
		int idx = FindIdx(env, jobj);
		return idx != -1 ? &values[idx] : NULL;
	}
	
private:
	int FindIdx(JNIEnv *env, jobject jobj)
	{
		std::string hashCode = GetJobjectHasCode(env, jobj);
		for(int i = 0; i < hashCodes.size(); i++) {
			if(hashCode == hashCodes[i]) {
				return i;
			}
		}
		return -1;
	}

	std::string GetJobjectHasCode(JNIEnv *env, jobject jobj)
	{
		jclass cls = env->GetObjectClass(jobj);
		jmethodID mid = env->GetMethodID(cls, "hashCode", "()I");
		if(mid == 0) {
			return "";
		}
		int ret = env->CallIntMethod(jobj, mid);
	
		std::stringstream ss;
		ss << ret;
	
		return ss.str();
	}

private:
	std::vector<std::string> hashCodes;
	std::vector<T> values; 
};

#endif
