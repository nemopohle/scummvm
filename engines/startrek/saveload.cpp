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

#include "graphics/thumbnail.h"

#include "common/file.h"
#include "common/savefile.h"
#include "common/serializer.h"

#include "startrek/bridge.h"
#include "startrek/resource.h"
#include "startrek/room.h"
#include "startrek/startrek.h"

namespace StarTrek {

bool StarTrekEngine::hasFeature(EngineFeature f) const {
	return
	    (f == kSupportsReturnToLauncher) ||
	    (f == kSupportsLoadingDuringRuntime) ||
	    (f == kSupportsSavingDuringRuntime);
}

bool StarTrekEngine::canLoadGameStateCurrently(Common::U32String *msg) {
	return _canSaveLoad;
}

bool StarTrekEngine::canSaveGameStateCurrently(Common::U32String *msg) {
	return _canSaveLoad;
}

Common::Error StarTrekEngine::loadGameState(int slot) {
	// The main loop loads the game after leaving the current game mode
	_pendingLoadSlot = slot;
	_resetGameMode = true;
	return Common::kNoError;
}

Common::Error StarTrekEngine::saveGameState(int slot, const Common::String &desc, bool isAutosave) {
	return saveGame(slot, desc) ? Common::kNoError : Common::kWritingFailed;
}

// The options menus can always save and load
bool StarTrekEngine::showSaveMenu() {
	_canSaveLoad = true;
	bool result = originalSaveLoad() ? showOriginalSaveMenu() : saveGameDialog();
	_canSaveLoad = false;
	return result;
}

bool StarTrekEngine::showLoadMenu() {
	_canSaveLoad = true;
	bool result = originalSaveLoad() ? showOriginalLoadMenu() : loadGameDialog();
	_canSaveLoad = false;
	return result;
}

struct SavedGamesMenu {
	const char *header;
	const SaveStateList *saves;
};

// The original lists the newest saved game first
static SaveStateList listSavedGames(const MetaEngine *metaEngine, const Common::String &target, bool withAutosave) {
	SaveStateList saves = metaEngine->listSaves(target.c_str());
	SaveStateList result;
	for (int i = saves.size() - 1; i >= 0; i--) {
		if (withAutosave || saves[i].getSaveSlot() != metaEngine->getAutosaveSlot())
			result.push_back(saves[i]);
	}
	return result;
}

Common::String StarTrekEngine::readTextFromSaveList(int choiceIndex, uintptr data, Common::String *headerTextOutput) {
	const SavedGamesMenu *menu = (const SavedGamesMenu *)data;
	int numSaves = menu->saves->size();

	*headerTextOutput = centerTextboxHeader(localizeText(menu->header));
	if (choiceIndex < numSaves)
		return localizeText("Description:\n") + (*menu->saves)[choiceIndex].getDescription();
	if (choiceIndex > numSaves)
		return Common::String();
	if (numSaves != 0)
		return localizeText("Cancel.");
	if (getPlatform() == Common::kPlatformAmiga && getLanguage() == Common::EN_ANY)
		return "There are no saved games.";
	return localizeText("#GENE\\GENERJ01#No games saved.");
}

int StarTrekEngine::chooseSavedGame(const char *header, const SaveStateList &saves) {
	SavedGamesMenu menu = { header, &saves };
	int index = showText(&StarTrekEngine::readTextFromSaveList, (uintptr)&menu, 20, 20, TEXTCOLOR_YELLOW, true, 0, true);
	return index < (int)saves.size() ? index : -1;
}

bool StarTrekEngine::showOriginalSaveMenu() {
	static const char *const options[] = {
		"Save Game",
		"#GENE\\GENER023#Save new game.",
		"#GENE\\GENER019#Replace previous game.",
		"#GENE\\GENER002#Delete previous game.",
		"#GENE\\GENER000#Cancel.",
		""
	};
	const MetaEngine *metaEngine = getMetaEngine();

	while (!shouldQuit()) {
		int choice = showText(&StarTrekEngine::readTextFromArray, (uintptr)options, 20, 20, TEXTCOLOR_YELLOW, true, 0, true);
		SaveStateList saves = listSavedGames(metaEngine, _targetName, false);

		if (choice == 0) {
			// A new game gets the slot after the newest one
			int slot = saves.empty() ? 1 : saves[0].getSaveSlot() + 1;
			for (int i = 1; slot > metaEngine->getMaximumSaveSlot() && i <= metaEngine->getMaximumSaveSlot(); i++) {
				if (!metaEngine->querySaveMetaInfos(_targetName.c_str(), i).isValid() && i != metaEngine->getAutosaveSlot())
					slot = i;
			}
			if (slot > metaEngine->getMaximumSaveSlot()) {
				showTextbox("", localizeText("#GENE\\GENER024#Too many games saved.  Replace or delete a previous game."), 20, 20, TEXTCOLOR_YELLOW, 0);
				continue;
			}

			memset(_textInputBuffer, 0, TEXT_INPUT_BUFFER_SIZE - 1);
			Common::String desc = showTextInputBox(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, localizeText("Description:\n                    "));
			while (desc.hasPrefix(" "))
				desc.deleteChar(0);
			return !desc.empty() && saveGameState(slot, desc).getCode() == Common::kNoError;
		} else if (choice == 1) {
			int index = chooseSavedGame("Save Game", saves);
			if (index < 0)
				return false;
			return saveGameState(saves[index].getSaveSlot(), saves[index].getDescription()).getCode() == Common::kNoError;
		} else if (choice == 2) {
			int index = chooseSavedGame("Delete Game", saves);
			if (index >= 0)
				metaEngine->removeSaveState(_targetName.c_str(), saves[index].getSaveSlot());
		} else {
			return false;
		}
	}
	return false;
}

bool StarTrekEngine::showOriginalLoadMenu() {
	SaveStateList saves = listSavedGames(getMetaEngine(), _targetName, true);
	int index = chooseSavedGame("Load Game", saves);
	return index >= 0 && loadGameState(saves[index].getSaveSlot()).getCode() == Common::kNoError;
}

// F5 opens the ScummVM menu, like in other engines, or the options of the game
void StarTrekEngine::showScummVMMenu() {
	_canSaveLoad = !(_gameDescription->desc.flags & ADGF_DEMO);
	openMainMenuDialog();
	_canSaveLoad = false;
}

bool StarTrekEngine::popMainLoopEvent(TrekEvent *e, bool canSaveLoad) {
	_canSaveLoad = canSaveLoad && !(_gameDescription->desc.flags & ADGF_DEMO);
	bool result = popNextEvent(e);
	_canSaveLoad = false;
	// Nothing else happens after loading a game
	return result && !_resetGameMode;
}

const uint32 CURRENT_SAVEGAME_VERSION = 5;

bool StarTrekEngine::saveGame(int slot, Common::String desc) {
	Common::String filename = getSavegameFilename(slot);
	Common::OutSaveFile *out;

	if (!(out = _saveFileMan->openForSaving(filename))) {
		warning("Can't create file '%s', game not saved", filename.c_str());
		return false;
	} else {
		debug(3, "Successfully opened %s for writing", filename.c_str());
	}

	SavegameMetadata meta;
	meta.version = CURRENT_SAVEGAME_VERSION;
	memset(meta.description, 0, sizeof(meta.description));
	strncpy(meta.description, desc.c_str(), SAVEGAME_DESCRIPTION_LEN);

	TimeDate curTime;
	_system->getTimeAndDate(curTime);
	meta.setSaveTimeAndDate(curTime);
	meta.playTime = g_engine->getTotalPlayTime();

	if (!saveOrLoadMetadata(nullptr, out, &meta)) {
		delete out;
		return false;
	}
	if (!saveOrLoadGameData(nullptr, out, &meta)) {
		delete out;
		return false;
	}

	out->finalize();
	delete out;
	return true;
}

bool StarTrekEngine::loadGame(int slot) {
	Common::String filename = getSavegameFilename(slot);
	Common::InSaveFile *in;

	if (!(in = _saveFileMan->openForLoading(filename))) {
		warning("Can't open file '%s', game not loaded", filename.c_str());
		return false;
	} else {
		debug(3, "Successfully opened %s for loading", filename.c_str());
	}

	SavegameMetadata meta;
	if (!saveOrLoadMetadata(in, nullptr, &meta)) {
		delete in;
		return false;
	}

	if (meta.version > CURRENT_SAVEGAME_VERSION) {
		delete in;
		error("Savegame version (%u) is newer than current version (%u). A newer version of ScummVM is needed", meta.version, CURRENT_SAVEGAME_VERSION);
	}

	if (!saveOrLoadGameData(in, nullptr, &meta)) {
		delete in;
		return false;
	}

	delete in;

	_lastGameMode = _gameMode;

	if (_gameMode == GAMEMODE_AWAYMISSION) {
		for (int i = 0; i < NUM_ACTORS; i++) {
			Actor *a = &_actorList[i];
			if (a->spriteDrawn) {
				if (a->animType != 1)
					a->animFile = SharedPtr<Common::MemoryReadStreamEndian>(_resource->loadFile(a->animFilename + ".anm"));
				_gfx->addSprite(&a->sprite);
				a->sprite.setBitmap(loadAnimationFrame(a->bitmapFilename, a->scale));
			}
		}
	} else if (_gameMode == -1) {
		initBridge(true);
		_lastGameMode = GAMEMODE_BRIDGE;
	} else {
		_missionName = _missionToLoad;
		setBridgeTexts();

		// The bridge loads its own music, but the game continues with the saved one
		Common::String music = _sound->_loadedMidiFilename;
		MidiTracks track = _sound->_loopingMidiTrack;
		initBridge(false);
		if (!music.empty()) {
			_sound->loadMusicFile(music);
			_sound->_loopingMidiTrack = track;
			_sound->playMidiTrackInSlot(0, track);
		}
	}

	return true;
}

bool StarTrekEngine::saveOrLoadGameData(Common::SeekableReadStream *in, Common::WriteStream *out, SavegameMetadata *meta) {
	Common::Serializer ser(in, out);

	if (ser.isLoading()) {
		if (_lastGameMode == GAMEMODE_BRIDGE)
			cleanupBridge();
		else // Assume GAMEMODE_AWAYMISSION
			unloadRoom();
	}

	ser.syncAsUint16LE(_gameMode);

	Common::String midiFilename = _sound->_loadedMidiFilename;
	ser.syncString(midiFilename);
	int16 loopingMidiTrack = _sound->_loopingMidiTrack;
	ser.syncAsSint16LE(loopingMidiTrack);

	if (ser.isLoading()) {
		if (midiFilename.empty())
			_sound->clearAllMidiSlots();
		else {
			_sound->loadMusicFile(midiFilename);
			_sound->_loopingMidiTrack = (MidiTracks)loopingMidiTrack;
			_sound->playMidiTrackInSlot(0, _sound->_loopingMidiTrack);
		}
	}

	ser.syncAsUint16LE(_frameIndex);
	ser.syncAsUint16LE(_mouseControllingShip);
	if (meta->version >= 1) {
		ser.syncAsSint16LE(_enterpriseState.inOrbit);
		ser.syncAsSint16LE(_enterpriseState.underAttack);
		ser.syncAsSint16LE(_randomEncounterType);

		ser.syncAsSint16LE(_randomEncounterCountdown);
		// Older savegames have no countdown, and the hail was shown already
		if (ser.isLoading() && meta->version < 4)
			_randomEncounterCountdown = -1;
		ser.syncAsSint32LE(_missionTicks);
		ser.syncAsSint16LE(_currentPlanet);
		ser.syncAsSint16LE(_targetPlanet);
		ser.syncAsSint16LE(_viewDirection);
	}

	ser.syncString(_missionToLoad);

	if (meta->version >= 1) {
		int16 emergencyPowerUsed = _emergencyPowerUsed;
		ser.syncAsSint16LE(emergencyPowerUsed);
		_emergencyPowerUsed = emergencyPowerUsed != 0;

		uint16 hailedTarget = 0;
		ser.syncAsUint16LE(hailedTarget);
		ser.syncAsSint16LE(_lastMissionId);
		for (int i = 0; i < 7; i++) {
			ser.syncAsUint16LE(_missionPoints[i]);
		}
	}

	ser.syncString(_sound->_loopingAudioName);

	if (ser.isLoading()) {
		if (!_sound->_loopingAudioName.empty())
			_sound->playVoc(_sound->_loopingAudioName);
	}

	if (meta->version >= 1) {
		int16 bridgeSequence = -1;
		ser.syncAsSint16LE(bridgeSequence);
	}

	for (int i = 0; i < NUM_OBJECTS; i++) {
		ser.syncAsByte(_itemList[i].have);
	}

	if (_gameMode == GAMEMODE_AWAYMISSION) {
		ser.syncString(_missionName);
		ser.syncAsSint16LE(_roomIndex);

		if (ser.isLoading()) {
			_gfx->fadeoutScreen();
			_resource->setTxtFileName("ground");

			// This must be done before loading the actor variables, since this clears
			// them.
			loadRoom(_missionName, _roomIndex);
		}

		ser.syncAsUint32LE(_roomFrameCounter);
		ser.syncAsUint32LE(_frameIndex); // FIXME: redundant

		byte filler = 0;

		// Serialize the "actor" class
		for (int i = 0; i < NUM_ACTORS; i++) {
			Actor *a = &_actorList[i];
			ser.syncAsUint16LE(a->spriteDrawn);
			ser.syncString(a->animFilename);
			if (a->animFilename.size() < 15) {
				filler = 0;
				for (uint j = 0; j < 16 - a->animFilename.size() - 1; ++j)
					ser.syncAsByte(filler);	// make sure that exactly 16 bytes are synced
			}
			a->animFilename.trim();

			ser.syncAsUint16LE(a->animType);

			a->sprite.saveLoadWithSerializer(ser);

			ser.syncString(a->bitmapFilename);
			if (a->bitmapFilename.size() < 9) {
				filler = 0;
				for (uint j = 0; j < 10 - a->bitmapFilename.size() - 1; ++j)
					ser.syncAsByte(filler);	// make sure that exactly 10 bytes are synced
			}
			a->bitmapFilename.trim();

			a->scale.saveLoadWithSerializer(ser);
			// Can't save "animFile" (will be reloaded)
			ser.syncAsUint16LE(a->numAnimFrames);
			ser.syncAsUint16LE(a->animFrame);
			ser.syncAsUint32LE(a->frameToStartNextAnim);
			ser.syncAsSint16LE(a->pos.x);
			ser.syncAsSint16LE(a->pos.y);
			ser.syncAsUint16LE(a->field60);
			ser.syncAsUint16LE(a->field62);
			ser.syncAsUint16LE(a->triggerActionWhenAnimFinished);
			ser.syncAsUint16LE(a->finishedAnimActionParam);
			ser.syncString(a->animationString2);
			if (a->animationString2.size() < 7) {
				filler = 0;
				for (uint j = 0; j < 8 - a->animationString2.size() - 1; ++j)
					ser.syncAsByte(filler);	// make sure that exactly 8 bytes are synced
			}
			ser.syncAsUint16LE(a->field70);
			ser.syncAsUint16LE(a->field72);
			ser.syncAsUint16LE(a->field74);
			ser.syncAsUint16LE(a->field76);
			ser.syncAsSint16LE(a->iwSrcPosition);
			ser.syncAsSint16LE(a->iwDestPosition);
			a->granularPosX.saveLoadWithSerializer(ser);
			a->granularPosY.saveLoadWithSerializer(ser);
			a->speedX.saveLoadWithSerializer(ser);
			a->speedY.saveLoadWithSerializer(ser);
			ser.syncAsSint16LE(a->dest.x);
			ser.syncAsSint16LE(a->dest.y);
			ser.syncAsUint16LE(a->field90);
			ser.syncAsByte(a->field92);
			ser.syncAsByte(a->direction);
			ser.syncAsUint16LE(a->field94);
			ser.syncAsUint16LE(a->field96);
			ser.syncString(a->animationString);
			if (a->animationString.size() < 9) {
				filler = 0;
				for (uint j = 0; j < 10 - a->animationString.size() - 1; ++j)
					ser.syncAsByte(filler);	// make sure that exactly 10 bytes are synced
			}
			ser.syncAsUint16LE(a->fielda2);
			ser.syncAsUint16LE(a->fielda4);
			ser.syncAsUint16LE(a->fielda6);
		}

		Common::String unused = getScreenName();
		ser.syncString(unused);

		// Away mission struct
		for (int i = 0; i < 8; i++)
			ser.syncAsSint16LE(_awayMission.timers[i]);
		ser.syncAsSint16LE(_awayMission.mouseX);
		ser.syncAsSint16LE(_awayMission.mouseY);
		for (int i = 0; i < 4; i++)
			ser.syncAsSint16LE(_awayMission.crewGetupTimers[i]);
		ser.syncAsByte(_awayMission.disableWalking);
		ser.syncAsByte(_awayMission.disableInput);
		ser.syncAsByte(_awayMission.redshirtDead);
		ser.syncAsByte(_awayMission.activeAction);
		ser.syncAsByte(_awayMission.activeObject);
		ser.syncAsByte(_awayMission.passiveObject);
		ser.syncAsByte(_awayMission.rdfStillDoDefaultAction);
		ser.syncAsByte(_awayMission.crewDownBitset);
		for (int i = 0; i < 4; i++)
			ser.syncAsByte(_awayMission.crewDirectionsAfterWalk[i]);

		if (_missionName == "DEMON") {
			_awayMission.demon.saveLoadWithSerializer(ser);
			_room->_roomVar.demon.saveLoadWithSerializer(ser);
		} else if (_missionName == "TUG") {
			_awayMission.tug.saveLoadWithSerializer(ser);
			_room->_roomVar.tug.saveLoadWithSerializer(ser);
		} else if (_missionName == "LOVE") {
			_awayMission.love.saveLoadWithSerializer(ser);
			_room->_roomVar.love.saveLoadWithSerializer(ser);
		} else if (_missionName == "MUDD") {
			_awayMission.mudd.saveLoadWithSerializer(ser);
			_room->_roomVar.mudd.saveLoadWithSerializer(ser);
		} else if (_missionName == "FEATHER") {
			_awayMission.feather.saveLoadWithSerializer(ser);
			_room->_roomVar.feather.saveLoadWithSerializer(ser);
		} else if (_missionName == "TRIAL") {
			_awayMission.trial.saveLoadWithSerializer(ser);
			_room->_roomVar.trial.saveLoadWithSerializer(ser);
		} else if (_missionName == "SINS") {
			_awayMission.sins.saveLoadWithSerializer(ser);
			_room->_roomVar.sins.saveLoadWithSerializer(ser);
		} else if (_missionName == "VENG") {
			_awayMission.veng.saveLoadWithSerializer(ser);
			_room->_roomVar.veng.saveLoadWithSerializer(ser);
		}

		// The action queue
		if (ser.isLoading()) {
			_actionQueue = Common::Queue<Action>();
			int16 n = 0;
			ser.syncAsSint16LE(n);
			for (int i = 0; i < n; i++) {
				Action a;
				a.saveLoadWithSerializer(ser);
				_actionQueue.push(a);
			}
		} else { // Saving
			int16 n = _actionQueue.size();
			ser.syncAsSint16LE(n);
			for (int i = 0; i < n; i++) {
				Action a = _actionQueue.pop();
				a.saveLoadWithSerializer(ser);
				_actionQueue.push(a);
			}
		}

		// Original game located changes in RDF files and saved them. Since RDF files
		// aren't modified directly here, that's skipped.

		ser.syncAsSint16LE(_objectHasWalkPosition);
		ser.syncAsSint16LE(_objectWalkPosition.x);
		ser.syncAsSint16LE(_objectWalkPosition.y);

		for (int i = 0; i < MAX_BUFFERED_WALK_ACTIONS; i++) {
			_actionOnWalkCompletion[i].saveLoadWithSerializer(ser);
			ser.syncAsByte(_actionOnWalkCompletionInUse[i]);
		}

		ser.syncAsSint16LE(_warpHotspotsActive);
	}

	if (meta->version >= 2) {
		syncSpaceState(ser);
	} else if (ser.isLoading()) {
		// Older savegames have no ships in space
		for (int i = 0; i < NUM_ENEMY_SHIPS; i++) {
			if (_enemyShips[i].field1e != 0)
				delR3(&_enemyShips[i]);
			_enemyShips[i] = R3();
		}
		removeProjectiles();
		_targetShip = nullptr;
	}

	// The frame index above has only 16 bits, but the weapons wait for frames with 32 bits
	if (meta->version >= 5) {
		ser.syncAsUint32LE(_frameIndex);
	} else if (ser.isLoading()) {
		for (int i = -1; i < NUM_ENEMY_SHIPS; i++) {
			R3 &ship = i < 0 ? _enterpriseR3 : _enemyShips[i];
			for (int j = 0; j < ARRAYSIZE(ship.weapons); j++)
				ship.weapons[j].nextFireTime = MIN<uint32>(ship.weapons[j].nextFireTime, _frameIndex);
		}
	}

	if (meta->version >= 3) {
		ser.syncAsSint16LE(_missionState);
		ser.syncAsSint32LE(_missionTicks);
		for (int i = 0; i < ARRAYSIZE(_missionVars); i++)
			ser.syncAsSint16LE(_missionVars[i]);
		ser.syncAsSint16LE(_missionScore);
		ser.syncAsSint16LE(_missionMaxScore);
		ser.syncAsSint16LE(_missionEndFlag);

		if (ser.isLoading()) {
			delete _orbitObjectR3.bitmap;
			_orbitObjectR3.bitmap = nullptr;
		}
		syncR3(ser, _orbitObjectR3);
		if (ser.isLoading()) {
			// Older savegames may contain an uninitialized orbit object
			if (_orbitObjectR3.field1e != 3 || _orbitObjectR3.field20 < 4 || _orbitObjectR3.field20 > 7)
				_orbitObjectR3 = R3();
			else
				addOrbitObject();
		}
	} else if (ser.isLoading()) {
		// Older savegames only know the mission, so continue with its away mission
		static const struct {
			const char *mission;
			int16 state;
		} kMissionStates[] = {
			{ "DEMON", kMissionDemonPollux },
			{ "TUG", kMissionTugMasada },
			{ "LOVE", kMissionLoveArk7 },
			{ "MUDD", kMissionMuddDerelict },
			{ "FEATHER", kMissionFeatherOrbit },
			{ "TRIAL", kMissionTrialStart },
			{ "SINS", kMissionSinsAlphaProxima },
			{ "VENG", kMissionVengStart }
		};
		_missionState = kMissionNone;
		_missionTicks = 1000;
		for (int i = 0; i < ARRAYSIZE(kMissionStates); i++) {
			if (_missionToLoad.equalsIgnoreCase(kMissionStates[i].mission))
				_missionState = kMissionStates[i].state;
		}
	}

	return true;
}

static void syncBool(Common::Serializer &ser, bool &value) {
	byte b = value;
	ser.syncAsByte(b);
	value = b != 0;
}

static void syncPoint(Common::Serializer &ser, Point3 &point) {
	for (int i = 0; i < 3; i++)
		ser.syncAsSint32LE(point[i]);
}

static void syncMatrix(Common::Serializer &ser, Matrix &matrix) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			int16 value = matrix[i][j].raw();
			ser.syncAsSint16LE(value);
			matrix[i][j] = Fixed14::fromRaw(value);
		}
	}
}

