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

#include "startrek/room.h"

#define OBJECT_MUDD 8
#define OBJECT_9 9
#define OBJECT_ALIEN_IMAGE 10
#define OBJECT_ALIENDV 11
#define OBJECT_NORTH_DOOR 12
#define OBJECT_REPAIR_TOOL 13

#define HOTSPOT_SCREEN 0x20
#define HOTSPOT_SPHERE 0x21
#define HOTSPOT_NORTH_DOOR 0x22
#define HOTSPOT_EAST_DOOR 0x23

namespace StarTrek {

extern const RoomAction mudd3ActionList[] = {
	{ {ACTION_TICK, 1, 0, 0, 0}, &Room::mudd3Tick1 },
	{ {ACTION_USE, OBJECT_ICOMM, 0xff, 0, 0},    &Room::mudd3UseCommunicator },
	{ {ACTION_LOOK, 0x20, 0, 0, 0},              &Room::mudd3LookAtScreen },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0x20, 0, 0}, &Room::mudd3UseSTricorderOnScreen },
	{ {ACTION_USE, OBJECT_KIRK, 0x20, 0, 0},     &Room::mudd3UseKirkOnScreen },
	{ {ACTION_USE, OBJECT_SPOCK, 0x21, 0, 0},    &Room::mudd3UseSpockOnSphere },
	{ {ACTION_TIMER_EXPIRED, 3, 0, 0, 0},        &Room::mudd3Timer3Expired },
	{ {ACTION_USE, OBJECT_MCCOY, 0x21, 0, 0},    &Room::mudd3UseMccoyOnSphere },
	{ {ACTION_USE, OBJECT_REDSHIRT, 0x21, 0, 0}, &Room::mudd3UseRedshirtOnSphere },
	{ {ACTION_USE, OBJECT_IMTRICOR, 0x21, 0, 0}, &Room::mudd3UseMTricorderOnSphere },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0x21, 0, 0}, &Room::mudd3UseSTricorderOnSphere },
	{ {ACTION_FINISHED_WALKING, 2, 0, 0, 0},     &Room::mudd3ReadyToHookUpTricorders },
	{ {ACTION_TIMER_EXPIRED, 1, 0, 0, 0},        &Room::mudd3Timer1Expired },
	{ {ACTION_TIMER_EXPIRED, 2, 0, 0, 0},        &Room::mudd3Timer2Expired },
	{ {ACTION_TIMER_EXPIRED, 4, 0, 0, 0},        &Room::mudd3Timer4Expired },
	{ {ACTION_USE, OBJECT_IDISKS, 0x21, 0, 0},   &Room::mudd3UseMemoryDiskOnSphere },

	// Common code (next 4 lines)
	{ {ACTION_USE, OBJECT_IDEGRIME, 0xff, 0, 0}, &Room::muddaUseDegrimer },
	{ {ACTION_USE, OBJECT_ILENSES, OBJECT_IDEGRIME, 0, 0}, &Room::muddaUseLenseOnDegrimer },
	{ {ACTION_USE, OBJECT_IALIENDV, 0xff, 0, 0}, &Room::muddaUseAlienDevice },
	{ {ACTION_FINISHED_ANIMATION, 9, 0, 0, 0}, &Room::muddaFiredAlienDevice },

	{ {ACTION_GET, 13, 0, 0, 0}, &Room::mudd3GetRepairTool },
	{ {ACTION_FINISHED_WALKING, 3, 0, 0, 0},   &Room::mudd3ReachedRepairTool },
	{ {ACTION_FINISHED_ANIMATION, 1, 0, 0, 0}, &Room::mudd3PickedUpRepairTool },
	{ {ACTION_LOOK, 0x21, 0, 0, 0},            &Room::mudd3LookAtSphere },
	{ {ACTION_WALK, 0x22, 0, 0, 0},            &Room::mudd3WalkToNorthDoor },
	{ {ACTION_TOUCHED_HOTSPOT, 0, 0, 0, 0},    &Room::mudd3TouchedHotspot0 },
	{ {ACTION_WALK, 0x23, 0, 0, 0},            &Room::mudd3WalkToEastDoor },
	{ {ACTION_TOUCHED_HOTSPOT, 1, 0, 0, 0},    &Room::mudd3TouchedHotspot1 },
	{ {ACTION_LOOK, OBJECT_KIRK,     0, 0, 0}, &Room::mudd3LookAtKirk },
	{ {ACTION_LOOK, OBJECT_SPOCK,    0, 0, 0}, &Room::mudd3LookAtSpock },
	{ {ACTION_LOOK, OBJECT_MCCOY,    0, 0, 0}, &Room::mudd3LookAtMccoy },
	{ {ACTION_LOOK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::mudd3LookAtRedshirt },
	{ {ACTION_LOOK, 8,               0, 0, 0}, &Room::mudd3LookAtMudd },
	{ {ACTION_TALK, OBJECT_KIRK,     0, 0, 0}, &Room::mudd3TalkToKirk },
	{ {ACTION_TALK, OBJECT_SPOCK,    0, 0, 0}, &Room::mudd3TalkToSpock },
	{ {ACTION_TALK, OBJECT_MCCOY,    0, 0, 0}, &Room::mudd3TalkToMccoy },
	{ {ACTION_TALK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::mudd3TalkToRedshirt },
	{ {ACTION_TALK, 8,               0, 0, 0}, &Room::mudd3TalkToMudd },
	{ {ACTION_USE, OBJECT_IMEDKIT,  0xff, 0, 0}, &Room::mudd3UseMedkit },

	// Common code (countdown for losing atmosphere when life support malfunctioning)
	{ {ACTION_TICK, 0xff, 0xff, 0xff, 0},           &Room::muddaTick },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum mudd3TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_UHURA,
	TX_SPEAKER_BUCHERT, TX_SPEAKER_MUDD,
	TX_MUD3_002, TX_MUD3_003, TX_MUD3_005, TX_MUD3_006, TX_MUD3_007,
	TX_MUD3_008, TX_MUD3_009, TX_MUD3_010, TX_MUD3_011, TX_MUD3_012,
	TX_MUD3_013, TX_MUD3_014, TX_MUD3_015, TX_MUD3_016, TX_MUD3_017,
	TX_MUD3_018, TX_MUD3_019, TX_MUD3_020, TX_MUD3_021, TX_MUD3_022,
	TX_MUD3_023, TX_MUD3_024, TX_MUD3_025, TX_MUD3_026, TX_MUD3_027,
	TX_MUD3_028, TX_MUD3_029, TX_MUD3_030, TX_MUD3_031, TX_MUD3_032,
	TX_MUD3_033, TX_MUD3_034, TX_MUD3_035, TX_MUD3_037, TX_MUD3_038,
	TX_MUD3_039, TX_MUD3_040, TX_MUD3_041, TX_MUD3_042, TX_MUD3_043,
	TX_MUD3_044, TX_MUD3_046, TX_MUD3_047, TX_MUD3_048, TX_MUD3_049,
	TX_MUD3_050, TX_MUD3_051, TX_MUD3_052, TX_MUD3_053, TX_MUD3_054,
	TX_MUD3_055, TX_MUD3_056, TX_MUD3_057, TX_MUD3_058, TX_MUD3_059,
	TX_MUD3_060, TX_MUD3_061, TX_MUD3_062, TX_MUD3_063, TX_MUD3_064,
	TX_MUD3_065, TX_MUD3_066, TX_MUD3_067, TX_MUD3_068, TX_MUD3_069,
	TX_MUD3_070, TX_MUD3_071, TX_MUD3N000, TX_MUD3N002, TX_MUD3N003,
	TX_MUD3N004, TX_MUD3N005, TX_MUD3N006, TX_MUD3N007, TX_MUD3N008,
	TX_MUD3N009, TX_MUD3N010, TX_MUD3N011, TX_MUD3N012, TX_MUD3N013,
	TX_MUD3N014, TX_MUD3N015, TX_MUD3N017, TX_MUD3N018, TX_MUD3N019,
	TX_MUD3N020, TX_DEM0N009, TX_STATICU1
};

extern const RoomTextOffsets mudd3TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 3778, 3632, 3821, 3796, 3648, 3644, 3722, 3648, 3828 },
	{ TX_SPEAKER_MCCOY, 3789, 3643, 3832, 3807, 3659, 3658, 3734, 3659, 3839 },
	{ TX_SPEAKER_SPOCK, 3799, 3653, 3842, 3817, 3669, 3668, 3744, 3669, 3849 },
	{ TX_SPEAKER_UHURA, 524, 503, 523, 544, 517, 517, 515, 517, 549 },
	{ TX_SPEAKER_BUCHERT, 3809, 3663, 3852, 3827, 3679, 3678, 3754, 3679, 3859 },
	{ TX_SPEAKER_MUDD, 3821, 3675, 3864, 3839, 3691, 3690, 3766, 3691, 3871 },
	{ TX_MUD3_002, 12100, 10921, 13132, 12251, 10902, 11127, 11976, 10902, 12151 },
	{ TX_MUD3_003, 487, 461, 487, 487, 475, 475, 475, 475, 512 },
	{ TX_MUD3_005, 13314, 11955, 14527, 13553, 11936, 12275, 13195, 11936, 13365 },
	{ TX_MUD3_006, 9361, 8647, 10055, 9448, 8629, 8811, 9368, 8629, 9412 },
	{ TX_MUD3_007, 7734, 7206, 8301, 7825, 7231, 7397, 7826, 7231, 7785 },
	{ TX_MUD3_008, 9718, 8974, 10493, 9845, 8955, 9185, 9775, 8955, 9769 },
	{ TX_MUD3_009, 8704, 8096, 9351, 8831, 8079, 8301, 8754, 8079, 8755 },
	{ TX_MUD3_010, 8569, 7991, 9199, 8696, 7974, 8195, 8632, 7974, 8620 },
	{ TX_MUD3_011, 11631, 10527, 12629, 11807, 10508, 10776, 11548, 10508, 11682 },
	{ TX_MUD3_012, 9147, 8463, 9833, 9241, 8445, 8623, 9176, 8445, 9198 },
	{ TX_MUD3_013, 12946, 11647, 14130, 13170, 11628, 11950, 12858, 11628, 12997 },
	{ TX_MUD3_014, 11167, 10138, 12097, 11314, 10118, 10359, 11091, 10118, 11218 },
	{ TX_MUD3_015, 5526, 5168, 5766, 5571, 5186, 5248, 5458, 5186, 5576 },
	{ TX_MUD3_016, 11915, 10766, 12927, 12060, 10747, 10981, 11801, 10747, 11966 },
	{ TX_MUD3_017, 4507, 4269, 4688, 4537, 4287, 4301, 4500, 4287, 4557 },
	{ TX_MUD3_018, 6344, 5911, 6704, 6402, 5929, 6023, 6320, 5929, 6395 },
	{ TX_MUD3_019, 12146, 10952, 13188, 12297, 10933, 11161, 12017, 10933, 12197 },
	{ TX_MUD3_020, 3473, 3342, 3510, 3495, 3358, 3358, 3426, 3358, 3523 },
	{ TX_MUD3_021, 6291, 5873, 6640, 6349, 5891, 5985, 6271, 5891, 6342 },
	{ TX_MUD3_022, 5312, 4984, 5540, 5355, 5003, 5055, 5262, 5003, 5362 },
	{ TX_MUD3_023, 11763, 10644, 12751, 11904, 10625, 10857, 11655, 10625, 11814 },
	{ TX_MUD3_024, 9282, 8583, 9972, 9369, 8565, 8736, 9300, 8565, 9333 },
	{ TX_MUD3_025, 11373, 10314, 12338, 11542, 10295, 10555, 11302, 10295, 11424 },
	{ TX_MUD3_026, 8287, 7699, 8899, 8357, 7723, 7887, 8363, 7723, 8338 },
	{ TX_MUD3_027, 7770, 7227, 8338, 7860, 7252, 7417, 7848, 7252, 7821 },
	{ TX_MUD3_028, 8134, 7561, 8740, 8203, 7585, 7748, 8219, 7585, 8185 },
	{ TX_MUD3_029, 12593, 11324, 13691, 12784, 11305, 11590, 12448, 11305, 12644 },
	{ TX_MUD3_030, 12257, 11048, 13310, 12416, 11029, 11264, 12124, 11029, 12308 },
	{ TX_MUD3_031, 8858, 8225, 9510, 8988, 8208, 8414, 8899, 8208, 8909 },
	{ TX_MUD3_032, 7050, 6591, 7521, 7117, 6607, 6741, 7106, 6607, 7101 },
	{ TX_MUD3_033, 11586, 10497, 12588, 11756, 10478, 10740, 11522, 10478, 11637 },
	{ TX_MUD3_034, 4190, 3998, 4317, 4229, 4015, 4043, 4174, 4015, 4240 },
	{ TX_MUD3_035, 3833, 3687, 3876, 3851, 3703, 3702, 3778, 3703, 3883 },
	{ TX_MUD3_037, 4647, 4379, 4830, 4666, 4397, 4400, 4612, 4397, 4697 },
	{ TX_MUD3_038, 4388, 4165, 4532, 4421, 4183, 4201, 4359, 4183, 4438 },
	{ TX_MUD3_039, 3907, 3746, 3961, 3930, 3762, 3766, 3848, 3762, 3957 },
	{ TX_MUD3_040, 4049, 3872, 4121, 4078, 3889, 3899, 3993, 3889, 4099 },
	{ TX_MUD3_041, 6156, 5753, 6468, 6215, 5771, 5867, 6114, 5771, 6207 },
	{ TX_MUD3_042, 5896, 5508, 6179, 5970, 5527, 5629, 5841, 5527, 5947 },
	{ TX_MUD3_043, 5086, 4773, 5283, 5132, 4792, 4837, 5020, 4792, 5136 },
	{ TX_MUD3_044, 4851, 4553, 5036, 4882, 4571, 4586, 4788, 4571, 4901 },
	{ TX_MUD3_046, 7649, 7136, 8197, 7721, 7161, 7304, 7737, 7161, 7700 },
	{ TX_MUD3_047, 5629, 5256, 5891, 5676, 5275, 5340, 5568, 5275, 5680 },
	{ TX_MUD3_048, 9448, 8719, 10148, 9535, 8701, 8883, 9446, 8701, 9499 },
	{ TX_MUD3_049, 7393, 6896, 7942, 7460, 6920, 7054, 7497, 6920, 7444 },
	{ TX_MUD3_050, 7152, 6678, 7638, 7212, 6694, 6821, 7208, 6694, 7203 },
	{ TX_MUD3_051, 9784, 9025, 10561, 9909, 9005, 9233, 9827, 9005, 9835 },
	{ TX_MUD3_052, 9015, 8346, 9679, 9136, 8328, 8533, 9037, 8328, 9066 },
	{ TX_MUD3_053, 7890, 7332, 8470, 7963, 7356, 7504, 7964, 7356, 7941 },
	{ TX_MUD3_054, 6745, 6301, 7151, 6795, 6318, 6420, 6752, 6318, 6796 },
	{ TX_MUD3_055, 11459, 10385, 12438, 11628, 10366, 10628, 11387, 10366, 11510 },
	{ TX_MUD3_056, 5448, 5105, 5683, 5494, 5123, 5186, 5390, 5123, 5498 },
	{ TX_MUD3_057, 11261, 10217, 12190, 11408, 10197, 10437, 11169, 10197, 11312 },
	{ TX_MUD3_058, 13030, 11716, 14232, 13265, 11697, 12032, 12945, 11697, 13081 },
	{ TX_MUD3_059, 13225, 11881, 14438, 13459, 11862, 12194, 13121, 11862, 13276 },
	{ TX_MUD3_060, 12512, 11258, 13591, 12689, 11239, 11492, 12363, 11239, 12563 },
	{ TX_MUD3_061, 12832, 11548, 13982, 13045, 11529, 11840, 12724, 11529, 12883 },
	{ TX_MUD3_062, 12375, 11151, 13456, 12537, 11132, 11371, 12255, 11132, 12426 },
	{ TX_MUD3_063, 8836, 8218, 9488, 8965, 8201, 8405, 8892, 8201, 8887 },
	{ TX_MUD3_064, 8794, 8191, 9436, 8925, 8174, 8379, 8855, 8174, 8845 },
	{ TX_MUD3_065, 8447, 7884, 9058, 8559, 7868, 8076, 8507, 7868, 8498 },
	{ TX_MUD3_066, 8633, 8040, 9268, 8761, 8023, 8245, 8686, 8023, 8684 },
	{ TX_MUD3_067, 12427, 11188, 13508, 12598, 11169, 11417, 12295, 11169, 12478 },
	{ TX_MUD3_069, 13141, 11812, 14350, 13381, 11793, 12132, 13048, 11793, 13192 },
	{ TX_MUD3_070, 11823, 10689, 12817, 11969, 10670, 10906, 11706, 10670, 11874 },
	{ TX_MUD3_071, 11966, 10802, 12988, 12116, 10783, 11008, 11847, 10783, 12017 },
	{ TX_MUD3N000, 8982, 8328, 9635, 9101, 8310, 8513, 9008, 8310, 9033 },
	{ TX_MUD3N002, 10865, 9896, 11777, 11026, 9876, 10136, 10831, 9876, 10916 },
	{ TX_MUD3N003, 11075, 10061, 12005, 11228, 10041, 10289, 11014, 10041, 11126 },
	{ TX_MUD3N004, 10800, 9846, 11699, 10971, 9826, 10097, 10768, 9826, 10851 },
	{ TX_MUD3N005, 10481, 9587, 11355, 10650, 9567, 9841, 10485, 9567, 10532 },
	{ TX_MUD3N006, 10993, 9994, 11929, 11146, 9974, 10225, 10953, 9974, 11044 },
	{ TX_MUD3N007, 10692, 9753, 11578, 10857, 9733, 10016, 10662, 9733, 10743 },
	{ TX_MUD3N008, 2490, 2417, 2489, 2507, 2433, 2430, 2464, 2433, 2540 },
	{ TX_MUD3N009, 10937, 9953, 11854, 11085, 9933, 10180, 10893, 9933, 10988 },
	{ TX_MUD3N010, 10640, 9716, 11527, 10799, 9696, 9957, 10627, 9696, 10691 },
	{ TX_MUD3N011, 4615, 4362, 4795, 4634, 4380, 4383, 4592, 4380, 4665 },
	{ TX_MUD3N012, 10570, 9661, 11431, 10729, 9641, 9903, 10546, 9641, 10621 },
	{ TX_MUD3N013, 2698, 2610, 2717, 2722, 2626, 2630, 2677, 2626, 2748 },
	{ TX_MUD3N014, 9939, 9150, 10765, 10092, 9130, 9386, 10001, 9130, 9990 },
	{ TX_MUD3N015, 4749, 4466, 4936, 4786, 4484, 4505, 4703, 4484, 4799 },
	{ TX_MUD3N017, 4331, 4123, 4468, 4368, 4141, 4163, 4310, 4141, 4381 },
	{ TX_MUD3N018, 10144, 9310, 10980, 10297, 9290, 9546, 10171, 9290, 10195 },
	{ TX_MUD3N019, 10095, 9276, 10922, 10245, 9256, 9509, 10128, 9256, 10146 },
	{ TX_MUD3N020, 10005, 9201, 10820, 10162, 9181, 9441, 10041, 9181, 10056 },
	{ TX_STATICU1, 534, 513, 533, 554, 527, 527, 525, 527, 559 },
	{          -1, 0,    0, 0, 0, 0, 0, 0, 0, 0 }
};

