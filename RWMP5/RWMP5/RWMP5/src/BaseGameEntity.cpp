#include "BaseGameEntity.h"

BaseGameEntity::BaseGameEntity()
{

}

BaseGameEntity::ENTITYTYPE BaseGameEntity::getType()
{
	return m_entitytype;
}

BaseGameEntity::BaseGameEntity(ENTITYTYPE type, hkVector4& pos, unsigned int id)
{
	m_entitytype = type;
	m_position = pos;
	m_id = id;
}