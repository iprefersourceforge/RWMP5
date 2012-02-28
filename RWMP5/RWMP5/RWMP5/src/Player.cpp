#include "Player.h"

//public functions
Player::Player(unsigned int id, float size, hkVector4& pos, Ogre::SceneManager * sceneMgr, float mass, float restitution, float friction)
: mSceneMgr(sceneMgr)
, mSize(size)
, mMoveForward(false)
, mMoveBackward(false)
, mMoveLeft(false)
, mMoveRight(false)
{
	camMode = FREE_VIEW;
	mSphereMan = SphereManager::getInstance();

	mSphere = mSphereMan->newSphere(size, id, pos, mass, restitution, friction);
	mBody = mSphere->getBody();
}

Player::~Player(){}

void Player::Update(float tslf, Ogre::Quaternion orientation)
{
	mSphere->update();
	mDeltaTime = tslf;
	mOrientation = orientation;
	hkVector4 p(0,0,0);
	p = mBody->getPosition();
	mPosition = Vector3(p(0),p(1),p(2));

	HandleMovement();

}

void Player::HandleMovement()
{
	if (mMoveForward)
	{//move the player forward
		Vector3 v(0.0,0.0,1.0);
		Vector3 result = mOrientation * v;
		switch (camMode)
		{
		case FREE_VIEW:
			break;
		case TPS:
			
			mBody->applyLinearImpulse(hkVector4(result.x,0,result.z));
			break;
		case TOP_DOWN:
			//mBody->applyForce(mDeltaTime,hkVector4(0.0,0.0,-10.0));
			//mBody->applyTorque(mDeltaTime,hkVector4(0.0,0.0,-10.0));
			mBody->applyLinearImpulse(hkVector4(0.0,0.0,-1.0));
			break;
		}
		
	}
	else if (mMoveBackward)
	{//move the player backward
		Vector3 v(0.0,0.0,-1.0);
		Vector3 result = mOrientation * v;
			switch (camMode)
		{
		case FREE_VIEW:
			break;
		case TPS:
			
			mBody->applyLinearImpulse(hkVector4(result.x,0,result.z));
			break;
		case TOP_DOWN:
			mBody->applyLinearImpulse(hkVector4(0.0,0.0,1.0));
			break;
		}
	}

	if (mMoveLeft)
	{//move the player left
		Vector3 v(1.0,0.0,0.0);
		Vector3 result = mOrientation * v;
			switch (camMode)
		{
		case FREE_VIEW:
			break;
		case TPS:
			
			mBody->applyLinearImpulse(hkVector4(result.x,0,result.z));
			break;
		case TOP_DOWN:
			mBody->applyLinearImpulse(hkVector4(-1.0,0.0,0.0));
			break;
		}
	}
	else if (mMoveRight)
	{//move the player right
		Vector3 v(-1.0,0.0,0.0);
		Vector3 result = mOrientation * v;
			switch (camMode)
		{
		case FREE_VIEW:
			break;
		case TPS:
			mBody->applyLinearImpulse(hkVector4(result.x,0,result.z));
			break;
		case TOP_DOWN:
			//mBody->applyForce(mDeltaTime,hkVector4(-10.0,0.0,0.0));
			mBody->applyLinearImpulse(hkVector4(1.0,0.0,0.0));
			break;
		}
	}
}

hkpRigidBody* Player::getBody(){return mBody;}
unsigned int Player::getId(){return 0;/*m_id;*/}
Ogre::SceneNode* Player::getSceneNode(){return mNode;}
float Player::getSize(){return mSize;}
void Player::setForward(bool arg){mMoveForward = arg;}
void Player::setBackward(bool arg){mMoveBackward = arg;}
void Player::setLeft(bool arg){mMoveLeft = arg;}
void Player::setRight(bool arg){mMoveRight = arg;}
Vector3 Player::getPosition(){return mPosition;}
void Player::setCamMode(int cType){camMode = (CAM_MODE)cType;}