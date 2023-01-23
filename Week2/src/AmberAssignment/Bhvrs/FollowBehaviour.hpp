#ifndef FOLLOWBEHAVIOUR_HPP
#define FOLLOWBEHAVIOUR_HPP

#include "mge/behaviours/AbstractBehaviour.hpp"
#include "mge/core/GameObject.hpp"


/**
* FollowBehaviour follows the given object
 */
class FollowBehaviour : public AbstractBehaviour
{
public:
	FollowBehaviour(GameObject* objectToFollow);
	virtual ~FollowBehaviour() ;
	virtual void update(float pStep);

private:
	GameObject* followedObject;
};

#endif // FOLLOWBEHAVIOUR_HPP
