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

void WOPManager::setPegRot(PxQuat in)
{
	PegRot = in;
}

void WOPManager::setFloorRot(PxQuat in)
{
	FloorRot = in;
}

void WOPManager::setBoardRot(PxQuat in)
{
	BoardRot = in;
}

WO* WOPManager::createPachinkoPeg(Vector pos)
{
	PachinkoWOP* wo = PachinkoWOP::New(p, scene, (ManagerEnvironmentConfiguration::getSMM() + "/models/WOTeapot/cylinder/cylinder.3ds"), { .25, .25, .5 }, MESH_SHADING_TYPE::mstFLAT, PachinkoWOP::PxObj::Peg, { pos.x, pos.y, pos.z }, PegRot);
	wo->setPosition(pos);
	return wo;
}

WO* WOPManager::__createPachinkoBall()
{
	float rx, ry = 0;
	rx = 0;//rand() % 5;
	ry = 10 + rand() % 5;
	
	return createPachinkoBall({ rx, ry, 50.0 });

}

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

WO* WOPManager::createFloor(std::string path)
{
	return PachinkoWOP::New(p, scene, path, Vector(1, 1, 1), MESH_SHADING_TYPE::mstFLAT, PachinkoWOP::PxObj::Plane, physx::PxVec3{ 0, 0, 0 }, FloorRot);
}

WO* WOPManager::createBoard(Vector pos, std::string path)
{
	float rx, ry, rz = 0;
	rx = pos.x;
	ry = pos.y;
	rz = pos.z;
	return PachinkoWOP::New(p, scene, path, Vector(1, 1, 1), MESH_SHADING_TYPE::mstFLAT, PachinkoWOP::PxObj::Plane, physx::PxVec3{ rx, ry, rz }, BoardRot);
}
