#ifndef _JniMath_MemoryManager_h_
#define _JniMath_MemoryManager_h_

#include <jni.h>
#include <vector>

/**
 * Java simply memory manager.
 */
template <class T>
class MemoryManager {
public:
	void Insert(JNIEnv *env, jobject jobj, const T& obj)
	{
		jobject weakGlobalRef = env->NewWeakGlobalRef(jobj);
		if(!env->IsSameObject(weakGlobalRef, NULL)) {
			objs.push_back(env->NewWeakGlobalRef(weakGlobalRef));
			values.push_back(obj);
		}
	}
	
	void MakeCopy(JNIEnv *env, jobject jobjSrc, jobject jobjDst)
	{
		if(!env->IsSameObject(jobjSrc, jobjDst)) {
			T t(*Get(env, jobjSrc));
			Insert(env, jobjSrc, t);
		}
	}
	
	void Erase(JNIEnv *env, jobject jobj)
	{
		int idx = FindIdx(env, jobj);
		if(idx >= 0) {
			objs.erase(objs.begin() + idx);
			values.erase(values.begin() + idx);
		}
	}
	
	T* Get(JNIEnv *env, jobject jobj)
	{
		int idx = FindIdx(env, jobj);
		return idx >= 0 ? &values[idx] : NULL;
	}
	
	int GetCount()
	{
		return static_cast<int>(values.size());
	}
	
private:
	int FindIdx(JNIEnv *env, jobject jobj)
	{
		for(int i = 0; i < objs.size(); i++) {
			if(env->IsSameObject(objs[i], jobj)) {
				return i;
			}
		}
		return 0;
	}

private:
	std::vector<jobject> objs;
	std::vector<T> values;
};

#endif
