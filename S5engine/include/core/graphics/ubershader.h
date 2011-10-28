#ifndef UBERSHADERTREE_H
#define UBERSHADERTREE_H

#include <core/graphics/shader.h>

#include <QMap>
#include <QtOpenGL>

static QString UberShaderParamNames[] =
{
	"SKINNED",
	"COLOR_MAP",
	"NORMAL_MAP",
	"SHADOW_MAP",
	"REFLECTION_MAP",
};

class UberShader;

class UberShaderData : public ShaderProgramData
{
	friend class ResourceHandle<UberShaderData>;
	friend class ResourceManager<UberShaderData, UberShader>;

public:
	enum UberShaderParam
	{
		US_SKINNED = 0,
		US_COLORMAPPED,
		US_NORMALMAPPED,
		US_SHADOWED,
		US_REFLECTIONMAPPED,
		US_NB_PARAMS
	};

	enum UberShaderTextureType
	{
		TEX_COLOR_MAP = 0,
		TEX_NORMAL_MAP,
		TEX_NB_TEXTURES
	};

	UberShaderData(const QString& name, const QString& path, IResourceFactory* factory) : ShaderProgramData(name, path, factory), m_tree(NULL), m_current(NULL) {
		m_tree = new UberShaderNode();
		for(int i=0 ; i<US_NB_PARAMS ; i++) {
			m_defines[i] = false;
		}
	}
	virtual ~UberShaderData() {if(m_tree!=NULL) {delete m_tree;}}
	void setParamValue(UberShaderParam param, bool value) {m_defines[param] = value; m_current = NULL;}
	virtual bool isUber() {return true;}
	virtual void compile() {
		UberShaderNode* node = getCurrentNode();
		if(node->m_program == NULL) {
			compileCurrentState();
		}
	}
	virtual QGLShaderProgram* program() {
		UberShaderNode* node = getCurrentNode();
		if(node->m_program == NULL) {
			compile();
		}
		return getCurrentNode()->m_program;
	}

	int getTexUnit(UberShaderTextureType texture) {
		return getCurrentNode()->m_texunits[texture];
	}

	virtual void use() = 0;
	virtual void unset() = 0;

protected:
	struct UberShaderNode
	{
		UberShaderNode* m_yes_son;
		UberShaderNode* m_no_son;

		QGLShaderProgram* m_program;
		QGLShader* m_fragment_shader;
		QGLShader* m_vertex_shader;
		int  m_texunits[TEX_NB_TEXTURES];

		UberShaderNode() :
			m_yes_son(NULL),
			m_no_son(NULL),
			m_program(NULL),
			m_fragment_shader(NULL),
			m_vertex_shader(NULL) {}
		~UberShaderNode() {
			if(m_yes_son != NULL) delete m_yes_son;
			if(m_no_son != NULL)  delete m_no_son;
			if(m_program != NULL) delete m_program;
			if(m_vertex_shader != NULL) delete m_vertex_shader;
			if(m_fragment_shader != NULL) delete m_fragment_shader;
		}
	};

	virtual void compileCurrentState() = 0;

	QString buildDefineList() {
		QString ret;

		for(int i=0 ; i< US_NB_PARAMS ; i++) {
			if(m_defines[i]) {
				ret += "#define "+ UberShaderParamNames[i] +" \n";
			}
		}

		return ret;
	}

	void buildTexunitsList(int* texunits) {
		int texunit = 0;

		texunits[TEX_COLOR_MAP] = m_defines[US_COLORMAPPED] ? texunit++ : -1;
		texunits[TEX_NORMAL_MAP] = m_defines[US_NORMALMAPPED] ? texunit++ : -1;
	}

	UberShaderNode* getCurrentNode(int param_read = 0, UberShaderNode* current_branch = NULL) {
		if(m_current != NULL) {
			return m_current;
		}

		if(current_branch == NULL) {
			current_branch = m_tree;
		}

		UberShaderNode* ret = current_branch;
		if(param_read < US_NB_PARAMS) {
			if(m_defines[param_read]) {
				if(current_branch->m_yes_son == NULL) {
					current_branch->m_yes_son = new UberShaderNode();
				}
				ret = getCurrentNode(param_read+1, current_branch->m_yes_son);
			} else {
				if(current_branch->m_no_son == NULL) {
					current_branch->m_no_son = new UberShaderNode();
				}
				ret = getCurrentNode(param_read+1, current_branch->m_no_son);
			}
		} else {
			ret = current_branch;
			m_current = current_branch;
		}

		return ret;
	}

	UberShaderNode* m_tree;
	UberShaderNode* m_current;
	bool m_defines[US_NB_PARAMS];
};

class UberShader : public ResourceHandle<UberShaderData>
{
public:
	UberShader() {}
	UberShader(const UberShader& from) : ResourceHandle<UberShaderData>(from) {}
	UberShader(UberShaderData& from) : ResourceHandle<UberShaderData>(from) {}
	virtual ~UberShader() {}
};

#endif // UBERSHADERTREE_H
