#include "TextBitmap.h"

#include "model.h"
#include "camera.h"
#include "program.h"

//------------------------------------------------------------
// CFont
//------------------------------------------------------------
CFontManager::CFont::CFont()
{
	m_face = NULL;
	m_rgba = NULL;
	m_antialias = false;
	m_bold = false;
	m_tall = 0;
}

CFontManager::CFont::~CFont()
{
	release();
}

BOOL CFontManager::CFont::create(FT_Library library, const char* filename, FT_Long face_index, INT tall, BOOL bold,
                                 BOOL italic, BOOL antialias)
{
	FT_Error err;

	if (tall > 256)
	{
		// Bigger than a page size
		return false;
	}

	if ((err = FT_New_Face(library, filename, face_index, &m_face)) != FT_Err_Ok)
	{
		printf("FT_New_Face() Error %d\n", err);
		return false;
	}

	if ((err = FT_Set_Pixel_Sizes(m_face, 0, tall)) != FT_Err_Ok)
	{
		printf("FT_Set_Pixel_Sizes() Error %d\n", err);
		return false;
	}

	m_rgbaSize = (tall * 2) * tall * 4;

	m_rgba = new UCHAR[m_rgbaSize];

	if (m_rgba == NULL)
	{
		return false;
	}

	m_library = library;
	m_antialias = antialias;
	m_bold = bold;
	m_tall = tall;

	if (italic)
	{
		FT_Matrix m;
		m.xx = 0x10000L;
		m.xy = 0.5f * 0x10000L;
		m.yx = 0;
		m.yy = 0x10000L;
		FT_Set_Transform(m_face, &m, NULL);
	}

	return true;
}

void CFontManager::CFont::release(void)
{
	FT_Error err;

	if (m_face)
	{
		if ((err = FT_Done_Face(m_face)) != FT_Err_Ok)
		{
			printf("FT_Done_Face() Error %d\n", err);
		}
		m_face = NULL;
	}

	if (m_rgba)
	{
		delete[] m_rgba;
		m_rgba = NULL;
	}

	for (TCharMap::iterator it = m_chars.begin(); it != m_chars.end(); it++)
	{
		delete it->second;
		it->second = NULL;
	}

	m_chars.clear();

	for (size_t i = 0; i < m_pages.size(); i++)
	{
		delete m_pages[i];
		m_pages[i] = NULL;
	}

	m_pages.clear();
}

BOOL CFontManager::CFont::getCharInfo(INT code, glyphMetrics* metrics, UINT* texture, float coords[])
{
	// fast find it
	TCharMap::iterator it = m_chars.find(code);

	if (it != m_chars.end())
	{
		it->second->getInfo(metrics, texture, coords);
		return true;
	}

	glyphMetrics gm;

	if (loadChar(code, &gm) == false)
	{
		return false;
	}

	CChar* ch = new CChar();

	ch->m_code = code;
	ch->setInfo(&gm);

	for (size_t i = 0; i < m_pages.size(); i++)
	{
		CPage* page = m_pages[i];

		if (page->append(gm.width, gm.height, m_rgba, ch->m_coords))
		{
			ch->m_texture = page->getTexture();
			ch->getInfo(metrics, texture, coords);
			m_chars.insert(TCharMap::value_type(code, ch));
			return true;
		}
	}

	CPage* page = new CPage();

	if (page->append(gm.width, gm.height, m_rgba, ch->m_coords))
	{
		ch->m_texture = page->getTexture();
		ch->getInfo(metrics, texture, coords);
		m_chars.insert(TCharMap::value_type(code, ch));
		m_pages.push_back(page);
		return true;
	}

	delete ch;
	delete page;

	return false;
}

INT CFontManager::CFont::getFontTall(void)
{
	return m_tall;
}

