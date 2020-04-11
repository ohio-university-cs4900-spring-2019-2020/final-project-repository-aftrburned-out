#include <PachinkoWOP.h>
#include "PxPhysicsAPI.h"
#include "PxRigidStatic.h"
#include "PxMaterial.h"
#include "PxActor.h"
#include "PxRigidStatic.h"
#include "PxShape.h"
/*
using physx::PxMaterial;
using physx::PxShape;
using physx::PxTransform;
using physx::PxActor;
using physx::PxRigidActor;
using physx::PxRigidStatic;*/
using namespace physx;

PachinkoWOP* PachinkoWOP::New(physx::PxPhysics* p, physx::PxScene* s, const std::string& path, Aftr::Vector scale, Aftr::MESH_SHADING_TYPE mst, bool stat, physx::PxVec3 pos)
{
	PachinkoWOP* wo = new PachinkoWOP(p, s);
	wo->onCreate(path, scale, mst, stat, pos);
	return wo;
}

PachinkoWOP::PachinkoWOP(physx::PxPhysics* p, physx::PxScene* s) : IFace(this), WO()
{
	this->p = p;
	this->s = s;
	this->a = nullptr;
}

void PachinkoWOP::onCreate(const std::string& path, const Aftr::Vector& scale, Aftr::MESH_SHADING_TYPE mst, bool stat, physx::PxVec3& pos)
{
	WO::onCreate(path, scale, mst);
	PxMaterial* gMaterial = p->createMaterial(0.9f, 0.9f, 0.3f);
	PxShape* shape;
	PxTransform t(pos);
	if (stat)
	{
		shape = p->createShape(PxBoxGeometry(100, 100, 1), *gMaterial, true);
		a = p->createRigidStatic(t);
	}
	else
	{
		shape = p->createShape(PxSphereGeometry(1), *gMaterial, true);
		a = p->createRigidDynamic(t);
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
	setPosition({ lt.p.x, lt.p.y, lt.p.z });
}

void PachinkoWOP::setPosition(Aftr::Vector v)
{
	WO* wo = this->getWO();
	wo->setPosition(v);	//find out why 'this' was nullptr
}
