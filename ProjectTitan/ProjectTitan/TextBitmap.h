#pragma once
#include "define.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_BITMAP_H
#include FT_OUTLINE_H

#define CFONT_ROW_SPACING 2
#define CONVERT_FONT_INDEX(x) (((x) < 1 || (x) > (INT)m_fonts.size()) ? -1 : (x) - 1)

#define UCHAR unsigned char

class CFontManager
{
public:
	CFontManager();
	~CFontManager();

	BOOL initialize(void);
	void release(void);
	INT createFont(const char* filename, INT face, INT tall, BOOL bold, BOOL italic, BOOL antialias);
	BOOL getCharInfo(INT font_index, INT code, INT* wide, INT* tall, INT* horiBearingX, INT* horiBearingY,
	                 INT* horiAdvance, UINT* texture, FLOAT coords[]);
	INT getFontTall(INT font_index);

private:
	struct glyphMetrics
	{
		INT width;
		INT height;
		INT horiBearingX;
		INT horiBearingY;
		INT horiAdvance;
		//INT    vertBearingX;
		//INT    vertBearingY;
		//INT    vertAdvance;
	};

	class CFont
	{
	public:
		CFont();
		~CFont();

		BOOL create(FT_Library library, const char* filename, FT_Long face_index, INT tall, BOOL bold, BOOL italic,
		            BOOL antialias);
		void release(void);
		BOOL getCharInfo(INT code, glyphMetrics* metrics, UINT* texture, FLOAT coords[]);
		INT getFontTall(void);

	private:
		BOOL loadChar(INT code, glyphMetrics* metrics);

		class CChar
		{
		public:
			void setInfo(glyphMetrics* metrics);
			void getInfo(glyphMetrics* metrics, UINT* texture, FLOAT coords[]);

		public:
			INT m_code;
			UINT m_texture;
			FLOAT m_coords[4]; // left top right bottom

		private:
			glyphMetrics m_metrics;
		};

		class CPage
		{
		public:
			CPage();
			~CPage();

			BOOL append(INT wide, INT tall, UCHAR* rgba, FLOAT coords[]);
			UINT getTexture(void);

		private:
			UINT m_texture;
			INT m_wide;
			INT m_tall;
			INT m_posx;
			INT m_posy;
			INT m_maxCharTall;
		};

		typedef std::map<INT, CChar *> TCharMap;
		TCharMap m_chars;
		std::vector<CPage *> m_pages;

		FT_Library m_library;
		FT_Face m_face;
		BOOL m_antialias;
		BOOL m_bold;
		INT m_tall;
		INT m_rgbaSize;
		UCHAR* m_rgba;
	};

	FT_Library m_library;
	std::vector<CFont *> m_fonts;
};

class Program;
class VertexBuffer;

class TextBitmap
{
public:
	TextBitmap();
	~TextBitmap();

	void Init(const CHAR* filename,FLOAT width, FLOAT height, INT tall, BOOL bold = FALSE, BOOL italic = FALSE, BOOL antialias = TRUE);
	void Draw(FLOAT x, FLOAT y, const WCHAR* string);

	CFontManager mFontManager;
	INT mFont;
	VertexBuffer * mVertexBuffer;
	Program * mProgram;

	INT mPosLoc, mTexcLoc;
	INT mUTextLoc;

private:
	void _draw(UINT texture);
};
