//**********************************************************************************
// STEAMiE's Entry Point.
//**********************************************************************************

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "GLViewPachinko.h" //GLView subclass instantiated to drive this simulation

/// Saves the in passed params argc and argv in a vector of strings.
std::vector< std::string > saveInputParams( int argc, char** argv );

/**
   This creates a GLView subclass instance and begins the GLView's main loop.
   Each iteration of this loop occurs when a reset request is received. A reset
   request causes the entire GLView to be destroyed (since its exits scope) and
   begin again (simStatus == -1). This loop exits when a request to exit the 
   application is received (simStatus == 0 ).
*/
int main( int argc, char* argv[] )
{
   std::vector< std::string > args = saveInputParams( argc, argv ); ///< Command line arguments passed via argc and argv, reserved to size of argc
   int simStatus = 0;

   do
   {
      std::unique_ptr< Aftr::GLViewPachinko > glView( Aftr::GLViewPachinko::New( args ) );
      simStatus = glView->startWorldSimulationLoop(); // Runs until simulation exits or requests a restart (values 0 or -1, respectively)
   }
   while( simStatus != 0 );

   std::cout << "Exited AfterBurner Engine Normally..." << std::endl;
   return 0;
}

std::vector< std::string > saveInputParams( int argc, char** argv )
{
   std::vector< std::string > args( argc );
   for( int i = 0; i < argc; ++i )
   {
      std::string arg( argv[i] );
      args[i] = arg;
   }
   return args;
}