extern const RoomText mudd3Texts[] = {
	{ TX_MUD3_068, Common::EN_ANY, "#MUD3\\MUD3_068#Not my area of expertise, I'm afraid." },	// Same as MUD3_021, spoken by Buchert
	{ TX_MUD3_068, Common::DE_DEU, "#MUD3\\MUD3_068#Das ist nicht gerade mein Fachgebiet." },
	{ TX_MUD3_068, Common::FR_FRA, "#MUD3\\MUD3_068#J'ai bien peur que ce ne soit pas ma sp\xe9" "cialit\xe9." },
	{ TX_DEM0N009, Common::EN_ANY, "#DEM0\\DEM0N009#Spock raises an eyebrow" },
	{ TX_DEM0N009, Common::DE_DEU, "#DEM0\\DEM0N009#Spock hebt eine Augenbraue." },
	{ TX_DEM0N009, Common::FR_FRA, "#DEM0\\DEM0N009#Spock hausse un sourcil." },
	{ -1, Common::UNK_LANG, "" }
};

// BUG-ish: trying to scan the sphere while someone is behind it causes it to scan that
// object instead.

void Room::mudd3Tick1() {
	playVoc("MUD3LOOP");

	_awayMission->crewDirectionsAfterWalk[OBJECT_REDSHIRT] = DIR_W;

	if (_awayMission->mudd.databaseDestroyed) {
		loadActorAnim(OBJECT_9, "s4lbpb", 0xa2, 0x9f);

		if (!_awayMission->mudd.tookRepairTool)
			loadActorAnim(OBJECT_REPAIR_TOOL, "s4lbdv", 0xb7, 0xa8);
	}

	_awayMission->mudd.muddInDatabaseRoom = false;

	if (!_awayMission->mudd.muddVisitedDatabaseRoom && _awayMission->mudd.translatedAlienLanguage && !_awayMission->mudd.muddUnavailable) {
		_awayMission->mudd.muddVisitedDatabaseRoom = true;
		loadActorAnim(OBJECT_MUDD, "s4lbhs", 0xa2, 0x9f);
		playMidiMusicTracks(MIDITRACK_3);
		_awayMission->mudd.muddInDatabaseRoom = true;
		_awayMission->timers[2] = 16;
	}
}

