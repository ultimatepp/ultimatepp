class ImageDraw : public Draw, NoCopy {
	Size    size;

#ifdef PLATFORM_WIN32
	struct  Section {
		HDC     dc;
		HBITMAP hbmp, hbmpOld;
		RGBA   *pixels;

		void Init(int cx, int cy);
		~Section();
	};

	Section rgb;
	Section a;
	Draw    alpha;
#endif

#ifdef PLATFORM_X11
	Draw    alpha;
#endif

	bool    has_alpha;

	void Init();
	Image Get(bool pm) const;

public:
	Draw& Alpha();                       

	operator Image() const;
	
	Image GetStraight() const;
	
	ImageDraw(Size sz);
	ImageDraw(int cx, int cy);
	~ImageDraw();
};
