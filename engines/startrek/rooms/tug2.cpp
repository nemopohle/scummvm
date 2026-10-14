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

namespace StarTrek {

#define OBJECT_GUARD_1 8
#define OBJECT_GUARD_2 9
#define OBJECT_BRIG 10
#define OBJECT_BOMB 11
#define OBJECT_WIRE 12
#define OBJECT_13 13

#define HOTSPOT_BUTTON 0x20
#define HOTSPOT_DOOR   0x21
#define HOTSPOT_OPEN_BRIG 0x22

#define GUARDSTAT_UP 0
#define GUARDSTAT_STUNNED 1
#define GUARDSTAT_DEAD 2
#define GUARDSTAT_TIED 8

extern const RoomAction tug2ActionList[] = {
	{ {ACTION_TICK, 1, 0, 0, 0}, &Room::tug2Tick1 },
	{ {ACTION_TICK, 60, 0, 0, 0}, &Room::tug2Tick60 },
	{ {ACTION_LOOK, 0x20, 0, 0, 0}, &Room::tug2LookAtButton },
	{ {ACTION_LOOK, OBJECT_MCCOY, 0, 0, 0}, &Room::tug2LookAtMccoy },
	{ {ACTION_LOOK, OBJECT_SPOCK, 0, 0, 0}, &Room::tug2LookAtSpock },
	{ {ACTION_LOOK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::tug2LookAtRedshirt },
	{ {ACTION_GET, 11, 0, 0, 0}, &Room::tug2GetBomb },
	{ {ACTION_FINISHED_WALKING, 12, 0, 0, 0}, &Room::tug2KirkReachedBomb },
	{ {ACTION_FINISHED_ANIMATION, 13, 0, 0, 0}, &Room::tug2KirkGotBomb },
	{ {ACTION_LOOK, 11, 0, 0, 0}, &Room::tug2LookAtBomb },
	{ {ACTION_LOOK, 8, 0, 0, 0}, &Room::tug2LookAtGuard1 },
	{ {ACTION_LOOK, 9, 0, 0, 0}, &Room::tug2LookAtGuard2 },
	{ {ACTION_LOOK, 12, 0, 0, 0}, &Room::tug2LookAtWires },
	{ {ACTION_USE, OBJECT_ISTRICOR, 12, 0, 0}, &Room::tug2UseSTricorderOnButton },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0x20, 0, 0}, &Room::tug2UseSTricorderOnButton },
	{ {ACTION_USE, OBJECT_MCCOY, 12, 0, 0}, &Room::tug2UseMccoyOnWires },
	{ {ACTION_USE, OBJECT_MCCOY, 11, 0, 0}, &Room::tug2UseMccoyOnBomb },
	{ {ACTION_USE, OBJECT_REDSHIRT, 12, 0, 0}, &Room::tug2UseRedshirtOnWires },
	{ {ACTION_FINISHED_WALKING, 6, 0, 0, 0}, &Room::tug2RedshirtReachedWires },
	{ {ACTION_FINISHED_ANIMATION, 7, 0, 0, 0}, &Room::tug2RedshirtDefusedBomb },
	{ {ACTION_FINISHED_WALKING, 8, 0, 0, 0}, &Room::tug2RedshirtReturnedToPosition },
	{ {ACTION_USE, OBJECT_KIRK, 12, 0, 0}, &Room::tug2UseKirkOnWires },
	{ {ACTION_FINISHED_WALKING, 5, 0, 0, 0}, &Room::tug2KirkReachedWires },
	{ {ACTION_USE, OBJECT_SPOCK, 12, 0, 0}, &Room::tug2UseSpockOnWires },
	{ {ACTION_FINISHED_WALKING, 9, 0, 0, 0}, &Room::tug2SpockReachedWires },
	{ {ACTION_FINISHED_WALKING, 11, 0, 0, 0}, &Room::tug2SpockReturnedToPosition },
	{ {ACTION_GET, 12, 0, 0, 0}, &Room::tug2GetWires },
	{ {ACTION_FINISHED_WALKING, 28, 0, 0, 0}, &Room::tug2KirkReachedWiresToGet },
	{ {ACTION_FINISHED_ANIMATION, 29, 0, 0, 0}, &Room::tug2KirkGotWires },
	{ {ACTION_USE, OBJECT_KIRK, 0x20, 0, 0}, &Room::tug2UseKirkOnButton },
	{ {ACTION_FINISHED_WALKING, 14, 0, 0, 0}, &Room::tug2KirkReachedButton },
	{ {ACTION_USE, OBJECT_SPOCK, 0x20, 0, 0}, &Room::tug2UseSpockOnButton },
	{ {ACTION_FINISHED_WALKING, 16, 0, 0, 0}, &Room::tug2SpockReachedButton },
	{ {ACTION_USE, OBJECT_MCCOY, 0x20, 0, 0}, &Room::tug2UseMccoyOnButton },
	{ {ACTION_FINISHED_WALKING, 18, 0, 0, 0}, &Room::tug2MccoyReachedButton },
	{ {ACTION_USE, OBJECT_REDSHIRT, 0x20, 0, 0}, &Room::tug2UseRedshirtOnButton },
	{ {ACTION_FINISHED_WALKING, 20, 0, 0, 0}, &Room::tug2RedshirtReachedButton },
	{ {ACTION_FINISHED_ANIMATION, 15, 0, 0, 0}, &Room::tug2TurnedOffForceField },
	{ {ACTION_FINISHED_ANIMATION, 17, 0, 0, 0}, &Room::tug2TurnedOffForceField },
	{ {ACTION_FINISHED_ANIMATION, 19, 0, 0, 0}, &Room::tug2TurnedOffForceField },
	{ {ACTION_FINISHED_ANIMATION, 21, 0, 0, 0}, &Room::tug2TurnedOffForceField },
	{ {ACTION_FINISHED_ANIMATION, 1, 0, 0, 0}, &Room::tug2PrisonersDead },
	{ {ACTION_FINISHED_ANIMATION, 2, 0, 0, 0}, &Room::tug2PrisonersReleased },
	{ {ACTION_USE, OBJECT_IPHASERS, 10, 0, 0}, &Room::tug2UsePhaserOnBrig },
	{ {ACTION_USE, OBJECT_IPHASERK, 10, 0, 0}, &Room::tug2UsePhaserOnBrig },
	{ {ACTION_USE, OBJECT_IPHASERS, 8, 0, 0}, &Room::tug2UseStunPhaserOnGuard1 },
	{ {ACTION_FINISHED_ANIMATION, 22, 0, 0, 0}, &Room::tug2KirkShootGuard1 },
	{ {ACTION_USE, OBJECT_IPHASERS, 9, 0, 0}, &Room::tug2UseStunPhaserOnGuard2 },
	{ {ACTION_FINISHED_ANIMATION, 23, 0, 0, 0}, &Room::tug2KirkShootGuard2 },
	{ {ACTION_USE, OBJECT_IPHASERK, 8, 0, 0}, &Room::tug2UseKillPhaserOnGuard1 },
	{ {ACTION_FINISHED_ANIMATION, 24, 0, 0, 0}, &Room::tug2KirkKillGuard1 },
	{ {ACTION_USE, OBJECT_IPHASERK, 9, 0, 0}, &Room::tug2UseKillPhaserOnGuard2 },
	{ {ACTION_FINISHED_ANIMATION, 25, 0, 0, 0}, &Room::tug2KirkKillGuard2 },

