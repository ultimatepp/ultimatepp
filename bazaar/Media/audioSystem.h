#ifndef _Media_audioSystem_h_
#define _Media_audioSystem_h_

#define MAX_MAINVOLUME	10000

bool GetMainVolume(int &left, int &right);
int GetMainVolume();
bool SetMainVolume(int left, int right);
bool SetMainVolume(int vol);

#endif
