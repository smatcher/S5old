#ifndef UBERSHADERTREE_H
#define UBERSHADERTREE_H

#include <core/graphics/shader.h>

#include <QMap>
#include <QtOpenGL>

namespace UberShaderDefine
{
	enum Type
	{
		SKINNED = 0,
		COLORMAPPED,
		NORMALMAPPED,
		SPECULARMAPPED,
		GRADIENTMAPPED,
		SSS_PREPASS,
		SSS_FINAL,
		SSS_MAP,
		LIGHT_OMNI,
		LIGHT_OMNI_0,
		LIGHT_OMNI_1,
		LIGHT_OMNI_2,
		LIGHT_OMNI_3,
		LIGHT_OMNI_4,
		LIGHT_OMNI_5,
		LIGHT_OMNI_6,
		LIGHT_OMNI_7,
		LIGHT_SPOT,
		LIGHT_SPOT_0,
		LIGHT_SPOT_1,
		LIGHT_SPOT_2,
		LIGHT_SPOT_3,
		LIGHT_SPOT_4,
		LIGHT_SPOT_5,
		LIGHT_SPOT_6,
		LIGHT_SPOT_7,
		LIGHT_SUN,
		LIGHT_SUN_0,
		LIGHT_SUN_1,
		LIGHT_SUN_2,
		LIGHT_SUN_3,
		LIGHT_SUN_4,
		LIGHT_SUN_5,
		LIGHT_SUN_6,
		LIGHT_SUN_7,
		SHADOWED,
		SPLATTING,
		ENVIRONMENTMAPPED,
		SKY,
		BLOOM,
		NB_DEFINES,
		UNKNOWN
	};

	static QString Names[] =
	{
		"SKINNED",
		"COLOR_MAP",
		"NORMAL_MAP",
		"SPECULAR_MAP",
		"GRADIENT_MAP",
		"SSS_PREPASS",
		"SSS_FINAL",
		"SSS_MAP",
		"LIGHT_OMNI",
		"LIGHT_OMNI_0",
		"LIGHT_OMNI_1",
		"LIGHT_OMNI_2",
		"LIGHT_OMNI_3",
		"LIGHT_OMNI_4",
		"LIGHT_OMNI_5",
		"LIGHT_OMNI_6",
		"LIGHT_OMNI_7",
		"LIGHT_SPOT",
		"LIGHT_SPOT_0",
		"LIGHT_SPOT_1",
		"LIGHT_SPOT_2",
		"LIGHT_SPOT_3",
		"LIGHT_SPOT_4",
		"LIGHT_SPOT_5",
		"LIGHT_SPOT_6",
		"LIGHT_SPOT_7",
		"LIGHT_SUN",
		"LIGHT_SUN_0",
		"LIGHT_SUN_1",
		"LIGHT_SUN_2",
		"LIGHT_SUN_3",
		"LIGHT_SUN_4",
		"LIGHT_SUN_5",
		"LIGHT_SUN_6",
		"LIGHT_SUN_7",
		"SHADOW_MAP",
		"SPLATTING",
		"ENVIRONMENT_MAP",
		"SKY",
		"BLOOM",
		"NB_DEFINES",
		"UNKOWN"
	};

	Type fromStr(const QString& name);
	QString toString(int i);
	QString toString(Type type);
}

namespace UberShaderTextureType
{
	enum Type
	{
		COLOR_MAP = 0,
		NORMAL_MAP,
		SPECULAR_MAP,
		GRADIENT_MAP,
		ENVIRONMENT_MAP,
		SPLATTING,
		SPLATTING_R,
		SPLATTING_G,
		SPLATTING_B,
		SSS_MAP,
		NB_TEXTURES,
		UNKNOWN
	};

	static QString Names[] =
	{
		"COLOR_MAP",
		"NORMAL_MAP",
		"SPECULAR_MAP",
		"GRADIENT_MAP",
		"ENVIRONMENT_MAP",
		"SPLATTING",
		"SPLATTING_R",
		"SPLATTING_G",
		"SPLATTING_B",
		"SSS_MAP",
		"NB_TEXTURES",
		"UNKNOWN"
	};

	Type fromStr(const QString& name);
	QString toString(int i);
	QString toString(Type type);
}

class UberShader;

class UberShaderData : public ShaderProgramData
{
	friend class ResourceHandle<UberShaderData>;
	friend class ResourceManager<UberShaderData, UberShader>;

public:


	UberShaderData(const QString& name, const QString& path, IResourceFactory* factory) : ShaderProgramData(name, path, factory), m_tree(NULL), m_current(NULL) {
		m_tree = new UberShaderNode();
		for(int i=0 ; i<UberShaderDefine::NB_DEFINES ; i++) {
			m_defines[i] = false;
		}
	}
	virtual ~UberShaderData() {if(m_tree!=NULL) {delete m_tree;}}
	void setParamValue(UberShaderDefine::Type param, bool value) {m_defines[param] = value; m_current = NULL;}
	void resetParams() {
		for(int i=0 ; i<UberShaderDefine::NB_DEFINES ; i++) {
			m_defines[i] = false;
		}
		m_current = NULL;
	}
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

	int getTexUnit(UberShaderTextureType::Type texture) {
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
		int  m_texunits[UberShaderTextureType::NB_TEXTURES];

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

		for(int i=0 ; i< UberShaderDefine::NB_DEFINES ; i++) {
			if(m_defines[i]) {
				ret += "#define "+ UberShaderDefine::toString(i) +" \n";
			}
		}

		return ret;
	}

	void buildTexunitsList(int* texunits) {
		int texunit = 0;

		texunits[UberShaderTextureType::COLOR_MAP]       = m_defines[UberShaderDefine::COLORMAPPED]       ? texunit++ : -1;
		texunits[UberShaderTextureType::NORMAL_MAP]      = m_defines[UberShaderDefine::NORMALMAPPED]      ? texunit++ : -1;
		texunits[UberShaderTextureType::SPECULAR_MAP]    = m_defines[UberShaderDefine::SPECULARMAPPED]    ? texunit++ : -1;
		texunits[UberShaderTextureType::GRADIENT_MAP]    = m_defines[UberShaderDefine::GRADIENTMAPPED]    ? texunit++ : -1;
		texunits[UberShaderTextureType::SSS_MAP]         = m_defines[UberShaderDefine::SSS_MAP]           ? texunit++ : -1;
		texunits[UberShaderTextureType::ENVIRONMENT_MAP] = m_defines[UberShaderDefine::ENVIRONMENTMAPPED] ? texunit++ : -1;
		texunits[UberShaderTextureType::SPLATTING]       = m_defines[UberShaderDefine::SPLATTING]         ? texunit++ : -1;
		texunits[UberShaderTextureType::SPLATTING_R]     = m_defines[UberShaderDefine::SPLATTING]         ? texunit++ : -1;
		texunits[UberShaderTextureType::SPLATTING_G]     = m_defines[UberShaderDefine::SPLATTING]         ? texunit++ : -1;
		texunits[UberShaderTextureType::SPLATTING_B]     = m_defines[UberShaderDefine::SPLATTING]         ? texunit++ : -1;
	}

	UberShaderNode* getCurrentNode(int param_read = 0, UberShaderNode* current_branch = NULL) {
		if(m_current != NULL) {
			return m_current;
		}

		if(current_branch == NULL) {
			current_branch = m_tree;
		}

		UberShaderNode* ret = current_branch;
		if(param_read < UberShaderDefine::NB_DEFINES) {
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
	bool m_defines[UberShaderDefine::NB_DEFINES];
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
