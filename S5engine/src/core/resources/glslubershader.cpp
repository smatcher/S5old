#include "core/resources/glslubershader.h"
#include <QtOpenGL>
#include "core/log/log.h"
#include <QtXml/QtXml>
#include "core/resources/managers.h"
#include "core/managers/rendermanager.h"

#include <QMatrix4x4>

GLSLUberShader::GLSLUberShader(const QString &name, const QString &path, IResourceFactory *factory) :
	UberShaderData(name, path, factory)
{
}

GLSLUberShader::~GLSLUberShader()
{
}

bool GLSLUberShader::unload()
{
	m_uniforms.clear();
	m_vertex_shader.clear();
	m_fragment_shader.clear();
	delete m_tree;
	m_tree = new UberShaderNode();
	for(int i=0 ; i<UberShaderDefine::NB_DEFINES ; i++) {
		m_defines[i] = false;
	}
	m_current = NULL;
	return true;
}

void GLSLUberShader::setUniform(const UniformBase* uniform)
{
	QGLShaderProgram* _program = program();

	if(_program != NULL && uniform != NULL)
		uniform->sendTo(*_program);
}

void GLSLUberShader::setEngineUniforms()
{
	debugGL("before setEngineUniforms");
	QGLShaderProgram* _program = program();

	if(_program != NULL) {
		int location;

		const QHash<QString, UniformBase*>& engine_uniforms = RENDER_MANAGER.getEngineUniforms();

		for(QHash<QString, UniformBase*>::const_iterator it = engine_uniforms.begin();
			it != engine_uniforms.end() ; it++) {
			location = _program->uniformLocation(it.key());
			if(location >= 0) {
				it.value()->sendTo(*_program);
			}
			debugGL("setting uniform" << it.key());
		}
	}
}

void GLSLUberShader::setAllUniforms()
{
	int nb_uniforms = nbUniforms();
	for(int i=0 ; i<nb_uniforms ; i++)
	{
		setUniform(uniform(i));
	}
	setEngineUniforms();
}

const GLSLUberShader::UniformBase* GLSLUberShader::uniform(int nb)
{
	return m_uniforms[nb];
}

int GLSLUberShader::nbUniforms()
{
	return m_uniforms.length();
}

void GLSLUberShader::compileCurrentState()
{
	UberShaderNode* node = getCurrentNode();

	if(node->m_program != NULL) {
		logWarn("UberShader state already compiled");
	} else {
		node->m_program = new QGLShaderProgram();
		node->m_vertex_shader = new QGLShader(QGLShader::Vertex);
		node->m_fragment_shader = new QGLShader(QGLShader::Fragment);

		node->m_vertex_shader->compileSourceCode(buildDefineList() + m_vertex_shader);
		node->m_fragment_shader->compileSourceCode(buildDefineList() + m_fragment_shader);
		node->m_program->addShader(node->m_vertex_shader);
		node->m_program->addShader(node->m_fragment_shader);

		buildTexunitsList(node->m_texunits);
	}
}

void GLSLUberShader::use()
{
	QGLShaderProgram* _program = program();
	if(_program != NULL) {
		_program->bind();

		if(m_current->m_texunits[UberShaderTextureType::COLOR_MAP] >= 0)
			_program->setUniformValue("colormap",m_current->m_texunits[UberShaderTextureType::COLOR_MAP]);

		if(m_current->m_texunits[UberShaderTextureType::NORMAL_MAP] >= 0)
			_program->setUniformValue("normalmap",m_current->m_texunits[UberShaderTextureType::NORMAL_MAP]);

		if(m_current->m_texunits[UberShaderTextureType::SPECULAR_MAP] >= 0)
			_program->setUniformValue("specularmap",m_current->m_texunits[UberShaderTextureType::SPECULAR_MAP]);

		if(m_current->m_texunits[UberShaderTextureType::GRADIENT_MAP] >= 0)
			_program->setUniformValue("gradientmap",m_current->m_texunits[UberShaderTextureType::GRADIENT_MAP]);

		if(m_current->m_texunits[UberShaderTextureType::SPLATTING] >= 0)
			_program->setUniformValue("spatting",m_current->m_texunits[UberShaderTextureType::SPLATTING]);

		if(m_current->m_texunits[UberShaderTextureType::SPLATTING_R] >= 0)
			_program->setUniformValue("splat_r",m_current->m_texunits[UberShaderTextureType::SPLATTING_R]);

		if(m_current->m_texunits[UberShaderTextureType::SPLATTING_G] >= 0)
			_program->setUniformValue("splat_g",m_current->m_texunits[UberShaderTextureType::SPLATTING_G]);

		if(m_current->m_texunits[UberShaderTextureType::SPLATTING_B] >= 0)
			_program->setUniformValue("splat_b",m_current->m_texunits[UberShaderTextureType::SPLATTING_B]);
	}
}

void GLSLUberShader::unset()
{
	QGLShaderProgram* _program = program();
	if(_program != NULL) {
		_program->release();
	}
}

void GLSLUberShaderFactory::load(ResourceData *resource)
{
	GLSLUberShader* program = static_cast<GLSLUberShader*>(resource);

	QFile file(program->m_path);

	if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		logWarn("GLSLUberShader : Can't open file" << program->m_path);
		return;
	}

	QDomDocument doc;
	if(!doc.setContent(&file))
	{
		logWarn("GLSLUberShader : Can't parse file" << program->m_path);
		return;
	}

	QDomNodeList programs = doc.elementsByTagName("ubershader");

	if(programs.length() < 1)
	{
		logWarn("GLSLUberShader : No program tag in file" << program->m_path);
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
					QGLShader* qgl_shader = shader->shader();
					if(qgl_shader->shaderType() == QGLShader::Fragment) {
						program->m_fragment_shader = qgl_shader->sourceCode();
						program->m_fragment_shader_file = shader_name;
					} else {
						program->m_vertex_shader = qgl_shader->sourceCode();
						program->m_vertex_shader_file = shader_name;
					}
				}
				else
				{
					logError("shader" << shader_name << "not available" << nodes.at(i).lineNumber());
				}
			}
		}
		else if(tag == "uniform")
		{
			GLSLUberShader::UniformBase* uniform;
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
				uniform = new GLSLUberShader::Uniform<GLint>(name,data,vallist.length(),1);
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
				uniform = new GLSLUberShader::Uniform<GLfloat>(name,data,vallist.length(),1);
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

	program->m_state = GLSLUberShader::STATE_LOADED;
	logInfo("GLSLUberShader loaded" << program->name());

	debugGL("while loading shader program" << program->m_path);
}

void GLSLUberShaderFactory::reload(ResourceData *resource)
{
	GLSLUberShader* program = dynamic_cast<GLSLUberShader*>(resource);

	if(program) {
		SHADER_MANAGER.get(program->m_vertex_shader_file)->reload();
		SHADER_MANAGER.get(program->m_fragment_shader_file)->reload();
		program->unload();
		program->load();
	}
}

void GLSLUberShaderFactory::parseFile(const QString &path, QList<ResourceData *> &content, const QHash<QString,QString>&)
{
	QDir dir(path);
	QString name = dir.dirName();

	if(!name.endsWith(".ush"))
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
		debug( "RESOURCE PARSING" , "GLSLUberShader found " << name);
		ShaderProgramData* program = new GLSLUberShader(name,path,this);
		content.push_back(program);
	}
	else
	{
		debug( "RESOURCE PARSING" , path << " : " << dir << " does not exist");
	}
}

