#ifndef _MySupport_Windows_h_
#define _MySupport_Windows_h_

// Functions
void DelKey(HKEY base_key, const char *dir, const char *key);
bool AssociateFileType(const String ext, const String desc, const String epp, const String icnf);
String GetExeFileTypeAssociation(const String ext);

#endif