void Room::mudd3UseCommunicator() {
	showText(TX_SPEAKER_KIRK,  TX_MUD3_003);
	showText(TX_SPEAKER_UHURA, TX_STATICU1);
}

void Room::mudd3LookAtScreen() {
	// BUGFIX: the condition was flipped in the original; the more "vague" description
	// should be shown before the alien language is understood.
	if (_awayMission->mudd.translatedAlienLanguage)
		showText(TX_SPEAKER_SPOCK, TX_MUD3_038);
	else
		showDescription(TX_MUD3N017);
}

void Room::mudd3UseSTricorderOnScreen() {
	if (_roomVar.mudd.tricordersUnavailable)
		showText(TX_SPEAKER_SPOCK, TX_MUD3_039);
	else
		spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_MUD3_017, false);
}

// BUGFIX: Event was actually "use screen on kirk", which makes no sense.
void Room::mudd3UseKirkOnScreen() {
	if (!_awayMission->mudd.translatedAlienLanguage) {
		showDescription(TX_MUD3N011);
		showText(TX_SPEAKER_SPOCK, TX_MUD3_037);
	}
}

void Room::mudd3UseSpockOnSphere() {
	_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_S;
	loadActorStandAnim(OBJECT_SPOCK);

	if (_awayMission->mudd.computerDataErasedOrDestroyed) {
		if (!_awayMission->mudd.databaseDestroyed) {
			showDescription(TX_MUD3N000);
			showText(TX_SPEAKER_SPOCK, TX_MUD3_052);
			showText(TX_SPEAKER_KIRK,  TX_MUD3_012);
			showText(TX_SPEAKER_MCCOY, TX_MUD3_024);
		}
		showText(TX_SPEAKER_KIRK,  TX_MUD3_006);
		showText(TX_SPEAKER_SPOCK, TX_MUD3_048);
		showText(TX_SPEAKER_KIRK,  TX_MUD3_008);
		showText(TX_SPEAKER_SPOCK, TX_MUD3_051);
	} else if (_awayMission->mudd.translatedAlienLanguage) {
		if (_awayMission->mudd.accessedAlienDatabase)
			showText(TX_SPEAKER_SPOCK, TX_MUD3_035);
		else {
			_awayMission->mudd.accessedAlienDatabase = true;

			showText(TX_SPEAKER_SPOCK, TX_MUD3_054);
			showText(TX_SPEAKER_MCCOY, TX_MUD3_032);
			showText(TX_SPEAKER_SPOCK, TX_MUD3_050);
			showText(TX_SPEAKER_SPOCK, TX_MUD3_049);
			showText(TX_SPEAKER_SPOCK, TX_MUD3_046);
			showText(TX_SPEAKER_KIRK,  TX_MUD3_007);

			_awayMission->mudd.missionScore++;
			playVoc("LSHAIANS");
			loadActorAnim(OBJECT_ALIEN_IMAGE, "s4lbap", 0x32, 0x64);

			_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_W;
			_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_W;
			_awayMission->crewDirectionsAfterWalk[OBJECT_MCCOY] = DIR_W;
			_awayMission->crewDirectionsAfterWalk[OBJECT_REDSHIRT] = DIR_W;
			loadActorStandAnim(OBJECT_KIRK);
			loadActorStandAnim(OBJECT_SPOCK);
			loadActorStandAnim(OBJECT_MCCOY);
			loadActorStandAnim(OBJECT_REDSHIRT);

			_awayMission->disableInput = true;
			_awayMission->timers[3] = 40;
		}
	} else if (_awayMission->mudd.discoveredBase3System) {
		showText(TX_SPEAKER_SPOCK, TX_MUD3_043);
		showText(TX_SPEAKER_MCCOY, TX_MUD3_022);
		showText(TX_SPEAKER_SPOCK, TX_MUD3_056);
		showText(TX_SPEAKER_KIRK,  TX_MUD3_015);
		showText(TX_SPEAKER_SPOCK, TX_MUD3_047);
		_roomVar.mudd.suggestedUsingTricorders = true;
	} else {
		showText(TX_SPEAKER_SPOCK, TX_MUD3_044);
		_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_W;
		walkCrewman(OBJECT_SPOCK, 0xcd, 0x87);
	}
}

