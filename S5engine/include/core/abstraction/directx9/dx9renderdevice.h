#ifndef DX9RENDERDEVICE_H
#define DX9RENDERDEVICE_H

#include <irenderdevice.h>

class DX9RenderDevice : public iRenderDevice
{
public:
		virtual bool isSupported(Feature feature) {return false;}
};

#endif // DX9RENDERDEVICE_H
