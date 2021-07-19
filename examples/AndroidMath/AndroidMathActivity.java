package org.upp.AndroidMath;

import android.app.Activity;
import android.widget.TextView;
import android.widget.ScrollView;
import android.os.Bundle;

public class AndroidMathActivity extends Activity
{
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		
		ScrollView scroller = new ScrollView(this);
		TextView tv = new TextView(this);
		
		calculatePowerOperations(tv, 20);
		calculateVectorOperations(tv);
		
		scroller.addView(tv);
		setContentView(scroller);
	}

	private void calculatePowerOperations(TextView tv, int maxPower)
	{
		String text = "Power:\n";
		int maxNumber = maxPower;
		for(int i = 1; i <= maxNumber; i++) {
			int pow = AndroidMath.power(i, 2);
			text += Integer.toString(i) + "^2 = " + Integer.toString(pow) + "\n";
		}
		
		tv.append(text);
	}
	
	private void calculateVectorOperations(TextView tv)
	{
		String text = "Vector operations:\n";
		
		Vector vec1 = new Vector(3);
		vec1.set(0, 3.0f);
		vec1.set(1, -1.0f);
		vec1.set(2, 5.0f);
		
		text += "Vec1: " + vec1.toString() + "\n";
		
		Vector vec1Copy = new Vector(vec1);
		vec1Copy.multipleByScalar(5.0f);
		text += "3 * Vec1: " + vec1Copy.toString() + "\n";
		
		Vector vec2 = new Vector(4);
		vec2.set(0, -2.0f);
		vec2.set(1, 4.0f);
		vec2.set(2, 7.0f);
		vec2.set(3, -9.0f);
		
		text += "Vec2: " + vec2.toString() + "\n";
		
		tv.append(text);
	}
	
	static {
		// For Android version less than 5.0 we need to specific standard library
		// If you have a crash uncomment library that you specific in your Android Builder
		// specification.
		
		// System.loadLibrary("stlport_shared");
		// System.loadLibrary("gnustl_shared");
		// System.loadLibrary("c++_shared");
		
		// In this place we are loading native libraries (In revers dependency order).
		// IMPORTANT: Native library always has upp package name!!!
		System.loadLibrary("AndroidMathUtility");
		System.loadLibrary("AndroidMath");
	}
}
