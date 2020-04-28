#pragma once
#include "irrKlang.h"
#include "ik_ISoundEngine.h"
#include "Vector.h"

namespace Aftr {
	class SoundMngr {

	protected:
		irrklang::ISoundEngine* engine = nullptr;
		std::vector<irrklang::ISound*> sound2D;
		std::vector<irrklang::ISound*> sound3D;

	public:
		static SoundMngr* init();
		virtual void play2D(const char* fileName, bool looped, bool paused, bool track);
		virtual void play3D(const char* fileName, Aftr::Vector position, bool playLooped, bool paused, bool track);
		virtual void stopSound2D();
		virtual void stopSound3D();
		virtual irrklang::vec3df convertFromVector(Vector position);
		virtual irrklang::ISoundEngine* getSoundEngine();
		virtual void setSoundEngine(irrklang::ISoundEngine* soundEngine);
		virtual std::vector<irrklang::ISound*> getSound2D();
		virtual std::vector<irrklang::ISound*> getSound3D();  
	};
}