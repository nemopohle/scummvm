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

#include "startrek/bridge.h"
#include "startrek/resource.h"
#include "startrek/startrek.h"

namespace StarTrek {

#define ACTOR_STANDING_SPOCK "xstndsp"
#define ACTOR_STANDING_MCCOY "xstndmc"
#define ACTOR_SITTING_SPOCK "bstndsp"

// The mission scripts get the bridge events first. They return true when the
// event is handled, otherwise the default bridge actions are used.
bool StarTrekEngine::handleMissionEvent(int type, int value) {
	// Random encounters have their own script
	if (_randomEncounterType != 0)
		return false;

	if (_missionState >= kMissionDemonStart && _missionState <= kMissionDemonEnd)
		return handleDemonEvent(type, value);
	if (_missionState >= kMissionTugStart && _missionState <= kMissionTugEnd)
		return handleTugEvent(type, value);
	if (_missionState >= kMissionLoveStart && _missionState <= kMissionLoveEnd)
		return handleLoveEvent(type, value);
	if (_missionState >= kMissionMuddStart && _missionState <= kMissionMuddDerelict)
		return handleMuddEvent(type, value);
	if (_missionState >= kMissionFeatherStart && _missionState <= kMissionFeatherBattle)
		return handleFeatherEvent(type, value);
	if (_missionState >= kMissionTrialStart && _missionState <= kMissionTrialBattle)
		return handleTrialEvent(type, value);
	if (_missionState >= kMissionSinsStart && _missionState <= kMissionSinsAlphaProxima)
		return handleSinsEvent(type, value);
	if (_missionState >= kMissionVengStart && _missionState <= kMissionVengBattle)
		return handleVengEvent(type, value);
	return false;
}

void StarTrekEngine::setMissionState(int16 state) {
	_missionState = state;
	_missionTicks = 0;
}

void StarTrekEngine::startMission(const char *name, const char *music) {
	_sound->loadMusicFile(music);
	resetShipForMission();
	_missionName = _missionToLoad = name;
	_resource->setTxtFileName(_missionName);
	Common::fill(_missionVars, _missionVars + ARRAYSIZE(_missionVars), 0);
}

void StarTrekEngine::playMissionFlyby(const char *sequence, const char *name) {
	showMissionStartEnterpriseFlyby(sequence, name);
	// The mission script continues after the flyby
	_missionTicks = 150;
}

void StarTrekEngine::resetShipForMission() {
	_emergencyPowerUsed = false;
	turnOffShieldsAndWeapons();
	for (int i = 0; i < ARRAYSIZE(_enterpriseR3.systems); i++)
		_enterpriseR3.systems[i].strength = _enterpriseR3.systems[i].maxStrength;
	_repairPriority = kRepairNone;
}

void StarTrekEngine::turnOffShieldsAndWeapons() {
	_enterpriseState.shields = false;
	_enterpriseState.weapons = false;
	setBridgeMouseCursor();
}

void StarTrekEngine::setCrewStanding(bool standing) {
	if (standing) {
		loadActorAnim(1, ACTOR_STANDING_SPOCK, 0, 0, 1.0);
		loadActorAnim(2, ACTOR_STANDING_MCCOY, 0, 0, 1.0);
	} else {
		loadActorAnim(1, ACTOR_SITTING_SPOCK, 0, 0, 1.0);
		removeActorFromScreen(2);
	}
}

void StarTrekEngine::stopMissionMusic() {
	if (_sound->getPlayingMidiTrack() != MIDITRACK_NONE)
		_sound->playMidiMusicTracks(MIDITRACK_NONE);
}

// Chapter 1: Demon world

bool StarTrekEngine::handleDemonEvent(int type, int value) {
	const char *lowerShieldsText = "#BRID\\B_336#Lowering shields and disarming weapons.";
	R3 &republic = _enemyShips[0];

	switch (_missionState) {
	case kMissionDemonStart:
		if (type != kMissionEventTick)
			return true;
		if (value == 1) {
			startMission("DEMON", "bridgew");
			playMissionFlyby("DEM0\\FLYBY", "demon");
		} else if (value == 151) {
			showTextboxBridge(kBridgeTalkerCaptainsLog, 0);
		} else if (value == 165) {
			showTextboxBridge(kBridgeTalkerUhura, 1);
			showTextboxBridge(kBridgeTalkerSpock, 2);
			// The U.S.S. Republic is the opponent of the mock battle
			spawnEnemyShip("enterpri");
			_missionState = kMissionDemonMockBattle;
			setMouseControllingShip(true);
			_sound->playMidiMusicTracks(MIDITRACK_2, kLoopTypeRepeat);
			_sound->playSoundEffectIndex(kSfxRedAlert);
		}
		return true;

	case kMissionDemonMockBattle: {
		// The battle ends when a ship has lost half of its hull
		const ShipSystem &republicHull = republic.systems[6];
		const ShipSystem &hull = _enterpriseR3.systems[6];
		if (republicHull.strength < (republicHull.maxStrength >> 1)) {
			_sound->playMidiMusicTracks(MIDITRACK_3);
			showTextboxBridge(kBridgeTalkerSpock, 4);
			showTextboxBridge(kBridgeTalkerUhura, 5);
		} else if (hull.strength < (hull.maxStrength >> 1)) {
			_sound->playMidiMusicTracks((MidiTracks)4);
			showTextboxBridge(kBridgeTalkerSpock, 6);
			showTextboxBridge(kBridgeTalkerScotty, 7);
			showTextboxBridge(kBridgeTalkerKirk, 8);
			showTextboxBridge(kBridgeTalkerUhura, 9);
		} else if (type == kMissionEventMenu && value == kBridgeUhura) {
			showTextboxBridge(kBridgeTalkerUhura, 10);
			return true;
		} else if (type == kMissionEventMenu && value == kBridgeSpock) {
			showTextboxBridge(kBridgeTalkerSpock, 11);
			return true;
		} else {
			return false;
		}

		removeProjectiles();
		resetShipForMission();
		showSuluText(lowerShieldsText, 0xb0);
		for (int i = 8; i < 12; i++)
			republic.systems[i].present = 0;
		republic.retreating = true;
		setMissionState(kMissionDemonAfterBattle);
		return true;
	}

	case kMissionDemonAfterBattle:
		if (type == kMissionEventTick && value == 20) {
			_sound->playSoundEffectIndex(kSfxHailing);
			showTextboxBridge(kBridgeTalkerUhura, 13);
			showTextboxBridge(kBridgeTalkerKirk, 14);
			if (republic.field1e == 2)
				delR3(&republic);
			showBridgeScreenTalkerWithMessage(12, "Admiral", "woman");
			setMissionState(kMissionDemonPollux);
			_targetPlanet = kPlanetPollux;
		}
		return true;

	case kMissionDemonPollux:
		if (type == kMissionEventReturn) {
			setMissionState(kMissionDemonEnd);
			return true;
		}
		if (type != kMissionEventMenu)
			return false;

		switch (value) {
		case kBridgeChekovNavigation: {
			bool arrived = _currentPlanet == _targetPlanet;
			dispatchBridgeMenu(value);
			if (!arrived && _currentPlanet == _targetPlanet)
				showTextboxBridge(kBridgeTalkerSpock, 15);
			return true;
		}
		case kBridgeUhura:
			if (_enterpriseState.underAttack)
				return false;
			if (_currentPlanet != _targetPlanet) {
				showTextboxBridge(kBridgeTalkerUhura, 16);
			} else if (!_missionVars[0]) {
				_sound->playSoundEffectIndex(kSfxHailing);
				showTextboxBridge(kBridgeTalkerUhura, 17);
				showBridgeScreenTalkerWithMessage(18, "Priest", "prst");
				_missionVars[0] = 1;
			} else {
				showTextboxBridge(kBridgeTalkerUhura, 19);
			}
			return true;
		case kBridgeSpock:
			if (_enterpriseState.underAttack)
				return false;
			if (_currentPlanet != _targetPlanet)
				showTextboxBridge(kBridgeTalkerSpock, 20);
			else if (!_enterpriseState.inOrbit)
				showTextboxBridge(kBridgeTalkerSpock, 21);
			else
				showTextboxBridge(kBridgeTalkerSpock, 22);
			return true;
		default:
			return false;
		}

	case kMissionDemonEnd:
		if (type != kMissionEventTick)
			return true;
		if (value == 1) {
			_bridgeInputLocked = true;
			setCrewStanding(true);
		} else if (value == 10) {
			_sound->playSoundEffectIndex(kSfxHailing);
			showTextboxBridge(kBridgeTalkerUhura, 23);
			showTextboxBridge(kBridgeTalkerKirk, 24);
			showMissionPerformance(0, 29, _missionScore, _missionMaxScore);
			_missionTicks = 500;
		} else if (value == 520) {
			showTextboxBridge(kBridgeTalkerMcCoy, 25);
			showTextboxBridge(kBridgeTalkerKirk, 26);
			showTextboxBridge(kBridgeTalkerSpock, 27);
			showTextboxBridge(kBridgeTalkerMcCoy, 28);
			playOrbitSequence(false, false);
			setCrewStanding(false);
			_bridgeInputLocked = false;
			setMissionState(kMissionTugStart);
		}
		return true;

	default:
		return false;
	}
}

// Chapter 2: Hijacked

bool StarTrekEngine::handleTugEvent(int type, int value) {
	R3 &pirate = _enemyShips[0];

	switch (_missionState) {
	case kMissionTugStart:
		_targetPlanet = kPlanetBetaMyamid;
		if (_currentPlanet == _targetPlanet) {
			setMissionState(kMissionTugPirate);
			return type == kMissionEventTick || handleTugEvent(type, value);
		}

		if (type == kMissionEventTick) {
			if (value == 1) {
				startMission("TUG", "bridge");
				_missionScore = 0;
				_missionMaxScore = 32;
				_missionEndFlag = 0;
				playMissionFlyby("TUG0\\FLYBY", "hijacked");
			} else if (value == 151) {
				_sound->playMidiMusicTracks(MIDITRACK_0);
				_sound->playSoundEffectIndex(kSfxHailing);
				showTextboxBridge(kBridgeTalkerUhura, 0);
				showTextboxBridge(kBridgeTalkerKirk, 1);
			} else if (value == 171) {
				showBridgeScreenTalkerWithMessage(2, "Admiral", "woman");
			} else if (value == 176) {
				showTextboxBridge(kBridgeTalkerChekov, 3);
			}
			return true;
		}
		if (type == kMissionEventMenu && value == kBridgeUhura) {
			showTextboxBridge(kBridgeTalkerUhura, 4);
			return true;
		}
		if (type == kMissionEventMenu && value == kBridgeSpock) {
			showTextboxBridge(kBridgeTalkerSpock, 5);
			return true;
		}
		return false;

	case kMissionTugPirate: {
		bool handled = true;
		if (type == kMissionEventTick) {
			if (value == 33) {
				showTextboxBridge(kBridgeTalkerSpock, 6);
			} else if (value == 65) {
				_sound->playSoundEffectIndex(kSfxHailing);
				showTextboxBridge(kBridgeTalkerUhura, 7);
				showTextboxBridge(kBridgeTalkerKirk, 8);
				showBridgeScreenTalker("pira");
				showTextboxBridge(kBridgeTalkerElasiCaptain, 9);
				showTextboxBridge(kBridgeTalkerKirk, 10);
				restartBridgeScreenTalker();
				showTextboxBridge(kBridgeTalkerElasiCaptain, 11);
				removeBridgeScreenTalker();
				_sound->playSoundEffectIndex(kSfxRedAlert);
			} else if (value == 129) {
				spawnEnemyShip("orion");
				_enterpriseState.underAttack = true;
				_sound->playMidiMusicTracks(MIDITRACK_2, kLoopTypeRepeat);
			}
		} else if (type == kMissionEventMenu && value == kBridgeSpock) {
			showTextboxBridge(kBridgeTalkerSpock, 13);
		} else if (type == kMissionEventMenu && value == kBridgeUhura) {
			showTextboxBridge(kBridgeTalkerUhura, 12);
		} else if (type != kMissionEventMenu || value != kBridgeSuluOrbit) {
			handled = false;
		}

		// The pirate flees when its hull is damaged
		const ShipSystem &hull = pirate.systems[6];
		if (_enterpriseState.underAttack && hull.strength < (hull.maxStrength >> 1)) {
			_sound->playMidiMusicTracks(MIDITRACK_3);
			showTextboxBridge(kBridgeTalkerChekov, 14);
			showTextboxBridge(kBridgeTalkerSpock, 15);
			for (int i = 8; i < 12; i++)
				pirate.systems[i].present = 0;
			pirate.retreating = true;
			setMissionState(kMissionTugPirateFled);
		}
		return handled;
	}

	case kMissionTugPirateFled:
		if (type != kMissionEventMenu)
			return false;

		switch (value) {
		case kBridgeSuluOrbit:
			if (pirate.field1e != 0)
				delR3(&pirate);
			orbitPlanetSequence(6);
			setMissionState(kMissionTugMasada);
			return true;
		case kBridgeUhura:
			showTextboxBridge(kBridgeTalkerUhura, 16);
			return true;
		case kBridgeSpock:
			showTextboxBridge(kBridgeTalkerSpock, 17);
			return true;
		default:
			return false;
		}

	case kMissionTugMasada:
		// Variables: killed hostages, prefix code sent, code suggested, Elasi contacted
		if (type == kMissionEventReturn) {
			setMissionState(kMissionTugEnd);
			return true;
		}
		if (type == kMissionEventTick) {
			if (value == 17)
				showTextbox("Mr. Spock", _resource->getLoadedText(20), 160, 190, 44, 0);
			return true;
		}

		switch (value) {
		case kBridgeUhura:
			hailTheMasada();
			return true;
		case kBridgeSpock:
			showTextboxBridge(kBridgeTalkerSpock, 27);
			_missionVars[2] = 1;
			return true;
		case kBridgeKirkTransporter:
			if (_missionVars[1])
				return false;
			showSuluText(_resource->getLoadedText(28), 0xa1);
			return true;
		default:
			return false;
		}

	case kMissionTugEnd:
		if (type != kMissionEventTick)
			return true;
		if (value == 1) {
			_bridgeInputLocked = true;
			if (_missionEndFlag != 2)
				setCrewStanding(true);
			if (_missionEndFlag == 1)
				removeOrbitObject();
		} else if (value == 20) {
			_sound->playSoundEffectIndex(kSfxHailing);
			if (_missionEndFlag == 0) {
				showTextboxBridge(kBridgeTalkerUhura, 29);
				showTextboxBridge(kBridgeTalkerKirk, 30);
				showMissionPerformance(1, 31, _missionScore, _missionMaxScore);
				showTextboxBridge(kBridgeTalkerMcCoy, 32);
				showTextboxBridge(kBridgeTalkerSpock, 33);
				showTextboxBridge(kBridgeTalkerMcCoy, 34);
				showTextboxBridge(kBridgeTalkerKirk, 35);
				showTextboxBridge(kBridgeTalkerSpock, 36);
				showTextboxBridge(kBridgeTalkerKirk, 37);
			} else if (_missionEndFlag == 1) {
				// The Masada was destroyed
				showTextboxBridge(kBridgeTalkerUhura, 39);
				showTextboxBridge(kBridgeTalkerKirk, 40);
				showMissionPerformance(1, 41, 0, 0);
			} else {
				// Too many hostages were killed
				showTextboxBridge(kBridgeTalkerUhura, 42);
				showMissionPerformance(1, 43, 0, 0);
			}
			removeOrbitObject();
			playOrbitSequence(false, false);
			setCrewStanding(false);
			_bridgeInputLocked = false;
			setMissionState(kMissionLoveStart);
		}
		return true;

	default:
		return false;
	}
}

void StarTrekEngine::hailTheMasada() {
	// Kirk's choices, from tug.txt: "Hail the masada.", "Send prefix code."
	const int options[] = { 18, 19, -1 };

	int choice = _missionVars[2] ? showKirkChoices(options) : 0;
	if (choice == 0) {
		showTextboxBridge(kBridgeTalkerUhura, 21);
		if (!_missionVars[3]) {
			negotiateWithElasiCereth();
			_missionVars[3] = 1;
		} else {
			showBridgeScreenTalker("pira");
			showTextboxBridge(kBridgeTalkerElasiCereth, 22);
			showTextboxBridge(kBridgeTalkerKirk, 23);
			removeBridgeScreenTalker();
			_missionVars[0]++;
			showTextboxBridge(kBridgeTalkerSpock, 60);
		}

		if (_missionVars[0] > 4) {
			_missionEndFlag = 2;
			setMissionState(kMissionTugEnd);
		}
	} else if (choice == 1) {
		if (showCodeInputBox() == "293391-197736-3829") {
			showTextboxBridge(kBridgeTalkerUhura, 25);
			_missionVars[1] = 1;
		} else {
			showTextboxBridge(kBridgeTalkerUhura, 26);
		}
	}
}

void StarTrekEngine::negotiateWithElasiCereth() {
	// Kirk's choices, from tug.txt
	const int options1[] = { 66, 67, 68, -1 };
	const int options2[] = { 48, 49, -1 };
	const int options3[] = { 55, 56, 57, -1 };

	showBridgeScreenTalker("pira");
	showTextboxBridge(kBridgeTalkerElasiCereth, 65);

	int choice = showKirkChoices(options1);
	restartBridgeScreenTalker();
	if (choice == 1) {
		_missionVars[0]++;
		showTextboxBridge(kBridgeTalkerElasiCereth, 46);
		showTextboxBridge(kBridgeTalkerSpock, 60);
	} else {
		if (choice == 2) {
			showTextboxBridge(kBridgeTalkerElasiCereth, 47);
			restartBridgeScreenTalker();
		}
		showTextboxBridge(kBridgeTalkerElasiCereth, 44);
		showTextboxBridge(kBridgeTalkerUhura, 45);
	}

	choice = showKirkChoices(options2);
	restartBridgeScreenTalker();
	if (choice == 0 || choice == 1) {
		showTextboxBridge(kBridgeTalkerElasiCereth, 51);
		showTextboxBridge(kBridgeTalkerScotty, 52);
		showTextboxBridge(kBridgeTalkerSulu, 53);
		restartBridgeScreenTalker();
		showTextboxBridge(kBridgeTalkerElasiCereth, 54);
	} else {
		showTextboxBridge(kBridgeTalkerElasiCereth, 50);
	}

	choice = showKirkChoices(options3);
	if (choice == 1) {
		restartBridgeScreenTalker();
		showTextboxBridge(kBridgeTalkerElasiCereth, 69);
	} else if (choice != 0) {
		if (choice != 2)
			showTextboxBridge(kBridgeTalkerKirk, 58);
		restartBridgeScreenTalker();
		showTextboxBridge(kBridgeTalkerElasiCereth, 70);
		_missionVars[0]++;
		showTextboxBridge(kBridgeTalkerSpock, 60);
		restartBridgeScreenTalker();
		showTextboxBridge(kBridgeTalkerElasiCereth, 71);
	}

	removeBridgeScreenTalker();
}

// Chapter 3: Love's Labor Jeopardized

bool StarTrekEngine::handleLoveEvent(int type, int value) {
	R3 &romulan = _enemyShips[0];

	switch (_missionState) {
	case kMissionLoveStart:
		if (type != kMissionEventTick)
			return true;
		if (value == 1) {
			startMission("LOVE", "bridge");
			playMissionFlyby("LOV0\\FLYBY", "tlove");
		} else if (value == 151) {
			_sound->playSoundEffectIndex(kSfxHailing);
			showTextboxBridge(kBridgeTalkerUhura, 0);
			showTextboxBridge(kBridgeTalkerKirk, 1);
			showBridgeScreenTalkerWithMessage(2, "Admiral", "woman");
			setMissionState(kMissionLoveEnRoute);
			_targetPlanet = kPlanetArk7;
		}
		return true;

	case kMissionLoveEnRoute:
		// The music of a random encounter stops after the battle
		if (!_enterpriseState.underAttack && _sound->getPlayingMidiTrack() == MIDITRACK_2)
			_sound->playMidiMusicTracks(MIDITRACK_NONE);
		if (type != kMissionEventMenu)
			return false;

		switch (value) {
		case kBridgeChekovNavigation: {
			bool arrived = _currentPlanet == _targetPlanet;
			dispatchBridgeMenu(value);
			if (!arrived && _currentPlanet == _targetPlanet) {
				showTextboxBridge(kBridgeTalkerSpock, 3);
				setMissionState(kMissionLoveAmbush);
				_missionVars[0] = 1;
			}
			return true;
		}
		case kBridgeSpock:
			showTextboxBridge(kBridgeTalkerSpock, 4);
			return true;
		case kBridgeUhura:
			if (_enterpriseState.underAttack)
				return false;
			showTextboxBridge(kBridgeTalkerUhura, 5);
			return true;
		default:
			return false;
		}

	case kMissionLoveAmbush:
		// Variable 0 blocks the menus until the Romulans have spoken
		if (type == kMissionEventTick) {
			if (value == 1) {
				spawnEnemyShip("romulan");
				romulan.systems[8].present = 0;
				showTextboxBridge(kBridgeTalkerSpock, "#BRID\\B_160#Romulan ship decloaking, Captain.");
				_sound->playSoundEffectIndex(kSfxHailing);
				showTextboxBridge(kBridgeTalkerUhura, 6);
				_missionVars[0] = 0;
				showBridgeScreenTalkerWithMessage(9, "Romulan Captain", "romula");
				showTextboxBridge(kBridgeTalkerSpock, 10);
				romulan.systems[8].present = 1;
				_sound->playMidiMusicTracks(MIDITRACK_2, kLoopTypeRepeat);
				_sound->playSoundEffectIndex(kSfxRedAlert);
			} else if (!_enterpriseState.underAttack) {
				setMissionState(kMissionLoveAfterBattle);
			}
			return true;
		}
		if (type != kMissionEventMenu)
			return false;
		if (value == kBridgeUhura) {
			if (!_missionVars[0])
				showTextboxBridge(kBridgeTalkerUhura, 7);
			return true;
		}
		if (value == kBridgeSpock) {
			if (!_missionVars[0])
				showTextboxBridge(kBridgeTalkerSpock, 8);
			return true;
		}
		return _missionVars[0] != 0;

	case kMissionLoveAfterBattle:
		if (type != kMissionEventTick)
			return false;
		if (value == 1) {
			showTextboxBridge(kBridgeTalkerSpock, 11);
			showTextboxBridge(kBridgeTalkerSulu, 12);
			orbitPlanetSequence(4);
			setMissionState(kMissionLoveArk7);
		}
		return true;

	case kMissionLoveArk7:
		// Variable 1 is set when ARK7 was hailed
		if (type == kMissionEventReturn) {
			setMissionState(kMissionLoveEnd);
			return true;
		}
		if (type == kMissionEventTick) {
			if (value == 10) {
				showTextboxBridge(kBridgeTalkerUhura, 17);
				showTextboxBridge(kBridgeTalkerAutobeacon, 16);
			}
			return true;
		}
		if (value == kBridgeSpock) {
			showTextboxBridge(kBridgeTalkerSpock, 13);
			return true;
		}
		if (value == kBridgeUhura && !_missionVars[1]) {
			_sound->playSoundEffectIndex(kSfxHailing);
			showTextboxBridge(kBridgeTalkerUhura, 14);
			showBridgeScreenTalkerWithMessage(15, "Centurion Preax", "romula");
			_missionVars[1] = 1;
			return true;
		}
		return false;

	case kMissionLoveEnd:
		if (type != kMissionEventTick)
			return true;
		if (value == 1) {
			_bridgeInputLocked = true;
			setCrewStanding(true);
		} else if (value == 10) {
			_sound->playSoundEffectIndex(kSfxHailing);
			showTextboxBridge(kBridgeTalkerUhura, 18);
			showTextboxBridge(kBridgeTalkerKirk, 19);
			showMissionPerformance(2, 26, _missionScore, _missionMaxScore);
			_missionTicks = 500;
		} else if (value == 520) {
			showTextboxBridge(kBridgeTalkerKirk, 20);
			showTextboxBridge(kBridgeTalkerMcCoy, 21);
			showTextboxBridge(kBridgeTalkerKirk, 22);
			showTextboxBridge(kBridgeTalkerMcCoy, 23);
			showTextboxBridge(kBridgeTalkerScotty, 24);
			showTextboxBridge(kBridgeTalkerKirk, 25);
			removeOrbitObject();
			playOrbitSequence(false, false);
			setCrewStanding(false);
			_bridgeInputLocked = false;
			setMissionState(kMissionMuddStart);
		}
		return true;

	default:
		return false;
	}
}

// Chapter 4: Another Fine Mess

bool StarTrekEngine::handleMuddEvent(int type, int value) {
	// Variables: briefing done, battle started, Mudd called, navigated,
	// back from the away mission, ticks until the debriefing
	switch (_missionState) {
	case kMissionMuddStart:
		if (type == kMissionEventTick) {
			if (value == 1) {
				startMission("MUDD", "bridge");
				playMissionFlyby("MUD0\\FLYBY", "another");
			} else if (value == 151) {
				_sound->playSoundEffectIndex(kSfxHailing);
				showTextboxBridge(kBridgeTalkerUhura, 0);
				showTextboxBridge(kBridgeTalkerKirk, 1);
				showBridgeScreenTalkerWithMessage(2, "Admiral", "woman");
				_missionVars[0] = 1;
				_targetPlanet = kPlanetHarlequin;
				setMouseControllingShip(true);
			}
			return true;
		}
		if (type != kMissionEventMenu)
			return false;
		if (!_missionVars[0])
			return true;

		switch (value) {
		case kBridgeUhura:
			if (_missionVars[3])
				return false;
			showTextboxBridge(kBridgeTalkerUhura, 3);
			return true;
		case kBridgeSpock:
			if (_missionVars[3])
				return false;
			showTextboxBridge(kBridgeTalkerSpock, 4);
			return true;
		case kBridgeChekovNavigation:
			_missionVars[3] = 1;
			dispatchBridgeMenu(value);
			if (_currentPlanet == kPlanetHarlequin) {
				showTextboxBridge(kBridgeTalkerSpock, 5);
				setMissionState(kMissionMuddHarlequin);
			}
			return true;
		default:
			return false;
		}

	case kMissionMuddHarlequin:
		if (type == kMissionEventTick) {
			if (value == 10) {
				_missionVars[1] = 0;
				showTextboxBridge(kBridgeTalkerSpock, 6);
				_sound->playSoundEffectIndex(kSfxHailing);
				showTextboxBridge(kBridgeTalkerUhura, 7);
				showBridgeScreenTalkerWithMessage(14, "Elasi Pirate", "pira");
				spawnEnemyShip("orion");
				spawnEnemyShip("orion");
				_missionVars[1] = 1;
				_sound->playMidiMusicTracks(MIDITRACK_2, kLoopTypeRepeat);
				return true;
			}

			if (_missionVars[1] && !_enterpriseState.underAttack && !_missionVars[2]) {
				stopMissionMusic();
				showTextboxBridge(kBridgeTalkerSpock, 8);
				showTextboxBridge(kBridgeTalkerUhura, 9);
				showTextboxBridge(kBridgeTalkerKirk, 10);
				showTextboxBridge(kBridgeTalkerHarryMuddsVoice, 11);
				showTextboxBridge(kBridgeTalkerKirk, 12);
				showTextboxBridge(kBridgeTalkerUhura, 13);
				_missionVars[2] = 1;
				_targetPlanet = kPlanetHarrapa;

				showBridgeScreenTalker("mudd");
				showTextboxBridge(kBridgeTalkerHarryMudd, 15);
				showTextboxBridge(kBridgeTalkerKirk, 16);
				restartBridgeScreenTalker();
				showTextboxBridge(kBridgeTalkerHarryMudd, 17);
				removeBridgeScreenTalker();
				showTextboxBridge(kBridgeTalkerSpock, 18);
			}

			// The pirates flee when their hulls are damaged
			if (_enterpriseState.underAttack) {
				for (int i = 0; i < 2; i++) {
					R3 &ship = _enemyShips[i];
					if (ship.field1e != 0 && ship.systems[6].strength * 2 <= ship.systems[6].maxStrength)
						ship.retreating = true;
				}
			}
			return true;
		}
		if (type != kMissionEventMenu)
			return false;

		switch (value) {
		case kBridgeUhura:
			if (_enterpriseState.underAttack)
				showTextboxBridge(kBridgeTalkerUhura, 19);
			else if (_missionVars[2])
				showTextboxBridge(kBridgeTalkerUhura, 20);
			else
				return false;
			return true;
		case kBridgeSpock:
			if (_enterpriseState.underAttack)
				showTextboxBridge(kBridgeTalkerSpock, 21);
			else if (_missionVars[2])
				showTextboxBridge(kBridgeTalkerSpock, 22);
			else
				return false;
			return true;
		case kBridgeChekovNavigation:
			if (!_missionVars[2])
				return true;
			dispatchBridgeMenu(value);
			if (_currentPlanet == _targetPlanet) {
				orbitPlanetSequence(5);
				setMissionState(kMissionMuddDerelict);
			}
			return true;
		default:
			return false;
		}

	case kMissionMuddDerelict:
		if (type == kMissionEventReturn) {
			_missionVars[4] = 1;
			_missionVars[5] = 10;
			setCrewStanding(true);
			_bridgeInputLocked = true;
			// Mudd doesn't call when the landing party beamed over before
			if (_missionTicks <= 100)
				_missionTicks = 101;
			return true;
		}

		if (type == kMissionEventTick) {
			if (value == 100) {
				showTextboxBridge(kBridgeTalkerSpock, 23);
				showTextboxBridge(kBridgeTalkerKirk, 24);
				showTextboxBridge(kBridgeTalkerSpock, 25);
				_sound->playSoundEffectIndex(kSfxHailing);
				showTextboxBridge(kBridgeTalkerUhura, 26);
				showBridgeScreenTalker("mudd");
				showTextboxBridge(kBridgeTalkerHarryMudd, 27);
				showTextboxBridge(kBridgeTalkerKirk, 28);
				restartBridgeScreenTalker();
				showTextboxBridge(kBridgeTalkerHarryMudd, 29);
				showTextboxBridge(kBridgeTalkerSpock, 30);
				restartBridgeScreenTalker();
				showTextboxBridge(kBridgeTalkerHarryMudd, 31);
				showTextboxBridge(kBridgeTalkerKirk, 32);
				restartBridgeScreenTalker();
				showTextboxBridge(kBridgeTalkerHarryMudd, 33);
				showTextboxBridge(kBridgeTalkerKirk, 34);
				removeBridgeScreenTalker();
			}

			if (_missionVars[5] == 1) {
				_missionVars[5] = 0;
				showMissionPerformance(3, 35, _missionScore, _missionMaxScore);
				if (_missionEndFlag) {
					// The alien contraption was brought aboard
					showTextboxBridge(kBridgeTalkerScotty, 36);
					showTextboxBridge(kBridgeTalkerKirk, 37);
				} else {
					showTextboxBridge(kBridgeTalkerKirk, 38);
				}
				showTextboxBridge(kBridgeTalkerUhura, 41);
				showTextboxBridge(kBridgeTalkerKirk, 42);
				showTextboxBridge(kBridgeTalkerUhura, 43);
				showTextboxBridge(kBridgeTalkerKirk, 44);
				removeOrbitObject();
				playOrbitSequence(false, false);
				setCrewStanding(false);
				_bridgeInputLocked = false;
				setMissionState(kMissionFeatherStart);
			} else if (_missionVars[5] != 0) {
				_missionVars[5]--;
			}
			return true;
		}

		if (_missionVars[4])
			return true;
		switch (value) {
		case kBridgeUhura:
		case kBridgeChekovNavigation:
			showTextboxBridge(kBridgeTalkerUhura, 45);
			return true;
		case kBridgeSpock:
			showTextboxBridge(kBridgeTalkerSpock, 46);
			return true;
		case kBridgeSuluOrbit:
			return true;
		default:
			return false;
		}

	default:
		return false;
	}
}

// Chapter 5A: The Feathered Serpent

bool StarTrekEngine::handleFeatherEvent(int type, int value) {
	// Variables: fighting the Nizra, arrived at Digifal, ticks until the Nizra arrives
	R3 &nizra = _enemyShips[0];

	switch (_missionState) {
	case kMissionFeatherStart:
		if (type == kMissionEventTick) {
			if (value == 1) {
				startMission("FEATHER", "bridgeb");
				_targetPlanet = kPlanetDigifal;
				playMissionFlyby("FEA0\\FLYBY", "feather");
			} else if (value == 151) {
				showTextboxBridge(kBridgeTalkerKirk, 5);
			}

			// More Klingons arrive when the shields of the Nizra are weakened
			if (_missionVars[0] == 1) {
				int strength = 0;
				int maxStrength = 0;
				for (int i = 0; i < 6; i++) {
					strength += nizra.systems[i].strength;
					maxStrength += nizra.systems[i].maxStrength;
				}
				if (strength * 3 < maxStrength * 2) {
					nizra.retreating = true;
					spawnEnemyShip("klingon");
				}
			}

			if (_missionVars[1] && !_missionVars[0]) {
				if (_missionVars[2])
					_missionVars[2]--;
				else
					negotiateWithTaraz();
			}
			return true;
		}
		if (type != kMissionEventMenu)
			return false;

		switch (value) {
		case kBridgeFirePhasers:
		case kBridgeFireTorpedoes:
			// Firing at the Nizra starts the battle
			if (!_missionVars[0] && _enterpriseState.underAttack && _enterpriseR3.weaponsActive) {
				_missionVars[0] = 1;
				showBridgeScreenTalkerWithMessage(12, "Taraz", "klg1");
				nizra.systems[8].present = 1;
				nizra.systems[10].present = 1;
				nizra.systems[11].present = 1;
			}
			return false;
		case kBridgeUhura:
			showTextboxBridge(kBridgeTalkerUhura, 13);
			return true;
		case kBridgeSuluOrbit:
			if (_missionVars[0] || _currentPlanet != _targetPlanet)
				return false;
			setMissionState(kMissionFeatherOrbit);
			return true;
		case kBridgeChekovNavigation:
			if (_currentPlanet == _targetPlanet) {
				if (!_missionVars[0])
					_enterpriseState.underAttack = false;
				return false;
			}
			dispatchBridgeMenu(value);
			if (_currentPlanet == _targetPlanet) {
				_missionVars[1] = 1;
				_missionVars[2] = 30;
			}
			return true;
		default:
			return false;
		}

	case kMissionFeatherOrbit:
		// The peaceful Nizra doesn't block the menus
		_enterpriseState.underAttack = false;
		if (type == kMissionEventReturn) {
			playOrbitSequence(false, false);
			showTextboxBridge(kBridgeTalkerUhura, 21);
			_sound->playSoundEffectIndex(kSfxHailing);
			showTextboxBridge(kBridgeTalkerUhura, 22);
			negotiateWithKenka();
			return true;
		}
		if (type == kMissionEventTick) {
			if (value == 1)
				playOrbitSequence(true, true);
			return true;
		}

		switch (value) {
		case kBridgeSpock:
			showTextboxBridge(kBridgeTalkerSpock, 20);
			return true;
		case kBridgeSuluOrbit:
			showSuluText(_resource->getLoadedText(35), 0x2c);
			return true;
		default:
			return false;
		}

	case kMissionFeatherBattle:
		// The Klingons never stop coming
		spawnEnemyShip("klingon");
		return false;

	default:
		return false;
	}
}

void StarTrekEngine::negotiateWithTaraz() {
	const int options1[] = { 0, 1, 2, -1 };
	const int options2[] = { 3, 4, -1 };
	R3 &nizra = _enemyShips[0];

	_missionVars[1] = 0;
	spawnEnemyShip("klingon");
	nizra.systems[8].present = 0;
	nizra.systems[10].present = 0;
	nizra.systems[11].present = 0;
	if (!_viewscreenFullscreen)
		updateBridgeDisplays();
	updateStarfieldAndShips(false);
	_gfx->drawAllSprites();

	showTextboxBridge(kBridgeTalkerUhura, 6);
	_sound->playSoundEffectIndex(kSfxHailing);
	showTextboxBridge(kBridgeTalkerKirk, 7);
	showBridgeScreenTalker("klg1");
	showTextboxBridge(kBridgeTalkerTaraz, 8);

	if (showKirkChoices(options1) == 1) {
		showTextboxBridge(kBridgeTalkerTaraz, 11);
		if (showKirkChoices(options2) != 1) {
			// The Nizra stays peacefully
			removeBridgeScreenTalker();
			return;
		}
	}

	showTextboxBridge(kBridgeTalkerTaraz, 9);
	removeBridgeScreenTalker();
	_sound->playMidiMusicTracks(MIDITRACK_2, kLoopTypeRepeat);
	_sound->playSoundEffectIndex(kSfxRedAlert);
	showTextboxBridge(kBridgeTalkerSpock, 10);
	_missionVars[0] = 1;
	nizra.systems[8].present = 1;
	nizra.systems[10].present = 1;
	nizra.systems[11].present = 1;
}

void StarTrekEngine::negotiateWithKenka() {
	const int options1[] = { 14, 15, 16, -1 };
	const int options2[] = { 17, 18, 19, -1 };

	showBridgeScreenTalker("klg2");
	showTextboxBridge(kBridgeTalkerAdmiralKenka, 23);
	if (showKirkChoices(options1) != 2) {
		showTextboxBridge(kBridgeTalkerAdmiralKenka, 24);
		showTextboxBridge(kBridgeTalkerKirk, 25);
		showTextboxBridge(kBridgeTalkerAdmiralKenka, 26);
		if (showKirkChoices(options2) != 2) {
			showTextboxBridge(kBridgeTalkerAdmiralKenka, 27);
			_sound->playSoundEffectIndex(kSfxHailing);
			showTextboxBridge(kBridgeTalkerUhura, 28);
			showTextboxBridge(kBridgeTalkerKirk, 29);
			showTextboxBridge(kBridgeTalkerAdmiralKenka, 30);
			removeBridgeScreenTalker();
			showBridgeScreenTalkerWithMessage(31, "Admiral", "woman");
			showTextboxBridge(kBridgeTalkerScotty, 32);
			showTextboxBridge(kBridgeTalkerKirk, 33);

			// Quetzecoatl is taken to the Klingons
			startBridgeWarp();
			if (_enemyShips[0].field1e == 2)
				delR3(&_enemyShips[0]);
			_currentPlanet = _targetPlanet = kPlanetHrakkour;
			initSystemPlanet(kPlanetHrakkour);
			endBridgeWarp();
			setMissionState(kMissionTrialStart);
			return;
		}
	}

	removeBridgeScreenTalker();
	showSuluText(_resource->getLoadedText(34), 0xb0);
	_sound->playMidiMusicTracks(MIDITRACK_2, kLoopTypeRepeat);
	_sound->playSoundEffectIndex(kSfxRedAlert);
	spawnEnemyShip("K'LRTA");
	setMissionState(kMissionFeatherBattle);
}

// Chapter 5B: The Feathered Serpent

bool StarTrekEngine::handleTrialEvent(int type, int value) {
	// Variables 1 and 2 are the score and the maximum score of the first part
	switch (_missionState) {
	case kMissionTrialStart:
		if (type == kMissionEventReturn) {
			playOrbitSequence(false, false);
			setMissionState(kMissionTrialEnd);
			return true;
		}
		if (type == kMissionEventMenu)
			return value != kBridgeKirkTransporter;

		if (value == 1) {
			_currentPlanet = _targetPlanet = kPlanetHrakkour;
			_missionName = _missionToLoad = "TRIAL";
			_resource->setTxtFileName(_missionName);
			Common::fill(_missionVars, _missionVars + ARRAYSIZE(_missionVars), 0);
			_missionVars[1] = _missionScore;
			_missionVars[2] = _missionMaxScore;
			playOrbitSequence(true, false);
		} else if (value == 50) {
			showTextboxBridge(kBridgeTalkerKirk, 0);
			// The landing party beams down at once
			dispatchBridgeMenu(kBridgeKirkTransporter);
		}
		return true;

	case kMissionTrialEnd:
		if (type != kMissionEventTick || value != 20)
			return true;

		_bridgeInputLocked = true;
		switch (_missionEndFlag) {
		case 0:
			// The landing party left during the trial
			_sound->playSoundEffectIndex(kSfxHailing);
			showTextboxBridge(kBridgeTalkerUhura, 1);
			showBridgeScreenTalkerWithMessage(2, "Vlict Kenka", "klg2");
			showTextboxBridge(kBridgeTalkerUhura, 3);
			showTextboxBridge(kBridgeTalkerKirk, 4);
			break;
		case 1:
		case 2:
			// Quetzecoatl died
			showTextboxBridge(kBridgeTalkerKirk, 5);
			showTextboxBridge(kBridgeTalkerSpock, 6);
			showTextboxBridge(kBridgeTalkerKirk, 7);
			showTextboxBridge(kBridgeTalkerSpock, 8);
			showTextboxBridge(kBridgeTalkerKirk, 9);
			break;
		case 3:
			showTextboxBridge(kBridgeTalkerSulu, 10);
			showTextboxBridge(kBridgeTalkerKirk, 11);
			showTextboxBridge(kBridgeTalkerSpock, 12);
			showTextboxBridge(kBridgeTalkerKirk, 13);
			break;
		case 4: {
			const int options[] = { 14, 15, 16, -1 };

			_sound->playSoundEffectIndex(kSfxHailing);
			showTextboxBridge(kBridgeTalkerUhura, 17);
			showBridgeScreenTalker("klg1");
			showTextboxBridge(kBridgeTalkerKallarax, 18);
			int choice = showKirkChoices(options);
			if (choice != 0 && choice != 1) {
				// Kallarax doesn't like jokes
				setMissionState(kMissionTrialBattle);
				return true;
			}
			showTextboxBridge(kBridgeTalkerKallarax, 19);
			removeBridgeScreenTalker();
			showTextboxBridge(kBridgeTalkerUhura, 20);
			showTextboxBridge(kBridgeTalkerKirk, 21);
			break;
		}
		default:
			break;
		}
		endMissionTrial();
		return true;

	case kMissionTrialBattle:
		if (type != kMissionEventTick)
			return false;
		if (value == 1) {
			showTextboxBridge(kBridgeTalkerKallarax, 22);
			removeBridgeScreenTalker();
			_sound->playMidiMusicTracks(MIDITRACK_2, kLoopTypeRepeat);
			_sound->playSoundEffectIndex(kSfxRedAlert);
			showTextboxBridge(kBridgeTalkerSpock, 23);
			spawnEnemyShip("K'LRTA");
			_enterpriseState.underAttack = true;
			_bridgeInputLocked = false;
		} else if (value > 2 && !_enterpriseState.underAttack) {
			_bridgeInputLocked = true;
			stopMissionMusic();
			showTextboxBridge(kBridgeTalkerKirk, 24);
			showTextboxBridge(kBridgeTalkerSpock, 25);
			endMissionTrial();
		}
		return true;

	default:
		return false;
	}
}

void StarTrekEngine::endMissionTrial() {
	startBridgeWarp();
	endBridgeWarp();
	// Both parts of the mission are rated together
	showMissionPerformance(4, 26, _missionVars[1] + _missionScore, _missionVars[2] + _missionMaxScore);
	setMissionState(kMissionSinsStart);
	_bridgeInputLocked = false;
}

// Chapter 6: That Old Devil Moon

bool StarTrekEngine::handleSinsEvent(int type, int value) {
	switch (_missionState) {
	case kMissionSinsStart:
		if (type == kMissionEventTick) {
			if (value == 1) {
				startMission("SINS", "bridge");
				playMissionFlyby("SIN0\\FLYBY", "devilmon");
			} else if (value == 151) {
				_sound->playSoundEffectIndex(kSfxHailing);
				showTextboxBridge(kBridgeTalkerUhura, 0);
				showTextboxBridge(kBridgeTalkerKirk, 1);
				showBridgeScreenTalkerWithMessage(2, "Admiral", "woman");
				showTextboxBridge(kBridgeTalkerSpock, 3);
				showTextboxBridge(kBridgeTalkerKirk, 4);
				showTextboxBridge(kBridgeTalkerSpock, 5);
				showTextboxBridge(kBridgeTalkerChekov, 6);
				showTextboxBridge(kBridgeTalkerSpock, 7);
				showTextboxBridge(kBridgeTalkerKirk, 8);
				showTextboxBridge(kBridgeTalkerSpock, 9);
				_targetPlanet = kPlanetAlphaProxima;
			}
			return true;
		}
		if (type != kMissionEventMenu)
			return false;

		switch (value) {
		case kBridgeUhura:
			showTextboxBridge(kBridgeTalkerUhura, 10);
			return true;
		case kBridgeSpock:
			showTextboxBridge(kBridgeTalkerSpock, 11);
			return true;
		case kBridgeChekovNavigation:
			dispatchBridgeMenu(value);
			if (_currentPlanet == _targetPlanet)
				setMissionState(kMissionSinsAlphaProxima);
			return true;
		default:
			return false;
		}

	case kMissionSinsAlphaProxima:
		// Variable 0 is set when the asteroid was scanned
		if (type == kMissionEventReturn) {
			_bridgeInputLocked = true;
			updateStarfieldAndShips(false);
			setCrewStanding(true);
			_sound->playSoundEffectIndex(kSfxHailing);
			showTextboxBridge(kBridgeTalkerUhura, 22);
			showTextboxBridge(kBridgeTalkerKirk, 23);
			showMissionPerformance(5, 24, _missionScore, _missionMaxScore);
			showTextboxBridge(kBridgeTalkerMcCoy, 25);
			showTextboxBridge(kBridgeTalkerSpock, 26);
			showTextboxBridge(kBridgeTalkerMcCoy, 27);
			showTextboxBridge(kBridgeTalkerKirk, 28);
			// The original shows this at McCoy's position
			showTextbox("Captain Kirk", _resource->getLoadedText(29), 160, 100, 44, 0);
			showTextboxBridge(kBridgeTalkerMcCoy, 30);
			showTextboxBridge(kBridgeTalkerKirk, 31);
			playOrbitSequence(false, false);
			setCrewStanding(false);
			setMissionState(kMissionVengStart);
			_bridgeInputLocked = false;
			return true;
		}
		if (type != kMissionEventMenu)
			return false;

		switch (value) {
		case kBridgeUhura:
			showTextboxBridge(kBridgeTalkerUhura, _missionVars[0] ? 12 : 13);
			return true;
		case kBridgeSpock:
			if (!_enterpriseState.inOrbit) {
				showTextboxBridge(kBridgeTalkerSpock, 14);
			} else if (!_missionVars[0]) {
				_missionVars[0] = 1;
				showTextboxBridge(kBridgeTalkerSpock, 15);
				showTextboxBridge(kBridgeTalkerSulu, 16);
				showTextboxBridge(kBridgeTalkerKirk, 17);
				showTextboxBridge(kBridgeTalkerUhura, 18);
				showTextboxBridge(kBridgeTalkerKirk, 19);
				showTextboxBridge(kBridgeTalkerSpock, 20);
				showTextboxBridge(kBridgeTalkerSpock, 21);
			} else {
				showTextboxBridge(kBridgeTalkerSpock, 21);
			}
			return true;
		default:
			return false;
		}

	default:
		return false;
	}
}

// Chapter 7: Vengeance

bool StarTrekEngine::handleVengEvent(int type, int value) {
	// Variables 0 and 1 are the score and the maximum score of the away mission
	switch (_missionState) {
	case kMissionVengStart:
		if (type == kMissionEventReturn) {
			returnFromRepublic();
			return true;
		}
		if (type == kMissionEventMenu)
			return value != kBridgeKirkTransporter;

		if (value == 1) {
			startMission("VENG", "bridgew");
			_targetPlanet = _currentPlanet;
			playMissionFlyby("VEN0\\FLYBY", "vengence");	// a typo in the original
		} else if (value == 151) {
			const int options[] = { 0, 1, -1 };

			showTextboxBridge(kBridgeTalkerKirk, 10);
			showTextboxBridge(kBridgeTalkerSpock, 11);
			orbitPlanetSequence(7);
			showTextboxBridge(kBridgeTalkerKirk, 12);
			showTextboxBridge(kBridgeTalkerUhura, 13);
			// The answer has no effect
			showKirkChoices(options);
			dispatchBridgeMenu(kBridgeKirkTransporter);
		}
		return true;

	case kMissionVengBattle:
		if (type == kMissionEventTick && value == 300 && _enterpriseState.underAttack) {
			_sound->playSoundEffectIndex(kSfxRedAlert);
			showTextboxBridge(kBridgeTalkerSpock, 34);
			spawnEnemyShip("3PIRATE");
			spawnEnemyShip("orion");
		}
		if (type == kMissionEventMenu) {
			if (value == kBridgeSpock) {
				showTextboxBridge(kBridgeTalkerSpock, 35);
				return true;
			}
			if (value == kBridgeUhura) {
				showTextboxBridge(kBridgeTalkerUhura, 36);
				return true;
			}
			return false;
		}
		if (type == kMissionEventTick && !_enterpriseState.underAttack)
			endGame();
		return true;

	default:
		return false;
	}
}

void StarTrekEngine::returnFromRepublic() {
	const int hailOptions[] = { 2, 3, -1 };
	const int bredellOptions[] = { 4, 5, 6, -1 };
	const int constitutionOptions[] = { 7, 8, 9, -1 };

	_missionVars[0] = _missionScore;
	_missionVars[1] = _missionMaxScore;
	_enterpriseState.inOrbit = false;
	removeOrbitObject();
	startBridgeWarp();
	initSystemPlanet(kPlanetNone);
	endBridgeWarp();
	loadActorAnim(2, ACTOR_STANDING_MCCOY, 0, 0, 1.0);
	_enterpriseR3.commandedSpeed = _enterpriseR3.maxSpeed;

	showTextboxBridge(kBridgeTalkerKirk, 14);
	showTextboxBridge(kBridgeTalkerSpock, 15);
	showTextboxBridge(kBridgeTalkerKirk, 16);
	showTextboxBridge(kBridgeTalkerSpock, 17);
	showTextboxBridge(kBridgeTalkerSpock, 18);
	showTextboxBridge(kBridgeTalkerUhura, 19);
	showTextboxBridge(kBridgeTalkerSpock, 20);
	if (showKirkChoices(hailOptions) != 0) {
		showTextboxBridge(kBridgeTalkerSulu, 32);
		_sound->playSoundEffectIndex(kSfxHailing);
		showTextboxBridge(kBridgeTalkerUhura, 33);
	}

	showBridgeScreenTalker("maddoc");
	showTextboxBridge(kBridgeTalkerBredell, 21);
	showTextboxBridge(kBridgeTalkerKirk, 22);
	showTextboxBridge(kBridgeTalkerBredell, 23);
	switch (showKirkChoices(bredellOptions)) {
	case 0:
		showTextboxBridge(kBridgeTalkerBredell, 24);
		showTextboxBridge(kBridgeTalkerKirk, 25);
		showTextboxBridge(kBridgeTalkerBredell, 26);
		showTextboxBridge(kBridgeTalkerKirk, 27);
		showTextboxBridge(kBridgeTalkerBredell, 28);
		showKirkChoices(constitutionOptions);
		showTextboxBridge(kBridgeTalkerBredell, 29);
		break;
	case 1:
		showTextboxBridge(kBridgeTalkerBredell, 30);
		break;
	case 2:
		showTextboxBridge(kBridgeTalkerBredell, 31);
		break;
	default:
		break;
	}

	spawnEnemyShip("vardain");
	_enterpriseState.underAttack = true;
	removeBridgeScreenTalker();
	setMissionState(kMissionVengBattle);
	_sound->playMidiMusicTracks(MIDITRACK_2, kLoopTypeRepeat);
	_sound->playSoundEffectIndex(kSfxRedAlert);
}

void StarTrekEngine::endGame() {
	// Speaker and text of the epilogues, depending on the yearly review
	static const int16 kEpilogues[3][7][2] = {
		{
			{ kBridgeTalkerMcCoy, 47 }, { kBridgeTalkerSpock, 48 }, { kBridgeTalkerKirk, 49 }, { kBridgeTalkerMcCoy, 50 },
			{ kBridgeTalkerSpock, 51 }, { kBridgeTalkerKirk, 52 }, { kBridgeTalkerNone, 0 }
		},
		{
			{ kBridgeTalkerKirk, 54 }, { kBridgeTalkerSpock, 55 }, { kBridgeTalkerMcCoy, 56 }, { kBridgeTalkerSpock, 57 },
			{ kBridgeTalkerKirk, 58 }, { kBridgeTalkerMcCoy, 59 }, { kBridgeTalkerKirk, 60 }
		},
		{
			{ kBridgeTalkerMcCoy, 62 }, { kBridgeTalkerSpock, 63 }, { kBridgeTalkerMcCoy, 64 }, { kBridgeTalkerKirk, 65 },
			{ kBridgeTalkerSpock, 66 }, { kBridgeTalkerMcCoy, 67 }, { kBridgeTalkerKirk, 68 }
		}
	};
	static const int kAdmiralTexts[] = { 46, 53, 61 };

	leaveFullscreenView();
	setMouseControllingShip(true);
	_bridgeInputLocked = true;
	showTextboxBridge(kBridgeTalkerSpock, 37);
	showTextboxBridge(kBridgeTalkerKirk, 38);
	showTextboxBridge(kBridgeTalkerScotty, 39);
	showTextboxBridge(kBridgeTalkerChekov, 40);
	showTextboxBridge(kBridgeTalkerKirk, 41);
	showTextboxBridge(kBridgeTalkerUhura, 42);
	showTextboxBridge(kBridgeTalkerKirk, 43);
	showMissionPerformance(6, 44, _missionVars[0], _missionVars[1]);

	// The yearly review rates all missions
	int points = 0;
	for (int i = 0; i < ARRAYSIZE(_missionPoints); i++)
		points += _missionPoints[i];
	int percent = CLIP(points * 100 / 28, 0, 100);
	Common::String review = Common::String::format(_resource->getLoadedText(45).c_str(), getSpeechSampleForNumber(percent).c_str(), percent);
	showTextbox("Admiral", review, 160, 190, 44, -7);

	int epilogue = percent <= 50 ? 0 : (percent <= 85 ? 1 : 2);
	showTextbox("Admiral", _resource->getLoadedText(kAdmiralTexts[epilogue]), 160, 190, 44, -7);
	removeBridgeScreenTalker();
	for (int i = 0; i < 7 && kEpilogues[epilogue][i][0] != kBridgeTalkerNone; i++)
		showTextboxBridge(kEpilogues[epilogue][i][0], kEpilogues[epilogue][i][1]);

	enterFullscreenView();
	_mouseControllingShip = false;
	playWarpSequence();
	showEndCredits();

	_gfx->fillBackgroundRect(Common::Rect(SCREEN_WIDTH, SCREEN_HEIGHT), 0);
	_gfx->clearScreenAndPriBuffer();
	_bridgeInputLocked = false;
	setMissionState(kMissionNone);
	showGameOverMenu();
}

void StarTrekEngine::showEndCredits() {
	_sound->clearAllMidiSlots();
	_sound->loadMusicFile("title");
	_gfx->loadPalette("bridge");

	// The camera looks back while the credits fly away from it
	_starfieldPosition = Point3(0, 0, 0);
	_someMatrix = initMatrix();
	rotateMatrix(_someMatrix, 1, angleSin(0x200), angleCos(0x200));

	R3 credits = R3();
	credits.field1e = 3;
	credits.field22 = 1;
	credits.field24 = 1;
	credits.bitmap = new Bitmap(_resource->loadFile("credit00.shp"));
	addR3(&credits);
	initIntroR3ObjectToMove(&credits, 0, 0, 0, -_starfieldPointDivisor, 10);

	bool aborted = playEndCreditsFrames(&credits, 10);
	if (!aborted) {
		credits.pos.z = _starfieldPosition.z - _starfieldPointDivisor;
		credits.speed.z = 0;
		aborted = playEndCreditsFrames(&credits, 50);
	}
	delR3(&credits);
	if (!aborted)
		aborted = playEndCreditsFrames(&credits, 15);

	// A eulogy for Gene Roddenberry
	if (!aborted) {
		credits = R3();
		credits.field1e = 3;
		credits.field22 = 1;
		credits.field24 = 1;
		credits.bitmap = new Bitmap(_resource->loadFile("credgene.shp"));
		credits.matrix = initMatrix();
		credits.pos = _starfieldPosition;
		credits.pos.z -= _starfieldPointDivisor;
		addR3(&credits);
		if (_sfxEnabled && _sfxWorking)
			_sound->playSpeech("sfx\\geneeulo");
		do {
			aborted = playEndCreditsFrames(&credits, 220);
		} while (!aborted && _sound->isPlayingSpeech());
		_sound->stopPlayingSpeech();
		delR3(&credits);
		if (!aborted)
			playEndCreditsFrames(&credits, 15);
	}

	_sound->loadMusicFile("bridge");
}

// Returns true when a key aborts the credits
bool StarTrekEngine::playEndCreditsFrames(R3 *r3, int frames) {
	for (int frame = 0; frame < frames; frame++) {
		_gfx->incPaletteFadeLevel();
		updateStarfieldAndShips(false);
		_gfx->drawAllSprites();
		r3->pos.z += r3->speed.z - 10;
		_starfieldPosition.z -= 10;

		for (int i = 0; i < 3; i++) {
			waitForNextTick(true);
			TrekEvent event;
			while (popNextEvent(&event, false)) {
				if (event.type == TREKEVENT_KEYDOWN)
					return true;
			}
		}
		if (shouldQuit())
			return true;
	}
	return false;
}

} // End of namespace StarTrek
