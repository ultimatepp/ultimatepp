topic "Introduction to U++";
[ $$0,0#00000000000000000000000000000000:Default]
[=b133;*R5 $$1,2#50732784410233463530219518343157:Heading1]
[a17;*@(170.42.0) $$2,3#48345324464242285505540720043115:Heading2]
[2 $$3,3#42410412485283044863419059360682:Body1]
[{_}%EN-US 
[s0; &]
[s1; Introduction to U`+`+&]
[s0; &]
[s0; &]
[s2; Welcome to U`+`+.&]
[s3; U`+`+ is both an application framework (class library for Win32 
and Linux) as well as a complete development tool including RAD 
GUI designer, source editor, image editor, code documenting tool, 
and C`+`+ project builder.  It also provides a powerful `"C`-like`" 
scripting language (ESC) enabling you to provide end user extensibility 
and customization of your own applications, as well as extend 
TheIDE (the U`+`+ IDE)&]
[s3; &]
[s3; This topic describes the key concepts of U`+`+ to help you get 
started as quickly as possible.  If you want to try out U`+`+ 
before reading any further, you can build and run the HelloWorld 
example and read the HelloWorld tutorial [@(255.42.0) here (fix)].&]
[s0; &]
[s0; &]
[s2; [_ TheIDE]&]
[s3; Before you can run TheIDE, you must first choose the package 
and assembly you wish to work with using the `"Select`-main`-package`" 
dialog.  After an assembly and package have been selected, the 
package is opened in TheIDE.  The package that has been opened 
is referred to as the [* main package] and its name is shown in 
the title bar of the U`+`+ application window.  The uppermost 
pane at the left hand side of TheIDE shows the name of the main 
package followed by a list of the packages used by the main package. 
 The lower left`-hand pane lists the source files belonging to 
the package whose name is highlighted in the upper pane.  To 
select a different main package, the `"Set main package`" option 
from the File menu is used.  For more detail on using TheIDE, 
[^topic`:`/`/ide`/app`/Intro`_TheIDE`$en`-us^ see this][@(226.42.0) .]&]
[s0; &]
[s0; &]
[s2; Packages, assemblies and nests&]
[s3; Packages are centric to U`+`+.  An executable application is 
built from a package.  A package can also build into a dynamic 
link library, a static library, or a set of object files.  A 
package can be used by other packages.  A package corresponds 
to a single directory whose name is the name of the package. 
 TheIDE itself is a package and you can rebuild it from within 
TheIDE if you wish!&]
[s3; &]
[s3; An assembly can be thought of as a collection of packages but 
it is actually just a set of paths which determine where U`+`+ 
looks for the packages and source files needed when building 
one of the assembly packages.  The paths defined by the assembly 
are stored in an assembly definition file which has a .var extension 
and is stored in the U`+`+ root installation directory.  A package 
can `"appear`" in multiple assemblies.&]
[s3; &]
[s3; U`+`+ requires that packages be organized into nests.  A nest 
is actually just a directory containing a set of package directories, 
folders or files.  An assembly defines an ordered list of nests 
(paths) and the packages contained in those nests form the packages 
of the assembly.  For more detail on packages and assemblies 
see [^topic`:`/`/ide`/app`/PackagesAssembliesAndNests`$en`-us^ `"Packages, 
Assemblies and Nests`" ]and. [^topic`:`/`/ide`/app`/ConfiguringPackagesAssemblies`$en`-us^ `"
Creating and Configuring Assemblies and Packages`"]&]
[s0;_@(28.85.0) &]
[s0;_@(28.85.0) &]
[s2; RAD GUI designer&]
[s0; &]
[s0; &]
[s0; &]
[s2; The ESC macro language&]
[s0; &]
[s0; &]
[s2; The help system.&]
[s3; U`+`+ documentation is created with Topic`+`+.&]
[s3; &]
[s3; &]
[s0; &]
[s2; U`+`+ License&]
[s3; The U`+`+ license is BSD, allowing almost unrestricted use of 
the product and its source&]
[s3; code.  Full C`+`+ source code is provided for both TheIDE and 
the class framework,&]
[s3; ensuring both the future of U`+`+ and the ability to support 
and extend your &]
[s3; own applications.&]
[s0; &]
[s0; ]]