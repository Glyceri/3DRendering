#ifndef UPDOWNBEHAVIOUR_HPP
#define UPDOWNBEHAVIOUR_HPP

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include "SFML/Window/Keyboard.hpp"

/**
* UpDownBehaviour follows the given object
 */
class UpDownBehaviour : public AbstractBehaviour
{
public:
	UpDownBehaviour();
	virtual ~UpDownBehaviour();
	virtual void update(float pStep);

private:

};

#endif // UPDOWNBEHAVIOUR
