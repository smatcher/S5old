#ifndef SHADERWIDGET_H
#define SHADERWIDGET_H

#include "tools/widgets/resourcewidget.h"

class ShaderProgramData;

class ShaderWidget : public ResourceWidget
{
public:
	ShaderWidget(ShaderProgramData& resource);
	virtual ~ShaderWidget();
};


#endif // SHADERWIDGET_H
