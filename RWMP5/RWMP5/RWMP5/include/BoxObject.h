#ifndef BOXOBJ_H
#define BOXOBJ_H

#include "BaseGameEntity.h"
#include "HKBodyFactory.h"
using namespace Ogre;

class BoxObject : public BaseGameEntity
{
public:
	enum BOX_TYPE {BLOCK, DESTRUCTABLE_BLOCK, PLATFORM, MOVING_PLATFORM, TILTING_PLATFORM, PRESSURE_PAD};
private:
	BOX_TYPE boxType;
	HKBodyFactory * bodyFactory;
	hkpRigidBody* m_body;
	SceneNode* m_node;
	SceneManager * m_sceneMgr;
	Entity* m_entity;
	float m_sizeX,m_sizeY,m_sizeZ;
	bool mIsStatic;

	Radian angleRot;
	Ogre::Vector3 axisRot;
	Ogre::Quaternion newOgRot;
	
public:

	BoxObject(BOX_TYPE bType, float size, SceneManager * sceneMgr, unsigned int id, hkVector4& position, bool isStatic = false, float mass = 20.f, float restitution = .1f, float friction = .1f);
	BoxObject(BOX_TYPE bType, float sizeX, float sizeY, float sizeZ, SceneManager * sceneMgr, unsigned int id, hkVector4& position, bool isStatic = false, float mass = 20.f, float restitution = .1f, float friction = .1f);
	~BoxObject();
	void update();
	void onContact();
	hkpRigidBody* getBody();
};

#endif