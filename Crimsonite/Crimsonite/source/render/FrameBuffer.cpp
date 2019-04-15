#include "FrameBuffer.h"

#include <gl/glew.h>

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferId);
}
