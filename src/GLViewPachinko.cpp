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

GLViewPachinko::~GLViewPachinko()
{

}

void GLViewPachinko::updateWorld()
{
   GLView::updateWorld(); //Just call the parent's update world first.
                          //If you want to add additional functionality, do it after
                          //this call.

   wm->updatePhysics();

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
   {	
	   worldLst->push_back(wm->__createPachinkoBall());
   }
   if (key.keysym.sym == SDLK_2)
   {
	   worldLst->push_back(wm->createPachinkoPeg({ 0, 0, 5 }));
   }
}

void GLViewPachinko::onKeyUp( const SDL_KeyboardEvent& key )
{
   GLView::onKeyUp( key );
}

std::vector<Vector> presetOne(WOPManager* wm){
	std::vector<Vector> list;

	//row 1
	list.push_back(Vector(0, 0, 5));
	list.push_back(Vector(0, 5, 5));
	list.push_back(Vector(0, 10, 5));
	list.push_back(Vector(0, 15, 5));
	list.push_back(Vector(0, 20, 5));

	//row 2 offset
	list.push_back(Vector(0, 2.5, 10));
	list.push_back(Vector(0, 7.5, 10));
	list.push_back(Vector(0, 12.5, 10));
	list.push_back(Vector(0, 17.5, 10));

	//row 3
	list.push_back(Vector(0, 0, 15));
	list.push_back(Vector(0, 5, 15));
	list.push_back(Vector(0, 10, 15));
	list.push_back(Vector(0, 15, 15));
	list.push_back(Vector(0, 20, 15));

	//row 4 offset
	list.push_back(Vector(0, 2.5, 20));
	list.push_back(Vector(0, 7.5, 20));
	list.push_back(Vector(0, 12.5, 20));
	list.push_back(Vector(0, 17.5, 20));

	//row 5 
	list.push_back(Vector(0, 0, 25));
	list.push_back(Vector(0, 5, 25));
	list.push_back(Vector(0, 10, 25));
	list.push_back(Vector(0, 15, 25));
	list.push_back(Vector(0, 20, 25));

	//row 6 offset
	list.push_back(Vector(0, 2.5, 30));
	list.push_back(Vector(0, 7.5, 30));
	list.push_back(Vector(0, 12.5, 30));
	list.push_back(Vector(0, 17.5, 30));

	//row 7
	list.push_back(Vector(0, 0, 35));
	list.push_back(Vector(0, 5, 35));
	list.push_back(Vector(0, 10, 35));
	list.push_back(Vector(0, 15, 35));
	list.push_back(Vector(0, 20, 35));

	wm->setBallBounds(0, 20, 45);

	return list;
}

