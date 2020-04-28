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
#include "WOFTGLString.h"
#include "MGLFTGLString.h"
#include "WOGUILabel.h"

using namespace Aftr;

GLViewPachinko* GLViewPachinko::New( const std::vector< std::string >& args )
{
   GLViewPachinko* glv = new GLViewPachinko( args );
   glv->init( Aftr::GRAVITY, Vector( 0, 0, -1.0f ), "aftr.conf", PHYSICS_ENGINE_TYPE::petODE );
   glv->score = 0;
   glv->onCreate();
   glv->started = false;
   glv->ballOut = false;
   

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
void GLViewPachinko::createGUI() {

	std::string trebuc(ManagerEnvironmentConfiguration::getSMM() + "/fonts/TREBUC.ttf");

	WOGUILabel* easyBoard = WOGUILabel::New(nullptr);
	easyBoard->setText(" 1 - Easy Board");
	easyBoard->setColor(255, 255, 255, 255);
	easyBoard->setFontSize(15); //font size is correlated with world size
	easyBoard->setPosition(Vector(0, 1, 0));
	easyBoard->setFontOrientation(FONT_ORIENTATION::foLEFT_TOP);
	easyBoard->setFontPath(trebuc);
	worldLst->push_back(easyBoard);

	WOGUILabel* medBoard = WOGUILabel::New(nullptr);
	medBoard->setText(" 2- Medium Board");
	medBoard->setColor(255, 255, 255, 255);
	medBoard->setFontSize(15); //font size is correlated with world size
	medBoard->setPosition(Vector(0, 0.95, 0));
	medBoard->setFontOrientation(FONT_ORIENTATION::foLEFT);
	medBoard->setFontPath(trebuc);
	worldLst->push_back(medBoard);

	WOGUILabel* lrgBoard = WOGUILabel::New(nullptr);
	lrgBoard->setText(" 3- Hard Board");
	lrgBoard->setColor(255, 255, 255, 255);
	lrgBoard->setFontSize(15); //font size is correlated with world size
	lrgBoard->setPosition(Vector(0, 0.92, 0));
	lrgBoard->setFontOrientation(FONT_ORIENTATION::foLEFT);
	lrgBoard->setFontPath(trebuc);
	worldLst->push_back(lrgBoard);

	WOGUILabel* spwnBall = WOGUILabel::New(nullptr);
	spwnBall->setText(" Space- Spawn Ball");
	spwnBall->setColor(255, 255, 255, 255);
	spwnBall->setFontSize(15); //font size is correlated with world size
	spwnBall->setPosition(Vector(0, 0.89, 0));
	spwnBall->setFontOrientation(FONT_ORIENTATION::foLEFT);
	spwnBall->setFontPath(trebuc);
	worldLst->push_back(spwnBall);

	WOGUILabel* delBall = WOGUILabel::New(nullptr);
	delBall->setText(" e- Erase Ball");
	delBall->setColor(255, 255, 255, 255);
	delBall->setFontSize(15); //font size is correlated with world size
	delBall->setPosition(Vector(0, 0.85, 0));
	delBall->setFontOrientation(FONT_ORIENTATION::foLEFT);
	delBall->setFontPath(trebuc);
	worldLst->push_back(delBall);

	WOGUILabel* resHub = WOGUILabel::New(nullptr);
	resHub->setText(" r- Reset Hub");
	resHub->setColor(255, 255, 255, 255);
	resHub->setFontSize(15); //font size is correlated with world size
	resHub->setPosition(Vector(0, 0.81, 0));
	resHub->setFontOrientation(FONT_ORIENTATION::foLEFT);
	resHub->setFontPath(trebuc);
	worldLst->push_back(resHub);

	scoreLabel = WOGUILabel::New(nullptr);
	scoreLabel->setText("Score: " + std::to_string(score));
	scoreLabel->setColor(255, 255, 255, 255);
	scoreLabel->setFontSize(25); //font size is correlated with world size
	scoreLabel->setPosition(Vector(0.99, 0.98, 0));
	scoreLabel->setFontOrientation(FONT_ORIENTATION::foRIGHT_TOP);
	scoreLabel->setFontPath(trebuc);
	worldLst->push_back(scoreLabel);
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


   this->snd = SoundMngr::init();
   this->snd->play2D("/include/music/dreamscape.mp3", true, false, true);
   this->snd->getSound2D().at(0)->setVolume(0.5f);

   createGUI();
}

GLViewPachinko::~GLViewPachinko()
{
	delete wm;
}

void GLViewPachinko::updateWorld()
{
   GLView::updateWorld(); //Just call the parent's update world first.
                          //If you want to add additional functionality, do it after
                          //this call.

   if (wm)
   {
	   wm->updatePhysics();
   }

   WO* ball = wm->getBall();
   // if ball is out AND ball is below z = 4
   if (ball != nullptr && ballOut && ball->getPosition().z < 4.0)
   {
	   // if ball is sleeping
	   if (((PxRigidDynamic*)((PachinkoWOP*)ball)->getActor())->isSleeping())
	   {
		   float ypos = ball->getPosition().y;

		   int toAdd = wm->getBucketVals().at(int(floor(ypos + 5)) / 5);

		   updateScore(toAdd);

		   eraseBall();
	   }
   }

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

void GLViewPachinko::kill()
{
	ballOut = false;
	worldLst->clear();
	wm->changeScene();	//delete the physx objects
}

void Aftr::GLViewPachinko::eraseBall()
{
	WO* wo = wm->getBall();
	PxActor* a = ((PachinkoWOP*)wo)->getActor();
	worldLst->eraseViaWOptr(wo);
	if (a->isReleasable())
	{
		a->release();
	}
	
	ballOut = false;
}

void GLViewPachinko::updateScore(int toAdd)
{
	score += toAdd;
	scoreLabel->setText("Score: " + std::to_string(score));
}

void GLViewPachinko::onKeyDown( const SDL_KeyboardEvent& key )
{
   GLView::onKeyDown( key );
   if( key.keysym.sym == SDLK_0 )
      this->setNumPhysicsStepsPerRender( 1 );

   if(!started && key.keysym.sym == SDLK_1 )
   {	
	   kill();

	   WO* wo = WOSkyBox::New((ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_lemon_lime+6.jpg"), this->getCameraPtrPtr());
	   wo->setPosition(Vector(0, 0, 0));
	   wo->setLabel("Sky Box");
	   wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
	   worldLst->push_back(wo);

	   createField(SMALL);
   }
   if (!started && key.keysym.sym == SDLK_2)
   {
	   kill();

	   WO* wo = WOSkyBox::New((ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_water+6.jpg"), this->getCameraPtrPtr());
	   wo->setPosition(Vector(0, 0, 0));
	   wo->setLabel("Sky Box");
	   wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
	   worldLst->push_back(wo);

	   createField(MEDIUM);
   }
   if (!started && key.keysym.sym == SDLK_3)
   {

	   kill();

	   WO* wo = WOSkyBox::New((ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_dust+6.jpg"), this->getCameraPtrPtr());
	   wo->setPosition(Vector(0, 0, 0));
	   wo->setLabel("Sky Box");
	   wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
	   worldLst->push_back(wo);

	   createField(LARGE);
   }
   if (started && !ballOut && key.keysym.sym == SDLK_SPACE)
   {
	    ballOut = true;
		WO* wo = wm->__createPachinkoBall();
		wm->setBall(wo);
		worldLst->push_back(wo);
   }
   if (key.keysym.sym == SDLK_r)
   {
	   //this->resetEngine(); // this doesn't work with physx
	   worldLst->clear();
	   wm->setBall(nullptr);
	   
	   WO* wo = WOSkyBox::New((ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_Hubble_Nebula+6.jpg"), this->getCameraPtrPtr());
	   wo->setPosition(Vector(0, 0, 0));
	   wo->setLabel("Sky Box");
	   wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
	   worldLst->push_back(wo);

	   started = false;
	   
	   score = 0;
	   updateScore(0);
	   
   }
   if (ballOut && key.keysym.sym == SDLK_e)
   {
	   eraseBall();
   }
}

void GLViewPachinko::onKeyUp( const SDL_KeyboardEvent& key )
{
   GLView::onKeyUp( key );
}

std::vector<Vector> GLViewPachinko::presetOne(){
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
	wm->setSize(5);

	setBoardState(Vector(1.5, 1, 0.9));

	std::vector vals = { 0, 2, 4, 4, 2, 0 };

	wm->setBucketVals(vals);
	createBucketFonts(vals);

	return list;
}

std::vector<Vector> GLViewPachinko::presetTwo(){
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
	wm->setSize(8);

	setBoardState(Vector(1.9, 1, 1.3));

	std::vector vals = { -5, 0, 5, 15, 30, 15, 5, 0, -5 };

	wm->setBucketVals(vals);
	createBucketFonts(vals);

	return list;
}

std::vector<Vector> GLViewPachinko::presetThree(){
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
	wm->setSize(12);

	setBoardState(Vector(2.3, 1, 1.7));

	std::vector vals = { -20, 0, 10, 20, 10, -10, 60, -10, 10, 20, 10, 0, -20 };

	wm->setBucketVals(vals);
	createBucketFonts(vals);

	return list;

}

void GLViewPachinko::createField(int preset)
{
	started = true;
	std::vector<Vector> list;
	switch (preset)
	{
	case 1:
	{
		list = presetOne();
		break;
	}
	case 2:
	{
		list = presetTwo();
		break;
	}
	case 3:
	{
		list = presetThree();
		break;
	}
	}

	// to get the rotation angles, use this website:
	// https://www.andre-gaschler.com/rotationconverter/
	// and use the "Euler angles of multiple axis rotations" input
	wm->setPegRot({ 0.0864101, -0.1300295, -0.0113761, 0.9876721 });

	for (int i = 0; i < list.size(); i++)
	{
		worldLst->push_back(wm->createPachinkoPeg(list[i]));
	}
}

void GLViewPachinko::createBucketFonts(std::vector<int> bucketVals) {

	int size = bucketVals.size();

	float start = -2.5;

	for (int i = 0; i < size; i++) {
		WOFTGLString* scoreLabel = WOFTGLString::New(ManagerEnvironmentConfiguration::getSMM() + "/fonts/TREBUC.ttf", 30);
		
		scoreLabel->getModelT<MGLFTGLString>()->setFontColor(aftrColor4f(255.0f, 255.0f, 255.0f, 255.0f));
		scoreLabel->getModelT<MGLFTGLString>()->setSize(2, 1);
		scoreLabel->setPosition(0, start, 5);
		scoreLabel->rotateAboutGlobalZ(90 * Aftr::DEGtoRAD);
		scoreLabel->rotateAboutGlobalY(90 * Aftr::DEGtoRAD);
		scoreLabel->getModelT<MGLFTGLString>()->setText(std::to_string(bucketVals[i]));
		worldLst->push_back(scoreLabel);

		start += 5;
	}

}

void Aftr::GLViewPachinko::loadMap()
{
   // create the Physics Manager
   wm = new WOPManager();

   this->worldLst = new WorldList(); //WorldList is a 'smart' vector that is used to store WO*'s
   this->actorLst = new WorldList();
   this->netLst = new WorldList();

   ManagerOpenGLState::GL_CLIPPING_PLANE = 1000.0;
   ManagerOpenGLState::GL_NEAR_PLANE = 0.1f;
   ManagerOpenGLState::enableFrustumCulling = false;
   Axes::isVisible = false;
   this->glRenderer->isUsingShadowMapping( false ); //set to TRUE to enable shadow mapping, must be using GL 3.2+
   this->cam->setCameraProjectionType(Camera::PROJ_MATRIX_PROJ_TYPE::pmptORTHOGRAPHIC);
   this->cam->rotateAboutGlobalZ(180 * Aftr::DEGtoRAD);
   this->cam->updateProjectionMatrix();

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
   //wm->setPlaneRot({ 0, 0, 0, 1 });
   //wo = wm->createPlane({ 0, 0, 0 });
   //PachinkoWOP::New(p, scene, grass, Vector(1,1,1), MESH_SHADING_TYPE::mstFLAT, PachinkoWOP::PxObj::Floor, physx::PxVec3{ 0, 0, 0 });
   //wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
   //wo->getModel()->getModelDataShared()->getModelMeshes().at(0)->getSkins().at(0).getMultiTextureSet().at(0)->setTextureRepeats( 5.0f );
   //wo->setLabel( "Grass" );
   //worldLst->push_back( wo );
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

   //worldLst->push_back(wm->createBoard({ 50, 50, 50 }, grass));//(ManagerEnvironmentConfiguration::getSMM() + "images/DefaultTexture.jpg")));

}


/*
	This will set all board surrounding pieces that will hold the balls in. Front, back, and both sides
*/
void GLViewPachinko::setBoardState(Vector scale) {

	//style for the background board
	std::string board(ManagerEnvironmentConfiguration::getLMM() + "/models/finally.obj");

	//this is the background board
	wm->setBoardRot({ 0, 0.7071068, 0, 0.7071068 });
	WO* wo = wm->createBoard(Vector(-3.5, wm->getMax()/2.0, (wm->getHeight() - 10)/2.0), board, scale);

	this->cam->setPosition(60, wm->getMax() / 2.0, (wm->getHeight() - 10) / 2.0);


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

	// create pots in the bottom of the board
	for (int i = 0; i < wm->getSize(); i++)
	{
		int v = i * 5;
		wo = wm->createBucket(Vector(0, v, 0));
		worldLst->push_back(wo);
	}

	wo = wm->createFloor((ManagerEnvironmentConfiguration::getLMM() + "/models/divider.obj"));
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
