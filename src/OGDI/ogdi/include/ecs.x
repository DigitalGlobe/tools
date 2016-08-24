#ifdef RPC_HDR
%/* 
% * fichier genere par RPCGEN a partir de ecs.x
% *
% *    Declaration des structures & prototypes de l'extension client/serveur.
% *    Ce fichier est recu par rpcgen et ce dernier generera 4 fichiers pour
% *    pour l'extension client/serveur. Le "header" suivant doit ce retrouver
% *    dans tout les fichiers 
% */
%
%
% /* Copyright (C) 1995 Logiciels et Applications Scientifiques (L.A.S.) Inc*/
% /* Permission to use, copy, modify and distribute this software and       */
% /* its documentation for any purpose and without fee is hereby granted,   */
% /* provided that the above copyright notice appear in all copies, that    */
% /* both the copyright notice and this permission notice appear in         */
% /* supporting documentation, and that the name of L.A.S. Inc not be used  */
% /* in advertising or publicity pertaining to distribution of the software */
% /* without specific, written prior permission. L.A.S. Inc. makes no       */
% /* representations about the suitability of this software for any purpose.*/
% /* It is provided "as is" without express or implied warranty.            */
%
%extern u_int ecs_compression_type;
%extern u_int ecs_compression_version;
%extern u_int ecs_compression_blksize;
%extern u_int ecs_compression_level;
%extern u_int ecs_compression_fullsize;
%
#endif

#ifdef RPC_SVC
%#define main dummy
%#define RPC_SVC_FG
%
%extern ecs_Server *svr_handle;
%
%void start_closedown_check(void)
%{
%	extern int _rpcpmstart;
%	static void closedown(int sig);
%
%	_rpcpmstart = 1;
%	(void) signal(SIGALRM, (SIG_PF) closedown);
%	(void) alarm(_RPCSVC_CLOSEDOWN);
%}
%
#endif

const ECS_SUCCESS = 0;
const ECS_FAILURE = 1;

const ECS_RASTERSIZE = 1000;
const ECS_MTEXT = 1024;
const ECS_TRANSIENT_MIN = 1073741824;
const ECS_TRANSIENT_MAX = 1610612735;

const ECS_COMPRESS_NONE = 0;
const ECS_COMPRESS_ZLIB = 1;

const ECS_ZLIB_VERSION = 0;
const ECS_ZLIB_LEVEL_DEFAULT = 1;
const ECS_ZLIB_BLKSIZE_DEFAULT = 512;
const ECS_ZLIB_BLKSIZE_MAX = 131072;

const ECS_CACHE_DEFAULT = 25;
const ECS_CACHE_MIN = 1;
const ECS_CACHE_MAX = 10000;

enum ecs_Family {
	Area = 1,
	Line = 2,
	Point = 3,
	Matrix = 4,
	Image = 5,
	Text = 6,
	Edge = 7,
	Face = 8,
	Node = 9,
	Ring =10
};

/* 
 * Structure contenant la description d'une region geographique.
 */

struct ecs_Region {
  double north;
  double south;
  double east;
  double west;
  double ns_res;
  double ew_res;
};

/*
 * Structure contenant les informations relatif a la conversion
 * de projection de raster
 */

enum ecs_Resampling {
   nn = 1
};

enum ecs_Transformation {
   projective = 1,
   affine = 2,
   similarity = 3
};

struct ecs_RasterConversion {
  double coef<>;
  int isProjEqual;
  ecs_Resampling r_method;
  ecs_Transformation t_method;
};

/*
 * Description d'une coordonnee geographique.
 */

struct ecs_Coordinate {
  double x;
  double y;
};

/*
 * Description d'un objet geographique de type Area.
 */

struct ecs_FeatureRing {
	ecs_Coordinate centroid;
	ecs_Coordinate c<>;
};

struct ecs_Area {
	ecs_FeatureRing ring<>;
};

/*
 * Description d'un objet geographique de type Line.
 */

struct ecs_Line {
	ecs_Coordinate c<>;
   };

/*
 * Description d'un objet geographique de type Point.
 */

struct ecs_Point {
	ecs_Coordinate c;
};

/*
 * Description d'un objet geographique de type Matrix.
 */

