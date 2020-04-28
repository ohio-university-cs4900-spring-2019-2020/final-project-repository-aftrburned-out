#include "SoundMngr.h"

using namespace Aftr;
using namespace std;


// Initialize soundengine.
SoundMngr* SoundMngr::init() {
	SoundMngr* snd = new SoundMngr();
	irrklang::ISoundEngine* tmpEngine = irrklang::createIrrKlangDevice();

	if (tmpEngine == NULL) {
		cout << "Create Sound Engine Error!!!" << endl;
	}
	snd->setSoundEngine(tmpEngine);

	return snd;
}

void SoundMngr::play2D(const char* fileName, bool looped, bool paused, bool track) {
	if (!this->engine)
		return;

	this->sound2D.push_back(this->engine->play2D(fileName, looped, paused, track));
}

void SoundMngr::play3D(const char* soundFileName, Vector position, bool playLooped, bool startPaused, bool track) {
	if (!this->engine)
		return;

	this->sound3D.push_back(this->engine->play3D(soundFileName, this->convertFromVector(position), playLooped, startPaused, track));
}

void SoundMngr::stopSound2D() {
	if (sound2D.empty()) {
		return;
	}

	sound2D.at(0)->stop();
	sound2D.erase(sound2D.begin());
}

// Stop a 3D sound.
void SoundMngr::stopSound3D() {
	if (sound3D.empty()) {
		return;
	}

	sound3D.at(0)->stop();
	sound3D.erase(sound3D.begin());
}

// Convert position type from Aftr::Vector into irrklang vec3df.
irrklang::vec3df SoundMngr::convertFromVector(Vector position) {
	return irrklang::vec3df(position.x, position.y, position.z);
}


void SoundMngr::setSoundEngine(irrklang::ISoundEngine* soundEngine) {
	this->engine = soundEngine;
}


irrklang::ISoundEngine* SoundMngr::getSoundEngine() {
	return this->engine;
}

std::vector<irrklang::ISound*> SoundMngr::getSound2D() {
	return this->sound2D;
}

std::vector<irrklang::ISound*> SoundMngr::getSound3D() {
	return this->sound3D;
}