	{ {ACTION_USE, OBJECT_IPHASERS, OBJECT_IPWE, 0, 0}, &Room::tug2UsePhaserOnWelder },
	{ {ACTION_USE, OBJECT_IPHASERK, OBJECT_IPWE, 0, 0}, &Room::tug2UsePhaserOnWelder },
	{ {ACTION_USE, OBJECT_IPWF, OBJECT_IWIRSCRP, 0, 0}, &Room::tug2UseWelderOnWireScraps },
	{ {ACTION_USE, OBJECT_IPWF, OBJECT_IJNKMETL, 0, 0}, &Room::tug2UseWelderOnMetalScraps },
	{ {ACTION_USE, OBJECT_ICOMBBIT, OBJECT_IRT, 0, 0}, &Room::tug2UseCombBitOnTransmogrifier },

	{ {ACTION_FINISHED_ANIMATION, 26, 0, 0, 0}, &Room::tug2ShotByElasi },
	{ {ACTION_FINISHED_ANIMATION, 27, 0, 0, 0}, &Room::tug2ShotByElasi },
	{ {ACTION_WALK, 0x21, 0, 0, 0}, &Room::tug2WalkToDoor },
	{ {ACTION_LOOK, 0x21, 0, 0, 0}, &Room::tug2LookAtDoor },
	{ {ACTION_LOOK, OBJECT_KIRK, 0, 0, 0}, &Room::tug2LookAtKirk },
	{ {ACTION_TALK, OBJECT_KIRK, 0, 0, 0}, &Room::tug2TalkToKirk },
	{ {ACTION_TALK, OBJECT_MCCOY, 0, 0, 0}, &Room::tug2TalkToMccoy },
	{ {ACTION_TALK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::tug2TalkToRedshirt },
	{ {ACTION_TALK, OBJECT_SPOCK, 0, 0, 0}, &Room::tug2TalkToSpock },
	{ {ACTION_USE, OBJECT_ICOMM, 0, 0, 0}, &Room::tug2UseCommunicator },
	{ {ACTION_TIMER_EXPIRED, 0, 0, 0, 0}, &Room::tug2Timer0Expired },
	{ {ACTION_TIMER_EXPIRED, 2, 0, 0, 0}, &Room::tug2Timer2Expired },
	{ {ACTION_USE, OBJECT_ISTRICOR, 11, 0, 0}, &Room::tug2UseSTricorderOnBomb },
	{ {ACTION_USE, OBJECT_IMTRICOR, 8, 0, 0}, &Room::tug2UseMTricorderOnGuard1 },
	{ {ACTION_USE, OBJECT_IMTRICOR, 9, 0, 0}, &Room::tug2UseMTricorderOnGuard2 },
	{ {ACTION_TALK, 8, 0, 0, 0}, &Room::tug2TalkToGuard1 },
	{ {ACTION_TALK, 9, 0, 0, 0}, &Room::tug2TalkToGuard2 },
	{ {ACTION_USE, OBJECT_IMEDKIT, 11, 0, 0}, &Room::tug2UseMedkitOnBomb },
	{ {ACTION_USE, OBJECT_IMEDKIT, 8, 0, 0}, &Room::tug2UseMedkitOnGuard1 },
	{ {ACTION_USE, OBJECT_IMEDKIT, 9, 0, 0}, &Room::tug2UseMedkitOnGuard2 },
	{ {ACTION_LOOK, 10, 0, 0, 0}, &Room::tug2LookAnywhere },
	{ {ACTION_LOOK, 0x22, 0, 0, 0}, &Room::tug2LookAnywhere },
	{ {ACTION_LOOK, 0xff, 0, 0, 0}, &Room::tug2LookAnywhere },
	{ {ACTION_TALK, 10, 0, 0, 0}, &Room::tug2TalkToBrig },
	{ {ACTION_USE, OBJECT_IMTRICOR, 10, 0, 0}, &Room::tug2UseMTricorderOnBrig },
	{ {ACTION_USE, OBJECT_IMTRICOR, 0x22, 0, 0}, &Room::tug2UseMTricorderOnOpenBrig },
	{ {ACTION_USE, OBJECT_IPHASERS, 0xff, 0, 0}, &Room::tug2UsePhaserAnywhere },
	{ {ACTION_USE, OBJECT_IPHASERK, 0xff, 0, 0}, &Room::tug2UsePhaserAnywhere },
	{ {ACTION_LIST_END, 0, 0, 0, 0}, nullptr }
};

enum tug2TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_CHRISTENSEN, TX_SPEAKER_ELASI_GUARD,
	TX_SPEAKER_MASADA_CREWMAN,
	TX_TUG2_001, TX_TUG2_002, TX_TUG2_003, TX_TUG2_004, TX_TUG2_005,
	TX_TUG2_006, TX_TUG2_007, TX_TUG2_008, TX_TUG2_009, TX_TUG2_010,
	TX_TUG2_011, TX_TUG2_012, TX_TUG2_013, TX_TUG2_014, TX_TUG2_015,
	TX_TUG2_016, TX_TUG2_017, TX_TUG2_018, TX_TUG2_019, TX_TUG2_020,
	TX_TUG2_021, TX_TUG2_022, TX_TUG2_023, TX_TUG2_024, TX_TUG2_025,
	TX_TUG2_026, TX_TUG2_027, TX_TUG2_028, TX_TUG2_029, TX_TUG2_030,
	TX_TUG2_112, TX_TUG1_011, TX_TUG2J000, TX_TUG2J001, TX_TUG2J002,
	TX_TUG2J003, TX_TUG2L002, TX_TUG2L004, TX_TUG2L007, TX_TUG2L011,
	TX_TUG2L085, TX_TUG2L086, TX_TUG2N000, TX_TUG2N001, TX_TUG2N002,
	TX_TUG2N003, TX_TUG2N004, TX_TUG2N005, TX_TUG2N006, TX_TUG2N007,
	TX_TUG2N009, TX_TUG2N010, TX_TUG2N011, TX_TUG2N012, TX_TUG2N013,
	TX_TUG2N014, TX_TUG2N015, TX_TUG2N016, TX_TUG2N017, TX_TUG2N018,
	TX_TUG2N019, TX_TUG2N020, TX_TUG2N021, TX_TUG2N022, TX_TUG2N023,
	TX_TUG2N024, TX_TUG2N025, TX_TUG2N026, TX_TUG2N037, TX_TUG2N106,
	TX_TUG2C001
};

