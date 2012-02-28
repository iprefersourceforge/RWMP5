#include "SphereObject.h"

SphereObject::SphereObject(float size, SceneManager * sceneMgr, unsigned int id, hkVector4& position, float mass, float restitution, float friction)
: BaseGameEntity(BaseGameEntity::ENTITYTYPE::SPHERE, position, id)
, m_size(size)
, m_sceneMgr(sceneMgr)
{
	bodyFactory = HKBodyFactory::getInstance();
	m_body = bodyFactory->CreateSphere(position, size, mass, restitution, friction);

	m_node = m_sceneMgr->getRootSceneNode()->createChildSceneNode("SphereNode" + StringConverter::toString(m_id));
	m_entity = m_sceneMgr->createEntity("Sphere" + StringConverter::toString(m_id), "TileSphere.mesh");
	//m_entity->setMaterialName("Examples/Chrome");
	AxisAlignedBox abb = m_entity->getBoundingBox();
	Vector3 sz = abb.getSize();
	float scalesize = size/sz.x;
	m_node->setPosition(position.getComponent(0),position.getComponent(1),position.getComponent(2));
	m_node->setScale(scalesize,scalesize,scalesize);
	m_node->attachObject(m_entity);
	m_size = size;
}

SphereObject::~SphereObject(){}

void SphereObject::update()
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

void SphereObject::onContact()
{

}

hkpRigidBody* SphereObject::getBody()
{
	return m_body;
}