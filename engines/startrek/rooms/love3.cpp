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
#include "startrek/startrek.h"

#define OBJECT_SERVICE_PANEL 8
#define OBJECT_INSULATION 9
#define OBJECT_WRENCH 10
#define OBJECT_GRATE 11
#define OBJECT_GAS_TANK 12

#define HOTSPOT_ENGINEERING_PANEL 0x20
#define HOTSPOT_MONITOR 0x21
#define HOTSPOT_SHAFT 0x22
#define HOTSPOT_TURBINES 0x23

namespace StarTrek {

extern const RoomAction love3ActionList[] = {
	{ {ACTION_TICK, 1, 0, 0, 0}, &Room::love3Tick1 },
	{ {ACTION_TICK, 80, 0, 0, 0}, &Room::love3Tick80 },
	{ {ACTION_TOUCHED_WARP, 1, 0, 0, 0}, &Room::love3TouchedWarp1 },
	{ {ACTION_TIMER_EXPIRED, 3, 0, 0, 0}, &Room::love3Timer3Expired },
	{ {ACTION_LOOK, 8, 0, 0, 0}, &Room::love3LookAtServicePanel },
	{ {ACTION_LOOK, 0x21, 0, 0, 0}, &Room::love3LookAtMonitor },
	{ {ACTION_LOOK, 10, 0, 0, 0}, &Room::love3LookAtWrench },
	{ {ACTION_LOOK, OBJECT_KIRK, 0, 0, 0}, &Room::love3LookAtKirk },
	{ {ACTION_LOOK, OBJECT_MCCOY, 0, 0, 0}, &Room::love3LookAtMccoy },
	{ {ACTION_LOOK, OBJECT_SPOCK, 0, 0, 0}, &Room::love3LookAtSpock },
	{ {ACTION_LOOK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::love3LookAtRedshirt },
	{ {ACTION_LOOK, 0xff, 0, 0, 0}, &Room::love3LookAnywhere },
	{ {ACTION_LOOK, 0x22, 0, 0, 0}, &Room::love3LookAtShaft },
	{ {ACTION_LOOK, 11, 0, 0, 0}, &Room::love3LookAtGrate },
	{ {ACTION_LOOK, 9, 0, 0, 0}, &Room::love3LookAtPanel },
	{ {ACTION_LOOK, 0x20, 0, 0, 0}, &Room::love3LookAtPanel },
	{ {ACTION_LOOK, 12, 0, 0, 0}, &Room::love3LookAtGasTank },
	{ {ACTION_LOOK, 0x23, 0, 0, 0}, &Room::love3LookAtTurbines },
	{ {ACTION_TALK, OBJECT_KIRK, 0, 0, 0}, &Room::love3TalkToKirk },
	{ {ACTION_TALK, OBJECT_SPOCK, 0, 0, 0}, &Room::love3TalkToSpock },
	{ {ACTION_TALK, OBJECT_MCCOY, 0, 0, 0}, &Room::love3TalkToMccoy },
	{ {ACTION_TALK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::love3TalkToRedshirt },
	{ {ACTION_USE, OBJECT_IMTRICOR, 0xff, 0, 0}, &Room::love3UseMTricorderAnywhere },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0x23, 0, 0}, &Room::love3UseSTricorderOnTurbines },
	{ {ACTION_USE, OBJECT_ISTRICOR, 9, 0, 0}, &Room::love3UseSTricorderOnInsulation },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0xff, 0, 0}, &Room::love3UseSTricorderAnywhere },
	{ {ACTION_USE, OBJECT_KIRK, 8, 0, 0}, &Room::love3UseKirkOnServicePanel },
	{ {ACTION_USE, OBJECT_SPOCK, 8, 0, 0}, &Room::love3UseSpockOnServicePanel },
	{ {ACTION_USE, OBJECT_MCCOY, 8, 0, 0}, &Room::love3UseMccoyOnServicePanel },
	{ {ACTION_USE, OBJECT_REDSHIRT, 8, 0, 0}, &Room::love3UseRedshirtOnServicePanel },
	{ {ACTION_FINISHED_WALKING, 1, 0, 0, 0}, &Room::love3CrewmanReachedServicePanel },
	{ {ACTION_FINISHED_ANIMATION, 1, 0, 0, 0}, &Room::love3OpenedOrClosedServicePanel },

	{ {ACTION_USE, OBJECT_IWRENCH, 12, 0, 0}, &Room::love3UseWrenchOnGasTank },
	{ {ACTION_FINISHED_WALKING, 2, 0, 0, 0}, &Room::love3ReachedGasTankToUnscrew },
	{ {ACTION_FINISHED_ANIMATION, 2, 0, 0, 0}, &Room::love3ScrewedOrUnscrewedGasTank },

	{ {ACTION_USE, OBJECT_IWRENCH, OBJECT_IO2GAS, 0, 0}, &Room::love3UseWrenchOnO2OrH2Gas },
	{ {ACTION_USE, OBJECT_IWRENCH, OBJECT_IH2GAS, 0, 0}, &Room::love3UseWrenchOnO2OrH2Gas },

	{ {ACTION_USE, OBJECT_IWRENCH, OBJECT_IN2GAS, 0, 0}, &Room::love3UseWrenchOnN2Gas },
	{ {ACTION_FINISHED_WALKING, 15, 0, 0, 0}, &Room::love3ReachedGasTankToPutDown },
	{ {ACTION_FINISHED_ANIMATION, 3, 0, 0, 0}, &Room::love3PutN2TankBack },

	{ {ACTION_USE, OBJECT_IWRENCH, 0x20, 0, 0}, &Room::love3UseWrenchOnEngineeringPanel },
	{ {ACTION_FINISHED_WALKING, 3, 0, 0, 0}, &Room::love3ReachedEngineeringPanel },
	{ {ACTION_FINISHED_ANIMATION, 4, 0, 0, 0}, &Room::love3OpenedEngineeringPanel },

	{ {ACTION_USE, OBJECT_IWRENCH, 0x21, 0, 0}, &Room::love3UseWrenchOnMonitor },

	{ {ACTION_USE, OBJECT_IH2O, 0x21, 0, 0}, &Room::love3UseWaterOnMonitor },
	{ {ACTION_FINISHED_WALKING, 14, 0, 0, 0}, &Room::love3ReachedMonitorToUseWater },
	{ {ACTION_FINISHED_ANIMATION, 6, 0, 0, 0}, &Room::love3PouredWaterOnMonitor },

	{ {ACTION_USE, OBJECT_KIRK, 0x20, 0, 0}, &Room::love3UseCrewmanOnEngineeringPanelOrGrate },
	{ {ACTION_USE, OBJECT_SPOCK, 0x20, 0, 0}, &Room::love3UseCrewmanOnEngineeringPanelOrGrate },
	{ {ACTION_USE, OBJECT_MCCOY, 0x20, 0, 0}, &Room::love3UseCrewmanOnEngineeringPanelOrGrate },
	{ {ACTION_USE, OBJECT_REDSHIRT, 0x20, 0, 0}, &Room::love3UseCrewmanOnEngineeringPanelOrGrate },
	{ {ACTION_USE, OBJECT_KIRK, 11, 0, 0}, &Room::love3UseCrewmanOnEngineeringPanelOrGrate },
	{ {ACTION_USE, OBJECT_SPOCK, 11, 0, 0}, &Room::love3UseCrewmanOnEngineeringPanelOrGrate },
	{ {ACTION_USE, OBJECT_MCCOY, 11, 0, 0}, &Room::love3UseCrewmanOnEngineeringPanelOrGrate },
	{ {ACTION_USE, OBJECT_REDSHIRT, 11, 0, 0}, &Room::love3UseCrewmanOnEngineeringPanelOrGrate },

	{ {ACTION_USE, OBJECT_IWRENCH, 11, 0, 0}, &Room::love3UseWrenchOnGrate },
	{ {ACTION_FINISHED_WALKING, 4, 0, 0, 0}, &Room::love3ReachedGateWithWrench },
	{ {ACTION_FINISHED_ANIMATION, 7, 0, 0, 0}, &Room::love3OpenedOrClosedGrate },

	{ {ACTION_USE, OBJECT_KIRK, 0x22, 0, 0}, &Room::love3UseCrewmanOnShaft },
	{ {ACTION_USE, OBJECT_SPOCK, 0x22, 0, 0}, &Room::love3UseCrewmanOnShaft },
	{ {ACTION_USE, OBJECT_MCCOY, 0x22, 0, 0}, &Room::love3UseCrewmanOnShaft },
	{ {ACTION_USE, OBJECT_REDSHIRT, 0x22, 0, 0}, &Room::love3UseCrewmanOnShaft },

	{ {ACTION_USE, OBJECT_IH2O, 0x22, 0, 0}, &Room::love3UseWaterOnShaft },
	{ {ACTION_FINISHED_WALKING, 5, 0, 0, 0}, &Room::love3ReachedShaftUsingWater },
	{ {ACTION_FINISHED_ANIMATION, 8, 0, 0, 0}, &Room::love3PouredWaterDownShaft },

	{ {ACTION_USE, OBJECT_IN2O, 0x22, 0, 0}, &Room::love3UseNitrousOxideOnShaft },
	{ {ACTION_FINISHED_WALKING, 6, 0, 0, 0}, &Room::love3ReachedShaftUsingNitrousOxide },
	{ {ACTION_FINISHED_ANIMATION, 9, 0, 0, 0}, &Room::love3PouredNitrousOxideDownShaft },

	{ {ACTION_USE, OBJECT_INH3, 0x22, 0, 0}, &Room::love3UseAmmoniaOnShaft },
	{ {ACTION_FINISHED_WALKING, 7, 0, 0, 0}, &Room::love3ReachedShaftUsingAmmonia },
	{ {ACTION_FINISHED_ANIMATION, 10, 0, 0, 0}, &Room::love3PouredAmmoniaDownShaft },

	{ {ACTION_USE, OBJECT_IRLG, 0x22, 0, 0}, &Room::love3UseRomulanLaughingGasOnShaft },
	{ {ACTION_FINISHED_WALKING, 8, 0, 0, 0}, &Room::love3ReachedShaftUsingRomulanLaughingGas },
	{ {ACTION_FINISHED_ANIMATION, 11, 0, 0, 0}, &Room::love3PouredRomulanLaughingGasDownShaft },

	{ {ACTION_GET, 10, 0, 0, 0}, &Room::love3GetWrench },
	{ {ACTION_FINISHED_WALKING, 9, 0, 0, 0}, &Room::love3ReachedWrenchToGet },
	{ {ACTION_FINISHED_ANIMATION, 12, 0, 0, 0}, &Room::love3PickedUpWrench },

	{ {ACTION_GET, 12, 0, 0, 0}, &Room::love3GetGasTank },
	{ {ACTION_FINISHED_WALKING, 10, 0, 0, 0}, &Room::love3ReachedGasTankToGet },

	{ {ACTION_USE, OBJECT_IANTIGRA, 12, 0, 0}, &Room::love3UseAntigravOnGasTank },
	{ {ACTION_FINISHED_WALKING, 11, 0, 0, 0}, &Room::love3ReachedGasTankUsingAntigrav },
	{ {ACTION_FINISHED_ANIMATION, 13, 0, 0, 0}, &Room::love3PickedUpGasTank },

	{ {ACTION_GET, 9, 0, 0, 0}, &Room::love3GetInsulation },
	{ {ACTION_FINISHED_WALKING, 12, 0, 0, 0}, &Room::love3ReachedInsulationToGet },
	{ {ACTION_FINISHED_ANIMATION, 14, 0, 0, 0}, &Room::love3PickedUpInsulation },

	// Common code
	{ {ACTION_TIMER_EXPIRED, 0, 0, 0, 0}, &Room::loveaTimer0Expired },
	{ {ACTION_TIMER_EXPIRED, 1, 0, 0, 0}, &Room::loveaTimer1Expired },
	{ {ACTION_USE, OBJECT_IMEDKIT, OBJECT_SPOCK, 0, 0}, &Room::loveaUseMedkitOnSpock },
	{ {ACTION_USE, OBJECT_ISAMPLE, OBJECT_SPOCK, 0, 0}, &Room::loveaUseCureSampleOnSpock },
	{ {ACTION_USE, OBJECT_ICURE, OBJECT_SPOCK, 0, 0}, &Room::loveaUseCureOnSpock },
	{ {ACTION_FINISHED_WALKING,   99, 0, 0, 0}, &Room::loveaSpockOrMccoyInPositionToUseCure },
	{ {ACTION_FINISHED_ANIMATION, 99, 0, 0, 0}, &Room::loveaFinishedCuringSpock },
	{ {ACTION_TIMER_EXPIRED, 2, 0, 0, 0}, &Room::loveaTimer2Expired },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_SPOCK, 0, 0}, &Room::loveaUseMTricorderOnSpock },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_KIRK, 0, 0}, &Room::loveaUseMTricorderOnHuman },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_MCCOY, 0, 0}, &Room::loveaUseMTricorderOnHuman },
	{ {ACTION_USE, OBJECT_IMTRICOR, OBJECT_REDSHIRT, 0, 0}, &Room::loveaUseMTricorderOnHuman },
	{ {ACTION_USE, OBJECT_IRLG, 0xff, 0, 0}, &Room::loveaUseRomulanLaughingGas },
	{ {ACTION_USE, OBJECT_IN2O, 0xff, 0, 0}, &Room::loveaUseHumanLaughingGas },
	{ {ACTION_USE, OBJECT_INH3, 0xff, 0, 0}, &Room::loveaUseAmmonia },
	{ {ACTION_USE, OBJECT_ICOMM, 0xff, 0, 0}, &Room::loveaUseCommunicator },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum love3TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_FERRIS,
	TX_LOV0_011, TX_LOV0_012, TX_LOV0_013, TX_LOV0_014, TX_LOV0_015,
	TX_LOV0_016, TX_LOV0_017, TX_LOV0_019, TX_LOV0_020, TX_LOV0_021,
	TX_LOV0_022, TX_LOV0_025, TX_LOV0_026, TX_LOV0_029, TX_LOV0_033,
	TX_LOV0_039, TX_LOV0_040, TX_LOV0_041, TX_LOV0_042, TX_LOV0_043,
	TX_LOV0_045, TX_LOV0_046, TX_LOV0_047, TX_LOV0_048, TX_LOV0_049,
	TX_LOV0_050, TX_LOV0_101, TX_LOV0_102, TX_LOV0_103, TX_LOV0_104,
	TX_LOV0_105, TX_LOV0_106, TX_LOV0_107, TX_LOV0_124, TX_LOV3_001,
	TX_LOV3_002, TX_LOV3_003, TX_LOV3_004, TX_LOV3_005, TX_LOV3_006,
	TX_LOV3_007, TX_LOV3_008, TX_LOV3_009, TX_LOV3_010, TX_LOV3_011,
	TX_LOV3_012, TX_LOV3_013, TX_LOV3_014, TX_LOV3_015, TX_LOV3_016,
	TX_LOV3_017, TX_LOV3_018, TX_LOV3_019, TX_LOV3_020, TX_LOV3_021,
	TX_LOV3_022, TX_LOV3_023, TX_LOV3_024, TX_LOV3_025, TX_LOV3_026,
	TX_LOV5_015, TX_LOV5_019, TX_LOV5_027, TX_LOV5_030, TX_LOV5_038,
	TX_LOVA_100, TX_LOVA_F01, TX_LOVA_F02, TX_LOVA_F03, TX_LOVA_F04,
	TX_LOVA_F07, TX_LOVA_F08, TX_LOVA_F10, TX_LOVA_F54, TX_LOVA_F55,
	TX_MUD2_040, TX_MUD4_018, TX_TUG2_010, TX_VENA_F41, TX_BRIDU146,
	TX_GENER004, TX_LOV2N005, TX_LOV3N000, TX_LOV3N001, TX_LOV3N002,
	TX_LOV3N003, TX_LOV3N004, TX_LOV3N005, TX_LOV3N006, TX_LOV3N007,
	TX_LOV3N008, TX_LOV3N009, TX_LOV3N010, TX_LOV3N011, TX_LOV3N012,
	TX_LOV3N013, TX_LOV3N014, TX_LOV3N015, TX_LOV3N016, TX_LOV3N017,
	TX_LOV3N018, TX_LOV3N019, TX_LOV3N020, TX_LOV3N021, TX_LOV3N022,
	TX_LOV3N023, TX_LOV3N024, TX_LOV3N025, TX_LOV3N026, TX_LOV3N027,
	TX_LOV3N028, TX_LOV3N029, TX_LOV3N030, TX_LOV3N031, TX_LOV3N032,
	TX_LOV3NA08, TX_LOV3NA09, TX_LOV3NA20, TX_LOV3NA21, TX_LOV3NA22,
	TX_LOV3NA23, TX_LOV3NJ32, TX_DIALOG_ERROR
};

