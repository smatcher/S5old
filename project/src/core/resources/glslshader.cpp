#include "core/resources/glslshader.h"
#include <QtOpenGL>
#include "debug/log/log.h"
#include <QtXml/QtXml>
#include "core/resources/managers.h"

GLSLShaderProgram::GLSLShaderProgram(const QString &name, const QString &path, IResourceFactory *factory) :
	ShaderProgramData(name, path, factory),
	m_program(NULL)
{
}

GLSLShaderProgram::~GLSLShaderProgram()
{
	if(m_program != NULL)
		delete m_program;
}

bool GLSLShaderProgram::unload()
{
	debug("IMPLEMENTATION","Not implemented yet");
	return false;
}

void GLSLShaderProgram::use()
{
	if(m_program != NULL)
		m_program->bind();
}

void GLSLShaderProgram::unset()
{
	if(m_program != NULL)
		m_program->release();
}

void GLSLShaderProgramFactory::load(ResourceData *resource)
{
	GLSLShaderProgram* program = static_cast<GLSLShaderProgram*>(resource);
	program->m_program = new QGLShaderProgram();

	QFile file(program->m_path);

	if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		logWarn("GLSLShaderProgram : Can't open file" << program->m_path);
		return;
	}

	QDomDocument doc;
	if(!doc.setContent(&file))
	{
		logWarn("GLSLShaderProgram : Can't parse file" << program->m_path);
		return;
	}

	QDomNodeList programs = doc.elementsByTagName("program");

	if(programs.length() < 1)
	{
		logWarn("GLSLShaderProgram : No program tag in file" << program->m_path);
		return;
	}

	QDomNodeList nodes = programs.at(0).childNodes();

	for(int i=0 ; i < nodes.length() ; i++)
	{
		QString tag = nodes.at(i).nodeName();

		if(tag == "shader")
		{
			QDomNamedNodeMap attributes = nodes.at(i).attributes();
			if(attributes.contains("source"))
			{
				QString shader_name = attributes.namedItem("source").nodeValue();
				Shader shader = SHADER_MANAGER.get(shader_name);
				if(shader.isValid())
				{
					program->m_program->addShader(shader->shader());
				}
				else
				{
					logError("shader" << shader_name << "not available" << nodes.at(i).lineNumber());
				}
			}
		}
		else if(tag == "uniform")
		{

		}
		else
		{
			logWarn("In file" << program->m_name << "Unknown tag" << tag);
		}
	}

	program->m_state = GLSLShaderProgram::STATE_LOADED;
	logInfo("GLSLShaderProgram loaded" << program->name());

	debugGL("while loading shader program" << program->m_path);
}

void GLSLShaderProgramFactory::parseFile(const QString &path, QList<ResourceData *> &content)
{
	QDir dir(path);
	QString name = dir.dirName();

	if(!name.endsWith(".xml"))
		return;

	// Chop extension
	int id = name.lastIndexOf(".");
	if(id > 1)
	{
		name.chop(name.length() - id);
	}

	dir.cdUp();
	if(dir.exists())
	{
		debug( "RESOURCE PARSING" , "GLSLShaderProgram found " << name);
		ShaderProgramData* program = new GLSLShaderProgram(name,path,this);
		content.push_back(program);
	}
	else
	{
		debug( "RESOURCE PARSING" , path << " : " << dir << " does not exist");
	}
}

GLSLShader::GLSLShader(const QString &name, const QString &path, IResourceFactory *factory, ShaderType type) :
	ShaderData(name, path, factory, type)
{
}

bool GLSLShader::unload()
{
	debug("IMPLEMENTATION","Not implemented yet");
	return false;
}

QGLShader* GLSLShader::shader()
{
	return m_shader;
}

void GLSLShaderFactory::load(ResourceData *resource)
{
	GLSLShader* shader = static_cast<GLSLShader*>(resource);

	if(shader->m_type == GLSLShader::VERTEX_SHADER)
		shader->m_shader = new QGLShader(QGLShader::Vertex);
	else
		shader->m_shader = new QGLShader(QGLShader::Fragment);

	bool ok = shader->m_shader->compileSourceFile(shader->m_path);

	if(ok)
	{
		shader->m_state = ResourceData::STATE_LOADED;
		logInfo("GLSLShader loaded" << shader->name());
	}
	else
	{
		logError("GLSLShader failed loading" << shader->name() << "\n" << shader->m_shader->log());
		delete shader->m_shader;
		shader->m_shader = NULL;
	}

	debugGL("while loading shader" << shader->m_path);
}

void GLSLShaderFactory::parseFile(const QString &path, QList<ResourceData *> &content)
{
	QDir dir(path);
	QString name = dir.dirName();
	ShaderData* shader;

	bool isVert;
	if(name.endsWith(".vert"))
	{
		isVert = true;
	}
	else if(name.endsWith(".frag"))
	{
		isVert = false;
	}
	else
	{
		return;
	}

	dir.cdUp();
	if(dir.exists())
	{
		debug( "RESOURCE PARSING" , "GLSLShader found " << name);
		shader = new GLSLShader(name,path,this, isVert ? ShaderData::VERTEX_SHADER : ShaderData::FRAGMENT_SHADER);
		content.push_back(shader);
	}
	else
	{
		debug( "RESOURCE PARSING" , path << " : " << dir << " does not exist");
	}
}

