#ifndef GRID_H
#define GRID_H

#include "core/properties/iproperty.h"

class Grid : public IProperty
{
public:
	 /// Constructor
	 Grid(float stepw, float stepyh, int w, int h);
	 /// Destructor
	 virtual ~Grid();

	 void drawDebug(const GLWidget*, const RenderManager::DebugGizmosFilter& filter) const;

private:
	 float m_stepw;
	 float m_steph;
	 int m_w;
	 int m_h;
};

#endif // GRID_H
