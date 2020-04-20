#include "PxPhysicsAPI.h"
#include "extensions/PxDefaultAllocator.h"
#include "extensions/PxDefaultErrorCallback.h"
#include "PxFoundation.h"

#include "PachinkoWOP.h"

using namespace physx;
using namespace Aftr;

class WOPManager
{
public:
	WOPManager();
	void updatePhysics();
	WO* createFloor(std::string path);
	WO* __createPachinkoBall();	// do not use
	WO* createPachinkoBall(Vector pos);
	WO* createPachinkoPeg(Vector pos);
protected:
	PxDefaultAllocator a;
	PxDefaultErrorCallback e;
	PxFoundation* f;// = PxCreateFoundation(PX_PHYSICS_VERSION, a, e);
	PxPhysics* p;// = PxCreatePhysics(PX_PHYSICS_VERSION, *f, PxTolerancesScale(), true, NULL);
	PxScene* scene;
	PxPvd* gPvd;
	PxDefaultCpuDispatcher* d;
	//PxCooking* cook;
};