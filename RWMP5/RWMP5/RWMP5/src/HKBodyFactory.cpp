#include "HKBodyFactory.h"

HKBodyFactory * HKBodyFactory::mInstance = 0;

HKBodyFactory::HKBodyFactory(Ogre::SceneManager * sceneMgr) 
	: mSceneMgr(sceneMgr)
{
	mIDGen = UniqueIDGenerator::getInstance();
}

HKBodyFactory::~HKBodyFactory()
{}

HKBodyFactory * HKBodyFactory::getInstance(Ogre::SceneManager * sceneMgr)
{
	if (mInstance == 0)
	{
		mInstance = new HKBodyFactory(sceneMgr);
	}
	return mInstance;
}

hkpRigidBody* HKBodyFactory::CreateSphere(hkVector4& pos, float size, float mass, float restitution, float friction)
{
	hkVector4 halfSize( size/2.0,size/2.0,size/2.0);
	hkReal radius(size/2.0);
	hkpSphereShape* m_shapeDef = new hkpSphereShape( radius);

	hkpRigidBodyCinfo sphereInfo;
	sphereInfo.m_mass = mass;
	//
	hkpMassProperties massProperties;
	hkpInertiaTensorComputer::computeSphereVolumeMassProperties(radius, sphereInfo.m_mass, massProperties);
	//
	sphereInfo.m_mass = massProperties.m_mass;
	sphereInfo.m_centerOfMass = massProperties.m_centerOfMass;
	sphereInfo.m_inertiaTensor = massProperties.m_inertiaTensor;
	sphereInfo.m_solverDeactivation = sphereInfo.SOLVER_DEACTIVATION_MEDIUM; // used to deal with resting contacts
	sphereInfo.m_shape = m_shapeDef;
	sphereInfo.m_qualityType = HK_COLLIDABLE_QUALITY_MOVING;// default collision quality
	sphereInfo.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;
	sphereInfo.m_friction = friction;
	sphereInfo.m_restitution = restitution;
	sphereInfo.m_position = pos;

	hkpRigidBody* m_body = new hkpRigidBody(sphereInfo);

	mBodies.pushBack(m_body);
	return m_body;
}

hkpRigidBody* HKBodyFactory::CreateBox(hkVector4& pos, float sizeX, float sizeY, float sizeZ, bool isStatic, float mass, float restitution, float friction)
{
	hkVector4 halfSize( sizeX/2.f,sizeY/2.f,sizeZ/2.f);
	hkpBoxShape* m_shapeDef = new hkpBoxShape( halfSize , 0 );
	m_shapeDef->setRadius( 0.0f );

	hkpRigidBodyCinfo boxInfo;
	boxInfo.m_friction = friction;
	boxInfo.m_restitution = restitution;
	if (isStatic)
	{
		boxInfo.m_motionType = hkpMotion::MOTION_FIXED;
		boxInfo.m_qualityType = HK_COLLIDABLE_QUALITY_FIXED;// default collision quality
	}
	else
	{
		boxInfo.m_motionType = hkpMotion::MOTION_BOX_INERTIA;
		boxInfo.m_mass = mass;
		hkpMassProperties massProperties;
		hkpInertiaTensorComputer::computeBoxVolumeMassProperties(halfSize, boxInfo.m_mass, massProperties);
		boxInfo.m_qualityType = HK_COLLIDABLE_QUALITY_MOVING;// default collision quality
		boxInfo.m_mass = massProperties.m_mass;
		boxInfo.m_centerOfMass = massProperties.m_centerOfMass;
		boxInfo.m_inertiaTensor = massProperties.m_inertiaTensor;

	}
	boxInfo.m_shape = m_shapeDef;
	boxInfo.m_solverDeactivation = boxInfo.SOLVER_DEACTIVATION_MEDIUM; // used to deal with resting contacts
	boxInfo.m_position = pos;



	hkpRigidBody* m_body = new hkpRigidBody(boxInfo);

	mBodies.pushBack(m_body);
	return m_body;
}


