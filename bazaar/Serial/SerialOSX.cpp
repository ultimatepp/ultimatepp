#include "Serial.h"

#ifdef PLATFORM_OSX11

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <paths.h>
#include <termios.h>
#include <sysexits.h>
#include <sys/param.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/serial/IOSerialKeys.h>
#include <IOKit/IOBSD.h>

namespace Upp
{

	static bool __IsSymLink(const char *path)
	{
		struct stat stf;
		lstat(path, &stf);
		return S_ISLNK(stf.st_mode);
	}

	dword Serial::stdBauds[] =
	{
		0, 50, 75, 110, 134, 150,
		200, 300, 600, 1200, 1800,
		2400, 4800, 9600, 19200,
		38400, 57600, 115200, 230400, 345600, 460800
	};

	int Serial::stdBaudsCount = sizeof(Serial::stdBauds) / sizeof(dword);

	// constructor
	Serial::Serial()
	{
		isError = false;
		errCode = Ok;
		fd = -1;
	}

	Serial::Serial(String const &port, dword speed, byte parity, byte bits, byte stopBits)
	{
		isError = false;
		errCode = Ok;
		fd = -1;
		Open(port, speed, parity, bits, stopBits);
	}

	// destructor
	Serial::~Serial()
	{
		Close();
	}

	// open the port
	bool Serial::Open(String const &port, dword lSpeed, byte parity, byte bits, byte stopBits)
	{
		static int stdBaudCodes[] =
		{
			B0, B50, B75, B110, B134, B150,
			B200, B300, B600, B1200, B1800,
			B2400, B4800, B9600, B19200,
			B38400, B57600, B115200, B230400
		};

		fd = open(port, O_RDWR | O_NOCTTY | O_NONBLOCK);
		if (fd < 0)
		{
			fd = -1;
			isError = true;
			errCode = DeviceError;
			return false;
		}
		if (ioctl(fd, TIOCEXCL) == -1)
		{
			// unable to get exclusive access to port
			close(fd);
			fd = -1;
			isError = true;
			errCode = DeviceError;
			return false;
		}

		unsigned int ctl;
		if (ioctl(fd, TIOCMGET, &ctl) < 0)
		{
			// unable to query serial ports signals
			close(fd);
			fd = -1;
			isError = true;
			errCode = DeviceError;
			return false;
		}
		ctl &= ~(TIOCM_DTR | TIOCM_RTS);
		if (ioctl(fd, TIOCMSET, &ctl) < 0)
		{
			// unable to control serial ports signals
			close(fd);
			fd = -1;
			isError = true;
			errCode = DeviceError;
			return false;
		}

		// sets interface parameters
		struct termios tty;
		memset(&tty, 0, sizeof tty);
		if (tcgetattr(fd, &tty) != 0)
		{
			Close();
			isError = true;
			errCode = DeviceError;
			return false;
		}

		//// CFLAG /////

		int idx = GetStandardBaudRates().Find(lSpeed);
		dword speed;
		if (idx < 0)
		{
			// custom speed
			// @@ maybe add support later
			Close();
			isError = true;
			errCode = InvalidSpeed;
			return false;
		}
		else
		{
			// standard speed
			speed = stdBaudCodes[idx];
			cfsetospeed(&tty, speed);
			cfsetispeed(&tty, speed);
		}

		// parity
		tty.c_cflag &= ~(PARENB | PARODD);
		switch (parity)
		{
			case ParityNone:
				break;
			case ParityEven:
				tty.c_cflag |= PARENB;
				break;
			case ParityOdd:
				tty.c_cflag |= PARENB | PARODD;
				break;
			default:
				Close();
				isError = true;
				errCode = InvalidParity;
				return false;
		}

		// set data size
		switch (bits)
		{
			case 5:
				tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS5;
				break;
			case 6:
				tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS6;
				break;
			case 7:
				tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS7;
				break;
			case 8:
				tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
				break;
			default:
				Close();
				isError = true;
				errCode = InvalidSize;
				return false;
		}

		// stop bits
		if (stopBits == 1)
			tty.c_cflag &= ~CSTOPB;
		else if (stopBits == 2)
			tty.c_cflag |= CSTOPB;
		else
		{
			Close();
			isError = true;
			errCode = InvalidStopBits;
			return false;
		}

		// ignore modem controls, enable reading
		tty.c_cflag |= (CLOCAL | CREAD);

		// no rts/cts
		tty.c_cflag &= ~CRTSCTS;

		//// LFLAG /////

		// no signaling chars, no echo, no canonical mode
		tty.c_lflag = 0;
//		tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

		//// IFLAG /////

		// disable IGNBRK for mismatched speed tests; otherwise receive break as \000 chars
		// disable break processing
		tty.c_iflag &= ~IGNBRK;
//		tty.c_iflag |= IGNBRK;

		// DO NOT ignore carriage returns
		tty.c_iflag &= ~IGNCR;

		// DO NOT translate cr/lf
		tty.c_iflag &= ~ICRNL;

		// DO NOT translate cr
		tty.c_iflag &= ~INLCR;

		// shut off xon/xoff ctrl
		tty.c_iflag &= ~(IXON | IXOFF | IXANY);

		//// OFLAG /////

		// no remapping, no delays
		tty.c_oflag = 0;

		//// CONTROL CHARACTERS /////

		// read doesn't block
		tty.c_cc[VMIN]  = 0;

		// 0.5 seconds read timeout
		tty.c_cc[VTIME] = 0;

		/*
			RLOG("CFLAGS : " << tty.c_cflag);
			RLOG("LFLAGS : " << tty.c_lflag);
			RLOG("IFLAGS : " << tty.c_iflag);
			RLOG("OFLAGS : " << tty.c_oflag);
		*/

		if (tcsetattr(fd, TCSANOW, &tty) != 0)
		{
			Close();
			isError = true;
			errCode = DeviceError;
			return false;
		}

		return true;
	}

