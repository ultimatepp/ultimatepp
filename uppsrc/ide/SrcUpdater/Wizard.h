#ifndef _SourceUpdater_Wizard_h_
#define _SourceUpdater_Wizard_h_

#include <CtrlLib/CtrlLib.h>
namespace Upp {

struct NullStep : public ParentCtrl {
	static Size GetLayoutSize() {
		return Ctrl::LayoutZoom(1,1);
	}
};

inline
void InitLayout(Upp::Ctrl& parent, NullStep& layout, NullStep& uts, NullStep&){
	parent.LayoutId("NullStepLayout");
};

//$-
template < class T0, class T1,
           class T2 = NullStep, class T3 = NullStep, class T4 = NullStep, class T5 = NullStep,
           class T6 = NullStep, class T7 = NullStep, class T8 = NullStep, class T9 = NullStep
         >
//$+
//$ template<class T0,class T1,class T2 = NullStep,...>
class Wizard : public TopWindow {
	int step,count;
	Buffer<bool> allowed;
public:
	typedef Wizard CLASSNAME;
	T0 s0; T1 s1; T2 s2; T3 s3; T4 s4; T5 s5; T6 s6; T7 s7; T8 s8; T9 s9;
	Button cancel,prev,next,finish;
	Event<>  WhenFinish;
	Event<>  WhenCancel;
	Event<int> WhenStep;
	Wizard();
	void NextStep();
	void PrevStep();
	void SetStep(int n);
	void Block(int n,bool block=true) {allowed[n]=!block; UpdateButtons();}
	void Unblock(int n)               {allowed[n]=true; UpdateButtons();}
	bool IsBlocked(int n)             {return !allowed[n];}
	int GetCount()                    {return count;}
private:
	void CountStep(const NullStep&)             {}
	template<class T> void CountStep(const T&)  {count++;}
	void AddStep(Ctrl& sN);
	bool IsFirst(int step);
	bool IsLast(int step);
	void SetVisibility(Ctrl& sN,int n);
	void UpdateButtons();
	void DoStep();
	void PerformChanges();
	void Cancel();
};

//$-
//just a simple typing savers ...
#define MAP_FN_ON_STEPS(FN) \
	FN(s0); FN(s1); FN(s2); FN(s3); FN(s4); FN(s5); FN(s6); FN(s7); FN(s8); FN(s9);
#define MAP_FN2_ON_STEPS(FN) \
	FN(s0,0); FN(s1,1); FN(s2,2); FN(s3,3); FN(s4,4); FN(s5,5); FN(s6,6); FN(s7,7); FN(s8,8); FN(s9,9);
#define WIZMEM(x) \
template <class T0,class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9> x Wizard<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>
//$+

template <class T0,class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9>\
Wizard<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>::Wizard():step(0),count(0){
	SetRect(0,0, HorzLayoutZoom(600),VertLayoutZoom(400));
	*this << cancel.SetLabel(t_("Cancel")).RightPosZ(244, 72).BottomPosZ(4, 24)
	      << prev.SetLabel(String("< ")+t_("Previous")).RightPosZ(164, 72).BottomPosZ(4, 24)
	      << next.SetLabel(t_("Next")+String(" >")).RightPosZ(84, 72).BottomPosZ(4, 24)
	      << finish.SetLabel(t_("Finish")).RightPosZ(4, 72).BottomPosZ(4, 24);
	finish.Ok()<<=Acceptor(IDOK);
	cancel.Cancel()<<=Rejector(IDCANCEL);
	MAP_FN_ON_STEPS(CtrlLayout);
	MAP_FN_ON_STEPS(CountStep);
	MAP_FN_ON_STEPS(AddStep);
	allowed=Buffer<bool>(count,true);
	next<<=THISBACK(NextStep);
	prev<<=THISBACK(PrevStep);
	cancel<<THISBACK(Cancel);
	finish<<THISBACK(PerformChanges);
	next.SetFocus();
	DoStep();
}
WIZMEM(void)::NextStep(){
	while(!allowed[++step]){}
	DoStep();
}
WIZMEM(void)::PrevStep(){
	while(!allowed[--step]){}
	DoStep();
}
WIZMEM(void)::SetStep(int n){
	ASSERT(n<count);
	step=n;
	DoStep();
}
WIZMEM(void)::AddStep(Ctrl& sN){
	Add(sN.HSizePosZ(4,4).VSizePosZ(4,28));
}
WIZMEM(bool)::IsFirst(int step){
	if(step==0) return true;
	for(int i=step-1; i>=0; i--)
		if(allowed[i]) return false;
	return true;
}
WIZMEM(bool)::IsLast(int step){
	if(step==count-1) return true;
	for(int i=step+1; i<count; i++)
		if(allowed[i]) return false;
	return true;
}
WIZMEM(void)::SetVisibility(Ctrl& sN,int n){
	sN.Show(step==n);
}
WIZMEM(void)::UpdateButtons(){
	prev.Enable(!IsFirst(step));
	next.Enable(!IsLast(step));
}
WIZMEM(void)::DoStep(){
	WhenStep(step);
	UpdateButtons();
	MAP_FN2_ON_STEPS(SetVisibility);
}
WIZMEM(void)::PerformChanges(){
	Hide();
	WhenFinish();
	Close();
}
WIZMEM(void)::Cancel(){
	Hide();
	WhenCancel();
	Close();
}

#undef WIZMEM
#undef MAP_FN_ON_STEPS
#undef MAP_FN2_ON_STEPS

};
#endif