void StarTrekEngine::syncR3(Common::Serializer &ser, R3 &r3) {
	syncPoint(ser, r3.pos);
	syncMatrix(ser, r3.matrix);
	ser.syncAsSint16LE(r3.field1e);
	ser.syncAsSint16LE(r3.field20);
	ser.syncAsSint16LE(r3.field22);
	ser.syncAsSint16LE(r3.field24);
	for (int i = 0; i < 3; i++)
		ser.syncAsSint16LE(r3.speed[i]);

	for (int i = 0; i < ARRAYSIZE(r3.systems); i++) {
		ser.syncAsSint16LE(r3.systems[i].present);
		ser.syncAsSint16LE(r3.systems[i].maxStrength);
		ser.syncAsSint16LE(r3.systems[i].strength);
	}
	ser.syncAsSint32LE(r3.maxTurnRate);
	ser.syncAsSint32LE(r3.turnAcceleration);
	ser.syncAsSint32LE(r3.maxSpeed);
	ser.syncAsSint32LE(r3.speedAcceleration);
	ser.syncAsSint32LE(r3.commandedSpeed);
	syncPoint(ser, r3.targetTurnRate);
	ser.syncAsSint32LE(r3.targetSpeed);
	ser.syncAsSint32LE(r3.currentSpeed);
	syncPoint(ser, r3.turnRate);
	syncPoint(ser, r3.turnAngle);
	ser.syncAsSint16LE(r3.field140);
	ser.syncAsSint16LE(r3.energy);
	syncBool(ser, r3.shieldsOn);
	syncBool(ser, r3.shieldsActive);
	syncBool(ser, r3.weaponsOn);
	syncBool(ser, r3.weaponsActive);

	ser.syncAsSint32LE(r3.enterpriseDistance);
	ser.syncAsSint32LE(r3.enterpriseDistanceChange);
	syncBool(ser, r3.maneuverActive);
	ser.syncAsSint16LE(r3.maneuver);
	ser.syncAsSint16LE(r3.maneuverState);
	for (int i = 0; i < ARRAYSIZE(r3.weapons); i++) {
		ShipWeapon &weapon = r3.weapons[i];
		ser.syncAsUint32LE(weapon.nextFireTime);
		ser.syncAsSint16LE(weapon.lifetime);
		ser.syncAsSint16LE(weapon.speed);
		ser.syncAsSint16LE(weapon.cooldown);
		syncPoint(ser, weapon.mount);
		ser.syncAsSint16LE(weapon.shape);
	}
	ser.syncAsSint32LE(r3.engageRange);
	syncBool(ser, r3.canCloak);
	syncBool(ser, r3.cloakRequested);
	syncBool(ser, r3.cloakActive);
	ser.syncAsSint16LE(r3.cloakLevel);
	syncBool(ser, r3.exploding);
	ser.syncAsSint16LE(r3.explosionFrame);
	syncBool(ser, r3.retreating);

	ser.syncAsSint16LE(r3.planetSize);
	ser.syncString(r3.planetName);
	ser.syncAsUint16LE(r3.planetSeed);
	ser.syncAsSint16LE(r3.planetColors);
	ser.syncAsSint16LE(r3.projectileLife);
	ser.syncAsSint16LE(r3.projectileDamage);
	ser.syncAsSint16LE(r3.projectileColor);
}