extern const RoomTextOffsets tug2TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 14607, 13718, 15480, 15045, 13730, 13892, 14308, 13730, 14613 },
	{ TX_SPEAKER_MCCOY, 14618, 13729, 15491, 15056, 13741, 13905, 14319, 13741, 14624 },
	{ TX_SPEAKER_SPOCK, 14628, 13739, 15501, 15066, 13751, 13915, 14329, 13751, 14634 },
	{ TX_SPEAKER_CHRISTENSEN, 14638, 13749, 15511, 15076, 13761, 13925, 14339, 13761, 14644 },
	{ TX_SPEAKER_ELASI_GUARD, 14654, 13765, 15527, 15092, 13777, 13941, 14355, 13777, 14660 },
	{ TX_SPEAKER_MASADA_CREWMAN, 14666, 13777, 15539, 15104, 13789, 13953, 14367, 13789, 14672 },
	{ TX_TUG2_001, 7024, 6580, 7363, 7220, 6597, 6536, 6710, 6597, 7014 },
	{ TX_TUG2_002, 7078, 6619, 7420, 7279, 6636, 6580, 6752, 6636, 7068 },
	{ TX_TUG2_003, 3102, 2744, 3195, 3203, 2747, 2827, 2853, 2747, 3092 },
	{ TX_TUG2_004, 10763, 10223, 11226, 11042, 10240, 10220, 10438, 10240, 10753 },
	{ TX_TUG2_005, 7536, 7041, 7953, 7808, 7058, 7031, 7210, 7058, 7526 },
	{ TX_TUG2_006, 5335, 4771, 5603, 5531, 4775, 4954, 5056, 4775, 5325 },
	{ TX_TUG2_007, 2395, 2112, 2415, 2443, 2115, 2162, 2162, 2115, 2385 },
	{ TX_TUG2_008, 3462, 3050, 3593, 3570, 3054, 3140, 3197, 3054, 3452 },
	{ TX_TUG2_009, 2535, 2237, 2579, 2591, 2240, 2295, 2311, 2240, 2525 },
	{ TX_TUG2_010, 7256, 6786, 7629, 7487, 6803, 6758, 6931, 6803, 7246 },
	{ TX_TUG2_011, 2654, 2341, 2707, 2723, 2344, 2412, 2424, 2344, 2644 },
	{ TX_TUG2_012, 11592, 11004, 12172, 11889, 11020, 11272, 11557, 11020, 11598 },
	{ TX_TUG2_013, 11832, 11213, 12435, 12140, 11227, 11272, 11557, 11227, 11838 },
	{ TX_TUG2_014, 12010, 11361, 12623, 12330, 11375, 11501, 11798, 11375, 12016 },
	//{ TX_TUG2_014, 12109, 0, 0, 0 },	// ignore duplicate text
	{ TX_TUG2_015, 14235, 13227, 15073, 14655, 13241, 13547, 13946, 13241, 14241 },
	//{ TX_TUG2_015, 14058, 0, 0, 0 },	// ignore duplicate text
	{ TX_TUG2_016, 11045, 10490, 11532, 11315, 10507, 10478, 10973, 10507, 11035 },
	{ TX_TUG2_017, 11918, 11284, 12524, 12235, 11298, 11352, 11631, 11298, 11924 },
	{ TX_TUG2_018, 7168, 6713, 7524, 7390, 6730, 6676, 6841, 6730, 7158 },
	{ TX_TUG2_019, 13734, 12933, 14528, 14123, 12947, 13056, 13447, 12947, 13740 },
	{ TX_TUG2_020, 7601, 7091, 8030, 7874, 7108, 7082, 7272, 7108, 7591 },
	{ TX_TUG2_021, 6648, 6023, 6966, 6840, 6026, 6200, 6358, 6026, 6638 },
	{ TX_TUG2_022, 13822, 13006, 14629, 14222, 13020, 13140, 13533, 13020, 13828 },
	{ TX_TUG2_023, 3162, 2789, 3264, 3274, 2793, 2883, 2907, 2793, 3152 },
	{ TX_TUG2_024, 3370, 2973, 3485, 3486, 2977, 3071, 3104, 2977, 3360 },
	{ TX_TUG2_025, 14359, 13500, 15204, 14777, 13514, 13654, 14062, 13514, 14365 },
	//{ TX_TUG2_025, 14447, 0, 0, 0 },	// ignore duplicate text
	{ TX_TUG2_026, 13455, 12684, 14192, 13815, 12698, 12778, 13141, 12698, 13461 },
	{ TX_TUG2_027, 13594, 12808, 14358, 13967, 12822, 12915, 13292, 12822, 13600 },
	{ TX_TUG2_028, 5097, 4548, 5348, 5266, 4552, 4705, 4816, 4552, 5087 },
	{ TX_TUG2_029, 4791, 4273, 4960, 4913, 4277, 4376, 4458, 4277, 4781 },
	{ TX_TUG2_030, 4909, 4375, 5107, 5058, 4380, 4514, 4591, 4380, 4899 },
	{ TX_TUG2_112, 6475, 5865, 6763, 6658, 5869, 6034, 6171, 5869, 6465 },
	{ TX_TUG1_011, 7694, 7169, 8132, 7976, 7186, 7169, 7359, 7186, 7684 },
	{ TX_TUG2J000, 14729, 13825, 15596, 15168, 13837, 14002, 14421, 13837, 14735 },
	{ TX_TUG2J001, 14761, 13842, 15630, 15204, 13854, 14025, 14442, 13854, 14767 },
	{ TX_TUG2J002, 14803, 13869, 15683, 15246, 13881, 14052, 14480, 13881, 14809 },
	{ TX_TUG2J003, 7422, 6942, 7834, 7695, 6959, 6933, 7106, 6959, 7412 },
	{ TX_TUG2L002, 2177, 1924, 2169, 2215, 1927, 1964, 1947, 1927, 2167 },
	{ TX_TUG2L004, 7332, 6847, 7713, 7566, 6864, 6819, 7000, 6864, 7322 },
	{ TX_TUG2L007, 2762, 2434, 2839, 2840, 2437, 2514, 2541, 2437, 2752 },
	{ TX_TUG2L011, 2937, 2594, 3023, 3033, 2597, 2672, 2696, 2597, 2927 },
	{ TX_TUG2L085, 993, 967, 993, 993, 967, 967, 967, 967, 983 },
	{ TX_TUG2L086, 11680, 11076, 12263, 11986, 11092, 11133, 11400, 11092, 11686 },
	//{ TX_TUG2L086, 11440, 0, 0, 0 },	// ignore duplicate text
	{ TX_TUG2N000, 1579, 1461, 1559, 1580, 1463, 1464, 1472, 1463, 1569 },
	{ TX_TUG2N001, 1661, 1528, 1655, 1671, 1530, 1540, 1553, 1530, 1651 },
	{ TX_TUG2N002, 14681, 13792, 15554, 15124, 13804, 13973, 14394, 13804, 14687 },
	{ TX_TUG2N003, 6934, 6505, 7266, 7129, 6522, 6459, 6628, 6522, 6924 },
	{ TX_TUG2N004, 1288, 1202, 1291, 1292, 1202, 1206, 1205, 1202, 1278 },
	{ TX_TUG2N005, 1165, 1109, 1174, 1169, 1109, 1113, 1118, 1109, 1155 },
	{ TX_TUG2N006, 6266, 5687, 6562, 6457, 5691, 5865, 6000, 5691, 6256 },
	{ TX_TUG2N007, 1230, 1159, 1233, 1231, 1159, 1160, 1162, 1159, 1220 },
	{ TX_TUG2N009, 6329, 5735, 6619, 6534, 5739, 5927, 6042, 5739, 6319 },
	{ TX_TUG2N010, 2244, 1976, 2256, 2287, 1979, 2021, 2019, 1979, 2234 },
	{ TX_TUG2N011, 1059, 1018, 1064, 1051, 1018, 1010, 1023, 1018, 1049 },
	{ TX_TUG2N012, 6858, 6444, 7187, 7056, 6461, 6401, 6564, 6461, 6848 },
	{ TX_TUG2N013, 1723, 1575, 1718, 1738, 1577, 1592, 1601, 1577, 1713 },
	{ TX_TUG2N014, 1984, 1776, 1978, 2013, 1577, 1592, 1601, 1577, 1974 },
	{ TX_TUG2N015, 1779, 1616, 1773, 1796, 1618, 1635, 1641, 1618, 1769 },
	{ TX_TUG2N016, 2040, 1817, 2033, 2071, 1618, 1635, 1641, 1618, 2030 },
	{ TX_TUG2N017, 12453, 11759, 13087, 12755, 11773, 11811, 12127, 11773, 12459 },
	{ TX_TUG2N018, 12832, 12107, 13497, 13152, 12121, 12175, 12507, 12121, 12838 },
	{ TX_TUG2N019, 12246, 11567, 12871, 12536, 11581, 11608, 11926, 11581, 12252 },
	{ TX_TUG2N020, 12644, 11935, 13294, 12952, 11949, 11992, 12319, 11949, 12650 },
	{ TX_TUG2N021, 13004, 12264, 13691, 13330, 12278, 12341, 12686, 12278, 13010 },
	{ TX_TUG2N022, 13177, 12436, 13895, 13518, 12450, 12513, 12875, 12450, 13183 },
	{ TX_TUG2N023, 13339, 12583, 14074, 13699, 12597, 12678, 13039, 12597, 13345 },
	{ TX_TUG2N024, 1370, 1269, 1367, 1373, 1270, 1272, 1266, 1270, 1360 },
	{ TX_TUG2N025, 14846, 13897, 15725, 15306, 13909, 14097, 14507, 13909, 14852 },
	{ TX_TUG2N026, 4604, 4101, 4749, 4728, 4105, 4207, 4263, 4105, 4594 },
	{ TX_TUG2N037, 1922, 1729, 1915, 1946, 1530, 1540, 1553, 1530, 1912 },
	{ TX_TUG2N106, 1835, 1657, 1828, 1856, 1659, 1680, 1681, 1659, 1825 },
	//{ TX_TUG2N106, 2096, 0, 0, 0 },	// ignore duplicate text
	{          -1, 0,    0, 0, 0, 0, 0, 0, 0, 0 }
};

