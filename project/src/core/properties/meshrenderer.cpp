#include "core/properties/meshrenderer.h"
#include <QtOpenGL>

#include "assimp.h"
#include "aiPostProcess.h"
#include "aiScene.h"

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

void apply_material(const struct aiMaterial *mtl)
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
   if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
	  color4_to_float4(&diffuse, c);
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

   set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
   if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
	  color4_to_float4(&specular, c);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

   set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
   if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
	  color4_to_float4(&ambient, c);
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

   set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
   if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
	  color4_to_float4(&emission, c);
   glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

   max = 1;
   ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
   max = 1;
   ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
   if((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS))
	  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
   else {
	  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
	  set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
	  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
   }

   max = 1;
   if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
	  fill_mode = wireframe ? GL_LINE : GL_FILL;
   else
	  fill_mode = GL_FILL;
   glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

   max = 1;
   if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
	  glEnable(GL_CULL_FACE);
   else
	  glDisable(GL_CULL_FACE);
}

void Color4f(const struct aiColor4D *color)
{
   glColor4f(color->r, color->g, color->b, color->a);
}

MeshRenderer::MeshRenderer(aiMesh* mesh, aiMaterial* material) : IRenderable("MeshRenderer")
{
	m_mesh = mesh;
	m_material = material;
}

void MeshRenderer::render(double elapsed_time, GLWidget* context)
{
	apply_material(m_material);

	if(m_mesh->mNormals == NULL) {
		glDisable(GL_LIGHTING);
	} else {
		glEnable(GL_LIGHTING);
	}

	if(m_mesh->mColors[0] != NULL) {
		glEnable(GL_COLOR_MATERIAL);
	} else {
		glDisable(GL_COLOR_MATERIAL);
	}

	for (unsigned t = 0; t < m_mesh->mNumFaces; ++t) {
		const struct aiFace* face = &m_mesh->mFaces[t];
		GLenum face_mode;

		switch(face->mNumIndices) {
			case 1: face_mode = GL_POINTS; break;
			case 2: face_mode = GL_LINES; break;
			case 3: face_mode = GL_TRIANGLES; break;
			default: face_mode = GL_POLYGON; break;
		}

		glBegin(face_mode);

		for(unsigned i = 0; i < face->mNumIndices; i++) {
			int index = face->mIndices[i];
			if(m_mesh->mColors[0] != NULL)
			Color4f(&m_mesh->mColors[0][index]);
			if(m_mesh->mNormals != NULL)
			glNormal3fv(&m_mesh->mNormals[index].x);
			glVertex3fv(&m_mesh->mVertices[index].x);
		}

		glEnd();
	}
}
