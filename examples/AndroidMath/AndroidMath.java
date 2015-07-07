package org.upp.AndroidMath;

public class AndroidMath
{
	private AndroidMath() {}
	
    // Native stuff - C/C++
    public static native int power(int number, int n);
    
    static {
        // In this place we are loading native libraries.
        // Native library always has upp package name!
    	System.loadLibrary("AndroidMath");
    }
}