struct ecs_Matrix {
  unsigned int x<>;
};


/*
 * Description d'un objet geographique de type Image.
 */

struct ecs_Image {
  unsigned int x<>;
};

/*
 * Description d'un objet geographique de type Text.
 */

struct ecs_Text {
  string desc<>;
  ecs_Coordinate c;
};

/* 
 * Description de la primitive NODE
 */

struct ecs_Node {
	int id;
	int containfaceid;
	ecs_Coordinate c;
};

/* 
 * Description de la primitive EDGE       
 */

struct ecs_Edge {
	int id;
	int startnodeid;
	int endnodeid;
	int rightfaceid;
	int leftfaceid;
	int rightfedgeid;
	int leftfedgeid;
	ecs_Coordinate c<>;
};

/* 
 * Description de la primitive RING. Si topology level 0,1 ou 2 une liste de "edges" 
 * si topology level 3 une face (ecs_Face) avec une liste de "edges"
 */

enum ecs_TopoLevel {
	Level012 = 1,
	Level3 = 2
};

/* 
 * Descrition de la primitive FACE
 */

struct ecs_Face {
	int id;
	int edgeid<>;
};

union ecs_AreaPrim switch (ecs_TopoLevel level) {
	case Level012 : int edgeid<>;
	case Level3   : ecs_Face fedgeid<>;
	default : void;
};


/*
 * Description de la geometrie d'un object ecs
 */

union ecs_Geometry switch (ecs_Family family) {
	case Area: ecs_Area area;
	case Line: ecs_Line line;
	case Point: ecs_Point point;
	case Matrix: ecs_Matrix matrix;
	case Image: ecs_Image image;
	case Text: ecs_Text text;
	case Node: ecs_Node node;
	case Edge: ecs_Edge edge;
	case Ring: ecs_AreaPrim ring;
	default : void;
};

struct ecs_Object {
	string Id<>;
	ecs_Geometry geom;
	string attr<>;
	double xmin; /* "bounding box" de l'objet */
	double ymin;
	double xmax;
	double ymax;
};

/*
 * Format des types disponible dans SQL
 */

enum ecs_AttributeFormat {
        Char = 1,
        Varchar = 2,
        Longvarchar = 3,
        Decimal = 4,
        Numeric = 5,
        Smallint = 6,
        Integer = 7,
        Real = 8,
        Float = 9,
        Double = 10
};

/* 
 * Description d'un type d'attribut.
 */

struct ecs_ObjAttribute {
        string name<>;
        ecs_AttributeFormat type;
        int lenght;
        int precision;
        int nullable;
};

struct ecs_ObjAttributeFormat {
	ecs_ObjAttribute oa<>;
};

struct ecs_Category {
        long no_cat;
        unsigned int r;
        unsigned int g;
        unsigned int b;
        string label<>;
        unsigned long qty; /* nbre de cellules ayant cette categorie dans la matrice */
};

struct ecs_RasterInfo {
        long mincat;
        long maxcat;
	int width;
	int height;
        ecs_Category cat<>;
};
	
/*
 * Structure for compression parameters
 */

struct ecs_Compression {
	unsigned int cachesize;	/* Number of items to get with getnextobject */
	unsigned int ctype;	/* Compression type i.e. ECS_COMPRESS_ZLIB */
	unsigned int cversion;	/* Which version of this compression */
	unsigned int clevel;	/* How agressively to compress min 1 - 9 max */
	unsigned int cblksize;	/* Number of bytes to compress at a time */
	unsigned int cfullsize;	/* Used by server */
};

/* 
 * Description du resultat retourne par ECS
 */

enum ecs_ResultType { 
	SimpleError		= 0,
	Object		 	= 1,
	GeoRegion 		= 2,
	objAttributeFormat	= 3,
	RasterInfo 		= 4,
	AText			= 5,
	MultiResult		= 6
};


union ecs_ResultUnion switch(ecs_ResultType type) {
	case Object: ecs_Object dob;
	case GeoRegion: ecs_Region gr;
	case objAttributeFormat: ecs_ObjAttributeFormat oaf;
	case RasterInfo: ecs_RasterInfo ri;
	case AText : string s<>;
	case MultiResult : ecs_ResultUnion results<>;
	default : void;
};

