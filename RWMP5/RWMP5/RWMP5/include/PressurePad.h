#ifndef PRESSUREPAD_H
#define PRESSUREPAD_H

#include "BoxManager.h"

class PressurePad
{
private:
	BoxObject * mPad;
	BoxManager * mBoxMan;
public:
	PressurePad(float size, unsigned int id, hkVector4& position, float mass, float restitution, float friction);
	PressurePad(float sizeX, float sizeY, float sizeZ, unsigned int id, hkVector4& position, float mass, float restitution, float friction);
	~PressurePad();
	void Update();
};

#endif