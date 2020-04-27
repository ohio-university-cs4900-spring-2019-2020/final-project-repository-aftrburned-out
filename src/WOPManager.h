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
	/*This sets the WOPManager's initial Peg placement's Rotation*/
	void setPegRot(PxQuat in);
	/*This sets the WOPManager's initial Floor placement's Rotation*/
	void setFloorRot(PxQuat in);
	/*This sets the WOPManager's initial Board placement's Rotation*/
	void setBoardRot(PxQuat in);
	/*This sets the WOPManager's initial Plane placement's Rotation*/
	void setPlaneRot(PxQuat in);
	/*This creates a Floor at the given postion.
	Remember to use setFloorRot() before using this to give it a rotation.*/
	WO* createFloor(std::string path);
	/*This creates a Board at the given postion.
	Remember to use setBoardRot() before using this to give it a rotation.*/
	WO* createBoard(Vector pos, std::string path);
	/*This creates a Plane at the given postion.
	Remember to use setPlaneRot() before using this to give it a rotation.*/
	WO* createPlane(Vector pos);
	/*DO NOT USE*/
	WO* __createPachinkoBall();
	/*Creates a Pachinko Ball at the position specified*/
	WO* createPachinkoBall(Vector pos);
	/*This creates a Pachinko Peg at the given postion.
	Remember to use setPegRot() before using this to give it a rotation.*/
	WO* createPachinkoPeg(Vector pos);
	/*This sets the min and max y values that the Pachinko ball can spawn*/
	void setMinMaxBallPos(float min, float max);
protected:
	PxDefaultAllocator a;
	PxDefaultErrorCallback e;
	PxFoundation* f;// = PxCreateFoundation(PX_PHYSICS_VERSION, a, e);
	PxPhysics* p;// = PxCreatePhysics(PX_PHYSICS_VERSION, *f, PxTolerancesScale(), true, NULL);
	PxScene* scene;
	PxPvd* gPvd;
	PxDefaultCpuDispatcher* d;
	PxQuat PegRot;
	PxQuat FloorRot;
	PxQuat BoardRot;
	PxQuat PlaneRot;
	float max;
	float min;
	//PxCooking* cook;
};