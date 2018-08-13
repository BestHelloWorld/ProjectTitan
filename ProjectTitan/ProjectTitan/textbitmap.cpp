#include "textbitmap.h"

#include "model.h"


TextBitmap::TextBitmap()
{
}

void TextBitmap::Init(const CHAR * vs, const CHAR * fs, FLOAT screen_width, FLOAT screen_height, const CHAR * path, INT pixel_size)
{
	FT_Error error;
	error = FT_Init_FreeType(&mLibrary);
	if (error)
	{
		std::cout << "FT_Init_FreeType() Error" << std::endl;
	}

	error = FT_New_Face(mLibrary, path, 0, &mFace);
	if (error)
	{
		std::cout << "FT_New_Face() Error" << std::endl;
	}

	error = FT_Set_Pixel_Sizes(mFace, 0, pixel_size);
	if (error)
	{
		std::cout << "FT_Set_Pixel_Sizes() Error" << std::endl;
	}

	mProgram = new Program;
	mCamera = new Camera;
	mVBO = new VertexBuffer;
	mCamera->Init();
	mCamera->Switch2D(screen_width, screen_height);
	mProgram->Init(vs, fs, mCamera);
	mVBO->SetSize(4);
	mColor = cm::vec4(1.0f);
}

void TextBitmap::SetColor(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
{
	mColor = cm::vec4(r, g, b, a);
}

void TextBitmap::SetAlpha(FLOAT a)
{
	mColor.a = a;
}

void TextBitmap::Draw(const WCHAR * str, FLOAT x, FLOAT y)
{
	INT len = wcslen(str);

	if (len <= 0) return;

	//FT_GlyphSlot slot = mFace->glyph;

	FLOAT pen_x = x;
	FLOAT pen_y = y;


	for (INT i = 0; i < len; ++i)
	{
		auto iter = mCharTexture.find(str[i]);
		Char * chr;

		if (iter != mCharTexture.end())
		{
			chr = iter->second;
		}
		else
		{
			chr = new Char{ 0 };
			LoadChar(str[i], chr);
			mCharTexture.insert(FontPair(str[i], chr));
		}
		InitCharVBO(mVBO, pen_x + chr->left, pen_y + chr->top, chr->width, chr->rows);

		DrawBitmap(chr->texture, mVBO->GetVBO());

		pen_x += chr->advancex;
	}
}

void TextBitmap::Release()
{
	auto iter = mCharTexture.begin();
	while (iter != mCharTexture.end())
	{
		glDeleteTextures(1, &iter->second->texture);
		delete iter->second;

		iter++;
	}
	mCharTexture.clear();

	FT_Done_Face(mFace);
	FT_Done_FreeType(mLibrary);

	mVBO->Clear();
	VertexBuffer::ReleaseVertexBufferObject(mVBO->GetVBO());

	delete mProgram;
	delete mCamera;
	delete mVBO;
}

TextBitmap * TextBitmap::mText = new TextBitmap;

TextBitmap * TextBitmap::GetInstance()
{
	return mText;
}

void TextBitmap::ConvertGRAY2RGBA(FT_Bitmap * bitmap, UCHAR * rgba)
{
	if (!rgba) return;

	int width = bitmap->width;
	int height = bitmap->rows;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			UCHAR c = bitmap->buffer[(height - 1 - i) * width + j];
			//c = c > 0 ? 0xFF : 0x00;
			UINT ind = (i*width + j) * 4;

			rgba[ind] = c;
			rgba[ind + 1] = c;
			rgba[ind + 2] = c;
			rgba[ind + 3] = c;

		}
	}
}

