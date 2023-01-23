#ifndef ROTATINGBEHAVIOUR_HPP
#define ROTATINGBEHAVIOUR_HPP

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "glm.hpp"

/**
 * Simply rotates the object around its origin with a fixed speed.
 */
class RotatingBehaviour : public AbstractBehaviour
{
	float speed = 45.0f;
	glm::vec3 axis = glm::vec3(0, 1, 0);

	public:
		RotatingBehaviour();
		RotatingBehaviour(float speed, glm::vec3 axis);
		virtual ~RotatingBehaviour();

		virtual void update(float pStep);

};

#endif // ROTATINGBEHAVIOUR_HPP