// Finished looking at alien image
void Room::mudd3Timer3Expired() {
	_awayMission->disableInput = false;

	showText(TX_SPEAKER_MCCOY, TX_MUD3_027);
	showText(TX_SPEAKER_SPOCK, TX_MUD3_053);
	showText(TX_SPEAKER_MCCOY, TX_MUD3_028);
	showText(TX_SPEAKER_MCCOY, TX_MUD3_026);
}

void Room::mudd3UseMccoyOnSphere() {
	// ENHANCEMENT: Turn to face south
	_awayMission->crewDirectionsAfterWalk[OBJECT_MCCOY] = DIR_S;
	loadActorStandAnim(OBJECT_MCCOY);

	showText(TX_SPEAKER_MCCOY, TX_MUD3_021);
	_awayMission->crewDirectionsAfterWalk[OBJECT_MCCOY] = DIR_W;
	walkCrewman(OBJECT_MCCOY, 0xec, 0x84);
}

void Room::mudd3UseRedshirtOnSphere() {
	// ENHANCEMENT: Turn to face south
	_awayMission->crewDirectionsAfterWalk[OBJECT_REDSHIRT] = DIR_S;
	loadActorStandAnim(OBJECT_REDSHIRT);

	showText(TX_SPEAKER_BUCHERT, TX_MUD3_068); // BUGFIX: original played McCoy's audio by mistake
	_awayMission->crewDirectionsAfterWalk[OBJECT_REDSHIRT] = DIR_W;
	walkCrewman(OBJECT_REDSHIRT, 0xec, 0x84);
}