void TextBitmap::LoadChar(WCHAR chr, Char * c)
{
	//std::cout << "LoadChar()" << std::endl;

	FT_Error error;
	error = FT_Load_Char(mFace, chr, FT_LOAD_RENDER);
	if (error) return;

	//GLuint glyph_index = FT_Get_Char_Index(mFace, chr);
	//if (glyph_index <= 0) return;
	//error = FT_Load_Glyph(mFace, glyph_index, FT_LOAD_DEFAULT);
	//if (error) return;
	//error = FT_Outline_Embolden(&mFace->glyph->outline, 30);
	//if (error) return;
	//error = FT_Render_Glyph(mFace->glyph, FT_RENDER_MODE_NORMAL);
	//if (error) return;
	//*bitmap = mFace->glyph->bitmap;

	FT_Bitmap * bitmap = &mFace->glyph->bitmap;

	int width = bitmap->width;
	int height = bitmap->rows;

	c->width = width;
	c->rows = height;
	c->advancex = mFace->glyph->advance.x >> 6;
	c->left = mFace->glyph->bitmap_left;
	c->top = mFace->glyph->bitmap_top;

	UCHAR * rgba = new UCHAR[width * height * 4];
	ConvertGRAY2RGBA(bitmap, rgba);
	glGenTextures(1, &c->texture);
	glBindTexture(GL_TEXTURE_2D, c->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
	glBindTexture(GL_TEXTURE_2D, 0);

	delete rgba;
}

void TextBitmap::InitCharVBO(VertexBuffer * vb, FLOAT x, FLOAT y, FLOAT width, FLOAT height)
{
	// INVERT Y
	//vb->SetTexcoord(0, 0.0f, 1.0f);
	//vb->SetTexcoord(1, 1.0f, 1.0f);
	//vb->SetTexcoord(2, 1.0f, 0.0f);
	//vb->SetTexcoord(3, 0.0f, 0.0f);

	vb->SetTexcoord(0, 0.0f, 0.0f);
	vb->SetTexcoord(1, 1.0f, 0.0f);
	vb->SetTexcoord(2, 1.0f, 1.0f);
	vb->SetTexcoord(3, 0.0f, 1.0f);
	vb->SetPosition(0, x,			y - height, -1.0f);
	vb->SetPosition(1, x + width,	y - height, -1.0f);
	vb->SetPosition(2, x + width,	y,			-1.0f);
	vb->SetPosition(3, x,			y,			-1.0f);

	vb->UpdateData();
}

void TextBitmap::DrawBitmap(UINT texture, INT vbo)
{

	//glBindTexture(GL_TEXTURE_2D, texture);
	//glBegin(GL_TRIANGLE_FAN);
	//glTexCoord2f(0.0f, 1.0f);
	//glVertex3f(x, y + height, -1.0f);
	//glTexCoord2f(0.0f, 0.0f);
	//glVertex3f(x, y, -1.0f);
	//glTexCoord2f(1.0f, 0.0f);
	//glVertex3f(x + width, y, -1.0f);
	//glTexCoord2f(1.0f, 1.0f);
	//glVertex3f(x + width, y + height, -1.0f);
	//glEnd();
	//glBindTexture(GL_TEXTURE_2D, 0);

	mProgram->Bind();

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	INT location;
	location = glGetAttribLocation(mProgram->GetProgramId(), SHADER_ATTRIB_POSITION);
	if (location >= 0)
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)((Vertex*)0)->Position);
	}
	location = glGetAttribLocation(mProgram->GetProgramId(), SHADER_ATTRIB_TEXCOORD);
	if (location >= 0)
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)((Vertex*)0)->Texcoord);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	location = glGetUniformLocation(mProgram->GetProgramId(), SHADER_UNIFORM_MODEL_MATRIX);
	if (location >= 0)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, cm::mat4(1.0f).v);
	}

	location = glGetUniformLocation(mProgram->GetProgramId(), SHADER_SAMPLER2D_MAIN_TEXTURE);
	if (location >= 0 && texture != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(location, 0);
	}

	location = glGetUniformLocation(mProgram->GetProgramId(), SHADER_UNIFORM_COLOR);
	if (location >= 0)
	{
		glUniform4fv(location, 1, mColor.v);
	}

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindTexture(GL_TEXTURE_2D, 0);
	mProgram->Unbind();
}