std::vector<Vector> presetTwo(WOPManager* wm){
	std::vector<Vector> list;

	//row 1
	list.push_back(Vector(0, 0, 5));
	list.push_back(Vector(0, 5, 5));
	list.push_back(Vector(0, 10, 5));
	list.push_back(Vector(0, 15, 5));
	list.push_back(Vector(0, 20, 5));
	list.push_back(Vector(0, 25, 5));
	list.push_back(Vector(0, 30, 5));
	list.push_back(Vector(0, 35, 5));

	//row 2 offset
	list.push_back(Vector(0, 2.5, 10));
	list.push_back(Vector(0, 7.5, 10));
	list.push_back(Vector(0, 12.5, 10));
	list.push_back(Vector(0, 17.5, 10));
	list.push_back(Vector(0, 22.5, 10));
	list.push_back(Vector(0, 27.5, 10));
	list.push_back(Vector(0, 32.5, 10));

	//row 3
	list.push_back(Vector(0, 0, 15));
	list.push_back(Vector(0, 5, 15));
	list.push_back(Vector(0, 10, 15));
	list.push_back(Vector(0, 15, 15));
	list.push_back(Vector(0, 20, 15));
	list.push_back(Vector(0, 25, 15));
	list.push_back(Vector(0, 30, 15));
	list.push_back(Vector(0, 35, 15));

	//row 4 offset
	list.push_back(Vector(0, 2.5, 20));
	list.push_back(Vector(0, 7.5, 20));
	list.push_back(Vector(0, 12.5, 20));
	list.push_back(Vector(0, 17.5, 20));
	list.push_back(Vector(0, 22.5, 20));
	list.push_back(Vector(0, 27.5, 20));
	list.push_back(Vector(0, 32.5, 20));

	//row 5
	list.push_back(Vector(0, 0, 25));
	list.push_back(Vector(0, 5, 25));
	list.push_back(Vector(0, 10, 25));
	list.push_back(Vector(0, 15, 25));
	list.push_back(Vector(0, 20, 25));
	list.push_back(Vector(0, 25, 25));
	list.push_back(Vector(0, 30, 25));
	list.push_back(Vector(0, 35, 25));

	//row 6 offset
	list.push_back(Vector(0, 2.5, 30));
	list.push_back(Vector(0, 7.5, 30));
	list.push_back(Vector(0, 12.5, 30));
	list.push_back(Vector(0, 17.5, 30));
	list.push_back(Vector(0, 22.5, 30));
	list.push_back(Vector(0, 27.5, 30));
	list.push_back(Vector(0, 32.5, 30));

	//row 7
	list.push_back(Vector(0, 0, 35));
	list.push_back(Vector(0, 5, 35));
	list.push_back(Vector(0, 10, 35));
	list.push_back(Vector(0, 15, 35));
	list.push_back(Vector(0, 20, 35));
	list.push_back(Vector(0, 25, 35));
	list.push_back(Vector(0, 30, 35));
	list.push_back(Vector(0, 35, 35));

	//row 8 offset
	list.push_back(Vector(0, 2.5, 40));
	list.push_back(Vector(0, 7.5, 40));
	list.push_back(Vector(0, 12.5, 40));
	list.push_back(Vector(0, 17.5, 40));
	list.push_back(Vector(0, 22.5, 40));
	list.push_back(Vector(0, 27.5, 40));
	list.push_back(Vector(0, 32.5, 40));

	//row 9
	list.push_back(Vector(0, 0, 45));
	list.push_back(Vector(0, 5, 45));
	list.push_back(Vector(0, 10, 45));
	list.push_back(Vector(0, 15, 45));
	list.push_back(Vector(0, 20, 45));
	list.push_back(Vector(0, 25, 45));
	list.push_back(Vector(0, 30, 45));
	list.push_back(Vector(0, 35, 45));

	wm->setBallBounds(0, 35, 55);

	return list;
}

