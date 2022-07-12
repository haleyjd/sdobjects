/*
  sdobjects

  Definitions for reading SNES Doom objects

  The MIT License (MIT)

  Copyright (c) 2022 James Haley

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include "elib/elib.h"
#include "sdobjects.h"

// Reverse translation table from ObjDataNums back to DoomEd numbers
const int16_t doomednums[ToIndex(ObjDataNums::MAX)] =
{
    1,    // Player1Start,
    2,    // Player2Start,
    3,    // Player3Start,
    4,    // Player4Start,
    11,   // DeathMatchStart,
    14,   // TeleportSpot,
    3004, // Soldier,
    9,    // Sergeant,
    3001, // Trooper,
    3002, // Demon,
    3005, // CacoDemon,
    3006, // LostSoul,
    3003, // BaronOfHell,
    16,   // CyberDemon,
    7,    // SpiderDemon,
    2001, // ShotGun,
    2005, // ChainSaw,
    2002, // ChainGun,
    2003, // RocketLauncher,
    2004, // PlasmaGun,
    2006, // BFG9000,
    13,   // RedKeyCard,
    5,    // BlueKeyCard,
    6,    // YellowKeyCard,
    38,   // RedSkullKey,
    39,   // YellowSkullKey,
    40,   // BlueSkullKey,
    8,    // BackPack,
    2007, // Clip,
    2048, // AmmoBox,
    2008, // Shells,
    2049, // ShellsBox,
    2010, // Rocket,
    2046, // RocketBox,
    2047, // Cell,
    17,   // CellPack,
    2011, // StimPak,
    2012, // Medikit,
    2014, // HealthBonus,
    2015, // ArmorBonus,
    2018, // ArmorGreen,
    2019, // ArmorBlue,
    2013, // SoulSphere,
    2022, // Invulnerable,
    2023, // Berserk,
    2024, // Invisible,
    2025, // RadiationSuit,
    2026, // ComputerMap,
    2045, // LightGoggles,
    2035, // Barrel,
    2028, // FloorLamp,
    10,   // BloodyMess,
    20,   // DeadTrooper,
    21,   // DeadDemon,
    27,   // SkullOnPole,
    31,   // PillarShortGreen,
    34,   // Candle,
    35,   // Candelabra,
    42,   // FlamingSkullRock,
    43,   // TreeGray,
    46,   // FireStickTallRed,
    47,   // ShrubBrown,
    48,   // ColumnTechTall,
    55    // FireStickShortBlue,
};

// EOF
