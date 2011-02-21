#ifndef QTLOGO_H
#define QTLOGO_H

#include <QColor>
#include "include/glwidget.h"

class QtLogo
{
private:

	QColor m_color;
	const GLWidget* m_widget;
	int m_theint;

public :

	QtLogo(const GLWidget* widget, int theint)
	{
		m_color = QColor(Qt::black);
		m_widget = widget;
		m_theint = theint;
	}

	void draw()
	{
		m_widget->qglColor(m_color);
		glBegin(GL_QUADS);
			glVertex3f( 0.25,-0.25,-0.25);
			glVertex3f( 0.25, 0.25,-0.25);
			glVertex3f( 0.25, 0.25, 0.25);
			glVertex3f( 0.25,-0.25, 0.25);
		glEnd();
		glBegin(GL_QUADS);
			glVertex3f(-0.25,-0.25,-0.25);
			glVertex3f(-0.25,-0.25, 0.25);
			glVertex3f(-0.25, 0.25, 0.25);
			glVertex3f(-0.25, 0.25,-0.25);
		glEnd();
		glBegin(GL_QUADS);
			glVertex3f(-0.25,-0.25,-0.25);
			glVertex3f( 0.25,-0.25,-0.25);
			glVertex3f( 0.25,-0.25, 0.25);
			glVertex3f(-0.25,-0.25, 0.25);
		glEnd();
		glBegin(GL_QUADS);
			glVertex3f( 0.25, 0.25,-0.25);
			glVertex3f(-0.25, 0.25,-0.25);
			glVertex3f(-0.25, 0.25, 0.25);
			glVertex3f( 0.25, 0.25, 0.25);
		glEnd();
		glBegin(GL_QUADS);
			glVertex3f(-0.25,-0.25, 0.25);
			glVertex3f( 0.25,-0.25, 0.25);
			glVertex3f( 0.25, 0.25, 0.25);
			glVertex3f(-0.25, 0.25, 0.25);
		glEnd();
		glBegin(GL_QUADS);
			glVertex3f(-0.25,-0.25,-0.25);
			glVertex3f(-0.25, 0.25,-0.25);
			glVertex3f( 0.25, 0.25,-0.25);
			glVertex3f( 0.25,-0.25,-0.25);
		glEnd();
	}

	void setColor(QColor color)
	{
		m_color = color;
	}

};

#endif // QTLOGO_H
