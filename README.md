# U++

![alt text](uppbox/uppweb/Resources/Images/Logo.png?raw=true "U++ Logo")

[![CircleCI](https://dl.circleci.com/status-badge/img/gh/ultimatepp/ultimatepp/tree/master.svg?style=shield)](https://dl.circleci.com/status-badge/redirect/gh/ultimatepp/ultimatepp/tree/master)

U++ is a C++ cross-platform rapid application development framework focused on programmers productivity. It includes a set of libraries (GUI, SQL, etc.), and an integrated development environment.

Rapid development is achieved by the [smart and aggressive use of C++](https://www.ultimatepp.org/www$uppweb$overview$en-us.html) rather than through fancy code generators. In this respect, U++ competes with popular scripting languages while preserving C/C++ runtime characteristics.

The U++ integrated development environment, TheIDE, introduces modular concepts to C++ programming. It features BLITZ-build technology to speedup C++ rebuilds up to 4 times, Visual designers for U++ libraries, a [Topic++](https://www.ultimatepp.org/app$ide$Topic$en-us.html) system for documenting code and creating rich text resources for applications (like help and code documentation) and [Assist++](https://www.ultimatepp.org/app$ide$Assist$en-us.html) - a powerful C++ code analyzer that provides features like code completion, navigation and transformation.

TheIDE can work with GCC, Clang, MinGW and Visual C++ and contains a full featured debugger. TheIDE can also be used to develop non-U++ applications.

U++ supports following platforms on the production level: **Windows**, **macOS**, **GNU/Linux** & **FreeBSD**.

## What you can get with U++?

* A very effective C++ library for cross-platform development in source form.
* A good integrated development environment, designed for developing large C++ applications.

You can use both, or you can use whichever you need.

## License
U++ uses BSD license.

## Download

Main downloads:
* [Latest stable release](https://sourceforge.net/projects/upp/files/latest/download) - starts download with automatic platform detection

Other downloads:
* [Stable releases](https://sourceforge.net/projects/upp/) - all stable releases including historical ones
* [Nightly build](https://www.ultimatepp.org/www$uppweb$download$en-us.html) - latest unstable release of U++ (might be unstable)

## U++ Resources

More information about the framework, can be found on the official [site](https://www.ultimatepp.org). Don't forget to check our rich [documentation](https://www.ultimatepp.org/www$uppweb$documentation$en-us.html).

## Examples

### GUI

Below is the code of trivial GUI application that displays "Hello World" string inside window:

```c++
#include <CtrlLib/CtrlLib.h>

class MyApp: public Upp::TopWindow {
public:
    MyApp()
    {
        Title("My application").Zoomable().Sizeable().SetRect(0, 0, 320, 200);
    }

    virtual void Paint(Upp::Draw& w) override
    {
        w.DrawRect(GetSize(), Upp::SWhite);
        w.DrawText(10, 10, "Hello, world!", Upp::Arial(30), Upp::Magenta);
    }
};

GUI_APP_MAIN
{
    MyApp().Run();
}
```

### TheIDE

Standard part of U++ framework is integrated development environment, TheIDE.

<p align="center">
  <img alt="TheIDE - U++ Integrated Developemnt Enviroment" src="/uppbox/uppweb/Resources/Images/TheIDE.png" width="80%" height="80%">
</p>

### Additional examples

See here: [examples](https://www.ultimatepp.org/www$uppweb$examples$en-us.html). Moreover, exactly the same examples can be found in the **examples** and **references** directories located in this repository.

If you would like to see more screenshots, click [here](https://www.ultimatepp.org/www$uppweb$ss$en-us.html).

# Repository

## Repository layout

The U++ repository is divided into several directories. The most important ones are:
* **examples** - exemplary codes of complex applications
* **references** - reference examples demonstrating individual features of U++
* **uppsrc** - main sources of the framework
