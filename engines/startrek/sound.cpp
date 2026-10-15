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

#include "startrek/resource.h"
#include "startrek/sound.h"

#include "common/file.h"
#include "common/macresman.h"
#include "common/tokenizer.h"

#include "audio/audiostream.h"
#include "audio/decoders/quicktime.h"
#include "audio/decoders/raw.h"
#include "audio/decoders/voc.h"
#include "audio/mods/protracker.h"

namespace StarTrek {

// Main Sound Functions

Sound::Sound(StarTrekEngine *vm) : _vm(vm) {
	_midiDevice = MT_AUTO;
	_midiDriver = nullptr;
	_loopingMidiTrack = MIDITRACK_NONE;

	if (_vm->getPlatform() == Common::kPlatformDOS) {
		_midiDevice = MidiDriver::detectDevice(MDT_PCSPK | MDT_ADLIB | MDT_MIDI | MDT_PREFER_MT32);
		_midiDriver = MidiDriver::createMidi(_midiDevice);
		_midiDriver->open();

		for (int i = 0; i < NUM_MIDI_SLOTS; i++) {
			_midiSlots[i].slot = i;
			_midiSlots[i].track = -1;

			// The main PC versions use XMIDI. ST25 Demo and Macintosh versions use SMF.
			if ((_vm->getGameType() == GType_ST25 && _vm->getFeatures() & GF_DEMO) || _vm->getPlatform() == Common::kPlatformMacintosh)
				_midiSlots[i].midiParser = MidiParser::createParser_SMF();
			else
				_midiSlots[i].midiParser = MidiParser::createParser_XMIDI();

			_midiSlots[i].midiParser->setMidiDriver(_midiDriver);
			_midiSlots[i].midiParser->setTimerRate(_midiDriver->getBaseTempo());
		}

		_midiDriver->setTimerCallback(this, Sound::midiDriverCallback);
	} else if (_vm->getPlatform() != Common::kPlatformAmiga && _vm->getPlatform() != Common::kPlatformMacintosh) {
		_vm->_musicWorking = false;
	}

	if (!_midiDriver) {
		for (int i = 0; i < NUM_MIDI_SLOTS; i++)
			_midiSlots[i].midiParser = nullptr;
	}
	_macLoader = nullptr;
	_macDriver = nullptr;
	_macTracks = nullptr;
	_macTrackCount = 0;
	_macMusicTrack = -1;
	if (_vm->getPlatform() == Common::kPlatformMacintosh && !(_vm->getFeatures() & GF_DEMO))
		_vm->_musicWorking = initMacSound();

	_amigaTracks = nullptr;
	_amigaTrackCount = 0;
	_amigaMusicTrack = -1;
	_amigaBank = nullptr;
	_amigaBankSize = 0;

	_soundHandle = new Audio::SoundHandle();
	_loadedSoundData = nullptr;
	_loadedSoundDataSize = 0;
	_demoMusicData[0] = _demoMusicData[1] = nullptr;

	for (int i = 1; i < NUM_MIDI_SLOTS; i++) {
		_midiSlotList.push_back(&_midiSlots[i]);
	}

	if (!(_vm->getFeatures() & GF_CDROM))
		_vm->_sfxWorking = false;
	else if (_vm->getPlatform() == Common::kPlatformMacintosh) {
		if (!SearchMan.hasFile("Voice Data/SFX/LOGO")) {
			warning("Couldn't find the 'Voice Data' folder of the CD. Continuing without CD audio");
			_vm->_sfxWorking = false;
		}
	} else if (!SearchMan.hasFile("voc/speech.mrk")) {
		warning("Couldn't find 'voc/speech.mrk'. The 'trekcd/voc/' directory should be dumped from the CD. Continuing without CD audio");
		_vm->_sfxWorking = false;
	}

	_playingSpeech = false;
}

Sound::~Sound() {
	for (int i = 0; i < NUM_MIDI_SLOTS; i++)
		delete _midiSlots[i].midiParser;
	delete _midiDriver;
	delete _soundHandle;
	delete[] _loadedSoundData;
	delete[] _demoMusicData[0];
	delete[] _demoMusicData[1];
	delete[] _amigaBank;
	deinitMacSound();
}


void Sound::clearAllMidiSlots() {
	for (int i = 0; i < NUM_MIDI_SLOTS; i++) {
		clearMidiSlot(i);
	}
}

void Sound::playMidiTrack(MidiTracks track) {
	if (!_vm->_musicEnabled || !_vm->_musicWorking)
		return;

	// TODO: Demo music
	if (_vm->getFeatures() & GF_DEMO)
		return;

	if (_vm->getPlatform() == Common::kPlatformAmiga) {
		playAmigaTrack(track, false);
		return;
	}
	if (_vm->getPlatform() == Common::kPlatformMacintosh) {
		playMacTrack(track, false);
		return;
	}

	assert(_loadedSoundData != nullptr);

	// Check if a midi slot for this track exists already
	for (int i = 1; i < NUM_MIDI_SLOTS; i++) {
		if (_midiSlots[i].track == track) {
			debugC(6, kDebugSound, "Playing MIDI track %d (slot %d)", track, i);
			_midiSlots[i].midiParser->loadMusic(_loadedSoundData, _loadedSoundDataSize);
			_midiSlots[i].midiParser->setTrack(track);

			// Shift this to the back (most recently used)
			_midiSlotList.remove(&_midiSlots[i]);
			_midiSlotList.push_back(&_midiSlots[i]);
			return;
		}
	}

	// Take the least recently used slot and use that for the sound effect
	MidiPlaybackSlot *slot = _midiSlotList.front();
	_midiSlotList.pop_front();
	_midiSlotList.push_back(slot);
	playMidiTrackInSlot(slot->slot, track);
}

void Sound::playMidiTrackInSlot(int slot, MidiTracks track) {
	if (_vm->getPlatform() == Common::kPlatformAmiga) {
		clearMidiSlot(slot);
		if (track != -1)
			playAmigaTrack(track, slot == 0);
		return;
	}
	if (_vm->getPlatform() == Common::kPlatformMacintosh) {
		clearMidiSlot(slot);
		if (track != -1)
			playMacTrack(track, slot == 0);
		return;
	}

	assert(_loadedSoundData != nullptr);
	debugC(6, kDebugSound, "Playing MIDI track %d (slot %d)", track, slot);

	clearMidiSlot(slot);

	if (track != -1) {
		_midiSlots[slot].track = track;
		_midiSlots[slot].midiParser->loadMusic(_loadedSoundData, _loadedSoundDataSize);
		_midiSlots[slot].midiParser->setTrack(track);
	}
}

bool Sound::isMidiPlaying() {
	if (!_vm->_musicWorking)
		return false;

	if (_vm->getPlatform() == Common::kPlatformAmiga) {
		if (isAmigaMusicPlaying())
			return true;
		for (int i = 0; i < MAX_SFX_PLAYING; i++) {
			if (_vm->_system->getMixer()->isSoundHandleActive(_amigaSfxHandles[i]))
				return true;
		}
		return false;
	}
	if (_vm->getPlatform() == Common::kPlatformMacintosh)
		return isMacMusicPlaying();

	for (int i = 0; i < NUM_MIDI_SLOTS; i++) {
		if (_midiSlots[i].midiParser->isPlaying())
			return true;
	}

	return false;
}

bool Sound::isMT32() const {
	return _vm->_musicWorking && MidiDriver::getMusicType(_midiDevice) == MT_MT32;
}

void Sound::loadMusicFile(const Common::String &baseSoundName) {
	bool isDemo = _vm->getFeatures() & GF_DEMO;

	clearAllMidiSlots();
	_loopingMidiTrack = MIDITRACK_NONE;

	if (baseSoundName == _loadedMidiFilename)
		return;

	_loadedMidiFilename = baseSoundName;

	if (_vm->getPlatform() == Common::kPlatformDOS && !isDemo) {
		loadPCMusicFile(baseSoundName);
	} else if (_vm->getPlatform() == Common::kPlatformDOS && isDemo) {
		//playSMFSound(baseSoundName);
	} else if (_vm->getPlatform() == Common::kPlatformAmiga) {
		loadAmigaMusicFile(baseSoundName);
	} else if (_vm->getPlatform() == Common::kPlatformMacintosh) {
		loadMacMusicFile(baseSoundName);
	}
}

void Sound::playMidiMusicTracks(MidiTracks startTrack, MidiLoopType loopType) {
	if (!_vm->_musicWorking || !_vm->_musicEnabled)
		return;

	if (loopType == kLoopTypeRepeat)
		_loopingMidiTrack = startTrack;
	else if (loopType == kLoopTypeNone)
		_loopingMidiTrack = MIDITRACK_NONE;
	else if (loopType >= 0)
		_loopingMidiTrack = (MidiTracks)loopType;

	if (_vm->_musicEnabled)
		playMidiTrackInSlot(0, startTrack);
}

static bool hasVocRepeatBlock(Common::SeekableReadStream *stream) {
	bool found = false;
	stream->seek(0x14);
	uint32 pos = stream->readUint16LE();
	while (!found && pos + 4 <= stream->size()) {
		stream->seek(pos);
		byte type = stream->readByte();
		if (type == 0)
			break;
		uint32 size = stream->readByte();
		size |= stream->readUint16LE() << 8;
		found = type == 6;
		pos += 4 + size;
	}
	stream->seek(0);
	return found;
}

/**
 * TODO: original game had some caching of loaded voc files.
 */
void Sound::playVoc(const Common::String &baseSoundName) {
	/*
	if (_vm->getPlatform() == Common::kPlatformAmiga)
		playAmigaSoundEffect(baseSoundName);
	else if (_vm->getPlatform() == Common::kPlatformMacintosh)
		playMacSoundEffect(baseSoundName);
	else
	*/
	bool loop = false;
	if (baseSoundName.size() == 8 && baseSoundName.hasSuffixIgnoreCase("loop")) {
		_loopingAudioName = baseSoundName;
		loop = true;
	}

	if (!_vm->_sfxEnabled || !_vm->_sfxWorking)
		return;

	/*
	// This is probably just driver initialization stuff...
	if (word_5113a == 0)
		sub_2aaa3();
	*/

	for (int i = 0; i < MAX_SFX_PLAYING; i++) {
		if (_vm->_system->getMixer()->isSoundHandleActive(_sfxHandles[i]))
			continue;

		debugC(5, kDebugSound, "Playing sound effect '%s'", baseSoundName.c_str());

		// Sounds with a repeat block loop, like the ones named "....loop"
		bool repeats = false;
		Audio::RewindableAudioStream *srcStream = openCDSound("sfx\\" + baseSoundName, &repeats);
		if (!srcStream)
			return;
		if (!loop && repeats) {
			_loopingAudioName = baseSoundName;
			loop = true;
		}

		Audio::AudioStream *audioStream;
		if (loop)
			audioStream = new Audio::LoopingAudioStream(srcStream, 0, DisposeAfterUse::YES);
		else
			audioStream = srcStream;
		_vm->_system->getMixer()->playStream(Audio::Mixer::kSFXSoundType, &_sfxHandles[i], audioStream);
		return;
	}

	debugC(3, kDebugSound, "No sound slot to play '%s'", baseSoundName.c_str());
}

void Sound::playSpeech(const Common::String &basename) {
	stopPlayingSpeech();

	Audio::QueuingAudioStream *audioQueue = nullptr;
	Common::StringTokenizer tok(basename, ",");

	// Play a list of comma-separated audio files in sequence (usually there's only one)
	while (!tok.empty()) {
		Common::String name = tok.nextToken();
		debugC(5, kDebugSound, "Playing speech '%s'", name.c_str());
		Audio::AudioStream *audioStream = openCDSound(name, nullptr);
		if (audioStream != nullptr) {
			if (audioQueue == nullptr)
				audioQueue = Audio::makeQueuingAudioStream(audioStream->getRate(), audioStream->isStereo());
			audioQueue->queueAudioStream(audioStream, DisposeAfterUse::YES);
		}
	}

	if (audioQueue != nullptr) {
		audioQueue->finish();
		_vm->_system->getMixer()->playStream(Audio::Mixer::kSpeechSoundType, &_speechHandle, audioQueue);
		_playingSpeech = true;
	}
}

Audio::RewindableAudioStream *Sound::openCDSound(const Common::String &name, bool *loop) {
	// The original plays nothing when a file is missing
	if (_vm->getPlatform() == Common::kPlatformMacintosh) {
		Common::Path filename = Common::Path("Voice Data/").append(Common::Path(name, '\\'));
		Audio::RewindableAudioStream *stream = Audio::makeQuickTimeStream(filename);
		if (stream == nullptr)
			warning("Couldn't open '%s'", filename.toString().c_str());
		return stream;
	}

	Common::Path filename = Common::Path("voc/").append(Common::Path(name + ".voc", '\\'));
	Common::SeekableReadStream *readStream = SearchMan.createReadStreamForMember(filename);
	if (readStream == nullptr) {
		warning("Couldn't open '%s'", filename.toString().c_str());
		return nullptr;
	}
	if (loop)
		*loop = hasVocRepeatBlock(readStream);
	return Audio::makeVOCStream(readStream, Audio::FLAG_UNSIGNED, DisposeAfterUse::YES);
}

void Sound::playDemoMidiFile(int slot, const Common::String &name) {
	// Without a Roland the demo plays no music
	if (!_vm->_musicWorking || !_vm->_musicEnabled || !isMT32())
		return;

	clearMidiSlot(slot);
	delete[] _demoMusicData[slot];
	Common::MemoryReadStreamEndian *stream = _vm->_resource->loadFile(name + ".mus");
	uint32 size = stream->size();
	_demoMusicData[slot] = new byte[size];
	stream->read(_demoMusicData[slot], size);
	delete stream;

	_midiSlots[slot].track = 0;
	_midiSlots[slot].midiParser->loadMusic(_demoMusicData[slot], size);
	_midiSlots[slot].midiParser->setTrack(0);
}

void Sound::playDemoMusic(const Common::String &name) {
	playDemoMidiFile(0, name);
}

void Sound::startDemoMusicSetup() {
	playDemoMidiFile(1, "sta0v2");
}

bool Sound::isDemoMusicSetupPlaying() {
	return _midiDriver && _midiSlots[1].midiParser->isPlaying();
}

void Sound::stopDemoMusic() {
	clearMidiSlot(0);
}

void Sound::stopAllVocSounds() {
	stopPlayingSpeech();

	for (int i = 0; i < MAX_SFX_PLAYING; i++) {
		_vm->_system->getMixer()->stopHandle(_sfxHandles[i]);
	}
}

void Sound::stopPlayingSpeech() {
	if (_playingSpeech) {
		debugC(5, kDebugSound, "Canceled speech playback");
		_playingSpeech = false;
		_vm->_system->getMixer()->stopHandle(_speechHandle);
	}
}

void Sound::playSoundEffectIndex(SoundEffects index) {
	if (!(_vm->getFeatures() & GF_CDROM)) {
		// The sounds of the bridge are other tracks of its music file
		static const struct {
			SoundEffects sound;
			MidiTracks track;
		} kFloppyBridgeSounds[] = {
			{ kSfxHailing, (MidiTracks)1 },
			{ kSfxPhaser2, (MidiTracks)5 },
			{ kSfxPhotonTorpedoes, (MidiTracks)6 },
			{ kSfxShieldHit, (MidiTracks)7 },
			{ kSfxBridgeMenu, (MidiTracks)8 },
			{ kSfxRedAlert, (MidiTracks)9 },
			{ kSfxWarp, (MidiTracks)10 }
		};
		MidiTracks track = (MidiTracks)index;
		for (int i = 0; i < ARRAYSIZE(kFloppyBridgeSounds); i++) {
			if (kFloppyBridgeSounds[i].sound == index)
				track = kFloppyBridgeSounds[i].track;
		}
		playMidiTrack(track);
	} else {
		switch (index) {
		case kSfxTricorder:
			playVoc("tricorde");
			break;
		case kSfxDoor:
			playVoc("STDOOR1");
			break;
		case kSfxPhaser:
			playVoc("PHASSHOT");
			break;
		case kSfxButton:
			playMidiTrack(MIDITRACK_SFX_BUTTON);
			break;
		case kSfxTransporterDematerialize:
			playVoc("TRANSDEM");
			break;
		case kSfxTransporterMaterialize:
			playVoc("TRANSMAT");
			break;
		case kSfxTransporterEnergize:
			playVoc("TRANSENE");
			break;
		case kSfxSelection:
			playMidiTrack(MIDITRACK_SFX_SELECTION);
			break;
		case kSfxHailing:
			playVoc("HAILING");
			break;
		case kSfxPhaser2:
			playVoc("PHASSHOT");
			break;
		case kSfxPhotonTorpedoes:
			playVoc("PHOTSHOT");
			break;
		case kSfxShieldHit:
			playVoc("HITSHIEL");
			break;
		case kSfxBridgeMenu:
			playMidiTrack(MIDITRACK_SFX_BRIDGE_MENU);
			break;
		case kSfxRedAlert:
			playVoc("REDALERT");
			break;
		case kSfxWarp:
			playVoc("WARP");
			break;
		default:
			debugC(kDebugSound, 6, "Unmapped sound 0x%x", index);
			break;
		}
	}
}

void Sound::toggleMusic() {
	setMusicEnabled(!_vm->_musicEnabled);
}

void Sound::setMusicEnabled(bool enable) {
	if (!_vm->_musicWorking || _vm->_musicEnabled == enable)
		return;

	_vm->_musicEnabled = enable;

	if (enable)
		playMidiTrackInSlot(0, _loopingMidiTrack);
	else
		clearMidiSlot(0);
}

void Sound::toggleSfx() {
	setSfxEnabled(!_vm->_sfxEnabled);
}

void Sound::setSfxEnabled(bool enable) {
	if (!_vm->_sfxWorking || _vm->_sfxEnabled == enable)
		return;

	_vm->_sfxEnabled = enable;

	if (!enable) {
		for (int i = 1; i < NUM_MIDI_SLOTS; i++)
			clearMidiSlot(i);
	}

	if (!enable) {
		stopAllVocSounds();
	} else if (!_loopingAudioName.empty()) {
		playVoc(_loopingAudioName);
	}
}

void Sound::checkLoopMusic() {
	if (!_vm->_musicWorking || !_vm->_musicEnabled || _loopingMidiTrack == MIDITRACK_NONE)
		return;
	bool playing;
	if (_vm->getPlatform() == Common::kPlatformAmiga)
		playing = isAmigaMusicPlaying();
	else if (_vm->getPlatform() == Common::kPlatformMacintosh)
		playing = isMacMusicPlaying();
	else
		playing = _midiSlots[0].midiParser->isPlaying();
	if (!playing)
		playMidiTrackInSlot(0, _loopingMidiTrack);
}

MidiTracks Sound::getPlayingMidiTrack() {
	if (_vm->getPlatform() == Common::kPlatformAmiga)
		return isAmigaMusicPlaying() ? (MidiTracks)_amigaMusicTrack : MIDITRACK_NONE;
	if (_vm->getPlatform() == Common::kPlatformMacintosh)
		return isMacMusicPlaying() ? (MidiTracks)_macMusicTrack : MIDITRACK_NONE;
	if (!_vm->_musicWorking || !_midiSlots[0].midiParser->isPlaying())
		return MIDITRACK_NONE;
	return (MidiTracks)_midiSlots[0].track;
}

bool Sound::isPlayingSpeech() const {
	return _playingSpeech;
}


// XMIDI or SM sound
void Sound::loadPCMusicFile(const Common::String &baseSoundName) {
	Common::String soundName = baseSoundName;

	soundName += '.';

	switch (MidiDriver::getMusicType(_midiDevice)) {
	case MT_MT32:
		if (_vm->getFeatures() & GF_DEMO)
			soundName += "ROL";
		else
			soundName += "MT";
		break;
	case MT_PCSPK:
		if (_vm->getFeatures() & GF_DEMO)
			return; // Not supported...
		else
			soundName += "PC";
		break;
	default:
		if (_vm->getFeatures() & GF_DEMO)
			soundName += "ADL";
		else
			soundName += "AD";
		break;
	}

	debugC(5, kDebugSound, "Loading midi \'%s\'\n", soundName.c_str());
	Common::MemoryReadStreamEndian *soundStream = _vm->_resource->loadFile(soundName.c_str());

	if (_loadedSoundData != nullptr)
		delete[] _loadedSoundData;
	_loadedSoundDataSize = soundStream->size();
	_loadedSoundData = new byte[_loadedSoundDataSize];
	soundStream->read(_loadedSoundData, _loadedSoundDataSize);

	// FIXME: should music start playing when this is called?
	//_midiSlots[0].midiParser->loadMusic(_loadedSoundData, soundStream->size());

	delete soundStream;
}

void Sound::clearMidiSlot(int slot) {
	if (!_vm->_musicWorking)
		return;

	if (_vm->getPlatform() == Common::kPlatformAmiga) {
		stopAmigaSounds(slot == 0, slot != 0);
		return;
	}
	if (_vm->getPlatform() == Common::kPlatformMacintosh) {
		stopMacSounds(slot == 0, slot != 0);
		return;
	}

	_midiSlots[slot].midiParser->stopPlaying();
	_midiSlots[slot].midiParser->unloadMusic();
	_midiSlots[slot].track = -1;
}

// Static callback method
void Sound::midiDriverCallback(void *data) {
	Sound *s = (Sound *)data;
	for (int i = 0; i < NUM_MIDI_SLOTS; i++)
		s->_midiSlots[i].midiParser->onTimer();

	// TODO: put this somewhere other than the midi callback...
	if (s->_playingSpeech && !s->_vm->_system->getMixer()->isSoundHandleActive(s->_speechHandle)) {
		s->stopPlayingSpeech();
		s->_vm->_finishedPlayingSpeech = true;
	}
}

} // End of namespace StarTrek
