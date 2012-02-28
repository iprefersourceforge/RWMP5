#include "SphereManager.h"

SphereManager * SphereManager::mInstance = 0;

SphereManager::SphereManager(hkpWorld* physicsWorld, SceneManager * sceneMgr)
	: mPhysicsWorld(physicsWorld)
	, mSceneMgr(sceneMgr)
{

}

SphereManager::~SphereManager()
{

}

SphereManager * SphereManager::getInstance(hkpWorld* physicsWorld, SceneManager * sceneMgr)
{
	if (mInstance == 0)
	{
		mInstance = new SphereManager(physicsWorld, sceneMgr);
	}
	return mInstance;
}

void SphereManager::addSphere(float size, unsigned int id, hkVector4& position, float mass, float restitution, float friction)
{
	SphereObject * s = new SphereObject(size, mSceneMgr, id, position, mass, restitution, friction);
	mPhysicsWorld->addEntity((hkpEntity*)s->getBody());
	mSpheres.push_back(s);

}

SphereObject * SphereManager::newSphere(float size, unsigned int id, hkVector4& position, float mass, float restitution, float friction)
{
	SphereObject * s = new SphereObject(size, mSceneMgr, id, position, mass, restitution, friction);
	mPhysicsWorld->addEntity((hkpEntity*)s->getBody());
	mSpheres.push_back(s);
	return s;
}

void SphereManager::update()
{
	currIter = mSpheres.begin();
	endIter = mSpheres.end();
	for (; currIter != endIter; ++currIter)
	{
		(*currIter)->update();
	}
}