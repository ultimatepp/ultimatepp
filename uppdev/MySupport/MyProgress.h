#ifndef _MySupport_MyProgress_h_
#define _MySupport_MyProgress_h_

class MyProgress : public Progress {
  private:
  int ts, pp;
  
  public:  
  void  SetMax(int max);
  void  SetLev(int lev);

  bool	PartialProgress(int par, int tot);
  
  operator Gate2<int, int>() { return callback(this, &MyProgress::PartialProgress); }
  
  typedef MyProgress CLASSNAME;
  
};

#endif