void ConvertMONOToRGBA(FT_Bitmap* source, UCHAR* rgba)
{
	UCHAR* s = source->buffer;
	UCHAR* t = rgba;

	for (UINT y = source->rows; y > 0; y--)
	{
		UCHAR* ss = s;
		UCHAR* tt = t;

		for (UINT x = source->width >> 3; x > 0; x--)
		{
			UINT val = *ss;

			for (UINT i = 8; i > 0; i--)
			{
				tt[0] = tt[1] = tt[2] = tt[3] = (val & (1 << (i - 1))) ? 0xFF : 0x00;
				tt += 4;
			}

			ss += 1;
		}

		UINT rem = source->width & 7;

		if (rem > 0)
		{
			UINT val = *ss;

			for (UINT x = rem; x > 0; x--)
			{
				tt[0] = tt[1] = tt[2] = tt[3] = (val & 0x80) ? 0xFF : 0x00;
				tt += 4;
				val <<= 1;
			}
		}

		s += source->pitch;
		t += source->width * 4; //pitch
	}
}

//GRAY模式1个像素用1个字节保存。
void ConvertGRAYToRGBA(FT_Bitmap* source, UCHAR* rgba)
{
	for (UINT y = 0; y < source->rows; y++)
	{
		for (UINT x = 0; x < source->width; x++)
		{
			UCHAR* s = &source->buffer[(y * source->pitch) + x];
			UCHAR* t = &rgba[((y * source->pitch) + x) * 4];

			t[0] = t[1] = t[2] = (*s > 0 ? 0xFF : 0x00); //0xFF;
			t[3] = *s;
		}
	}
}

BOOL CFontManager::CFont::loadChar(INT code, glyphMetrics* metrics)
{
	FT_Error err;

	UINT glyph_index = FT_Get_Char_Index(m_face, (FT_ULong)code);

	if (glyph_index > 0)
	{
		if ((err = FT_Load_Glyph(m_face, glyph_index, FT_LOAD_DEFAULT)) == FT_Err_Ok)
		{
			FT_GlyphSlot glyph = m_face->glyph;

			FT_Render_Mode render_mode = m_antialias ? FT_RENDER_MODE_NORMAL : FT_RENDER_MODE_MONO;

			if (m_antialias && m_bold)
			{
				if ((err = FT_Outline_EmboldenXY(&glyph->outline, 60, 60)) != FT_Err_Ok)
				{
					printf("FT_Outline_EmboldenXY() Error %d\n", err);
				}
			}

			if ((err = FT_Render_Glyph(glyph, render_mode)) == FT_Err_Ok)
			{
				FT_Bitmap* bitmap = &glyph->bitmap;

				switch (bitmap->pixel_mode)
				{
				case FT_PIXEL_MODE_MONO:
					{
						if (!m_antialias && m_bold)
						{
							if ((err = FT_Bitmap_Embolden(m_library, bitmap, 60, 0)) != FT_Err_Ok)
							{
								printf("FT_Bitmap_Embolden() Error %d\n", err);
							}
						}
						ConvertMONOToRGBA(bitmap, m_rgba);
						break;
					}
				case FT_PIXEL_MODE_GRAY:
					{
						ConvertGRAYToRGBA(bitmap, m_rgba);
						break;
					}
				default:
					{
						memset(m_rgba, 0xFF, m_rgbaSize);
						break;
					}
				}

				metrics->width = bitmap->width;
				metrics->height = bitmap->rows;
				metrics->horiBearingX = glyph->bitmap_left;
				metrics->horiBearingY = glyph->bitmap_top;
				metrics->horiAdvance = glyph->advance.x >> 6;

				return true;
			}
			else
			{
				printf("FT_Render_Glyph() Error %d\n", err);
			}
		}
		else
		{
			printf("FT_Load_Glyph() Error %d\n", err);
		}
	}

	memset(metrics, 0, sizeof(glyphMetrics));

	return false;
}

//------------------------------------------------------------
// CChar
//------------------------------------------------------------
void CFontManager::CFont::CChar::setInfo(glyphMetrics* metrics)
{
	memcpy(&m_metrics, metrics, sizeof(glyphMetrics));
}

