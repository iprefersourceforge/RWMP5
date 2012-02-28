#ifndef HKBODYFACT_H
#define HKBODYFACT_H

#include "Physics.h"
#include "Ogre.h"
#include "UniqueIDGenerator.h"

class HKBodyFactory
{
public:
	enum SHAPE_TYPE {BOX, SPHERE};
	static HKBodyFactory * getInstance(Ogre::SceneManager * sceneMgr = NULL);
	~HKBodyFactory();
	hkpRigidBody* CreateSphere(hkVector4& pos, float size, float mass = 20.f, float restitution = .287f, float friction = .2f);
	hkpRigidBody* CreateBox(hkVector4& pos, float sizeX, float sizeY, float sizeZ, bool isStatic, float mass = 20.f, float restitution = .1f, float friction = .5f);
private:
	hkArray<hkpRigidBody*> mBodies;
	static HKBodyFactory * mInstance;
	HKBodyFactory(Ogre::SceneManager * sceneMgr);
	Ogre::SceneManager * mSceneMgr;
	UniqueIDGenerator * mIDGen;

};

#endif