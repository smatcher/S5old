#include <tools/3D/manipulator.h>
#include <core/framework/glwidget.h>

#include "core/scenegraph/node.h"
#include "QtOpenGL"
#include <QColor>

void Manipulator::draw(GLWidget* widget)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
		if(m_selected.size() > 0) {
			m_selected[0]->getGlobalTransform(false).glMultf();

			widget->qglColor(Qt::red);
			glBegin(GL_LINES);
				glVertex3d(0,0,0);
				glVertex3d(1,0,0);
			glEnd();
			widget->qglColor(Qt::green);
			glBegin(GL_LINES);
				glVertex3d(0,0,0);
				glVertex3d(0,1,0);
			glEnd();
			widget->qglColor(Qt::blue);
			glBegin(GL_LINES);
				glVertex3d(0,0,0);
				glVertex3d(0,0,1);
			glEnd();
		}
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

void Manipulator::update()
{
}

void Manipulator::addSelected(Node* node)
{
	if(!m_selected.contains(node) && node != NULL) {
		m_selected.push_back(node);
	}
}

void Manipulator::removeSelected(Node* node)
{
	if(!m_selected.contains(node)) {
		m_selected.remove(m_selected.indexOf(node));
	}
}

void Manipulator::unselectAll()
{
	m_selected.clear();
}

void Manipulator::select(Node* node)
{
	m_selected.clear();

	if(node != NULL) {
		m_selected.push_back(node);
	}
}
