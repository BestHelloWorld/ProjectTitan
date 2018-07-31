#ifndef __FRAME_BUFFER_H
#define __FRAME_BUFFER_H

#include "define.h"

class FrameBuffer
{
public:
	void Init()
	{

		glGenFramebuffers(1, &mFrameBufferObject);
	}

	void AttachColorBuffer(const CHAR *bufferName, ENUM attachment, INT width, INT height)
	{
		UINT colorBuffer;
		glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject);
		glGenTextures(1, &colorBuffer);
		glBindTexture(GL_TEXTURE_2D, colorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		mDrawBuffers.push_back(attachment);
		mBufferTextures.insert(std::pair<std::string, UINT>(bufferName, colorBuffer));
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, colorBuffer, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void AttachDepthBuffer(const CHAR *bufferName, INT width, INT height)
	{
		UINT depthBuffer;
		glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject);
		glGenTextures(1, &depthBuffer);
		glBindTexture(GL_TEXTURE_2D, depthBuffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		mBufferTextures.insert(std::pair<std::string, UINT>(bufferName, depthBuffer));
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Bind()
	{
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mPrevFrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mPrevFrameBuffer);
	}

	UINT GetBuffer(const CHAR*bufferName)
	{
		auto iter = mBufferTextures.find(bufferName);
		if (iter != mBufferTextures.end())
		{
			return iter->second;
		}
		return 0;
	}

	void AttachFinish()
	{
		INT nCount = (INT)mDrawBuffers.size();
		if (nCount > 0)
		{
			ENUM *buffers = new ENUM[nCount];
			for (INT i = 0; i < nCount; ++i)
			{
				buffers[i] = mDrawBuffers[i];
			}

			glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject);
			glDrawBuffers(nCount, buffers);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

public:
    UINT mFrameBufferObject;
    INT mPrevFrameBuffer;
    std::map<std::string, UINT> mBufferTextures;
    std::vector<ENUM> mDrawBuffers;
};

#endif