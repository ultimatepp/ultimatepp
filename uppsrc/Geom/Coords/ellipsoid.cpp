#include "GeomCoords.h"

namespace Upp {

struct EPSGEllipsoid
{
	int         code;
	const char *name;
	double      a;
	int         unit;
	double      i;
	double      b;
	double      from_wgs84[4][3];
	const char *description;
	const char *source;
};

static EPSGEllipsoid epsg_ellipsoids[] =
{
	{
		7001,
		"Airy 1830",
		6377563.396,
		9001,
		299.3249646,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Original definition is a=20923713 and b=20853810 feet of 1796.   "
		"For the 1936 retriangulation OSGB defines the relationship of feet of 1796 "
		"to the International metre through log(1.48401603) exactly [=0.3048007491...]. "
		"1/f is given to 7 decimal places.",
		"Ordnance Survey of Great Britain."
	},

	{
		7002,
		"Airy Modified 1849",
		6377340.189,
		9001,
		299.3249646,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"OSGB Airy 1830 figure (ellipsoid code 7001) rescaled by 0.999965 to best fit "
		"the scale of the 19th century primary triangulation of Ireland.",
		"\"The Irish Grid - A Description of the Co-ordinate Reference System\" published by "
		"Ordnance Survey of Ireland, Dublin and Ordnance Survey of Northern Ireland, Belfast."
	},

	{
		7003,
		"Australian National Spheroid",
		6378160,
		9001,
		298.25,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Based on the GRS 1967 figure but with 1/f taken to 2 decimal places exactly.  "
		"The dimensions are also used as the GRS 1967 (SAD69) ellipsoid (see code 7050).",
		"\"Australian Map Grid Technical Manual\"; National Mapping Council of Australia "
		"Special Publication #7; 1972"
	},

	{
		7004,
		"Bessel 1841",
		6377397.15508,
		9001,
		299.152812853,
		0,
/*
		{
			{ 0.9999001532, 0.0001003391, 0.0001410308 },
			{ 0.000000035, 0.9999890082, 0.000006789 },
			{ -0.0000892616, 0.0001700881, 1.0001042705 },
			{ -752.6802330762, -118.3304874504, -671.1345682954 }
		},
*/
/*
		{
			{ 0.9999587017, 0.0000771428, 0.0001469385 },
			{ 0.0000018086, 1.0000315577, 0.0000089026 },
			{ -0.0000715101, 0.0001430558, 1.0001561833 },
			{ -569.8893823209, -64.387031166, -439.280263002 }
		},
*/
/*
		{
			{ 0.9999470981, 0.0001003053, 0.0001410471 },
			{ 0.0000000365, 1.000035941,  0.0000067921 },
			{ -0.0000894093, 0.0001700079, 1.0001513634 },
			{ -565.889445039, -66.6072244279, -442.284815087 }
		},
*/
/*
		{
			{ 0.9999470671, 0.0002844202, 0.0001410362 },
			{ -0.0001840925, 1.0000359244, 0.000006789 },
			{ -0.0000894353, 0.0001699928, 1.00015135 },
			{ 77.7166364223, -1328.5538510867, -1743.8357660789 }
		},
*/
/*
		{
			{ 0.9999470981, 0.0001003053, 0.0001410471 },
			{ 0.0000000365, 1.000035941, 0.0000067921  },
			{ -0.0000894093, 0.0001700079, 1.0001513634 },
			{ -565.889445039, -66.6072244279, -442.284815087 }
		},
//*/
//*
#define BCOEF (1 - 3.543e-6)
#define A_RAD (-4.99821 * SECRAD)
#define B_RAD (-1.58676 * SECRAD)
#define C_RAD (-5.26110 * SECRAD)

		{
/*
			{      1 * BCOEF, -C_RAD * BCOEF,  B_RAD * BCOEF },
			{  C_RAD * BCOEF,      1 * BCOEF, -A_RAD * BCOEF },
			{ -B_RAD * BCOEF,  A_RAD * BCOEF,      1 * BCOEF },
			{ -570.690, -85.690, -462.840 }
*/
			{      1 * BCOEF, +C_RAD * BCOEF, -B_RAD * BCOEF },
			{ -C_RAD * BCOEF,      1 * BCOEF, +A_RAD * BCOEF },
			{ +B_RAD * BCOEF, -A_RAD * BCOEF,      1 * BCOEF },
			{ -570.690, -85.690, -462.840 }
		},
#undef BCOEF
//*/
		"Original Bessel definition is a=3272077.14 and b=3261139.33 toise. "
		"This used a weighted mean of values from several authors but did not account "
		"for differences in the length of the various toise: the \"Bessel toise\" "
		"is therefore of uncertain length.",
		"US Army Map Service Technical Manual; 1943."
	},

	{
		7005,
		"Bessel Modified",
		6377492.018,
		9001,
		299.1528128,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Used in Norway and also in Sweden with a 1mm increase in semi-major axis.",
		""
	},

	{
		7006,
		"Bessel Namibia",
		6377483.865,
		9001,
		299.1528128,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"a = 6377397.155 German legal metres. This is the same value as the Bessel 1841 figure "
		"(code 7004) but in different units.  Used in Namibia.",
		"Chief Directorate: Surveys and Mapping, Mowbray, South Africa."
	},

	{
		7007,
		"Clarke 1858",
		20926348,
		9005,
		0,
		20855233,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Clarke's 1858/II solution. Derived parameters: a = 6378293.645m using his 1865 "
		"ratio of 0.304797265 feet per metre; 1/f = 294.26068 deg  In Australia and Amoco "
		"Trinidad 1/f taken to two decimal places (294.26 exactly}, elsewhere a and b "
		"used to derive 1/f.",
		"\"Ellipsoidisch Parameter der Erdfigur (1800-1950)\" by Georg Strasser."
	},

	{
		7008,
		"Clarke 1866",
		6378206.4,
		9001,
		0,
		6356583.8,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Original definition a=20926062 and b=20855121 (British) feet. Uses Clarke's "
		"1865 inch-metre ratio of 39.370432 to obtain metres. (Metric value then converted "
		"to US survey feet for use in the United States using 39.37 exactly giving "
		"a=20925832.16 ft US).", "US Army Map Service Technical Manual No. 7; 1943."
	},

	{
		7009,
		"Clarke 1866 Michigan",
		20926631.531,
		9003,
		0,
		20855688.674,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Used for Michigan NAD27 State Plane zones.  Radius = ellipsoid radius + 800 feet; "
		"this approximates the average elevation of the state.   Derived parameter: "
		"1/f = 294.97870", "USGS Professional Paper #1395."
	},

	{
		7010,
		"Clarke 1880 (Benoit)",
		6378300.789,
		9001,
		0,
		6356566.435,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Adopts Clarke's values for a and b.  Uses Benoit's 1895 ratio of 0.9143992 metres "
		"per yard to convert to metres."
	},

	{
		7011,
		"Clarke 1880 (IGN)",
		6378249.2,
		9001,
		0,
		6356515,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Adopts Clarke's values for a and b using his 1865 ratio of 39.370432 inches per metre "
		"to convert axes to metres."
	},

	{
		7012,
		"Clarke 1880 (RGS)",
		6378249.145,
		9001,
		293.465,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Adopts Clarke's values for a and 1/f.  Adopts his 1865 ratio of 39.370432 inches per metre "
		"to convert semi-major axis to metres. Also known as Clarke Modified 1880.",
		"Empire Survey Review #32; 1939."
	},

	{
		7013,
		"Clarke 1880 (Arc)",
		6378249.145,
		9001,
		293.4663077,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Adopts Clarke's value for a with derived 1/f.  Uses his 1865 ratio of 39.370432 "
		"inch per metre to convert semi-major axis to metres.",
		"Chief Directorate: Surveys and Mapping, Mowbray, South Africa."
	},

	{
		7014,
		"Clarke 1880 (SGA 1922)",
		6378249.2,
		9001,
		293.46598,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Used in Old French Triangulation (ATF).   Uses Clarke's 1865 inch-metre ratio "
		"of 39.370432 to convert axes to metres.", ""
	},

	{
		7015,
		"Everest 1830 (1937 Adjustment)",
		6377276.345,
		9001,
		300.8017,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Used for the 1937 readjustment of Indian triangulation.  Clarke's 1865 "
		"Indian-British foot ratio (0.99999566) and Benoit's 1898 British inch-metre "
		"ratio (39.370113) rounded as 0.30479841 exactly and applied to Everest's "
		"1830 definition taken as a and 1/f",
		"Survey of India professional paper #28; 1939"
	},

	{
		7016,
		"Everest 1830 (1967 Definition)",
		6377298.556,
		9001,
		300.8017,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Adopted 1967 for use in East Malaysia.  Applies Sears 1922 inch-metre ratio of "
		"39.370147 to Everest 1830 original definition of a and 1/f but with a taken "
		"to be in British rather than Indian feet."
	},

	{
		7018,
		"Everest 1830 Modified",
		6377304.063,
		9001,
		300.8017,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Adopted 1967 for use in West Malaysia.  Applies Benoit 1898 inch-metre ratio "
		"of 39.370113 to Everest 1830 original definition of a and 1/f but with a taken "
		"to be in British rather than Indian feet."
	},

	{
		7019,
		"GRS 1980",
		6378137,
		9001,
		298.257222101,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Adopted by IUGG 1979 Canberra.  Inverse flattening is derived from geocentric "
		"gravitational constant GM = 3986005e8 m*m*m/s/s; dynamic form factor "
		"J2 = 108263e8 and Earth's angular velocity = 7292115e-11 rad/s.",
		"\"Geodetic Reference System 1980\" by H. Moritz; Bulletin Geodesique"
	},

	{
		7020,
		"Helmert 1906",
		6378200,
		9001,
		298.3,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Helmert 1906/III solution.",
		"\"Ellipsoidisch Parameter der Erdfigur (1800-1950)\" by Georg Strasser"
	},

	{
		7021,
		"Indonesian National Spheroid",
		6378160,
		9001,
		298.247,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Based on the GRS 1967 figure but with 1/f taken to 3 decimal places exactly.",
		"Rais paper."
	},

	{
		7022,
		"International 1924",
		6378388,
		9001,
		297,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Adopted by IUGG 1924 in Madrid. Based on Hayford 1909/1910 figures. ",
		""
	},

	{
		7024,
		"Krassowsky 1940",
		6378245,
		9001,
		298.3,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { -25, 122.5, 81.25 } },
		"",
		""
	},

