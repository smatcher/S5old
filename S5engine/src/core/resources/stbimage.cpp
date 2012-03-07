#include "core/resources/stbimage.h"
#include <QDir>

#include "core/log/log.h"

#include "core/managers/rendermanager.h"

#ifdef _WIN32
	#define GL_GENERATE_MIPMAP 0x8191
#endif

/*

 DATA

  */

StbImage::StbImage(const QString& name, const QString& path, IResourceFactory* factory, stbi_uc* data, bool mipmap, bool filtering, bool wrap_s, bool wrap_t, int comp) :
	TextureData(name, path, factory),
	m_data(data),
	m_comp(comp),
	m_mipmap(mipmap),
	m_filtering(filtering),
	m_wrap_s(wrap_s),
	m_wrap_t(wrap_t)
{
	if(m_data != 0)
	{
		buildTexture();
		m_state = STATE_LOADED;
	}
	else
	{
		m_state = STATE_UNLOADED;
	}
}

bool StbImage::unload()
{
	if(m_data != 0)
		stbi_image_free(m_data);

	if(m_irdtexture != 0)
	{
		RENDER_MANAGER.getRenderDevice()->destroyTexture(m_irdtexture);
	}

	m_data = 0;
	m_irdtexture = 0;
	m_state = STATE_UNLOADED;

	return true;
}

void StbImage::buildTexture()
{
	if(m_data != 0)
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

		IRD::Texture::Params params;
		params.m_format = IRD::Texture::TF_RGBA8;
		params.m_type = IRD::Texture::TT_TEX2D;
		params.m_genmipmap = m_mipmap;
		params.m_height = m_height;
		params.m_width = m_width;
		params.m_samplerState = (
			(m_filtering*IRD::Texture::TSS_FILTER) |
			(m_wrap_s*IRD::Texture::TSS_WRAP_U) |
			(m_wrap_t*IRD::Texture::TSS_WRAP_V)
		);

		m_irdtexture = RENDER_MANAGER.getRenderDevice()->createTexture(params);
		RENDER_MANAGER.getRenderDevice()->sendTextureData(m_irdtexture,0,0,0,0,m_data);

		/*TODO: supprimer l'image ou en fonction d'un flag, en attendant on la garde :D */
		/*
		stbi_image_free(m_data);
		m_data = 0;
		*/
	}
}

stbi_uc* StbImage::getData() {
	return this->m_data;
}

int StbImage::getComp() {
	return this->m_comp;
}

/*

  FACTORY

  */

void StbImageFactory::load(ResourceData *resource)
{
	StbImage* stbresource = static_cast<StbImage*>(resource);
	int x,y,comp;

	stbi_uc* data = stbi_load(stbresource->path().toLocal8Bit().data(), &x, &y, &comp, stbresource->m_comp);

	if(data != 0)
	{
		logInfo( "StbImage loaded " << resource->name() << " (" << x << "x" << y << ")");
		stbresource->m_data = data;
		stbresource->m_width = x;
		stbresource->m_height = y;

		// If we asked 0 components comp now contains the number of components
		if(stbresource->m_comp == 0)
			stbresource->m_comp = comp;

		stbresource->m_state = StbImage::STATE_LOADED;
		stbresource->buildTexture();
	}
	else
	{
		logWarn( "StbImage failed to load " << resource->name() << "\n" << stbi_failure_reason());
	}
}

void StbImageFactory::parseFile(const QString &path, QList<ResourceData *> &content, const QHash<QString,QString>& rules)
{
	QDir dir(path);
	QString name = dir.dirName();
	dir.cdUp();
	if(dir.exists())
	{
		debug( "RESOURCE PARSING" , "StbImage found " << name);
		int comp = 4; // alpha by default
		//int comp = 3;
		bool mipmap = true;
		bool filtering = true;
		bool wrap_s = true;
		bool wrap_t = true;
		if(rules.contains("alpha") && rules.find("alpha").value() == "true")
			comp = 4;
		if(rules.contains("mipmap") && rules.find("mipmap").value() == "false")
			mipmap = false;
		if(rules.contains("filtering") && rules.find("filtering").value() == "false")
			filtering = false;
		if(rules.contains("wrap_s") && rules.find("wrap_s").value() == "false")
			wrap_s = false;
		if(rules.contains("wrap_t") && rules.find("wrap_t").value() == "false")
			wrap_t = false;

		StbImage* image = new StbImage(name,path,this,0,mipmap,filtering,wrap_s,wrap_t,comp);
		content.push_back(image);
	}
	else
	{
		debug( "RESOURCE PARSING" , path << " : " << dir << " does not exist");
	}
}