void CFontManager::CFont::CChar::getInfo(glyphMetrics* metrics, UINT* texture, float coords[])
{
	memcpy(metrics, &m_metrics, sizeof(glyphMetrics));

	*texture = m_texture;
	memcpy(coords, m_coords, sizeof(float) * 4);
}

//------------------------------------------------------------
// CPage
//------------------------------------------------------------
CFontManager::CFont::CPage::CPage()
{
	m_wide = m_tall = 256;
	m_posx = m_posy = 0;

	// In a line, for a max height character
	m_maxCharTall = 0;

	glGenTextures(1, &m_texture); // Using your API here
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_wide, m_tall, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

CFontManager::CFont::CPage::~CPage()
{
	// free the texture
}

BOOL CFontManager::CFont::CPage::append(INT wide, INT tall, UCHAR* rgba, float coords[])
{
	if (m_posy + tall > m_tall)
	{
		// not enough line space in this page
		return false;
	}

	// If this line is full ...
	if (m_posx + wide > m_wide)
	{
		INT newLineY = m_posy + m_maxCharTall;

		if (newLineY + tall > m_tall)
		{
			// No more space for new line in this page, should allocate a new one
			return false;
		}

		// Begin in new line
		m_posx = 0;
		m_posy = newLineY;
		// Reset
		m_maxCharTall = 0;
	}

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, m_posx, m_posy, wide, tall, GL_RGBA, GL_UNSIGNED_BYTE, rgba);

	coords[0] = m_posx / (float)m_wide; // left
	coords[1] = m_posy / (float)m_tall; // top
	coords[2] = (m_posx + wide) / (float)m_wide; // right
	coords[3] = (m_posy + tall) / (float)m_tall; // bottom

	m_posx += wide;

	if (tall > m_maxCharTall)
	{
		m_maxCharTall = tall;
	}

	return true;
}

UINT CFontManager::CFont::CPage::getTexture(void)
{
	return m_texture;
}

//------------------------------------------------------------
// CFontManager
//------------------------------------------------------------
CFontManager::CFontManager()
{
	m_library = NULL;
}

CFontManager::~CFontManager()
{
	release();
}

BOOL CFontManager::initialize(void)
{
	FT_Error err;

	if ((err = FT_Init_FreeType(&m_library)) != FT_Err_Ok)
	{
		printf("FT_Init_FreeType() Error %d\n", err);
		return false;
	}

	return true;
}

void CFontManager::release(void)
{
	FT_Error err;

	for (size_t i = 0; i < m_fonts.size(); i++)
	{
		delete m_fonts[i];
		m_fonts[i] = NULL;
	}

	m_fonts.clear();

	if ((err = FT_Done_FreeType(m_library)) != FT_Err_Ok)
	{
		printf("FT_Done_FreeType() Error %d\n");
	}
}

INT CFontManager::createFont(const char* filename, INT face, INT tall, BOOL bold, BOOL italic, BOOL antialias)
{
	CFont* font = new CFont();

	if (font->create(m_library, filename, face, tall, bold, italic, antialias) != true)
	{
		delete font;
		return 0;
	}

	m_fonts.push_back(font);

	return m_fonts.size();
}


BOOL CFontManager::getCharInfo(INT font_index, INT code, INT* wide, INT* tall, INT* horiBearingX, INT* horiBearingY,
                               INT* horiAdvance, UINT* texture, float coords[])
{
	INT i = CONVERT_FONT_INDEX(font_index);

	if (i == -1)
	{
		return false;
	}

	CFont* font = m_fonts[i];

	glyphMetrics metrics;

	if (font->getCharInfo(code, &metrics, texture, coords) == false)
	{
		return false;
	}

	*wide = metrics.width;
	*tall = metrics.height;
	*horiBearingX = metrics.horiBearingX;
	*horiBearingY = metrics.horiBearingY;
	*horiAdvance = metrics.horiAdvance;

	return true;
}