struct ecs_Result {
	ecs_Compression compression;
	int error;
	string message<>;
	ecs_ResultUnion res;
};


/* 
 * Structure pour le parametre compose de SELECTLAYER 
 */

struct ecs_LayerSelection {
	string Select<>;
	ecs_Family F;
};

/*
 * Structure for proxy server connect
 */

struct ecs_ProxyCreateServer {
	string server_name<>;
	string server_url<>;
};

#ifdef RPC_HDR
%
%#include <ecs_util.h> /* Outils pour l'aide a ecs */
%
#endif

#if (!defined(RPC_SVC) && !defined(RPC_CLNT)) || \
    ((defined(RPC_SVC) || defined(RPC_CLNT)) && !defined(PROXY))
/* Definition des programmes */
program ECSPROG {
	version ECSVERS {
		ecs_Result CREATESERVER(string) = 1;
		ecs_Result DESTROYSERVER(void) = 2;
		ecs_Result SELECTLAYER(ecs_LayerSelection) = 3;
		ecs_Result RELEASELAYER(ecs_LayerSelection) = 4;
		ecs_Result SELECTREGION(ecs_Region) = 5;
		ecs_Result GETDICTIONNARY(void) = 6;
		ecs_Result GETATTRIBUTEFORMAT(void) = 7;
		ecs_Result GETNEXTOBJECT(void) = 8;
		ecs_Result GETRASTERINFO(void) = 9;
		ecs_Result GETOBJECT(string) = 10;
		ecs_Result GETOBJECTIDFROMCOORD(ecs_Coordinate) = 11;
		ecs_Result UPDATEDICTIONARY(string) = 12;
		ecs_Result GETSERVERPROJECTION(void) = 13;
		ecs_Result GETGLOBALBOUND(void) = 14;
		ecs_Result SETSERVERLANGUAGE(unsigned int) = 15;
		ecs_Result SETSERVERPROJECTION(string) = 16;
		ecs_Result SETRASTERCONVERSION(ecs_RasterConversion) = 17;
		ecs_Result CREATEPROXYSERVER(ecs_ProxyCreateServer) = 100;
		ecs_Result SETCOMPRESSION(ecs_Compression) = 101;
	} = 1;
} = 0x20000001;
#endif

#if (!defined(RPC_SVC) && !defined(RPC_CLNT)) || \
    ((defined(RPC_SVC) || defined(RPC_CLNT)) && defined(PROXY))
program ECSPROXYPROG {
	version ECSPROXYVERS {
		ecs_Result PROXY_CREATEREMOTESERVER(string) = 1;
		ecs_Result PROXY_DESTROYSERVER(void) = 2;
		ecs_Result PROXY_SELECTLAYER(ecs_LayerSelection) = 3;
		ecs_Result PROXY_RELEASELAYER(ecs_LayerSelection) = 4;
		ecs_Result PROXY_SELECTREGION(ecs_Region) = 5;
		ecs_Result PROXY_GETDICTIONNARY(void) = 6;
		ecs_Result PROXY_GETATTRIBUTEFORMAT(void) = 7;
		ecs_Result PROXY_GETNEXTOBJECT(void) = 8;
		ecs_Result PROXY_GETRASTERINFO(void) = 9;
		ecs_Result PROXY_GETOBJECT(string) = 10;
		ecs_Result PROXY_GETOBJECTIDFROMCOORD(ecs_Coordinate) = 11;
		ecs_Result PROXY_UPDATEDICTIONARY(string) = 12;
		ecs_Result PROXY_GETSERVERPROJECTION(void) = 13;
		ecs_Result PROXY_GETGLOBALBOUND(void) = 14;
		ecs_Result PROXY_SETSERVERLANGUAGE(unsigned int) = 15;
		ecs_Result PROXY_SETSERVERPROJECTION(string) = 16;
		ecs_Result PROXY_SETRASTERCONVERSION(ecs_RasterConversion) = 17;
		ecs_Result PROXY_CREATESERVER(ecs_ProxyCreateServer) = 100;
		ecs_Result PROXY_SETCOMPRESSION(ecs_Compression) = 101;
	} = 1;
} = 0x20000002;

#endif
