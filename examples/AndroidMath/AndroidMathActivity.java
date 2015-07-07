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
   		
   		text += "Vec1 Size: " + Integer.toString(vec1.getSize()) + "\n";
   		text += "Vec1: " + vec1.toString() + "\n";
   		
   		tv.append(text);
   	}
}
