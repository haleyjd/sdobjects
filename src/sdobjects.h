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

#pragma once

#include <vector>

// RLObject stores the data read from an OBJECTS lump.
struct RLObject
{
    uint8_t  flags; // values defined by RLObjectFlags
    uint8_t  type;  // values defined by ObjDataNums
    int16_t  x;     // x coordinate
    int16_t  y;     // y coordinate
    uint16_t angle; // angle (short BAM); only present for player starts, dm starts, and teleport landings
};

// The order of ObjData definitions from objdata.a, which is indexed by the 
// OBJECTS structure's type field.
enum class ObjDataNums : euint
{
    Player1Start,
    Player2Start,
    Player3Start,
    Player4Start,
    DeathMatchStart,
    TeleportSpot,
    Soldier,
    Sergeant,
    Trooper,
    Demon,
    CacoDemon,
    LostSoul,
    BaronOfHell,
    CyberDemon,
    SpiderDemon,
    ShotGun,
    ChainSaw,
    ChainGun,
    RocketLauncher,
    PlasmaGun,
    BFG9000,
    RedKeyCard,
    BlueKeyCard,
    YellowKeyCard,
    RedSkullKey,
    YellowSkullKey,
    BlueSkullKey,
    BackPack,
    Clip,
    AmmoBox,
    Shells,
    ShellsBox,
    Rocket,
    RocketBox,
    Cell,
    CellPack,
    StimPak,
    Medikit,
    HealthBonus,
    ArmorBonus,
    ArmorGreen,
    ArmorBlue,
    SoulSphere,
    Invulnerable,
    Berserk,
    Invisible,
    RadiationSuit,
    ComputerMap,
    LightGoggles,
    Barrel,
    FloorLamp,
    BloodyMess,
    DeadTrooper,
    DeadDemon,
    SkullOnPole,
    PillarShortGreen,
    Candle,
    Candelabra,
    FlamingSkullRock,
    TreeGray,
    FireStickTallRed,
    ShrubBrown,
    ColumnTechTall,
    FireStickShortBlue,
    MAX,
    LASTANGULAR = TeleportSpot // only objects <= to this have angles
};
constexpr std::underlying_type_t<ObjDataNums> ToIndex(ObjDataNums e) { 
    return static_cast<std::underlying_type_t<ObjDataNums>>(e);
}

constexpr bool RLObjectIsAngular(const RLObject &obj) {
    return obj.type <= ToIndex(ObjDataNums::LASTANGULAR);
}

// Reverse translation table from ObjDataNums back to DoomEd numbers
extern const int16_t doomednums[ToIndex(ObjDataNums::MAX)];

// SNES Doom object flags
enum RLObjectFlags : uint8_t
{
    rlpfSkill12     = 0x01u,
    rlpfSkill3      = 0x02u,
    rlpfSkill45     = 0x04u,
    rlpfMultiPlayer = 0x10u,
    rlpfMovable     = 0x40u,
    rlpfActive      = 0x80u
};

// Convert RL angle (short BAM) to Doom angle (0-359 degrees)
constexpr int16_t DoomAngleFromRLAngle(uint16_t rlangle) {
    return int16_t(euint(rlangle) * 360 / 0x10000);
}

// Collection of RLObject structures
using RLObjects = std::vector<RLObject>;

// Read in RLObject structures from a binary file
RLObjects ReadRLObjects(const ebyte *pdata, size_t len);

// Print information about RLObjects
void PrintRLObjects(const RLObjects &objects);

// EOF
