#ifndef _DXF_Tables_h_
#define _DXF_Tables_h_

// forward declaration for back linking
class DXF;
class DXFTables;

// layer definition
class DXFLayer
{
	friend class DXFTables;
	private:
	
		// container tables
		Ptr<DXFTables> tables;
	
		// write layer to stream
		bool Write(Stream &s);
		
		// layer name
		String name;
		
		// layer color
		int color;
		
		// layer linetype
		String lineType;

		// private constructor
		DXFLayer(DXFTables *t);

		// handle
		uint64 handle;

	protected:
	
	public:
	
		DXFLayer &SetName(String const &n);
		String const &GetName(void) const { return name; }
		
		DXFLayer &SetColor(int c);
		int GetColor(void) const { return color; }
		
		DXFLayer &SetLineType(String const &lt);
		String const &GetLineType(void) const { return lineType; }
};

class DXFLineType
{
	friend class DXFTables;
	private:
	
		// container tables
		Ptr<DXFTables> tables;
	
		// write linetype to stream
		bool Write(Stream &s);
		
		// array of linetype elements (mark-space-mark-space)
		// specifies widths of segments and spaces in units
		// should be an even number ?
		Vector<double> elements;
		
		// linetype name
		String name;
		
		// linetype description
		String desc;

		// private constructor
		DXFLineType(DXFTables *t);

		// handle
		uint64 handle;

	protected:
	
	public:

		DXFLineType &SetName(String const &name);
		String const &GetName(void) const { return name; }
		
		DXFLineType &SetDescription(String const &d);
		String const &GetDescription(void) const { return desc; }

		DXFLineType &SetElements(Vector<double> const &e);
		DXFLineType &SetElements(double e1, double e2, double e3 = Null, double e4 = Null, double e5 = Null, double e6 = Null, double e7 = Null, double e8 = Null);
		Vector<double> const &GetElements(void) const { return elements; }
};

class DXFTables : public Pte<DXFTables>
{
	friend class DXF;
	friend class DXFLayer;
	friend class DXFLineType;
	private:
	
		// back link to container DXF
		Ptr<DXF> dxf;
		
		// layers
		ArrayMap<String, DXFLayer> layers;
		uint64 layersHandle;
		
		// linetypes
		ArrayMap<String, DXFLineType> lineTypes;
		uint64 lineTypesHandle;
		
		// plot style handle -- hard coded from dxf template
		uint64 plotStyleHandle;
		
		// private constructor -- may be created only by DXF class
		DXFTables(DXF *d);
		
		// write tables section to stream
		bool Write(Stream &s);
	
	protected:
	
	public:
	
		// gets next available handle
		uint64 GetNextHandle(void);
		
		// adds a layer
		DXFLayer &AddLayer(String const &name, int color = 7, String const &lType = "CONTINUOUS");
		
		// checks if layer is there
		bool HasLayer(String const &name) const;
		
		// gets the layer descriptor
		DXFLayer &GetLayer(String const &name);
		
		// adds a linetype
		DXFLineType &AddLineType(String const &name, Vector<double> const &elements = Vector<double>());
		DXFLineType &AddLineType(String const &name, double e1, double e2, double e3 = Null, double e4 = Null, double e5 = Null, double e6 = Null, double e7 = Null, double e8 = Null);

		// check if linetype is there
		bool HasLineType(String const &name) const;
		
		// gets linetype descriptor
		DXFLineType &GetLineType(String const &name);
};
#endif