	// close the port
	void Serial::Close(void)
	{
		if (fd != -1)
			close(fd);
		fd = -1;
		isError = false;
		errCode = Ok;
	}

	// control DTR and RTS lines
	bool Serial::SetDTR(bool on)
	{
		unsigned int ctl;
		int r;

		r = ioctl(fd, TIOCMGET, &ctl);
		if (r < 0)
			return false;

		if (on)
			ctl |= TIOCM_DTR;
		else
			ctl &= ~TIOCM_DTR;

		r = ioctl(fd, TIOCMSET, &ctl);
		if (r < 0)
			return false;

		return true;
	}

	bool Serial::SetRTS(bool on)
	{
		unsigned int ctl;
		int r;

		r = ioctl(fd, TIOCMGET, &ctl);
		if (r < 0)
			return false;

		if (on)
			ctl |= TIOCM_RTS;
		else
			ctl &= ~TIOCM_RTS;

		r = ioctl(fd, TIOCMSET, &ctl);
		if (r < 0)
			return false;

		return true;
	}

	// flush data
	bool Serial::FlushInput(void)
	{
		if (fd == -1)
			return false;
		return !tcflush(fd, TCIFLUSH);
	}

	bool Serial::FlushOutput(void)
	{
		if (fd == -1)
			return false;
		return !tcflush(fd, TCOFLUSH);
	}

	bool Serial::FlushAll(void)
	{
		if (fd == -1)
			return false;
		return !tcflush(fd, TCIOFLUSH);
	}

	// check if data is available on serial port
	int Serial::Avail(void)
	{
		if (fd == -1)
			return false;
		int bytes = 0;
		ioctl(fd, FIONREAD, &bytes);
		return bytes;
	}

	// read a single byte, block 'timeout' milliseconds
	bool Serial::Read(byte &c, uint32_t timeout)
	{
		char buf[1];

		uint32_t tim = msecs() + timeout;
		for (;;)
		{
			if (!read(fd, buf, 1))
			{
				if ((uint32_t)msecs() > tim)
					return false;
				continue;
			}
			c = (byte)buf[0];
			return true;
		}
	}

	bool Serial::Read(char &c, uint32_t timeout)
	{
		char buf[1];

		uint32_t tim = msecs() + timeout;
		for (;;)
		{
			if (!read(fd, buf, 1))
			{
				if ((uint32_t)msecs() > tim)
					return false;
				continue;
			}
			c = buf[0];
			return true;
		}
	}

	// read data, requested amount, blocks 'timeout' milliseconds
	// return number of bytes got
	uint32_t Serial::Read(uint8_t *buf, uint32_t reqSize, uint32_t timeout)
	{
		if (!reqSize || !buf)
			return 0;

		uint32_t n;
		uint32_t tim = msecs() + timeout;
		uint32_t req = reqSize;
		while (req)
		{
			n = read(fd, buf, req);
			req -= n;
			buf += n;
			if (!req || msecs() > (int)tim)
				break;
		}
		return reqSize - req;
	}

