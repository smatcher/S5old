#include <GL/glew.h>
#include <GL/gl.h>

void __ClientActiveTexture(int texid)
{
	glClientActiveTexture(texid);
}
