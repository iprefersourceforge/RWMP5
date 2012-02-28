/*
-----------------------------------------------------------------------------
Filename:    RWMP5.cpp
-----------------------------------------------------------------------------


This source file is generated by the
___                   _              __    __ _                  _ 
/___\__ _ _ __ ___    /_\  _ __  _ __/ / /\ \ (_)______ _ _ __ __| |
//  // _` | '__/ _ \  //_\\| '_ \| '_ \ \/  \/ / |_  / _` | '__/ _` |
/ \_// (_| | | |  __/ /  _  \ |_) | |_) \  /\  /| |/ / (_| | | | (_| |
\___/ \__, |_|  \___| \_/ \_/ .__/| .__/ \/  \/ |_/___\__,_|_|  \__,_|
|___/                 |_|   |_|                                 
Ogre 1.7.x Application Wizard for VC10 (August 2010)
http://code.google.com/p/ogreappwizards/
-----------------------------------------------------------------------------
*/
#include "RWMP5.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "../res/resource.h"
#endif


//-------------------------------------------------------------------------------------
RWMP5::RWMP5(void)
	: mRoot(0),
	mCamera(0),
	mSceneMgr(0),
	mWindow(0),
	mResourcesCfg(Ogre::StringUtil::BLANK),
	mPluginsCfg(Ogre::StringUtil::BLANK),
	mTrayMgr(0),
	mCameraMan(0),
	mDetailsPanel(0),
	mCursorWasVisible(false),
	mShutDown(false),
	mInputManager(0),
	mMouse(0),
	mKeyboard(0)
{
}

//-------------------------------------------------------------------------------------
RWMP5::~RWMP5(void)
{
	if (mTrayMgr) delete mTrayMgr;
	if (mCameraMan) delete mCameraMan;

	//Remove ourself as a Window listener
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
	delete mRoot;
}

