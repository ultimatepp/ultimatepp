#ifndef _UltimatePlayer_StreamCtrl_h_
#define _UltimatePlayer_StreamCtrl_h_

class StreamCtrl : public Ctrl
{
	public:
	
		bool shaded;
		bool immediate;
		Color src;
		Color dst;
			
		float pos;
		String postext;
		String bitrate;		
		String name;
		
		int time;
		int time_total;
		int time_direction;
		
		bool noname;
		
		int progress_height;
		int info_width;
		
		void KillName()
		{
			name = "Ultimate Player";
			noname = true;
			Refresh();
		}
		void SetName(String &s)
		{
			name = s;
			noname = false;
			Refresh();
		}

		void KillTime()
		{
			time = 0;
			Refresh();
		}

		void SetTime(int ms, int ms_total)
		{
			time = ms;
			time_total = ms_total;
			noname = false;
			Refresh();
		}
		
		void SetBitRate(int br)
		{
			bitrate = AsString(br) + " kbps";
			Refresh();
		}
				
		typedef StreamCtrl CLASSNAME;
		
		StreamCtrl();
	
		virtual void Paint(Draw &w);
		virtual void LeftDown(Point p, dword keyflags);
		virtual void LeftUp(Point p, dword keyflags);
		virtual void MouseMove(Point p, dword keyflags);
		
		void   SetPos(float p);
		float  GetPos();
		
		Callback WhenLeftUp;
};

#endif
