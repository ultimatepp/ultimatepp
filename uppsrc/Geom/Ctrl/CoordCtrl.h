#ifndef _Geom_Ctrl_CoordCtrl_h_
#define _Geom_Ctrl_CoordCtrl_h_

NAMESPACE_UPP

class EditDegree : public EditValue<double, ConvertDegree>
{
public:
	typedef EditDegree CLASSNAME;
	EditDegree();

	virtual bool  Key(dword key, int repcnt);

private:
	void          OnInc();
	void          OnDec();

private:
	SpinButtons   spin;
};

END_UPP_NAMESPACE

#endif
