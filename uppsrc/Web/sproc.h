class SlaveProcess
{
public:
	SlaveProcess() {}
	virtual ~SlaveProcess() {}

	virtual void Kill() = 0;
	virtual bool IsRunning() = 0;
	virtual void Write(String s) = 0;
	virtual bool Read(String& s) = 0;
	virtual int  GetExitCode() = 0;
	virtual void Detach() {};

private:
	SlaveProcess(const SlaveProcess& sp);
	void         operator = (const SlaveProcess& sp);
};

One<SlaveProcess> StartLocalProcess(const char *cmdline, const char *envptr = NULL);
One<SlaveProcess> StartRemoteProcess(const char *host, int port, const char *cmdline, const char *envptr = NULL, int timeout = Null);
One<SlaveProcess> StartProcess(const char *cmdline, const char *envptr = NULL, int timeout = Null);
