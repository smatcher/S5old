#ifndef MESHWIDGET_H
#define MESHWIDGET_H

#include "tools/widgets/resourcewidget.h"

class MeshData;

class MeshWidget : public ResourceWidget
{
public:
	MeshWidget(MeshData& resource);
	virtual ~MeshWidget();
};

#endif // MESHWIDGET_H
