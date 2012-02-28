#ifndef PLAYER_H
#define PLAYER_H

//#include "BaseGameEntity.h"
#include "Ogre.h"
#include "SphereManager.h"
#include "SphereObject.h"

class Player
{
public:
	enum CAM_MODE { FREE_VIEW, TPS, TOP_DOWN };
	Player(unsigned int id, float size, hkVector4& pos, Ogre::SceneManager * sceneMgr, float mass = 20.f, float restitution = .1f, float friction = .1f);
	~Player();
	void Update(float tslf, Ogre::Quaternion orientation);

	void HandleMovement();
	//virtual void onContact();


	//getters/setters
	hkpRigidBody* getBody();
	unsigned int getId();
	Ogre::SceneNode* getSceneNode();
	float getSize();
	void setForward(bool arg);
	void setBackward(bool arg);
	void setLeft(bool arg);
	void setRight(bool arg);
	Vector3 getPosition();
	void setCamMode(int cType);
private:
	Ogre::SceneManager * mSceneMgr;
	hkpRigidBody* mBody;
	Ogre::SceneNode* mNode;
	SphereObject * mSphere;
	Ogre::Entity* mEntity;
	SphereManager * mSphereMan;
	float mSize;
	bool mMoveForward;
	bool mMoveBackward;
	bool mMoveLeft;
	bool mMoveRight;
	float mDeltaTime;
	Vector3 mPosition;
	Ogre::Quaternion mOrientation;
	CAM_MODE camMode;
};

#endif