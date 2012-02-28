#ifndef CONTACTLISTENER_H
#define CONTACTLISTENER_H

#include "Physics.h"


//class BaseGameEntity;
//class BaseBlockObject;
//class BlockManager;

//#define IS_PIG(x) (x->getType() == BaseGameEntity::ENTITYTYPE::BLOCK)

class ContactListener: public hkpContactListener{
public:
	virtual void contactPointCallback( const hkpContactPointEvent& event ) 
	{
		// Disable normal collision response.
		//event.m_contactPointProperties->m_flags |= hkContactPointMaterial::CONTACT_IS_DISABLED;

		hkContactPoint cp = *event.m_contactPoint;

		// The impulse we apply is a function of the contact normal.
		hkVector4 impulse;
		{
			// If the listener was attached to the B body in this collision, then the contact normal
			// is the wrong way around.
			hkReal factor = ( event.m_source == hkpCollisionEvent::SOURCE_A ) ? 1.5f : -1.5f;
			impulse.setMul4( factor, event.m_contactPoint->getNormal() );
		}

		//hkpRigidBody* box = event.getBody( event.m_source );
		//hkUlong id = box->getUserData();

		//m_blockManager = BlockManager::instance();
		//hkArray<BaseBlockObject*>* blocks = m_blockManager->m_blocks;

		//m_pigManager = PigManager::instance();
		//hkArray<BasePigObject*>* pigs = m_pigManager->m_pigs;

		//m_birdFactory = BirdFactory::getInstance();
		//hkArray<BaseBird*>* birds = m_birdFactory->m_birds;

		//for(int i=0;i< m_blockManager->m_blocks->getSize();i++){
		//	if((*blocks)[i]->getId() == id){
		//		(*blocks)[i]->onContact();
		//	}
		//}

		//for(int i=0;i< m_pigManager->m_pigs->getSize();i++){
		//	if((*pigs)[i]->getId() == id){
		//		(*pigs)[i]->onContact();
		//	}
		//}

		//for(int i=0;i< m_birdFactory->m_birds->getSize();i++){
		//	if((*birds)[i]->getId() == id){
		//		(*birds)[i]->onContact();
		//	}
		//}

	}
private:
	//BlockManager* m_blockManager;
	//PigManager* m_pigManager;
	//BirdFactory* m_birdFactory;
};

#endif