void Room::mudd3UseMTricorderOnSphere() {
	if (_roomVar.mudd.tricordersUnavailable)
		showText(TX_SPEAKER_SPOCK, TX_MUD3_039); // BUGFIX: speaker is Spock
	else {
		_awayMission->crewDirectionsAfterWalk[OBJECT_MCCOY] = DIR_S;
		loadActorStandAnim(OBJECT_MCCOY);
		if (!_awayMission->mudd.translatedAlienLanguage && _roomVar.mudd.suggestedUsingTricorders && _awayMission->mudd.discoveredBase3System) {
			_awayMission->disableInput = true;

			_awayMission->crewDirectionsAfterWalk[OBJECT_MCCOY] = DIR_S;
			walkCrewman(OBJECT_MCCOY, 0xac, 0x88);
			_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_S;
			walkCrewmanC(OBJECT_SPOCK, 0xa5, 0x8c, &Room::mudd3ReadyToHookUpTricorders);
		} else {
			showText(TX_SPEAKER_MCCOY, TX_MUD3_021);
			_awayMission->crewDirectionsAfterWalk[OBJECT_MCCOY] = DIR_W;
			walkCrewman(OBJECT_MCCOY, 0xec, 0x84);
		}
	}
}

void Room::mudd3UseSTricorderOnSphere() {
	_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_S;
	loadActorStandAnim(OBJECT_SPOCK);

	if (_roomVar.mudd.tricordersUnavailable)
		showText(TX_SPEAKER_SPOCK, TX_MUD3_039); // BUGFIX: speaker is Spock
	else if (_awayMission->mudd.translatedAlienLanguage || (_awayMission->mudd.discoveredBase3System && !_roomVar.mudd.suggestedUsingTricorders)) {
		showText(TX_SPEAKER_SPOCK, TX_MUD3_041);
		_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_W;
		walkCrewman(OBJECT_SPOCK, 0xcd, 0x87);
	} else if (_awayMission->mudd.discoveredBase3System && _roomVar.mudd.suggestedUsingTricorders) { // Hooking up tricorders
		_awayMission->disableInput = true;
		_awayMission->crewDirectionsAfterWalk[OBJECT_MCCOY] = DIR_S;
		walkCrewmanC(OBJECT_MCCOY, 0xca, 0x88, &Room::mudd3ReadyToHookUpTricorders);
	} else { // No idea how to access it
		showText(TX_SPEAKER_SPOCK, TX_MUD3_042);
		walkCrewman(OBJECT_SPOCK, 0xcd, 0x87);
	}
}