extern const RoomText tug2Texts[] = {
	{ TX_TUG2C001, Common::EN_ANY, "Snip...snip...snip." },
	{ -1, Common::UNK_LANG, "" }
};

void Room::tug2Tick1() {
	playVoc("TUG2LOOP");

	if (!_awayMission->tug.haveBomb)
		loadActorAnim2(OBJECT_BOMB, "t2bomb", -3, 1, 0);
	if (!_awayMission->tug.gotWires)
		loadActorAnim2(OBJECT_WIRE, "t2wire", 0, 1, 0);

	if (!_awayMission->tug.brigForceFieldDown)
		loadActorAnim2(OBJECT_BRIG, "fld01", 0, 0, 0);
	else
		loadActorAnim2(OBJECT_BRIG, "fld10d", 0, 0, 0);
	if (_awayMission->tug.field35 == 6)
		loadActorAnim2(OBJECT_BRIG, "zapdon", 0, 0, 0);

	// Security guy 1
	if (_awayMission->tug.guard1Status == 8)
		loadActorAnim2(OBJECT_GUARD_1, "p5tied", 0xe0, 0xb1, 0);
	else if (_awayMission->tug.guard1Status == 1 && _awayMission->tug.brigForceFieldDown) {
		loadActorAnim2(OBJECT_GUARD_1, "p5tied", 0xe0, 0xb1, 0);
		_awayMission->tug.guard1Status = 8;
	}
	if (_awayMission->tug.guard1Status == 1 && !_awayMission->tug.brigForceFieldDown) {
		_awayMission->tug.guard1Status = 0;
		_awayMission->tug.crewmanKilled[OBJECT_KIRK] = false;
		_awayMission->tug.crewmanKilled[OBJECT_SPOCK] = false;
		_awayMission->tug.crewmanKilled[OBJECT_MCCOY] = false;
		if (!_awayMission->redshirtDead)
			_awayMission->tug.crewmanKilled[OBJECT_REDSHIRT] = false;
		_awayMission->tug.field5b = 0;
	}
	if (_awayMission->tug.guard1Status == 0)
		loadActorAnim2(OBJECT_GUARD_1, "p5redy", 0xe0, 0xb1, 0);
	if (_awayMission->tug.guard1Status == 2)
		loadActorAnim2(OBJECT_GUARD_1, "p5hat", 0xe0, 0xb1, 0);

	// Security guy 2
	if (_awayMission->tug.guard2Status == 8)
		loadActorAnim2(OBJECT_GUARD_2, "p6tied", 0xfb, 0xc5, 0);
	else if (_awayMission->tug.guard2Status == 1 && _awayMission->tug.brigForceFieldDown) {
		loadActorAnim2(OBJECT_GUARD_2, "p6tied", 0xfb, 0xc5, 0);
		_awayMission->tug.guard2Status = 8;
	}
	if (_awayMission->tug.guard2Status == 1 && !_awayMission->tug.brigForceFieldDown) {
		_awayMission->tug.guard2Status = 0;
		_awayMission->tug.crewmanKilled[OBJECT_KIRK] = false;
		_awayMission->tug.crewmanKilled[OBJECT_SPOCK] = false;
		_awayMission->tug.crewmanKilled[OBJECT_MCCOY] = false;
		if (!_awayMission->redshirtDead)
			_awayMission->tug.crewmanKilled[OBJECT_REDSHIRT] = false;
		_awayMission->tug.field5b = 0;
	}
	if (_awayMission->tug.guard2Status == 0)
		loadActorAnim2(OBJECT_GUARD_2, "p6draw", 0xfb, 0xc5, 0);
	if (_awayMission->tug.guard2Status == 2)
		loadActorAnim2(OBJECT_GUARD_2, "p5hat", 0xfb, 0xc5, 0); // There is no "p6hat"


	if (_awayMission->tug.field4e == 0)
		loadActorAnim2(OBJECT_13, "wallht", 0, 0, 0);

	if (_awayMission->tug.field5b == 0) {
		_awayMission->disableWalking = true;
		playMidiMusicTracks(MIDITRACK_32);
	}

	_awayMission->tug.field5b = 1;
	_awayMission->tug.kirkPhaserDrawn = 0;
}

void Room::tug2Tick60() {
	if (!_awayMission->tug.guard1Status || !_awayMission->tug.guard2Status) {
		_awayMission->timers[0] = 60;
		showText(TX_SPEAKER_ELASI_GUARD, TX_TUG2L085);
		tug2ElasiReadyPhaser();
	}
}

void Room::tug2LookAtButton() {
	showDescription(TX_TUG2N011);
}

void Room::tug2LookAtMccoy() {
	showDescription(TX_TUG2N005);
}

void Room::tug2LookAtSpock() {
	showDescription(TX_TUG2N007);
}

void Room::tug2LookAtRedshirt() {
	showDescription(TX_TUG2N004);
}

void Room::tug2GetBomb() {
	if (_awayMission->tug.brigForceFieldDown) {
		_awayMission->disableInput = true;
		_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_N;
		walkCrewman(OBJECT_KIRK, 0xc9, 0xa0, 12);
	} else
		showDescription(TX_TUG2N024);
}

void Room::tug2KirkReachedBomb() {
	loadActorAnim2(OBJECT_KIRK, "kusehn", -1, -1, 13);
}

void Room::tug2KirkGotBomb() {
	loadActorStandAnim(OBJECT_BOMB);
	giveItem(OBJECT_IBOMB);
	_awayMission->tug.haveBomb = true;
	_awayMission->disableInput = false;
}

void Room::tug2LookAtBomb() {
	showDescription(TX_TUG2N000);
}

