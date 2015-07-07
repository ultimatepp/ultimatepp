package org.upp.AndroidMath;

/**
 * Class witch whole implementaiton is native.
 */
public class Vector
{
	public Vector(int size)
	{
		construct(size);
	}
	
	@Override
	public void finalize()
	{
		destroy();
	}
	
	// Native stuff - C/C++
	public native int   getSize();
	public native float get(int id);
	
	public native void set(int id, float data);
	
	public native String toString();
	
	private native void construct(int size);
	private native void destroy();
	
	static {
		System.loadLibrary("AndroidMath");
	}
}
