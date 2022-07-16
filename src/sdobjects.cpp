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
#include "elib/binary.h"
#include "elib/qstring.h"
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

#define ENUMSTR(e) #e

// Pretty printable names for each RLObject type
static const char *const ObjNames[ToIndex(ObjDataNums::MAX)] =
{
    ENUMSTR(Player1Start),
    ENUMSTR(Player2Start),
    ENUMSTR(Player3Start),
    ENUMSTR(Player4Start),
    ENUMSTR(DeathMatchStart),
    ENUMSTR(TeleportSpot),
    ENUMSTR(Soldier),
    ENUMSTR(Sergeant),
    ENUMSTR(Trooper),
    ENUMSTR(Demon),
    ENUMSTR(CacoDemon),
    ENUMSTR(LostSoul),
    ENUMSTR(BaronOfHell),
    ENUMSTR(CyberDemon),
    ENUMSTR(SpiderDemon),
    ENUMSTR(ShotGun),
    ENUMSTR(ChainSaw),
    ENUMSTR(ChainGun),
    ENUMSTR(RocketLauncher),
    ENUMSTR(PlasmaGun),
    ENUMSTR(BFG9000),
    ENUMSTR(RedKeyCard),
    ENUMSTR(BlueKeyCard),
    ENUMSTR(YellowKeyCard),
    ENUMSTR(RedSkullKey),
    ENUMSTR(YellowSkullKey),
    ENUMSTR(BlueSkullKey),
    ENUMSTR(BackPack),
    ENUMSTR(Clip),
    ENUMSTR(AmmoBox),
    ENUMSTR(Shells),
    ENUMSTR(ShellsBox),
    ENUMSTR(Rocket),
    ENUMSTR(RocketBox),
    ENUMSTR(Cell),
    ENUMSTR(CellPack),
    ENUMSTR(StimPak),
    ENUMSTR(Medikit),
    ENUMSTR(HealthBonus),
    ENUMSTR(ArmorBonus),
    ENUMSTR(ArmorGreen),
    ENUMSTR(ArmorBlue),
    ENUMSTR(SoulSphere),
    ENUMSTR(Invulnerable),
    ENUMSTR(Berserk),
    ENUMSTR(Invisible),
    ENUMSTR(RadiationSuit),
    ENUMSTR(ComputerMap),
    ENUMSTR(LightGoggles),
    ENUMSTR(Barrel),
    ENUMSTR(FloorLamp),
    ENUMSTR(BloodyMess),
    ENUMSTR(DeadTrooper),
    ENUMSTR(DeadDemon),
    ENUMSTR(SkullOnPole),
    ENUMSTR(PillarShortGreen),
    ENUMSTR(Candle),
    ENUMSTR(Candelabra),
    ENUMSTR(FlamingSkullRock),
    ENUMSTR(TreeGray),
    ENUMSTR(FireStickTallRed),
    ENUMSTR(ShrubBrown),
    ENUMSTR(ColumnTechTall),
    ENUMSTR(FireStickShortBlue)
};

constexpr size_t SMALLESTOBJLEN = 6;

//
// Read in RLObject structures from a binary file
//
RLObjects ReadRLObjects(const ebyte *pdata, size_t len)
{
    RLObjects objects;
    const ebyte *rover = pdata;
    size_t remaining = len;

    while(remaining >= SMALLESTOBJLEN)
    {
        RLObject obj {};
        
        obj.flags = *rover++;
        obj.type  = *rover++;
        obj.x     = E_GetBinaryWord(&rover);
        obj.y     = E_GetBinaryWord(&rover);

        remaining -= SMALLESTOBJLEN;

        if(RLObjectIsAngular(obj) && remaining >= 2)
        {
            obj.angle = E_GetBinaryUWord(&rover);
            remaining -= 2;
        }
        else
            obj.angle = 0;

        objects.push_back(obj);
    }

    return objects;
}

//
// Print information about RLObjects
//
void PrintRLObjects(const RLObjects &objects)
{
    static const struct flagdata_t
    {
        uint8_t flag;
        const char *name;
    } flagdata[] =
    {
        { rlpfSkill12,     "EASY"    },
        { rlpfSkill3,      "MEDIUM"  },
        { rlpfSkill45,     "HARD"    },
        { rlpfMultiPlayer, "MULTI"   },
        { rlpfMovable,     "MOVABLE" },
        { rlpfActive,      "ACTIVE"  }
    };

    std::printf("\"objects\": [\n");

    int num = 0;
    for(const RLObject &obj : objects)
    {
        if(num != 0)
            std::printf(",\n");

        qstring flagstr;
        if(obj.flags != 0)
        {
            for(const flagdata_t &fd : flagdata)
            {
                if(obj.flags & fd.flag)
                {
                    flagstr += fd.name;
                    flagstr += "|";
                }
            }
        }
        if(flagstr.endsWith('|'))
            flagstr.pop();
        
        const char *thingname = "Unknown";
        if(obj.type >= 0 && obj.type < earrlen(ObjNames))
            thingname = ObjNames[obj.type];

        int16_t angle = 0;
        if(RLObjectIsAngular(obj))
            angle = DoomAngleFromRLAngle(obj.angle);

        std::printf(
            "  {\n"
            "    \"num\":    %d,\n"
            "    \"flags\":  \"%s\",\n"
            "    \"type\":   \"%s\",\n"
            "    \"coords\": [%hd, %hd],\n"
            "    \"angle\":  %hd\n"
            "  }",
            num, flagstr.c_str(), thingname, obj.x, obj.y, angle
        );
        ++num;
    }
    std::printf("\n]\n");
}

// EOF