void Room::tug2LookAtGuard1() {
	if (_awayMission->tug.guard1Status == 0)
		showDescription(TX_TUG2N001);
	else if (_awayMission->tug.guard1Status == 1)
		showDescription(TX_TUG2N013);
	else if (_awayMission->tug.guard1Status == 8)
		showDescription(TX_TUG2N015);
	else if (_awayMission->tug.guard1Status == 2)
		showDescription(TX_TUG2N106);
}

void Room::tug2LookAtGuard2() {
	if (_awayMission->tug.guard2Status == 0)
		showDescription(TX_TUG2N037);
	else if (_awayMission->tug.guard2Status == 1)
		showDescription(TX_TUG2N014);
	else if (_awayMission->tug.guard2Status == 8)
		showDescription(TX_TUG2N016);
	else if (_awayMission->tug.guard2Status == 2)
		showDescription(TX_TUG2N106);
}

void Room::tug2LookAtWires() {
	showDescription(TX_TUG2N010);
}

void Room::tug2UseSTricorderOnButton() {
	if (_awayMission->tug.field35 != 0)
		return;
	spockScan(DIR_E, TX_SPEAKER_SPOCK, TX_TUG2_007, true);
}

void Room::tug2UseMccoyOnWires() {
	if (_awayMission->tug.field35 != 0)
		return;
	showText(TX_SPEAKER_MCCOY, TX_TUG2_009);
}

void Room::tug2UseMccoyOnBomb() {
	showText(TX_SPEAKER_MCCOY, TX_TUG2_011);
}

void Room::tug2UseRedshirtOnWires() {
	if (_awayMission->disableWalking || _awayMission->tug.field35 != 0)
		return;
	showText(TX_SPEAKER_CHRISTENSEN, TX_TUG2L007);
	_awayMission->crewDirectionsAfterWalk[OBJECT_REDSHIRT] = DIR_N;
	_awayMission->disableInput = true;
	walkCrewman(OBJECT_REDSHIRT, 0xc9, 0xa0, 6);
}

void Room::tug2RedshirtReachedWires() {
	loadActorAnim2(OBJECT_REDSHIRT, "rusemn", -1, -1, 7);
}

void Room::tug2RedshirtDefusedBomb() {
	showDescription(TX_TUG2C001);
	showText(TX_SPEAKER_CHRISTENSEN, TX_TUG2L011);
	_awayMission->tug.field35 = 4;
	_awayMission->crewDirectionsAfterWalk[OBJECT_REDSHIRT] = DIR_E;
	walkCrewman(OBJECT_REDSHIRT, 0x44, 0xc2, 8);
}

void Room::tug2RedshirtReturnedToPosition() {
	_awayMission->disableInput = false;
	_awayMission->tug.missionScore += 2;
}

void Room::tug2UseKirkOnWires() {
	if (_awayMission->disableWalking || _awayMission->tug.field35 != 0)
		return;
	_awayMission->disableInput = true;
	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_E;
	walkCrewman(OBJECT_KIRK, 0xc9, 0xa0, 5);
}

void Room::tug2KirkReachedWires() {
	showText(TX_SPEAKER_KIRK,  TX_TUG2_003);
	showText(TX_SPEAKER_SPOCK, TX_TUG2_023);

	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_E;
	walkCrewman(OBJECT_KIRK, 0x66, 0xb8, 0);
	_awayMission->disableInput = false;
	tug2UseSpockOnWires();
}

void Room::tug2UseSpockOnWires() {
	if (_awayMission->disableWalking || _awayMission->tug.field35 != 0)
		return;
	_awayMission->disableInput = true;
	_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_E;
	walkCrewman(OBJECT_SPOCK, 0xc9, 0xa0, 9);
}

void Room::tug2SpockReachedWires() {
	loadActorAnim2(OBJECT_SPOCK, "suseme", -1, -1, 0);
	playVoc("WIRESNIP");
	showText(TX_SPEAKER_SPOCK, TX_TUG2_024);
	_awayMission->tug.field35 = 4;

	if (_awayMission->tug.spockExaminedTransporter)
		showText(TX_SPEAKER_SPOCK, TX_TUG2_008);

	_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_E;
	walkCrewman(OBJECT_SPOCK, 0x56, 0xa9, 11);
}

void Room::tug2SpockReturnedToPosition() {
	_awayMission->disableInput = false;
	_awayMission->tug.missionScore += 2;
}

void Room::tug2GetWires() {
	if (_awayMission->disableWalking)
		return;
	_awayMission->disableInput = true;
	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_E;
	walkCrewman(OBJECT_KIRK, 0xc9, 0xa0, 28);
}

void Room::tug2KirkReachedWiresToGet() {
	loadActorAnim2(OBJECT_KIRK, "kuseme", -1, -1, 29);
}

void Room::tug2KirkGotWires() {
	if (_awayMission->tug.field35 == 0) {
		playMidiMusicTracks(MIDITRACK_2);
		loadActorAnim2(OBJECT_BRIG, "zap", 0, 0, 1);
		playVoc("NM8EXPLO");
		loadActorAnim2(OBJECT_BOMB, "t2bbo", -3, 1, 0);
		playSoundEffectIndex(kSfxPhaser);
		_awayMission->tug.field35 = 6;
		loadActorStandAnim(OBJECT_WIRE);
		giveItem(OBJECT_IWIRING);
		_awayMission->tug.gotWires = true;
		_awayMission->disableInput = false;
		_awayMission->tug.missionScore += 3;
	} else {
		loadActorStandAnim(OBJECT_WIRE);
		giveItem(OBJECT_IWIRING);
		_awayMission->tug.gotWires = true;
		_awayMission->disableInput = false;
		_awayMission->tug.missionScore += 3;
	}
}

void Room::tug2UseKirkOnButton() {
	if (_awayMission->disableWalking)
		return;
	_awayMission->disableInput = true;
	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_E;
	walkCrewman(OBJECT_KIRK, 0xc9, 0xa0, 14);
}

void Room::tug2KirkReachedButton() {
	loadActorAnim2(OBJECT_KIRK, "kusehe", -1, -1, 15);
	playSoundEffectIndex(kSfxButton);
}

void Room::tug2UseSpockOnButton() {
	if (_awayMission->disableWalking)
		return;
	_awayMission->disableInput = true;
	_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_E;
	walkCrewman(OBJECT_SPOCK, 0xc9, 0xa0, 16);
}

void Room::tug2SpockReachedButton() {
	loadActorAnim2(OBJECT_SPOCK, "susehe", -1, -1, 17);
	playSoundEffectIndex(kSfxButton);
}

void Room::tug2UseMccoyOnButton() {
	if (_awayMission->disableWalking)
		return;
	_awayMission->disableInput = true;
	_awayMission->crewDirectionsAfterWalk[OBJECT_MCCOY] = DIR_E;
	walkCrewman(OBJECT_MCCOY, 0xc9, 0xa0, 18);
}

void Room::tug2MccoyReachedButton() {
	loadActorAnim2(OBJECT_MCCOY, "musehe", -1, -1, 19);
	playSoundEffectIndex(kSfxButton);
}

void Room::tug2UseRedshirtOnButton() {
	if (_awayMission->disableWalking)
		return;
	_awayMission->disableInput = true;
	_awayMission->crewDirectionsAfterWalk[OBJECT_REDSHIRT] = DIR_E;
	walkCrewman(OBJECT_REDSHIRT, 0xc9, 0xa0, 20);
}

void Room::tug2RedshirtReachedButton() {
	loadActorAnim2(OBJECT_REDSHIRT, "rusehe", -1, -1, 21);
	playSoundEffectIndex(kSfxButton);
}

