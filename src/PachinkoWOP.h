#pragma once

#include <WO.h>
#include "PxActor.h"

using physx::PxActor;

class PachinkoWOP : public Aftr::WO
{
public:
	//WOMacroDeclaration(PachinkoWOP, WO);
	// enum to pass to PackingoWOP::New allowing the user to create a specific type of WOP;
	enum PxObj { Plane, Ball, Peg };
	static PachinkoWOP* New(physx::PxPhysics* p, physx::PxScene* s, const std::string& path, Aftr::Vector scale, Aftr::MESH_SHADING_TYPE mst, PxObj ty, physx::PxVec3 pos, physx::PxQuat& rot);
	static PxActor* getPxActor(physx::PxPhysics* p);
	//static PachinkoWOP* createPeg();
	//static void __setPx(physx::PxPhysics* p, physx::PxScene* s);
	virtual void setPosition(Aftr::Vector v);
	void setRotation(const float* info);
	void updatePoseFromPhysicsEngine(physx::PxActor* actor);
	PxObj t;
protected:
	PachinkoWOP(physx::PxPhysics* p, physx::PxScene* s);
	void onCreate(const std::string& path, const Aftr::Vector& scale, Aftr::MESH_SHADING_TYPE mst, PxObj ty, physx::PxVec3& pos, physx::PxQuat& rot);
private:
	physx::PxPhysics* p;
	physx::PxScene* s;
	physx::PxRigidActor* a;
	physx::PxShape* shape;
	
	//PachinkoWOP* t = this;
};