extern const RoomTextOffsets love3TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 9959, 9844, 10369, 10409, 9847, 10476, 10617, 9847, 9983 },
	{ TX_SPEAKER_MCCOY, 9970, 9855, 10380, 10420, 9858, 10489, 10628, 9858, 9994 },
	{ TX_SPEAKER_SPOCK, 9980, 9865, 10390, 10430, 9868, 10499, 10638, 9868, 10004 },
	{ TX_SPEAKER_FERRIS, 9990, 9875, 10400, 10440, 9878, 10509, 10648, 9878, 10014 },
	{ TX_LOV0_011, 12469, 11921, 13187, 13025, 11926, 12660, 13002, 11926, 12497 },
	{ TX_LOV0_012, 12596, 12003, 13317, 13150, 12008, 12743, 13087, 12008, 12624 },
	{ TX_LOV0_013, 12892, 12224, 13657, 13460, 12229, 12981, 13355, 12229, 12917 },
	{ TX_LOV0_014, 12656, 12048, 13391, 13208, 12053, 12787, 13149, 12053, 12684 },
	{ TX_LOV0_015, 12760, 12122, 13506, 13310, 12127, 12860, 13234, 12127, 12785 },
	{ TX_LOV0_016, 12563, 11985, 13281, 13117, 11990, 12722, 13066, 11990, 12591 },
	{ TX_LOV0_017, 13026, 12328, 13798, 13592, 12333, 13084, 13465, 12333, 13051 },
	{ TX_LOV0_019, 12946, 12263, 13719, 13511, 12268, 13017, 13402, 12268, 12971 },
	{ TX_LOV0_020, 12503, 11940, 13217, 13059, 11945, 12679, 13017, 11945, 12531 },
	{ TX_LOV0_021, 12814, 12161, 13570, 13359, 12166, 12895, 13283, 12166, 12839 },
	{ TX_LOV0_022, 12704, 12081, 13444, 13264, 12086, 12828, 13187, 12086, 12729 },
	{ TX_LOV0_025, 13952, 12735, 14842, 14560, 12741, 13580, 13981, 12741, 13977 },
	{ TX_LOV0_026, 13825, 12464, 14699, 14407, 12470, 13227, 13641, 12470, 13850 },
	{ TX_LOV0_029, 13705, 12374, 14547, 14279, 12379, 13129, 13519, 12379, 13730 },
	{ TX_LOV0_033, 13765, 12419, 14617, 14345, 12425, 13180, 13574, 12425, 13790 },
	{ TX_LOV0_039, 13087, 0, 13867, 13652, 11926, 12660, 13002, 11926, 13112 },
	{ TX_LOV0_040, 13214, 0, 13997, 13777, 12008, 12743, 13087, 12008, 13239 },
	{ TX_LOV0_041, 13510, 0, 14337, 14087, 12229, 12981, 13355, 12229, 13535 },
	{ TX_LOV0_042, 13274, 0, 14071, 13835, 12053, 12787, 13149, 12053, 13299 },
	{ TX_LOV0_043, 13378, 0, 14186, 13937, 12127, 12860, 13234, 12127, 13403 },
	{ TX_LOV0_045, 13181, 0, 13961, 13744, 11990, 12722, 13066, 11990, 13206 },
	{ TX_LOV0_046, 13644, 0, 14478, 14219, 12333, 13084, 13465, 12333, 13669 },
	{ TX_LOV0_047, 13564, 0, 14399, 14138, 12268, 13017, 13402, 12268, 13589 },
	{ TX_LOV0_048, 13121, 0, 13897, 13686, 11945, 12679, 13017, 11945, 13146 },
	{ TX_LOV0_049, 13432, 0, 14250, 13986, 12166, 12895, 13283, 12166, 13457 },
	{ TX_LOV0_050, 13322, 0, 14124, 13891, 12086, 12828, 13187, 12086, 13347 },
	{ TX_LOV0_101, 14033, 0, 14927, 14654, 0, 0, 0, 0, 14058 },
	{ TX_LOV0_102, 14097, 0, 14973, 14721, 0, 0, 0, 0, 14122 },
	{ TX_LOV0_103, 14167, 0, 15054, 14803, 0, 0, 0, 0, 14192 },
	{ TX_LOV0_104, 14217, 0, 15110, 14861, 0, 0, 0, 0, 14242 },
	{ TX_LOV0_105, 14274, 0, 15171, 14927, 0, 0, 0, 0, 14299 },
	{ TX_LOV0_106, 14386, 0, 15285, 15050, 0, 0, 0, 0, 14411 },
	{ TX_LOV0_107, 14454, 0, 15361, 15131, 0, 0, 0, 0, 14479 },
	{ TX_LOV0_124, 13902, 12526, 14788, 14504, 12532, 13309, 13715, 12532, 13927 },
	{ TX_LOV3_001, 1598, 1467, 1603, 1627, 1467, 1487, 1472, 1467, 1588 },
	{ TX_LOV3_002, 11502, 11120, 12039, 12024, 11124, 11827, 12019, 11124, 11530 },
	{ TX_LOV3_003, 11007, 10700, 11489, 11515, 10704, 11391, 11544, 10704, 11035 },
	{ TX_LOV3_004, 11136, 10814, 11625, 11651, 10818, 11512, 11665, 10818, 11164 },
	{ TX_LOV3_005, 11251, 10914, 11753, 11789, 10918, 11636, 11778, 10918, 11279 },
	{ TX_LOV3_006, 2083, 1888, 2157, 2136, 1888, 1930, 1958, 1888, 2073 },
	{ TX_LOV3_007, 14507, 12854, 15423, 15196, 12860, 13716, 14118, 12860, 14532 },
	{ TX_LOV3_008, 1826, 1665, 1849, 1875, 1665, 1702, 1684, 1665, 1816 },
	{ TX_LOV3_009, 10712, 10450, 11171, 11215, 10454, 11137, 11271, 10454, 10740 },
	{ TX_LOV3_010, 684, 658, 684, 684, 658, 658, 658, 658, 674 },
	{ TX_LOV3_011, 5079, 4472, 5348, 5225, 4471, 4598, 4735, 4471, 5069 },
	{ TX_LOV3_012, 4619, 4057, 4857, 4734, 4056, 4153, 4289, 4056, 4609 },
	{ TX_LOV3_013, 11334, 10982, 11834, 11868, 10986, 11701, 11844, 10986, 11362 },
	{ TX_LOV3_014, 11378, 11011, 11882, 11915, 11015, 11733, 11877, 11015, 11406 },
	{ TX_LOV3_015, 6046, 5281, 6431, 6217, 5280, 5439, 5659, 5280, 6036 },
	{ TX_LOV3_016, 3156, 2815, 3307, 3251, 2814, 2892, 2962, 2814, 3146 },
	{ TX_LOV3_017, 1696, 1550, 1701, 1734, 1550, 1576, 1552, 1550, 1686 },
	{ TX_LOV3_018, 11891, 11419, 12478, 12463, 11423, 12173, 12368, 11423, 11919 },
	{ TX_LOV3_019, 10794, 10517, 11260, 11296, 10521, 11203, 11345, 10521, 10822 },
	{ TX_LOV3_020, 851, 810, 861, 863, 810, 816, 820, 810, 841 },
	{ TX_LOV3_021, 3714, 3267, 3882, 3814, 3266, 3349, 3430, 3266, 3704 },
	{ TX_LOV3_022, 2953, 2644, 3079, 3051, 2643, 2722, 2764, 2643, 2943 },
	{ TX_LOV3_023, 4928, 4336, 5193, 5069, 4335, 4457, 4595, 4335, 4918 },
	{ TX_LOV3_024, 1960, 1780, 2004, 1994, 1780, 1806, 1823, 1780, 1950 },
	{ TX_LOV3_025, 5869, 5133, 6203, 6021, 5132, 5266, 5461, 5132, 5859 },
	{ TX_LOV3_026, 10954, 10662, 11427, 11462, 10666, 11354, 11497, 10666, 10982 },
	{ TX_LOV5_015, 7076, 0, 7486, 7278, 0, 0, 0, 0, 7066 },
	//{ TX_LOV5_015, 7255, 0, 0, 0 },	// ignore duplicate line
	{ TX_LOV5_019, 6998, 6021, 7413, 7190, 6020, 6200, 6429, 6020, 6988 },
	{ TX_LOV5_027, 7511, 6465, 7908, 7721, 6464, 6667, 6913, 6464, 7501 },
	{ TX_LOV5_030, 7415, 6396, 7816, 7624, 6395, 6589, 6840, 6395, 7405 },
	{ TX_LOV5_038, 7466, 6435, 7866, 7680, 6434, 6636, 6886, 6434, 7456 },
	{ TX_LOVA_100, 8620, 7563, 8993, 8946, 7564, 7890, 8114, 7564, 8636 },
	{ TX_LOVA_F01, 8062, 0, 8464, 8332, 0, 0, 0, 0, 8078 },
	{ TX_LOVA_F02, 8134, 0, 8529, 8404, 0, 0, 0, 0, 8150 },
	{ TX_LOVA_F03, 8333, 0, 8738, 8632, 0, 0, 0, 0, 8349 },
	{ TX_LOVA_F04, 8248, 0, 8646, 8530, 0, 0, 0, 0, 8264 },
	{ TX_LOVA_F07, 8697, 7625, 9072, 9024, 7626, 7953, 8178, 7626, 8713 },
	{ TX_LOVA_F08, 7686, 0, 8082, 7909, 0, 0, 0, 0, 7676 },
	//{ TX_LOVA_F08, 8401, 0, 0, 0 }, // ignore duplicate line
	{ TX_LOVA_F10, 8568, 0, 8946, 8886, 0, 0, 0, 0, 8584 },
	{ TX_LOVA_F54, 7842, 0, 8234, 8075, 0, 0, 0, 0, 7833 },
	{ TX_LOVA_F55, 7143, 6136, 7552, 7349, 6135, 6306, 6551, 6135, 7133 },
	{ TX_MUD2_040, 8893, 0, 9293, 9265, 0, 0, 0, 0, 8909 },
	{ TX_MUD4_018, 9435, 0, 9849, 9859, 0, 0, 0, 0, 9459 },
	//{ TX_MUD4_018, 9266, 0, 0, 0 },	// ignore duplicate line
	//{ TX_MUD4_018, 9599, 0, 0, 0 },	// ignore duplicate line
	//{ TX_MUD4_018, 9102, 0, 0, 0 },	// ignore duplicate line
	{ TX_TUG2_010, 8498, 7457, 8893, 8812, 7458, 7777, 7999, 7458, 8514 },
	{ TX_VENA_F41, 9389, 8654, 9803, 9812, 8657, 9110, 9287, 8657, 9413 },
	//{ TX_VENA_F41, 9722, 0, 0, 0 },	// ignore duplicate line
	//{ TX_VENA_F41, 9225, 0, 0, 0 },	// ignore duplicate line
	//{ TX_VENA_F41, 9558, 0, 0, 0 },	// ignore duplicate line
	{ TX_BRIDU146, 9668, 8711, 10078, 10105, 8714, 9168, 9249, 8714, 9692 },
	//{ TX_BRIDU146, 9335, 0, 0, 0 },	// ignore duplicate line
	//{ TX_BRIDU146, 9504, 0, 0, 0 },	// ignore duplicate line
	//{ TX_BRIDU146, 9171, 0, 0, 0 },	// ignore duplicate line
	{ TX_GENER004, 7991, 6882, 8393, 8261, 6883, 7143, 7382, 6883, 8007 },
	{ TX_LOV2N005, 8756, 0, 9142, 9084, 0, 0, 0, 0, 8772 },
	//{ TX_LOV2N005, 8982, 0, 0, 0 },	// ignore duplicate line
	//{ TX_LOV2N005, 8836, 0, 0, 0 },	// ignore duplicate line
	{ TX_LOV3N000, 10223, 10078, 10654, 10703, 10081, 10735, 10872, 10081, 10247 },	// typo: the audio file is wrong
	{ TX_LOV3N001, 4852, 4275, 5102, 4986, 4274, 4389, 4519, 4274, 4842 },
	{ TX_LOV3N002, 10260, 10103, 10703, 10742, 10106, 10762, 10909, 10106, 10287 },
	{ TX_LOV3N003, 10324, 10152, 10785, 10819, 10155, 10824, 10976, 10155, 10351 },
	{ TX_LOV3N004, 10475, 10258, 10911, 10968, 10261, 10928, 11057, 10261, 10502 },
	{ TX_LOV3N005, 10381, 10194, 10835, 10874, 10197, 10864, 11011, 10197, 10408 },
	{ TX_LOV3N006, 1503, 1387, 1500, 1532, 1387, 1408, 1384, 1387, 1493 },
	{ TX_LOV3N007, 5476, 4805, 5750, 5622, 4804, 4931, 5073, 4804, 5466 },
	{ TX_LOV3N008, 10423, 10221, 10868, 10922, 10224, 10897, 11029, 10224, 10450 },
	{ TX_LOV3N009, 4422, 3890, 4649, 4537, 3889, 3987, 4112, 3889, 4412 },
	{ TX_LOV3N010, 5947, 5197, 6310, 6102, 5196, 5332, 5553, 5196, 5937 },
	{ TX_LOV3N011, 4277, 3760, 4496, 4387, 3759, 3852, 3974, 3759, 4267 },
	{ TX_LOV3N012, 2895, 2601, 3011, 2982, 2600, 2668, 2711, 2600, 2885 },
	{ TX_LOV3N013, 3371, 2990, 3521, 3474, 2989, 3075, 3136, 2989, 3361 },
	{ TX_LOV3N014, 5550, 4870, 5843, 5683, 4869, 4977, 5157, 4869, 5540 },
	{ TX_LOV3N015, 3060, 2734, 3187, 3157, 2733, 2813, 2857, 2733, 3050 },
	{ TX_LOV3N016, 12045, 11558, 12680, 12610, 11562, 12305, 12555, 11562, 12073 },
	{ TX_LOV3N017, 1374, 1273, 1388, 1403, 1273, 1295, 1287, 1273, 1364 },
	{ TX_LOV3N018, 4564, 4017, 4796, 4678, 4016, 4113, 4244, 4016, 4554 },
	{ TX_LOV3N019, 1272, 1186, 1287, 1286, 1186, 1194, 1201, 1186, 1262 },
	{ TX_LOV3N020, 2580, 2356, 2672, 2648, 2355, 2413, 2443, 2355, 2570 },
	{ TX_LOV3N021, 11748, 11291, 12293, 12310, 11295, 12038, 12198, 11295, 11776 },
	{ TX_LOV3N022, 2675, 2421, 2774, 2760, 2420, 2489, 2514, 2420, 2665 },
	{ TX_LOV3N023, 2631, 2392, 2726, 2708, 2391, 2455, 2482, 2391, 2621 },
	{ TX_LOV3N024, 10535, 10303, 10975, 11016, 10307, 10968, 11106, 10307, 10562 },
	{ TX_LOV3N025, 12110, 11608, 12766, 12674, 11612, 12354, 12626, 11612, 12138 },
	{ TX_LOV3N026, 10600, 10353, 11042, 11085, 10357, 11022, 11157, 10357, 10627 },
	{ TX_LOV3N027, 11699, 11257, 12249, 12261, 11261, 12004, 12169, 11261, 11727 },
	{ TX_LOV3N028, 11606, 11194, 12160, 12148, 11198, 11921, 12110, 11198, 11634 },
	{ TX_LOV3N029, 11650, 11223, 12198, 12203, 11227, 11961, 12133, 11227, 11678 },
	{ TX_LOV3N030, 3914, 3452, 4099, 4014, 3451, 3534, 3632, 3451, 3904 },
	{ TX_LOV3N031, 11563, 11166, 12115, 12089, 11170, 11877, 12080, 11170, 11591 },
	{ TX_LOV3N032, 4188, 3686, 4395, 4293, 3685, 3773, 3888, 3685, 4178 },
	{ TX_LOV3NA08, 12359, 11826, 13051, 12926, 11831, 12576, 12881, 11831, 12387 },
	{ TX_LOV3NA09, 12156, 11639, 12826, 12727, 11643, 12392, 12671, 11643, 12184 },
	{ TX_LOV3NA20, 10123, 9993, 10546, 10583, 9996, 10636, 10779, 9996, 10147 },
	{ TX_LOV3NA21, 1187, 1116, 1194, 1185, 1116, 1108, 1123, 1116, 1177 },
	{ TX_LOV3NA22, 10002, 9887, 10412, 10452, 9890, 10521, 10660, 9890, 10026 },
	{ TX_LOV3NA23, 1089, 1033, 1095, 1093, 1033, 1031, 1039, 1033, 1079 },
	{ TX_LOV3NJ32, 5767, 5046, 6078, 5912, 5045, 5165, 5351, 5045, 5757 },
	{ TX_DIALOG_ERROR, 14528, 12860, 15445, 15219, 12866, 13725, 14125, 12866, 14553 },
	{          -1, 0,    0, 0, 0, 0, 0, 0, 0, 0 }
};