void Room::tug2TurnedOffForceField() {
	_awayMission->disableInput = false;
	if (_awayMission->tug.field35 == 0) {
		playMidiMusicTracks(MIDITRACK_2);
		loadActorAnim2(OBJECT_BRIG, "zap", 0, 0, 1);
		playVoc("NM8EXPLO");
		loadActorAnim2(OBJECT_BOMB, "t2bbo", -3, 1, 0);
		// Unlike tug2KirkGotWires(), the original plays no phaser sound here
		_awayMission->tug.field35 = 6;
	} else if (_awayMission->tug.field35 == 4 && !_awayMission->tug.brigForceFieldDown) {
		_awayMission->tug.brigForceFieldDown = true;
		loadActorAnim2(OBJECT_BRIG, "fld10", 0, 0, 2);
		_awayMission->tug.missionScore += 6;
		playVoc("EFX15");
	}
}

void Room::tug2PrisonersDead() {
	loadActorAnim2(OBJECT_BRIG, "zapdon", 0, 0, 0);
	showDescription(TX_TUG2N026);
}

void Room::tug2PrisonersReleased() {
	loadActorAnim2(OBJECT_BRIG, "fld10d", 0, 0, 0);
	showText(TX_SPEAKER_MASADA_CREWMAN, TX_TUG2_029);
	showText(TX_SPEAKER_MASADA_CREWMAN, TX_TUG2_030);
	showText(TX_SPEAKER_MASADA_CREWMAN, TX_TUG2_028);
	_awayMission->tug.savedPrisoners = true;
}

void Room::tug2UsePhaserOnBrig() {
	if (_awayMission->tug.field35 == 0)
		showText(TX_SPEAKER_SPOCK, TX_TUG2_006);
	else if (_awayMission->tug.brigForceFieldDown) {
		// BUGFIX: this function had two implementations; one for firing on the brig, and
		// one for firing on the masada crewman (who replaces the brig object). The first
		// took priority, meaning the latter code never ran. That's fixed here.
		showText(TX_SPEAKER_MASADA_CREWMAN, TX_TUG2_025);
	}
}

void Room::tug2ElasiReadyPhaser() {
	if (_awayMission->tug.guard1Status == 0 && _awayMission->tug.field59 == 0) {
		loadActorAnim2(OBJECT_GUARD_1, "p5draw", -1, -1, 0);
		_awayMission->tug.field59++;
	}
}

void Room::tug2CheckEndFirefight() {
	if (_awayMission->tug.guard1Status == GUARDSTAT_UP || _awayMission->tug.guard2Status == GUARDSTAT_UP)
		return;
	_awayMission->disableWalking = false;
	if (_awayMission->tug.kirkPhaserDrawn)
		loadActorStandAnim(OBJECT_KIRK);
}

void Room::tug2UseStunPhaserOnGuard1() {
	if (_awayMission->tug.guard1Status != GUARDSTAT_UP)
		tug2KirkShootGuard1();
	else {
		tug2ElasiReadyPhaser();
		_awayMission->disableInput = true;
		if (_awayMission->tug.kirkPhaserDrawn)
			tug2KirkShootGuard1();
		else {
			loadActorAnim2(OBJECT_KIRK, "kdrawe", -1, -1, 22);
			_awayMission->tug.kirkPhaserDrawn = true;
		}
	}
}

void Room::tug2KirkShootGuard1() {
	if (_awayMission->tug.guard1Status == GUARDSTAT_UP) {
		loadActorAnim2(OBJECT_GUARD_1, "p5stun", -1, -1, 0);
		_awayMission->tug.guard1Status = GUARDSTAT_STUNNED;
		playSoundEffectIndex(kSfxPhaser);
		showBitmapFor5Ticks("t2beem03", 5);
		tug2CheckEndFirefight();
	}

	_awayMission->disableInput = false;
}

void Room::tug2UseStunPhaserOnGuard2() {
	if (_awayMission->tug.guard2Status != GUARDSTAT_UP)
		tug2KirkShootGuard2();
	else {
		tug2ElasiReadyPhaser();
		_awayMission->disableInput = true;
		if (_awayMission->tug.kirkPhaserDrawn)
			tug2KirkShootGuard2();
		else {
			loadActorAnim2(OBJECT_KIRK, "kdrawe", -1, -1, 23);
			_awayMission->tug.kirkPhaserDrawn = true;
		}
	}
}

void Room::tug2KirkShootGuard2() {
	if (_awayMission->tug.guard2Status == GUARDSTAT_UP) {
		loadActorAnim2(OBJECT_GUARD_2, "p6stun", -1, -1, 0);
		_awayMission->tug.guard2Status = GUARDSTAT_STUNNED;
		playSoundEffectIndex(kSfxPhaser);
		showBitmapFor5Ticks("t2beem02", 5);
		tug2CheckEndFirefight();
	}

	_awayMission->disableInput = false;
}

void Room::tug2UseKillPhaserOnGuard1() {
	if (_awayMission->tug.guard1Status != GUARDSTAT_UP)
		tug2KirkKillGuard1();
	else {
		tug2ElasiReadyPhaser();
		_awayMission->disableInput = true;
		if (_awayMission->tug.kirkPhaserDrawn)
			tug2KirkKillGuard1();
		else {
			loadActorAnim2(OBJECT_KIRK, "kdrawe", -1, -1, 24);
			_awayMission->tug.kirkPhaserDrawn = true;
			_awayMission->tug.brigElasiPhasersOnKill = true;
		}
	}
}

void Room::tug2KirkKillGuard1() {
	if (_awayMission->tug.guard1Status == GUARDSTAT_UP) {
		loadActorAnim2(OBJECT_GUARD_1, "p5kill", -1, -1, 0);
		_awayMission->tug.guard1Status = GUARDSTAT_DEAD;
		playSoundEffectIndex(kSfxPhaser);
		showBitmapFor5Ticks("t2beem05", 5);
		tug2CheckEndFirefight();
	}

	_awayMission->disableInput = false;
}

void Room::tug2UseKillPhaserOnGuard2() {
	if (_awayMission->tug.guard2Status != GUARDSTAT_UP)
		tug2KirkKillGuard2();
	else {
		tug2ElasiReadyPhaser();
		_awayMission->disableInput = true;
		if (_awayMission->tug.kirkPhaserDrawn)
			tug2KirkKillGuard2();
		else {
			loadActorAnim2(OBJECT_KIRK, "kdrawe", -1, -1, 25);
			_awayMission->tug.kirkPhaserDrawn = true;
			_awayMission->tug.brigElasiPhasersOnKill = true;
		}
	}
}

void Room::tug2KirkKillGuard2() {
	if (_awayMission->tug.guard2Status == GUARDSTAT_UP) {
		loadActorAnim2(OBJECT_GUARD_2, "p6kill", -1, -1, 0);
		_awayMission->tug.guard2Status = GUARDSTAT_DEAD;
		playSoundEffectIndex(kSfxPhaser);
		showBitmapFor5Ticks("t2beem04", 5);
		tug2CheckEndFirefight();
	}

	_awayMission->disableInput = false;
}

void Room::tug2UsePhaserOnWelder() {
	loseItem(OBJECT_IPWE);
	giveItem(OBJECT_IPWF);
	showDescription(TX_TUG2N006);

	// BUGFIX: this following line didn't exist, despite it existing in TUG1; meaning this
	// was supposed to give points, but it only did in a specific room.
	_awayMission->tug.missionScore += 3;
}

void Room::tug2UseWelderOnWireScraps() {
	showDescription(TX_TUG2N009);
	loseItem(OBJECT_IWIRSCRP);
}

