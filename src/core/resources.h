/**
 * This file is part of OpenAtomic.
 *
 * OpenAtomic is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * OpenAtomic is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenAtomic. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * File:   resources.h
 * Author: minji
 *
 * Created on 5. Januar 2013, 20:37
 */

#ifndef RESOURCES_H
#define	RESOURCES_H

#include <SDL/SDL_mixer.h>
#include <SDL/SDL_thread.h>

#include <debug/errorreporter.h>
#include <readers/alireader.h>
#include <readers/anireader.h>
#include <core/gameobject.h>
#include <core/seqparser.h>

#include <string>
using namespace std;

// die0.wav -- die2.wav = 3 files! FIX!
#define C_DIE_SOUNDS	  3
// explode0.wav -- explode7.wav = 8 files! FIX!
#define C_EXPLODE_SOUNDS  8

class CResources {
public:

	bool Load();

	string StatusString() {
		return m_statusString;
	}

	bool Loaded() {
		return m_loaded;
	}

	static CResources *instance() {
		if (!s_instance)
			s_instance = new CResources;
		return s_instance;
	}

	// public resources:

	Mix_Chunk* getChooseSound();
	Mix_Chunk* getDieSound(Uint16 i);
	Mix_Chunk* getExplodeSound(Uint16 i);

private:
	CErrorReporter er;
	static CResources *s_instance;

	string m_statusString;
	bool m_loaded;

	bool LoadAni(CAliReader &ali);
	void SetupAni();
	bool LoadPcx();
	bool LoadAli(CAliReader &ali);
	bool LoadCfg();
	bool loadSounds();

	CResources();
	~CResources();

	// resource thread
	static int staticResThreadWorker(void *p) {
		return ((CResources*) p)->Load();
	}

};

#endif	/* RESOURCES_H */

