#include "tools/widgets/meshwidget.h"

#include "core/managers/rendermanager.h"
#include "core/framework/glwidget.h"

#include "core/graphics/mesh.h"
#include "core/maths/aabb.h"

#include <QLabel>

MeshWidget::MeshWidget(MeshData &resource) : ResourceWidget(resource), m_preview(0), m_info(0)
{
	if(resource.m_state == ResourceData::STATE_LOADED)
	{
		MeshData* mesh = (MeshData*)&resource;
		//QWidget* wid;

		m_info = new QLabel("Nb poly : " + QString().setNum(mesh->getNbFaces()));
		m_layout->addWidget(m_info);
		m_preview = new MeshPreview((QGLWidget*)RENDER_MANAGER.getContext(),mesh);
		m_layout->addWidget(m_preview);

		m_layout->addStretch(1);
	}
}

MeshWidget::~MeshWidget()
{
}

MeshWidget::MeshPreview::MeshPreview(QGLWidget *shared, MeshData* mesh) : QGLWidget(0,shared)
{
	this->m_mesh = mesh;
	m_last_pressed = false;
	setMinimumHeight(200);
	setMinimumWidth(200);

	const BoundingVolume* boundvol = mesh->getBoundingVolume();
	const AABB* aabb = dynamic_cast<const AABB*>(boundvol);
	if(aabb) {
		Vector3f halfdim = aabb->getHalfDimensions();
		Vector3f center = aabb->getCenter();
		float scale = 0.5f/qMax(halfdim.x,qMax(halfdim.y,halfdim.z));

		m_transform.move(-scale*center);
		m_transform.setScale(Vector3f(scale,scale,scale));
	}
}

void MeshWidget::MeshPreview::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHT0);
	GLfloat lightPosition[4] = {2,2,-2,1};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	GLfloat materialAmbient[4] = {0.5,0.5,0.5,1.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  materialAmbient);
	m_transform.glMultf();
	m_mesh->drawPreview();
	swapBuffers();
}

void MeshWidget::MeshPreview::mouseReleaseEvent(QMouseEvent *event)
{
	if(!(event->buttons() & Qt::LeftButton)) {
		m_last_pressed = false;
	}
}
void MeshWidget::MeshPreview::mousePressEvent(QMouseEvent *event)
{
	if(event->buttons() & Qt::LeftButton) {
		m_lastx = event->x();
		m_lasty = event->y();
		m_last_pressed = true;
	}
}
void MeshWidget::MeshPreview::mouseMoveEvent(QMouseEvent *event)
{
	if(event->buttons() & Qt::LeftButton) {

		int dx = -0.6*(event->x() - m_lastx);
		int dy = -0.6*(event->y() - m_lasty);
		m_lastx = event->x();
		m_lasty = event->y();

//		Transformf inverse_transform = m_transform.getInverse();
		m_transform.rotate(/*inverse_transform.apply*/(Vector3f(0.0,1.0,0.0)),dx);
		m_transform.rotate(/*inverse_transform.apply*/(Vector3f(1.0,0.0,0.0)),dy);

		updateGL();
	}
}

void MeshWidget::updateData()
{
	ResourceWidget::updateData();
	MeshData* mesh = (MeshData*)&m_resource;

	if(mesh->m_state == ResourceData::STATE_LOADED) {

		if(m_info) {
			m_info->setText("Nb poly : " + QString().setNum(mesh->getNbFaces()));
		} else {
			m_info = new QLabel("Nb poly : " + QString().setNum(mesh->getNbFaces()));
			m_layout->addWidget(m_info);
		}

		if(m_preview) {
			m_preview->updateGL();
		} else {
			m_preview = new MeshPreview((QGLWidget*)RENDER_MANAGER.getContext(),mesh);
			m_layout->addWidget(m_preview);
			m_layout->addStretch(1);
		}
	}
}
