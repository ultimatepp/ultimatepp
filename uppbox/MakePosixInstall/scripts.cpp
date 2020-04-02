const char *install_script =
R"--(#!/usr/bin/env bash

AskContinue()
{
  read -p "Continue (y/n)?" CONT
  if ! [ "$CONT" = "y" ]; then
	 exit;
  fi
}

if [ -x "$(command -v apt-get)" ]; then
  echo 'Debian packaging system (apt-get) detected'
  DEP="apt-get -y install g++ clang make libgtk-3-dev libnotify-dev libbz2-dev libssl-dev"
fi

if [ -x "$(command -v yum)" ]; then
  echo 'Fedora packaging system (yum) detected'
  DEP="yum -y install gcc-c++ clang make libgtk+3.0-devel pango-devel atk-devel cairo-devel libnotify-devel bzip2-devel xorg-x11-server-devel freetype-devel"
fi

if [ -x "$(command -v dnf)" ]; then
  echo 'Fedora packaging system (dnf) detected'
  DEP="dnf -y install gcc-c++ clang make libgtk+3.0-devel pango-devel atk-devel cairo-devel libnotify-devel bzip2-devel xorg-x11-server-devel freetype-devel"
fi

if [ -x "$(command -v zypper)" ]; then
  echo 'Zypper packaging system (dnf) detected'
  DEP="sudo zypper -n install gtk3-devel libbz2-devel ne xorg-x11-server-devel ssl-devel gcc-c++ make libXdmcp-devel clang"
fi

#add more packaging systems / distros here

if [ -z "$DEP" ]; then
  echo Packaging system was not identified.
  echo Automatic dependency instalation has failed.
  AskContinue
else
  echo We will now try to install required packages.
  echo Please enter your password so that we can do:
  echo sudo $DEP

  if ! eval 'sudo $DEP'; then
     echo Failed to install all required packages
     AskContinue
  fi
fi

if ./umks32; then
  echo Using umks32 to build
  UMK="./umks32"
else
  echo umks32 not available, building umk using make
  make -f uMakefile -j 4
  UMK="./umk"
fi

$UMK ./uppsrc ide CLANG -brs ./theide
$UMK ./uppsrc umk CLANG -brs ./umk

echo Install process has been finished. Do you want to start theide now?
AskContinue

./theide
)--";

const char *clean_script =
R"--(#!/usr/bin/env bash
#!/usr/bin/env bash

rm -rf .config/*
rm -rf .cache/*
rm theide
rm umk
)--";
