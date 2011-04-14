#include "core/resources/assimpmaterial.h"
#include <QtOpenGL>

#include <assimp.h>
#include <aiMaterial.h>

AssimpMaterial::AssimpMaterial(const QString& name, const QString& path, IResourceFactory* factory, const aiMaterial* material) :
	MaterialData(name,path,factory),
	m_material(material)
{
}

bool AssimpMaterial::unload()
{
	return false;
}

void color4_to_float4(const struct aiColor4D *c, float f[4])
{
   f[0] = c->r;
   f[1] = c->g;
   f[2] = c->b;
   f[3] = c->a;
}

void set_float4(float f[4], float a, float b, float c, float d)
{
   f[0] = a;
   f[1] = b;
   f[2] = c;
   f[3] = d;
}

void AssimpMaterial::apply()
{
   float c[4];

   GLenum fill_mode;
   int ret1, ret2;
   struct aiColor4D diffuse;
   struct aiColor4D specular;
   struct aiColor4D ambient;
   struct aiColor4D emission;
   float shininess, strength;
   int two_sided;
   int wireframe;
   unsigned int max;

   set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
   if(AI_SUCCESS == aiGetMaterialColor(m_material, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
	  color4_to_float4(&diffuse, c);
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

   set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
   if(AI_SUCCESS == aiGetMaterialColor(m_material, AI_MATKEY_COLOR_SPECULAR, &specular))
	  color4_to_float4(&specular, c);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

   set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
   if(AI_SUCCESS == aiGetMaterialColor(m_material, AI_MATKEY_COLOR_AMBIENT, &ambient))
	  color4_to_float4(&ambient, c);
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

   set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
   if(AI_SUCCESS == aiGetMaterialColor(m_material, AI_MATKEY_COLOR_EMISSIVE, &emission))
	  color4_to_float4(&emission, c);
   glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

   max = 1;
   ret1 = aiGetMaterialFloatArray(m_material, AI_MATKEY_SHININESS, &shininess, &max);
   max = 1;
   ret2 = aiGetMaterialFloatArray(m_material, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
   if((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS))
	  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
   else {
	  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
	  set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
	  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
   }

   max = 1;
   if(AI_SUCCESS == aiGetMaterialIntegerArray(m_material, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
	  fill_mode = wireframe ? GL_LINE : GL_FILL;
   else
	  fill_mode = GL_FILL;
   glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

   max = 1;
   if((AI_SUCCESS == aiGetMaterialIntegerArray(m_material, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
	  glEnable(GL_CULL_FACE);
   else
	  glDisable(GL_CULL_FACE);
}
