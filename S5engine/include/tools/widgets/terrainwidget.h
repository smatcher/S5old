#ifndef TERRAINWIDGET_H
#define TERRAINWIDGET_H

#include "tools/widgets/propertywidget.h"
#include <QLineEdit>
#include <QCheckBox>

class TerrainRenderer;

class TerrainWidget : public PropertyWidget
{
public :
	TerrainWidget(TerrainRenderer* property);
	virtual ~TerrainWidget();

	virtual void updateData();

private :
	QLineEdit *m_heightmap;
	QCheckBox *m_wireframe;
};


#endif // TERRAINWIDGET_H