void Room::tug2UseWelderOnMetalScraps() {
	showText(TX_SPEAKER_SPOCK, TX_TUG2_112);
	loseItem(OBJECT_IJNKMETL);
	giveItem(OBJECT_ICOMBBIT);
}

void Room::tug2UseCombBitOnTransmogrifier() {
	showText(TX_SPEAKER_SPOCK, TX_TUG2_021);
	loseItem(OBJECT_ICOMBBIT);
	loseItem(OBJECT_IRT);
	giveItem(OBJECT_IRTWB);
}

void Room::tug2ShotByElasi() {
	showDescription(TX_TUG2N025);
	showGameOverMenu();

	// Unused: additional textbox that says "***Game over man!***"
}

void Room::tug2WalkToDoor() {
	walkCrewman(OBJECT_KIRK, 0x0b, 0xab, 0);
}

void Room::tug2LookAtDoor() {
	showDescription(TX_TUG2N012);
}

void Room::tug2LookAtKirk() {
	showDescription(TX_TUG2N003);
}

void Room::tug2TalkToKirk() {
	if (_awayMission->tug.field35 == 6)
		showText(TX_SPEAKER_KIRK, TX_TUG2_001);
	else
		showText(TX_SPEAKER_KIRK, TX_TUG2_002);
}

void Room::tug2TalkToMccoy() {
	if (_awayMission->tug.field35 == 6)
		showText(TX_SPEAKER_MCCOY, TX_TUG2_018);
	else
		showText(TX_SPEAKER_MCCOY, TX_TUG2_010);
}

void Room::tug2TalkToRedshirt() {
	// BUGFIX: the original game has two implementations of this function; the first takes
	// precedence, however, it's just generic, unhelpful text. The second implementation
	// is more interesting, so that one is used instead.
	if (_awayMission->tug.field35 == 6)
		showText(TX_SPEAKER_CHRISTENSEN, TX_TUG2L004);
	else
		showText(TX_SPEAKER_CHRISTENSEN, TX_TUG2J003);

	// Original text: "I'm just a security officer, sir."
	//showText(TX_SPEAKER_CHRISTENSEN, TX_TUG2L002);
}

void Room::tug2TalkToSpock() {
	if (_awayMission->tug.field35 == 6)
		showText(TX_SPEAKER_SPOCK, TX_TUG2_005);
	else
		showText(TX_SPEAKER_SPOCK, TX_TUG2_020);
}

void Room::tug2UseCommunicator() {
	showText(TX_SPEAKER_SPOCK, TX_TUG1_011);
}

void Room::tug2DetermineElasiShooter() {
	if (_awayMission->tug.guard2Status == GUARDSTAT_UP)
		_roomVar.tug.shootingObject = OBJECT_GUARD_2;
	if (_awayMission->tug.guard1Status == GUARDSTAT_UP)
		_roomVar.tug.shootingObject = OBJECT_GUARD_1;
}

// Elasi shoots someone
void Room::tug2Timer0Expired() {
	if (_awayMission->tug.guard1Status != GUARDSTAT_UP && _awayMission->tug.guard2Status != GUARDSTAT_UP)
		return;

	_roomVar.tug.elasiPhaserOnKill = 0;
	if (_awayMission->tug.brigElasiPhasersOnKill)
		_roomVar.tug.elasiPhaserOnKill = 1;

	switch (_awayMission->tug.elasiTargetIndex) {
	case 0:
		_awayMission->tug.elasiTargetIndex++;
		if (_awayMission->tug.guard2Status != GUARDSTAT_UP) {
			// BUGFIX: reset the timer to allow guard 1 to continue if guard 2 is down
			_awayMission->timers[0] = 60;
			return;
		}

		// Guard misses and shoots wall
		loadActorAnim2(OBJECT_13, "wallht", 0, 0, 0);
		showBitmapFor5Ticks("t2beem00", 5);
		playSoundEffectIndex(kSfxPhaser);
		loadActorAnim2(OBJECT_GUARD_2, "p6fire", -1, -1, 0);
		_awayMission->tug.field4e = _roomVar.tug.elasiPhaserOnKill;
		_awayMission->timers[0] = 60;
		break;

	case 1:
		// Guard shoots redshirt
		_awayMission->tug.elasiTargetIndex++;
		if (!_awayMission->tug.crewmanKilled[OBJECT_REDSHIRT]) {
			tug2DetermineElasiShooter();
			_roomVar.tug.shootingTarget = OBJECT_REDSHIRT;
			_awayMission->timers[0] = 60;
			tug2GuardShootsCrewman();
		}
		break;

	case 2:
		// Guard shoots spock (or kirk)
		_awayMission->tug.elasiTargetIndex++;
		_roomVar.tug.shootKirkOverride = false;
		if (_awayMission->tug.brigElasiPhasersOnKill)
			_roomVar.tug.shootKirkOverride = true;
		tug2DetermineElasiShooter();
		if (_roomVar.tug.shootKirkOverride)
			_roomVar.tug.shootingTarget = OBJECT_KIRK;
		else
			_roomVar.tug.shootingTarget = OBJECT_SPOCK;
		_awayMission->timers[0] = 60;
		tug2GuardShootsCrewman();
		break;

	case 3:
		// Guard shoots Mccoy
		_awayMission->tug.elasiTargetIndex++;
		if (!_awayMission->tug.crewmanKilled[OBJECT_MCCOY]) {
			tug2DetermineElasiShooter();
			_roomVar.tug.shootingTarget = OBJECT_MCCOY;
			_awayMission->timers[0] = 80;
			tug2GuardShootsCrewman();
		}
		break;

	case 4:
		// Guard shoots kirk (or Spock)
		_awayMission->tug.elasiTargetIndex++;
		if (_roomVar.tug.shootKirkOverride) {
			tug2DetermineElasiShooter();
			_roomVar.tug.shootingTarget = OBJECT_SPOCK;
			_awayMission->timers[2] = 40;
			tug2GuardShootsCrewman();
		} else {
			tug2DetermineElasiShooter();
			_roomVar.tug.shootingTarget = OBJECT_KIRK;
			_awayMission->timers[2] = 40;
			tug2GuardShootsCrewman();
		}
		break;

	default:
		break;
	}
}

