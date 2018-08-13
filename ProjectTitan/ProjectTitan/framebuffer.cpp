#include "framebuffer.h"

void FrameBuffer::Init()
{
	glGenFramebuffers(1, &mFrameBufferObject);
}

void FrameBuffer::AttachColorBuffer(const CHAR *bufferName, ENUM attachment, INT width, INT height)
{
	UINT colorBuffer;
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject);
	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	mDrawBuffers.push_back(attachment);
	mBufferTextures.insert(std::pair<std::string, UINT>(bufferName, colorBuffer));
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, colorBuffer, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void FrameBuffer::AttachDepthBuffer(const CHAR *bufferName, INT width, INT height)
{
	UINT depthBuffer;
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject);
	glGenTextures(1, &depthBuffer);
	glBindTexture(GL_TEXTURE_2D, depthBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	mBufferTextures.insert(std::pair<std::string, UINT>(bufferName, depthBuffer));
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Bind()
{
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mPrevFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mPrevFrameBuffer);
}

UINT FrameBuffer::GetBuffer(const CHAR*bufferName)
{
	auto iter = mBufferTextures.find(bufferName);
	if (iter != mBufferTextures.end())
	{
		return iter->second;
	}
	return 0;
}

void FrameBuffer::AttachFinish()
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
	GL_CALL;
}

void FrameBuffer::Release()
{
	INT size = mBufferTextures.size();
	auto iter = mBufferTextures.begin();
	while (iter != mBufferTextures.end())
	{
		glDeleteTextures(1, &iter->second);
		iter++;
	}
	mBufferTextures.clear();
	glDeleteFramebuffers(1, &mFrameBufferObject);
}
