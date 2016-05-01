#ifndef _JniMath_MemoryManager_h_
#define _JniMath_MemoryManager_h_

#include <jni.h>
#include <vector>
#include <string>
#include <sstream>

static const char* FieldNativeAddress = "nativeAdress";

static const char* LongSig = "J";

/**
 * Java simple memory manager.
 */
template <class T>
class MemoryManager {
public:
	~MemoryManager()
	{
		for(int i = 0; i < values.size(); i++) {
			delete values[i];
		}
	}

	void Insert(JNIEnv *env, jobject jobj, T* value)
	{
		if(jobj != NULL) {
			values.push_back(value);
			
			SetNativeAdress(env, jobj, values[values.size() - 1]);
		}
	}
	
	void MakeCopy(JNIEnv *env, jobject jobjThis, jobject jobjThat)
	{
		if(!env->IsSameObject(jobjThis, jobjThat)) {
			T* t = Get(env, jobjThat);
			Insert(env, jobjThis, t);
		}
	}
	
	void Erase(JNIEnv *env, jobject jobj)
	{
		int idx = FindIdx(env, jobj);
		if(idx >= 0)
			values.erase(values.begin() + idx);
	}
	
	T* Get(JNIEnv *env, jobject jobj)
	{
		int idx = FindIdx(env, jobj);
		return idx >= 0 ? values[idx] : NULL;
	}
	
	int GetCount()
	{
		return static_cast<int>(values.size());
	}
	
private:
	int FindIdx(JNIEnv *env, jobject jobj)
	{
		for(int i = 0; i < values.size(); i++) {
			if(GetNativeAdress(env, jobj) == values[i]) {
				return i;
			}
		}
		return -1;
	}
	
private:
	T* GetNativeAdress(JNIEnv* env, jobject jobj)
	{
		jclass objectClass = env->GetObjectClass(jobj);
		
		jfieldID adressField = GetNativeAdressField(env, objectClass);
		
		env->DeleteLocalRef(objectClass);
		return (T*)env->GetLongField(jobj, adressField);
	}
	
	void SetNativeAdress(JNIEnv* env, jobject jobj, T* obj)
	{
		jclass objectClass = env->GetObjectClass(jobj);
		
		jfieldID adressField = GetNativeAdressField(env, objectClass);
		env->SetLongField(jobj, adressField, (jlong)obj);
		
		env->DeleteLocalRef(objectClass);
	}
	
	jfieldID GetNativeAdressField(JNIEnv* env, jclass clazz)
	{
		return env->GetFieldID(clazz, FieldNativeAddress, LongSig);
	}
	
private:
	std::vector<T*> values;
};

#endif
