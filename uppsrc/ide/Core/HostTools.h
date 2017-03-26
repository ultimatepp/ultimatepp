class HostTools {
public:
	virtual ~HostTools() {}
	
	virtual String NormalizeExecutablePath(const String& path) const = 0;
};

class LocalHostTools final : public HostTools {
public:
	String NormalizeExecutablePath(const String& path) const override;
};

