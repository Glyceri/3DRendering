#include "FollowBehaviour.hpp"

FollowBehaviour::FollowBehaviour(GameObject* objectToFollow): AbstractBehaviour()
{
	this->followedObject = objectToFollow;
}

FollowBehaviour::~FollowBehaviour()
{
}

void FollowBehaviour::update(float pStep)
{
	_owner->setLocalPosition(this->followedObject->getLocalPosition());
}
