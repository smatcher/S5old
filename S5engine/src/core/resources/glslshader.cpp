#include "core/resources/glslshader.h"
#include <QtOpenGL>
#include <GL/glu.h>
#include "core/log/log.h"
#include <QtXml/QtXml>
#include "core/resources/managers.h"
#include "core/managers/rendermanager.h"

#include <QMatrix4x4>

GLSLShaderProgram::GLSLShaderProgram(const QString &name, const QString &path, IResourceFactory *factory) :
	ShaderProgramData(name, path, factory),
	m_program(0)
{
}

GLSLShaderProgram::~GLSLShaderProgram()
{
	if(m_program != 0)
		delete m_program;
}

bool GLSLShaderProgram::unload()
{
	if(m_program != 0)
	{
		delete m_program;
		m_program = 0;
	}

	return true;
}

void GLSLShaderProgram::use()
{
	if(m_program != 0)
		m_program->bind();
}

void GLSLShaderProgram::unset()
{
	if(m_program != 0)
		m_program->release();
}

void GLSLShaderProgram::setUniform(const UniformBase* uniform)
{
	if(m_program != 0 && uniform != 0)
		uniform->sendTo(*m_program);
}

void GLSLShaderProgram::setEngineUniforms()
{
	debugGL("before setEngineUniforms");

	if(m_program != 0) {
		int location;

		const QHash<QString, UniformBase*>& engine_uniforms = RENDER_MANAGER.getEngineUniforms();

		for(QHash<QString, UniformBase*>::const_iterator it = engine_uniforms.begin();
			it != engine_uniforms.end() ; it++) {
			location = m_program->uniformLocation(it.key());
			if(location >= 0) {
				it.value()->sendTo(*m_program);
			}
			debugGL("setting uniform" << it.key());
		}
	}
}

void GLSLShaderProgram::setAllUniforms()
{
	int nb_uniforms = nbUniforms();
	for(int i=0 ; i<nb_uniforms ; i++)
	{
		setUniform(uniform(i));
	}
	setEngineUniforms();
}

const GLSLShaderProgram::UniformBase* GLSLShaderProgram::uniform(int nb)
{
	return m_uniforms[nb];
}

int GLSLShaderProgram::nbUniforms()
{
	return m_uniforms.length();
}

QGLShaderProgram* GLSLShaderProgram::program()
{
	return m_program;
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

	for(int i=0 ; i < (int)nodes.length() ; i++)
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
			GLSLShaderProgram::UniformBase* uniform;
			QDomNamedNodeMap attributes = nodes.at(i).attributes();
			QString uniform_type;
			QString name;
			QString value;
			if(attributes.contains("name"))
			{
				name = attributes.namedItem("name").nodeValue();
			}
			if(attributes.contains("value"))
			{
				value = attributes.namedItem("value").nodeValue();
			}
			if(attributes.contains("type"))
			{
				uniform_type = attributes.namedItem("type").nodeValue();
			}
			else
			{
				uniform_type = "float";
			}
			QStringList vallist = value.split(" ",QString::SkipEmptyParts);
			if(uniform_type == "int")
			{
				GLint* data = new GLint[vallist.length()]();
				uniform = new GLSLShaderProgram::Uniform<GLint>(name,data,vallist.length(),1);
				for(int i=0 ; i<vallist.length() ; i++)
				{
					bool ok;
					GLint val = vallist.at(i).toInt(&ok);
					if(ok)
						data[i] = val;
					else
						logWarn("non int while reading" << value << "in file" << program->m_path);
				}
			}
			else
			{
				GLfloat* data = new GLfloat[vallist.length()]();
				uniform = new GLSLShaderProgram::Uniform<GLfloat>(name,data,vallist.length(),1);
				for(int i=0 ; i<vallist.length() ; i++)
				{
					bool ok;
					GLfloat val = vallist.at(i).toFloat(&ok);
					if(ok)
						data[i] = val;
					else
						logWarn("non float while reading" << value << "in file" << program->m_path);
				}
			}
			program->m_uniforms.push_back(uniform);
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

void GLSLShaderProgramFactory::parseFile(const QString &path, QList<ResourceData *> &content, const QHash<QString,QString>&)
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
	if(m_shader)
	{
		delete m_shader;
		m_shader = 0;
	}

	return true;
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

	QString source;
	precompile(shader->m_path,source);
	bool ok = shader->m_shader->compileSourceCode(source);

	if(ok)
	{
		shader->m_state = ResourceData::STATE_LOADED;
		logInfo("GLSLShader loaded" << shader->name());
	}
	else
	{
		logError("GLSLShader failed loading" << shader->name() << "\n" << shader->m_shader->log());
		delete shader->m_shader;
		shader->m_shader = 0;
	}

	debugGL("while loading shader" << shader->m_path);
}

void GLSLShaderFactory::parseFile(const QString &path, QList<ResourceData *> &content, const QHash<QString,QString>&)
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

void GLSLShaderFactory::precompile(const QString& path, QString& output) const
{
	bool inFor = false;
	int forStart, forEnd;
	QString forContent;
	QFile file(path);
	output.clear();

	if(file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream t(&file);
		while(!t.atEnd())
		{
			QString line = t.readLine();
			line = line.trimmed();

			if(!line.isEmpty())
			{
				if(!inFor)
				{
					if(line.startsWith("#for"))
					{
						// For starts
						QStringList strlist = line.split(QRegExp("\\s+"));
						if(strlist.count()<3)
						{
							logError("Error while parsing a for " << line << "is not valid, should be #for begin end (begin and end being integers)");
							abort();
						}
						forStart = strlist.at(1).toInt();
						forEnd = strlist.at(2).toInt();
						inFor = true;
						forContent.clear();
					}
					else
					{
						// Regular line
						output.append(line + "\n");
					}
				}
				else
				{
					if(line.startsWith("#endfor"))
					{
						// For ends
						inFor = false;

						for(int i=forStart ; i<=forEnd ; i++)
						{
							QString toAdd = forContent;
							output.append(toAdd.replace('@',QString().setNum(i)));
						}
					}
					else
					{
						// Line inside for
						forContent.append(line + "\n");
					}
				}
			}
		}
	}
	else
	{
		logError("Can't open the file" << path);
	}
}
