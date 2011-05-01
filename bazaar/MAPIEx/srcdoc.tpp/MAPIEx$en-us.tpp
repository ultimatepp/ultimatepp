topic "General description";
[ $$0,0#00000000000000000000000000000000:Default]
[a83;*R6 $$1,0#31310162474203024125188417583966:caption]
[{_}%EN-US 
[s1; [+184 MAPIEx]&]
[s0; [2 MAPIEx handles Microsoft Outlook elements as messages, folders, 
contacts and appointments through Extended MAPI (Message API).]&]
[s0;2 &]
[s0; [2 With MAPIEx you can check email folders, open and save emails 
including images and attachments, create contacts or delete emails 
and folders. Many simple examples are included in MAPIEx`_demo 
package.]&]
[s0;2 &]
[s0; [2 MAPIEx is a wrapper of ][^http`:`/`/www`.mud`.ca`/MAPIEx`.zip^2 Noel 
Dillabough CMAPIEx library][2  It contains code from ][^http`:`/`/www`.wischik`.com`/lu`/programmer`/mapi`_utils`.html^2 L
ucian Wischik MAPI Utils][2 .]&]
[s0;2 &]
[s0; [2 The main changes in relationship with original code have been:]&]
[s0;2 &]
[s0;i150;O0; [2 `"new`" and `"delete`" are not necessary in user program. 
No heap errors are expected as now all user variables are in 
the stack. In addition internal use of heap has been reduced.]&]
[s0;i150;O0; [2 The library interface from user point of view has been 
simplified. GetContents() use is now internal.]&]
[s0;i150;O0; [2 To improve compatibility CString and TCHAR have been 
removed. They have been substituted by String.]&]
[s0;i150;O0; [2 Added documentation.]&]
[s0;2 &]
[s0;2 &]
[s0; [2 MAPIEx classes are:]&]
[s0;2 &]
[s0;i150;O0; [^topic`:`/`/MAPIEx`/src`/MAPIEx`$en`-us^2 MAPIEx]&]
[s0;i150;O0; [^topic`:`/`/MAPIEx`/src`/MAPIFolder`$en`-us^2 MAPIFolder]&]
[s0;i150;O0; [^topic`:`/`/MAPIEx`/src`/MAPIMessage`$en`-us^2 MAPIMessage]&]
[s0;i150;O0; [^topic`:`/`/MAPIEx`/src`/MAPIAppointment`$en`-us^2 MAPIAppoin][^topic`:`/`/Functions4U`/src`/SvgColors`$en`-us^2 t
ment]&]
[s0;i150;O0; [^topic`:`/`/MAPIEx`/src`/MAPIContact`$en`-us^2 MAPIContact]&]
[s0;2 &]
[s0;2 &]
[s0; [2 As MAPIEx uses internal Microsoft infrastructure, it can only 
work in Windows.]&]
[s0;2 &]
[s0; [2 There is also a ][^topic`:`/`/MAPIEx`/srcdoc`/ChangesLog`$en`-us^2 News 
and changes log][2 , ][^topic`:`/`/MAPIEx`/srcdoc`/License`$en`-us^2 License][2  
and a ][^topic`:`/`/MAPIEx`/srcdoc`/ToDo`$en`-us^2 ToDo list][2 .]&]
[s0;2 ]