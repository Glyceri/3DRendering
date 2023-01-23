#ifndef ZOOMBEHAVIOUR_HPP
#define ZOOMBEHAVIOUR_HPP

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "mge/core/GameObject.hpp"

/**
* ZoomBehaviour follows the given object
 */
class ZoomBehaviour : public AbstractBehaviour
{
public:
	ZoomBehaviour();
	virtual ~ZoomBehaviour();
	virtual void update(float pStep);
	void Zoomed(int value);

private:
	int zoomedThisTick = 0;
};

#endif // ZOOMBEHAVIOUR
