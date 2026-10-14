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

#ifndef STARTREK_BRIDGE_H
#define STARTREK_BRIDGE_H

namespace StarTrek {

enum BridgeMenuEvent {
	kBridgeNone = -1,
	kBridgeKirkCommand = 0,
	kBridgeFirePhasers = 1,
	kBridgeFireTorpedoes = 2,
	kBridgeStarfieldFullScreen = 3,
	kBridgeKirkCaptainsLog = 16,
	kBridgeKirkTransporter = 17,
	kBridgeKirkOptions = 18,
	kBridgeSpock = 32,
	kBridgeSpockComputer = 33,
	kBridgeScottyDamageControl = 48,
	kBridgeScottyEmergencyPower = 49,
	kBridgeUhura = 64,
	kBridgeSuluOrbit = 80,
	kBridgeSuluShields = 81,
	kBridgeChekovNavigation = 96,
	kBridgeChekovWeapons = 97,
	kBridgeChekovRepairShields = 112,
	kBridgeChekovRepairPhasers = 113,
	kBridgeChekovRepairPhotonTorpedoes = 114,
	kBridgeChekovRepairSensors = 115,
	kBridgeChekovRepairBridge = 116,
	kBridgeChekovRepairHull = 117,
	kBridgeChekovRepairWarpDrives = 118,
	kBridgeSuluTargetAnalysis = 119
};

enum Planet {
	kPlanetNone = -1,
	kPlanetCenturius = 0,
	kPlanetCameronsStar = 1,
	kPlanetArk7 = 2,      // Chapter 3: Love's Labor Jeopardized (love)
	kPlanetHarlequin = 3, // Chapter 4: Another Fine Mess (mudd)
	kPlanetHarrapa = 4,
	kPlanetElasiPrime = 5,
	kPlanetDigifal = 6, // Chapter 5A: The Feathered Serpent (feather)
	kPlanetStrahkeer = 7,
	kPlanetHrakkour = 8, // Chapter 5B: The Feathered Serpent (trial)
	kPlanetTriRhoNautica = 9,
	kPlanetShivaOmicron = 10,
	kPlanetAlphaProxima = 11, // Chapter 6: The Old Devil Moon (sins)
	kPlanetOmegaMaelstrom = 12,
	kPlanetArgosIV = 13,
	kPlanetBetaMyamid = 14, // Chapter 2: Hijacked (tug)
	kPlanetSirius = 15,
	kPlanetSigmaZhukova = 16,
	kPlanetCastor = 17,
	kPlanetPollux = 18, // Chapter 1: Demon world (demon)
	kPlanetChristgen = 19
};

enum BridgeTalkers {
	kBridgeTalkerNone = -1,
	kBridgeTalkerKirk = 0,
	kBridgeTalkerSpock,
	kBridgeTalkerSulu,
	kBridgeTalkerChekov,
	kBridgeTalkerUhura,
	kBridgeTalkerScotty,
	kBridgeTalkerMcCoy,
	kBridgeTalkerCaptainsLog,
	kBridgeTalkerElasiCaptain,
	kBridgeTalkerElasiCereth,
	kBridgeTalkerAutobeacon,
	kBridgeTalkerHarryMuddsVoice,
	kBridgeTalkerHarryMudd,
	kBridgeTalkerTaraz,
	kBridgeTalkerAdmiralKenka,
	kBridgeTalkerKallarax,
	kBridgeTalkerBredell
};

enum RandomEncounterType {
	kRandomEncounterNone = 0,
	kRandomEncounterKlingon = 1,
	kRandomEncounterRomulan = 2,
	kRandomEncounterElasi = 3
};

enum MissionEventType {
	kMissionEventTick,
	kMissionEventMenu,
	kMissionEventReturn	// Back from the away mission
};

enum MissionState {
	kMissionNone = -1,
	// -- Chapter 1 ----
	kMissionDemonStart = 0,
	kMissionDemonMockBattle,
	kMissionDemonAfterBattle,
	kMissionDemonPollux,
	kMissionDemonEnd,
	// -- Chapter 2 ----
	kMissionTugStart,
	kMissionTugPirate,
	kMissionTugPirateFled,
	kMissionTugMasada,
	kMissionTugEnd,
	// -- Chapter 3 ----
	kMissionLoveStart,
	kMissionLoveEnRoute,
	kMissionLoveAmbush,
	kMissionLoveAfterBattle,
	kMissionLoveArk7,
	kMissionLoveEnd,
	// -- Chapter 4 ----
	kMissionMuddStart,
	kMissionMuddHarlequin,
	kMissionMuddDerelict,
	// -- Chapter 5 ----
	kMissionFeatherStart,
	kMissionFeatherOrbit,
	kMissionFeatherBattle,
	kMissionTrialStart,
	kMissionTrialEnd,
	kMissionTrialBattle,
	// -- Chapter 6 ----
	kMissionSinsStart,
	kMissionSinsAlphaProxima,
	// -- Chapter 7 ----
	kMissionVengStart,
	kMissionVengBattle
};

} // End of namespace StarTrek

#endif