void Room::mudd3ReadyToHookUpTricorders() {
	showText(TX_SPEAKER_SPOCK, TX_MUD3_018);

	_roomVar.mudd.tricordersUnavailable = true;
	_awayMission->timers[1] = 50;

	loadActorAnim2(OBJECT_SPOCK, "sscans");
	playSoundEffectIndex(kSfxTricorder);
}

void Room::mudd3Timer1Expired() { // Tricorders ready for use again
	showText(TX_SPEAKER_SPOCK, TX_MUD3_040);
	showText(TX_SPEAKER_SPOCK, TX_MUD3_034);

	_awayMission->disableInput = false;
	_roomVar.mudd.tricordersUnavailable = false;
	_roomVar.mudd.suggestedUsingTricorders = false;
	_awayMission->mudd.translatedAlienLanguage = true;
	_awayMission->mudd.missionScore++;

	_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_W;
	_awayMission->crewDirectionsAfterWalk[OBJECT_MCCOY] = DIR_W;
	walkCrewman(OBJECT_SPOCK, 0xcd, 0x87);
	walkCrewman(OBJECT_MCCOY, 0xec, 0x84);
}

// Harry Mudd stumbled into the room
void Room::mudd3Timer2Expired() {
	const TextRef choices[] = {
		TX_SPEAKER_KIRK,
		TX_MUD3_010,
		TX_MUD3_009,
		TX_END
	};

	_awayMission->mudd.computerDataErasedOrDestroyed = true;

	showText(TX_SPEAKER_MUDD, TX_MUD3_065);
	int choice = showMultipleTexts(choices);

	if (choice == 0) { // Allow him to access the database (he ends up erasing it)
		showText(TX_SPEAKER_MUDD, TX_MUD3_066);
		_awayMission->mudd.muddErasedDatabase = true;

		// ENHANCEMENT: Add a few lines to make it clear that Mudd erased the databanks.
		// Otherwise, the end of the mission when you confront Mudd doesn't make sense
		// unless the player happened to try accessing the database again. Also, if you
		// talk to the crew, they berate him for no apparent reason if this isn't clear.
		showDescription(TX_MUD3N000);
		showText(TX_SPEAKER_MCCOY, TX_MUD3_031);

	} else { // Don't allow it (he destroys it by accident)
		showText(TX_SPEAKER_MUDD, TX_MUD3_064);
		_awayMission->timers[4] = 98;
		_awayMission->disableInput = 2;
		playMidiMusicTracks(MIDITRACK_26);
		loadActorAnim(OBJECT_MUDD, "s4lbhb", 0xa2, 0x9f);
	}
}