extern const RoomText love3Texts[] = {
	{ -1, Common::UNK_LANG, "" }
};

void Room::love3Tick1() {
	playVoc("LOV3LOOP");

	if (!_awayMission->love.wrenchTaken)
		loadActorAnim(OBJECT_WRENCH, "wrench", 0x10a, 0xb6, 0);

	if (_awayMission->love.grateRemoved)
		loadActorAnim(OBJECT_GRATE, "s3r4g3", 0, 0, 0);
	else
		loadActorAnim(OBJECT_GRATE, "s3r4g1", 0x116, 0x8f, 0);

	if (_awayMission->love.insulationOnGround)
		loadActorAnim(OBJECT_INSULATION, "s3r4p2", 0, 0, 0);

	if (!_awayMission->love.tookN2TankFromServicePanel)
		loadActorAnim(OBJECT_GAS_TANK, "r4n2", 0, 0, 0);

	if (_awayMission->love.servicePanelOpen)
		loadActorAnim(OBJECT_SERVICE_PANEL, "s3r4d1a", 1, 0, 0);
	else
		loadActorAnim(OBJECT_SERVICE_PANEL, "s3r4d2", 1, 0, 0);

	if (_awayMission->love.releasedHumanLaughingGas)
		_awayMission->timers[0] = getRandomWordInRange(200, 400);
	if (_awayMission->love.releasedRomulanLaughingGas)
		_awayMission->timers[1] = getRandomWordInRange(200, 400);
	_awayMission->timers[2] = 200;

	_roomVar.love.cmnXPosToCureSpock = 0xb4;
	_roomVar.love.cmnYPosToCureSpock = 0xb7;
}

