#ifndef __FRAME_BUFFER_H
#define __FRAME_BUFFER_H

#include "define.h"
#include "utils.h"

class FrameBuffer
{
public:
	void Init();
	void AttachColorBuffer(const CHAR *bufferName, ENUM attachment, INT width, INT height);
	void AttachDepthBuffer(const CHAR *bufferName, INT width, INT height);
	void Bind();
	void Unbind();
	UINT GetBuffer(const CHAR*bufferName);
	void AttachFinish();

	void Release();

private:
    UINT mFrameBufferObject;
    INT mPrevFrameBuffer;
    std::map<std::string, UINT> mBufferTextures;
    std::vector<ENUM> mDrawBuffers;
};

#endif