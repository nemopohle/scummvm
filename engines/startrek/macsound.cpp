/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "common/config-manager.h"
#include "common/macresman.h"

#include "audio/mac/halestorm.h"
#include "audio/mixer.h"

#include "startrek/sound.h"

namespace StarTrek {

// The Mac versions replace the tracks of the MIDI files with the songs (SONG) and
// sound effects (snd) of "Star Trek Audio". IDs from 12000 are sound effects.
static const int16 kMacTitleTracks[] = { 128 };
static const int16 kMacBridgeTracks[] = {
	155, 12001, 133, 136, 137, 12005, 12006, 12007, 12008, 12009,
	12010, 145, 142, 140, 144, 130, 12016, 132, 12018
};
static const int16 kMacBridgeWTracks[] = {
	155, 12001, 134, 136, 137, 12005, 12006, 12007, 12008, 12009,
	12010, 145, 142, 140, 144, 130, 12016, 132, 12018
};
static const int16 kMacBridgeBTracks[] = {
	155, 12001, 135, 136, 137, 12005, 12006, 12007, 12008, 12009,
	12010, 145, 142, 140, 144, 130, 12016, 132, 12018
};
static const int16 kMacGroundTracks[] = {
	129, 130, 131, 132, 15004, 15005, 15006, 15007, 15008, 15009,
	15010, 15011, 15012, -1, 154, 152, 12016, -1, 150, 153,
	15020, 155, 15022, 151, 146, 139, 140, 141, 142, 143,
	144, 138, 147, 145, -1, 15035, 15036, 15037, 15038
};

// The sound effects that each music file loads
static const uint16 kMacTitleSamples[] = { 12001, 0xffff };
static const uint16 kMacBridgeSamples[] = {
	12001, 12005, 12006, 12007, 12008, 12009, 12010, 12016, 12018, 0xffff
};
static const uint16 kMacGroundSamples[] = {
	15004, 15005, 15006, 15007, 15008, 15009, 15010, 15011, 15012, 12016,
	15020, 15022, 15035, 15036, 15037, 15038, 0xffff
};

static const struct {
	const char *name;
	const int16 *tracks;
	int count;
	const uint16 *samples;
} kMacMusicFiles[] = {
	{ "title", kMacTitleTracks, ARRAYSIZE(kMacTitleTracks), kMacTitleSamples },
	{ "bridge", kMacBridgeTracks, ARRAYSIZE(kMacBridgeTracks), kMacBridgeSamples },
	{ "bridgew", kMacBridgeWTracks, ARRAYSIZE(kMacBridgeWTracks), kMacBridgeSamples },
	{ "bridgeb", kMacBridgeBTracks, ARRAYSIZE(kMacBridgeBTracks), kMacBridgeSamples },
	{ "ground", kMacGroundTracks, ARRAYSIZE(kMacGroundTracks), kMacGroundSamples }
};

static const int16 kMacFirstSoundEffect = 12000;

// The songs that the original plays in a loop
static const int16 kMacLoopingSongs[] = { 138, 141, 146, 152 };

class MacSoundLoader : public Audio::HalestormLoader {
public:
	bool open() { return _resFork.open("Star Trek Audio") && _resFork.hasResFork(); }

	Common::SeekableReadStream *getResource(uint16 id, uint32 type) override {
		return _resFork.getResource(type, id);
	}

private:
	Common::MacResManager _resFork;
};

bool Sound::initMacSound() {
	_macLoader = new MacSoundLoader();
	if (!_macLoader->open()) {
		warning("Couldn't find 'Star Trek Audio'. Continuing without music");
		return false;
	}

	_macDriver = new Audio::HalestormDriver(_macLoader, _vm->_system->getMixer());
	if (!_macDriver->init(true, Audio::HalestormDriver::kSimple, 2, true))
		return false;
	// Like the original on a 68040: 4 song voices (3 of them polyphonic), 2 sound
	// effect voices, 22 kHz with interpolation
	_macDriver->changeSystemVoices(4, 3, 2);
	_macDriver->doCommand(Audio::HalestormDriver::kSetRateAndIntrplMode, 5);

	syncSoundSettings();
	return true;
}

void Sound::deinitMacSound() {
	delete _macDriver;
	delete _macLoader;
}

void Sound::syncSoundSettings() {
	if (!_macDriver)
		return;

	bool mute = ConfMan.hasKey("mute") && ConfMan.getBool("mute");
	_macDriver->setMusicVolume(mute ? 0 : CLIP<int>(ConfMan.getInt("music_volume"), 0, Audio::Mixer::kMaxMixerVolume));
	_macDriver->setSoundEffectVolume(mute ? 0 : CLIP<int>(ConfMan.getInt("sfx_volume"), 0, Audio::Mixer::kMaxMixerVolume));
}

void Sound::loadMacMusicFile(const Common::String &baseSoundName) {
	_macTracks = nullptr;
	_macTrackCount = 0;
	if (!_macDriver)
		return;

	for (int i = 0; i < ARRAYSIZE(kMacMusicFiles); i++) {
		if (baseSoundName.equalsIgnoreCase(kMacMusicFiles[i].name)) {
			_macTracks = kMacMusicFiles[i].tracks;
			_macTrackCount = kMacMusicFiles[i].count;
			_macDriver->registerSamples(kMacMusicFiles[i].samples, false);
			return;
		}
	}

	warning("Unknown music file '%s'", baseSoundName.c_str());
}

void Sound::playMacTrack(int track, bool music) {
	if (!_macDriver || track < 0 || track >= _macTrackCount)
		return;

	int16 id = _macTracks[track];
	if (id < 0)
		return;

	debugC(6, kDebugSound, "Playing Mac track %d (%d)", track, id);
	if (id >= kMacFirstSoundEffect) {
		_macDriver->startSoundEffect(id);
		return;
	}

	// There is only one song at a time, also for the sound effects that are songs
	int command = Audio::HalestormDriver::kSongPlayOnce;
	for (int i = 0; i < ARRAYSIZE(kMacLoopingSongs); i++) {
		if (kMacLoopingSongs[i] == id)
			command = Audio::HalestormDriver::kSongPlayLoop;
	}
	_macDriver->doCommand(command, id);
	if (music)
		_macMusicTrack = track;
}

void Sound::stopMacSounds(bool music, bool soundEffects) {
	if (!_macDriver)
		return;

	if (music) {
		_macDriver->doCommand(Audio::HalestormDriver::kSongAbort);
		_macMusicTrack = -1;
	}
	if (soundEffects)
		_macDriver->stopAllSoundEffects();
}

bool Sound::isMacMusicPlaying() const {
	return _macDriver && _macDriver->doCommand(Audio::HalestormDriver::kSongIsPlaying);
}

} // End of namespace StarTrek