void Room::love3Tick80() {
	if (!_awayMission->love.visitedRoomWithRomulans && haveItem(OBJECT_IRLG)) {
		showText(TX_SPEAKER_MCCOY, TX_LOV3_010);
		showText(TX_SPEAKER_SPOCK, TX_LOV3_020);
	}
}

void Room::love3TouchedWarp1() {
	_awayMission->disableInput = true;
	playSoundEffectIndex(kSfxDoor);
	_awayMission->timers[3] = 10;
}

void Room::love3Timer3Expired() {
	loadRoomIndex(1, 0);
}

void Room::love3LookAtServicePanel() {
	showDescription(TX_LOV3NA20);
}

void Room::love3LookAtMonitor() {
	showDescription(TX_LOV3N000); // BUGFIX: original didn't play the audio
}

void Room::love3LookAtWrench() {
	showDescription(TX_LOV3N002);
}

void Room::love3LookAtKirk() {
	showDescription(TX_LOV3N003);
}

void Room::love3LookAtMccoy() {
	showDescription(TX_LOV3N005);
}

void Room::love3LookAtSpock() {
	showDescription(TX_LOV3N008);
}

void Room::love3LookAtRedshirt() {
	showDescription(TX_LOV3N004);
}

void Room::love3LookAnywhere() {
	showDescription(TX_LOV3NA23);
}

