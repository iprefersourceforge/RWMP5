#ifndef SPHEREOBJ_H
#define SPHEREOBJ_H

#include "BaseGameEntity.h"
#include "HKBodyFactory.h"
using namespace Ogre;

class SphereObject : public BaseGameEntity
{
private:
	HKBodyFactory * bodyFactory;
	hkpRigidBody* m_body;
	SceneNode* m_node;
	SceneManager * m_sceneMgr;
	Entity* m_entity;
	float m_size;

	Radian angleRot;
	Ogre::Vector3 axisRot;
	Ogre::Quaternion newOgRot;

public:
	SphereObject(float size, SceneManager * sceneMgr, unsigned int id, hkVector4& position, float mass = 20.f, float restitution = .1f, float friction = .1f);
	~SphereObject();
	void update();
	void onContact();
	hkpRigidBody* getBody();
};

#endif