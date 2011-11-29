#ifndef MATERIALWIDGET_H
#define MATERIALWIDGET_H

#include "tools/widgets/resourcewidget.h"

class MaterialData;

class MaterialWidget : public ResourceWidget
{
public:
	MaterialWidget(MaterialData& resource);
	virtual ~MaterialWidget();
};

#endif // MATERIALWIDGET_H
