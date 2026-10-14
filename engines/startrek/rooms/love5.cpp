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

#define OBJECT_8 8
#define OBJECT_PREAX 9
#define OBJECT_MARCUS 10
#define OBJECT_CHEEVER 11

#define HOTSPOT_DEVICE 0x20
#define HOTSPOT_CONSOLE 0x21

namespace StarTrek {

extern const RoomAction love5ActionList[] = {
	{ {ACTION_TICK, 1, 0, 0, 0}, &Room::love5Tick1 },
	{ {ACTION_TOUCHED_WARP, 1, 0, 0, 0}, &Room::love5TouchedWarp1 },
	{ {ACTION_TIMER_EXPIRED, 4, 0, 0, 0}, &Room::love5Timer4Expired },
	{ {ACTION_USE, OBJECT_IPHASERS, 9, 0, 0}, &Room::love5UseStunPhaserOnPreax },
	// NOTE: nothing for OBJECT_IPHASERK. There's an unused audio file which might fit.
	{ {ACTION_LOOK, OBJECT_KIRK, 0, 0, 0}, &Room::love5LookAtKirk },
	{ {ACTION_LOOK, OBJECT_SPOCK, 0, 0, 0}, &Room::love5LookAtSpock },
	{ {ACTION_LOOK, OBJECT_MCCOY, 0, 0, 0}, &Room::love5LookAtMccoy },
	{ {ACTION_LOOK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::love5LookAtRedshirt },
	{ {ACTION_LOOK, 0xff, 0, 0, 0}, &Room::love5LookAnywhere },
	{ {ACTION_LOOK, 0x20, 0, 0, 0}, &Room::love5LookAtDevice },
	{ {ACTION_LOOK, 0x21, 0, 0, 0}, &Room::love5LookAtConsole },
	{ {ACTION_LOOK, 10, 0, 0, 0}, &Room::love5LookAtDrMarcus },
	{ {ACTION_LOOK, 11, 0, 0, 0}, &Room::love5LookAtDrCheever },
	{ {ACTION_LOOK, 9, 0, 0, 0}, &Room::love5LookAtPreax },
	{ {ACTION_TALK, 9, 0, 0, 0}, &Room::love5TalkToPreax },
	{ {ACTION_FINISHED_WALKING, 5, 0, 0, 0}, &Room::love5MccoyReachedSpockToCure },
	{ {ACTION_FINISHED_ANIMATION, 6, 0, 0, 0}, &Room::love5MccoyCuredSpock },
	{ {ACTION_FINISHED_WALKING, 6, 0, 0, 0}, &Room::love5CrewmanReachedBeamoutPosition },
	{ {ACTION_TALK, 10, 0, 0, 0}, &Room::love5TalkToDrMarcus },
	{ {ACTION_TALK, 11, 0, 0, 0}, &Room::love5TalkToDrCheever },
	{ {ACTION_TALK, OBJECT_KIRK, 0, 0, 0}, &Room::love5TalkToKirk },
	{ {ACTION_TALK, OBJECT_SPOCK, 0, 0, 0}, &Room::love5TalkToSpock },
	{ {ACTION_TALK, OBJECT_MCCOY, 0, 0, 0}, &Room::love5TalkToMccoy },
	{ {ACTION_TALK, OBJECT_REDSHIRT, 0, 0, 0}, &Room::love5TalkToRedshirt },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0xff, 0, 0}, &Room::love5UseSTricorderAnywhere },
	{ {ACTION_USE, OBJECT_ISTRICOR, 0x20, 0, 0}, &Room::love5UseSTricorderOnDevice },
	{ {ACTION_USE, OBJECT_IMTRICOR, 10, 0, 0}, &Room::love5UseMTricorderOnMarcusOrCheever },
	{ {ACTION_USE, OBJECT_IMTRICOR, 11, 0, 0}, &Room::love5UseMTricorderOnMarcusOrCheever },
	{ {ACTION_USE, OBJECT_IMTRICOR, 9, 0, 0}, &Room::love5UseMTricorderOnPreax },
	{ {ACTION_USE, OBJECT_IMEDKIT, 9, 0, 0}, &Room::love5UseMedkitOnPreax },
	{ {ACTION_USE, OBJECT_IH2O, 9, 0, 0}, &Room::love5UseWaterOnPreax },
	{ {ACTION_USE, OBJECT_ISAMPLE, 9, 0, 0}, &Room::love5UseCureSampleOnPreax },
	{ {ACTION_USE, OBJECT_ICURE, 9, 0, 0}, &Room::love5UseCureOnPreax },
	{ {ACTION_FINISHED_WALKING, 4, 0, 0, 0}, &Room::love5ReachedPreaxUsingCure },
	{ {ACTION_FINISHED_ANIMATION, 5, 0, 0, 0}, &Room::love5CuredPreax },
	{ {ACTION_USE, 0xff, 9, 0, 0}, &Room::love5UseAnythingOnPreax },
	{ {ACTION_USE, OBJECT_KIRK, 10, 0, 0}, &Room::love5UseKirkOnMarcusOrCheever },
	{ {ACTION_USE, OBJECT_KIRK, 11, 0, 0}, &Room::love5UseKirkOnMarcusOrCheever },
	{ {ACTION_FINISHED_WALKING,   1, 0, 0, 0}, &Room::love5KirkReachedCheever },
	{ {ACTION_FINISHED_ANIMATION, 2, 0, 0, 0}, &Room::love5KirkUntiedCheever },
	{ {ACTION_FINISHED_WALKING,   2, 0, 0, 0}, &Room::love5KirkReachedMarcus },
	{ {ACTION_FINISHED_ANIMATION, 3, 0, 0, 0}, &Room::love5KirkUntiedMarcus },
	{ {ACTION_FINISHED_ANIMATION, 4, 0, 0, 0}, &Room::love5MarcusStoodUp },

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

enum love5TextIds {
	TX_SPEAKER_KIRK, TX_SPEAKER_MCCOY, TX_SPEAKER_SPOCK, TX_SPEAKER_FERRIS, TX_SPEAKER_MARCUS,
	TX_SPEAKER_CHEEVER, TX_SPEAKER_PREAX,
	TX_LOV0_011, TX_LOV0_012, TX_LOV0_013, TX_LOV0_014, TX_LOV0_015,
	TX_LOV0_016, TX_LOV0_017, TX_LOV0_019, TX_LOV0_020, TX_LOV0_021,
	TX_LOV0_022, TX_LOV0_025, TX_LOV0_026, TX_LOV0_029, TX_LOV0_033,
	TX_LOV0_039, TX_LOV0_040, TX_LOV0_041, TX_LOV0_042, TX_LOV0_043,
	TX_LOV0_045, TX_LOV0_046, TX_LOV0_047, TX_LOV0_048, TX_LOV0_049,
	TX_LOV0_050, TX_LOV0_101, TX_LOV0_102, TX_LOV0_103, TX_LOV0_104,
	TX_LOV0_105, TX_LOV0_106, TX_LOV0_107, TX_LOV0_124, TX_LOV5_001,
	TX_LOV5_002, TX_LOV5_003, TX_LOV5_004, TX_LOV5_005, TX_LOV5_006,
	TX_LOV5_007, TX_LOV5_008, TX_LOV5_009, TX_LOV5_010, TX_LOV5_011,
	TX_LOV5_013, TX_LOV5_014, TX_LOV5_015, TX_LOV5_016, TX_LOV5_017,
	TX_LOV5_018, TX_LOV5_019, TX_LOV5_020, TX_LOV5_021, TX_LOV5_023,
	TX_LOV5_024, TX_LOV5_025, TX_LOV5_026, TX_LOV5_027, TX_LOV5_028,
	TX_LOV5_029, TX_LOV5_030, TX_LOV5_031, TX_LOV5_032, TX_LOV5_033,
	TX_LOV5_034, TX_LOV5_036, TX_LOV5_038, TX_LOV5_039, TX_LOV5_040,
	TX_LOV5_041, TX_LOV5_042, TX_LOV5_043, TX_LOV5_044, TX_LOV5_045,
	TX_LOV5_046, TX_LOV5_047, TX_LOV5_048, TX_LOV5_049, TX_LOV5_050,
	TX_LOV5_051, TX_LOV5_052, TX_LOV5_053, TX_LOV5_054, TX_LOV5_055,
	TX_LOV5_056, TX_LOV5_057, TX_LOV5_058, TX_LOV5_059, TX_LOV5_060,
	TX_LOV5_061, TX_FEA5_008, TX_LOV5N000, TX_LOV5N001, TX_LOV5N002,
	TX_LOV5N003, TX_LOV5N004, TX_LOV5N005, TX_LOV5N006, TX_LOV5N007,
	TX_LOV5N008, TX_LOV5N009, TX_LOV5N010, TX_LOV5N011, TX_LOV5N012,
	TX_LOV5N013, TX_LOV5N014, TX_LOV5N016, TX_LOV5N017, TX_LOV5N018,
	TX_LOV5N001_BOUND,
	/*TX_LOV5C001,*/ TX_DEM0N009, TX_G_024, TX_DIALOG_ERROR, TX_LOV5F001, TX_LOV5F002
};

extern const RoomTextOffsets love5TextOffsets[] = {
	{ TX_SPEAKER_KIRK, 8968, 9017, 9256, 9419, 9004, 9750, 9670 },
	{ TX_SPEAKER_MCCOY, 8979, 9028, 9267, 9430, 9015, 9763, 9681 },
	{ TX_SPEAKER_SPOCK, 8989, 9038, 9277, 9440, 9025, 9773, 9691 },
	{ TX_SPEAKER_FERRIS, 8999, 9048, 9287, 9450, 9035, 9783, 9701 },
	{ TX_SPEAKER_CHEEVER, 9027, 9076, 9315, 9478, 9063, 9811, 9729 },
	{ TX_SPEAKER_MARCUS, 9016, 9065, 9304, 9467, 9052, 9800, 9718 },
	{ TX_SPEAKER_PREAX, 9010, 9059, 9298, 9461, 9046, 9794, 9712 },
	{ TX_LOV0_011, 13010, 0, 13607, 13589, 12582, 13500, 13632 },
	{ TX_LOV0_012, 13137, 0, 13737, 13714, 12664, 13583, 13717 },
	{ TX_LOV0_013, 13433, 0, 14077, 14024, 12885, 13821, 13985 },
	{ TX_LOV0_014, 13197, 0, 13811, 13772, 12709, 13627, 13779 },
	{ TX_LOV0_015, 13301, 0, 13926, 13874, 12783, 13700, 13864 },
	{ TX_LOV0_016, 13104, 0, 13701, 13681, 12646, 13562, 13696 },
	{ TX_LOV0_017, 13567, 0, 14218, 14156, 12989, 13924, 14095 },
	{ TX_LOV0_019, 13487, 0, 14139, 14075, 12924, 13857, 14032 },
	{ TX_LOV0_020, 13044, 0, 13637, 13623, 12601, 13519, 13647 },
	{ TX_LOV0_021, 13355, 0, 13990, 13923, 12822, 13735, 13913 },
	{ TX_LOV0_022, 13245, 0, 13864, 13828, 12742, 13668, 13817 },
	{ TX_LOV0_025, 14493, 13410, 15262, 15124, 13397, 14420, 14611 },
	{ TX_LOV0_026, 14366, 13139, 15119, 14971, 13126, 14067, 14271 },
	{ TX_LOV0_029, 14246, 13049, 14967, 14843, 13035, 13969, 14149 },
	{ TX_LOV0_033, 14306, 13094, 15037, 14909, 13081, 14020, 14204 },
	{ TX_LOV0_039, 13628, 12596, 14287, 14216, 12582, 13500, 13632 },
	{ TX_LOV0_040, 13755, 12678, 14417, 14341, 12664, 13583, 13717 },
	{ TX_LOV0_041, 14051, 12899, 14757, 14651, 12885, 13821, 13985 },
	{ TX_LOV0_042, 13815, 12723, 14491, 14399, 12709, 13627, 13779 },
	{ TX_LOV0_043, 13919, 12797, 14606, 14501, 12783, 13700, 13864 },
	{ TX_LOV0_045, 13722, 12660, 14381, 14308, 12646, 13562, 13696 },
	{ TX_LOV0_046, 14185, 13003, 14898, 14783, 12989, 13924, 14095 },
	{ TX_LOV0_047, 14105, 12938, 14819, 14702, 12924, 13857, 14032 },
	{ TX_LOV0_048, 13662, 12615, 14317, 14250, 12601, 13519, 13647 },
	{ TX_LOV0_049, 13973, 12836, 14670, 14550, 12822, 13735, 13913 },
	{ TX_LOV0_050, 13863, 12756, 14544, 14455, 12742, 13668, 13817 },
	{ TX_LOV0_101, 14574, 0, 15347, 15218, 0, 0, 0 },
	{ TX_LOV0_102, 14638, 0, 15393, 15285, 0, 0, 0 },
	{ TX_LOV0_103, 14708, 0, 15474, 15367, 0, 0, 0 },
	{ TX_LOV0_104, 14758, 0, 15530, 15425, 0, 0, 0 },
	{ TX_LOV0_105, 14815, 0, 15591, 15491, 0, 0, 0 },
	{ TX_LOV0_106, 14927, 0, 15705, 15614, 0, 0, 0 },
	{ TX_LOV0_107, 14995, 0, 15781, 15695, 0, 0, 0 },
	{ TX_LOV0_124, 14443, 13201, 15208, 15068, 13188, 14149, 14345 },
	{ TX_LOV5_001, 2119, 1910, 2169, 2184, 1906, 2029, 1956 },
	{ TX_LOV5_002, 1715, 1596, 1737, 1735, 1589, 1650, 1614 },
	{ TX_LOV5_003, 10797, 10516, 11241, 11298, 10503, 11381, 11340 },
	{ TX_LOV5_004, 12334, 11876, 12876, 12875, 11863, 12705, 12798 },
	{ TX_LOV5_005, 4485, 3974, 4699, 4680, 3969, 4258, 4195 },
	{ TX_LOV5_006, 12464, 11991, 13021, 13013, 11978, 12828, 12928 },
	{ TX_LOV5_007, 4020, 3583, 4208, 4194, 3578, 3823, 3767 },
	{ TX_LOV5_008, 12178, 11735, 12699, 12727, 11722, 12573, 12636 },
	{ TX_LOV5_009, 10983, 10672, 11459, 11490, 10659, 11543, 11528 },
	{ TX_LOV5_010, 1502, 1413, 1526, 1515, 1406, 1472, 1433 },
	{ TX_LOV5_011,  614, 588, 614, 614, 588, 588, 588 },
	{ TX_LOV5_013, 4130, 3678, 4337, 4313, 3673, 3927, 3881 },
	{ TX_LOV5_014, 2897, 2595, 3015, 3005, 2591, 2758, 2711 },
	{ TX_LOV5_015, 3732, 3340, 3904, 3904, 3335, 3567, 3509 },
	{ TX_LOV5_016, 3576, 3214, 3745, 3754, 3209, 3447, 3380 },
	{ TX_LOV5_017, 4915, 4344, 5179, 5118, 4327, 4634, 4603 },
	{ TX_LOV5_018, 1263, 1144, 1276, 1237, 1137, 1151, 1154 },
	{ TX_LOV5_019, 3654, 3277, 3822, 3835, 3272, 3513, 3442 },
	{ TX_LOV5_020, 4640, 4099, 4871, 4851, 4082, 4375, 4325 },
	{ TX_LOV5_021, 1821, 1672, 1849, 1859, 1667, 1744, 1696 },
	{ TX_LOV5_024, 3407, 3060, 3556, 3563, 3056, 3271, 3207 },
	{ TX_LOV5_025, 11753, 11340, 12298, 12272, 11327, 12223, 12265 },
	{ TX_LOV5_026, 4543, 4017, 4758, 4742, 4012, 4375, 4239 },
	{ TX_LOV5_027, 2035, 1841, 2075, 2089, 1837, 6020, 6045 },
	{ TX_LOV5_028, 9148, 9182, 9453, 9614, 9169, 9932, 9852 },
	{ TX_LOV5_029, 3893, 3471, 4057, 4067, 3466, 3711, 3631 },
	{ TX_LOV5_030, 1939, 1775, 1972, 1989, 1771, 1859, 5972 },
	{ TX_LOV5_031, 9380, 9369, 9731, 9844, 9356, 10136, 10085 },
	{ TX_LOV5_032, 1629, 1525, 1653, 1643, 1518, 1573, 1545 },
	{ TX_LOV5_033, 3159, 2827, 3298, 3309, 2823, 3032, 2964 },
	{ TX_LOV5_034, 11526, 11143, 12036, 12041, 11130, 12022, 12033 },
	{ TX_LOV5_036, 11189, 10848, 11691, 11705, 10835, 11728, 11730 },
	{ TX_LOV5_038, 1990, 1811, 2033, 2043, 1807, 1898, 6018 },
	{ TX_LOV5_039, 2414, 2175, 2489, 2497, 2171, 2314, 2246 },
	{ TX_LOV5_040, 2334, 2110, 2393, 2408, 2106, 2238, 2165 },
	{ TX_LOV5_042, 2607, 2353, 2721, 2705, 2349, 2507, 2463 },
	{ TX_LOV5_043, 12668, 12283, 13217, 13225, 12270, 13166, 13272 },
	{ TX_LOV5_044, 9199, 9218, 9492, 9663, 9205, 9967, 9876 },
	{ TX_LOV5_045, 1756, 1622, 1781, 1785, 1616, 1685, 1643 },
	{ TX_LOV5_046, 9839, 9768, 10240, 10356, 9755, 10606, 10534 },
	{ TX_LOV5_047, 11049, 10723, 11531, 11550, 10710, 11588, 11585 },
	{ TX_LOV5_048, 10854, 10558, 11304, 11361, 10545, 11429, 11388 },
	{ TX_LOV5_049, 11642, 11244, 12176, 12157, 11231, 12123, 12158 },
	{ TX_LOV5_050, 9442, 9416, 9797, 9927, 9403, 10203, 10136 },
	{ TX_LOV5_051, 11290, 10934, 11789, 11799, 10921, 11807, 11813 },
	{ TX_LOV5_052,  3039, 2722, 3165, 3152, 2718, 2890, 2846 },
	{ TX_LOV5_053,  4426, 3930, 4628, 4608, 3925, 4200, 4139 },
	{ TX_LOV5_054, 10123, 9992, 10543, 10649, 9979, 10846, 10777 },
	{ TX_LOV5_055, 2685, 2416, 2800, 2788, 2412, 2575, 2527 },
	{ TX_LOV5_056, 2744, 2460, 2858, 2851, 2456, 2623, 2570 },
	{ TX_LOV5_057, 10052, 9936, 10461, 10583, 9923, 10788, 10710 },
	{ TX_LOV5_058, 10193, 10047, 10633, 10710, 10034, 10892, 10852 },
	{ TX_LOV5_059, 11980, 11552, 12494, 12516, 11539, 12377, 12446 },
	{ TX_LOV5_060, 12789, 12389, 13359, 13350, 12376, 13276, 13399 },
	{ TX_LOV5_061, 12604, 1353, 13150, 13163, 1346, 1411, 1370 },
	{ TX_LOV5_023,  5017, 4431, 5278, 5200, 4414, 4701, 4687 },
	{ TX_LOV5_041,  5296, 4695, 5557, 5477, 4678, 4963, 4951 },
	{ TX_FEA5_008,   706, 0, 702, 703, 0, 0, 0 },
	{ TX_G_024,    11501, 11130, 12013, 12015, 11117, 12008, 12022 },
	{ TX_LOV5N000, 10294, 10127, 10742, 10788, 10114, 10955, 10946 },
	{ TX_LOV5N001_BOUND, 9732, 9676, 10123, 10243, 9663, 10508, 10432 },
	{ TX_LOV5N001, 9890, 9804, 10285, 10414, 9791, 10649, 10564 },
	{ TX_LOV5N002, 10571, 10335, 11019, 11073, 10322, 11201, 11163 },
	{ TX_LOV5N003, 10691, 10425, 11129, 11186, 10412, 11284, 11243 },
	{ TX_LOV5N004, 10481, 10260, 10920, 10968, 10247, 11102, 11079 },
	{ TX_LOV5N005, 10633, 10382, 11075, 11122, 10369, 11235, 11204 },
	{ TX_LOV5N006, 1352, 1218, 1372, 1350, 1211, 1249, 1231 },
	{ TX_LOV5N007, 4813, 4257, 5063, 5024, 4240, 4544, 4502 },
	{ TX_LOV5N008,  863, 774, 859, 830, 767, 774, 767 },
	{ TX_LOV5N009, 1115, 1011, 1120, 1084, 1004, 1013, 1013 },
	{ TX_LOV5N010, 9660, 9619, 10051, 10170, 9606, 10450, 10375 },
	{ TX_LOV5N011, 10413, 10207, 10855, 10900, 10194, 11038, 11029 },
	{ TX_LOV5N012, 10359, 10168, 10801, 10842, 10155, 10994, 10990 },
	{ TX_LOV5N013, 9040, 9089, 9328, 9491, 9076, 9824, 9742 },
	{ TX_LOV5N014, 9961, 9860, 10367, 10490, 9847, 10710, 10631 },
	{ TX_LOV5N016, 9311, 9315, 9652, 9775, 9302, 10082, 10021 },
	{ TX_LOV5N017, 4320, 3853, 4521, 4508, 3848, 4107, 4050 },
	{ TX_LOV5N018, 3805, 3398, 3976, 3979, 3393, 3627, 3566 },
	//{ TX_LOV5C001, 1598, 0, 0, 0 },
	{ TX_DIALOG_ERROR, 11927, 11499, 12441, 12463, 11486, 12324, 12393 },
	{ TX_LOV5F001, 0, 12134, 0, 0, 12121, 13006, 13099 },
	{ TX_LOV5F002, 0, 1879, 0, 0, 1875, 1898, 1925 },
	{          -1, 0,    0, 0, 0, 0, 0, 0 }
};

extern const RoomText love5Texts[] = {
	{ TX_DEM0N009, Common::EN_ANY, "#DEM0\\DEM0N009#Spock raises an eyebrow" },
	{ TX_DEM0N009, Common::DE_DEU, "#DEM0\\DEM0N009#Spock hebt eine Augenbraue." },
	{ TX_DEM0N009, Common::FR_FRA, "#DEM0\\DEM0N009#Spock hausse un sourcil." },
	{ -1, Common::UNK_LANG, "" }
};

void Room::love5Tick1() {
	playVoc("LOV5LOOP");

	loadActorAnim(OBJECT_8, "s3r6c1", 0x27, 0xa9);

	if (_awayMission->love.freedMarcusAndCheever) {
		loadActorAnim(OBJECT_MARCUS, "marcusst", 0x28, 0xa6);
		loadActorAnim(OBJECT_CHEEVER, "s3gtupst", 0x49, 0xac);
	} else {
		loadActorAnim(OBJECT_MARCUS, "marcus2", 0x28, 0xa6);
		loadActorAnim(OBJECT_CHEEVER, "s3gtup2", 0x49, 0xac);
	}

	if (_awayMission->love.preaxCured)
		loadActorAnim(OBJECT_PREAX, "s3r6r3", 0x116, 0xba);
	else
		loadActorAnim(OBJECT_PREAX, "s3r6r2", 0x116, 0xba);

	if (_awayMission->love.releasedHumanLaughingGas)
		_awayMission->timers[0] = getRandomWordInRange(200, 400);
	if (_awayMission->love.releasedRomulanLaughingGas)
		_awayMission->timers[1] = getRandomWordInRange(200, 400);
	_awayMission->timers[2] = 200;

	_roomVar.love.cmnXPosToCureSpock = 0x89;
	_roomVar.love.cmnXPosToCureSpock = 0xb9;
}

void Room::love5TouchedWarp1() {
	_awayMission->disableInput = true;
	playSoundEffectIndex(kSfxDoor);
	_awayMission->timers[4] = 10;
}

void Room::love5Timer4Expired() {
	loadRoomIndex(4, 0);
}

void Room::love5UseStunPhaserOnPreax() {
	showText(TX_SPEAKER_MCCOY, TX_LOV5_011);

	// BUG: this function has two implementations. The second one (which is shadowed) is
	// below.
	// Perhaps one was supposed to be for IPHASERK, or for Dr. Marcus?
	//showText(TX_SPEAKER_MCCOY, TX_FEA5_008);
}

void Room::love5LookAtKirk() {
	showDescription(TX_LOV5N004);
}

void Room::love5LookAtSpock() {
	showDescription(TX_LOV5N005);
}

void Room::love5LookAtMccoy() {
	showDescription(TX_LOV5N002);
}

void Room::love5LookAtRedshirt() {
	showDescription(TX_LOV5N003);
}

void Room::love5LookAnywhere() {
	showDescription(TX_LOV5N008);

	// There is an unused version of this function, which states "This is Dr. Marcus's
	// quarters, and there are hostages and 4 romulans here". That description doesn't
	// match any actual room.
	// There are LOVE6.MAP and LOVE6.IW files, but none of the other necessary files to
	// load room LOVE6. This may be a leftover from a removed room that somehow ended up
	// here.

	//showText(TX_LOV5N009);
}

void Room::love5LookAtDevice() {
	showDescription(TX_LOV5N013);
	showText(TX_SPEAKER_MCCOY, TX_LOV5_028);
	showText(TX_SPEAKER_MARCUS, TX_LOV5_044);
}

void Room::love5LookAtConsole() {
	showDescription(TX_LOV5N016);
	showText(TX_SPEAKER_MCCOY, TX_LOV5_031);
	showText(TX_SPEAKER_MARCUS, TX_LOV5_050);
}

void Room::love5LookAtDrMarcus() {
	if (_awayMission->love.freedMarcusAndCheever)
		showDescription(TX_LOV5N001);
	else {
		// NOTE: The audio LOV5N001 only has the first sentence, LOV5N015 may fit better
		showDescription(TX_LOV5N001_BOUND);
		showText(TX_SPEAKER_MARCUS, TX_LOV5_046);
	}
}

void Room::love5LookAtDrCheever() {
	if (_awayMission->love.freedMarcusAndCheever)
		showDescription(TX_LOV5N000);
	else {
		showDescription(TX_LOV5N014);
		showText(TX_SPEAKER_CHEEVER, TX_LOV5_057);
		if (!_awayMission->redshirtDead) {
			showText(TX_SPEAKER_FERRIS, TX_LOV5_054);
			showText(TX_SPEAKER_CHEEVER, TX_LOV5_058);
		}
	}
}

void Room::love5LookAtPreax() {
	if (_awayMission->love.preaxCured)
		showDescription(TX_LOV5N011);
	else
		showDescription(TX_LOV5N012);
}

void Room::love5TalkToPreax() {
	if (_awayMission->love.preaxCured) {
		if (!_awayMission->love.freedMarcusAndCheever)
			showDescription(TX_LOV5N006);
		else {
			const TextRef choices[] = {
				TX_SPEAKER_KIRK,
				TX_LOV5_008,
				TX_LOV5_004,
				TX_LOV5_006,
				TX_END,
			};

			showText(TX_SPEAKER_PREAX, TX_LOV5_059);
			int choice = showMultipleTexts(choices);

			switch (choice) {
			case 1:
				showText(TX_SPEAKER_MARCUS, TX_LOV5_043);
			// fall through
			case 0:
				if (_vm->isFloppyVersion())
					showText(TX_SPEAKER_PREAX, TX_LOV5F001);
				showText(TX_SPEAKER_PREAX, TX_LOV5_061);
				showText(TX_SPEAKER_KIRK, TX_LOV5_010);

				// ENHANCEMENT: Original text was just "(Spock raises eyebrow)" without any audio.
				// The CD version uses a narration instead to make it flow better.
				showText(TX_SPEAKER_SPOCK, _vm->isFloppyVersion() ? TX_LOV5F002 : TX_DEM0N009);

				showText(TX_SPEAKER_MCCOY, TX_LOV5_032);

				break;
			case 2:
				showText(TX_SPEAKER_PREAX, TX_LOV5_060);
				break;
			default:
				showDescription(TX_DIALOG_ERROR);
				break;
			}

			showText(TX_SPEAKER_KIRK,   TX_LOV5_002);
			showText(TX_SPEAKER_MARCUS, TX_LOV5_045);

			_awayMission->disableInput = true;

			if (!_awayMission->love.spockCured) {
				showText(TX_SPEAKER_MCCOY, TX_LOV5_021);
				walkCrewmanC(OBJECT_MCCOY, 0x87, 0xc3, &Room::love5MccoyReachedSpockToCure);
			} else
				love5EndMission();
		}
	} else
		showText(TX_SPEAKER_MARCUS, TX_LOV5_018);
}

void Room::love5MccoyReachedSpockToCure() {
	loadActorAnimC(OBJECT_MCCOY, "musemn", -1, -1, &Room::love5MccoyCuredSpock);
}

void Room::love5MccoyCuredSpock() {
	showText(TX_SPEAKER_MCCOY, TX_LOV5_030);
	showText(TX_SPEAKER_SPOCK, TX_LOV5_038);
	showText(TX_SPEAKER_MCCOY, TX_LOV5_027);
	// ENHANCEMENT: Original text was just "(Spock raises eyebrow)" without any audio.
	// The CD version uses a narration instead to make it flow better.
	showText(TX_SPEAKER_SPOCK, _vm->isFloppyVersion() ? TX_LOV5F002 : TX_DEM0N009);
	love5EndMission();
}

void Room::love5EndMission() {
	showText(TX_SPEAKER_KIRK, TX_LOV5_001);

	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_S;
	walkCrewmanC(OBJECT_KIRK, 0x64, 0xc2, &Room::love5CrewmanReachedBeamoutPosition);

	_awayMission->crewDirectionsAfterWalk[OBJECT_SPOCK] = DIR_S;
	walkCrewmanC(OBJECT_SPOCK, 0x5a, 0xb8, &Room::love5CrewmanReachedBeamoutPosition);

	_awayMission->crewDirectionsAfterWalk[OBJECT_MCCOY] = DIR_S;
	walkCrewmanC(OBJECT_MCCOY, 0x6e, 0xb8, &Room::love5CrewmanReachedBeamoutPosition);

	if (!_awayMission->redshirtDead) {
		_awayMission->crewDirectionsAfterWalk[OBJECT_REDSHIRT] = DIR_S;
		walkCrewmanC(OBJECT_REDSHIRT, 0x69, 0xae, &Room::love5CrewmanReachedBeamoutPosition);
		_roomVar.love.numCrewmenReadyToBeamOut--;
	}
}

void Room::love5CrewmanReachedBeamoutPosition() {
	_roomVar.love.numCrewmenReadyToBeamOut++;
	if (_roomVar.love.numCrewmenReadyToBeamOut == 3) {
		_awayMission->love.missionScore += 17;
		endMission(_awayMission->love.missionScore, 0x2c, 0);
	}
}

void Room::love5TalkToDrMarcus() {
	if (!_awayMission->love.freedMarcusAndCheever)
		showText(TX_SPEAKER_MARCUS, TX_LOV5_040);
	else {
		if (!haveItem(OBJECT_ICURE))
			showText(TX_SPEAKER_MARCUS, TX_LOV5_039);
		else
			showText(TX_SPEAKER_MARCUS, TX_LOV5_042);
	}
}

void Room::love5TalkToDrCheever() {
	if (!_awayMission->love.freedMarcusAndCheever)
		showText(TX_SPEAKER_CHEEVER, TX_LOV5_055);
	else
		showText(TX_SPEAKER_CHEEVER, TX_LOV5_056);
}

void Room::love5TalkToKirk() {
	showText(TX_SPEAKER_KIRK,   TX_LOV5_003);
	showText(TX_SPEAKER_MARCUS, TX_LOV5_048);
	showText(TX_SPEAKER_KIRK,   TX_LOV5_009);
	showText(TX_SPEAKER_MARCUS, TX_LOV5_047);
}

void Room::love5TalkToSpock() {
	showText(TX_SPEAKER_SPOCK,  TX_LOV5_036);
	showText(TX_SPEAKER_MARCUS, TX_LOV5_051);
	showText(TX_SPEAKER_SPOCK,  TX_G_024);
}

void Room::love5TalkToMccoy() {
	if (!_awayMission->love.preaxCured)
		showText(TX_SPEAKER_MCCOY, TX_LOV5_014);
}

void Room::love5TalkToRedshirt() {
	showText(TX_SPEAKER_FERRIS, TX_LOV5_052);
}

void Room::love5UseSTricorderAnywhere() {
	spockScan(DIR_S, TX_SPEAKER_SPOCK, TX_LOV5_033, false);
}

void Room::love5UseSTricorderOnDevice() {
	spockScan(DIR_N, TX_SPEAKER_SPOCK, TX_LOV5_034, false);
	showText(TX_SPEAKER_MARCUS, TX_LOV5_049);
}

void Room::love5UseMTricorderOnMarcusOrCheever() {
	mccoyScan(DIR_W, TX_LOV5_025, false);
}

void Room::love5UseMTricorderOnPreax() {
	if (!_awayMission->love.preaxCured) {
		if (_awayMission->love.romulansUnconsciousFromLaughingGas)
			mccoyScan(DIR_E, TX_SPEAKER_MCCOY, TX_LOV5_024, false);
		else
			mccoyScan(DIR_E, TX_SPEAKER_MCCOY, TX_LOV5_016, false);
	}
}

void Room::love5UseMedkitOnPreax() {
	if (_awayMission->love.preaxCured)
		showText(TX_SPEAKER_MCCOY, TX_LOV5_015);
	else
		showText(TX_SPEAKER_MCCOY, TX_LOV5_019);
}

void Room::love5UseWaterOnPreax() {
	if (_awayMission->love.preaxCured) {
		showDescription(TX_LOV5N018);
		showText(TX_SPEAKER_MCCOY, TX_LOV5_029);
		showText(TX_SPEAKER_KIRK,  TX_LOV5_007);
		if (!_awayMission->love.gotPointsForHydratingPreax) {
			_awayMission->love.gotPointsForHydratingPreax = true;
			_awayMission->love.missionScore++;
		}
		loseItem(OBJECT_IH2O);
	} else {
		if (_awayMission->love.romulansUnconsciousFromVirus) {
			showText(TX_SPEAKER_MCCOY, TX_LOV5_013);
			// BUGFIX: original didn't have correct speaker. Also, you shouldn't lose your
			// water since it's not actually used here, so a "loseItem" line was removed.
		} else {
			showDescription(TX_LOV5N017);
			if (!_awayMission->redshirtDead) {
				showText(TX_SPEAKER_FERRIS, TX_LOV5_053);
				showText(TX_SPEAKER_KIRK,   TX_LOV5_005);
			}
			showText(TX_SPEAKER_MCCOY,  TX_LOV5_026);
			loseItem(OBJECT_IH2O);

			// BUGFIX: give a point for hydrating the Preax. It's inconsistent to only
			// give points if he's already cured.
			if (!_awayMission->love.gotPointsForHydratingPreax) {
				_awayMission->love.gotPointsForHydratingPreax = true;
				_awayMission->love.missionScore++;
			}
		}
	}
}

void Room::love5UseCureSampleOnPreax() {
	showText(TX_SPEAKER_MCCOY, TX_LOV5_020); // BUGFIX: original didn't have correct speaker
}


void Room::love5UseCureOnPreax() {
	if (!_awayMission->love.preaxCured)
		walkCrewmanC(OBJECT_MCCOY, 0x116, 0xbf, &Room::love5ReachedPreaxUsingCure);
}

void Room::love5ReachedPreaxUsingCure() {
	loadActorAnimC(OBJECT_MCCOY, "musemn", -1, -1, &Room::love5CuredPreax);
}

void Room::love5CuredPreax() {
	walkCrewman(OBJECT_MCCOY, 0xdc, 0xc3);
	_awayMission->crewDirectionsAfterWalk[OBJECT_MCCOY] = DIR_E;

	showDescription(TX_LOV5N007);

	// BUG: says he's dehydrated, but doesn't check whether you've given water to him
	// already (like it does in LOVE4).
	showText(TX_SPEAKER_MCCOY, TX_LOV5_017);

	loadActorAnim2(OBJECT_PREAX, "s3r6r1", 0x116, 0xba);
	_awayMission->love.preaxCured = true;
}


void Room::love5UseAnythingOnPreax() {
	showText(TX_SPEAKER_MCCOY, TX_LOV5_023);
}


// Kirk walks to Cheever, unties him, then walks to Marcus, unties her.
void Room::love5UseKirkOnMarcusOrCheever() {
	if (!_awayMission->love.freedMarcusAndCheever) {
		_awayMission->disableInput = true;
		walkCrewmanC(OBJECT_KIRK, 0x49, 0xb1, &Room::love5KirkReachedCheever);
		_awayMission->love.freedMarcusAndCheever = true;
	}
}

void Room::love5KirkReachedCheever() {
	loadActorAnimC(OBJECT_KIRK, "kuntie", -1, -1, &Room::love5KirkUntiedCheever);
}

void Room::love5KirkUntiedCheever() {
	loadActorAnim2(OBJECT_CHEEVER, "s3gtup", 0x49, 0xac);
	walkCrewmanC(OBJECT_KIRK, 0x26, 0xb1, &Room::love5KirkReachedMarcus);
}

void Room::love5KirkReachedMarcus() {
	loadActorAnimC(OBJECT_KIRK, "kuntie", -1, -1, &Room::love5KirkUntiedMarcus);
}

void Room::love5KirkUntiedMarcus() {
	loadActorStandAnim(OBJECT_KIRK);
	loadActorAnimC(OBJECT_MARCUS, "marcus", 0x28, 0xa6, &Room::love5MarcusStoodUp);
	walkCrewman(OBJECT_KIRK, 0x35, 0xb9);
	_awayMission->crewDirectionsAfterWalk[OBJECT_KIRK] = DIR_N;
}

void Room::love5MarcusStoodUp() {
	_awayMission->disableInput = false;
	showText(TX_SPEAKER_MARCUS, TX_LOV5_041);
}

}