// Ships are saved as -1 for none, 0 for the Enterprise and 1 to 4 for the enemies
int16 StarTrekEngine::getShipIndex(const R3 *ship) const {
	if (ship == &_enterpriseR3)
		return 0;
	for (int i = 0; i < NUM_ENEMY_SHIPS; i++) {
		if (ship == &_enemyShips[i])
			return i + 1;
	}
	return -1;
}

R3 *StarTrekEngine::getShipByIndex(int16 index) {
	if (index == 0)
		return &_enterpriseR3;
	if (index >= 1 && index <= NUM_ENEMY_SHIPS)
		return &_enemyShips[index - 1];
	return nullptr;
}

void StarTrekEngine::syncSpaceState(Common::Serializer &ser) {
	ser.syncAsUint32LE(_msRandState);
	syncPoint(ser, _starfieldPosition);
	syncMatrix(ser, _someMatrix);
	ser.syncAsSint16LE(_viewDirection);
	ser.syncAsSint16LE(_viewscreenZoom);
	syncBool(ser, _enterpriseState.shields);
	syncBool(ser, _enterpriseState.weapons);
	syncBool(ser, _enterpriseState.targetAnalysis);
	syncBool(ser, _emergencyPowerUsed);
	ser.syncAsSint32LE(_emergencyPowerTicks);
	ser.syncAsSint16LE(_emergencyPowerRate);
	ser.syncAsSint16LE(_emergencyPowerAmount);
	ser.syncAsSint16LE(_emergencyPowerDuration);
	ser.syncAsSint16LE(_repairPriority);

	syncR3(ser, _enterpriseR3);
	syncR3(ser, _planetR3);
	for (int i = 0; i < NUM_ENEMY_SHIPS; i++)
		syncR3(ser, _enemyShips[i]);
	for (int i = 0; i < NUM_PROJECTILES; i++) {
		syncR3(ser, _projectiles[i]);
		int16 owner = getShipIndex(_projectiles[i].projectileOwner);
		ser.syncAsSint16LE(owner);
		_projectiles[i].projectileOwner = getShipByIndex(owner);
	}
	int16 target = getShipIndex(_targetShip);
	ser.syncAsSint16LE(target);
	_targetShip = getShipByIndex(target);

	if (ser.isLoading()) {
		// Rebuild the list of objects in space
		Common::fill(_r3List, _r3List + NUM_SPACE_OBJECTS, (R3 *)nullptr);
		addR3(&_enterpriseR3);
		addR3(&_planetR3);
		for (int i = 0; i < NUM_ENEMY_SHIPS; i++) {
			if (_enemyShips[i].field1e != 0)
				addR3(&_enemyShips[i]);
		}
		for (int i = 0; i < NUM_PROJECTILES; i++) {
			if (_projectiles[i].field1e != 0)
				addR3(&_projectiles[i]);
		}
	}
}

