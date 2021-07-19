package org.upp.AndroidMath;

/**
 * Math vector class which whole implementation is native (c/c++).
 */
public class Vector
{
	/**
	 * Field use by MemoryManager to store pointer to c++ object.
	 * If you want to use MemoryManager with your class make sure you have this field
	 * in your class.
	 */
	private long nativeAdress = 0;
	
	/**
	 * We override finalize method, because we need to destroy native c++ object when
	 * there is not more reference to Java object. This method is called by default
	 * by garbage collector.
	 */
	@Override
	protected void finalize() throws Throwable
	{
		nativeFinalize();
		super.finalize();
	}
	
	public Vector(int size)
	{
		construct(size);
	}
	
	public Vector(Vector vec)
	{
		copyConstruct(vec);
	}
	
	// Native stuff - C/C++
	public native int   getSize();
	public native float get(int id);
	
	public native void set(int id, float value);
	
	public native void multipleByScalar(float scalar);
	
	public native String toString();
	
	private native void construct(int size);
	private native void copyConstruct(Vector vec);
	private native void nativeFinalize();
}