void Room::mudd3Timer4Expired() {
	_awayMission->mudd.databaseDestroyed = true;

	showText(TX_SPEAKER_MUDD, TX_MUD3_063);
	showText(TX_SPEAKER_MCCOY, TX_MUD3_031);

	_awayMission->disableInput = false;
}

void Room::mudd3UseMemoryDiskOnSphere() {
	// ENHANCEMENT: Turn to face south
	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_S;
	loadActorStandAnim(OBJECT_KIRK);

	if (_awayMission->mudd.databaseDestroyed)
		showDescription(TX_MUD3N014);
	else if (_awayMission->mudd.translatedAlienLanguage && !_awayMission->mudd.muddErasedDatabase) {
		showDescription(TX_MUD3N020);
		if (!_awayMission->mudd.gotPointsForDownloadingData) {
			_awayMission->mudd.missionScore += 3;
			_awayMission->mudd.gotPointsForDownloadingData = true;
		}
	} else
		showDescription(TX_MUD3N019);
}


void Room::mudd3GetRepairTool() {
	if (_awayMission->mudd.tookRepairTool)
		showDescription(TX_MUD3N018); // NOTE: unused, since the object disappears, can't be selected again
	else {
		_awayMission->disableInput = true;
		_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_N;
		walkCrewmanC(OBJECT_KIRK, 0xba, 0xc1, &Room::mudd3ReachedRepairTool);
	}
}

void Room::mudd3ReachedRepairTool() {
	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_N;
	loadActorAnimC(OBJECT_KIRK, "kusemn", -1, -1, &Room::mudd3PickedUpRepairTool);
}

void Room::mudd3PickedUpRepairTool() {
	_awayMission->disableInput = false;
	loadActorStandAnim(OBJECT_REPAIR_TOOL);
	_awayMission->mudd.tookRepairTool = true;
	_awayMission->mudd.missionScore++;
	giveItem(OBJECT_IDOOVER);
}