//
////------------------------------------------------------------
//// CFont
////------------------------------------------------------------
//CFontManager::CFont::CFont()
//{
//	m_face = NULL;
//	m_rgba = NULL;
//	m_antialias = false;
//	m_bold = false;
//	m_tall = 0;
//}
//
//CFontManager::CFont::~CFont()
//{
//	release();
//}
//
//BOOL CFontManager::CFont::create(FT_Library library, const char* filename, FT_Long face_index, INT tall, BOOL bold,
//                                 BOOL italic, BOOL antialias)
//{
//	FT_Error err;
//
//	if (tall > 256)
//	{
//		return false;
//	}
//
//	if ((err = FT_New_Face(library, filename, face_index, &m_face)) != FT_Err_Ok)
//	{
//		printf("FT_New_Face() Error %d\n", err);
//		return false;
//	}
//
//	if ((err = FT_Set_Pixel_Sizes(m_face, 0, tall)) != FT_Err_Ok)
//	{
//		printf("FT_Set_Pixel_Sizes() Error %d\n", err);
//		return false;
//	}
//
//	m_rgbaSize = tall * tall * 6;
//
//	m_rgba = new UCHAR[m_rgbaSize];
//
//	if (m_rgba == NULL)
//	{
//		return false;
//	}
//
//	m_library = library;
//	m_antialias = antialias;
//	m_bold = bold;
//	m_tall = tall;
//
//	if (italic)
//	{
//		FT_Matrix m;
//		m.xx = 0x10000L;
//		m.xy = 0.5f * 0x10000L;
//		m.yx = 0;
//		m.yy = 0x10000L;
//		FT_Set_Transform(m_face, &m, NULL);
//	}
//
//	return true;
//}
//
//void CFontManager::CFont::release(void)
//{
//	FT_Error err;
//
//	if (m_face)
//	{
//		if ((err = FT_Done_Face(m_face)) != FT_Err_Ok)
//		{
//			printf("FT_Done_Face() Error %d\n", err);
//		}
//		m_face = NULL;
//	}
//
//	if (m_rgba)
//	{
//		delete[] m_rgba;
//		m_rgba = NULL;
//	}
//
//	for (TCharMap::iterator it = m_chars.begin(); it != m_chars.end(); it++)
//	{
//		delete it->second;
//		it->second = NULL;
//	}
//
//	m_chars.clear();
//
//	for (size_t i = 0; i < m_pages.size(); i++)
//	{
//		delete m_pages[i];
//		m_pages[i] = NULL;
//	}
//
//	m_pages.clear();
//}
//
//BOOL CFontManager::CFont::getCharInfo(INT code, FontInfo* metrics, UINT* texture, float coords[])
//{
//	TCharMap::iterator it = m_chars.find(code);
//
//	if (it != m_chars.end())
//	{
//		it->second->getInfo(metrics, texture, coords);
//		return true;
//	}
//
//	FontInfo gm;
//
//	if (loadChar(code, &gm) == false)
//	{
//		return false;
//	}
//
//	CChar* ch = new CChar();
//
//	ch->m_code = code;
//	ch->setInfo(&gm);
//
//	for (size_t i = 0; i < m_pages.size(); i++)
//	{
//		CPage* page = m_pages[i];
//
//		if (page->append(gm.width, gm.height, m_rgba, ch->m_coords))
//		{
//			ch->m_texture = page->getTexture();
//			ch->getInfo(metrics, texture, coords);
//			m_chars.insert(TCharMap::value_type(code, ch));
//			return true;
//		}
//	}
//
//	CPage* page = new CPage();
//
//	if (page->append(gm.width, gm.height, m_rgba, ch->m_coords))
//	{
//		ch->m_texture = page->getTexture();
//		ch->getInfo(metrics, texture, coords);
//		m_chars.insert(TCharMap::value_type(code, ch));
//		m_pages.push_back(page);
//		return true;
//	}
//
//	delete ch;
//	delete page;
//
//	return false;
//}
//
//INT CFontManager::CFont::getFontTall(void)
//{
//	return m_tall;
//}
//
//void ConvertMONOToRGBA(FT_Bitmap* source, UCHAR* rgba)
//{
//	UCHAR* s = source->buffer;
//	UCHAR* t = rgba;
//
//	for (UINT y = source->rows; y > 0; y--)
//	{
//		UCHAR* ss = s;
//		UCHAR* tt = t;
//
//		for (UINT x = source->width >> 3; x > 0; x--)
//		{
//			UINT val = *ss;
//
//			for (UINT i = 8; i > 0; i--)
//			{
//				tt[0] = tt[1] = tt[2] = tt[3] = (val & (1 << (i - 1))) ? 0xFF : 0x00;
//				tt += 4;
//			}
//
//			ss += 1;
//		}
//
//		UINT rem = source->width & 7;
//
//		if (rem > 0)
//		{
//			UINT val = *ss;
//
//			for (UINT x = rem; x > 0; x--)
//			{
//				tt[0] = tt[1] = tt[2] = tt[3] = (val & 0x80) ? 0xFF : 0x00;
//				tt += 4;
//				val <<= 1;
//			}
//		}
//
//		s += source->pitch;
//		t += source->width * 4;
//	}
//}
//
//void ConvertGRAYToRGBA(FT_Bitmap* source, UCHAR* rgba)
//{
//	for (UINT y = 0; y < source->rows; y++)
//	{
//		for (UINT x = 0; x < source->width; x++)
//		{
//			UCHAR* s = &source->buffer[(y * source->pitch) + x];
//			UCHAR* t = &rgba[((y * source->pitch) + x) * 4];
//
//			t[0] = t[1] = t[2] = (*s > 0 ? 0xFF : 0x00); //0xFF;
//			t[3] = *s;
//		}
//	}
//}
//
//BOOL CFontManager::CFont::loadChar(INT code, FontInfo* metrics)
//{
//	FT_Error err;
//
//	UINT glyph_index = FT_Get_Char_Index(m_face, (FT_ULong)code);
//
//	if (glyph_index > 0)
//	{
//		if ((err = FT_Load_Glyph(m_face, glyph_index, FT_LOAD_DEFAULT)) == FT_Err_Ok)
//		{
//			FT_GlyphSlot glyph = m_face->glyph;
//
//			FT_Render_Mode render_mode = m_antialias ? FT_RENDER_MODE_NORMAL : FT_RENDER_MODE_MONO;
//
//			if (m_antialias && m_bold)
//			{
//				if ((err = FT_Outline_EmboldenXY(&glyph->outline, 60, 60)) != FT_Err_Ok)
//				{
//					printf("FT_Outline_EmboldenXY() Error %d\n", err);
//				}
//			}
//
//			if ((err = FT_Render_Glyph(glyph, render_mode)) == FT_Err_Ok)
//			{
//				FT_Bitmap* bitmap = &glyph->bitmap;
//
//				switch (bitmap->pixel_mode)
//				{
//				case FT_PIXEL_MODE_MONO:
//					{
//						if (!m_antialias && m_bold)
//						{
//							if ((err = FT_Bitmap_Embolden(m_library, bitmap, 60, 0)) != FT_Err_Ok)
//							{
//								printf("FT_Bitmap_Embolden() Error %d\n", err);
//							}
//						}
//						ConvertMONOToRGBA(bitmap, m_rgba);
//						break;
//					}
//				case FT_PIXEL_MODE_GRAY:
//					{
//						ConvertGRAYToRGBA(bitmap, m_rgba);
//						break;
//					}
//				default:
//					{
//						memset(m_rgba, 0xFF, m_rgbaSize);
//						break;
//					}
//				}
//
//				metrics->width = bitmap->width;
//				metrics->height = bitmap->rows;
//				metrics->horiBearingX = glyph->bitmap_left;
//				metrics->horiBearingY = glyph->bitmap_top;
//				metrics->horiAdvance = glyph->advance.x >> 6;
//
//				return true;
//			}
//			else
//			{
//				printf("FT_Render_Glyph() Error %d\n", err);
//			}
//		}
//		else
//		{
//			printf("FT_Load_Glyph() Error %d\n", err);
//		}
//	}
//
//	memset(metrics, 0, sizeof(FontInfo));
//
//	return false;
//}
//
//void CFontManager::CFont::setBold(BOOL bold)
//{
//	m_bold = bold;
//	m_chars.clear();
//}
//
//void CFontManager::CFont::setItalic(BOOL italic)
//{
//	if (italic)
//	{
//		FT_Matrix m;
//		m.xx = 0x10000L;
//		m.xy = 0.5f * 0x10000L;
//		m.yx = 0;
//		m.yy = 0x10000L;
//		FT_Set_Transform(m_face, &m, NULL);
//	}
//	m_chars.clear();
//}
//
//
////------------------------------------------------------------
//// CChar
////------------------------------------------------------------
//void CFontManager::CFont::CChar::setInfo(FontInfo* metrics)
//{
//	memcpy(&m_metrics, metrics, sizeof(FontInfo));
//}
//
//void CFontManager::CFont::CChar::getInfo(FontInfo* metrics, UINT* texture, float coords[])
//{
//	memcpy(metrics, &m_metrics, sizeof(FontInfo));
//
//	*texture = m_texture;
//	memcpy(coords, m_coords, sizeof(float) * 4);
//}
//
////------------------------------------------------------------
//// CPage
////------------------------------------------------------------
//CFontManager::CFont::CPage::CPage()
//{
//	m_wide = m_tall = 256;
//	m_posx = m_posy = 0;
//
//	m_maxCharTall = 0;
//
//	glGenTextures(1, &m_texture);
//	glBindTexture(GL_TEXTURE_2D, m_texture);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_wide, m_tall, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//}
//
//CFontManager::CFont::CPage::~CPage()
//{
//}
//
//BOOL CFontManager::CFont::CPage::append(INT wide, INT tall, UCHAR* rgba, float coords[])
//{
//	if (m_posy + tall > m_tall)
//	{
//		return false;
//	}
//
//	if (m_posx + wide > m_wide)
//	{
//		INT newLineY = m_posy + m_maxCharTall;
//
//		if (newLineY + tall > m_tall)
//		{
//			return false;
//		}
//
//		m_posx = 0;
//		m_posy = newLineY;
//		m_maxCharTall = 0;
//	}
//
//	glBindTexture(GL_TEXTURE_2D, m_texture);
//	glTexSubImage2D(GL_TEXTURE_2D, 0, m_posx, m_posy, wide, tall, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
//
//	coords[0] = m_posx / (float)m_wide; // left
//	coords[1] = m_posy / (float)m_tall; // top
//	coords[2] = (m_posx + wide) / (float)m_wide; // right
//	coords[3] = (m_posy + tall) / (float)m_tall; // bottom
//
//	m_posx += wide;
//
//	if (tall > m_maxCharTall)
//	{
//		m_maxCharTall = tall;
//	}
//
//	return true;
//}
//
//UINT CFontManager::CFont::CPage::getTexture(void)
//{
//	return m_texture;
//}
//
////------------------------------------------------------------
//// CFontManager
////------------------------------------------------------------
//CFontManager::CFontManager()
//{
//	m_library = NULL;
//}
//
//CFontManager::~CFontManager()
//{
//	release();
//}
//
//BOOL CFontManager::initialize(void)
//{
//	FT_Error err;
//
//	if ((err = FT_Init_FreeType(&m_library)) != FT_Err_Ok)
//	{
//		printf("FT_Init_FreeType() Error %d\n", err);
//		return false;
//	}
//
//	return true;
//}
//
//void CFontManager::release(void)
//{
//	FT_Error err;
//
//	for (size_t i = 0; i < m_fonts.size(); i++)
//	{
//		delete m_fonts[i];
//		m_fonts[i] = NULL;
//	}
//
//	m_fonts.clear();
//
//	if ((err = FT_Done_FreeType(m_library)) != FT_Err_Ok)
//	{
//		printf("FT_Done_FreeType() Error %d\n");
//	}
//}
//
//INT CFontManager::createFont(const char* filename, INT face, INT tall, BOOL bold, BOOL italic, BOOL antialias)
//{
//	CFont* font = new CFont();
//
//	if (font->create(m_library, filename, face, tall, bold, italic, antialias) != true)
//	{
//		delete font;
//		return 0;
//	}
//
//	m_fonts.push_back(font);
//
//	return m_fonts.size();
//}
//
//
//BOOL CFontManager::getCharInfo(INT font_index, INT code, INT* wide, INT* tall, INT* horiBearingX, INT* horiBearingY,
//                               INT* horiAdvance, UINT* texture, float coords[])
//{
//	INT i = CONVERT_FONT_INDEX(font_index);
//
//	if (i == -1)
//	{
//		return false;
//	}
//
//	CFont* font = m_fonts[i];
//
//	FontInfo metrics;
//
//	if (font->getCharInfo(code, &metrics, texture, coords) == false)
//	{
//		return false;
//	}
//
//	*wide = metrics.width;
//	*tall = metrics.height;
//	*horiBearingX = metrics.horiBearingX;
//	*horiBearingY = metrics.horiBearingY;
//	*horiAdvance = metrics.horiAdvance;
//
//	return true;
//}
//
//INT CFontManager::getFontTall(INT font_index)
//{
//	INT i = CONVERT_FONT_INDEX(font_index);
//
//	if (i == -1)
//	{
//		return false;
//	}
//
//	CFont* font = m_fonts[i];
//
//	return font->getFontTall();
//}
//
//void CFontManager::setBold(BOOL bold)
//{
//	for(int i = 0; i < m_fonts.size(); ++i)
//	{
//		m_fonts[i]->setBold(bold);
//	}
//}
//
//void CFontManager::setItalic(BOOL italic)
//{
//	for (int i = 0; i < m_fonts.size(); ++i)
//	{
//		m_fonts[i]->setItalic(italic);
//	}
//}
//
//
////------------------------------------------------------------
//// TextBitmap
////------------------------------------------------------------
//TextBitmap::TextBitmap()
//{
//}
//
//TextBitmap::~TextBitmap()
//{
//}
//
//void TextBitmap::Init(const CHAR* filename, FLOAT width, FLOAT height, INT tall, BOOL bold, BOOL italic, BOOL antialias)
//{
//	mFontManager.initialize();
//
//	mFont = mFontManager.createFont(filename, 0, tall, bold, italic, antialias);
//
//	if (mFont == 0)
//	{
//		printf("createFont failed\n");
//	}
//
//	mVertexBuffer = new VertexBuffer;
//	mProgram = new Program;
//
//	mVertexBuffer->SetSize(4);
//
//	Camera* cam = new Camera;
//	cam->Init();
//	cam->Switch2D(width, height);
//	mProgram->Init("res/shader/v.vs", "res/shader/f.fs", cam);
//
//	mPosLoc = glGetAttribLocation(mProgram->GetProgramId(), "position");
//	mTexcLoc = glGetAttribLocation(mProgram->GetProgramId(), "texcoord");
//	mUTextLoc = glGetUniformLocation(mProgram->GetProgramId(), "U_Texture");
//}
//
//void TextBitmap::Draw(FLOAT x, FLOAT y, const WCHAR* string)
//{
//	if (!mFont)
//		return;
//
//	int tall = mFontManager.getFontTall(mFont);
//
//	int dx = x;
//	int dy = y;
//
//	while (*string)
//	{
//		if (*string == L'\n')
//		{
//			string++;
//			dx = x;
//			dy -= tall + CFONT_ROW_SPACING;
//			continue;
//		}
//
//		int cw, ct, bx, by, av;
//		GLuint glt;
//		float crd[4];
//
//		if (!mFontManager.getCharInfo(mFont, *string, &cw, &ct, &bx, &by, &av, &glt, crd))
//		{
//			string++;
//			continue;
//		}
//
//
//		int px = dx + bx;
//		int py = dy; // -by;
//
//
//		mVertexBuffer->SetTexcoord(0, crd[0], crd[3]);
//		mVertexBuffer->SetTexcoord(1, crd[2], crd[3]);
//		mVertexBuffer->SetTexcoord(2, crd[2], crd[1]);
//		mVertexBuffer->SetTexcoord(3, crd[0], crd[1]);
//
//		mVertexBuffer->SetPosition(0, px, py, -1.0f);
//		mVertexBuffer->SetPosition(1, px + cw, py, -1.0f);
//		mVertexBuffer->SetPosition(2, px + cw, py + ct, -1.0f);
//		mVertexBuffer->SetPosition(3, px, py + ct, -1.0f);
//
//		mVertexBuffer->UpdateData();
//
//
//		_draw(glt);
//
//		dx += av;
//
//		string++;
//	}
//}
//
//void TextBitmap::SetBold(BOOL bold)
//{
//	mFontManager.setBold(bold);
//}
//
//void TextBitmap::SetItalic(BOOL italic)
//{
//	mFontManager.setItalic(italic);
//}
//
//void TextBitmap::_draw(UINT texture)
//{
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	mProgram->Bind();
//	mVertexBuffer->Bind();
//
//	glEnableVertexAttribArray(mPosLoc);
//	glEnableVertexAttribArray(mTexcLoc);
//	glVertexAttribPointer(mPosLoc, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)((Vertex*)0)->Position);
//	glVertexAttribPointer(mTexcLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)((Vertex*)0)->Texcoord);
//
//	mVertexBuffer->Unbind();
//
//	INT location = glGetUniformLocation(mProgram->GetProgramId(), SHADER_UNIFORM_MODEL_MATRIX);
//	if (location >= 0)
//		glUniformMatrix4fv(location, 1, GL_FALSE, esm::mat4(1.0f).v);
//
//	if (mUTextLoc >= 0 && texture != -1)
//	{
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, texture);
//		glUniform1i(mUTextLoc, 0);
//	}
//
//	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
//
//	mProgram->Unbind();
//
//	glDisable(GL_BLEND);
//}