void Room::love3LookAtShaft() {
	showDescription(TX_LOV3NA21);
}

void Room::love3LookAtGrate() {
	showDescription(TX_LOV3N019);
}

void Room::love3LookAtPanel() {
	if (_awayMission->love.insulationOnGround)
		showDescription(TX_LOV3N017);
	else
		showDescription(TX_LOV3NA22);
}

void Room::love3LookAtGasTank() {
	showDescription(TX_LOV3N006);
}

void Room::love3LookAtTurbines() {
	showDescription(TX_LOV3N026);
	showText(TX_SPEAKER_MCCOY, TX_LOV3_009);
	showText(TX_SPEAKER_SPOCK, TX_LOV3_019);
	if (!_awayMission->redshirtDead)
		showText(TX_SPEAKER_FERRIS, TX_LOV3_026);
}

void Room::love3TalkToKirk() {
	showText(TX_SPEAKER_KIRK, TX_LOV3_001);
}

void Room::love3TalkToSpock() {
	if (_awayMission->love.visitedRoomWithRomulans)
		showText(TX_SPEAKER_SPOCK, TX_LOV3_003);
	else if (_awayMission->love.romulansUnconsciousFromLaughingGas)
		showText(TX_SPEAKER_SPOCK, TX_LOV3_017);
	else if (haveItem(OBJECT_IRLG))
		showText(TX_SPEAKER_SPOCK, TX_LOV3_004);
	else
		showText(TX_SPEAKER_SPOCK, TX_LOV3_003);
}

