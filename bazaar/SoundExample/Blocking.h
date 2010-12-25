#ifndef _SoundExample_Blocking_h_
#define _SoundExample_Blocking_h_

//	float buf[1000];
//	SoundStream s;
//	s.OpenDefault(0,2);
//	for(int i=0;i<1000;i++){
//		buf[i]=(float)sin(M_PI*i/100.0);
//	}
//	s.Start();
//	for(int i=0;i<40;i++){
//		s.Write(buf,1000);
//	}

// Couple tricks borrowed from DlgCtrl.cpp
const int* Sin16();
inline int sin16(int i){return Sin16()[i & 65535];}

class Theremin:public StaticRect{
	volatile double f;
	volatile double A;
	bool play;
	SoundStream snd;
	float buf[40000];
public:
	typedef Theremin CLASSNAME;
	Theremin(){
		Color(Black());
	}
	void Beep(){
		static double s=1/655336.0;
		for(int i=0;i<40000;i++){
			buf[i]=(float)A*sin(M_PI*(1+10*f)*i/100.0);
		}
		snd.Write(buf,20000);
	}
	void MouseMove(Point p,dword){
		Size sz=GetSize();
		f=p.x/(double)sz.cx;
		A=1-p.y/(double)sz.cy;
		Color(HsvColorf(f,A,0.5));
		Sync();
	}
	void LeftDown(Point p,dword){
		Beep();
	}
	void MouseEnter(Point,dword){
		snd.OpenDefault();
		snd.Start();
	}
	void MouseLeave(){
		Color(Black());
		snd.Close();
	}
};

class Blocking:public WithBlockingLayout<ParentCtrl>{
	Theremin theremin;
public:
	typedef Blocking CLASSNAME;
	Blocking(){
		CtrlLayout(*this);
		Add(theremin.HSizePosZ(8,208).VSizePosZ(8,8));
		info<<="[1 Move your cursor above the area on the left to play a sound. This toy was originally inspired by russian musical instrument called Theremin, but the result sounds much worse than the real thing... && Disregard the clapping noises, they are artefacts caused by delay between emptiing buffer and next mouse move event. The only purpose of this tab is to show syntax of SoundStream. ]";
	}
};

#endif
