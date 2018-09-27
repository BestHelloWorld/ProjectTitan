#include "define.h"

#include "utils.h"

#ifdef __ANDROID__
static AAssetManager *assetManager = 0;

void SetAssetManager(AAssetManager *am)
{
	assetManager = am;
}
ULONG GetTickCount()
{
	return 0;
}
#endif

#ifdef linux
ULONG
GetTickCount()
{
	float t = (float)clock() / (float)CLOCKS_PER_SEC;
	return (ULONG)(t * 1000.0f);
}
#endif

UCHAR *
LoadFileContent(const CHAR *path, INT &fileSize)
{
#ifdef __ANDROID__
	UCHAR *fileContent = 0;
	if (assetManager == 0)
	{
		LOG_E("AssetManager error: no set AssetManager : %X", assetManager);
		return 0;
	}
	AAsset *asset = AAssetManager_open(assetManager, path, AASSET_MODE_UNKNOWN);
	if (asset == 0)
	{
		LOG_E("Load file error");
		return 0;
	}

	size_t len = AAsset_getLength(asset);
	fileSize = len;
	fileContent = new UCHAR[len + 1];
	AAsset_read(asset, (void *)fileContent, len);
	fileContent[len] = '\0';

	LOG_D("Load file success : length %d ", len);
	GL_CALL;
	return fileContent;
#else
	FILE*pFile = fopen(path, "rb");
	if (pFile)
	{
		fseek(pFile, 0, SEEK_END);
		INT nLen = ftell(pFile);
		fileSize = nLen;
		UCHAR*buffer = new UCHAR[nLen + 1];
		rewind(pFile);
		fread(buffer, sizeof(char), nLen + 1, pFile);
		fclose(pFile);
		buffer[nLen] = '\0';
		return buffer;
	}

	return 0;
#endif

}

void
CheckError(const char*file, const int line)
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		switch (error)
		{
		case GL_INVALID_ENUM:
			LOG_D("error:GL_INVALID_GLenum %s : %d\n", file, line)
				;
			break;
		case GL_INVALID_VALUE:
			LOG_D("error:GL_INVALID_VALUE %s : %d\n", file, line)
				;
			break;
		case GL_INVALID_OPERATION:
			LOG_D("error:GL_INVALID_OPERATION %s : %d\n", file, line)
				;
			break;
#ifndef GL_ES_VERSION_2_0
#ifndef GL_ES_VERSION_3_0
		case GL_STACK_OVERFLOW:
			LOG_D("error:GL_STACK_OVERFLOW %s : %d\n", file, line)
				;
			break;
		case GL_STACK_UNDERFLOW:
			LOG_D("error:GL_STACK_UNDERFLOW %s : %d\n", file, line)
				;
			break;
#endif
#endif
		case GL_OUT_OF_MEMORY:
			LOG_D("error:GL_OUT_OF_MEMORY %s : %d\n", file, line)
				;
			break;
		}
	}
}

UCHAR *
DecodeBMP(UCHAR *bmpFileData, INT &width, INT &height)
{
	if (*(SHORT*)bmpFileData == 0x4d42)
	{
		width = *(INT*)(bmpFileData + 18);
		height = *(INT *)(bmpFileData + 22);
		INT offset = *(INT*)(bmpFileData + 10);

		UCHAR* pixelData = bmpFileData + offset;
		UCHAR tmp;
		for (int i = 0; i < width * height * 3; i += 3)
		{
			tmp = pixelData[i];
			pixelData[i] = pixelData[i + 2];
			pixelData[i + 2] = tmp;
		}
		return pixelData;
	}
	return 0;
}

FLOAT
GetTimeElapse(UINT * time)
{
	if (!time)
	{
		return 0;
	}
	if (!*time)
	{
		*time = clock();
		return 0;
	}

	UINT currentTime = clock() - *time;
	*time = clock();
	return currentTime / (FLOAT)CLOCKS_PER_SEC * 1000.0f;
}

UINT
CreateTexture2D(UCHAR *pixelData, INT width, INT height, GLenum type)
{
	UINT texture = 0;
	glGenTextures(1, &texture);																   GL_CALL;
	glBindTexture(GL_TEXTURE_2D, texture);													   GL_CALL;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);						   GL_CALL;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);						   GL_CALL;
	glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, pixelData); GL_CALL;
	glBindTexture(GL_TEXTURE_2D, 0);														   GL_CALL;

	return texture;
}

UINT CreateTextureDepth(FLOAT * pixelData, INT width, INT height, GLenum type)
{
	UINT texture = 0;
	glGenTextures(1, &texture);															  GL_CALL;
	glBindTexture(GL_TEXTURE_2D, texture);												  GL_CALL;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);					  GL_CALL;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);					  GL_CALL;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, type, GL_FLOAT, pixelData);	  GL_CALL;
	glBindTexture(GL_TEXTURE_2D, 0);													  GL_CALL;

	return texture;
}

UINT
CreateTexture2DFromBMP(const CHAR* path)
{
	INT fileSize = 0;
	GL_CALL; UCHAR* fileContent = LoadFileContent(path, fileSize);
	if (fileContent == 0)
		return NEGATIVE_ONE;

	INT imgWidth, imgHeight;
	GL_CALL; UCHAR* imgData = DecodeBMP(fileContent, imgWidth, imgHeight);
	if (imgData == 0)
		return NEGATIVE_ONE;

	GL_CALL; UINT texture = CreateTexture2D(imgData, imgWidth, imgHeight, GL_RGB);
	delete fileContent;

	return texture;
}

UINT
CreateTextureCubeFromBMP(const CHAR* frontPath, const CHAR* backPath,
	const CHAR* leftPath, const CHAR* rightPath,
	const CHAR* topPath, const CHAR* bottomPath)
{
	INT fileSize = 0;
	INT width = 0, height = 0;
	UINT texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	UCHAR* frontFileContent = LoadFileContent(frontPath, fileSize);
	UCHAR* frontImgData = DecodeBMP(frontFileContent, width, height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, frontImgData);
	delete frontFileContent;

	UCHAR *bottomFileContent = LoadFileContent(bottomPath, fileSize);
	UCHAR *bottomImgData = DecodeBMP(bottomFileContent, width, height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, bottomImgData);
	delete bottomFileContent;

	UCHAR *leftFileContent = LoadFileContent(leftPath, fileSize);
	UCHAR *leftImgData = DecodeBMP(leftFileContent, width, height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, leftImgData);
	delete leftFileContent;

	UCHAR *backFileContent = LoadFileContent(backPath, fileSize);
	UCHAR *backImgData = DecodeBMP(backFileContent, width, height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, backImgData);
	delete backFileContent;

	UCHAR *topFileContent = LoadFileContent(topPath, fileSize);
	UCHAR *topImgData = DecodeBMP(topFileContent, width, height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, topImgData);
	delete topFileContent;

	UCHAR *rightFileContent = LoadFileContent(rightPath, fileSize);
	UCHAR *rightImgData = DecodeBMP(rightFileContent, width, height);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, rightImgData);
	delete rightFileContent;

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	GL_CALL;
	return texture;
}
