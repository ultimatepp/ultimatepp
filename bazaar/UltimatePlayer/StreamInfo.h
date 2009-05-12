#ifndef _UltimatePlayer_StreamInfo_h_
#define _UltimatePlayer_StreamInfo_h_

class StreamInfo : public Ctrl
{
		WString name;
		String time;
		
		bool noname;

	public:
	
				
		StreamInfo();
		void KillName()
		{
			name = "Ultimate Player";
			noname = true;
			Refresh();
		}
		void SetName(WString &s)
		{
			name = s;
			noname = false;
			Refresh();
		}
		void SetTime(String &s)
		{
			time = s;
			noname = false;
			Refresh();
		}
		virtual void Paint(Draw &w);
	
};

#endif
