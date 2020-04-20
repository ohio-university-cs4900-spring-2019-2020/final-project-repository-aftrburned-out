#include <PachinkoWOP.h>
#include "PxPhysicsAPI.h"
#include "PxRigidStatic.h"
#include "PxMaterial.h"
#include "PxActor.h"
#include "PxRigidStatic.h"
#include "PxShape.h"
#include "Model.h"
/*
using physx::PxMaterial;
using physx::PxShape;
using physx::PxTransform;
using physx::PxActor;
using physx::PxRigidActor;
using physx::PxRigidStatic;*/
using namespace physx;

PachinkoWOP* PachinkoWOP::New(physx::PxPhysics* p, physx::PxScene* s, const std::string& path, Aftr::Vector scale, Aftr::MESH_SHADING_TYPE mst, PxObj ty, physx::PxVec3 pos)
{
	PachinkoWOP* wo = new PachinkoWOP(p, s);
	wo->onCreate(path, scale, mst, ty, pos);
	return wo;
}

PachinkoWOP::PachinkoWOP(physx::PxPhysics* p, physx::PxScene* s) : IFace(this), WO()
{
	this->p = p;
	this->s = s;
	this->a = nullptr;
}

void PachinkoWOP::onCreate(const std::string& path, const Aftr::Vector& scale, Aftr::MESH_SHADING_TYPE mst, PxObj ty, physx::PxVec3& pos)
{
	WO::onCreate(path, scale, mst);
	PxMaterial* gMaterial = p->createMaterial(0.9f, 0.9f, 0.3f);
	PxShape* shape;
	PxTransform t(pos);

	switch (ty)
	{
	case Ball: 
			shape = p->createShape(PxSphereGeometry(1), *gMaterial, true);
			a = p->createRigidDynamic(t);
			break;
	case Peg:	// create box with contact and rest offsets to create a "lozenge" shape 
				// https://github.com/NVIDIAGameWorks/PhysX/issues/50#issuecomment-458509791
				// just use capsules for now, can't get the example to work
			shape = p->createShape(PxCapsuleGeometry(1, 2), *gMaterial, true);
			this->rotateAboutGlobalY(Aftr::DEGtoRAD * 90);
			//shape->setContactOffset(3.0f);
			//shape->setRestOffset(2.0f);
			a = p->createRigidStatic(t);
			break;
	case Floor: 
			shape = p->createShape(PxBoxGeometry(100, 100, 1), *gMaterial, true);
			a = p->createRigidStatic(t);
			break;
	default: 
			std::cerr << "Default case in PachinkoWOP OnCreate!" << std::endl;
			shape = p->createShape(PxBoxGeometry(1, 1, 1), *gMaterial, true);
			a = p->createRigidStatic(t);
			break;
	}
	//shape->setContactOffset(0.01f);

	a->attachShape(*shape);
	a->userData = this;
	this->s->addActor(*a);
}

PxActor* PachinkoWOP::getPxActor(physx::PxPhysics* p)
{
	PxMaterial* mat = p->createMaterial(.5f, .5f, .5f);
	PxRigidDynamic* aCapsuleActor = p->createRigidDynamic(PxTransform({ 0, 0, 10 }));
	PxTransform relativePose(PxQuat(PxHalfPi, { 0, 0, 1 }));
	PxReal pxr(2.0);
	PxShape* aCapsuleShape = PxRigidActorExt::createExclusiveShape(*aCapsuleActor, PxCapsuleGeometry(pxr, pxr), *mat);
	aCapsuleShape->setLocalPose(relativePose);
	PxRigidBodyExt::updateMassAndInertia(*aCapsuleActor, 1);

	return aCapsuleActor;
}

void PachinkoWOP::updatePoseFromPhysicsEngine(physx::PxActor* actor)
{
	PxRigidActor* ra = (PxRigidActor*)actor;
	PxTransform lt = ra->getGlobalPose();	//the position vector

	// position
	setPosition({ lt.p.x, lt.p.y, lt.p.z });

	// rotation
	PxMat44 m = PxMat44(lt);

	float info[16];

	for (int i = 0; i < 16; i++)
	{
		info[i] = m[i / 4][i % 4];
	}

	setRotation(info);
}

void PachinkoWOP::setPosition(Aftr::Vector v)
{
	WO* wo = this->getWO();
	wo->setPosition(v);
}

void PachinkoWOP::setRotation(const float* info)
{
	getModel()->setDisplayMatrix(info);
}
