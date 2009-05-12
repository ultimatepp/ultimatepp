#ifndef _UltimatePlayer_ProgressButton_h_
#define _UltimatePlayer_ProgressButton_h_

class ProgressButton : public Ctrl
{
	public:
	
		bool shaded;
		bool immediate;
		Color src;
		Color dst;
			
		float pos;
		String text;
		
		typedef ProgressButton CLASSNAME;
		
		ProgressButton();
	
		virtual void Paint(Draw &w);
		virtual void LeftDown(Point p, dword keyflags);
		virtual void LeftUp(Point p, dword keyflags);
		virtual void MouseMove(Point p, dword keyflags);
		
		void SetPos(float p);
		float  GetPos();
		
		Callback WhenLeftUp;
};


#endif
