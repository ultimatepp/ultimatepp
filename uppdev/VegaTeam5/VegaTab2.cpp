#include "VegaMain.h"
#include "Tournament.h"


VegaTab2::VegaTab2()
{
	CtrlLayout(*this);
	SizePos();
	              //id, label, width
	arrp.AddColumn("Board", "Board", 4) ;   //0
	arrp.AddColumn("White", "White Player", 25);         //1
	arrp.AddColumn("Black", "Black Player", 25);         //1
	arrp.AddColumn("ID", "IDW-IDB", 8);       //3
	arrp.EvenRowColor();
	arrp.SetLineCy(19); //option image adjustment...
}



void VegaTab2::ShowPairingCB()
{ int i, npair, code;
  int idw, idb, res;
  arrp.Clear();
  if (TD.IS_SWISS) npair = TD.pairing[0][TD.currentRound];
  else npair = TD.pairingRR[0][TD.currentRound];
  for (i=1; i<=npair; i++)
      { if (TD.IS_SWISS) code = TD.pairing[i][TD.currentRound];
        else code = TD.pairingRR[i][TD.currentRound];
        TD.DecodeResult(code, &idw, &idb, &res);
        arrp.Add( AsString(i),
                NFormat("%-25.25s  (%4.1f)", player[idw].name, 0.),
                NFormat("%-25.25s  (%4.1f)", player[idb].name, 0.),
                NFormat("%3d - %3d", idw, idb));
	    arrp.GoEnd();
      }
}
