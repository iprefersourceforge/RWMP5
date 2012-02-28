#pragma once
#include "havok.h"

class Physics
{
	hkpWorld* physicsWorld;
	hkJobQueue* jobQueue;
	hkJobThreadPool* threadPool;
public:
	Physics(void);
	~Physics(void);
	hkpWorld* GetPhysicsWorld(){return physicsWorld;}
	void SetUp(void);
	void Simulate(float);
	void TidyUp(void);
};