Common::String StarTrekEngine::getSavegameFilename(int slotId) const {
	Common::String saveLoadSlot = _targetName;
	saveLoadSlot += Common::String::format(".%.3d", slotId);
	return saveLoadSlot;
}


// Static function (reused in detection.cpp)
bool saveOrLoadMetadata(Common::SeekableReadStream *in, Common::WriteStream *out, SavegameMetadata *meta) {
	Common::Serializer ser(in, out);

	ser.syncAsUint32LE(meta->version);
	ser.syncBytes((byte *)meta->description, SAVEGAME_DESCRIPTION_LEN + 1);

	// Thumbnail
	if (ser.isLoading()) {
		if (!::Graphics::loadThumbnail(*in, meta->thumbnail))
			meta->thumbnail = nullptr;
	} else
		::Graphics::saveThumbnail(*out);

	// Creation date/time
	ser.syncAsUint32LE(meta->saveDate);
	debugC(5, kDebugSavegame, "Save date: %d", meta->saveDate);
	ser.syncAsUint16LE(meta->saveTime);
	debugC(5, kDebugSavegame, "Save time: %d", meta->saveTime);
	ser.syncAsByte(meta->saveTimeSecs); // write seconds of save time as well
	ser.syncAsUint32LE(meta->playTime);
	debugC(5, kDebugSavegame, "Play time: %d", meta->playTime);

	return true;
}

} // End of namespace StarTrek
