#include "PressurePad.h"

PressurePad::PressurePad(float size, unsigned int id, hkVector4& position, float mass, float restitution, float friction)
{
	mBoxMan = BoxManager::getInstance();
	mPad = mBoxMan->newBox(BoxObject::BOX_TYPE::PRESSURE_PAD, size, id, position, true, mass, restitution, friction);
}

PressurePad::PressurePad(float sizeX, float sizeY, float sizeZ, unsigned int id, hkVector4& position, float mass, float restitution, float friction)
{
	mBoxMan = BoxManager::getInstance();
	mPad = mBoxMan->newBox(BoxObject::BOX_TYPE::PRESSURE_PAD, sizeX, sizeY, sizeZ, id, position, true, mass, restitution, friction);
}


PressurePad::~PressurePad()
{

}

void PressurePad::Update()
{
	//mBoxMan->update();
}