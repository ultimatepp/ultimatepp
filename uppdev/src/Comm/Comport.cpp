#ifndef _COMPORT_
   #include "ComPort.hpp"
#endif

//using namespace std;

COMPort::COMPort () : dcb (NULL) {
	dcb = new char [sizeof(DCB)];
	getState(); // Riempie il DCB con le impostazioni attuali della porta
	setBlockingMode();
	setHandshaking();
	open = false;
}

COMPort::~COMPort() {
	delete [] dcb;

	CloseHandle ((HANDLE)portHandle);
	open = false;
}

COMPort& COMPort::connect(const char * const portName, unsigned long _rate, Parity _parity, DataBits _bytesize, StopBits _stopbits ) {
	DCB & aDCB = *((LPDCB)dcb);

	if(!open) {
		portHandle = (unsigned) CreateFile ( portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL );
		if (portHandle == HFILE_ERROR) return *this;

		aDCB.fBinary = true;
		aDCB.fNull = false;
		aDCB.BaudRate = _rate;
		aDCB.Parity = _parity;
		aDCB.ByteSize = _bytesize;
		aDCB.StopBits = _stopbits;
		aDCB.fAbortOnError = true;

		open = true;
		return setState();
	}
	else return *this;
}

COMPort& COMPort::disconnect() {
  if(open) CloseHandle((HANDLE)portHandle);
  open = false;
}

void COMPort::getState () const {
	if (!GetCommState ( (HANDLE)portHandle, (LPDCB)dcb ) ) {
	}
}

COMPort& COMPort::setState () {
	if (!SetCommState ( (HANDLE)portHandle, (LPDCB)dcb ) ) {
	}

	return *this;
}

COMPort& COMPort::setBitRate ( unsigned long Param ) {
	DCB & aDCB = *((LPDCB)dcb);
	aDCB.BaudRate = Param;
	return setState();
}

unsigned long COMPort::bitRate() const {
	DCB & aDCB = *((LPDCB)dcb);
	return aDCB.BaudRate;
}

COMPort& COMPort::setLineCharacteristics( char * inConfig ) {
	COMMTIMEOUTS aTimeout;
	if ( !BuildCommDCBAndTimeouts ( inConfig, (LPDCB)dcb, &aTimeout  ) ) {
	}
	if ( ! SetCommTimeouts ( (HANDLE(portHandle)), &aTimeout  ) ) {
	}
	return setState();
}

char COMPort::read () {
	char buffer;
	DWORD charsRead = 0;

	ReadFile ((HANDLE(portHandle)), &buffer, sizeof(char), &charsRead, NULL);
	return  buffer;
}

unsigned long COMPort::readString ( void *inBuffer, const unsigned long inCharsReq) {
	DWORD charsRead = 0;

	ReadFile ((HANDLE(portHandle)), inBuffer, inCharsReq, &charsRead, NULL);
	return charsRead;
}

COMPort & COMPort::write ( const char inChar ) {
	char buffer = inChar;
	DWORD charsWritten = 0;

	if ( !WriteFile ( (HANDLE(portHandle)), &buffer, sizeof(char), &charsWritten, NULL) ) {
	}
	return  *this;
}

unsigned long COMPort::writeString ( const void *inBuffer, const unsigned long inBufSize ) {

	DWORD charsWritten = 0;

	if ( !WriteFile ( (HANDLE(portHandle)), inBuffer, inBufSize, &charsWritten, NULL ) ) {
	}

	return  charsWritten;
}

COMPort& COMPort::setxONxOFF ( bool Param ) {
	DCB & aDCB = *((LPDCB)dcb);
	aDCB.fOutX = Param ? 1 : 0;
	aDCB.fInX = Param ? 1 : 0;
	return setState();
}

bool COMPort::isxONxOFF () const {
	DCB & aDCB = *((LPDCB)dcb);
	return (aDCB.fOutX && aDCB.fInX);
}