void Room::mudd3LookAtSphere() {
	// ENHANCEMENT: Turn to face south
	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_S;
	loadActorStandAnim(OBJECT_KIRK);

	showDescription(TX_MUD3N015);
}

void Room::mudd3WalkToNorthDoor() {
	_roomVar.mudd.walkingToDoor = true;
	_awayMission->disableInput = true;
	walkCrewman(OBJECT_KIRK, 0xb8, 0x70);
}

void Room::mudd3TouchedHotspot0() { // Triggers north door
	if (_roomVar.mudd.walkingToDoor) {
		playVoc("SMADOOR3");
		loadActorAnim(OBJECT_NORTH_DOOR, "s4lbdo", 0xba, 0x6f);
	}
}

void Room::mudd3WalkToEastDoor() {
	_roomVar.mudd.walkingToDoor = true;
	_awayMission->disableInput = true;
	walkCrewman(OBJECT_KIRK, 0x121, 0x81);
}

void Room::mudd3TouchedHotspot1() {
	if (_roomVar.mudd.walkingToDoor) {
		playVoc("SMADOOR3");
	}
}

void Room::mudd3LookAtKirk() {
	if (!_awayMission->mudd.muddInDatabaseRoom)
		showDescription(TX_MUD3N005);
	else
		showDescription(TX_MUD3N004);
}

void Room::mudd3LookAtSpock() {
	if (!_awayMission->mudd.muddInDatabaseRoom)
		showDescription(TX_MUD3N012);
	else
		showDescription(TX_MUD3N002);
}

void Room::mudd3LookAtMccoy() {
	if (!_awayMission->mudd.muddInDatabaseRoom)
		showDescription(TX_MUD3N010);
	else
		showDescription(TX_MUD3N009);
}

void Room::mudd3LookAtRedshirt() {
	if (!_awayMission->mudd.muddInDatabaseRoom)
		showDescription(TX_MUD3N007);
	else
		showDescription(TX_MUD3N006);
}

void Room::mudd3LookAtMudd() {
	showDescription(TX_MUD3N003);
}

void Room::mudd3TalkToKirk() {
	if (!_awayMission->mudd.muddInDatabaseRoom) {
		showText(TX_SPEAKER_KIRK,  TX_MUD3_014);
		showText(TX_SPEAKER_SPOCK, TX_MUD3_057);
		showText(TX_SPEAKER_MCCOY, TX_MUD3_025);
	} else
		showText(TX_SPEAKER_KIRK, TX_MUD3_002);
}

void Room::mudd3TalkToSpock() {
	if (!_awayMission->mudd.muddInDatabaseRoom) {
		showText(TX_SPEAKER_SPOCK, TX_MUD3_055);
		showText(TX_SPEAKER_MCCOY, TX_MUD3_033);
		showText(TX_SPEAKER_KIRK,  TX_MUD3_011);
	} else
		showText(TX_SPEAKER_SPOCK, TX_MUD3_019);
}

void Room::mudd3TalkToMccoy() {
	if (!_awayMission->mudd.muddInDatabaseRoom)
		showText(TX_SPEAKER_MCCOY, TX_MUD3_023);
	else {
		showText(TX_SPEAKER_MCCOY, TX_MUD3_030);
		showText(TX_SPEAKER_MUDD,  TX_MUD3_062);
	}
}

void Room::mudd3TalkToRedshirt() {
	if (!_awayMission->mudd.muddInDatabaseRoom) {
		showText(TX_SPEAKER_BUCHERT, TX_MUD3_070);
		showText(TX_SPEAKER_KIRK,    TX_MUD3_016);
		showText(TX_SPEAKER_BUCHERT, TX_MUD3_071);
	} else
		showText(TX_SPEAKER_BUCHERT, TX_MUD3_067);
}

void Room::mudd3TalkToMudd() {
	// ENHANCEMENT: the 2nd part of the if condition is new; whether he physically
	// destroys the database or does through software, he should give this reaction.
	if (_awayMission->mudd.databaseDestroyed || _awayMission->mudd.muddErasedDatabase) {
		showText(TX_SPEAKER_MUDD,    TX_MUD3_060);
		showText(TX_SPEAKER_MCCOY,   TX_MUD3_029);
		showText(TX_SPEAKER_MUDD,    TX_MUD3_061);
		showText(TX_SPEAKER_KIRK,    TX_MUD3_013);
		showText(TX_SPEAKER_MUDD,    TX_MUD3_058);
		showText(TX_SPEAKER_SPOCK,   TX_DEM0N009); // ENHANCEMENT: Original text was just "(Raises eyebrow)" without any audio.
		showText(TX_SPEAKER_BUCHERT, TX_MUD3_069);
		showText(TX_SPEAKER_MUDD,    TX_MUD3_059);
		showText(TX_SPEAKER_KIRK,    TX_MUD3_005);
	}
}

void Room::mudd3UseMedkit() {
	showText(TX_SPEAKER_MCCOY, TX_MUD3_020);
}

}
