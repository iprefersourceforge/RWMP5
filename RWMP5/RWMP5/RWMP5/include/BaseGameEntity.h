#ifndef BASEGAMEENTITY_H
#define BASEGAMEENTITY_H

#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include "Physics.h"
#include <vector>

class BaseGameEntity{
public:
	enum ENTITYTYPE{ BLOCK, DESTRUCTABLE_BLOCK, PLATFORM, MOVING_PLATFORM, TILTING_PLATFORM, PRESSURE_PAD, SPHERE };
	BaseGameEntity(ENTITYTYPE type, hkVector4& pos, unsigned int id);
	BaseGameEntity();
	virtual ~BaseGameEntity(){};
	virtual void update(){}
	virtual void onContact()=0;
	ENTITYTYPE getType();
	char* m_name;
protected:
	ENTITYTYPE m_entitytype;
	hkVector4 m_position;
	unsigned int m_id;
};

#endif