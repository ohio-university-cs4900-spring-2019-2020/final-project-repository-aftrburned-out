#pragma once

#include "GLView.h"
#include "WOPManager.h"
#include "WOFTGLString.h"
#include "MGLFTGLString.h"
#include "WOGUILabel.h"
#include "soundMngr.h"

namespace Aftr
{
   class Camera;

/**
   \class GLViewPachinko 
   \brief A child of an abstract GLView. This class is the top-most manager of the module.

   Read \see GLView for important constructor and init information.

   \see GLView

    \{
*/

class GLViewPachinko : public GLView
{
public:
   static GLViewPachinko* New( const std::vector< std::string >& outArgs );
   virtual ~GLViewPachinko();
   virtual void updateWorld(); ///< Called once per frame
   virtual void loadMap(); ///< Called once at startup to build this module's scene
   virtual void createPachinkoWayPoints();
   virtual void onResizeWindow( GLsizei width, GLsizei height );
   virtual void onMouseDown( const SDL_MouseButtonEvent& e );
   virtual void onMouseUp( const SDL_MouseButtonEvent& e );
   virtual void onMouseMove( const SDL_MouseMotionEvent& e );
   virtual void onKeyDown( const SDL_KeyboardEvent& key );
   virtual void onKeyUp( const SDL_KeyboardEvent& key );
   virtual void setBoardState(Vector scale);
   virtual std::vector<Vector> presetOne();
   virtual std::vector<Vector> presetTwo();
   virtual std::vector<Vector> presetThree();
   virtual void createField(int preset);
   virtual void createBucketFonts(std::vector<int>);
   virtual void kill();
   virtual void eraseBall();
   virtual void updateScore(int toAdd);
   virtual void createGUI();
   enum presets{SMALL = 1, MEDIUM = 2, LARGE = 3};


protected:
   GLViewPachinko( const std::vector< std::string >& args );
   virtual void onCreate();
   bool started;
   bool ballOut;
private:
	WOPManager* wm;
	int score;
	WOGUILabel* scoreLabel;
    SoundMngr* snd;
};

/** \} */

} //namespace Aftr
