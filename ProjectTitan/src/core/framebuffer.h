#ifndef __FRAME_BUFFER_H
#define __FRAME_BUFFER_H

#include "define.h"
#include "utils.h"


#define FBO_DEPTH "depth"
#define FBO_COLOR "color"

class FrameBuffer
{
public:
	void Init();
	void AttachColorBuffer(const CHAR *bufferName, GLenum attachment, INT width, INT height);
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
    std::vector<GLenum> mDrawBuffers;
};

#endif
