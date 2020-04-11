#include "GLViewPachinko.h"

#include "WorldList.h" //This is where we place all of our WOs
#include "ManagerOpenGLState.h" //We can change OpenGL State attributes with this
#include "Axes.h" //We can set Axes to on/off with this
#include "PhysicsEngineODE.h"

//Different WO used by this module
#include "WO.h"
#include "WOStatic.h"
#include "WOStaticPlane.h"
#include "WOStaticTrimesh.h"
#include "WOTrimesh.h"
#include "WOHumanCyborg.h"
#include "WOHumanCal3DPaladin.h"
#include "WOWayPointSpherical.h"
#include "WOLight.h"
#include "WOSkyBox.h"
#include "WOCar1970sBeater.h"
#include "Camera.h"
#include "CameraStandard.h"
#include "CameraChaseActorSmooth.h"
#include "CameraChaseActorAbsNormal.h"
#include "CameraChaseActorRelNormal.h"
#include "Model.h"
#include "ModelDataShared.h"
#include "ModelMesh.h"
#include "ModelMeshDataShared.h"
#include "ModelMeshSkin.h"
#include "WONVStaticPlane.h"
#include "WONVPhysX.h"
#include "WONVDynSphere.h"
#include "AftrGLRendererBase.h"

//If we want to use way points, we need to include this.
#include "PachinkoWayPoints.h"
#include "PachinkoWOP.h"

using namespace Aftr;

GLViewPachinko* GLViewPachinko::New( const std::vector< std::string >& args )
{
   GLViewPachinko* glv = new GLViewPachinko( args );
   glv->init( Aftr::GRAVITY, Vector( 0, 0, -1.0f ), "aftr.conf", PHYSICS_ENGINE_TYPE::petODE );
   glv->onCreate();
   return glv;
}


GLViewPachinko::GLViewPachinko( const std::vector< std::string >& args ) : GLView( args )
{
   //Initialize any member variables that need to be used inside of LoadMap() here.
   //Note: At this point, the Managers are not yet initialized. The Engine initialization
   //occurs immediately after this method returns (see GLViewPachinko::New() for
   //reference). Then the engine invoke's GLView::loadMap() for this module.
   //After loadMap() returns, GLView::onCreate is finally invoked.

   //The order of execution of a module startup:
   //GLView::New() is invoked:
   //    calls GLView::init()
   //       calls GLView::loadMap() (as well as initializing the engine's Managers)
   //    calls GLView::onCreate()

   //GLViewPachinko::onCreate() is invoked after this module's LoadMap() is completed.
}


void GLViewPachinko::onCreate()
{
   //GLViewPachinko::onCreate() is invoked after this module's LoadMap() is completed.
   //At this point, all the managers are initialized. That is, the engine is fully initialized.

   if( this->pe != NULL )
   {
      //optionally, change gravity direction and magnitude here
      //The user could load these values from the module's aftr.conf
      this->pe->setGravityNormalizedVector( Vector( 0,0,-1.0f ) );
      this->pe->setGravityScalar( Aftr::GRAVITY );
   }
   this->setActorChaseType( STANDARDEZNAV ); //Default is STANDARDEZNAV mode
   //this->setNumPhysicsStepsPerRender( 0 ); //pause physics engine on start up; will remain paused till set to 1
}

void GLViewPachinko::updatePhysics()
{
	scene->simulate(0.075);
	scene->fetchResults(true);
	{

		physx::PxU32 numActors = 0;

		physx::PxActor** actors = scene->getActiveActors(numActors);

		//make sure you set physx::PxSceneFlag::eENABLE_ACTIVE_ACTORS, true in your scene!

		//poses that have changed since the last update
		for (physx::PxU32 i = 0; i < numActors; ++i)

		{

			physx::PxActor* actor = actors[i];

			PachinkoWOP* wo = static_cast<PachinkoWOP*>(actor->userData);

			wo->updatePoseFromPhysicsEngine(actor);		//add this function to your inherited class
			//worldLst->push_back(wo);

		}

	}
}


GLViewPachinko::~GLViewPachinko()
{
   //Implicitly calls GLView::~GLView()
	if (scene != nullptr)
		scene->release();
	if (f != nullptr)
		f->release();
	if (p != nullptr)
		p->release();
}


void GLViewPachinko::updateWorld()
{
   GLView::updateWorld(); //Just call the parent's update world first.
                          //If you want to add additional functionality, do it after
                          //this call.

   updatePhysics();

}


void GLViewPachinko::onResizeWindow( GLsizei width, GLsizei height )
{
   GLView::onResizeWindow( width, height ); //call parent's resize method.
}


void GLViewPachinko::onMouseDown( const SDL_MouseButtonEvent& e )
{
   GLView::onMouseDown( e );
}


void GLViewPachinko::onMouseUp( const SDL_MouseButtonEvent& e )
{
   GLView::onMouseUp( e );
}


