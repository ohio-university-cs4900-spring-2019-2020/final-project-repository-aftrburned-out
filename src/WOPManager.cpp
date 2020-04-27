#include "WOPManager.h"
#include "PxRigidDynamic.h"

WOPManager::WOPManager()
{
	using namespace physx;
	f = PxCreateFoundation(PX_PHYSICS_VERSION, a, e);

	gPvd = PxCreatePvd(*f);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	p = PxCreateBasePhysics(PX_PHYSICS_VERSION, *f, PxTolerancesScale(), true, gPvd);
	PxSceneDesc s(p->getTolerancesScale());
	s.gravity = PxVec3(0.0f, 0.0f, -5.0f);
	s.flags = PxSceneFlag::eENABLE_ACTIVE_ACTORS;
	d = PxDefaultCpuDispatcherCreate(2);
	s.cpuDispatcher = d;
	s.filterShader = PxDefaultSimulationFilterShader;
	scene = p->createScene(s);

	FloorRot = { 0,0,0,1 };
	PegRot = { 0,0,0,1 };
	BoardRot = { 0,0,0,1 };

	srand(time(NULL)); //seed rand
}

void WOPManager::updatePhysics()
{
	scene->simulate(0.075);
	scene->fetchResults(true);
	{
		physx::PxU32 numActors = 0;

		physx::PxActor** actors = scene->getActiveActors(numActors);

		//poses that have changed since the last update
		for (physx::PxU32 i = 0; i < numActors; ++i)
		{
			physx::PxActor* actor = actors[i];
			PachinkoWOP* wo = static_cast<PachinkoWOP*>(actor->userData);
			wo->updatePoseFromPhysicsEngine(actor);
		}
	}
}

// This sets the WOPManager's initial Peg placement's Rotation 
void WOPManager::setPegRot(PxQuat in)
{
	PegRot = in;
}

// This sets the WOPManager's initial Floor placement's Rotation 
void WOPManager::setFloorRot(PxQuat in)
{
	FloorRot = in;
}

// This sets the WOPManager's initial Board placement's Rotation 
void WOPManager::setBoardRot(PxQuat in)
{
	BoardRot = in;
}

// This sets the WOPManager's initial Plane placement's Rotation 
void WOPManager::setPlaneRot(PxQuat in)
{
	PlaneRot = in;
}

// This creates a Pachinko Peg at the given postion.
// Remember to use setPegRot() before using this to give it a rotation.
WO* WOPManager::createPachinkoPeg(Vector pos)
{
	PachinkoWOP* wo = PachinkoWOP::New(p, scene, (ManagerEnvironmentConfiguration::getSMM() + "/models/WOTeapot/cylinder/cylinder.3ds"), { .25, .25, .5 }, MESH_SHADING_TYPE::mstFLAT, PachinkoWOP::PxObj::Peg, { pos.x, pos.y, pos.z }, PegRot);
	wo->setPosition(pos);
	return wo;
}

WO* WOPManager::createPlane(Vector pos)
{
	float rx, ry, rz = 0;
	rx = pos.x;
	ry = pos.y;
	rz = pos.z;
	return PachinkoWOP::New(p, scene, (ManagerEnvironmentConfiguration::getSMM() + "/models/grassFloor400x400_pp.wrl"), Vector(0, 0, 0), MESH_SHADING_TYPE::mstFLAT, PachinkoWOP::PxObj::Plane, physx::PxVec3{ rx, ry, rz }, PlaneRot);
}

// Do not use this, it is only to test
WO* WOPManager::__createPachinkoBall()
{
	float rx, ry = 0;
	rx = 0;//rand() % 5;
	ry = 10 + rand() % 25;
	
	return createPachinkoBall({ rx, ry, 75.0 });

}

// Creates a Pachinko Ball at the position specified
WO* WOPManager::createPachinkoBall(Vector pos)
{
	float rx, ry, rz = 0;
	rx = pos.x;
	ry = pos.y;
	rz = pos.z;
	PachinkoWOP* wo = PachinkoWOP::New(p, scene, (ManagerEnvironmentConfiguration::getSMM() + "/models/dice_twelveside_outline.wrl"), { 1, 1, 1 }, Aftr::MESH_SHADING_TYPE::mstAUTO, PachinkoWOP::PxObj::Ball, physx::PxVec3{ rx, ry, rz }, PegRot);

	wo->setPosition({ rx, ry, 25 });
	return wo;
}

// Creates the floor.
// Remember to use setPlaneRot() before using this to give it a rotation.
WO* WOPManager::createFloor(std::string path)
{
	return PachinkoWOP::New(p, scene, path, Vector(1, 1, 1), MESH_SHADING_TYPE::mstFLAT, PachinkoWOP::PxObj::Plane, physx::PxVec3{ 0, 0, 0 }, FloorRot);
}

void WOPManager::setBallBounds(float min, float max, float height)
{
	this->min = min;
	this->max = max;
	this->height = height;
}

// Creates the board at the given positon with the given model.
// Remember to use setPlaneRot() before using this to give it a rotation.
WO* WOPManager::createBoard(Vector pos, std::string path)
{
	float rx, ry, rz = 0;
	rx = pos.x;
	ry = pos.y;
	rz = pos.z;
	return PachinkoWOP::New(p, scene, path, Vector(1, 1, 1), MESH_SHADING_TYPE::mstFLAT, PachinkoWOP::PxObj::Plane, physx::PxVec3{ rx, ry, rz }, BoardRot);
}
