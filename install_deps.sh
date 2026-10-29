#!/usr/bin/env bash

for arg in "$@"; do
  if [ "$arg" = "-y" ] || [ "$arg" = "--non-interactive" ]; then
    NON_INTERACTIVE=true
  fi
done

AskContinue()
{
  read -p "Continue (Y/n)? " answer
  if [ "$answer" != "${answer//[nN]/x}" ]; then
    exit 1;
  fi
}

UNAME=$(uname)

if [ "$UNAME" = 'Darwin' ]; then
  if ! command -v brew &> /dev/null; then
    echo "Homebrew is not installed"
    echo "We use Homebrew to obtain OpenSSL libraries and clang-format required for U++"
    echo "Follow the instructions at https://brew.sh/ to install Homebrew"
    exit 1
  fi
  echo "Homebrew is installed"
  brew install libpng lz4 pcre2 xxhash zstd
  if clang++ --version >/dev/null 2>&1; then
    echo "Commandline Development Tools already installed"
  else
    echo "Please install the Commandline Development Tools with GUI."
    echo "Please wait until the installation is finished."
    AskContinue
  fi
  exit 0
fi

APT_OPT=""
DNF_OPT=""
YUM_OPT=""
URPMI_OPT=""
ZYPPER_OPT=""
PACMAN_OPT="-Sy"
PKG_OPT=""

if [ "$NON_INTERACTIVE" = "true" ]; then
  APT_OPT="-y"
  DNF_OPT="-y"
  YUM_OPT="-y"
  URPMI_OPT="--auto"
  ZYPPER_OPT="--non-interactive"
  PACMAN_OPT="-Sy --noconfirm"
  PKG_OPT="-y"
fi

DEP=""
if command -v apt-get >/dev/null; then
  DEP="apt-get install $APT_OPT g++ clang clang-format git make xdotool libbz2-dev libexpat1-dev libfontconfig1-dev libfreetype-dev libgif-dev libgtk-3-dev libjpeg-turbo8-dev liblz4-dev liblzma-dev libnotify-dev libpcre2-dev libpng-dev libssl-dev libx11-dev libxcb1-dev libxdmcp-dev libxext-dev libxft-dev libxinerama-dev libxrender-dev libxxhash-dev libzstd-dev zlib1g-dev"
elif command -v dnf >/dev/null; then
  DEP="dnf install $DNF_OPT gcc-c++ clang clang-tools-extra git make gtk3-devel libnotify-devel bzip2-devel freetype-devel openssl-devel"
elif command -v yum >/dev/null; then
  DEP="yum install $YUM_OPT gcc-c++ clang clang-tools-extra git make gtk3-devel libnotify-devel bzip2-devel freetype-devel openssl-devel"
elif command -v urpmi >/dev/null; then
  DEP="urpmi $URPMI_OPT gcc-c++ clang clang-tools-extra git make gtk3-devel libnotify-devel bzip2-devel freetype-devel openssl-devel"
elif command -v zypper >/dev/null; then
  DEP="zypper $ZYPPER_OPT install gcc-c++ clang git make gtk3-devel libnotify-devel bzip2-devel freetype-devel libopenssl-devel"
elif command -v pacman >/dev/null; then
  DEP="pacman $PACMAN_OPT --needed gcc clang git make xdotool bzip2 expat fontconfig freetype2 giflib gtk3 libjpeg-turbo lz4 xz libnotify pcre2 libpng openssl libx11 libxcb libxdmcp libxext libxft libxinerama libxrender xxhash zstd zlib pkgconf gdb" 
elif command -v pkg >/dev/null; then
  DEP="pkg install $PKG_OPT bash gmake git gtk3 libnotify llvm90 pkgconf"
  if [ "$UNAME" = 'SunOS' ]; then
     DEP="pkg install $PKG_OPT bash git gtk3 libnotify developer/clang-80 build-essential"
  fi
elif command -v pkg_add >/dev/null; then
  DEP="pkg_add bash git gmake gtk3 libnotify clang-devel"
fi

if [ -z "$DEP" ]; then
  if [ "$UNAME" = 'OpenBSD' ]; then
    echo "Automatic dependencies installation is not supported on OpenBSD."
    echo "See README for details."
  else
    echo "Packaging system was not identified."
    echo "Automatic dependency installation has failed."
    echo "You will have to install required packages manually."
  fi
  echo "Please make sure that build dependencies are satisfied."
  AskContinue
  exit 0
fi

echo "Following command should be used to install required packages:"
echo ""
echo "sudo $DEP"
echo ""

if [ "$NON_INTERACTIVE" = "true" ]; then
  echo "Non-interactive mode: updating apt package lists..."
  if command -v apt-get >/dev/null; then
    sudo apt-get update
  fi
  
  echo "Non-interactive mode: running installation automatically..."
  if ! sudo $DEP; then
     echo "Failed to install all required packages."
     exit 1
  fi
  exit 0
fi

echo "Install script can run this command for you, but that will require"
echo "your sudo password."
read -p "Do you want the script to do that (Y/n)? " answer

if [[ "$answer" =~ ^[nN] ]]; then
   echo "Please make sure that build dependencies are satisfied."
   AskContinue
   exit 0
fi

if ! sudo $DEP; then
   echo "Failed to install all required packages."
   echo "You will have to install required packages manually."
   AskContinue
fi