void Room::love3TalkToMccoy() {
	showText(TX_SPEAKER_MCCOY, TX_LOV3_008);
}

void Room::love3TalkToRedshirt() {
	showText(TX_SPEAKER_FERRIS, TX_LOV3_024);
}

void Room::love3UseMTricorderAnywhere() {
	if (_awayMission->love.knowAboutVirus)
		mccoyScan(DIR_S, TX_SPEAKER_MCCOY, TX_LOV3_005, false);
	else
		mccoyScan(DIR_S, TX_SPEAKER_MCCOY, TX_LOV3_006, false);
}

void Room::love3UseSTricorderOnTurbines() {
	spockScan(DIR_N, TX_SPEAKER_SPOCK, TX_LOV3_014, false);
}

void Room::love3UseSTricorderOnInsulation() {
	if (_awayMission->love.insulationOnGround)
		spockScan(DIR_W, TX_SPEAKER_SPOCK, TX_LOV3_002, false);
}

void Room::love3UseSTricorderAnywhere() {
	spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_LOV3_013, false);
}

void Room::love3UseKirkOnServicePanel() {
	_roomVar.love.activeCrewman = OBJECT_KIRK;
	walkCrewman(_roomVar.love.activeCrewman, 0x104, 0xae, 1);
}

void Room::love3UseSpockOnServicePanel() {
	_roomVar.love.activeCrewman = OBJECT_SPOCK;
	walkCrewman(_roomVar.love.activeCrewman, 0x104, 0xae, 1);
}

