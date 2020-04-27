#pragma once

#include "GLView.h"
#include "WOPManager.h"

namespace Aftr
{
   class Camera;

/**
   \class GLViewPachinko
   \author Scott Nykl 
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
   virtual void setBoardState();

protected:
   GLViewPachinko( const std::vector< std::string >& args );
   virtual void onCreate();
private:
	WOPManager* wm;
};

/** \} */

} //namespace Aftr