	{
		7025,
		"NWL 9D",
		6378145,
		9001,
		298.25,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Used by Transit Precise Ephemeris between October 1971 and January 1987.",
		""
	},

	{
		7027,
		"Plessis 1817",
		6376523,
		9001,
		308.64,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Rescaling of Delambre 1810 figure (a=6376985 m) to make meridional arc "
		"from equator to pole equal to 10000000 metres exactly. (Ref: Strasser).",
		"IGN Paris \"Constants d\'Ellipsoides\" February 1972."
	},

	{
		7028,
		"Struve 1860",
		6378298.3,
		9001,
		294.73,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Original definition of semi-major axis given as 3272539 toise.  In "
		"\"Ellipsoidisch Parameter der Erdfigur (1800-1950)\" , Strasser suggests "
		"a conversion factor of 1.94903631 which gives a=6378297.337 metres.",
		"\"Geodesia y Cartografia Matematica\" by Fernando Martin Asin; ISBN 84-398-0248-X."
	},

	{
		7029,
		"War Office",
		6378300,
		9001,
		296,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"In non-metric form, a=20926201 Gold Coast feet.",
		"Tables for the use of the Gold Coast Survey Department, 1935."
	},

	{
		7030,
		"WGS 84",
		6378137,
		9001,
		298.257223563,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Inverse flattening derived from four defining parameters (semi-major axis; "
		"C20 = -484.16685*10e-6; earth's angular velocity w = 7292115e11 rad/sec; "
		"gravitational constant GM = 3986005e8 m*m*m/s/s).",
		"DMA Technical Manual 8350.2-B "
	},

	{
		7031,
		"GEM 10C",
		6378137,
		9001,
		298.257223563,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Used for  GEM 10C Gravity Potential Model.",
		""
	},

	{
		7032,
		"OSU86F",
		6378136.2,
		9001,
		298.257223563,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Used for OSU86 gravity potential (geoidal) model.",
		""
	},

	{
		7033,
		"OSU91A",
		6378136.3,
		9001,
		298.257223563,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Used for OSU91 gravity potential (geoidal) model.",
		""
	},

	{
		7034,
		"Clarke 1880",
		20926202,
		9005,
		293.465,
		20854895,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Clarke gave a and b and also 1/f=293.465 (to 3 decimal places).  "
		"1/f derived from a and b = 293.4663077 deg",
		"\"Ellipsoidisch Parameter der Erdfigur (1800-1950)\" by Georg Strasser."
	},

	{
		7035,
		"Sphere",
		6371000,
		9001,
		0,
		6371000,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Authalic sphere.  1/f is infinite. Superseded by GRS 1980 authalic sphere (code 7047).",
		""
	},

	{
		7036,
		"GRS 1967",
		6378160,
		9001,
		298.247167427,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Adopted by IUGG 1967 Lucerne.  Inverse flattening given is derived from "
		"geocentric gravitational constant (GM)= 398603e9 m*m*m/s/s; dynamic form "
		"factor (J2) = 0.0010827 and Earth's angular velocity w = 7.2921151467e-5 rad/s. "
		"See also GRS 1967 (SAD69).",
		"\"Geodetic Reference System 1967\"; International Association of Geodesy "
		"special publication number 3; August 1971."
	},

	{
		7041,
		"Average Terrestrial System 1977",
		6378135,
		9001,
		298.257,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"",
		"New Brunswick Geographic Information Corporation land and water information standards manual"
	},

	{
		7042,
		"Everest (1830 Definition)",
		20922931.8,
		9080,
		300.8017,
		20853374.58,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Everest gave a and b to 2 decimal places and also 1/f=300.8017 (to 4 decimal places).",
		"\"Ellipsoidisch Parameter der Erdfigur (1800-1950)\" by Georg Strasser"
	},

	{
		7043,
		"WGS 72",
		6378135,
		9001,
		298.26,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"",
		""
	},

	{
		7044,
		"Everest 1830 (1962 Definition)",
		6377301.243,
		9001,
		300.8017255,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Used by Pakistan since metrication.  Clarke's 1865 Indian foot-British "
		"foot ratio (0.99999566) and his 1865 British inch-metre ratio (39.369971) "
		"rounded with slight error as 1 Ind ft = 0.3047995m exactly and applied to "
		"Everest's 1830 definition of a & b.",
		""
	},

	{
		7045,
		"Everest 1830 (1975 Definition)",
		6377299.151,
		9001,
		300.8017255,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Used by India since metrication.  Clarke's 1865 Indian foot-British foot "
		"ratio (0.99999566) and his 1865 British inch-metre ratio (39.369971) "
		"rounded as 1 Ind ft = 0.3047995m exactly applied to Everest's 1830 "
		"original definition taken as a and b.",
		""
	},

	{
		7046,
		"Bessel Namibia (GLM)",
		6377397.155,
		9031,
		299.1528128,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"The semi-major axis has the same value as the Bessel 1841 ellipsoid (code 7004) "
		"but is in different units - German Legal Metres rather than International metres "
		"- hence a different size.  a = 6377483.865 International metres. Used in Namibia.",
		"Chief Directorate: Surveys and Mapping, Mowbray, South Africa."
	},

	{
		7047,
		"GRS 1980 Authalic Sphere",
		6370997,
		9001,
		0,
		6370997,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Authalic sphere derived from Clarke 1866 ellipsoid (code 7008).  Deprecated "
		"as name and parameter values do not match; replaced by 7048.", "EPSG"
	},

	{
		7048,
		"GRS 1980 Authalic Sphere",
		6371007,
		9001,
		0,
		6371007,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Authalic sphere derived from GRS 1980 ellipsoid (code 7019).  (An authalic sphere "
		"is one with a surface area equal to the surface area of the ellipsoid). 1/f is infinite.",
		"EPSG"
	},

	{
		7049,
		"Xian 1980",
		6378140,
		9001,
		298.257,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"",
		"BP"
	},

	{
		7050,
		"GRS 1967 (SAD69)",
		6378160,
		9001,
		298.25,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Based on the GRS 1967 figure (code 7036) but with 1/f taken to 2 decimal places "
		"exactly. Used with SAD69 datum. The dimensions are also used as the Australian "
		"National Spheroid (code 7003).", "\"Geodetic Reference System 1967\"; International "
		"Association of Geodesy special publication number 3; August 1971."
	},

	{
		7051,
		"Danish 1876",
		6377019.27,
		9001,
		300,
		0,
		{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 0, 0, 0 } },
		"Semi-major axis originally given as 3271883.25 toise. Uses toise to French "
		"metre ratio of 1.94903631 to two decimal place precision. An alternative ratio "
		"with the German legal metre of 1.9490622 giving 6377104m has not been used in Danish work.",
		"Kort og Matrikelstyrelsen (KMS), Copenhagen."
	},
};

Pointf3 GisEllipsoid::To3D(double lon, double lat, double elev) const
{
	lon *= DEGRAD;
	lat *= DEGRAD;
	double cl = cos(lon), sl = sin(lon), cp = cos(lat), sp = sin(lat);
	double n = a / sqrt(1 - e2 * sp * sp);
	return Pointf3((n + elev) * cp * cl, (n + elev) * cp * sl, (n * (1 - e2) + elev) * sp);
}

Pointf GisEllipsoid::From3D(double x, double y, double z) const
{
	double p = hypot(x, y);
	double th = atan2(z * a, p * b);
	double st = sin(th);
	double ct = cos(th);
	double tn = (z + e2 * a * a / b * st * st * st);
	double td = (p - e2 * a * ct * ct * ct);
	return Pointf((2 / DEGRAD) * atan2(y, p + x), atan2(tn, td) / DEGRAD);
}

const Index<int>& GisEllipsoid::EnumEPSG()
{
	static Index<int> eem;
	if(eem.IsEmpty())
	{
		eem.Reserve(__countof(epsg_ellipsoids));
		for(int i = 0; i < __countof(epsg_ellipsoids); i++)
			eem.Add(epsg_ellipsoids[i].code);
	}
	return eem;
}

GisEllipsoid GisEllipsoid::GetEPSG(int code)
{
	int c = EnumEPSG().Find(code);
	GisEllipsoid out;
	if(c >= 0)
	{
		const EPSGEllipsoid& ee = epsg_ellipsoids[c];
		out.name = ee.name;
		out.description = ee.description;
		out.source = ee.source;
		out.a = ee.a;
		out.b = ee.b;
		out.i = ee.i;
		if(out.a == out.b)
			out.e2 = 0;
		else
		{
			if(!out.i) out.i = out.a / (out.a - out.b);
			if(!out.b) out.b = out.a * (1 - 1 / out.i);
			out.e2 = 1 - sqr(out.b / out.a);
		}
		out.from_wgs84 = Matrixf3(ee.from_wgs84[0][0], ee.from_wgs84[0][1], ee.from_wgs84[0][2],
			ee.from_wgs84[1][0], ee.from_wgs84[1][1], ee.from_wgs84[1][2],
			ee.from_wgs84[2][0], ee.from_wgs84[2][1], ee.from_wgs84[2][2],
			ee.from_wgs84[3][0], ee.from_wgs84[3][1], ee.from_wgs84[3][2]);
	}
	return out;
}

GisEllipsoid GisEllipsoid::AB(double a, double b)
{
	GisEllipsoid r;
	r.a = a;
	r.b = b;
	r.e2 = 1 - sqr(b / a);
	r.i = 1 - b / a;
	return r;
}

GisEllipsoid GisEllipsoid::AI(double a, double i)
{
	GisEllipsoid r;
	r.a = a;
	r.i = i;
	r.b = a * (1 - 1 / i);
	r.e2 = 1 - sqr(r.a / r.b);
	return r;
}

GisEllipsoid GisEllipsoid::AE2(double a, double e2)
{
	GisEllipsoid r;
	r.a = a;
	r.b = sqrt((1 - e2) * (a * a));
	r.e2 = e2;
	r.i = 1 - r.b / a;
	return r;
}

GisEllipsoid GisEllipsoid::R(double rad)
{
	GisEllipsoid r;
	r.a = r.b = rad;
	r.e2 = 0;
	r.i = Null;
	return r;
}

bool operator == (const GisEllipsoid& ea, const GisEllipsoid& eb)
{
	return ea.a == eb.a && ea.b == eb.b && ea.from_wgs84 == eb.from_wgs84;
}

}
