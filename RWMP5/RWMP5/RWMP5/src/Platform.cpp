#include "Platform.h"

Platform::Platform(float size, unsigned int id, hkVector4& position, float mass, float restitution, float friction)
{
	mBoxMan = BoxManager::getInstance();
	mPad = mBoxMan->newBox(BoxObject::BOX_TYPE::PLATFORM, size, id, position, true, mass, restitution, friction);
}

Platform::Platform(float sizeX, float sizeY, float sizeZ, unsigned int id, hkVector4& position, float mass, float restitution, float friction)
{
	mBoxMan = BoxManager::getInstance();
	mPad = mBoxMan->newBox(BoxObject::BOX_TYPE::PLATFORM, sizeX, sizeY, sizeZ, id, position, true, mass, restitution, friction);
}


Platform::~Platform()
{

}

void Platform::Update()
{
	//mBoxMan->update();
}