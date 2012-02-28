#include "BoxManager.h"

BoxManager * BoxManager::mInstance = 0;

BoxManager::BoxManager(hkpWorld* physicsWorld, SceneManager * sceneMgr)
	: mPhysicsWorld(physicsWorld)
	, mSceneMgr(sceneMgr)
{

}

BoxManager::~BoxManager()
{

}

BoxManager * BoxManager::getInstance(hkpWorld* physicsWorld, SceneManager * sceneMgr)
{
	if (mInstance == 0)
	{
		mInstance = new BoxManager(physicsWorld, sceneMgr);
	}
	return mInstance;
}

void BoxManager::addBox(BoxObject::BOX_TYPE bType, float size, unsigned int id, hkVector4& position, bool isStatic, float mass, float restitution, float friction)
{
	BoxObject *b = new BoxObject(bType, size, mSceneMgr, id, position, isStatic, mass, restitution, friction);
	mPhysicsWorld->addEntity((hkpEntity*)b->getBody());
	mBoxes.push_back(b);
}

void BoxManager::addBox(BoxObject::BOX_TYPE bType, float sizeX, float sizeY, float sizeZ, unsigned int id, hkVector4& position, bool isStatic, float mass, float restitution, float friction)
{
	BoxObject *b = new BoxObject(bType, sizeX, sizeY, sizeZ, mSceneMgr, id, position, isStatic, mass, restitution, friction);
	mPhysicsWorld->addEntity((hkpEntity*)b->getBody());
	mBoxes.push_back(b);
}

BoxObject * BoxManager::newBox(BoxObject::BOX_TYPE bType, float size, unsigned int id, hkVector4& position, bool isStatic, float mass, float restitution, float friction)
{
	BoxObject *b = new BoxObject(bType, size, mSceneMgr, id, position, isStatic, mass, restitution, friction);
	mPhysicsWorld->addEntity((hkpEntity*)b->getBody());
	mBoxes.push_back(b);
	return b;
}

BoxObject * BoxManager::newBox(BoxObject::BOX_TYPE bType, float sizeX, float sizeY, float sizeZ, unsigned int id, hkVector4& position, bool isStatic, float mass, float restitution, float friction)
{
	BoxObject *b = new BoxObject(bType, sizeX, sizeY, sizeZ, mSceneMgr, id, position, isStatic, mass, restitution, friction);
	mPhysicsWorld->addEntity((hkpEntity*)b->getBody());
	mBoxes.push_back(b);
	return b;
}

void BoxManager::update()
{
	currIter = mBoxes.begin();
	endIter = mBoxes.end();
	for (; currIter != endIter; ++currIter)
	{
		(*currIter)->update();
	}
}