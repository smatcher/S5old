#include "tools/widgets/terrainwidget.h"
#include "tools/widgets/propertywidget.h"
#include "core/properties/terrainrenderer.h"
#include <QtGui>

TerrainWidget::TerrainWidget(TerrainRenderer* property) : PropertyWidget(property)
{
		m_wireframe = new QCheckBox("wireframe");
		m_wireframe->setChecked(false);

		QVBoxLayout* layout = new QVBoxLayout();
		layout->setMargin(0);
		m_box->setLayout(layout);

		//layout->addWidget(m_heightmap);
		layout->addWidget(m_wireframe);
}

TerrainWidget::~TerrainWidget()
{
}

void TerrainWidget::updateData()
{
		TerrainRenderer* property = static_cast<TerrainRenderer*>(m_property);
		if(m_wireframe->isChecked()) {
			property->m_wireframe = true;
		}
		else {
			property->m_wireframe = false;
		}
}


