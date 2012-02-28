
// Keycode
#include <Common/Base/keycode.cxx>

#if !defined HK_FEATURE_PRODUCT_PHYSICS
#error Physics is needed to build this demo. It is included in the common package for reference only.
#endif

// This excludes libraries that are not going to be linked
// from the project configuration, even if the keycodes are
// present

#undef HK_FEATURE_PRODUCT_AI
#undef HK_FEATURE_PRODUCT_ANIMATION
#undef HK_FEATURE_PRODUCT_CLOTH
#undef HK_FEATURE_PRODUCT_DESTRUCTION
#undef HK_FEATURE_PRODUCT_BEHAVIOR
#define HK_EXCLUDE_LIBRARY_hkgpConvexDecomposition
#define HK_FEATURE_REFLECTION_PHYSICS
#define HK_CLASSES_FILE <Common/Serialize/Classlist/hkClasses.h>
#include <Common/Base/Config/hkProductFeatures.cxx>

#include "Physics.h"
Physics::Physics(void)
{
}

Physics::~Physics(void)
{
}

void Physics::SetUp(void)
{	//
	// Do platform specific initialization
	//
#if !defined(HK_PLATFORM_WIN32)
	extern void initPlatform();
	initPlatform();
#endif
	
	//
	// Initialize the base system including our memory system
	//

	// Allocate 0.5MB of physics solver buffer.
	hkMemoryRouter* memoryRouter = hkMemoryInitUtil::initDefault(hkMallocAllocator::m_defaultMallocAllocator,	hkMemorySystem::FrameInfo(500000));
	hkBaseSystem::init( memoryRouter, errorReport );

		//
		// Initialize the multi-threading classes, hkJobQueue, and hkJobThreadPool
		//

		// They can be used for all Havok multithreading tasks. In this exmaple we only show how to use
		// them for physics, but you can reference other multithreading demos in the demo framework
		// to see how to multithread other products. The model of usage is the same as for physics.
		// The hkThreadpool has a specified number of threads that can run Havok jobs.  These can work
		// alongside the main thread to perform any Havok multi-threadable computations.
		// The model for running Havok tasks in Spus and in auxilary threads is identical.  It is encapsulated in the
		// class hkJobThreadPool.  On PlayStation(R)3 we initialize the SPU version of this class, which is simply a SPURS taskset.
		// On other multi-threaded platforms we initialize the CPU version of this class, hkCpuJobThreadPool, which creates a pool of threads
		// that run in exactly the same way.  On the PlayStation(R)3 we could also create a hkCpuJobThreadPool.  However, it is only
		// necessary (and advisable) to use one Havok PPU thread for maximum efficiency. In this case we simply use this main thread
		// for this purpose, and so do not create a hkCpuJobThreadPool.
		//hkJobThreadPool* threadPool;

		// We can cap the number of threads used - here we use the maximum for whatever multithreaded platform we are running on. This variable is
		// set in the following code sections.
		int totalNumThreadsUsed;

	#if defined HK_PLATFORM_PS3_PPU

		hkSpuJobThreadPoolCinfo threadPoolCinfo;

		extern CellSpurs* initSpurs();
		HK_CELL_SPURS* spurs = initSpurs();

		hkSpuUtil* spuUtil = new hkSpuUtil( spurs );

		spuUtil->attachHelperThreadToSpurs();
		threadPoolCinfo.m_spuUtil = spuUtil;
		threadPoolCinfo.m_maxNumSpus = 5; // Use 5 SPUs for this example

		totalNumThreadsUsed = 1; // only use one CPU thread for PS3.

		// This line enables timers collection, by allocating 200 Kb per thread.  If you leave this at its default (0),
		// timer collection will not be enabled.
		threadPoolCinfo.m_perSpuMonitorBufferSize = 200000;
		threadPool = new hkSpuJobThreadPool( threadPoolCinfo );
		spuUtil->removeReference();

	#else

		// Get the number of physical threads available on the system
		hkHardwareInfo hwInfo;
		hkGetHardwareInfo(hwInfo);
		totalNumThreadsUsed = hwInfo.m_numThreads;

		// We use one less than this for our thread pool, because we must also use this thread for our simulation
		hkCpuJobThreadPoolCinfo threadPoolCinfo;
		threadPoolCinfo.m_numThreads = totalNumThreadsUsed - 1;

		// This line enables timers collection, by allocating 200 Kb per thread.  If you leave this at its default (0),
		// timer collection will not be enabled.
		threadPoolCinfo.m_timerBufferPerThreadAllocation = 200000;
		threadPool = new hkCpuJobThreadPool( threadPoolCinfo );

	#endif

		// We also need to create a Job queue. This job queue will be used by all Havok modules to run multithreaded work.
		// Here we only use it for physics.
		hkJobQueueCinfo info;
		info.m_jobQueueHwSetup.m_numCpuThreads = totalNumThreadsUsed;
		jobQueue = new hkJobQueue(info);

		//
		// Enable monitors for this thread.
		//

		// Monitors have been enabled for thread pool threads already (see above comment).
		hkMonitorStream::getInstance().resize(200000);

		//
		// <PHYSICS-ONLY>: Create the physics world.
		// At this point you would initialize any other Havok modules you are using.
		//
		
		{
			// The world cinfo contains global simulation parameters, including gravity, solver settings etc.
			hkpWorldCinfo worldInfo;

			// Set the simulation type of the world to multi-threaded.
			worldInfo.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_MULTITHREADED;

			// Flag objects that fall "out of the world" to be automatically removed - just necessary for this physics scene
			worldInfo.m_broadPhaseBorderBehaviour = 
					hkpWorldCinfo::BROADPHASE_BORDER_REMOVE_ENTITY;

			
			physicsWorld = new hkpWorld(worldInfo);

			// Disable deactivation, so that you can view timers in the VDB. This should not be done in your game.
			//physicsWorld->m_wantDeactivation = false;


			// When the simulation type is SIMULATION_TYPE_MULTITHREADED, in the debug build, the sdk performs checks
			// to make sure only one thread is modifying the world at once to prevent multithreaded bugs. Each thread
			// must call markForRead / markForWrite before it modifies the world to enable these checks.
			physicsWorld->markForWrite();


			// Register all collision agents, even though only box - box will be used in this particular example.
			// It's important to register collision agents before adding any entities to the world.
			hkpAgentRegisterUtil::registerAllAgents( physicsWorld->getCollisionDispatcher() );

			// We need to register all modules we will be running multi-threaded with the job queue
			physicsWorld->registerWithJobQueue( jobQueue );
		}
}

void Physics::Simulate(float secTime){
	// Step the physics world. This single call steps using this thread and all threads
	// in the threadPool. For other products you add jobs, call process all jobs and wait for completion.
	// See the multithreading chapter in the user guide for details
	//physicsWorld->stepMultithreaded( jobQueue, threadPool, secTime );
	if(secTime>0.00001)// time of zero causes an exception 
		hkpStepResult hr= physicsWorld->stepDeltaTime  (  secTime  )  ; 
}

void Physics::TidyUp(void)
{
	if(physicsWorld != NULL)
	{
		physicsWorld->markForWrite();
		physicsWorld->removeReference();
	}
	delete jobQueue;
	//
	// Clean up the thread pool
	//
	threadPool->removeReference();

	#if defined HK_PLATFORM_PS3_PPU
		extern void quitSpurs( CellSpurs* spurs );
		quitSpurs( spurs );
	#endif
	
	hkBaseSystem::quit();
    hkMemoryInitUtil::quit();
}