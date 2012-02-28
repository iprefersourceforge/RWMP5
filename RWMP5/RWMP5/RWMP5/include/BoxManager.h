#ifndef BOXMAN_H
#define BOXMAN_H

#include "BoxObject.h"
#include <vector>

class BoxManager
{
	private:
	static BoxManager * mInstance;
	hkpWorld* mPhysicsWorld;
	SceneManager * mSceneMgr;
	std::vector<BoxObject*> mBoxes;
	std::vector<BoxObject*>::iterator currIter;
	std::vector<BoxObject*>::iterator endIter;
	BoxManager(hkpWorld* physicsWorld, SceneManager * sceneMgr);
public:
	~BoxManager();
	static BoxManager * getInstance(hkpWorld* physicsWorld = NULL, SceneManager * sceneMgr = NULL);
	void addBox(BoxObject::BOX_TYPE bType, float size, unsigned int id, hkVector4& position = hkVector4(0.0,0.0,0.0), bool isStatic = false, float mass = 20.f, float restitution = .1f, float friction = .1f);
	void addBox(BoxObject::BOX_TYPE bType, float sizeX, float sizeY, float sizeZ, unsigned int id, hkVector4& position = hkVector4(0.0,0.0,0.0), bool isStatic = false, float mass = 20.f, float restitution = .1f, float friction = .1f);

	BoxObject * newBox(BoxObject::BOX_TYPE bType, float size, unsigned int id, hkVector4& position = hkVector4(0.0,0.0,0.0), bool isStatic = false, float mass = 20.f, float restitution = .1f, float friction = .1f);
	BoxObject * newBox(BoxObject::BOX_TYPE bType, float sizeX, float sizeY, float sizeZ, unsigned int id, hkVector4& position = hkVector4(0.0,0.0,0.0), bool isStatic = false, float mass = 20.f, float restitution = .1f, float friction = .1f);
	void update();
};

#endif