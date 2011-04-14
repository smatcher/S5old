#include "core/resources/stbimage.h"
#include <QDir>

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
		glGenTextures(1,&m_gltexture);

		glBindTexture(GL_TEXTURE_2D, m_gltexture);

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);

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

		//gluBuild2DMipmaps(GL_TEXTURE_2D, m_comp, m_width, m_height, mode, GL_UNSIGNED_BYTE, m_data);
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
		qDebug() << "StbImage loaded " << resource->name() << " (" << x << "x" << y <<")";
		stbresource->m_data = data;
		stbresource->m_width = x;
		stbresource->m_height = y;
		stbresource->m_comp = comp;
		stbresource->m_state = StbImage::STATE_LOADED;
		stbresource->buildGLTexture();
	}
	else
	{
		qDebug() << "StbImage failed to load " << resource->name() << "\n" << stbi_failure_reason();
	}

}

void StbImageFactory::parseFile(const QString &path, QList<ResourceData *> &content)
{
	QDir dir(path);
	QString name = dir.dirName();
	dir.cdUp();
	if(dir.exists())
	{
		qDebug() << "StbImage found " << name;
		StbImage* image = new StbImage(name,path,this);
		content.push_back(image);
	}
	else
	{
		qDebug() << path << " : " << dir << " does not exist";
	}
}