INT CFontManager::getFontTall(INT font_index)
{
	INT i = CONVERT_FONT_INDEX(font_index);

	if (i == -1)
	{
		return false;
	}

	CFont* font = m_fonts[i];

	return font->getFontTall();
}


TextBitmap::TextBitmap()
{
}

TextBitmap::~TextBitmap()
{
	//mFontManager.release();
}

void TextBitmap::Init(const CHAR* filename, FLOAT width, FLOAT height, INT tall, BOOL bold, BOOL italic, BOOL antialias)
{
	mFontManager.initialize();

	mFont = mFontManager.createFont(filename, 0, tall, bold, italic, antialias);

	if (mFont == 0)
	{
		printf("createFont failed\n");
	}

	mVertexBuffer = new VertexBuffer;
	mProgram = new Program;

	mVertexBuffer->SetSize(4);


	Camera* cam = new Camera;
	cam->Init();
	cam->Switch2D(width, height);
	mProgram->Init("res/shader/v.vs", "res/shader/f.fs", cam);

	mPosLoc = glGetAttribLocation(mProgram->GetProgramId(), "position");
	mTexcLoc = glGetAttribLocation(mProgram->GetProgramId(), "texcoord");
	mUTextLoc = glGetUniformLocation(mProgram->GetProgramId(), "U_Texture");
}

void TextBitmap::Draw(FLOAT x, FLOAT y, const WCHAR* string)
{
	if (!mFont)
		return;

	int tall = mFontManager.getFontTall(mFont);

	int dx = x;
	int dy = y;

	while (*string)
	{
		if (*string == L'\n')
		{
			string++;
			dx = x;
			dy -= tall + CFONT_ROW_SPACING;
			continue;
		}

		int cw, ct, bx, by, av;
		GLuint glt;
		float crd[4];

		if (!mFontManager.getCharInfo(mFont, *string, &cw, &ct, &bx, &by, &av, &glt, crd))
		{
			string++;
			continue;
		}


		int px = dx + bx;
		int py = dy;// -by;


		mVertexBuffer->SetTexcoord(0, crd[0], crd[3]);
		mVertexBuffer->SetTexcoord(1, crd[2], crd[3]);
		mVertexBuffer->SetTexcoord(2, crd[2], crd[1]);
		mVertexBuffer->SetTexcoord(3, crd[0], crd[1]);

		mVertexBuffer->SetPosition(0, px, py, -1.0f);
		mVertexBuffer->SetPosition(1, px + cw, py, -1.0f);
		mVertexBuffer->SetPosition(2, px + cw, py + ct, -1.0f);
		mVertexBuffer->SetPosition(3, px, py + ct, -1.0f);

		_draw(glt);

		dx += av;

		string++;
	}
}


void TextBitmap::_draw(UINT texture)
{
	//if (texture != 0)
	//{
	//	glBindTexture(GL_TEXTURE_2D, texture);
	//}

	//glPointSize(100.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mProgram->Bind();
	mVertexBuffer->Bind();

	glEnableVertexAttribArray(mPosLoc);
	glEnableVertexAttribArray(mTexcLoc);
	glVertexAttribPointer(mPosLoc, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)((Vertex*)0)->Position);
	glVertexAttribPointer(mTexcLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)((Vertex*)0)->Texcoord);

	mVertexBuffer->Unbind();

	INT location = glGetUniformLocation(mProgram->GetProgramId(), SHADER_UNIFORM_MODEL_MATRIX);
	if (location >= 0)
		glUniformMatrix4fv(location, 1, GL_FALSE, esm::value_ptr(esm::mat4(1.0f)));

	if (mUTextLoc >= 0 && texture != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(mUTextLoc, 0);
	}

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	mProgram->Unbind();

	glDisable(GL_BLEND);
}
