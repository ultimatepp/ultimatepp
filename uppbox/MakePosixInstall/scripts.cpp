const char *install_script =
R"--(#!/usr/bin/env bash

AskContinue()
{
  read -p "Continue (Y/n)?" answer
  if [ "$answer" != "${answer//[nN]/x}" ]; then
	 exit;
  fi
}

if [ -x "$(command -v apt-get)" ]; then
  echo 'Debian packaging system (apt-get) detected'
  DEP="apt-get install g++ clang make libgtk-3-dev libnotify-dev libbz2-dev libssl-dev"
fi

if [ -x "$(command -v yum)" ]; then
  echo 'Fedora packaging system (yum) detected'
  DEP="yum install gcc-c++ clang make gtk3-devel libnotify-devel bzip2-devel freetype-devel openssl-devel"
fi

if [ -x "$(command -v dnf)" ]; then
  echo 'Fedora packaging system (dnf) detected'
  DEP="dnf install gcc-c++ clang make gtk3-devel libnotify-devel bzip2-devel freetype-devel openssl-devel"
fi

if [ -x "$(command -v urpmi)" ]; then
  echo 'Mandriva packaging system (urpmi) detected'
  DEP="urpmi install gcc-c++ clang make gtk3-devel libnotify-devel bzip2-devel freetype-devel openssl-devel"
fi

if [ -x "$(command -v zypper)" ]; then
  echo 'Zypper packaging system (zypper) detected'
  DEP="sudo zypper install clang make gtk3-devel libnotify-devel bzip2-devel freetype-devel openssl-devel"
fi

#add more packaging systems / distros here

if [ -z "$DEP" ]; then
  echo Packaging system was not identified.
  echo Automatic dependency instalation has failed.
  echo You will have to install required packages manually.
  echo Please make sure that build dependecies are satisfied.
  AskContinue
else
  echo Following command should be used to install required packages:
  echo
  echo sudo $DEP
  echo
  echo Install script can run this command for you, but that will require
  echo your sudo password.
  read -p "Do you want the script to do that (Y/n)?" answer
  if [ "$answer" == "${answer//[nN]/x}" ]; then
     if ! eval 'sudo $DEP'; then
        echo Failed to install all required packages.
        echo You will have to install required packages manually.
        AskContinue
     fi
  else
     echo Please make sure that build dependecies are satisfied.
     AskContinue
  fi
fi

if ./umks32 >/dev/null; then
  echo
  read -p "Use prebuilt binary umks32 to accelerate the build (Y/n)?" answer
  if [ "$answer" == "${answer//[nN]/x}" ]; then
    echo Using umks32 to build
    UMK="./umks32"
  fi
fi

if [ -z "$UMK" ]; then
  echo umks32 cannot be used, building umk using make
  make -f uMakefile -j 4
  UMK="./umk"
fi

$UMK ./uppsrc ide CLANG -brs ./theide
$UMK ./uppsrc umk CLANG -brs ./umk

if [ -x ./theide ]; then
  echo Install process has been finished, TheIDE is built as ./theide
  read -p "Do you want to start TheIDE now? (Y/n):" answer
  if [ "$answer" == "${answer//[nN]/x}" ]; then
     ./theide
  fi
else
  echo Something went wrong.
  echo Please use 'make' to compile theide and/or notify developers.
fi

)--";

const char *clean_script =
R"--(#!/usr/bin/env bash
#!/usr/bin/env bash

rm -rf .config/*
rm -rf .cache/*
rm -f theide
rm -f umk
)--";
