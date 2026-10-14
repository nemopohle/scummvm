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


#include "base/plugins.h"
#include "base/version.h"
#include "common/archive.h"
#include "common/compression/stuffit.h"
#include "common/config-manager.h"
#include "common/debug-channels.h"
#include "common/error.h"
#include "common/events.h"
#include "common/file.h"
#include "common/macresman.h"

#include "graphics/paletteman.h"
#include "graphics/pixelformat.h"
#include "graphics/surface.h"

#include "engines/util.h"
#include "image/pict.h"
#include "video/qt_decoder.h"

#include "startrek/bridge.h"
#include "startrek/console.h"
#include "startrek/iwfile.h"
#include "startrek/lzss.h"
#include "startrek/resource.h"
#include "startrek/room.h"
#include "startrek/startrek.h"

namespace StarTrek {

StarTrekEngine::StarTrekEngine(OSystem *syst, const StarTrekGameDescription *gamedesc) :
	Engine(syst),
	_gameDescription(gamedesc),
	_randomSource("Star Trek"),
	_kirkActor(&_actorList[0]),
	_spockActor(&_actorList[1]),
	_mccoyActor(&_actorList[2]),
	_redshirtActor(&_actorList[3]) {

	if (getGameType() == GType_STJR)
		error("Star Trek: Judgment Rites is not yet supported");

	_gfx = nullptr;
	_activeMenu = nullptr;
	_sound = nullptr;
	_room = nullptr;
	_resource = nullptr;

	memset(_actionOnWalkCompletionInUse, 0, sizeof(_actionOnWalkCompletionInUse));

	_clockTicks = 0;

	_musicEnabled = true;
	_sfxEnabled = true;
	_word_467a6 = true;
	_musicWorking = true;
	_sfxWorking = true;
	_finishedPlayingSpeech = false;

	_lookActionBitmapIndex = 0;

	_mouseControllingShip = false;
	_keyboardControlsMouse = true;

	_resetGameMode = false;
	_pendingLoadSlot = -1;
	_pendingRestart = false;

	_inQuitGameMenu = false;
	_textDisplayMode = TEXTDISPLAY_SUBTITLES;
	_textboxVar2 = 0;
	_textboxVar6 = 0;
	_textboxHasMultipleChoices = false;

	_missionToLoad = "";
	_roomIndexToLoad = 0;
	_mapFile = nullptr;
	_iwFile = nullptr;

	_showSubtitles = true;
	_shipDitheringEnabled = true;
	_viewDirection = 0;
	_viewscreenZoom = 6;
	_viewscreenFullscreen = false;
	_planetSphereMap = nullptr;
	_targetShip = nullptr;
	for (int i = 0; i < NUM_ENEMY_SHIPS; i++)
		_enemyShips[i] = R3();
	for (int i = 0; i < NUM_PROJECTILES; i++)
		_projectiles[i] = R3();
	_msRandState = 1;
	_spaceMessages = 0;
	_cloakLutLevel = -1;
	_bridgeLightPosition = 0;
	_repairPriority = kRepairNone;
	_repairedGroup = kRepairNone;
	_emergencyPowerUsed = false;
	_emergencyPowerTicks = 0;
	_emergencyPowerRate = 0;
	_emergencyPowerAmount = 30;
	_emergencyPowerDuration = 450;
	_restoreFullscreenView = false;
	_heldKey = Common::KEYCODE_INVALID;
	Common::fill(_r3List, _r3List + NUM_SPACE_OBJECTS, (R3 *)nullptr);
	Common::fill(_orderedR3List, _orderedR3List + NUM_SPACE_OBJECTS, (R3 *)nullptr);

	for (int i = 0; i < NUM_OBJECTS; i++)
		_itemList[i] = g_itemList[i];

	for (int i = 0; i < MAX_BAN_FILES; i++)
		_banFiles[i] = nullptr;

	_targetPlanet = -1;
	_currentPlanet = -1;
	_gameIsPaused = false;
	_bridgeInputLocked = false;
	_currentScreenTalker = -1;
	_missionState = kMissionNone;
	_missionTicks = 0;
	Common::fill(_missionVars, _missionVars + ARRAYSIZE(_missionVars), 0);
	_missionScore = 0;
	_missionMaxScore = 0;
	_missionEndFlag = 0;
	_returnedFromAwayMission = false;
	_randomEncounterType = 0;
	_randomEncounterCountdown = -1;
	_lastMissionId = -1;
	Common::fill(_missionPoints, _missionPoints + 7, 0);

	_awayMission.demon.missionScore = 0;
	_awayMission.tug.missionScore = 0;
	_awayMission.love.missionScore = 0;
	_awayMission.mudd.missionScore = 0;
	_awayMission.feather.missionScore = 0;
	_awayMission.trial.missionScore = 0;
	_awayMission.sins.missionScore = 0;
	_awayMission.veng.missionScore = 0;

	const Common::FSNode gameDataDir(ConfMan.getPath("path"));
	SearchMan.addSubDirectoryMatching(gameDataDir, "patches");
}

StarTrekEngine::~StarTrekEngine() {
	delete _activeMenu->nextMenu;
	delete _activeMenu;

	delete _gfx;
	delete _sound;
	delete _resource;
	delete[] _planetSphereMap;
}

Common::Error StarTrekEngine::run() {
	const Common::FSNode gameDataDir(ConfMan.getPath("path"));
	SearchMan.addSubDirectoryMatching(gameDataDir, "misc");
	SearchMan.addSubDirectoryMatching(gameDataDir, "database");

	bool isDemo = getFeatures() & GF_DEMO;
	bool isCd = getFeatures() & GF_CDROM;

	if (getPlatform() == Common::kPlatformMacintosh && isCd) {
		// "Star Trek Audio" is only in the installer of the CD, if it wasn't installed
		Common::SeekableReadStream *installer = Common::MacResManager::openFileOrDataFork("Star Trek\xc2\xae CD-ROM Installer");
		Common::Archive *archive = installer ? Common::createStuffItArchive(installer, true) : nullptr;
		if (archive)
			SearchMan.add("Star Trek CD-ROM Installer", archive, -1);
	}

	_resource = new Resource(getPlatform(), isDemo);
	_gfx = new Graphics(this);
	_sound = new Sound(this);
	setDebugger(new Console(this));

	initGraphics(SCREEN_WIDTH, SCREEN_HEIGHT);
	initializeEventsAndMouse();
	initTrigTables();

	_enterpriseR3 = R3();
	_enterpriseR3.matrix = initMatrix();
	loadShipDescriptor(&_enterpriseR3, "enterpri");
	addR3(&_enterpriseR3);

	if (!isDemo)
		loadBridgeComputerTopics();

	_gfx->setMouseBitmap(!isDemo ? "pushbtn" : "cursor");

	bool loadedSave = false;

	if (ConfMan.hasKey("save_slot")) {
		initSpaceObjects();
		if (!loadGame(ConfMan.getInt("save_slot")))
			error("Failed to load savegame %d", ConfMan.getInt("save_slot"));
		loadedSave = true;
	}

	if (!loadedSave) {
		if (isCd || !isDemo)
			playIntro();
		initSpaceObjects();
	}

	if (isCd && getPlatform() == Common::kPlatformMacintosh) {
		// The Mac only has the text display mode in the options menu
		if (loadTextDisplayMode() != -1)
			showTextConfigurationMenu(false);
	} else if (isCd) {
		// The text display mode is asked for once
		bool askTextDisplayMode = !loadedSave && loadTextDisplayMode() == -1;
		if (askTextDisplayMode) {
			_gfx->loadPalette("bridge");
			_gfx->clearScreenAndPriBuffer();
			_gfx->fadeinScreen();
		}
		showTextConfigurationMenu(false);
		if (askTextDisplayMode)
			_gfx->fadeoutScreen();
	}

	if (!loadedSave) {
		if (isCd || !isDemo) {
			_missionToLoad = "DEMON";
			_missionState = kMissionDemonStart;
			runGameMode(GAMEMODE_BRIDGE, false);
		} else {
			_missionToLoad = "DEMO";
			runGameMode(GAMEMODE_AWAYMISSION, false);
		}
	} else {
		_roomIndexToLoad = -1;
		runGameMode(_gameMode, true);
	}

	return Common::kNoError;
}

// Starts a new game without the intro. The original loads SAVEGAME.NEW for this.
void StarTrekEngine::restartGame() {
	for (int i = 0; i < NUM_ENEMY_SHIPS; i++) {
		if (_enemyShips[i].field1e != 0)
			delR3(&_enemyShips[i]);
		_enemyShips[i] = R3();
	}
	removeProjectiles();
	removeOrbitObject();
	Common::fill(_r3List, _r3List + NUM_SPACE_OBJECTS, (R3 *)nullptr);

	_enterpriseR3 = R3();
	_enterpriseR3.matrix = initMatrix();
	loadShipDescriptor(&_enterpriseR3, "enterpri");
	addR3(&_enterpriseR3);
	initSpaceObjects();

	_enterpriseState = EnterpriseState();
	_targetShip = nullptr;
	_msRandState = 1;
	_spaceMessages = 0;
	_repairPriority = kRepairNone;
	_repairedGroup = kRepairNone;
	_emergencyPowerUsed = false;
	_emergencyPowerTicks = 0;
	_emergencyPowerRate = 0;
	_emergencyPowerAmount = 30;
	_emergencyPowerDuration = 450;
	_viewDirection = 0;
	_viewscreenZoom = 6;

	memset(&_awayMission, 0, sizeof(_awayMission));
	for (int i = 0; i < NUM_OBJECTS; i++)
		_itemList[i] = g_itemList[i];

	_targetPlanet = -1;
	_currentPlanet = -1;
	_randomEncounterType = 0;
	_randomEncounterCountdown = -1;
	_lastMissionId = -1;
	Common::fill(_missionPoints, _missionPoints + 7, 0);
	_missionToLoad = "DEMON";
	_missionState = kMissionDemonStart;
	_missionTicks = 0;
	Common::fill(_missionVars, _missionVars + ARRAYSIZE(_missionVars), 0);
	_missionScore = 0;
	_missionMaxScore = 0;
	_missionEndFlag = 0;
	_returnedFromAwayMission = false;
	_bridgeInputLocked = false;
	_frameIndex = 0;

	// The bridge is set up again by the game loop
	if (_gameMode == GAMEMODE_BRIDGE) {
		cleanupBridge();
		_lastGameMode = -1;
	}
	_gameMode = GAMEMODE_BRIDGE;
}

Common::Error StarTrekEngine::runGameMode(int mode, bool resume) {
	_gfx->toggleMouse(true);

	if (!resume) { // Only run this if not just resuming from a savefile
		_frameIndex = 0;
		_lastGameMode = -1;
		_gameMode = mode;

		_sound->stopAllVocSounds();

		_resetGameMode = true;
		if (_gameMode == GAMEMODE_START)
			_gameMode = GAMEMODE_BRIDGE;
	}

	while (true) {
		if (_resetGameMode) {
			if (_pendingRestart) {
				_pendingRestart = false;
				restartGame();
			}
			if (_pendingLoadSlot != -1) {
				int slot = _pendingLoadSlot;
				_pendingLoadSlot = -1;
				loadGame(slot);
			}
			if (!_sound->_loopingAudioName.empty())
				_sound->playVoc(_sound->_loopingAudioName);
			_resetGameMode = false;
		}
		TrekEvent event;

		if (_gameMode != _lastGameMode) {
			// Cleanup previous game mode
			switch (_lastGameMode) {
			case GAMEMODE_BRIDGE:
				cleanupBridge();
				break;

			case GAMEMODE_AWAYMISSION:
				cleanupAwayMission();
				break;

			case GAMEMODE_BEAMDOWN:
			case GAMEMODE_BEAMUP:
			default:
				break;
			}

			_lastGameMode = _gameMode;

			// Load next game mode
			switch (_gameMode) {
			case GAMEMODE_BRIDGE:
				_sound->loadMusicFile("bridge");
				initBridge(true);
				break;

			case GAMEMODE_AWAYMISSION:
				initAwayMission();
				break;

			case GAMEMODE_BEAMDOWN:
				_awayMission.redshirtDead = false;
				_sound->loadMusicFile("ground");
				runTransportSequence("teled");
				_gameMode = GAMEMODE_AWAYMISSION;
				continue; // Back to start of loop

			case GAMEMODE_BEAMUP:
				runTransportSequence("teleb");
				_gameMode = GAMEMODE_BRIDGE;
				delete _room;
				//sub_15c61();
				_sound->stopAllVocSounds();
				_sound->playVoc("bridloop");
				continue; // Back to start of loop

			default:
				break;
			}
		}

		// Run current game mode
		switch (_gameMode) {
		case GAMEMODE_BRIDGE:
			popNextEvent(&event);
			runBridge();
			break;

		case GAMEMODE_AWAYMISSION:
			runAwayMission();
			break;

		case GAMEMODE_BEAMDOWN:
		case GAMEMODE_BEAMUP:
			error("Can't be here.");
			break;

		default:
			break;
		}
	}

	return Common::kNoError;
}

void StarTrekEngine::runTransportSequence(const Common::String &name) {
	const uint16 crewmanTransportPositions[][2] = {
		{ 0x8e, 0x7c },
		{ 0xbe, 0x7c },
		{ 0x7e, 0x72 },
		{ 0xaa, 0x72 }
	};

	_sound->stopAllVocSounds();
	_gfx->fadeoutScreen();
	removeDrawnActorsFromScreen();
	initActors();

	_gfx->setBackgroundImage("transprt");
	_gfx->clearPri();
	_gfx->loadPalette("palette");
	_gfx->copyBackgroundScreen();
	_system->updateScreen();
	_system->delayMillis(10);

	for (int i = 0; i < (_awayMission.redshirtDead ? 3 : 4); i++) {
		Common::String filename = getCrewmanAnimFilename(i, name);
		int x = crewmanTransportPositions[i][0];
		int y = crewmanTransportPositions[i][1];
		loadActorAnim(i, filename, x, y, 1.0);
		_actorList[i].animationString.clear();
	}

	if (_missionToLoad.equalsIgnoreCase("feather") && name[4] == 'b') {
		loadActorAnim(9, "qteleb", 0x61, 0x79, 1.0);
	} else if (_missionToLoad.equalsIgnoreCase("trial")) {
		if (name[4] == 'd') {
			loadActorAnim(9, "qteled", 0x61, 0x79, 1.0);
		} else if (_missionEndFlag >= 3) {
			loadActorAnim(9, "qteleb", 0x61, 0x79, 1.0);
		}
	}

	loadActorAnim(8, "transc", 0, 0, 1.0);

	// TODO: redraw mouse and sprite_52c4e?

	_gfx->drawAllSprites();
	_gfx->fadeinScreen();

	_sound->playSoundEffectIndex(kSfxTransporterEnergize);

	if (name.equalsIgnoreCase("teled"))
		_sound->playSoundEffectIndex(kSfxTransporterDematerialize);
	else
		_sound->playSoundEffectIndex(kSfxTransporterMaterialize);

	while (_actorList[0].field62 == 0) {
		TrekEvent event;
		if (popNextEvent(&event)) {
			if (event.type == TREKEVENT_TICK) {
				// TODO: redraw sprite_52c4e?
				_frameIndex++;
				updateActorAnimations();
				_gfx->drawAllSprites();
			}
		}
	}

	// TODO: redraw sprite_52c4e?

	_gfx->drawAllSprites();
	_gfx->fadeoutScreen();
	removeDrawnActorsFromScreen();
	initActors();
}

void StarTrekEngine::playMovie(Common::String filename) {
	if (getPlatform() == Common::kPlatformMacintosh)
		playMovieMac(filename);
	else
		error("Interplay MVE not yet supported");
}

void StarTrekEngine::playMovieMac(Common::String filename) {
	// Swap to 16bpp mode
	initGraphics(512, 384, NULL);

	Video::QuickTimeDecoder *qtDecoder = new Video::QuickTimeDecoder();

	if (!qtDecoder->loadFile(Common::Path(filename)))
		error("Could not open '%s'", filename.c_str());

	bool continuePlaying = true;

	qtDecoder->start();

	while (!qtDecoder->endOfVideo() && !shouldQuit() && continuePlaying) {
		if (qtDecoder->needsUpdate()) {
			const ::Graphics::Surface *frame = qtDecoder->decodeNextFrame();

			if (frame) {
				::Graphics::Surface *convertedFrame = frame->convertTo(_system->getScreenFormat());
				_system->copyRectToScreen(convertedFrame->getPixels(), convertedFrame->pitch, 0, 0, convertedFrame->w, convertedFrame->h);
				_system->updateScreen();
				convertedFrame->free();
				delete convertedFrame;
			}
		}

		Common::Event event;
		while (g_system->getEventManager()->pollEvent(event))
			if (event.type == Common::EVENT_KEYDOWN || event.type == Common::EVENT_LBUTTONDOWN)
				continuePlaying = false;

		_system->delayMillis(10);
	}

	delete qtDecoder;

	// Swap back to 8bpp mode
	initGraphics(SCREEN_WIDTH, SCREEN_HEIGHT);
}

void StarTrekEngine::syncSoundSettings() {
	Engine::syncSoundSettings();
	if (_sound)
		_sound->syncSoundSettings();
}

void StarTrekEngine::showMacLogo(uint32 ticks) {
	const char *appName;
	if (getFeatures() & GF_DEMO)
		appName = "Star Trek\xc2\xae Demo";
	else if (getFeatures() & GF_CDROM)
		appName = "Star Trek\xc2\xae Enhanced CD-ROM";
	else
		appName = "Star Trek\xc2\xae";

	Common::MacResManager appFork;
	Common::SeekableReadStream *stream = nullptr;
	if (appFork.open(Common::Path(appName)))
		stream = appFork.getResource(MKTAG('P', 'I', 'C', 'T'), 128);
	if (!stream) {
		warning("Could not load the logo of '%s'", appName);
		return;
	}

	Image::PICTDecoder pict;
	bool loaded = pict.loadStream(*stream);
	delete stream;
	if (!loaded || pict.getSurface()->format.bytesPerPixel != 1)
		return;

	const ::Graphics::Surface *logo = pict.getSurface();
	initGraphics(512, 384);
	_system->getPaletteManager()->setPalette(pict.getPalette());
	_system->fillScreen(pict.getPalette().findBestColor(0, 0, 0));
	_system->copyRectToScreen(logo->getPixels(), logo->pitch, (512 - logo->w) / 2, (384 - logo->h) / 2, logo->w, logo->h);
	_system->updateScreen();

	const uint32 endTime = _system->getMillis() + ticks * 50;
	bool skip = false;
	while (!skip && !shouldQuit() && _system->getMillis() < endTime) {
		Common::Event event;
		while (_eventMan->pollEvent(event)) {
			if (event.type == Common::EVENT_KEYDOWN || event.type == Common::EVENT_LBUTTONDOWN)
				skip = true;
		}
		_system->delayMillis(10);
	}

	initGraphics(SCREEN_WIDTH, SCREEN_HEIGHT);
}

uint16 StarTrekEngine::getRandomWord() {
	return _randomSource.getRandomNumber(0xffff);
}

} // End of namespace StarTrek