COMPort& COMPort::setBlockingMode ( unsigned long inReadInterval, unsigned long inReadMultiplyer, unsigned long inReadConstant ) {
	COMMTIMEOUTS commTimeout;

	if ( !GetCommTimeouts ( (HANDLE(portHandle)), &commTimeout ) ) 	{
	}
	commTimeout.ReadIntervalTimeout = inReadInterval;
	if ( inReadInterval==MAXDWORD ) {
   		commTimeout.ReadTotalTimeoutMultiplier = 0;
   		commTimeout.ReadTotalTimeoutConstant = 0;
	}
	else {
   		commTimeout.ReadTotalTimeoutMultiplier = inReadMultiplyer;
   		commTimeout.ReadTotalTimeoutConstant = inReadConstant;
	}
	if ( !SetCommTimeouts ( (HANDLE(portHandle)), &commTimeout) ) {
	}
	return *this;
}

COMPort & COMPort::setHandshaking ( bool inHandshaking ) {
	DCB & aDCB = *((LPDCB)dcb);

	if (inHandshaking) {
   		aDCB.fOutxCtsFlow = TRUE;
   		aDCB.fOutxDsrFlow = FALSE;
   		aDCB.fRtsControl = RTS_CONTROL_HANDSHAKE;
	}
	else {
   		aDCB.fOutxCtsFlow = FALSE;
   		aDCB.fOutxDsrFlow = FALSE;
   		aDCB.fRtsControl = RTS_CONTROL_ENABLE;
	}
	return setState();
}

unsigned long COMPort::getMaximumBitRate() const {
	COMMPROP aProp;
	if ( !GetCommProperties ( (HANDLE)portHandle, &aProp ) ) {
	}
	return aProp.dwMaxBaud;
}

COMPort::MSPack COMPort::getModemSignals() const {
	MSPack aPack;

	if ( !GetCommModemStatus ( (HANDLE)portHandle, (LPDWORD)&aPack ) ) {
	}
	return aPack;
}

COMPort& COMPort::setParity ( Parity Param ) {
	DCB & aDCB = *((LPDCB)dcb);
	aDCB.Parity = Param;

	return setState();
}

COMPort& COMPort::setDataBits ( DataBits Param ) {
	DCB & aDCB = *((LPDCB)dcb);
	aDCB.ByteSize = Param;

	return setState();
}

COMPort& COMPort::setStopBits ( StopBits Param ) {
	DCB & aDCB = *((LPDCB)dcb);
	aDCB.StopBits = Param;

	return setState();
}

COMPort::Parity COMPort::parity () const {
	DCB & aDCB = *((LPDCB)dcb);

	return (COMPort::Parity)aDCB.Parity;
}

COMPort::DataBits COMPort::dataBits () const {
	DCB & aDCB = *((LPDCB)dcb);

	return (COMPort::DataBits)aDCB.ByteSize;
}

COMPort::StopBits COMPort::stopBits () const {
	DCB & aDCB = *((LPDCB)dcb);

	return (COMPort::StopBits)aDCB.StopBits;
}

COMPort::errorStatus COMPort::clearError() const {
    errorStatus eS;
    BOOL    fOOP, fOVERRUN, fPTO, fRXOVER, fRXPARITY, fTXFULL;
    BOOL    fBREAK, fDNS, fFRAME, fIOE, fMODE;

	// Get and clear current errors on the port.
    if (!ClearCommError((HANDLE(portHandle)), &eS.dwErrors, &eS.comStat)) {
    }
	return eS;
}

bool COMPort::Setup(DWORD dwInQueue, DWORD dwOutQueue) {
  return SetupComm((HANDLE(portHandle)), dwInQueue, dwOutQueue);
}

bool COMPort::Purge(DWORD dwFlags) {
  return PurgeComm((HANDLE(portHandle)), dwFlags);
}

bool COMPort::ClearWriteBuffer() {
  return Purge(PURGE_TXCLEAR);
}

bool COMPort::ClearReadBuffer() {
  return Purge(PURGE_RXCLEAR);
}

bool COMPort::Flush() {
  return FlushFileBuffers((HANDLE(portHandle)));
}
