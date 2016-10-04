#ifndef _E011_sep1_h_
#define _E011_sep1_h_

unsigned int SEP1_ReadVar(unsigned int n);
unsigned int SEP1_ReadFlash(unsigned int n);
unsigned int SEP1_ReadFile(unsigned int n);
void SEP1_WriteVar(unsigned int f, unsigned int v);
void SEP1_WriteFlash(unsigned int f, unsigned int v);
void SEP1_WriteFile(unsigned int f, unsigned int v);

void SEP1_EscCmd(const String& s);

int	SEP1_GetMode();
void SEP1_PutMode(int m);

#endif