void GLViewPachinko::onMouseMove( const SDL_MouseMotionEvent& e )
{
   GLView::onMouseMove( e );
}


void GLViewPachinko::onKeyDown( const SDL_KeyboardEvent& key )
{
   GLView::onKeyDown( key );
   if( key.keysym.sym == SDLK_0 )
      this->setNumPhysicsStepsPerRender( 1 );

   if( key.keysym.sym == SDLK_1 )
   {	//temp
	   float rx, ry = 0;
	   srand(r);
	   rx = rand() % 5;
	   ry = rand() % 5;

	   PachinkoWOP* wo = PachinkoWOP::New(p, scene, (ManagerEnvironmentConfiguration::getSMM() + "/models/dice_twelveside_outline.wrl"), { 1, 1, 1 }, Aftr::MESH_SHADING_TYPE::mstAUTO, false, physx::PxVec3{ rx, ry, 25 });

	   wo->setPosition({ rx, ry, 25 });
	   worldLst->push_back(wo);
	   if (scene != nullptr)
	   {
		   r++;
		   std::cout << "added shape" << std::endl;
	   }
	   else
	   {
		   std::cout << "scene is null" << std::endl;
	   }
   }
}


void GLViewPachinko::onKeyUp( const SDL_KeyboardEvent& key )
{
   GLView::onKeyUp( key );
}


void Aftr::GLViewPachinko::loadMap()
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


   this->worldLst = new WorldList(); //WorldList is a 'smart' vector that is used to store WO*'s
   this->actorLst = new WorldList();
   this->netLst = new WorldList();

   ManagerOpenGLState::GL_CLIPPING_PLANE = 1000.0;
   ManagerOpenGLState::GL_NEAR_PLANE = 0.1f;
   ManagerOpenGLState::enableFrustumCulling = false;
   Axes::isVisible = true;
   this->glRenderer->isUsingShadowMapping( false ); //set to TRUE to enable shadow mapping, must be using GL 3.2+

   this->cam->setPosition( 15,15,10 );

   std::string shinyRedPlasticCube( ManagerEnvironmentConfiguration::getSMM() + "/models/cube4x4x4redShinyPlastic_pp.wrl" );
   std::string wheeledCar( ManagerEnvironmentConfiguration::getSMM() + "/models/rcx_treads.wrl" );
   std::string grass( ManagerEnvironmentConfiguration::getSMM() + "/models/grassFloor400x400_pp.wrl" );
   std::string human( ManagerEnvironmentConfiguration::getSMM() + "/models/human_chest.wrl" );
   
   //SkyBox Textures readily available
   std::vector< std::string > skyBoxImageNames; //vector to store texture paths
   skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_Hubble_Nebula+6.jpg" );
  

   float ga = 0.1f; //Global Ambient Light level for this module
   ManagerLight::setGlobalAmbientLight( aftrColor4f( ga, ga, ga, 1.0f ) );
   WOLight* light = WOLight::New();
   light->isDirectionalLight( true );
   light->setPosition( Vector( 0, 0, 100 ) );
   //Set the light's display matrix such that it casts light in a direction parallel to the -z axis (ie, downwards as though it was "high noon")
   //for shadow mapping to work, this->glRenderer->isUsingShadowMapping( true ), must be invoked.
   light->getModel()->setDisplayMatrix( Mat4::rotateIdentityMat( { 0, 1, 0 }, 90.0f * Aftr::DEGtoRAD ) );
   light->setLabel( "Light" );
   worldLst->push_back( light );

   //Create the SkyBox
   WO* wo = WOSkyBox::New( skyBoxImageNames.at( 0 ), this->getCameraPtrPtr() );
   wo->setPosition( Vector( 0,0,0 ) );
   wo->setLabel( "Sky Box" );
   wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
   worldLst->push_back( wo );

  

   ////Create the infinite grass plane that uses NVIDIAPhysX(the floor)
   wo = PachinkoWOP::New(p, scene, grass, Vector(1,1,1), MESH_SHADING_TYPE::mstFLAT, true, physx::PxVec3{ 0, 0, 0 });
   wo->setPosition( Vector(0,0,0) );
   wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
   wo->getModel()->getModelDataShared()->getModelMeshes().at(0)->getSkins().at(0).getMultiTextureSet().at(0)->setTextureRepeats( 5.0f );
   wo->setLabel( "Grass" );
   worldLst->push_back( wo );
   //createPachinkoWayPoints();
}


void GLViewPachinko::createPachinkoWayPoints()
{
   // Create a waypoint with a radius of 3, a frequency of 5 seconds, activated by GLView's camera, and is visible.
   WayPointParametersBase params(this);
   params.frequency = 5000;
   params.useCamera = true;
   params.visible = true;
   WOWayPointSpherical* wayPt = WOWP1::New( params, 3 );
   wayPt->setPosition( Vector( 50, 0, 3 ) );
   worldLst->push_back( wayPt );
}