std::vector<Vector> presetThree(WOPManager* wm){
	std::vector<Vector> list;

	//row 1
	list.push_back(Vector(0, 0, 5));
	list.push_back(Vector(0, 5, 5));
	list.push_back(Vector(0, 10, 5));
	list.push_back(Vector(0, 15, 5));
	list.push_back(Vector(0, 20, 5));
	list.push_back(Vector(0, 25, 5));
	list.push_back(Vector(0, 30, 5));
	list.push_back(Vector(0, 35, 5));
	list.push_back(Vector(0, 40, 5));
	list.push_back(Vector(0, 45, 5));
	list.push_back(Vector(0, 50, 5));
	list.push_back(Vector(0, 55, 5));

	//row 2 offset
	list.push_back(Vector(0, 2.5, 10));
	list.push_back(Vector(0, 7.5, 10));
	list.push_back(Vector(0, 12.5, 10));
	list.push_back(Vector(0, 17.5, 10));
	list.push_back(Vector(0, 22.5, 10));
	list.push_back(Vector(0, 27.5, 10));
	list.push_back(Vector(0, 32.5, 10));
	list.push_back(Vector(0, 37.5, 10));
	list.push_back(Vector(0, 42.5, 10));
	list.push_back(Vector(0, 47.5, 10));
	list.push_back(Vector(0, 52.5, 10));

	//row 3
	list.push_back(Vector(0, 0, 15));
	list.push_back(Vector(0, 5, 15));
	list.push_back(Vector(0, 10, 15));
	list.push_back(Vector(0, 15, 15));
	list.push_back(Vector(0, 20, 15));
	list.push_back(Vector(0, 25, 15));
	list.push_back(Vector(0, 30, 15));
	list.push_back(Vector(0, 35, 15));
	list.push_back(Vector(0, 40, 15));
	list.push_back(Vector(0, 45, 15));
	list.push_back(Vector(0, 50, 15));
	list.push_back(Vector(0, 55, 15));

	//row 4 offset
	list.push_back(Vector(0, 2.5, 20));
	list.push_back(Vector(0, 7.5, 20));
	list.push_back(Vector(0, 12.5, 20));
	list.push_back(Vector(0, 17.5, 20));
	list.push_back(Vector(0, 22.5, 20));
	list.push_back(Vector(0, 27.5, 20));
	list.push_back(Vector(0, 32.5, 20));
	list.push_back(Vector(0, 37.5, 20));
	list.push_back(Vector(0, 42.5, 20));
	list.push_back(Vector(0, 47.5, 20));
	list.push_back(Vector(0, 52.5, 20));

	//row 5 
	list.push_back(Vector(0, 0, 25));
	list.push_back(Vector(0, 5, 25));
	list.push_back(Vector(0, 10, 25));
	list.push_back(Vector(0, 15, 25));
	list.push_back(Vector(0, 20, 25));
	list.push_back(Vector(0, 25, 25));
	list.push_back(Vector(0, 30, 25));
	list.push_back(Vector(0, 35, 25));
	list.push_back(Vector(0, 40, 25));
	list.push_back(Vector(0, 45, 25));
	list.push_back(Vector(0, 50, 25));
	list.push_back(Vector(0, 55, 25));

	//row 6 offset
	list.push_back(Vector(0, 2.5, 30));
	list.push_back(Vector(0, 7.5, 30));
	list.push_back(Vector(0, 12.5, 30));
	list.push_back(Vector(0, 17.5, 30));
	list.push_back(Vector(0, 22.5, 30));
	list.push_back(Vector(0, 27.5, 30));
	list.push_back(Vector(0, 32.5, 30));
	list.push_back(Vector(0, 37.5, 30));
	list.push_back(Vector(0, 42.5, 30));
	list.push_back(Vector(0, 47.5, 30));
	list.push_back(Vector(0, 52.5, 30));

	//row 7
	list.push_back(Vector(0, 0, 35));
	list.push_back(Vector(0, 5, 35));
	list.push_back(Vector(0, 10, 35));
	list.push_back(Vector(0, 15, 35));
	list.push_back(Vector(0, 20, 35));
	list.push_back(Vector(0, 25, 35));
	list.push_back(Vector(0, 30, 35));
	list.push_back(Vector(0, 35, 35));
	list.push_back(Vector(0, 40, 35));
	list.push_back(Vector(0, 45, 35));
	list.push_back(Vector(0, 50, 35));
	list.push_back(Vector(0, 55, 35));

	//row 8 offset
	list.push_back(Vector(0, 2.5, 40));
	list.push_back(Vector(0, 7.5, 40));
	list.push_back(Vector(0, 12.5, 40));
	list.push_back(Vector(0, 17.5, 40));
	list.push_back(Vector(0, 22.5, 40));
	list.push_back(Vector(0, 27.5, 40));
	list.push_back(Vector(0, 32.5, 40));
	list.push_back(Vector(0, 37.5, 40));
	list.push_back(Vector(0, 42.5, 40));
	list.push_back(Vector(0, 47.5, 40));
	list.push_back(Vector(0, 52.5, 40));

	//row 9
	list.push_back(Vector(0, 0, 45));
	list.push_back(Vector(0, 5, 45));
	list.push_back(Vector(0, 10, 45));
	list.push_back(Vector(0, 15, 45));
	list.push_back(Vector(0, 20, 45));
	list.push_back(Vector(0, 25, 45));
	list.push_back(Vector(0, 30, 45));
	list.push_back(Vector(0, 35, 45));
	list.push_back(Vector(0, 40, 45));
	list.push_back(Vector(0, 45, 45));
	list.push_back(Vector(0, 50, 45));
	list.push_back(Vector(0, 55, 45));

	//row 10 offset
	list.push_back(Vector(0, 2.5, 50));
	list.push_back(Vector(0, 7.5, 50));
	list.push_back(Vector(0, 12.5, 50));
	list.push_back(Vector(0, 17.5, 50));
	list.push_back(Vector(0, 22.5, 50));
	list.push_back(Vector(0, 27.5, 50));
	list.push_back(Vector(0, 32.5, 50));
	list.push_back(Vector(0, 37.5, 50));
	list.push_back(Vector(0, 42.5, 50));
	list.push_back(Vector(0, 47.5, 50));
	list.push_back(Vector(0, 52.5, 50));

	//row 11
	list.push_back(Vector(0, 0, 55));
	list.push_back(Vector(0, 5, 55));
	list.push_back(Vector(0, 10, 55));
	list.push_back(Vector(0, 15, 55));
	list.push_back(Vector(0, 20, 55));
	list.push_back(Vector(0, 25, 55));
	list.push_back(Vector(0, 30, 55));
	list.push_back(Vector(0, 35, 55));
	list.push_back(Vector(0, 40, 55));
	list.push_back(Vector(0, 45, 55));
	list.push_back(Vector(0, 50, 55));
	list.push_back(Vector(0, 55, 55));

	//row 12 offset
	list.push_back(Vector(0, 2.5, 60));
	list.push_back(Vector(0, 7.5, 60));
	list.push_back(Vector(0, 12.5, 60));
	list.push_back(Vector(0, 17.5, 60));
	list.push_back(Vector(0, 22.5, 60));
	list.push_back(Vector(0, 27.5, 60));
	list.push_back(Vector(0, 32.5, 60));
	list.push_back(Vector(0, 37.5, 60));
	list.push_back(Vector(0, 42.5, 60));
	list.push_back(Vector(0, 47.5, 60));
	list.push_back(Vector(0, 52.5, 60));

	//row 13
	list.push_back(Vector(0, 0, 65));
	list.push_back(Vector(0, 5, 65));
	list.push_back(Vector(0, 10, 65));
	list.push_back(Vector(0, 15, 65));
	list.push_back(Vector(0, 20, 65));
	list.push_back(Vector(0, 25, 65));
	list.push_back(Vector(0, 30, 65));
	list.push_back(Vector(0, 35, 65));
	list.push_back(Vector(0, 40, 65));
	list.push_back(Vector(0, 45, 65));
	list.push_back(Vector(0, 50, 65));
	list.push_back(Vector(0, 55, 65));

	wm->setBallBounds(0, 55, 75);

	return list;

}

