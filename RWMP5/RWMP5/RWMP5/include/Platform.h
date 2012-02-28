#ifndef PLATFORM_H
#define PLATFORM_H

#include "BoxManager.h"

class Platform
{
private:
	BoxObject * mPad;
	BoxManager * mBoxMan;
public:
	Platform(float size, unsigned int id, hkVector4& position, float mass, float restitution, float friction);
	Platform(float sizeX, float sizeY, float sizeZ, unsigned int id, hkVector4& position, float mass, float restitution, float friction);
	~Platform();
	void Update();
};

#endif