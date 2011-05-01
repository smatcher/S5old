#include "core/resources/stbimage.h"
#include <QDir>

#include "debug/log/log.h"

/*

 DATA

  */

StbImage::StbImage(const QString& name, const QString& path, IResourceFactory* factory, stbi_uc* data) :
	TextureData(name, path, factory),
	m_data(data)
{
	if(m_data != NULL)
	{
		buildGLTexture();
		m_state = STATE_LOADED;
	}
	else
	{
		m_state = STATE_UNLOADED;
	}
}

bool StbImage::unload()
{
	if(m_data != NULL)
		stbi_image_free(m_data);

	if(m_hasgltex)
		glDeleteTextures(1,&m_gltexture);

	m_data = NULL;
	m_hasgltex = false;
	m_state = STATE_UNLOADED;
	return true;
}

void StbImage::buildGLTexture()
{
	if(m_data != NULL)
	{
		// Do a vertical flip (openGL prefers bottomtop images)
		int i, j;
		for( j = 0; j*2 < m_height; ++j )
		{
			int index1 = j * m_width * m_comp;
			int index2 = (m_height - 1 - j) * m_width * m_comp;
			for( i = m_width * m_comp; i > 0; --i )
			{
				unsigned char temp = m_data[index1];
				m_data[index1] = m_data[index2];
				m_data[index2] = temp;
				++index1;
				++index2;
			}
		}
		// Get GL texture
		glEnable(GL_TEXTURE_2D);

		glGenTextures(1,&m_gltexture);

		glBindTexture(GL_TEXTURE_2D, m_gltexture);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);

		GLenum mode = GL_RGBA;

		switch(m_comp)
		{
			case 1 :
				mode = GL_LUMINANCE;
				break;
			case 2 :
				mode = GL_LUMINANCE_ALPHA;
				break;
			case 3 :
				mode = GL_RGB;
				break;
		}

		glTexParameteri(GL_TEXTURE_2D,GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, m_comp, m_width, m_height, 0, mode, GL_UNSIGNED_BYTE, m_data);

		m_hasgltex = true;

		stbi_image_free(m_data);
		m_data = NULL;
	}
}

/*

  FACTORY

  */

void StbImageFactory::load(ResourceData *resource)
{
	StbImage* stbresource = static_cast<StbImage*>(resource);
	int x,y,comp;

	stbi_uc* data = stbi_load(stbresource->path().toLocal8Bit().data(), &x, &y, &comp, 0);

	if(data != NULL)
	{
		logInfo( "StbImage loaded " << resource->name() << " (" << x << "x" << y << ")");
		stbresource->m_data = data;
		stbresource->m_width = x;
		stbresource->m_height = y;
		stbresource->m_comp = comp;
		stbresource->m_state = StbImage::STATE_LOADED;
		stbresource->buildGLTexture();
	}
	else
	{
		logWarn( "StbImage failed to load " << resource->name() << "\n" << stbi_failure_reason());
	}

}

void StbImageFactory::parseFile(const QString &path, QList<ResourceData *> &content)
{
	QDir dir(path);
	QString name = dir.dirName();
	dir.cdUp();
	if(dir.exists())
	{
		debug( "RESOURCE PARSING" , "StbImage found " << name);
		StbImage* image = new StbImage(name,path,this);
		content.push_back(image);
	}
	else
	{
		debug( "RESOURCE PARSING" , path << " : " << dir << " does not exist");
	}
}