void Room::love3UseMccoyOnServicePanel() {
	_roomVar.love.activeCrewman = OBJECT_MCCOY;
	walkCrewman(_roomVar.love.activeCrewman, 0x104, 0xae, 1);
}

void Room::love3UseRedshirtOnServicePanel() {
	_roomVar.love.activeCrewman = OBJECT_REDSHIRT;
	walkCrewman(_roomVar.love.activeCrewman, 0x104, 0xae, 1);
}

void Room::love3CrewmanReachedServicePanel() {
	Common::String anim = getCrewmanAnimFilename(_roomVar.love.activeCrewman, "usehe");
	loadActorAnim2(_roomVar.love.activeCrewman, anim, -1, -1, 1);
}

void Room::love3OpenedOrClosedServicePanel() {
	if (_awayMission->love.servicePanelOpen)
		loadActorAnim2(OBJECT_SERVICE_PANEL, "s3r4d1b", 1, 0, 0);
	else
		loadActorAnim2(OBJECT_SERVICE_PANEL, "s3r4d1", 1, 0, 0);

	if (_vm->isFloppyVersion())
		playSoundEffectIndex(kSfxDoor);
	else
		playVoc("SMADOOR1");
	_awayMission->love.servicePanelOpen = !_awayMission->love.servicePanelOpen;

	if (_awayMission->love.servicePanelOpen) {
		if (_awayMission->love.tookN2TankFromServicePanel)
			showDescription(TX_LOV3N023);
		else
			showDescription(TX_LOV3N022);
	} else
		showDescription(TX_LOV3N020);
}

// Use wrench on gas tank, to screw or unscrew it in position
void Room::love3UseWrenchOnGasTank() {
	walkCrewman(OBJECT_KIRK, 0x10e, 0xae, 2);
}

void Room::love3ReachedGasTankToUnscrew() {
	loadActorAnim2(OBJECT_KIRK, "kusehe", -1, -1, 2);
	playVoc("MUR3E1S");
}

void Room::love3ScrewedOrUnscrewedGasTank() {
	if (!_awayMission->love.gasTankUnscrewed) {
		showDescription(TX_LOV3N012);
		if (!_awayMission->redshirtDead)
			showText(TX_SPEAKER_FERRIS, TX_LOV3_022);
		_awayMission->love.gasTankUnscrewed = true;
	} else {
		showDescription(TX_LOV3N015);
		_awayMission->love.gasTankUnscrewed = false;
	}
}


void Room::love3UseWrenchOnO2OrH2Gas() {
	showText(TX_SPEAKER_SPOCK, TX_LOV3_016);
}


// Use wrench on N2 gas = putting it from the inventory back to the service panel
void Room::love3UseWrenchOnN2Gas() {
	walkCrewman(OBJECT_KIRK, 0x10e, 0xae, 15);
}

void Room::love3ReachedGasTankToPutDown() {
	loadActorAnim2(OBJECT_KIRK, "kusehe", -1, -1, 3);
	playVoc("MUR3E1S");
}

void Room::love3PutN2TankBack() {
	showDescription(TX_LOV3N013);
	_awayMission->love.tookN2TankFromServicePanel = false;
	loseItem(OBJECT_IN2GAS);
	loadActorAnim(OBJECT_GAS_TANK, "r4n2", 0, 0, 0);
}


// Use wrench on engineering panel, insulation comes out
void Room::love3UseWrenchOnEngineeringPanel() {
	walkCrewman(OBJECT_KIRK, 0x37, 0xb4, 3);
}

void Room::love3ReachedEngineeringPanel() {
	loadActorAnim2(OBJECT_KIRK, "kuselw", -1, -1, 4);
	playVoc("MUR3E1S");
}

void Room::love3OpenedEngineeringPanel() {
	if (!_awayMission->love.insulationOnGround) {
		playVoc("MADR4E4A");
		loadActorAnim(OBJECT_INSULATION, "s3r4p1", 0, 0, 0);
		_awayMission->love.insulationOnGround = true;
	}
	showDescription(TX_LOV3N021);
}


void Room::love3UseWrenchOnMonitor() {
	showText(TX_SPEAKER_SPOCK, TX_LOV3_018);
}


void Room::love3UseWaterOnMonitor() {
	walkCrewmanC(OBJECT_KIRK, 0xc8, 0xb2, &Room::love3ReachedMonitorToUseWater);
}

void Room::love3ReachedMonitorToUseWater() {
	loadActorAnimC(OBJECT_KIRK, "kusemn", -1, -1, &Room::love3PouredWaterOnMonitor);
}

void Room::love3PouredWaterOnMonitor() {
	// BUGFIX: the speaker should be Lt. Ferris. In the original, the speaker wasn't
	// specified.
	// However, there is no substitute text for when Lt. Ferris is dead.
	showText(TX_SPEAKER_FERRIS, TX_LOV3_021);

	loseItem(OBJECT_IH2O);
}


void Room::love3UseCrewmanOnEngineeringPanelOrGrate() {
	showDescription(TX_LOV3N030);
}


void Room::love3UseWrenchOnGrate() {
	walkCrewmanC(OBJECT_KIRK, 0xf6, 0xaa, &Room::love3ReachedGateWithWrench);
	_awayMission->disableInput = true;
}

void Room::love3ReachedGateWithWrench() {
	if (_awayMission->love.grateRemoved)
		loadActorAnimC(OBJECT_KIRK, "s3r4g2a", -1, -1, &Room::love3OpenedOrClosedGrate);
	else
		loadActorAnimC(OBJECT_KIRK, "s3r4g2",  -1, -1, &Room::love3OpenedOrClosedGrate);
	loadActorStandAnim(OBJECT_GRATE);
	playVoc("MUR3E1S");
}

void Room::love3OpenedOrClosedGrate() {
	_awayMission->disableInput = false;
	loadActorStandAnim(OBJECT_KIRK);

	if (!_awayMission->love.grateRemoved) {
		_awayMission->love.grateRemoved = true;
		loadActorAnim(OBJECT_GRATE, "s3r4g3", 0, 0, 0);
		showDescription(TX_LOV3N016);
		if (!_awayMission->love.gotPointsForOpeningGrate) {
			_awayMission->love.gotPointsForOpeningGrate = true;
			_awayMission->love.missionScore++;
		}
	} else {
		loadActorAnim(OBJECT_GRATE, "s3r4g1", 0x116, 0x8f, 0);
		_awayMission->love.grateRemoved = false;
		showDescription(TX_LOV3N032);
	}
}