void Room::tug2GuardShootsCrewman() {
	const char *g1ShotsOnStun[] = {
		"t2beem15",
		"t2beem14",
		"t2beem17",
		"t2beem16"
	};
	const char *g1ShotsOnKill[] = {
		"t2beem19",
		"t2beem20",
		"t2beem21",
		"t2beem18",
	};
	const char *g2ShotsOnStun[] = { // BUG: these look wrong. But correct animations don't seem to exist.
		"t2beem07",
		"t2beem08",
		"t2beem09",
		"t2beem06"
	};
	const char *g2ShotsOnKill[] = {
		"t2beem11",
		"t2beem12",
		"t2beem13",
		"t2beem10",
	};

	const char *crewPrefixes = "ksmr";

	if (!_roomVar.tug.elasiPhaserOnKill)
		_awayMission->crewDirectionsAfterWalk[_roomVar.tug.shootingTarget] = DIR_W;

	Common::String anim;
	const char **bitmapArray;
	if (_roomVar.tug.elasiPhaserOnKill) {
		anim = "_Kille";
		if (_roomVar.tug.shootingObject == OBJECT_GUARD_1)
			bitmapArray = g1ShotsOnKill;
		else
			bitmapArray = g2ShotsOnKill;
	} else {
		anim = "_stune";
		if (_roomVar.tug.shootingObject == OBJECT_GUARD_1)
			bitmapArray = g1ShotsOnStun;
		else
			bitmapArray = g2ShotsOnStun;
	}
	anim.setChar(crewPrefixes[_roomVar.tug.shootingTarget], 0);

	if (_roomVar.tug.shootingTarget != OBJECT_KIRK)
		loadActorAnim2(_roomVar.tug.shootingTarget, anim, -1, -1, 0);
	else if (_roomVar.tug.elasiPhaserOnKill)
		loadActorAnim2(_roomVar.tug.shootingTarget, anim, -1, -1, 27);
	else
		loadActorAnim2(_roomVar.tug.shootingTarget, anim, -1, -1, 26);

	showBitmapFor5Ticks(bitmapArray[_roomVar.tug.shootingTarget], 5);
	playSoundEffectIndex(kSfxPhaser);

	if (_roomVar.tug.shootingObject == OBJECT_GUARD_1)
		loadActorAnim2(OBJECT_GUARD_1, "p5fire", -1, -1, 0);
	else
		loadActorAnim2(OBJECT_GUARD_2, "p6fire", -1, -1, 0);

	_awayMission->tug.crewmanKilled[_roomVar.tug.shootingTarget] = _roomVar.tug.elasiPhaserOnKill;

	if (!_roomVar.tug.elasiPhaserOnKill) {
		// BUG: crew goes down for 120 frames, but they start to get up before the fight
		// is over. Perhaps this number should be increased.
		_awayMission->crewDirectionsAfterWalk[_roomVar.tug.shootingTarget] = DIR_W;
		_awayMission->crewGetupTimers[_roomVar.tug.shootingTarget] = 120;
		_awayMission->crewDownBitset |= (1 << _roomVar.tug.shootingTarget);
	}
	if (_roomVar.tug.elasiPhaserOnKill && _roomVar.tug.shootingTarget == OBJECT_REDSHIRT)
		_awayMission->redshirtDead = true;
}

void Room::tug2Timer2Expired() {
	// NOTE: is this function redundant? There's another that shows the game over menu
	// when Kirk's death animation finishes. That one seems to take precedence.
	showGameOverMenu();
}

void Room::tug2UseSTricorderOnBomb() {
	spockScan(DIR_E, TX_SPEAKER_SPOCK, TX_TUG2_004, true);
}

void Room::tug2UseMTricorderOnGuard1() {
	if (_awayMission->tug.guard1Status == GUARDSTAT_STUNNED)
		mccoyScan(DIR_E, TX_SPEAKER_MCCOY, TX_TUG2_016, true);
}

void Room::tug2UseMTricorderOnGuard2() {
	// BUGFIX: original didn't play audio for this (despite being the same as above).
	if (_awayMission->tug.guard2Status == GUARDSTAT_STUNNED)
		mccoyScan(DIR_E, TX_SPEAKER_MCCOY, TX_TUG2_016, true);
}

void Room::tug2TalkToGuard1() {
	if (_awayMission->tug.guard1Status == GUARDSTAT_TIED)
		showText(TX_SPEAKER_ELASI_GUARD, TX_TUG2L086);
	else if (_awayMission->tug.guard1Status == GUARDSTAT_STUNNED)
		showText(TX_SPEAKER_MCCOY, TX_TUG2_012);
}

void Room::tug2TalkToGuard2() {
	if (_awayMission->tug.guard2Status == GUARDSTAT_TIED)
		showText(TX_SPEAKER_ELASI_GUARD, TX_TUG2L086);
	else if (_awayMission->tug.guard2Status == GUARDSTAT_STUNNED)
		showText(TX_SPEAKER_MCCOY, TX_TUG2_013);
}

void Room::tug2UseMedkitOnBomb() {
	showText(TX_SPEAKER_MCCOY, TX_TUG2_017);
}

void Room::tug2UseMedkitOnGuard1() {
	if (_awayMission->tug.guard1Status == GUARDSTAT_STUNNED)
		showText(TX_SPEAKER_MCCOY, TX_TUG2_014);
}

void Room::tug2UseMedkitOnGuard2() {
	if (_awayMission->tug.guard2Status == GUARDSTAT_STUNNED)
		showText(TX_SPEAKER_MCCOY, TX_TUG2_014);
}

void Room::tug2LookAnywhere() {
	if (_awayMission->tug.guard1Status == GUARDSTAT_DEAD && _awayMission->tug.guard2Status == GUARDSTAT_DEAD && _awayMission->tug.field35 == 6)
		showDescription(TX_TUG2N019);
	else if (_awayMission->tug.guard1Status == GUARDSTAT_DEAD && _awayMission->tug.guard2Status == GUARDSTAT_DEAD && !_awayMission->tug.brigForceFieldDown)
		showDescription(TX_TUG2N017);
	else if (_awayMission->tug.guard1Status == GUARDSTAT_STUNNED && _awayMission->tug.guard2Status == GUARDSTAT_STUNNED && _awayMission->tug.field35 == 6)
		showDescription(TX_TUG2N020);
	else if (_awayMission->tug.guard1Status == GUARDSTAT_STUNNED && _awayMission->tug.guard2Status == GUARDSTAT_STUNNED && !_awayMission->tug.brigForceFieldDown)
		showDescription(TX_TUG2N018);
	else if (_awayMission->tug.guard1Status == GUARDSTAT_STUNNED && _awayMission->tug.guard2Status == GUARDSTAT_STUNNED && _awayMission->tug.brigForceFieldDown)
		showDescription(TX_TUG2N021);
	else if (_awayMission->tug.guard1Status == GUARDSTAT_DEAD && _awayMission->tug.guard2Status == GUARDSTAT_DEAD && _awayMission->tug.brigForceFieldDown)
		showDescription(TX_TUG2N022);
	else
		showDescription(TX_TUG2N023);
}

void Room::tug2TalkToBrig() {
	// The "brig" can also be a masada crewman, when the force field is down, so there are
	// at least two cases here.

	if (_awayMission->tug.brigForceFieldDown && !_awayMission->tug.talkedToBrigCrewman) {
		loadActorAnim2(OBJECT_BRIG, "fld10d", 0, 0, 0);
		showText(TX_SPEAKER_MASADA_CREWMAN, TX_TUG2_026);
		_awayMission->tug.talkedToBrigCrewman = true;
	}

	if (_awayMission->tug.brigForceFieldDown && _awayMission->tug.talkedToBrigCrewman)
		showText(TX_SPEAKER_MASADA_CREWMAN, TX_TUG2_027);
	else if (!_awayMission->tug.brigForceFieldDown)
		mccoyScan(DIR_E, TX_SPEAKER_MCCOY, TX_TUG2_015, true);
}

void Room::tug2UseMTricorderOnBrig() {
	if (_awayMission->tug.field35 == 6)
		mccoyScan(DIR_E, TX_SPEAKER_MCCOY, TX_TUG2_019, true);
	else if (!_awayMission->tug.brigForceFieldDown)
		mccoyScan(DIR_E, TX_SPEAKER_MCCOY, TX_TUG2_015, true);
}

void Room::tug2UseMTricorderOnOpenBrig() {
	if (_awayMission->tug.brigForceFieldDown)
		mccoyScan(DIR_E, TX_SPEAKER_MCCOY, TX_TUG2_015, true);
}

void Room::tug2UsePhaserAnywhere() {
	// Stub function to suppress "you can't do that" messages.
}

}
