#include "BoxObject.h"

BoxObject::BoxObject(BOX_TYPE bType, float size, SceneManager * sceneMgr, unsigned int id, hkVector4& position, bool isStatic, float mass, float restitution, float friction)
	: BaseGameEntity(BaseGameEntity::ENTITYTYPE((int)bType), position, id), m_sizeX(size), m_sizeY(size), m_sizeZ(size)
	, boxType(bType)
	, m_sceneMgr(sceneMgr)
	, mIsStatic(isStatic)
{
	bodyFactory = HKBodyFactory::getInstance();
	m_body = bodyFactory->CreateBox(position, size, size, size, isStatic, mass, restitution, friction);
	m_node = m_sceneMgr->getRootSceneNode()->createChildSceneNode("BoxNode" + StringConverter::toString(m_id));
	m_entity = m_sceneMgr->createEntity("Box" + StringConverter::toString(m_id), "cube.mesh");
	m_entity->setMaterialName("Examples/BumpyMetal");
	AxisAlignedBox abb = m_entity->getBoundingBox();
	Vector3 sz = abb.getSize();
	float scalesizex = m_sizeX/sz.x;
	float scalesizey = m_sizeY/sz.y;
	float scalesizez = m_sizeZ/sz.z;
	m_node->setPosition(position.getComponent(0),position.getComponent(1),position.getComponent(2));
	m_node->setScale(scalesizex,scalesizey,scalesizez);
	m_node->attachObject(m_entity);
}

BoxObject::BoxObject(BOX_TYPE bType, float sizeX, float sizeY, float sizeZ, SceneManager * sceneMgr, unsigned int id, hkVector4& position, bool isStatic, float mass, float restitution, float friction)
	: BaseGameEntity(BaseGameEntity::ENTITYTYPE((int)bType), position, id), m_sizeX(sizeX), m_sizeY(sizeY), m_sizeZ(sizeZ)
	, boxType(bType)
	, m_sceneMgr(sceneMgr)
	, mIsStatic(isStatic)
{
	bodyFactory = HKBodyFactory::getInstance();
	m_body = bodyFactory->CreateBox(position, sizeX, sizeY, sizeZ, isStatic, mass, restitution, friction);

	m_node = m_sceneMgr->getRootSceneNode()->createChildSceneNode("BoxNode" + StringConverter::toString(m_id));
	m_entity = m_sceneMgr->createEntity("Box" + StringConverter::toString(m_id), "cube.mesh");
	m_entity->setMaterialName("Examples/BumpyMetal");
	AxisAlignedBox abb = m_entity->getBoundingBox();
	Vector3 sz = abb.getSize();
	float scalesizex = m_sizeX/sz.x;
	float scalesizey = m_sizeY/sz.y;
	float scalesizez = m_sizeZ/sz.z;
	m_node->setPosition(position.getComponent(0),position.getComponent(1),position.getComponent(2));
	m_node->setScale(scalesizex,scalesizey,scalesizez);
	m_node->attachObject(m_entity);
}

BoxObject::~BoxObject(){}

void BoxObject::update()
{
	hkVector4 p(0,0,0);
	p = m_body->getPosition();
	Vector3 pos = Vector3(p(0),p(1),p(2));
	m_node->setPosition(pos);

	hkQuaternion q; 
	q = hkQuaternion(m_body->getRotation());
	angleRot = q.getAngle();

	hkVector4 axis = hkVector4(0,0,0);
	if(q.hasValidAxis())
		q.getAxis(axis); 

	axisRot=Ogre::Vector3(axis(0),axis(1),axis(2));
	newOgRot=Ogre::Quaternion(angleRot,axisRot);

	m_node->setOrientation(newOgRot);
}

void BoxObject::onContact()
{

}

hkpRigidBody* BoxObject::getBody()
{
	return m_body;
}