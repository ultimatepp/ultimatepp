# Ultimate++

Ultimate++ is a C++ cross-platform rapid application development framework focused on programmers productivity. It includes a set of libraries (GUI, SQL, etc.), and an integrated development environment.

Rapid development is achieved by the [smart and aggressive use of C++](https://www.ultimatepp.org/www$uppweb$overview$en-us.html) rather than through fancy code generators. In this respect, U++ competes with popular scripting languages while preserving C/C++ runtime characteristics.

The U++ integrated development environment, TheIDE, introduces modular concepts to C++ programming. It features BLITZ-build technology to speedup C++ rebuilds up to 4 times, Visual designers for U++ libraries, a [Topic++](https://www.ultimatepp.org/app$ide$Topic$en-us.html) system for documenting code and creating rich text resources for applications (like help and code documentation) and [Assist++](https://www.ultimatepp.org/app$ide$Assist$en-us.html) - a powerful C++ code analyzer that provides features like code completion, navigation and transformation.

TheIDE can work with GCC, MinGW and Visual C++ and contains a full featured debugger. TheIDE can also be used to develop non-U++ applications.

U++ supports following platforms on the production level: **Windows**, **macOS**, **GNU/Linux** & **FreeBSD**.

## What you can get with Ultimate++?

* A very effective C++ library for cross-platform development in source form.
* A good integrated development environment, designed for developing large C++ applications.

You can use both, or you can use whichever you need.

## License
U++ uses BSD license.

## Download

Main downloads:
* [Latest stable release](https://sourceforge.net/projects/upp/files/latest/download) - starts download with automatically detected platform

Other downloads:
* [Stable releases](https://sourceforge.net/projects/upp/) - all stable releases including historical ones
* [Nightly build](https://www.ultimatepp.org/www$uppweb$download$en-us.html) - latest unstable release of U++ (might be unstable)

## U++ Resources

More information about the framework, can be found on the official [site](https://ww.ultimatepp.org). Don't forget to check our rich [documentation](https://www.ultimatepp.org/www$uppweb$documentation$en-us.html).

## Examples

### GUI

Below is the code of trivial GUI application that displays "Hellow World" string inside window:

```
#include <CtrlLib/CtrlLib.h>

class MyAppWindow : public Upp::TopWindow {
public:
    MyAppWindow() {
        Title("My application").Zoomable().Sizeable();
    }

    virtual void Paint(Upp::Draw& w) override {
        w.DrawRect(GetSize(), Upp::SWhite);
        w.DrawText(20, 20, "Hello world!", Upp::Arial(30), Upp::Magenta);
    }
};

GUI_APP_MAIN
{
    MyAppWindow app;
    app.SetRect(0, 0, 200, 100);
    app.Run();
}
```

### Additional examples

More examples you can find directly on special section of U++ framework website deticated to [examples](https://www.ultimatepp.org/www$uppweb$examples$en-us.html). Moreover, exactly the same examples can be find in **examples** and **references** directories located in this repository.

# Repository

## Repository layout

The U++ repository is divided into several directories. The most important ones are:
* examples - examplary code of complex applications
* references - referential implementation
* **uppsrc** - contains main sources of the framework
