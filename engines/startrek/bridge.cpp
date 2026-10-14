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

#include "common/str-array.h"

#include "startrek/bridge.h"
#include "startrek/iwfile.h"
#include "startrek/resource.h"
#include "startrek/room.h"
#include "startrek/startrek.h"

namespace StarTrek {

struct BridgeActorAndMenu {
	int id;
	const char *anim;
	const char *menu;
	int16 x;
	int16 y;
};

BridgeActorAndMenu bridgeActorsAndMenus[] = {
	{ 0, "bstndki", "command",  60,  100 },	// Kirk
	{ 1, "bstndsp", "comp",    200,   30 },	// Spock
	{ 4, "bstndsu", "helm",     60,   70 },	// Sulu
	{ 5, "bstndch", "nav",      60,   70 },	// Chekov
	{ 6, "bstnduh", "comm",     30,   30 },	// Uhura
	{ 7, "bstndsc", "eng",      30,   30 },	// Scotty
};

void StarTrekEngine::initBridge(bool b) {
	_gfx->loadPalette("bridge");
	_sound->loadMusicFile("bridge");

	if (b && !_enterpriseState.inOrbit)
		initStarfieldPosition();
	_enterpriseR3.pos = _starfieldPosition;
	_enterpriseR3.matrix = _someMatrix;

	loadBridge();
}

void StarTrekEngine::loadBridge() {
	_viewscreenFullscreen = false;
	initViewscreenStarfield();

	_gfx->setBackgroundImage("bridge");
	_gfx->loadPri("bridge");
	_gfx->copyBackgroundScreen();
	_system->updateScreen();

	loadBridgeActors();
	initStarfieldSprite(&_starfieldSprite, new StubBitmap(0, 0), _starfieldRect);
	initBridgeDisplays();

	//sub_1312C();	// TODO

	// TODO
	//initStarfieldSprite();
	//initStarfieldSprite();
	//initStarfieldSprite();
	//initStarfieldSprite();
	//initStarfieldSprite();
}

void StarTrekEngine::loadBridgeActors() {
	for (int i = 0; i < ARRAYSIZE(bridgeActorsAndMenus); ++i) {
		BridgeActorAndMenu a = bridgeActorsAndMenus[i];
		loadActorAnim(a.id, a.anim, 0, 0, 1.0);
	}
}

// TODO: 2 params, change Enterprise state
void StarTrekEngine::setBridgeMouseCursor() {
	if (!_mouseControllingShip)
		_gfx->setMouseBitmap("pushbtn");
	else
		_gfx->setMouseBitmap(_enterpriseState.weapons ? "cursor" : "entcur");
}

void StarTrekEngine::showTextboxBridge(int talker, int textId) {
	showTextboxBridge(talker, _resource->getLoadedText(textId));
}

void StarTrekEngine::showTextboxBridge(int talker, Common::String text) {
	switch (talker) {
	case kBridgeTalkerKirk:
		showTextbox("Captain Kirk", text, 160, 130, 176, 0);
		break;
	case kBridgeTalkerSpock:
		showTextbox("Mr. Spock", text, 294, 106, 44, 0);
		break;
	case kBridgeTalkerSulu:
		showTextbox("Mr. Sulu", text, 122, 116, 176, 0);
		break;
	case kBridgeTalkerChekov:
		showTextbox("Mr. Chekov", text, 196, 116, 176, 0);
		break;
	case kBridgeTalkerUhura:
		showTextbox("Lieutenant Uhura", text, 298, 150, 161, 0);
		break;
	case kBridgeTalkerScotty:
		showTextbox("Mr. Scott", text, 64, 100, 161, 0);
		break;
	case kBridgeTalkerMcCoy:
		showTextbox("Dr. McCoy", text, 160, 100, 44, 0);
		break;
	case kBridgeTalkerCaptainsLog:
		showTextbox("Captain's Log", text, 160, 130, 176, 0);
		break;
	case kBridgeTalkerElasiCaptain:
		showTextbox("Elasi Captain", text, 160, 190, 44, 0);
		break;
	case kBridgeTalkerElasiCereth:
		showTextbox("Elasi Cereth", text, 160, 190, 44, 0);
		break;
	case kBridgeTalkerAutobeacon:
		showTextbox("Autobeacon", text, 160, 130, 161, 0);
		break;
	case kBridgeTalkerHarryMuddsVoice:
		showTextbox("Voice", text, 160, 130, 44, 0);
		break;
	case kBridgeTalkerHarryMudd:
		showTextbox("Harry Mudd", text, 160, 190, 44, -7);
		break;
	case kBridgeTalkerTaraz:
		showTextbox("Taraz", text, 160, 190, 44, -7);
		break;
	case kBridgeTalkerAdmiralKenka:
		showTextbox("Admiral Kenka", text, 160, 190, 44, -7);
		break;
	case kBridgeTalkerKallarax:
		showTextbox("KALLARAX", text, 160, 190, 44, -7);
		break;
	case kBridgeTalkerBredell:
		showTextbox("Bredell", text, 160, 190, 44, -7);
		break;
	}
}

void StarTrekEngine::showSuluText(const Common::String &text, byte color) {
	showTextbox("Mr. Sulu", text, 122, 116, color, 0);
}

void StarTrekEngine::bridgeCrewAction(int crewId) {
	const char *underAttackText = "#BRID\\B_348#Unable to comply, Captain.  We're under attack.";
	const char *orbitUnderAttackText = "#BRID\\B_348#Unable to comply, Captain. We're under attack.";
	const char *missionNotOverText = "#BRID\\B_338#May I respectfully remind the Captain that we haven't accomplished our mission, Sir.";
	const char *leaveOrbitText = "#BRID\\B_349#We must first leave orbit, Sir.";
	const char *nothingToReportText = "#BRID\\B_155#Nothing to report, Captain.";
	const char *hailingFrequenciesOpenText = "#BRID\\BRIDU016#Hailing frequencies open.";
	const char *shieldsUpText = "#BRID\\B_332#Captain, the shields are up.";
	const char *notInOrbitText = "#BRID\\B_350#We're not in orbit, Captain.";
	const char *wrongDestinationText = "#BRID\\B_346#This isn't our destination.";
	const char *transporterText = "#BRID\\C_060#Spock, come with me. Mr Scott, you have the conn.";
	const char *transporterTextFeather = "#BRID\\C_006#Assemble a landing party. Unless we find this so-called criminal, we're going to war.";

	switch (crewId) {
	case kBridgeTalkerSpock:
		if (_randomEncounterType != 0)
			showTextboxBridge(crewId, 7);
		else
			showTextboxBridge(crewId, nothingToReportText);
		break;
	case kBridgeTalkerUhura:
		if (_randomEncounterType != 0)
			showTextboxBridge(crewId, 8);
		else
			showTextboxBridge(crewId, hailingFrequenciesOpenText);
		break;
	case kBridgeTalkerSulu:
		// Leaving orbit is only possible in the mission scripts
		if (_enterpriseState.underAttack)
			showSuluText(orbitUnderAttackText, 0xb0);
		else if (_currentPlanet == _targetPlanet && !_enterpriseState.inOrbit)
			playOrbitSequence(true, true);
		else
			showSuluText(missionNotOverText, 0x2c);
		break;
	case kBridgeTalkerChekov:
		if (_enterpriseState.underAttack) {
			showSuluText(underAttackText, 0xb0);
		} else if (_currentPlanet == _targetPlanet) {
			showSuluText(missionNotOverText, 0x2c);
		} else if (_enterpriseState.inOrbit) {
			showSuluText(leaveOrbitText, 0xb0);
		} else {
			_currentPlanet = chooseDestination();
			if (_currentPlanet != _targetPlanet)
				wrongDestinationRandomEncounter();
		}
		break;
	case kBridgeTalkerKirk:
		if (_enterpriseR3.shieldsActive) {
			showSuluText(shieldsUpText, 0xa1);
		} else if (!_enterpriseState.inOrbit) {
			showSuluText(notInOrbitText, 0xa1);
		} else if (_currentPlanet != _targetPlanet) {
			showSuluText(wrongDestinationText, 0xb0);
		} else {
			if (_missionName == "FEATHER")
				showTextboxBridge(kBridgeTalkerKirk, transporterTextFeather);
			else if (_missionName != "VENG")
				showTextboxBridge(kBridgeTalkerKirk, transporterText);
			_gameMode = GAMEMODE_BEAMDOWN;
		}
		break;
	default:
		break;
	}
}

int StarTrekEngine::showKirkChoices(const int *textIds) {
	Common::StringArray texts;
	for (int i = 0; textIds[i] != -1; i++)
		texts.push_back(_resource->getLoadedText(textIds[i]));

	Common::Array<const char *> options;
	options.push_back("Captain Kirk");
	for (uint i = 0; i < texts.size(); i++)
		options.push_back(texts[i].c_str());
	options.push_back("");

	return showText(&StarTrekEngine::readTextFromArrayWithChoices, (uintptr)options.data(), 160, 130, 176, true, false, false);
}

Common::String StarTrekEngine::getSpeechSampleForNumber(int number) {
	const char *speechTemplate = ",BRID\\B_%03d";
	Common::String result;

	if (number <= 19) {
		// Speech samples 228 ("zero") - 247 ("nineteen")
		result = Common::String::format(speechTemplate, number + 228);
	} else if (number >= 20 && number <= 99) {
		// Speech samples 248 ("twenty") - 255 ("ninety")
		result = Common::String::format(speechTemplate, number / 10 - 2 + 248);
		if (number % 10 > 0)
			result += Common::String::format(speechTemplate, number % 10 + 228);
	} else if (number == 100) {
		result = Common::String::format(speechTemplate, 256);
	}

	return result;
}

void StarTrekEngine::showMissionPerformance(int missionId, int textId, int16 score, int16 maxScore) {
	int percent = 0;
	if (maxScore >= 1)
		percent = MAX<int>(MIN(score, maxScore), 0) * 100 / maxScore;

	// The floppy version has unvoiced texts
	static const char *const kDescriptions[2][5] = {
		{
			"#BRID\\B_199#I'll be frank, Kirk. Starfleet expects more of you than that. Try to do better on your next assignment.",
			"#BRID\\B_197#A satisfactory performance, Captain, but there's still room for improvement.",
			"#BRID\\B_214#Well done, Captain. Keep up the good work.",
			"#BRID\\B_414#The top brass at Starfleet are impressed. Outstanding work, Jim.",
			"#BRID\\B_195#A perfect mission, Jim! You are a model for all Starfleet!"
		},
		{
			"I'll be frank, Kirk, Star Fleet expects more out of you than that. Try to do better on your next assignment.",
			"A satisfactory performance, Captain, but there's still room for improvement.",
			"Well done, Captain. Keep up the good work.",
			"The top brass at Star Fleet are impressed. Outstanding work, Jim.",
			"A perfect mission, Jim! You are a model for all Star Fleet"
		}
	};
	static const MidiTracks kTracks[] = { MIDITRACK_13, MIDITRACK_13, MIDITRACK_11, MIDITRACK_12, MIDITRACK_14 };

	int commendationPoints;
	if (percent <= 50)
		commendationPoints = 0;
	else if (percent <= 70)
		commendationPoints = 1;
	else if (percent <= 85)
		commendationPoints = 2;
	else if (percent <= 99)
		commendationPoints = 3;
	else
		commendationPoints = 4;
	MidiTracks midiTrack = kTracks[commendationPoints];
	Common::String performanceDescription = localizeText(kDescriptions[isFloppyVersion()][commendationPoints]);

	_sound->playMidiMusicTracks(midiTrack, kLoopTypeLast);

	// Good missions improve the emergency power, weapons and shields
	_emergencyPowerAmount += commendationPoints >> 1;
	_emergencyPowerDuration += commendationPoints * 15;
	for (int i = 0; i < 6; i++)
		_enterpriseR3.systems[i].maxStrength += commendationPoints >> 1;
	for (int i = 8; i < 12; i++)
		_enterpriseR3.systems[i].maxStrength += commendationPoints >> 2;

	// The Admiral stays on the viewscreen after the last mission
	bool removeTalker = missionId != 6;
	Common::String text = _resource->getLoadedText(textId);
	if (isFloppyVersion()) {
		// One textbox with the description, and either of two admirals
		Common::String texts[] = {
			Common::String::format(text.c_str(), percent, commendationPoints, performanceDescription.c_str()),
			""
		};
		showBridgeScreenTalkerWithMessages(texts, "Admiral", (msRand() & 1) ? "admira" : "woman", removeTalker);
	} else {
		Common::String texts[] = {
			Common::String::format(
				text.c_str(),
				getSpeechSampleForNumber(percent).c_str(),
				getSpeechSampleForNumber(commendationPoints).c_str(),
				percent,
				commendationPoints,
				"" // The German texts of sins and veng end with a stray %s
			),
			performanceDescription,
			""
		};
		showBridgeScreenTalkerWithMessages(texts, "Admiral", "woman", removeTalker);
	}

	_lastMissionId = missionId;
	_missionPoints[missionId] = commendationPoints;
}

void StarTrekEngine::showBridgeScreenTalkerWithMessage(int textId, Common::String talkerHeader, Common::String talkerId, bool removeTalker) {
	Common::String text = _resource->getLoadedText(textId);
	Common::String texts[] = {
		text,
		""
	};
	showBridgeScreenTalkerWithMessages(texts, talkerHeader, talkerId, removeTalker);
}

void StarTrekEngine::showBridgeScreenTalkerWithMessages(Common::String texts[], Common::String talkerHeader, Common::String talkerId, bool removeTalker) {
	showBridgeScreenTalker(talkerId);
	// Texts on the viewscreen have at most 7 lines
	for (int i = 0; texts[i] != ""; i++)
		showTextbox(talkerHeader, texts[i], 160, 190, 44, -7);

	if (removeTalker)
		removeBridgeScreenTalker();
}

void StarTrekEngine::showBridgeScreenTalker(Common::String talkerId) {
	leaveFullscreenView();
	if (talkerId == "romula" || talkerId == "pira" || talkerId == "klg1" || talkerId == "klg2" || talkerId == "maddoc")
		_sound->playMidiMusicTracks(MIDITRACK_15, kLoopTypeLast);
	else if (talkerId == "mudd")
		_sound->playMidiMusicTracks(MIDITRACK_17, kLoopTypeLast);

	// The talker is shown on a black viewscreen
	_gfx->fillBackgroundRect(_starfieldRect, 0);
	_starfieldSprite.bitmapChanged = true;
	_gfx->addSprite(&_screenTalkerSprite);
	_screenTalkerSprite.setBitmap(_resource->loadBitmapFile(talkerId));
	_screenTalkerSprite.setXYAndPriority(72, 30, 13);
	_currentScreenTalkerAnim = talkerId;
	_currentScreenTalker = loadActorAnim(-1, talkerId, 72, 30, 1.0);
}

void StarTrekEngine::restartBridgeScreenTalker() {
	loadActorAnim(_currentScreenTalker, _currentScreenTalkerAnim, 72, 30, 1.0);
}

void StarTrekEngine::removeBridgeScreenTalker() {
	removeActorFromScreen(_currentScreenTalker);
	_screenTalkerSprite.dontDrawNextFrame();
	_gfx->drawAllSprites();
	_gfx->delSprite(&_screenTalkerSprite);
	delete _screenTalkerSprite.bitmap;
	_screenTalkerSprite.bitmap = nullptr;
	updateStarfieldAndShips(false);
	_gfx->drawAllSprites();
}

void StarTrekEngine::bridgeLeftClick() {
	Sprite *sprite = _gfx->getSpriteAt(_gfx->getMousePos());
	int clickedActor = -1;

	if (sprite == nullptr)
		return;

	for (int i = 0; i < NUM_ACTORS; i++) {
		Actor *actor = &_actorList[i];
		if (sprite == &actor->sprite) {
			clickedActor = i;
			break;
		}
	}

	if (clickedActor == -1)
		return;

	for (int i = 0; i < ARRAYSIZE(bridgeActorsAndMenus); ++i) {
		BridgeActorAndMenu a = bridgeActorsAndMenus[i];
		if (a.id == clickedActor) {
			showBridgeMenu(a.menu, a.x, a.y);
			break;
		}
	}
}

void StarTrekEngine::cleanupBridge() {
	if (_gfx->hasSprite(&_starfieldSprite))
		_gfx->delSprite(&_starfieldSprite);
	removeBridgeDisplays();
	// TODO
	// if (!v_starfieldInitialized)
	// sub_12691()
	// else
	// clearScreenAndDelSeveralSprites()
	// v_mouseControllingShip = 0
	// v_keyboardControlsMouse = 1
}

void StarTrekEngine::runBridge() {
	while (_gameMode == GAMEMODE_BRIDGE && !_resetGameMode) {
		if (_returnedFromAwayMission) {
			_returnedFromAwayMission = false;
			// The away mission has its own texts
			_resource->setTxtFileName(_missionName);
			handleMissionEvent(kMissionEventReturn, 0);
			continue;
		}
		handleBridgeEvents();
	}
}

void StarTrekEngine::handleBridgeEvents() {
	TrekEvent event;

	if (popNextEvent(&event)) {
		if (_gameIsPaused) {
			if (event.type == TREKEVENT_LBUTTONDOWN || event.type == TREKEVENT_KEYDOWN) {
				_gameIsPaused = false;
			}
			return;
		}

		// Only pausing is possible while the mission script runs a sequence
		if (_bridgeInputLocked && event.type != TREKEVENT_TICK) {
			if (event.type == TREKEVENT_KEYDOWN && event.kbd.keycode == Common::KEYCODE_p)
				_gameIsPaused = true;
			return;
		}

		switch (event.type) {
		case TREKEVENT_TICK:
			updateBridgeTick();
			showSpaceMessages();
			_missionTicks++;
			handleMissionEvent(kMissionEventTick, _missionTicks);
			break;

		case TREKEVENT_LBUTTONDOWN:
			if (_mouseControllingShip)
				handleBridgeMenu(kBridgeFirePhasers);
			else
				bridgeLeftClick();
			break;

		case TREKEVENT_MOUSEMOVE:
			restrictBridgeMouse();
			if (!_enterpriseState.inOrbit && _mouseControllingShip) {
				_enterpriseR3.targetTurnRate.y = (int32)(event.mouse.x - _starfieldXVar1) << 16;
				_enterpriseR3.targetTurnRate.x = (int32)(event.mouse.y - _starfieldYVar1) << 16;
			}
			break;

		case TREKEVENT_RBUTTONDOWN:
			if (_mouseControllingShip)
				handleBridgeMenu(kBridgeFireTorpedoes);
			break;

		case TREKEVENT_KEYDOWN:
			switch (event.kbd.keycode) {
			case Common::KEYCODE_w:	// Toggle weapons on/off
				handleBridgeMenu(kBridgeChekovWeapons);
				break;
			case Common::KEYCODE_s:	// Toggle shields on/off
				handleBridgeMenu(kBridgeSuluShields);
				break;
			case Common::KEYCODE_a:	// Target analysis
				handleBridgeMenu(kBridgeSuluTargetAnalysis);
				break;
			case Common::KEYCODE_d:	// Damage control
				handleBridgeMenu(kBridgeScottyDamageControl);
				break;
			case Common::KEYCODE_RETURN:
			case Common::KEYCODE_KP_ENTER:
			case Common::KEYCODE_F1:
				// Fire phasers
				if (_mouseControllingShip)
					handleBridgeMenu(kBridgeFirePhasers);
				break;
			case Common::KEYCODE_SPACE:	// Fire photon torpedoes
			case Common::KEYCODE_F2:
				if (_mouseControllingShip)
					handleBridgeMenu(kBridgeFireTorpedoes);
				break;
			case Common::KEYCODE_v:	// Toggle between the normal bridge view and full screen view
				handleBridgeMenu(kBridgeStarfieldFullScreen);
				break;
			case Common::KEYCODE_TAB:	// Toggle between movement/fire mode and crew selection mode
				setMouseControllingShip(!_mouseControllingShip);
				restrictBridgeMouse();
				break;
			case Common::KEYCODE_COMMA:	// Reduce the main view screen magnification
				setViewscreenZoom(_viewscreenZoom - 1);
				break;
			case Common::KEYCODE_PERIOD:	// Enlarge the main view screen magnification
				setViewscreenZoom(_viewscreenZoom + 1);
				break;
			case Common::KEYCODE_o:	// Enter/exit orbit
				handleBridgeMenu(kBridgeSuluOrbit);
				break;
			case Common::KEYCODE_KP5:	// Center controls
				_enterpriseR3.targetTurnRate = Point3(0, 0, 0);
				if (_mouseControllingShip)
					_gfx->warpMouse(160, _starfieldYVar1);
				break;
			case Common::KEYCODE_n:	// Main star navigational map
				handleBridgeMenu(kBridgeChekovNavigation);
				break;
			case Common::KEYCODE_k:	// Kirk's options
				handleBridgeMenu(kBridgeKirkOptions);
				break;
			case Common::KEYCODE_t:	// Ask Mr. Spock for advice
				handleBridgeMenu(kBridgeSpock);
				break;
			case Common::KEYCODE_c:	// Spock's library computer
				handleBridgeMenu(kBridgeSpockComputer);
				break;
			case Common::KEYCODE_h:	// Uhura's communication icon
				handleBridgeMenu(kBridgeUhura);
				break;
			case Common::KEYCODE_p:	// Pause game
				_gameIsPaused = true;
				break;
			case Common::KEYCODE_e:	// Toggle SFX / emergency power
				if (event.kbd.flags & Common::KBD_CTRL)
					_sound->toggleSfx();
				else
					handleBridgeMenu(kBridgeScottyEmergencyPower);
				break;
			case Common::KEYCODE_m:
				if (event.kbd.flags & Common::KBD_CTRL)
					_sound->toggleMusic();
				break;
			case Common::KEYCODE_q:
				if (event.kbd.flags & Common::KBD_CTRL)
					showQuitGamePrompt(20, 20);
				break;
			case Common::KEYCODE_BACKQUOTE:
			case Common::KEYCODE_TILDE:	// Ship speed: reverse
				if (!_enterpriseState.inOrbit)
					_enterpriseR3.commandedSpeed = -_enterpriseR3.maxSpeed >> 3;
				break;
			case Common::KEYCODE_1:	// Ship speed: stop
			case Common::KEYCODE_2:
			case Common::KEYCODE_3:
			case Common::KEYCODE_4:
			case Common::KEYCODE_5:
			case Common::KEYCODE_6:
			case Common::KEYCODE_7:
			case Common::KEYCODE_8:
			case Common::KEYCODE_9:
			case Common::KEYCODE_0:
				// Ship speed: toggle slow -> fast
				if (!_enterpriseState.inOrbit) {
					int speed = (event.kbd.keycode == Common::KEYCODE_0) ? 9 : event.kbd.keycode - Common::KEYCODE_1;
					_enterpriseR3.commandedSpeed = speed * _enterpriseR3.maxSpeed / 9;
				}
				break;
			default:
				break;
			}
			break;

		default:
			break;
		}
	}
}

void StarTrekEngine::updateBridgeTick() {
	if (!_enterpriseState.inOrbit) {
		// Roll while Ins or Del is held
		int32 roll = 0;
		if (_heldKey == Common::KEYCODE_INSERT || _heldKey == Common::KEYCODE_KP0)
			roll = _enterpriseR3.maxTurnRate;
		else if (_heldKey == Common::KEYCODE_DELETE || _heldKey == Common::KEYCODE_KP_PERIOD)
			roll = -_enterpriseR3.maxTurnRate;
		_enterpriseR3.targetTurnRate.z = roll;
	}
	updateBridgeSpace();
	if (!_viewscreenFullscreen)
		updateBridgeDisplays();
	updateActorAnimations();

	//updateMouseBitmap();
	renderBanBelowSprites();
	_gfx->drawAllSprites(false);
	renderBanAboveSprites();
	_gfx->updateScreen();

	_sound->checkLoopMusic();
	_frameIndex++;
	_roomFrameCounter++;
	addAction(ACTION_TICK, _roomFrameCounter & 0xff, (_roomFrameCounter >> 8) & 0xff, 0);
	if (_roomFrameCounter >= 2) {
		cycleBridgeColors();
		_gfx->incPaletteFadeLevel();
	}
}

void StarTrekEngine::updateBridgeSpace() {
	_enterpriseR3.shieldsOn = _enterpriseState.shields;
	_enterpriseR3.weaponsOn = _enterpriseState.weapons;
	distributeEnergy(&_enterpriseR3);
	moveR3(&_enterpriseR3);
	if ((_frameIndex & 0x1f) == 7)
		repairShipSystems(&_enterpriseR3);
	updateEnemyShips();
	updateProjectiles();
	if ((_frameIndex & 0x3f) == 0)
		normalizeShipMatrices();
	rotateR3(&_planetR3, 1, 4);
	updateCamera();

	// The viewscreen shows static depending on the damage of this system
	const ShipSystem &viewscreen = _enterpriseR3.systems[12];
	bool showStatic = viewscreen.maxStrength > 0 && viewscreen.strength < (int16)(msRand() % viewscreen.maxStrength);
	updateStarfieldAndShips(showStatic);
}

void StarTrekEngine::setMouseControllingShip(bool enable) {
	if (_mouseControllingShip || _viewscreenFullscreen)
		_restoreFullscreenView = _viewscreenFullscreen;
	_mouseControllingShip = enable;
	setBridgeMouseCursor();

	// The full screen view is only available while steering
	if (enable && _restoreFullscreenView)
		enterFullscreenView();
	else
		leaveFullscreenView();

	_gfx->warpMouse(160, enable ? _starfieldYVar1 : 166);
}

void StarTrekEngine::enterFullscreenView() {
	if (_viewscreenFullscreen)
		return;

	_viewscreenZoom = 4;
	removeDrawnActorsFromScreen();
	removeBridgeDisplays();
	if (_gfx->hasSprite(&_starfieldSprite))
		_gfx->delSprite(&_starfieldSprite);
	_gfx->fillBackgroundRect(Common::Rect(SCREEN_WIDTH, SCREEN_HEIGHT), 0);
	_gfx->clearScreenAndPriBuffer();

	_viewscreenFullscreen = true;
	initViewscreenStarfield();
	initStarfieldSprite(&_starfieldSprite, new StubBitmap(0, 0), _starfieldRect);
}

void StarTrekEngine::leaveFullscreenView() {
	if (!_viewscreenFullscreen)
		return;

	_viewscreenZoom = 6;
	loadBridge();
}

void StarTrekEngine::initViewscreenStarfield() {
	if (_viewscreenFullscreen)
		initStarfield(2, 2, 319, 199, 0x200 / _viewscreenZoom);
	else
		initStarfield(72, 30, 247, 102, 0x200 / _viewscreenZoom);
}

void StarTrekEngine::restrictBridgeMouse() {
	Common::Point mouse = _gfx->getMousePos();
	Common::Point restricted = mouse;
	if (_mouseControllingShip) {
		restricted.x = CLIP<int16>(mouse.x, _starfieldRect.left, _starfieldRect.right - 1);
		restricted.y = CLIP<int16>(mouse.y, _starfieldRect.top, _starfieldRect.bottom - 1);
	} else {
		// Keep the mouse below the viewscreen
		restricted.y = MAX<int16>(mouse.y, _starfieldRect.bottom + 3);
	}

	if (restricted != mouse)
		_gfx->warpMouse(restricted.x, restricted.y);
}

void StarTrekEngine::setViewscreenZoom(int16 zoom) {
	if (zoom < 4 || zoom > 8)
		return;

	_viewscreenZoom = zoom;
	showTextboxBridge(kBridgeTalkerSulu, Common::String::format(localizeText("Magnification %d"), zoom - 3));
	initViewscreenStarfield();
}

// Ship system groups that Scotty repairs first, for each item of the repair menu
static const int16 kRepairGroups[] = {
	kRepairShields, kRepairPhasers, kRepairTorpedoes, kRepairSensors, kRepairBridge, kRepairHull, kRepairEngines
};

void StarTrekEngine::handleBridgeMenu(int menuEvent) {
	if (!handleMissionEvent(kMissionEventMenu, menuEvent))
		dispatchBridgeMenu(menuEvent);
}

void StarTrekEngine::dispatchBridgeMenu(int menuEvent) {
	const char *raiseShieldsText = "#BRID\\B_340#Raising shields.";
	const char *lowerShieldsText = "#BRID\\B_337#Lowering shields, Captain.";
	const char *armWeaponsText = "#BRID\\B_351#Arming weapons.";
	const char *disarmWeaponsText = "#BRID\\B_354#Disarming weapons.";
	const char *emergencyPowerText = "#BRID\\BRID_S32#I don't know how long she can take it, Captain.";
	const char *noEmergencyPowerText = "#BRID\\BRID_S41#She can't take it, Captain.";
	const char *targetAnalysisOnText = "#BRID\\B_344#Target Analysis On.";
	const char *targetAnalysisOffText = "#BRID\\B_343#Target Analysis Off.";
	const char *ayeSirText = "#BRID\\BRID_S22#Aye Sir.";

	switch (menuEvent) {
	case kBridgeKirkCommand:
		showBridgeMenu("command", 60, 100);
		break;
	case kBridgeFirePhasers:
		if (!_enterpriseState.inOrbit)
			fireEnterpriseWeapons(false);
		break;
	case kBridgeFireTorpedoes:
		if (!_enterpriseState.inOrbit)
			fireEnterpriseWeapons(true);
		break;
	case kBridgeStarfieldFullScreen:
		if (_viewscreenFullscreen) {
			leaveFullscreenView();
		} else {
			enterFullscreenView();
			setMouseControllingShip(true);
		}
		break;
	case kBridgeKirkCaptainsLog: // Kirk, captain's log
		captainsLog();
		break;
	case kBridgeKirkTransporter: // Kirk, transporter
		bridgeCrewAction(kBridgeTalkerKirk);
		break;
	case kBridgeKirkOptions: // Kirk, options
		showOptionsMenu(isFloppyVersion() ? 75 : 65, 60);
		break;
	case kBridgeSpock: // Spock, consult
		bridgeCrewAction(kBridgeTalkerSpock);
		break;
	case kBridgeSpockComputer: // Spock, consult computer
		handleBridgeComputer();
		break;
	case kBridgeScottyDamageControl: // Scotty, damage control
		showBridgeMenu("repair", 30, 30);
		break;
	case kBridgeScottyEmergencyPower: // Scotty, emergency power
		if (!_emergencyPowerUsed) {
			_emergencyPowerUsed = true;
			_emergencyPowerTicks = _emergencyPowerDuration;
			_emergencyPowerRate = _emergencyPowerAmount;
			showTextboxBridge(kBridgeTalkerScotty, emergencyPowerText);
		} else {
			showTextboxBridge(kBridgeTalkerScotty, noEmergencyPowerText);
		}
		break;
	case kBridgeUhura: // Uhura, communications
		bridgeCrewAction(kBridgeTalkerUhura);
		break;
	case kBridgeSuluOrbit: // Sulu, orbit
		bridgeCrewAction(kBridgeTalkerSulu);
		break;
	case kBridgeSuluShields: // Sulu, shields
		_enterpriseState.shields = !_enterpriseState.shields;
		showTextboxBridge(kBridgeTalkerSulu, _enterpriseState.shields ? raiseShieldsText : lowerShieldsText);
		break;
	case kBridgeChekovNavigation: // Chekov, navigation
		bridgeCrewAction(kBridgeTalkerChekov);
		break;
	case kBridgeChekovWeapons: // Chekov, weapons
		_enterpriseState.weapons = !_enterpriseState.weapons;
		showTextboxBridge(kBridgeTalkerChekov, _enterpriseState.weapons ? armWeaponsText : disarmWeaponsText);
		setBridgeMouseCursor();
		break;
	case kBridgeChekovRepairShields:
	case kBridgeChekovRepairPhasers:
	case kBridgeChekovRepairPhotonTorpedoes:
	case kBridgeChekovRepairSensors:
	case kBridgeChekovRepairBridge:
	case kBridgeChekovRepairHull:
	case kBridgeChekovRepairWarpDrives:
		_repairPriority = kRepairGroups[menuEvent - kBridgeChekovRepairShields];
		showTextboxBridge(kBridgeTalkerScotty, ayeSirText);
		break;
	case kBridgeSuluTargetAnalysis: // Chekov, target analysis
		_enterpriseState.targetAnalysis = !_enterpriseState.targetAnalysis;
		showTextboxBridge(kBridgeTalkerSulu, _enterpriseState.targetAnalysis ? targetAnalysisOnText : targetAnalysisOffText);
		_targetShip = nullptr;
		if (_enterpriseState.targetAnalysis) {
			for (int i = 0; i < NUM_ENEMY_SHIPS && !_targetShip; i++) {
				if (_enemyShips[i].field1e != 0)
					_targetShip = &_enemyShips[i];
			}
		}
		break;
	default:
		break;
	}
}

// The first 20 stars are the star systems, in the order of the planet IDs
static const int16 kStarMapPositions[][2] = {
	{  14,  16 }, {  28,  59 }, {  62,  32 }, { 102,  38 }, { 116,  33 },
	{ 167,  18 }, { 245,  27 }, { 263,   8 }, { 294,  37 }, { 260,  63 },
	{ 253, 118 }, { 230, 122 }, { 237,  93 }, { 220,  77 }, { 177,  62 },
	{ 167, 111 }, { 118,  79 }, {  90, 102 }, {  73, 111 }, { 111, 124 },
	{  21,  95 }, {  49,  28 }, {  81, 153 }, {  40, 133 }, {  25, 121 },
	{ 112, 159 }, { 154, 169 }, { 160, 152 }, { 184, 135 }, { 213, 166 },
	{ 271, 165 }, { 269, 136 }, { 297, 145 }, { 303, 103 }, { 279, 111 },
	{ 280,  85 }, { 300,  14 }, { 217,  33 }, { 218,  55 }, { 166,  85 },
	{  50,  44 }, { 151,  44 }, { 156,  16 }, {  83,  72 }, { 150, 102 }
};

static int getClosestStar(const Common::Point &pos) {
	int closest = 0;
	int closestDistance = 0x7fff;

	for (int i = 0; i < ARRAYSIZE(kStarMapPositions); i++) {
		int distance = ABS(kStarMapPositions[i][0] - pos.x) + ABS(kStarMapPositions[i][1] - pos.y);
		if (distance < closestDistance) {
			closest = i;
			closestDistance = distance;
		}
	}

	return closest;
}

int StarTrekEngine::showStarMap() {
	_gfx->pushSprites();
	_gfx->setMouseBitmap("cross");

	int star = getClosestStar(_gfx->getMousePos());
	Sprite circleSprite;
	_gfx->addSprite(&circleSprite);
	circleSprite.setBitmap(_resource->loadBitmapFile("circle"));
	circleSprite.setXYAndPriority(kStarMapPositions[star][0], kStarMapPositions[star][1], 14);

	_gfx->setBackgroundImage("starmap");
	_gfx->copyBackgroundScreen();
	_gfx->drawAllSprites();

	bool selected = false;
	while (!selected) {
		TrekEvent event;
		if (!popNextEvent(&event))
			continue;

		switch (event.type) {
		case TREKEVENT_TICK:
			_frameIndex++;
			break;

		case TREKEVENT_MOUSEMOVE:
			star = getClosestStar(_gfx->getMousePos());
			circleSprite.setXYAndPriority(kStarMapPositions[star][0], kStarMapPositions[star][1], 14);
			_gfx->drawAllSprites();
			break;

		case TREKEVENT_LBUTTONDOWN:
		case TREKEVENT_RBUTTONDOWN:
			selected = true;
			break;

		case TREKEVENT_KEYDOWN:
			switch (event.kbd.keycode) {
			case Common::KEYCODE_RETURN:
			case Common::KEYCODE_KP_ENTER:
			case Common::KEYCODE_SPACE:
			case Common::KEYCODE_F1:
			case Common::KEYCODE_F2:
				selected = true;
				break;
			default:
				break;
			}
			break;

		default:
			break;
		}
	}

	circleSprite.dontDrawNextFrame();
	_gfx->drawAllSprites();
	_gfx->delSprite(&circleSprite);
	delete circleSprite.bitmap;
	circleSprite.bitmap = nullptr;
	_gfx->popSprites();
	setBridgeMouseCursor();

	playWarpSequence();
	_gfx->loadPalette("bridge");
	initSystemPlanet(star);

	return star;
}

int StarTrekEngine::chooseDestination() {
	bool wasFullscreen = _viewscreenFullscreen;
	_gfx->fadeoutScreen();
	enterFullscreenView();
	_mouseControllingShip = false;

	int star = showStarMap();

	setMouseControllingShip(true);
	if (!wasFullscreen)
		leaveFullscreenView();
	_gfx->fadeoutScreen();
	return star;
}

// The Enterprise flies past the camera while the title of the mission is shown
void StarTrekEngine::showMissionStartEnterpriseFlyby(Common::String sequence, Common::String name) {
	initSystemPlanet(kPlanetNone);
	if (isFloppyVersion()) {
		// The music of the bridge follows
		_sound->playMidiMusicTracks(MIDITRACK_11, kLoopTypeTrack0);
	} else {
		_sound->playMidiMusicTracks(MIDITRACK_11);
		_sound->stopAllVocSounds();
		_sound->playVoc("bridloop");
	}
	enterFullscreenView();

	// Positions of the camera and the Enterprise are velocity * frame + start
	int16 velocity[2][3] = { { 100, 0, 0 }, { 125, 0, 0 } };
	int16 start[2][3] = { { 1974, 0, 720 }, { 0, 0, 0 } };

	_someMatrix = initMatrix();
	_enterpriseR3.matrix = initMatrix();
	rotateR3(&_enterpriseR3, 1, 0x100);
	_viewDirection = -1;
	resetShipControls(&_enterpriseR3);
	_mouseControllingShip = false;
	_gfx->setMouseBitmap("blank");

	Sprite titleSprite;
	for (int frame = 2; frame <= 150; frame++) {
		for (int i = 0; i < 3; i++)
			_enterpriseR3.pos[i] = (int16)(velocity[1][i] * frame + start[1][i]);
		if (_viewDirection == -1) {
			for (int i = 0; i < 3; i++)
				_starfieldPosition[i] = (int16)(velocity[0][i] * frame + start[0][i]);
			pointCameraAt(_enterpriseR3.pos, _enterpriseR3.matrix[1]);
		}

		switch (frame) {
		case 65: {
			if (!isFloppyVersion() && _sfxEnabled && _sfxWorking)
				_sound->playSpeech(sequence);
			Bitmap *bitmap = new Bitmap(_resource->loadBitmapFile(name));
			bitmap->xoffset = bitmap->width / 2;
			bitmap->yoffset = bitmap->height / 2;
			_gfx->addSprite(&titleSprite);
			titleSprite.setXYAndPriority(160, 60, 14);
			titleSprite.setBitmap(bitmap);
			break;
		}
		case 95:
			titleSprite.dontDrawNextFrame();
			break;
		case 96:
			_gfx->delSprite(&titleSprite);
			break;
		case 97:
			delete titleSprite.bitmap;
			titleSprite.bitmap = nullptr;
			break;
		case 105:
			leaveFullscreenView();
			_viewDirection = 0;
			start[1][0] = velocity[1][0] * frame;
			velocity[1][0] = _enterpriseR3.maxSpeed >> 18;
			_enterpriseR3.commandedSpeed = _enterpriseR3.maxSpeed >> 2;
			break;
		default:
			break;
		}

		updateBridgeTick();
		waitForNextTick(false);
	}

	setMouseControllingShip(true);
}

void StarTrekEngine::showSpaceMessages() {
	const char *shieldsFallingText = "#BRID\\BRID_S45#The shields are falling!";
	const char *weaponsOfflineText = "#BRID\\BRID_S37#Our weapons are off-line!";

	// The original shows both messages when the shields fall
	if (_spaceMessages & kSpaceMessageShieldsFalling)
		showTextbox("Mr. Scott", shieldsFallingText, 0, 160, 161, 0);
	if (_spaceMessages & (kSpaceMessageShieldsFalling | kSpaceMessageWeaponsOffline))
		showTextbox("Mr. Scott", weaponsOfflineText, 0, 160, 161, 0);

	if (_spaceMessages & kSpaceMessageRepaired) {
		static const struct {
			int16 group;
			const char *name;
		} kRepairNames[] = {
			{ kRepairShields, "#BRID\\SCOTSHIE#Shields" },
			{ kRepairHull, "#BRID\\SCOTHULL#Hull" },
			{ kRepairBridge, "#BRID\\SCOTBRID#Bridge" },
			{ kRepairSensors, "#BRID\\SCOTSENS#Sensors" },
			{ kRepairTorpedoes, "#BRID\\SCOTTORP#Torpedoes" },
			{ kRepairPhasers, "#BRID\\SCOTPHAS#Phasers" },
			{ kRepairEngines, "#BRID\\SCOTENGI#Engines" }
		};
		for (int i = 0; i < ARRAYSIZE(kRepairNames); i++) {
			if (kRepairNames[i].group == _repairedGroup) {
				Common::String text = Common::String::format(localizeText("%s repaired, Captain."), localizeText(kRepairNames[i].name));
				showTextbox("Mr. Scott", text, 64, 100, 161, 0);
				break;
			}
		}
	}
	_spaceMessages = 0;
}

void StarTrekEngine::wrongDestinationRandomEncounter() {
	static const char *const kElasiShips[] = { "orion", "pirate1", "mpirate" };

	msSrand(_frameIndex & 0x7fff);
	_randomEncounterType = msRand() % 3 + 1;
	int count = (msRand() & 3) + 1;
	for (int i = 0; i < count; i++) {
		switch (_randomEncounterType) {
		case kRandomEncounterKlingon:
			spawnEnemyShip("klingon");
			break;
		case kRandomEncounterRomulan:
			spawnEnemyShip((msRand() & 1) ? "romulan" : "ckling");
			break;
		default:
			spawnEnemyShip(kElasiShips[msRand() % 3]);
			break;
		}
	}

	switch (_randomEncounterType) {
	case kRandomEncounterKlingon:
		_resource->setTxtFileName("klingon");
		break;
	case kRandomEncounterRomulan:
		_resource->setTxtFileName("romulan");
		break;
	default:
		_resource->setTxtFileName("elasi");
		break;
	}
	_enterpriseState.underAttack = true;
	_sound->loadMusicFile(count >= 3 ? "bridgeb" : "bridge");

	// The enemies hail the Enterprise after a short while
	uint32 hailTime = _frameIndex + 10;
	while (_frameIndex < hailTime && !_resetGameMode && !shouldQuit())
		handleBridgeEvents();

	showTextbox("Mr. Chekov", _resource->getLoadedText(0), 196, 116, 176, 0);
	showTextbox("Mr. Spock", _resource->getLoadedText(1), 294, 106, 44, 0);
	// In the floppy version, Sulu always says this
	const char *header = "Mr. Sulu";
	if (!isFloppyVersion() && _randomEncounterType == kRandomEncounterKlingon)
		header = "Mr. Spock";
	else if (!isFloppyVersion() && _randomEncounterType == kRandomEncounterElasi)
		header = "Captain Kirk";
	showTextbox(header, _resource->getLoadedText(2), 122, 116, 176, 0);
	showTextbox("Mr. Scott", _resource->getLoadedText(3), 64, 100, 161, 0);
	_sound->playSoundEffectIndex(kSfxHailing);
	showTextbox("Lieutenant Uhura", _resource->getLoadedText(4), 298, 150, 161, 0);
	showTextbox("Captain Kirk", _resource->getLoadedText(5), 160, 130, 176, 0);
	const char *talker = "pira";
	if (_randomEncounterType == kRandomEncounterKlingon)
		talker = "klg1";
	else if (_randomEncounterType == kRandomEncounterRomulan)
		talker = "romula";
	showBridgeScreenTalkerWithMessage(6, "Captain", talker);
	_sound->playMidiMusicTracks(MIDITRACK_2, kLoopTypeRepeat);
	_sound->playSoundEffectIndex(kSfxRedAlert);

	while (hasEnemyShips() && !_resetGameMode && !shouldQuit())
		handleBridgeEvents();

	_randomEncounterType = 0;
	_resource->setTxtFileName(_missionName);
	_sound->loadMusicFile("bridge");
	if (isFloppyVersion())
		_sound->playMidiMusicTracks(MIDITRACK_0, kLoopTypeRepeat);
}

// Approaches a structure in space and orbits it: ark7, derelict, masada or republic
void StarTrekEngine::orbitPlanetSequence(int shape) {
	removeProjectiles();
	_enterpriseState.inOrbit = true;
	enterFullscreenView();

	_orbitObjectR3 = R3();
	_orbitObjectR3.pos = _planetR3.pos + Point3(-1000, 1000, -5000);
	_orbitObjectR3.matrix = initMatrix();
	_orbitObjectR3.field1e = 3;
	_orbitObjectR3.field20 = shape;
	_orbitObjectR3.field22 = 250;
	_orbitObjectR3.field24 = 2;
	addOrbitObject();

	// The Enterprise slows down until the structure is shown at twice its size
	_someMatrix = initMatrix();
	_enterpriseR3.matrix = initMatrix();
	_enterpriseR3.pos = _orbitObjectR3.pos;
	_viewDirection = 0;
	resetShipControls(&_enterpriseR3);
	int32 distance = _starfieldPointDivisor * 2;
	_enterpriseR3.pos.z -= distance + 9801;

	int16 speed = 197;
	for (int frame = 0; frame < 100; frame++) {
		_starfieldPosition = _enterpriseR3.pos;
		updateStarfieldAndShips(false);
		_gfx->drawAllSprites();
		rotateR3(&_planetR3, 1, 4);
		_frameIndex++;
		waitForNextTick(false);
		_enterpriseR3.pos.z += speed;
		speed -= 2;
	}

	leaveFullscreenView();
	_enterpriseR3.pos.z = _orbitObjectR3.pos.z - distance;
	_starfieldPosition = _enterpriseR3.pos;
	updateStarfieldAndShips(false);
	_gfx->drawAllSprites();
}

void StarTrekEngine::addOrbitObject() {
	static const char *const kShapes[] = { "ark7", "derelict", "masada", "republic" };
	_orbitObjectR3.bitmap = new Bitmap(_resource->loadFile(Common::String::format("%s.shp", kShapes[_orbitObjectR3.field20 - 4])));
	addR3(&_orbitObjectR3);
}

void StarTrekEngine::removeOrbitObject() {
	if (_orbitObjectR3.field1e != 0)
		delR3(&_orbitObjectR3);
}

void StarTrekEngine::playOrbitSequence(bool enterOrbit, bool showText) {
	const char *enteringOrbitText = "#BRID\\B_333#Entering standard orbit.";
	const char *leavingOrbitText = "#BRID\\B_335#Leaving orbit.";

	int16 zoom = _viewscreenZoom;
	_viewscreenZoom = 6;
	_gfx->fadeoutScreen();
	enterFullscreenView();
	if (enterOrbit)
		playOrbitArrivalSequence();
	else
		playOrbitDepartureSequence();
	leaveFullscreenView();
	updateStarfieldAndShips(false);
	_gfx->drawAllSprites();

	if (showText)
		showTextboxBridge(kBridgeTalkerSulu, enterOrbit ? enteringOrbitText : leavingOrbitText);
	_enterpriseState.inOrbit = enterOrbit;
	_viewscreenZoom = zoom;
}

// The warp ends in the system that the caller sets up
void StarTrekEngine::startBridgeWarp() {
	enterFullscreenView();
	_mouseControllingShip = false;
	playWarpSequence();
	_gfx->loadPalette("bridge");
}

void StarTrekEngine::endBridgeWarp() {
	leaveFullscreenView();
	setMouseControllingShip(true);
}

void StarTrekEngine::captainsLog() {
	const char *missionNames[] = {
		"Demon World",
		"Hijacked",
		"Love's Labor Jeopardized",
		"Another Fine Mess",
		"Feathered Serpent",
		"That Old Devil Moon",
		"Vengeance"
	};

	int totalPoints = 0;
	for (int i = 0; i < 7; i++)
		totalPoints += _missionPoints[i];

	Common::String captainsLogMessage = "";

	if (_lastMissionId > -1) {
		captainsLogMessage = localizeText("#BRID\\C_007#We have earned ");

		if (totalPoints == 1)
			captainsLogMessage += localizeText("1 point ");
		else
			captainsLogMessage += Common::String::format(localizeText("%d points "), totalPoints);

		captainsLogMessage += localizeText("for the following recently completed missions:\n\n");

		for (int i = 6; i >= 0; i--) {
			captainsLogMessage += localizeText(missionNames[i]);
			if (_missionPoints[i] == 1)
				captainsLogMessage += localizeText(":  1 point.\n\n");
			else
				captainsLogMessage += Common::String::format(localizeText(":  %d points.\n\n"), _missionPoints[i]);
		}
	} else {
		captainsLogMessage = localizeText("#BRID\\C_007#No recent missions have been completed.");
	}

	showTextboxBridge(kBridgeTalkerCaptainsLog, captainsLogMessage);
}

void StarTrekEngine::loadBridgeComputerTopics() {
	Common::MemoryReadStreamEndian *indexFile = _resource->loadFile("db.key");
	Common::String line;
	while (!indexFile->eos() && !indexFile->err()) {
		line = indexFile->readLine();
		if (line == "$")
			break;
		ComputerTopic topic;
		topic.fileName = line.substr(0, 7);
		topic.topic = line.substr(8);
		_computerTopics.push_back(topic);
	}
	delete indexFile;
}

void StarTrekEngine::handleBridgeComputer() {
	Common::String line;
	Common::String input = showComputerInputBox();
	if (input.size() < 3)
		return;

	char topics[10][8];
	int topicIndex = 0;

	memset(topics, 0, ARRAYSIZE(topics) * 8);

	for (Common::List<ComputerTopic>::const_iterator i = _computerTopics.begin(), end = _computerTopics.end(); i != end; ++i) {
		if (i->topic.hasPrefixIgnoreCase(input)) {
			memcpy(topics[topicIndex++], i->fileName.c_str(), i->fileName.size());
			if (topicIndex >= ARRAYSIZE(topics))
				break;
		}
	}

	showText(&StarTrekEngine::readTextFromFoundComputerTopics, (uintptr)topics, 20, 20, TEXTCOLOR_YELLOW, true, false, true);
}

} // End of namespace StarTrek