void Room::love3UseCrewmanOnShaft() {
	showDescription(TX_LOV3N011);
}


void Room::love3UseWaterOnShaft() {
	walkCrewmanC(OBJECT_KIRK, 0x100, 0xa6, &Room::love3ReachedShaftUsingWater);
}

void Room::love3ReachedShaftUsingWater() {
	if (!_awayMission->love.grateRemoved)
		showDescription(TX_LOV3N025);
	else
		loadActorAnimC(OBJECT_KIRK, "kuseme", -1, -1, &Room::love3PouredWaterDownShaft);
}

void Room::love3PouredWaterDownShaft() {
	showDescription(TX_LOV3N009);
	loseItem(OBJECT_IH2O);
}


void Room::love3UseNitrousOxideOnShaft() {
	walkCrewmanC(OBJECT_KIRK, 0x100, 0xa6, &Room::love3ReachedShaftUsingNitrousOxide);
}

void Room::love3ReachedShaftUsingNitrousOxide() {
	if (!_awayMission->love.grateRemoved)
		showDescription(TX_LOV3N025);
	else
		loadActorAnimC(OBJECT_KIRK, "kuseme", -1, -1, &Room::love3PouredNitrousOxideDownShaft);
}

void Room::love3PouredNitrousOxideDownShaft() {
	showDescription(TX_LOV3N018);
	showText(TX_SPEAKER_SPOCK, TX_LOV3_012);
	loseItem(OBJECT_IN2O);
}


void Room::love3UseAmmoniaOnShaft() {
	walkCrewmanC(OBJECT_KIRK, 0x100, 0xa6, &Room::love3ReachedShaftUsingAmmonia);
}

void Room::love3ReachedShaftUsingAmmonia() {
	if (!_awayMission->love.grateRemoved)
		showDescription(TX_LOV3N025);
	else
		loadActorAnimC(OBJECT_KIRK, "kuseme", -1, -1, &Room::love3PouredAmmoniaDownShaft);
}

void Room::love3PouredAmmoniaDownShaft() {
	if (!_awayMission->love.romulansUnconsciousFromVirus) {
		showDescription(TX_LOV3N001);
		if (!_awayMission->redshirtDead)
			showText(TX_SPEAKER_FERRIS, TX_LOV3_023);
	}

	loseItem(OBJECT_INH3);
}


void Room::love3UseRomulanLaughingGasOnShaft() {
	if (_awayMission->love.gotPointsForGassingRomulans) {
		// BUG-ish: you could get the points for this by starting the action, then
		// canceling it before Kirk reaches the shaft.
		_awayMission->love.gotPointsForGassingRomulans = true;
		_awayMission->love.missionScore += 6;
	}
	if (_awayMission->love.romulansUnconsciousFromLaughingGas)
		showText(TX_SPEAKER_SPOCK, TX_LOV3_011);
	else {
		walkCrewmanC(OBJECT_KIRK, 0xf6, 0xa4, &Room::love3ReachedShaftUsingRomulanLaughingGas);
	}
}

void Room::love3ReachedShaftUsingRomulanLaughingGas() {
	if (!_awayMission->love.grateRemoved)
		showDescription(TX_LOV3N025);
	else {
		loadActorAnimC(OBJECT_KIRK, "kuseme", -1, -1, &Room::love3PouredRomulanLaughingGasDownShaft);
		playVoc("EFX24");
	}
}

void Room::love3PouredRomulanLaughingGasDownShaft() {
	loseItem(OBJECT_IRLG);
	_awayMission->love.romulansUnconsciousFromLaughingGas = true;

	if (_awayMission->love.romulansUnconsciousFromVirus) {
		showDescription(TX_LOV3NA08);
		_awayMission->love.romulansUnconsciousFromLaughingGas = false;
	} else {
		showDescription(TX_LOV3NA09);
		playVoc("ROMULANL");
	}
}


// Get the wrench on the ground
void Room::love3GetWrench() {
	walkCrewmanC(OBJECT_KIRK, 0x104, 0xb6, &Room::love3ReachedWrenchToGet);
}

void Room::love3ReachedWrenchToGet() {
	loadActorAnimC(OBJECT_KIRK, "s5r1kg", -1, -1, &Room::love3PickedUpWrench);
}

void Room::love3PickedUpWrench() {
	loadActorStandAnim(OBJECT_KIRK);
	giveItem(OBJECT_IWRENCH);
	loadActorStandAnim(OBJECT_WRENCH);
	_awayMission->love.wrenchTaken = true;
	showDescription(TX_LOV3N031);
}


// Get the N2 gas tank in the service panel (fails due to weight)
void Room::love3GetGasTank() {
	walkCrewmanC(OBJECT_KIRK, 0x121, 0xb7, &Room::love3ReachedGasTankToGet);
}

void Room::love3ReachedGasTankToGet() {
	if (_awayMission->love.gasTankUnscrewed)
		showDescription(TX_LOV3N007);
	else
		showDescription(TX_LOV3N014);
}


// Use antigrav unit N2 gas tank in the service panel (succeeds)
void Room::love3UseAntigravOnGasTank() {
	walkCrewmanC(OBJECT_KIRK, 0x10e, 0xae, &Room::love3ReachedGasTankUsingAntigrav);
}

void Room::love3ReachedGasTankUsingAntigrav() {
	if (_awayMission->love.gasTankUnscrewed) {
		loadActorAnimC(OBJECT_KIRK, "kusehe", -1, -1, &Room::love3PickedUpGasTank);
		playVoc("SE3PLBAT");
	} else {
		showDescription(TX_LOV3N010);
		showText(TX_SPEAKER_SPOCK, TX_LOV3_015);
	}
}

void Room::love3PickedUpGasTank() {
	giveItem(OBJECT_IN2GAS);
	loadActorStandAnim(OBJECT_GAS_TANK);
	_awayMission->love.tookN2TankFromServicePanel = true;

	showDescription(TX_LOV3NJ32);
	if (!_awayMission->redshirtDead)
		showText(TX_SPEAKER_FERRIS, TX_LOV3_025);
}

void Room::love3GetInsulation() {
	walkCrewmanC(OBJECT_KIRK, 0x32, 0xbe, &Room::love3ReachedInsulationToGet);
}

void Room::love3ReachedInsulationToGet() {
	if (!_awayMission->love.field3c)
		loadActorAnimC(OBJECT_KIRK, "s5r1kg", -1, -1, &Room::love3PickedUpInsulation);
}

void Room::love3PickedUpInsulation() {
	loadActorStandAnim(OBJECT_KIRK);
	giveItem(OBJECT_IINSULAT);
	showDescription(TX_LOV3N029);
}

}
