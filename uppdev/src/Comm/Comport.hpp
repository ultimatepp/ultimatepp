#ifndef _COMPORT_
#define _COMPORT_

#ifndef _WINDOWS_
   #define WIN32_LEAN_AND_MEAN
   #include <windows.h>
#endif

#ifndef _STDEXCEPT_
   #include <stdexcept>
#endif

class COMPort {
public:
		bool open;

    enum Parity {
         None = 0
       , Odd
       , Even
       , Mark
       , Space
    };

    enum DataBits {
         db4 = 4
       , db5
       , db6
       , db7
       , db8
    };

    enum StopBits {
       sb1 = 0,
       sb15,
       sb2
    };

    enum BitRate {
       br110 = 110,
       br300 = 300,
       br600 = 600,
       br1200 = 1200,
       br2400 = 2400,
       br4800 = 4800,
       br9600 = 9600,
       br19200 = 19200,
       br28800 = 28800,
       br38400 = 38400,
       br56000 = 56000,
       br57600 = 57600,
       br115200 = 115200,
       br256000 = 256000
    };

    // for function getModemSignals
    struct MSPack {
      unsigned char DTR : 1;
      unsigned char RTS : 1;
      unsigned char     : 2;
      unsigned char CTS : 1;
      unsigned char DSR : 1;
      unsigned char RI  : 1;
      unsigned char DCD : 1;
    };

    // for function clearError
    struct errorStatus {
        COMSTAT comStat;
    	DWORD   dwErrors;
    };

private:

   // disable copy constructor and assignment operator
   COMPort (const COMPort &);
   COMPort& operator= (const COMPort &);

   void getState () const;
   COMPort& setState ();

   unsigned portHandle;
   char * dcb;

protected:

public:

    COMPort& connect( const char * const portName, unsigned long baudRate, Parity parity, DataBits bytesize, StopBits stopbits );
    COMPort& disconnect();
    char read ();
    COMPort & write (const char inChar);
    unsigned long readString  ( void *, const unsigned long count );
    unsigned long writeString ( const void *, const unsigned long count );
    COMPort& setBitRate ( unsigned long Param );
    unsigned long bitRate () const;
    COMPort& setParity ( Parity Param );
    Parity parity () const;
    COMPort& setDataBits ( DataBits Param );
    DataBits dataBits () const;
    COMPort& setStopBits ( StopBits Param );
    StopBits stopBits () const;
    COMPort & setHandshaking ( bool inHandshaking = true );
    COMPort& setLineCharacteristics ( char * Param );
    unsigned long getMaximumBitRate () const;
    COMPort & setxONxOFF ( bool Param = true);
    bool isxONxOFF () const;
    MSPack getModemSignals () const;
    COMPort& setBlockingMode ( unsigned long inReadInterval  = 0, unsigned long inReadMultiplyer = 0, unsigned long inReadConstant = 0 );
    errorStatus clearError() const;
    bool Setup(DWORD dwInQueue, DWORD dwOutQueue);
    bool Purge(DWORD dwFlags);
    bool ClearWriteBuffer();
    bool ClearReadBuffer();
    bool Flush();

    bool IsOpen()							{ return open; }

    COMPort ();
    ~COMPort ();

};

#endif
