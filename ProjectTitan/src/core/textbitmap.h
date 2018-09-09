#pragma once
#include "define.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "camera.h"
#include "program.h"

#define DEFAULT_FONT_SIZE 20

class VertexBuffer;

/*
	SIMPLE TEXT BITMAP VERSION
*/

class TextBitmap
{
private:
	TextBitmap();
public:
	typedef struct tagChar
	{
		UINT texture;
		INT vbo;
		INT top;
		INT left;
		INT width;
		INT rows;
		INT advancex;
	}Char;


	void Init(const CHAR * vs, const CHAR * fs, FLOAT screen_width, FLOAT screen_height, const CHAR * path, INT pixel_size = DEFAULT_FONT_SIZE);
	void SetColor(FLOAT r, FLOAT g, FLOAT b, FLOAT a = 1.0f);
	void SetAlpha(FLOAT a);
	void Draw(const WCHAR * str, FLOAT x, FLOAT y);
	void Release();

	static TextBitmap * mText;

	static TextBitmap * GetInstance();

private:
	void ConvertGRAY2RGBA(FT_Bitmap * bitmap, UCHAR * rgba);
	void LoadChar(WCHAR chr, Char * c);
	void InitCharVBO(VertexBuffer * vb, FLOAT x, FLOAT y, FLOAT width, FLOAT height);
	void DrawBitmap(UINT texture, INT vbo);

	FT_Library mLibrary;
	FT_Face mFace;

	typedef std::map<UINT, Char*>::value_type FontPair;
	std::map<UINT, Char*> mCharTexture;

	Program * mProgram;
	Camera * mCamera;

	VertexBuffer * mVBO;

	cm::vec4 mColor;

	//FLOAT mAlpha;
};

/*
	TEXT BITMAP OLD VERSION
*/

//#include FT_BITMAP_H
//#include FT_OUTLINE_H
//
//#define CFONT_ROW_SPACING 2
//#define CONVERT_FONT_INDEX(x) (((x) < 1 || (x) > (INT)m_fonts.size()) ? -1 : (x) - 1)
//
//#define UCHAR unsigned char
//
//class CFontManager
//{
//public:
//	CFontManager();
//	~CFontManager();
//
//	BOOL initialize(void);
//	void release(void);
//	INT createFont(const char* filename, INT face, INT tall, BOOL bold, BOOL italic, BOOL antialias);
//	BOOL getCharInfo(INT font_index, INT code, INT* wide, INT* tall, INT* horiBearingX, INT* horiBearingY,
//	                 INT* horiAdvance, UINT* texture, FLOAT coords[]);
//	INT getFontTall(INT font_index);
//	void setBold(BOOL bold);
//	void setItalic(BOOL italic);
//
//private:
//	struct FontInfo
//	{
//		INT width;
//		INT height;
//		INT horiBearingX;
//		INT horiBearingY;
//		INT horiAdvance;
//		//INT    vertBearingX;
//		//INT    vertBearingY;
//		//INT    vertAdvance;
//	};
//
//	class CFont
//	{
//	public:
//		CFont();
//		~CFont();
//
//		BOOL create(FT_Library library, const char* filename, FT_Long face_index, INT tall, BOOL bold, BOOL italic,
//		            BOOL antialias);
//		void release(void);
//		BOOL getCharInfo(INT code, FontInfo* metrics, UINT* texture, FLOAT coords[]);
//		INT getFontTall(void);
//		void setBold(BOOL bold);
//		void setItalic(BOOL italic);
//
//	private:
//		BOOL loadChar(INT code, FontInfo* metrics);
//
//		class CChar
//		{
//		public:
//			void setInfo(FontInfo* metrics);
//			void getInfo(FontInfo* metrics, UINT* texture, FLOAT coords[]);
//
//		public:
//			INT m_code;
//			UINT m_texture;
//			FLOAT m_coords[4]; // left top right bottom
//
//		private:
//			FontInfo m_metrics;
//		};
//
//		class CPage
//		{
//		public:
//			CPage();
//			~CPage();
//
//			BOOL append(INT wide, INT tall, UCHAR* rgba, FLOAT coords[]);
//			UINT getTexture(void);
//
//		private:
//			UINT m_texture;
//			INT m_wide;
//			INT m_tall;
//			INT m_posx;
//			INT m_posy;
//			INT m_maxCharTall;
//		};
//
//		typedef std::map<INT, CChar *> TCharMap;
//		TCharMap m_chars;
//		std::vector<CPage *> m_pages;
//
//		FT_Library m_library;
//		FT_Face m_face;
//		BOOL m_antialias;
//		BOOL m_bold;
//		INT m_tall;
//		INT m_rgbaSize;
//		UCHAR* m_rgba;
//	};
//
//	FT_Library m_library;
//	std::vector<CFont *> m_fonts;
//};
//
//class Program;
//class VertexBuffer;
//
//class TextBitmap
//{
//public:
//	TextBitmap();
//	~TextBitmap();
//
//	void Init(const CHAR* filename,FLOAT width, FLOAT height, INT tall, BOOL bold = FALSE, BOOL italic = FALSE, BOOL antialias = TRUE);
//	void Draw(FLOAT x, FLOAT y, const WCHAR* string);
//	void SetBold(BOOL bold);
//	void SetItalic(BOOL italic);
//
//	CFontManager mFontManager;
//	INT mFont;
//	VertexBuffer * mVertexBuffer;
//	Program * mProgram;
//
//	INT mPosLoc, mTexcLoc;
//	INT mUTextLoc;
//
//private:
//	void _draw(UINT texture);
//};
