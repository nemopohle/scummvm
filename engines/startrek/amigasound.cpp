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

#include "common/file.h"
#include "common/memstream.h"

#include "audio/audiostream.h"
#include "audio/decoders/raw.h"
#include "audio/mods/protracker.h"

#include "startrek/resource.h"
#include "startrek/sound.h"

namespace StarTrek {

// The Amiga version replaces the tracks of the MIDI files with songs and sound effects.
// A positive value is a song, a negative one a sound effect.
static const int8 kAmigaTitleTracks[] = { 1 };
static const int8 kAmigaBridgeTracks[] = {
	-19, -15, 6, 9, 10, -23, -25, -17, -4, -26, -41, 20, 17, 15, 19, 3, -3, 5
};
static const int8 kAmigaBridgeWTracks[] = {
	-19, -15, 7, 9, 10, -23, -25, -17, -4, -26, -41, 20, 17, 15, 19, 3, -3, 5
};
static const int8 kAmigaBridgeBTracks[] = {
	-19, -15, 8, 9, 10, -23, -25, -17, -4, -26, -41, 20, 17, 15, 19, 3, -3, 5
};
static const int8 kAmigaGroundTracks[] = {
	2, 3, 4, 5, -27, -30, -14, -4, -35, -37, -36, 0, -2, -1, -34, -9, -5, -22, -1,
	-18, -10, -40, -12, 0, 13, 14, 15, 16, 17, 18, 19, 11, 12, -34, -3, -20, -8
};

static const struct {
	const char *name;
	const int8 *tracks;
	int count;
} kAmigaMusicFiles[] = {
	{ "title", kAmigaTitleTracks, ARRAYSIZE(kAmigaTitleTracks) },
	{ "bridge", kAmigaBridgeTracks, ARRAYSIZE(kAmigaBridgeTracks) },
	{ "bridgew", kAmigaBridgeWTracks, ARRAYSIZE(kAmigaBridgeWTracks) },
	{ "bridgeb", kAmigaBridgeBTracks, ARRAYSIZE(kAmigaBridgeBTracks) },
	{ "ground", kAmigaGroundTracks, ARRAYSIZE(kAmigaGroundTracks) }
};

// The sample bank of each song
static const char kAmigaSongBanks[] = "0daaaabbbaabbbbbbbbbc";

// Period and duration (in frames) of the sound effects, from SND.BIN
static const struct {
	uint16 period;
	uint16 duration;
} kAmigaSoundEffects[] = {
	{ 0x153, 0x36 }, { 0x1ac, 0x57 }, { 0x140, 0x30 }, { 0x17d, 0x0f }, { 0x17d, 0x19 },
	{ 0x17d, 0x16 }, { 0x17d, 0x12 }, { 0x17d, 0x0f }, { 0x140, 0x3d }, { 0x140, 0x1388 },
	{ 0x1ac, 0x7c }, { 0x1ac, 0xb5 }, { 0x1ac, 0x42 }, { 0x1ac, 0x76 }, { 0x140, 0x2f },
	{ 0x140, 0x36 }, { 0x23a, 0x79 }, { 0x23a, 0x75 }, { 0x1ac, 0x19e4 }, { 0x140, 0x1d64 },
	{ 0x17d, 0x48 }, { 0x17d, 0x3b }, { 0x17d, 0x48 }, { 0x17d, 0x5a }, { 0x17d, 0x28 },
	{ 0x17d, 0x43 }, { 0x140, 0x32 }, { 0x140, 0x31 }, { 0x140, 0x2c }, { 0x140, 0x39 },
	{ 0x140, 0x2c }, { 0x140, 0x24 }, { 0x140, 0x1b }, { 0x140, 0x1d }, { 0x23a, 0x136 },
	{ 0x1ac, 0x10d }, { 0x140, 0x4a }, { 0x1ac, 0x100 }, { 0x140, 0x50 }, { 0x140, 0x2e },
	{ 0x140, 0x5c }, { 0x17d, 0x7c }
};

static const int kAmigaClock = 3546895; // PAL

// ProTracker periods of the notes C-1 to B-3
static const uint16 kPeriods[] = {
	0, 856, 808, 762, 720, 678, 640, 604, 570, 538, 508, 480, 453,
	428, 404, 381, 360, 340, 320, 302, 285, 269, 254, 240, 226,
	214, 202, 190, 180, 170, 160, 151, 143, 135, 127, 120, 113
};

static void writeP41AEvent(byte *dest, byte c1, byte c2, byte c3) {
	uint note = (c1 & 0x7f) / 2;
	if (note >= ARRAYSIZE(kPeriods)) {
		memset(dest, 0, 4);
		return;
	}
	uint16 period = kPeriods[note];
	if ((c2 & 0x0f) == 0x08)
		c2 -= 0x08;
	byte effect = c2 & 0x0f;
	if ((effect == 0x05 || effect == 0x06 || effect == 0x0a) && c3 > 0x7f)
		c3 = (c3 << 4) & 0xf0;
	dest[0] = ((c1 << 4) & 0x10) | (period >> 8);
	dest[1] = period & 0xff;
	dest[2] = c2;
	dest[3] = c3;
}

// Unpacks a run-length coded track of "The Player 4.1A" into 64 events of a pattern
static void readP41ATrack(const byte *data, uint32 size, uint32 trackData, uint32 pos, byte *pattern, int channel) {
	int row = 0;

	// Writes an event and its repetitions, returns the last row written
	auto put = [&](const byte *ev) {
		writeP41AEvent(pattern + row * 16 + channel * 4, ev[0], ev[1], ev[2]);
		byte count = ev[3];
		if (count > 0 && count < 0x80) {
			row += count;
		} else if (count >= 0x80) {
			for (int i = 0; i < 256 - count && row < 63; i++) {
				row++;
				writeP41AEvent(pattern + row * 16 + channel * 4, ev[0], ev[1], ev[2]);
			}
		}
		row++;
	};

	// A track can end with a pattern break before row 64
	while (row < 64 && pos + 4 <= size) {
		const byte *ev = data + pos;
		pos += 4;
		if (ev[0] != 0x80) {
			put(ev);
			continue;
		}
		// Repeats events from an earlier place
		uint32 from = trackData + (ev[2] << 8) + ev[3];
		for (int i = 0; i <= ev[1] && row < 64 && from + 4 <= size; i++, from += 4)
			put(data + from);
	}
}

// Converts a song of "The Player 4.1A" to a ProTracker module. Its samples are in a bank.
static Common::MemoryReadStream *convertP41A(const byte *data, uint32 size, const byte *bank, uint32 bankSize) {
	if (size < 20 || READ_BE_UINT32(data) != MKTAG('P', '4', '1', 'A'))
		return nullptr;

	byte length = data[5];
	byte numSamples = MIN<byte>(data[6], 31);
	uint32 trackData = READ_BE_UINT32(data + 8) + 4;
	uint32 trackTable = READ_BE_UINT32(data + 12) + 4;

	uint32 samplesSize = 0;
	for (int i = 0; i < numSamples; i++)
		samplesSize += READ_BE_UINT16(data + 20 + i * 16 + 4) * 2;

	uint32 modSize = 1084 + length * 1024 + samplesSize;
	byte *mod = (byte *)calloc(modSize, 1);
	byte *dest = mod + 20;
	byte *sampleDest = mod + 1084 + length * 1024;

	for (int i = 0; i < 31; i++, dest += 30) {
		if (i >= numSamples) {
			WRITE_BE_UINT16(dest + 28, 1);
			continue;
		}
		const byte *info = data + 20 + i * 16;
		uint32 address = READ_BE_UINT32(info);
		uint16 length16 = READ_BE_UINT16(info + 4);
		uint32 loopAddress = READ_BE_UINT32(info + 6);
		int16 finetune = READ_BE_INT16(info + 14);

		WRITE_BE_UINT16(dest + 22, length16);
		dest[24] = (finetune / 74) & 0x0f;
		dest[25] = info[13];
		WRITE_BE_UINT16(dest + 26, (loopAddress - address) / 2);
		WRITE_BE_UINT16(dest + 28, READ_BE_UINT16(info + 10));

		uint32 sampleSize = length16 * 2;
		if (address < bankSize)
			memcpy(sampleDest, bank + address, MIN(sampleSize, bankSize - address));
		sampleDest += sampleSize;
	}

	// Each position of the song has its own pattern
	mod[950] = length;
	mod[951] = 0x7f;
	for (int i = 0; i < length; i++)
		mod[952 + i] = i;
	WRITE_BE_UINT32(mod + 1080, MKTAG('M', '.', 'K', '.'));

	for (int i = 0; i < length; i++) {
		for (int channel = 0; channel < 4; channel++) {
			uint32 pos = trackTable + i * 8 + channel * 2;
			if (pos + 2 > size)
				break;
			readP41ATrack(data, size, trackData, trackData + READ_BE_UINT16(data + pos), mod + 1084 + i * 1024, channel);
		}
	}

	return new Common::MemoryReadStream(mod, modSize, DisposeAfterUse::YES);
}

void Sound::loadAmigaMusicFile(const Common::String &baseSoundName) {
	_amigaTracks = nullptr;
	_amigaTrackCount = 0;
	for (int i = 0; i < ARRAYSIZE(kAmigaMusicFiles); i++) {
		if (baseSoundName.equalsIgnoreCase(kAmigaMusicFiles[i].name)) {
			_amigaTracks = kAmigaMusicFiles[i].tracks;
			_amigaTrackCount = kAmigaMusicFiles[i].count;
		}
	}
}

void Sound::playAmigaTrack(int track, bool music) {
	if (!_vm->_musicEnabled || track < 0 || track >= _amigaTrackCount)
		return;

	int value = _amigaTracks[track];
	Audio::SoundHandle *handle = music ? &_amigaMusicHandle : nullptr;
	if (value > 0) {
		// Songs always play as music
		_amigaMusicTrack = track;
		playAmigaSong(value);
	} else if (value < 0) {
		if (music)
			_amigaMusicTrack = track;
		playAmigaSoundEffect(-value, handle);
	}
}

void Sound::playAmigaSong(int song) {
	Audio::Mixer *mixer = _vm->_system->getMixer();
	mixer->stopHandle(_amigaMusicHandle);

	if (song <= 0 || song >= (int)strlen(kAmigaSongBanks))
		return;

	char bank = kAmigaSongBanks[song];
	Common::String songName = Common::String::format("TREK%d%c.SNG", song, toupper(bank));
	Common::String bankName = Common::String::format("samples.%c", bank);

	if (_amigaBankName != bankName) {
		Common::File file;
		if (!file.open(Common::Path(bankName))) {
			warning("Could not open the sample bank %s", bankName.c_str());
			return;
		}
		delete[] _amigaBank;
		_amigaBankSize = file.size();
		_amigaBank = new byte[_amigaBankSize];
		file.read(_amigaBank, _amigaBankSize);
		_amigaBankName = bankName;
	}

	Common::MemoryReadStreamEndian *songFile = _vm->_resource->loadFile(songName);
	uint32 size = songFile->size();
	byte *data = new byte[size];
	songFile->read(data, size);
	delete songFile;

	Common::MemoryReadStream *module = convertP41A(data, size, _amigaBank, _amigaBankSize);
	delete[] data;
	if (!module)
		return;

	debugC(5, kDebugSound, "Playing song %s", songName.c_str());
	Audio::AudioStream *stream = Audio::makeProtrackerStream(module);
	delete module;
	if (stream)
		mixer->playStream(Audio::Mixer::kMusicSoundType, &_amigaMusicHandle, stream);
}

void Sound::playAmigaSoundEffect(int effect, Audio::SoundHandle *handle) {
	// Some sound effects have random variations
	if (effect == 3)
		effect += _vm->getRandomWord() % 5;
	else if (effect == 10)
		effect += _vm->getRandomWord() & 3;
	else if (effect == 20 || effect == 30)
		effect += _vm->getRandomWord() % 3;

	if (effect >= ARRAYSIZE(kAmigaSoundEffects))
		return;

	Audio::Mixer *mixer = _vm->_system->getMixer();
	if (!handle) {
		handle = &_amigaSfxHandles[0];
		for (int i = 0; i < MAX_SFX_PLAYING; i++) {
			if (!mixer->isSoundHandleActive(_amigaSfxHandles[i])) {
				handle = &_amigaSfxHandles[i];
				break;
			}
		}
	}
	mixer->stopHandle(*handle);

	Common::MemoryReadStreamEndian *file = _vm->_resource->loadFile(Common::String::format("TREK%d.SFX", effect));
	uint32 size = file->size();
	byte *data = (byte *)malloc(size);
	file->read(data, size);
	delete file;

	debugC(5, kDebugSound, "Playing sound effect %d", effect);
	// The sample repeats until its duration ends
	int rate = kAmigaClock / kAmigaSoundEffects[effect].period;
	Audio::SeekableAudioStream *sample = Audio::makeRawStream(data, size, rate, 0, DisposeAfterUse::YES);
	Audio::AudioStream *looping = Audio::makeLoopingAudioStream(sample, 0);
	Audio::AudioStream *stream = Audio::makeLimitingAudioStream(looping, Audio::Timestamp(kAmigaSoundEffects[effect].duration * 20));
	mixer->playStream(Audio::Mixer::kSFXSoundType, handle, stream);
}

void Sound::stopAmigaSounds(bool music, bool soundEffects) {
	Audio::Mixer *mixer = _vm->_system->getMixer();
	if (music)
		mixer->stopHandle(_amigaMusicHandle);
	if (soundEffects) {
		for (int i = 0; i < MAX_SFX_PLAYING; i++)
			mixer->stopHandle(_amigaSfxHandles[i]);
	}
}

bool Sound::isAmigaMusicPlaying() const {
	return _vm->_system->getMixer()->isSoundHandleActive(_amigaMusicHandle);
}

} // End of namespace StarTrek
