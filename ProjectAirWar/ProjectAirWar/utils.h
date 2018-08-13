#ifndef __UTILS_H
#define __UTILS_H

#include "define.h"

#ifdef __ANDROID__
// #define AAssetManager void
void SetAssetManager(AAssetManager *am);
ULONG GetTickCount();
#endif

UCHAR *LoadFileContent(const CHAR *path, INT &fileSize);
UCHAR *DecodeBMP(UCHAR *bmpFileData, INT &width, INT &height);
UINT GetTimeElapse(UINT * time);
UINT CreateTexture2D(UCHAR *pixelData, INT width, INT height, ENUM type);
void CheckError(const char*file, const int line);
UINT CreateTexture2DFromBMP(const CHAR *path);
UINT CreateTextureCubeFromBMP(const CHAR* frontPath, const CHAR* backPath, const CHAR* leftPath, const CHAR* rightPath, const CHAR* topPath, const CHAR* bottomPath);

#define GL_CALL CheckError(__FILE__, __LINE__)


#endif