	// read data, requested amount, blocks 'timeout' milliseconds
	// if reqSize == 0 just read all available data, waiting for 'timeout' if != 0
	String Serial::Read(uint32_t reqSize, uint32_t timeout)
	{
		char buf[1001];
		buf[1000] = 0;
		String data;

		size_t n;
		uint32_t tim = msecs() + timeout;
		if (reqSize)
		{
			n = min(reqSize, (uint32_t)1000);
			while (reqSize)
			{
				n = read(fd, buf, n);
				if (!n)
				{
					if ((uint32_t)msecs() > tim)
						break;
					n = min(reqSize, (uint32_t)1000);
					continue;
				}
				tim = msecs() + timeout;
				if (n)
				{
					reqSize -= n;
					data.Cat(buf, n);
				}
				n = min(reqSize, (uint32_t)1000);
			}
		}
		else
		{
			for (;;)
			{
				n = read(fd, buf, 1000);
				if (!n)
				{
					if ((uint32_t)msecs() > tim)
						break;
					continue;
				}
				tim = msecs() + timeout;
				if (n)
					data.Cat(buf, n);
			}
		}
		return data;
	}

	// write a single byte
	bool Serial::Write(char c, uint32_t timeout)
	{
		if (!timeout)
			return write(fd, &c, 1);

		uint32_t tim = msecs() + timeout;
		for (;;)
		{
			if (write(fd, &c, 1) == 1)
				return true;
			if ((uint32_t)msecs() > tim)
				break;
		}

		return false;
	}

	// write buffer
	bool Serial::Write(uint8_t const *buf, uint32_t len, uint32_t timeout)
	{
		if (!timeout)
			return (write(fd, buf, len) == len);

		uint32_t tim = msecs() + timeout;
		const uint8_t *dPos = buf;
		for (;;)
		{
			int written = write(fd, dPos, len);
			if (written == len)
				return true;
			if (written >= 0)
			{
				dPos += written;
				len -= written;
				continue;
			}
			if ((uint32_t)msecs() >= tim)
				break;
		}
		return false;
	}

	// writes string
	bool Serial::Write(String const &data, uint32_t timeout)
	{
		return Write((const uint8_t *)~data, data.GetCount(), timeout);
	}

	// check if opened
	bool Serial::IsOpened(void) const
	{
		return fd != -1;
	}

	static void macos_ports(io_iterator_t  *PortIterator, ArrayMap<String, String> &list)
	{
		io_object_t modemService;
		CFTypeRef nameCFstring;
		char s[MAXPATHLEN];

		while ((modemService = IOIteratorNext(*PortIterator)))
		{
			nameCFstring = IORegistryEntryCreateCFProperty(modemService, CFSTR(kIOCalloutDeviceKey), kCFAllocatorDefault, 0);
			if (nameCFstring)
			{
				if (CFStringGetCString((const __CFString *)nameCFstring, s, sizeof(s), kCFStringEncodingASCII))
					list.Add(s, "");

				CFRelease(nameCFstring);
			}
			IOObjectRelease(modemService);
		}
	}

	// get a list of connected serial ports
	// for old-style serials, check if something is connected
	// for usb ones, return also a description
	ArrayMap<String, String> Serial::GetSerialPorts(void)
	{
		ArrayMap<String, String> res;

		mach_port_t masterPort;
		CFMutableDictionaryRef classesToMatch;
		io_iterator_t serialPortIterator;

		if (IOMasterPort(0, &masterPort) != KERN_SUCCESS)
			return res;

		// a usb-serial adaptor is usually considered a "modem",
		// especially when it implements the CDC class spec
		classesToMatch = IOServiceMatching(kIOSerialBSDServiceValue);
		if (!classesToMatch)
			return res;

		CFDictionarySetValue(classesToMatch, CFSTR(kIOSerialBSDTypeKey), CFSTR(kIOSerialBSDModemType));
		if (IOServiceGetMatchingServices(masterPort, classesToMatch, &serialPortIterator) != KERN_SUCCESS)
			return res;

		macos_ports(&serialPortIterator, res);
		IOObjectRelease(serialPortIterator);

		// but it might be considered a "rs232 port", so repeat this search for rs232 ports
		classesToMatch = IOServiceMatching(kIOSerialBSDServiceValue);
		if (!classesToMatch)
			return res;

		CFDictionarySetValue(classesToMatch, CFSTR(kIOSerialBSDTypeKey), CFSTR(kIOSerialBSDRS232Type));
		if (IOServiceGetMatchingServices(masterPort, classesToMatch, &serialPortIterator) != KERN_SUCCESS)
			return res;
		macos_ports(&serialPortIterator, res);
		IOObjectRelease(serialPortIterator);

		return res;
	}

} // END_UPP_NAMESPACE

#endif
