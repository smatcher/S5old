#include "core/properties/grid.h"
#include "core/framework/glwidget.h"
#include <QtOpenGL>

Grid::Grid(float stepw, float steph, int w, int h) :
	IProperty("Debug grid"), m_stepw(stepw), m_steph(steph), m_w(w), m_h(h)
{
}

Grid::~Grid()
{

}

void Grid::drawDebug(const GLWidget*) const
{
	int i;

	glPushMatrix();
	glTranslatef(-(m_w*m_stepw)/2, -(m_h*m_steph)/2, 0.0f);
	glColor3f(.5f, .5f, .5f);
	glBegin(GL_LINES);
	for(i=0; i<=m_w; i++)
	{
	   if(i%(m_w/5) == 0)
		   glColor3f(1.0f, 1.0f, 1.0f);
	   glVertex3f(i*m_stepw, 0, 0);
	   glVertex3f(i*m_stepw, m_h*m_steph, 0);
	   if(i%(m_w/5) == 0)
		   glColor3f(.5f, .5f, .5f);
	}

	for(i=0; i<=m_h; i++)
	{
		if(i%(m_h/5)== 0)
			glColor3f(1.0f, 1.0f, 1.0f);
	   glVertex3f(0, i*m_stepw, 0);
	   glVertex3f(m_w*m_stepw, i*m_stepw, 0);
	   if(i%(m_h/5) == 0)
		   glColor3f(.5f, .5f, .5f);
	}
	glEnd();

	glPopMatrix();
}
