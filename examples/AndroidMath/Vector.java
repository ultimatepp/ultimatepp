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
	 * In order to avoid memory leaks on the native side. The cleaning method needs to be
	 * called.
	 *
	 * In previous version of this example finalize method was used. However, in Java 9 the
	 * finalize method was deprecated and we can not longer relay on them.
	 */
	protected void dispose()
	{
		nativeDispose();
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
	private native void nativeDispose();
}