void Aftr::GLViewPachinko::loadMap()
{
	// create the WOP Manager
	wm = new WOPManager();
	
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
   wm->setPlaneRot({ 0, 0, 0, 1 });
   wo = wm->createPlane({ 0, 0, 0 });
   //PachinkoWOP::New(p, scene, grass, Vector(1,1,1), MESH_SHADING_TYPE::mstFLAT, PachinkoWOP::PxObj::Floor, physx::PxVec3{ 0, 0, 0 });
   wo->setPosition( Vector(0,0,0) );
   wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
   wo->getModel()->getModelDataShared()->getModelMeshes().at(0)->getSkins().at(0).getMultiTextureSet().at(0)->setTextureRepeats( 5.0f );
   wo->setLabel( "Grass" );
   worldLst->push_back( wo );
   //createPachinkoWayPoints();

   //
   // example of an invisible wall that sits behind the pegs
   // wm->setPlaneRot({ 0, 0.7071068, 0, 0.7071068 });
   // wo = wm->createPlane({ -3.5, 0, 0 });
   // worldLst->push_back(wo);
   // wm->setPlaneRot({ 0.7071068, 0, 0, 0.7071068 });
   // wo = wm->createPlane({ 0, -5, 0 });
   // worldLst->push_back(wo);
   //


   std::vector<Vector> list = presetThree(wm);

   // to get the rotation angles, use this website:
   // https://www.andre-gaschler.com/rotationconverter/
   // and use the "Euler angles of multiple axis rotations" input
   wm->setPegRot({ 0.0864101, -0.1300295, -0.0113761, 0.9876721 });

   for (int i = 0; i < list.size(); i++)
   {
	   worldLst->push_back(wm->createPachinkoPeg(list[i]));
   }

   //worldLst->push_back(wm->createBoard({ 50, 50, 50 }, grass));//(ManagerEnvironmentConfiguration::getSMM() + "images/DefaultTexture.jpg")));

   setBoardState();

}


/*
	This will set all board surrounding pieces that will hold the balls in. Front, back, and both sides
*/
void GLViewPachinko::setBoardState() {

	//style for the background board
	std::string board(ManagerEnvironmentConfiguration::getLMM() + "/models/finally.obj");

	//this is the background board
	wm->setBoardRot({ 0, 0.7071068, 0, 0.7071068 });
	WO* wo = wm->createBoard(Vector(-3.5, 0, 0), board, Vector(2, 1, 2));

	worldLst->push_back(wo);

	//this is the transparent front board
	wm->setPlaneRot({ 0, 0.7071068, 0, 0.7071068 });
	wo = wm->createPlane({ 3.5, 0, 0 });

	worldLst->push_back(wo);

	//left side plane
	wm->setPlaneRot({ 0.7071068, 0, 0, 0.7071068 });
	wo = wm->createPlane({ 0, -5, 0 });

	worldLst->push_back(wo);

	//right side plane
	wo = wm->createPlane({0, wm->getMax() + 5, 0});

	worldLst->push_back(wo);
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