//-------------------------------------------------------------------------------------
bool RWMP5::configure(void)
{
	// Show the configuration dialog and initialise the system
	// You can skip this and use root.restoreConfig() to load configuration
	// settings if you were sure there are valid ones saved in ogre.cfg
	if(mRoot->showConfigDialog())
	{
		// If returned true, user clicked OK so initialise
		// Here we choose to let the system create a default rendering window by passing 'true'
		mWindow = mRoot->initialise(true, "RWMP5 Render Window");

		// Let's add a nice window icon
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		HWND hwnd;
		mWindow->getCustomAttribute("WINDOW", (void*)&hwnd);
		LONG iconID   = (LONG)LoadIcon( GetModuleHandle(0), MAKEINTRESOURCE(IDI_APPICON) );
		SetClassLong( hwnd, GCL_HICON, iconID );
#endif
		return true;
	}
	else
	{
		return false;
	}
}
//-------------------------------------------------------------------------------------
void RWMP5::chooseSceneManager(void)
{
	// Get the SceneManager, in this case a generic one
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
}
//-------------------------------------------------------------------------------------
void RWMP5::createCamera(void)
{
	// Create the camera
	mCamera = mSceneMgr->createCamera("PlayerCam");
	camType = FREE_VIEW;

	switch (camType)
	{
	case FREE_VIEW:
		//original cam settings
		// Position it at 500 in Z direction
		mCamera->setPosition(Ogre::Vector3(0,20,80));
		// Look back along -Z
		//mCamera->lookAt(Ogre::Vector3(0,0,-300));
		mCamera->setNearClipDistance(1);
		break;
	case TPS:
		break;
	case TOP_DOWN:
		// Create the camera facing downwards
		mCamera->setPosition(Ogre::Vector3(0,25,0));
		mCamera->setNearClipDistance(5);
		mCamera->pitch(Ogre::Degree(-90));
		break;
	}

	// Setup basic node structure to handle 3rd person cameras
    mMainTPSNode = mSceneMgr->getRootSceneNode ()->createChildSceneNode ("TPSNODE");
    mTPSSightNode = mMainTPSNode->createChildSceneNode ("TPSNODE_sight", Vector3 (0, 0, 10));
    mTPSCameraNode = mMainTPSNode->createChildSceneNode ("TPSNODE_camera", Vector3 (0, 0.2, -3));
	mTPSCameraNode->setAutoTracking (true, mTPSSightNode); // The camera will always look at the camera target
    mTPSCameraNode->setFixedYawAxis (true); // Needed because of auto tracking
	mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // create a default camera controller


}
//-------------------------------------------------------------------------------------
void RWMP5::createFrameListener(void)
{
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	mInputManager = OIS::InputManager::createInputSystem( pl );

	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);

	//Set initial mouse clipping size
	windowResized(mWindow);

	//Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mMouse, this);
	mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
	mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
	mTrayMgr->hideCursor();

	// create a params panel for displaying sample details
	Ogre::StringVector items;
	items.push_back("cam.pX");
	items.push_back("cam.pY");
	items.push_back("cam.pZ");
	items.push_back("");
	items.push_back("cam.oW");
	items.push_back("cam.oX");
	items.push_back("cam.oY");
	items.push_back("cam.oZ");
	items.push_back("");
	items.push_back("Filtering");
	items.push_back("Poly Mode");

	mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
	mDetailsPanel->setParamValue(9, "Bilinear");
	mDetailsPanel->setParamValue(10, "Solid");
	mDetailsPanel->hide();

	mRoot->addFrameListener(this);
}
//-------------------------------------------------------------------------------------
void RWMP5::destroyScene(void)
{
}
//-------------------------------------------------------------------------------------
void RWMP5::createViewports(void)
{
	// Create one viewport, entire window
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}
//-------------------------------------------------------------------------------------
void RWMP5::setupResources(void)
{
	// Load resource paths from config file
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);

	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}
}
//-------------------------------------------------------------------------------------
void RWMP5::createResourceListener(void)
{

}
//-------------------------------------------------------------------------------------
void RWMP5::loadResources(void)
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//-------------------------------------------------------------------------------------
void RWMP5::go(void)
{
#ifdef _DEBUG
	mResourcesCfg = "resources_d.cfg";
	mPluginsCfg = "plugins_d.cfg";
#else
	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";
#endif

	if (!setup())
		return;

	mRoot->startRendering();

	// clean up
	destroyScene();
}
//-------------------------------------------------------------------------------------
bool RWMP5::setup(void)
{
	mRoot = new Ogre::Root(mPluginsCfg);

	setupResources();

	bool carryOn = configure();
	if (!carryOn) return false;

	chooseSceneManager();
	createCamera();
	createViewports();

	// Set default mipmap level (NB some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// Create any resource listeners (for loading screens)
	createResourceListener();
	// Load resources
	loadResources();

	// Create the scene
	createScene();

	createFrameListener();

	return true;
};
//-------------------------------------------------------------------------------------
void RWMP5::CreatePlanes(){
	hkVector4 groundBox( 200.0f, 2.0f, 200.0f );
	hkVector4 position( 0.0f, 0.0f, 0.0f );
	hkpConvexShape* shape = new hkpBoxShape( groundBox , 0 );

	hkpRigidBodyCinfo ci;

	ci.m_shape = shape;
	ci.m_motionType = hkpMotion::MOTION_FIXED;
	ci.m_position = position;
	ci.m_qualityType = HK_COLLIDABLE_QUALITY_FIXED;
	ci.m_restitution=1.0;
	ci.m_friction=0.8;

	hkpRigidBody* floor=new hkpRigidBody( ci );
	physics.GetPhysicsWorld()->addEntity( floor );

	floor->removeReference(); 
	shape->removeReference();


	Ogre::MeshPtr p =Ogre::MeshManager::getSingleton().createPlane("GroundPlane", 
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		Ogre::Plane(Ogre::Vector3(0.0,1.0,0.0),
		Ogre::Vector3(position(0),position(1),position(2)))
		,600,600,20,20,true, 1,1.0f,1.0f,Ogre::Vector3::UNIT_X);

	Ogre::SceneNode* planeNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	Ogre::Entity* entGround = mSceneMgr->createEntity("Viewer_ZXPlane","GroundPlane");
	entGround->setMaterialName("Examples/Rockwall");
	entGround->setCastShadows(false);

	planeNode->attachObject(entGround);
}
//-------------------------------------------------------------------------------------
void RWMP5::createScene(void)
{
	// Set ambient light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	// Create a light
	Ogre::Light* l = mSceneMgr->createLight("MainLight");
	l->setPosition(20,80,50);

	physics.SetUp();
	mIDGen = UniqueIDGenerator::getInstance();
	mBodyFactory = HKBodyFactory::getInstance(mSceneMgr);
	mSphereMan = SphereManager::getInstance(physics.GetPhysicsWorld(), mSceneMgr);
	mBoxMan = BoxManager::getInstance(physics.GetPhysicsWorld(), mSceneMgr);


	//create the player
	mPlayer = new Player(mIDGen->getNewID(), 1, hkVector4(20.0,18.0,0.0), mSceneMgr);


	pPad = new PressurePad(2.f, .2f, 2.f, mIDGen->getNewID(), hkVector4(30.0f, 16.1f, 5.0f ), 20.f,.1f,1.f);
	pPlat = new Platform(20.f, 2.f, 20.f, mIDGen->getNewID(), hkVector4(25.0f, 15.0f, 0.0f ), 20.f,.1f,1.f);

	mBoxMan->addBox(BoxObject::BOX_TYPE::BLOCK, 1.f, mIDGen->getNewID(), hkVector4(25.0f, 25.0f, 0.0f ), false);
	mSphereMan->addSphere( 2.f, mIDGen->getNewID(), hkVector4(25.0f, 30.0f, 0.0f ));
	
	CreatePlanes();
}
//-------------------------------------------------------------------------------------
bool RWMP5::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if(mWindow->isClosed())
		return false;

	if(mShutDown)
		return false;


	physics.Simulate(evt.timeSinceLastFrame);
	mBoxMan->update();
	mSphereMan->update();

	/*bObj->update();
	bObj2->update();
	sObj->update();*/
	//Need to capture/update each device
	mKeyboard->capture();
	mMouse->capture();
	mPlayer->Update(evt.timeSinceLastFrame, mMainTPSNode->getOrientation());

	switch (camType)
	{
	case FREE_VIEW:
		break;
	case TPS:
		mMainTPSNode->setPosition(mPlayer->getPosition());
		//mCamera->setPosition(mTPSCameraNode->getPosition());
		break;
	case TOP_DOWN:
		mCamera->setPosition(Ogre::Vector3(mPlayer->getPosition().x,mCamera->getPosition().y, mPlayer->getPosition().z));
		break;
	}

	mTrayMgr->frameRenderingQueued(evt);

	if (!mTrayMgr->isDialogVisible())
	{
		mCameraMan->frameRenderingQueued(evt);   // if dialog isn't up, then update the camera
		if (mDetailsPanel->isVisible())   // if details panel is visible, then update its contents
		{
			mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(mCamera->getDerivedPosition().x));
			mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(mCamera->getDerivedPosition().y));
			mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(mCamera->getDerivedPosition().z));
			mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().w));
			mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().x));
			mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().y));
			mDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().z));
		}
	}

	return true;
}
//-------------------------------------------------------------------------------------
bool RWMP5::keyPressed( const OIS::KeyEvent &arg )
{
	if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up



	if (arg.key == OIS::KC_Z) 
	{
		Quaternion q = Quaternion();
		switch (camType)
		{
		case FREE_VIEW:
			camType = TPS;
			mMainTPSNode->setPosition(mPlayer->getPosition());
			mCamera->setPosition(mTPSCameraNode->getPosition());
			mCamera->setPosition(0.0,0.0,0.0);
			mCamera->setOrientation(q);
			mTPSCameraNode->attachObject(mCamera);
			break;
		case TPS:
			mTPSCameraNode->detachObject(mCamera);
			mCamera->setOrientation(q);
			mCamera->pitch(Ogre::Degree(-90));
			camType = TOP_DOWN;
			TDCamPos = Vector3(mPlayer->getPosition().x, mPlayer->getPosition().y + 10, mPlayer->getPosition().z);
			mCamera->setPosition(TDCamPos);
			break;
		case TOP_DOWN:
			camType = FREE_VIEW;
			break;
		}
		mPlayer->setCamMode((int)camType);
	}
	if (arg.key == OIS::KC_F)   // toggle visibility of advanced frame stats
	{
		mTrayMgr->toggleAdvancedFrameStats();
	}
	else if (arg.key == OIS::KC_G)   // toggle visibility of even rarer debugging details
	{
		if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
		{
			mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
			mDetailsPanel->show();
		}
		else
		{
			mTrayMgr->removeWidgetFromTray(mDetailsPanel);
			mDetailsPanel->hide();
		}
	}
	else if (arg.key == OIS::KC_T)   // cycle polygon rendering mode
	{
		Ogre::String newVal;
		Ogre::TextureFilterOptions tfo;
		unsigned int aniso;

		switch (mDetailsPanel->getParamValue(9).asUTF8()[0])
		{
		case 'B':
			newVal = "Trilinear";
			tfo = Ogre::TFO_TRILINEAR;
			aniso = 1;
			break;
		case 'T':
			newVal = "Anisotropic";
			tfo = Ogre::TFO_ANISOTROPIC;
			aniso = 8;
			break;
		case 'A':
			newVal = "None";
			tfo = Ogre::TFO_NONE;
			aniso = 1;
			break;
		default:
			newVal = "Bilinear";
			tfo = Ogre::TFO_BILINEAR;
			aniso = 1;
		}

		Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
		Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
		mDetailsPanel->setParamValue(9, newVal);
	}
	else if (arg.key == OIS::KC_R)   // cycle polygon rendering mode
	{
		Ogre::String newVal;
		Ogre::PolygonMode pm;

		switch (mCamera->getPolygonMode())
		{
		case Ogre::PM_SOLID:
			newVal = "Wireframe";
			pm = Ogre::PM_WIREFRAME;
			break;
		case Ogre::PM_WIREFRAME:
			newVal = "Points";
			pm = Ogre::PM_POINTS;
			break;
		default:
			newVal = "Solid";
			pm = Ogre::PM_SOLID;
		}

		mCamera->setPolygonMode(pm);
		mDetailsPanel->setParamValue(10, newVal);
	}
	else if(arg.key == OIS::KC_F5)   // refresh all textures
	{
		Ogre::TextureManager::getSingleton().reloadAll();
	}
	else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
	{
		mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
	}
	else if (arg.key == OIS::KC_ESCAPE)
	{
		mShutDown = true;
	}

	switch (camType)
	{
	case FREE_VIEW:
		mCameraMan->injectKeyDown(arg);
			break;
	case TPS:
	case TOP_DOWN:
		if (arg.key == OIS::KC_W){mPlayer->setForward(true);}
		if (arg.key == OIS::KC_S){mPlayer->setBackward(true);}
		if (arg.key == OIS::KC_A){mPlayer->setLeft(true);}
		if (arg.key == OIS::KC_D){mPlayer->setRight(true);}
		break;
	}
	;
	return true;
}

