#ifndef STDAPP_SPLASH_HPP
#define STDAPP_SPLASH_HPP

#include <stdapp/stdappDef.hpp>

#define IMAGECLASS UPP_LogoImg
#define IMAGEFILE <stdapp/UPP_Logo_033.iml>
#include <Draw/iml_header.h>

class Splash : public Ctrl
{
private:
    Array<Ctrl> widgets;

    void CloseSplash();
    Size MakeSplash(Ctrl& parent, Array<Ctrl>& ctrl);

    typedef Splash CLASSNAME;

public:
    Splash(const int language, const int ms);
};

#endif
