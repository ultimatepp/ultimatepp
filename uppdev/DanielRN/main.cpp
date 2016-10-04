#include <CtrlLib/CtrlLib.h>
//#include "ArrayCtrl/ArrayCtrl.h"

const int test = 0;  //change this to 0 for tab test

struct App : public TopWindow
{
    public:
        ArrayCtrl arr1;
        ArrayCtrl arr2;
        ArrayCtrl arr3;
        TabCtrl tab;

        App()
        {
            Title("Test");

            tab.HSizePos(0,0);
            tab.VSizePos(0,0);

            arr1.HSizePos(0,0);
            arr1.VSizePos(0,0);
            arr1.HeaderObject().Absolute();
            arr1.HeaderObject().Track();

            arr2.HSizePos(0,0);
            arr2.VSizePos(0,0);
            arr2.HeaderObject().Proportional();
            arr2.HeaderObject().Track();

            arr3.HSizePos(0,0);
            arr3.VSizePos(0,0);
            arr3.HeaderObject().ReduceNext();
            arr3.HeaderObject().Track();


            const int fx = 30;
            const int fy = 140;

            for (int i = 0; i < fx; i++)
            {
                arr1.AddColumn(AsString(i), 100).HeaderTab().Min(50);
                arr2.AddColumn(AsString(i), 100).HeaderTab().Min(50);
                arr3.AddColumn(AsString(i), 100).HeaderTab().Min(50);
            }

            for (int i = 0; i < fx; i++)
                for (int j = 0; j < fy; j++)
                {
                    String str = "Value " + AsString(j) + "." + AsString(i);
                    arr1.Set(j, i, str);
                    arr2.Set(j, i, str);
                    arr3.Set(j, i, str);
                }


            if (test == 0)
            {
                Add(tab);
                tab.Add(arr1, "Absolute");
                tab.Add(arr2, "Proportional");
                tab.Add(arr3, "ReduceNext");
            }
            else
            {
                Add(arr1);
            }

            Sizeable().Zoomable();
        }

    virtual bool Key(dword key, int count)
    {
        //LOG(Format("Key(%x, %d) ", key, count) + GetKeyDesc(key));
        return false;
    }

};

GUI_APP_MAIN
{
    App().Run();
}
