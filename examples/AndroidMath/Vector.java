package org.upp.AndroidMath;

/**
 * Class which whole funtional implementaiton is native.
 */
public class Vector
{
	public Vector(int size)
	{
		construct(size);
	}
	
	public Vector(Vector vec)
	{
		copyConstruct(vec);
	}
	
	/**
	 * We override finalize method due to possibilite of memory leaks.
	 */
	@Override
	protected void finalize() throws Throwable
	{
		destroy();
		super.finalize();
	}
	
	// Native stuff - C/C++
	public native int   getSize();
	public native float get(int id);
	
	public native void set(int id, float value);
	
	public native void multipleByScalar(float scalar);
	
	public native String toString();
	
	private native void construct(int size);
	private native void copyConstruct(Vector vec);
	private native void destroy();
	
	static {
		// In this place we are loading native libraries.
		// Native library always has upp package name!
		System.loadLibrary("AndroidMath");
	}
}
