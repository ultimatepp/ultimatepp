#include <CtrlLib/CtrlLib.h>
#include <Synth/Synth.h>

using namespace Upp;

const char *score_qsf =
	R"(
		#ORGAN !261.62:L250V65A70D40R40w1:L50V90f0.50A98R50w-4:V120D60S0B:V70f3w-4:w-4
		#TOMTOM !174.61:L2000V100r-8D40S0R35:V90f3r-2D50S0B:V80r2D55S0N:f5r-2:
		#PINGER !415.30:L250V100A98D40S0R40w3:L50V90f0.50A98R50w-4:V120D60S0B:w-4:w-4
		
		!loop !volume:0.85
		{ /8 $TOMTOM  -5757 5777 5757 5777 5757577757575777 5757 5777 5757 5777 5757577757575777 }
		{ /8 $TOMTOM  -/64_ /8 5757 5777 5757 57 /16 57 57 /8 5757 5777 5757 57 /16 57 57  /8 5757 5777 5757 57 /16 57 57 /8 5757 5777 5757 57 /16 57 57 }
		{ /8 $PINGER  0_3_2_3_ 0_3_2_3_ 003_2_33 5_8_7_55 5875 5875 0323 0323 5875 ____ +3&8&7 5&8&7 0&8&7 _ }
		{ $ORGAN      $4(0&5&7 0&5&9) }
	)";


GUI_APP_MAIN
{
	InitSoundSynth();

	SoundSequence s = ParseQSF(score_qsf);
	PlaySequence(s);

	TopWindow().Run();

	CloseSoundSynth();
}