bool RWMP5::keyReleased( const OIS::KeyEvent &arg )
{
	switch (camType)
	{
	case FREE_VIEW:
		mCameraMan->injectKeyUp(arg);
		break;
	case TPS:
	case TOP_DOWN:
		if (arg.key == OIS::KC_W){mPlayer->setForward(false);}
		if (arg.key == OIS::KC_S){mPlayer->setBackward(false);}
		if (arg.key == OIS::KC_A){mPlayer->setLeft(false);}
		if (arg.key == OIS::KC_D){mPlayer->setRight(false);}
		break;
	}

	return true;
}

bool RWMP5::mouseMoved( const OIS::MouseEvent &arg )
{
	switch (camType)
	{
	case FREE_VIEW:
		if (mTrayMgr->injectMouseMove(arg)) return true;
		mCameraMan->injectMouseMove(arg);
		break;
	case TPS:
		mMainTPSNode->yaw(Ogre::Degree(-arg.state.X.rel * 0.25f));
		//mMainTPSNode->pitch(Ogre::Degree(-arg.state.Y.rel * 0.25f));
		break;
	case TOP_DOWN:
		TDCamPos.y = TDCamPos.y + arg.state.Y.rel * 0.025f;
		if (TDCamPos.y < 8) TDCamPos.y = 8;
		mCamera->setPosition(TDCamPos);
		break;
	}
	return true;
}

bool RWMP5::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if (mTrayMgr->injectMouseDown(arg, id)) return true;
	mCameraMan->injectMouseDown(arg, id);
	return true;
}

bool RWMP5::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	if (mTrayMgr->injectMouseUp(arg, id)) return true;
	mCameraMan->injectMouseUp(arg, id);
	return true;
}

//Adjust mouse clipping area
void RWMP5::windowResized(Ogre::RenderWindow* rw)
{
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void RWMP5::windowClosed(Ogre::RenderWindow* rw)
{
	//Only close for window that created OIS (the main window in these demos)
	if( rw == mWindow )
	{
		if( mInputManager )
		{
			mInputManager->destroyInputObject( mMouse );
			mInputManager->destroyInputObject( mKeyboard );

			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = 0;
		}
	}
}


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
	int main(int argc, char *argv[])
#endif
	{
		// Create application object
		RWMP5 app;

		try {
			app.go();
		} catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
			std::cerr << "An exception has occured: " <<
				e.getFullDescription().c_str() << std::endl;
#endif
		}

		return 0;
	}

#ifdef __cplusplus
}
#endif
