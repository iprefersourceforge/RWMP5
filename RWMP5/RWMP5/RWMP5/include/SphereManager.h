#ifndef SPHEREMAN_H
#define SPHEREMAN_H

#include "SphereObject.h"
#include <vector>

class SphereManager
{
private:
	static SphereManager * mInstance;
	hkpWorld* mPhysicsWorld;
	SceneManager * mSceneMgr;
	std::vector<SphereObject*> mSpheres;
	std::vector<SphereObject*>::iterator currIter;
	std::vector<SphereObject*>::iterator endIter;
	SphereManager(hkpWorld* physicsWorld, SceneManager * sceneMgr);
public:
	~SphereManager();
	static SphereManager * getInstance(hkpWorld* physicsWorld = NULL, SceneManager * sceneMgr = NULL);
	void addSphere(float size, unsigned int id, hkVector4& position = hkVector4(0.0,0.0,0.0), float mass = 20.f, float restitution = .1f, float friction = .1f);
	SphereObject * newSphere(float size, unsigned int id, hkVector4& position = hkVector4(0.0,0.0,0.0), float mass = 20.f, float restitution = .1f, float friction = .1f);
	void update();
};

#endif