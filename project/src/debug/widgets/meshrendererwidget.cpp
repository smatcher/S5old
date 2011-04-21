#include "debug/widgets/meshrendererwidget.h"
#include "core/properties/meshrenderer.h"
#include "core/graphics/mesh.h"

MeshRendererWidget::MeshRendererWidget(MeshRenderer* property) : PropertyWidget(property)
{
	m_texture = new QLineEdit((&property->m_texture)->name());
	m_texture->setDisabled(true);
	m_material = new QLineEdit((&property->m_material)->name());
	m_material->setDisabled(true);
	m_mesh = new QLineEdit((&property->m_mesh)->name());
	m_mesh->setDisabled(true);

	QVBoxLayout* layout = new QVBoxLayout();
	layout->setMargin(0);
	m_box->setLayout(layout);

	layout->addWidget(m_texture);
	layout->addWidget(m_material);
	layout->addWidget(m_mesh);
}

MeshRendererWidget::~MeshRendererWidget()
{
}

void MeshRendererWidget::updateData()
{
	MeshRenderer* property = static_cast<MeshRenderer*>(m_property);

	m_texture->setText((&property->m_texture)->name());
	m_material->setText((&property->m_material)->name());
	m_mesh->setText((&property->m_mesh)->name());
}
