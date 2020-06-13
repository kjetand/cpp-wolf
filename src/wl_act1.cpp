// WL_ACT1.C

#include "wl_def.h"
#pragma hdrstop

/*
=============================================================================

                                                        STATICS

=============================================================================
*/

statstruct  statobjlist[MAXSTATS];
statstruct* laststatobj;

struct
{
    short     picnum;
    wl_stat_t type;
    uint32_t  specialFlags; // they are ORed to the statstruct flags
} statinfo[] = {
    { static_cast<short>(sprite::SPR_STAT_0) },                                                                         // puddle          spr1v
    { static_cast<short>(sprite::SPR_STAT_1), wl_stat_t::block },                                                       // Green Barrel    "
    { static_cast<short>(sprite::SPR_STAT_2), wl_stat_t::block },                                                       // Table/chairs    "
    { static_cast<short>(sprite::SPR_STAT_3), wl_stat_t::block, static_cast<std::uint32_t>(objflag_t::FL_FULLBRIGHT) }, // Floor lamp      "
    { static_cast<short>(sprite::SPR_STAT_4), wl_stat_t::none, static_cast<std::uint32_t>(objflag_t::FL_FULLBRIGHT) },  // Chandelier      "
    { static_cast<short>(sprite::SPR_STAT_5), wl_stat_t::block },                                                       // Hanged man      "
    { static_cast<short>(sprite::SPR_STAT_6), wl_stat_t::bo_alpo },                                                     // Bad food        "
    { static_cast<short>(sprite::SPR_STAT_7), wl_stat_t::block },                                                       // Red pillar      "
    //
    // NEW PAGE
    //
    { static_cast<short>(sprite::SPR_STAT_8), wl_stat_t::block },                                                       // Tree            spr2v
    { static_cast<short>(sprite::SPR_STAT_9) },                                                                         // Skeleton flat   "
    { static_cast<short>(sprite::SPR_STAT_10), wl_stat_t::block },                                                      // Sink            " (SOD:gibs)
    { static_cast<short>(sprite::SPR_STAT_11), wl_stat_t::block },                                                      // Potted plant    "
    { static_cast<short>(sprite::SPR_STAT_12), wl_stat_t::block },                                                      // Urn             "
    { static_cast<short>(sprite::SPR_STAT_13), wl_stat_t::block },                                                      // Bare table      "
    { static_cast<short>(sprite::SPR_STAT_14), wl_stat_t::none, static_cast<std::uint32_t>(objflag_t::FL_FULLBRIGHT) }, // Ceiling light   "
#ifndef SPEAR
    { static_cast<short>(sprite::SPR_STAT_15) }, // Kitchen stuff   "
#else
    { static_cast<short>(sprite::SPR_STAT_15), block }, // Gibs!
#endif
    //
    // NEW PAGE
    //
    { static_cast<short>(sprite::SPR_STAT_16), wl_stat_t::block },   // suit of armor   spr3v
    { static_cast<short>(sprite::SPR_STAT_17), wl_stat_t::block },   // Hanging cage    "
    { static_cast<short>(sprite::SPR_STAT_18), wl_stat_t::block },   // SkeletoninCage  "
    { static_cast<short>(sprite::SPR_STAT_19) },                     // Skeleton relax  "
    { static_cast<short>(sprite::SPR_STAT_20), wl_stat_t::bo_key1 }, // Key 1           "
    { static_cast<short>(sprite::SPR_STAT_21), wl_stat_t::bo_key2 }, // Key 2           "
    { static_cast<short>(sprite::SPR_STAT_22), wl_stat_t::block },   // stuff             (SOD:gibs)
    { static_cast<short>(sprite::SPR_STAT_23) },                     // stuff
    //
    // NEW PAGE
    //
    { static_cast<short>(sprite::SPR_STAT_24), wl_stat_t::bo_food },       // Good food       spr4v
    { static_cast<short>(sprite::SPR_STAT_25), wl_stat_t::bo_firstaid },   // First aid       "
    { static_cast<short>(sprite::SPR_STAT_26), wl_stat_t::bo_clip },       // Clip            "
    { static_cast<short>(sprite::SPR_STAT_27), wl_stat_t::bo_machinegun }, // Machine gun     "
    { static_cast<short>(sprite::SPR_STAT_28), wl_stat_t::bo_chaingun },   // Gatling gun     "
    { static_cast<short>(sprite::SPR_STAT_29), wl_stat_t::bo_cross },      // Cross           "
    { static_cast<short>(sprite::SPR_STAT_30), wl_stat_t::bo_chalice },    // Chalice         "
    { static_cast<short>(sprite::SPR_STAT_31), wl_stat_t::bo_bible },      // Bible           "
    //
    // NEW PAGE
    //
    { static_cast<short>(sprite::SPR_STAT_32), wl_stat_t::bo_crown },                                                          // crown           spr5v
    { static_cast<short>(sprite::SPR_STAT_33), wl_stat_t::bo_fullheal, static_cast<std::uint32_t>(objflag_t::FL_FULLBRIGHT) }, // one up          "
    { static_cast<short>(sprite::SPR_STAT_34), wl_stat_t::bo_gibs },                                                           // gibs            "
    { static_cast<short>(sprite::SPR_STAT_35), wl_stat_t::block },                                                             // barrel          "
    { static_cast<short>(sprite::SPR_STAT_36), wl_stat_t::block },                                                             // well            "
    { static_cast<short>(sprite::SPR_STAT_37), wl_stat_t::block },                                                             // Empty well      "
    { static_cast<short>(sprite::SPR_STAT_38), wl_stat_t::bo_gibs },                                                           // Gibs 2          "
    { static_cast<short>(sprite::SPR_STAT_39), wl_stat_t::block },                                                             // flag            "
//
// NEW PAGE
//
#ifndef SPEAR
    { static_cast<short>(sprite::SPR_STAT_40), wl_stat_t::block }, // Call Apogee          spr7v
#else
    { static_cast<short>(sprite::SPR_STAT_40) },        // Red light
#endif
    //
    // NEW PAGE
    //
    { static_cast<short>(sprite::SPR_STAT_41) }, // junk            "
    { static_cast<short>(sprite::SPR_STAT_42) }, // junk            "
    { static_cast<short>(sprite::SPR_STAT_43) }, // junk            "
#ifndef SPEAR
    { static_cast<short>(sprite::SPR_STAT_44) }, // pots            "
#else
    { static_cast<short>(sprite::SPR_STAT_44), block }, // Gibs!
#endif
    { static_cast<short>(sprite::SPR_STAT_45), wl_stat_t::block }, // stove           " (SOD:gibs)
    { static_cast<short>(sprite::SPR_STAT_46), wl_stat_t::block }, // spears          " (SOD:gibs)
    { static_cast<short>(sprite::SPR_STAT_47) },                   // vines           "
//
// NEW PAGE
//
#ifdef SPEAR
    { static_cast<short>(sprite::SPR_STAT_48), wl_stat_t::block },     // marble pillar
    { static_cast<short>(sprite::SPR_STAT_49), wl_stat_t::bo_25clip }, // bonus 25 clip
    { static_cast<short>(sprite::SPR_STAT_50), wl_stat_t::block },     // truck
    { static_cast<short>(sprite::SPR_STAT_51), wl_stat_t::bo_spear },  // SPEAR OF DESTINY!
#endif

    { static_cast<short>(sprite::SPR_STAT_26), wl_stat_t::bo_clip2 }, // Clip            "

    { -1 } // terminator
};

/*
===============
=
= InitStaticList
=
===============
*/

void InitStaticList(void)
{
    laststatobj = &statobjlist[0];
}

/*
===============
=
= SpawnStatic
=
===============
*/

void SpawnStatic(int tilex, int tiley, int type)
{
    laststatobj->shapenum = statinfo[type].picnum;
    laststatobj->tilex = tilex;
    laststatobj->tiley = tiley;
    laststatobj->visspot = &spotvis[tilex][tiley];

    switch (statinfo[type].type) {
    case wl_stat_t::block:
        actorat[tilex][tiley] = (objstruct*)64; // consider it a blocking tile
    case wl_stat_t::none:
        laststatobj->flags = 0;
        break;

    case wl_stat_t::bo_cross:
    case wl_stat_t::bo_chalice:
    case wl_stat_t::bo_bible:
    case wl_stat_t::bo_crown:
    case wl_stat_t::bo_fullheal:
        if (!loadedgame)
            gamestate.treasuretotal++;

    case wl_stat_t::bo_firstaid:
    case wl_stat_t::bo_key1:
    case wl_stat_t::bo_key2:
    case wl_stat_t::bo_key3:
    case wl_stat_t::bo_key4:
    case wl_stat_t::bo_clip:
    case wl_stat_t::bo_25clip:
    case wl_stat_t::bo_machinegun:
    case wl_stat_t::bo_chaingun:
    case wl_stat_t::bo_food:
    case wl_stat_t::bo_alpo:
    case wl_stat_t::bo_gibs:
    case wl_stat_t::bo_spear:
        laststatobj->flags = static_cast<uint32_t>(objflag_t::FL_BONUS);
        laststatobj->itemnumber = statinfo[type].type;
        break;
    }

    laststatobj->flags |= statinfo[type].specialFlags;

    laststatobj++;

    if (laststatobj == &statobjlist[MAXSTATS])
        Quit("Too many static objects!\n");
}

/*
===============
=
= PlaceItemType
=
= Called during game play to drop actors' items.  It finds the proper
= item number based on the item type (bo_???).  If there are no free item
= spots, nothing is done.
=
===============
*/

void PlaceItemType(wl_stat_t itemtype, int tilex, int tiley)
{
    int         type;
    statstruct* spot;

    //
    // find the item number
    //
    for (type = 0;; type++) {
        if (statinfo[type].picnum == -1) // end of list
            Quit("PlaceItemType: couldn't find type!");
        if (statinfo[type].type == itemtype)
            break;
    }

    //
    // find a spot in statobjlist to put it in
    //
    for (spot = &statobjlist[0];; spot++) {
        if (spot == laststatobj) {
            if (spot == &statobjlist[MAXSTATS])
                return;    // no free spots
            laststatobj++; // space at end
            break;
        }

        if (spot->shapenum == -1) // -1 is a free spot
            break;
    }
    //
    // place it
    //
    spot->shapenum = statinfo[type].picnum;
    spot->tilex = tilex;
    spot->tiley = tiley;
    spot->visspot = &spotvis[tilex][tiley];
    spot->flags = static_cast<std::uint32_t>(objflag_t::FL_BONUS) | statinfo[type].specialFlags;
    spot->itemnumber = statinfo[type].type;
}

/*
=============================================================================

                                  DOORS

doorobjlist[] holds most of the information for the doors

doorposition[] holds the amount the door is open, ranging from 0 to 0xffff
        this is directly accessed by AsmRefresh during rendering

The number of doors is limited to 64 because a spot in tilemap holds the
        door number in the low 6 bits, with the high bit meaning a door center
        and bit 6 meaning a door side tile

Open doors conect two areas, so sounds will travel between them and sight
        will be checked when the player is in a connected area.

Areaconnect is incremented/decremented by each door. If >0 they connect

Every time a door opens or closes the areabyplayer matrix gets recalculated.
        An area is true if it connects with the player's current spor.

=============================================================================
*/

#define DOORWIDTH 0x7800
#define OPENTICS  300

doorstruct doorobjlist[MAXDOORS], *lastdoorobj;
short      doornum;

word doorposition[MAXDOORS]; // leading edge of door 0=closed
                             // 0xffff = fully open

byte areaconnect[NUMAREAS][NUMAREAS];

bool areabyplayer[NUMAREAS];

/*
==============
=
= ConnectAreas
=
= Scans outward from playerarea, marking all connected areas
=
==============
*/

void RecursiveConnect(int areanumber)
{
    int i;

    for (i = 0; i < NUMAREAS; i++) {
        if (areaconnect[areanumber][i] && !areabyplayer[i]) {
            areabyplayer[i] = true;
            RecursiveConnect(i);
        }
    }
}

void ConnectAreas(void)
{
    memset(areabyplayer, 0, sizeof(areabyplayer));
    areabyplayer[player->areanumber] = true;
    RecursiveConnect(player->areanumber);
}

void InitAreas(void)
{
    memset(areabyplayer, 0, sizeof(areabyplayer));
    if (player->areanumber < NUMAREAS)
        areabyplayer[player->areanumber] = true;
}

/*
===============
=
= InitDoorList
=
===============
*/

void InitDoorList(void)
{
    memset(areabyplayer, 0, sizeof(areabyplayer));
    memset(areaconnect, 0, sizeof(areaconnect));

    lastdoorobj = &doorobjlist[0];
    doornum = 0;
}

/*
===============
=
= SpawnDoor
=
===============
*/

void SpawnDoor(int tilex, int tiley, bool vertical, door_t lock)
{
    word* map;

    if (doornum == MAXDOORS)
        Quit("64+ doors on level!");

    doorposition[doornum] = 0; // doors start out fully closed
    lastdoorobj->tilex = tilex;
    lastdoorobj->tiley = tiley;
    lastdoorobj->vertical = vertical;
    lastdoorobj->lock = lock;
    lastdoorobj->action = doortype::dr_closed;

    actorat[tilex][tiley] = (objstruct*)(uintptr_t)(doornum | 0x80); // consider it a solid wall

    //
    // make the door tile a special tile, and mark the adjacent tiles
    // for door sides
    //
    tilemap[tilex][tiley] = doornum | 0x80;
    map = mapsegs[0] + (tiley << mapshift) + tilex;
    if (vertical) {
        *map = *(map - 1); // set area number
        tilemap[tilex][tiley - 1] |= 0x40;
        tilemap[tilex][tiley + 1] |= 0x40;
    } else {
        *map = *(map - mapwidth); // set area number
        tilemap[tilex - 1][tiley] |= 0x40;
        tilemap[tilex + 1][tiley] |= 0x40;
    }

    doornum++;
    lastdoorobj++;
}

//===========================================================================

/*
=====================
=
= OpenDoor
=
=====================
*/

void OpenDoor(int door)
{
    if (doorobjlist[door].action == doortype::dr_open)
        doorobjlist[door].ticcount = 0; // reset open time
    else
        doorobjlist[door].action = doortype::dr_opening; // start it opening
}

/*
=====================
=
= CloseDoor
=
=====================
*/

void CloseDoor(int door)
{
    int        tilex, tiley, area;
    objstruct* check;

    //
    // don't close on anything solid
    //
    tilex = doorobjlist[door].tilex;
    tiley = doorobjlist[door].tiley;

    if (actorat[tilex][tiley])
        return;

    if (player->tilex == tilex && player->tiley == tiley)
        return;

    if (doorobjlist[door].vertical) {
        if (player->tiley == tiley) {
            if (((player->x + MINDIST) >> TILESHIFT) == tilex)
                return;
            if (((player->x - MINDIST) >> TILESHIFT) == tilex)
                return;
        }
        check = actorat[tilex - 1][tiley];
        if (ISPOINTER(check) && ((check->x + MINDIST) >> TILESHIFT) == tilex)
            return;
        check = actorat[tilex + 1][tiley];
        if (ISPOINTER(check) && ((check->x - MINDIST) >> TILESHIFT) == tilex)
            return;
    } else if (!doorobjlist[door].vertical) {
        if (player->tilex == tilex) {
            if (((player->y + MINDIST) >> TILESHIFT) == tiley)
                return;
            if (((player->y - MINDIST) >> TILESHIFT) == tiley)
                return;
        }
        check = actorat[tilex][tiley - 1];
        if (ISPOINTER(check) && ((check->y + MINDIST) >> TILESHIFT) == tiley)
            return;
        check = actorat[tilex][tiley + 1];
        if (ISPOINTER(check) && ((check->y - MINDIST) >> TILESHIFT) == tiley)
            return;
    }

    //
    // play door sound if in a connected area
    //
    area = *(mapsegs[0] + (doorobjlist[door].tiley << mapshift)
               + doorobjlist[door].tilex)
        - AREATILE;
    if (areabyplayer[area]) {
        PlaySoundLocTile(CLOSEDOORSND, doorobjlist[door].tilex, doorobjlist[door].tiley); // JAB
    }

    doorobjlist[door].action = doortype::dr_closing;
    //
    // make the door space solid
    //
    actorat[tilex][tiley] = (objstruct*)(uintptr_t)(door | 0x80);
}

/*
=====================
=
= OperateDoor
=
= The player wants to change the door's direction
=
=====================
*/

void OperateDoor(int door)
{
    byte lock = static_cast<byte>(doorobjlist[door].lock);

    if (lock >= static_cast<byte>(door_t::dr_lock1) && lock <= static_cast<byte>(door_t::dr_lock4)) {
        if (!(gamestate.keys & (1 << (lock - static_cast<byte>(door_t::dr_lock1))))) {

            SD_PlaySound(NOWAYSND); // locked
            return;
        }
    }

    switch (doorobjlist[door].action) {
    case doortype::dr_closed:
    case doortype::dr_closing:
        OpenDoor(door);
        break;
    case doortype::dr_open:
    case doortype::dr_opening:
        CloseDoor(door);
        break;
    }
}

//===========================================================================

/*
===============
=
= DoorOpen
=
= Close the door after three seconds
=
===============
*/

void DoorOpen(int door)
{
    if ((doorobjlist[door].ticcount += (short)tics) >= OPENTICS)
        CloseDoor(door);
}

/*
===============
=
= DoorOpening
=
===============
*/

void DoorOpening(int door)
{
    unsigned area1, area2;
    word*    map;
    int32_t  position;

    position = doorposition[door];
    if (!position) {
        //
        // door is just starting to open, so connect the areas
        //
        map = mapsegs[0] + (doorobjlist[door].tiley << mapshift)
            + doorobjlist[door].tilex;

        if (doorobjlist[door].vertical) {
            area1 = *(map + 1);
            area2 = *(map - 1);
        } else {
            area1 = *(map - mapwidth);
            area2 = *(map + mapwidth);
        }
        area1 -= AREATILE;
        area2 -= AREATILE;

        if (area1 < NUMAREAS && area2 < NUMAREAS) {
            areaconnect[area1][area2]++;
            areaconnect[area2][area1]++;

            if (player->areanumber < NUMAREAS)
                ConnectAreas();

            if (areabyplayer[area1])
                PlaySoundLocTile(OPENDOORSND, doorobjlist[door].tilex, doorobjlist[door].tiley); // JAB
        }
    }

    //
    // slide the door by an adaptive amount
    //
    position += tics << 10;
    if (position >= 0xffff) {
        //
        // door is all the way open
        //
        position = 0xffff;
        doorobjlist[door].ticcount = 0;
        doorobjlist[door].action = doortype::dr_open;
        actorat[doorobjlist[door].tilex][doorobjlist[door].tiley] = 0;
    }

    doorposition[door] = (word)position;
}

/*
===============
=
= DoorClosing
=
===============
*/

void DoorClosing(int door)
{
    unsigned area1, area2;
    word*    map;
    int32_t  position;
    int      tilex, tiley;

    tilex = doorobjlist[door].tilex;
    tiley = doorobjlist[door].tiley;

    if (((int)(uintptr_t)actorat[tilex][tiley] != (door | 0x80))
        || (player->tilex == tilex && player->tiley == tiley)) { // something got inside the door
        OpenDoor(door);
        return;
    }

    position = doorposition[door];

    //
    // slide the door by an adaptive amount
    //
    position -= tics << 10;
    if (position <= 0) {
        //
        // door is closed all the way, so disconnect the areas
        //
        position = 0;

        doorobjlist[door].action = doortype::dr_closed;

        map = mapsegs[0] + (doorobjlist[door].tiley << mapshift) + doorobjlist[door].tilex;

        if (doorobjlist[door].vertical) {
            area1 = *(map + 1);
            area2 = *(map - 1);
        } else {
            area1 = *(map - mapwidth);
            area2 = *(map + mapwidth);
        }
        area1 -= AREATILE;
        area2 -= AREATILE;

        if (area1 < NUMAREAS && area2 < NUMAREAS) {
            areaconnect[area1][area2]--;
            areaconnect[area2][area1]--;

            if (player->areanumber < NUMAREAS)
                ConnectAreas();
        }
    }

    doorposition[door] = (word)position;
}

/*
=====================
=
= MoveDoors
=
= Called from PlayLoop
=
=====================
*/

void MoveDoors(void)
{
    int door;

    if (gamestate.victoryflag) // don't move door during victory sequence
        return;

    for (door = 0; door < doornum; door++) {
        switch (doorobjlist[door].action) {
        case doortype::dr_open:
            DoorOpen(door);
            break;

        case doortype::dr_opening:
            DoorOpening(door);
            break;

        case doortype::dr_closing:
            DoorClosing(door);
            break;
        }
    }
}

/*
=============================================================================

                                PUSHABLE WALLS

=============================================================================
*/

word         pwallstate;
word         pwallpos; // amount a pushable wall has been moved (0-63)
word         pwallx, pwally;
controldir_t pwalldir;
byte         pwalltile;
int          dirs[4][2] = { { 0, -1 }, { 1, 0 }, { 0, 1 }, { -1, 0 } };

/*
===============
=
= PushWall
=
===============
*/

void PushWall(int checkx, int checky, controldir_t dir)
{
    int oldtile, dx, dy;

    if (pwallstate)
        return;

    oldtile = tilemap[checkx][checky];
    if (!oldtile)
        return;

    dx = dirs[static_cast<byte>(dir)][0];
    dy = dirs[static_cast<byte>(dir)][1];

    if (actorat[checkx + dx][checky + dy]) {
        SD_PlaySound(NOWAYSND);
        return;
    }
    actorat[checkx + dx][checky + dy] = (objstruct*)(uintptr_t)(tilemap[checkx + dx][checky + dy] = oldtile);

    gamestate.secretcount++;
    pwallx = checkx;
    pwally = checky;
    pwalldir = dir;
    pwallstate = 1;
    pwallpos = 0;
    pwalltile = tilemap[pwallx][pwally];
    tilemap[pwallx][pwally] = 64;
    tilemap[pwallx + dx][pwally + dy] = 64;
    *(mapsegs[1] + (pwally << mapshift) + pwallx) = 0;                                                           // remove P tile info
    *(mapsegs[0] + (pwally << mapshift) + pwallx) = *(mapsegs[0] + (player->tiley << mapshift) + player->tilex); // set correct floorcode (BrotherTank's fix)

    SD_PlaySound(PUSHWALLSND);
}

/*
=================
=
= MovePWalls
=
=================
*/

void MovePWalls(void)
{
    int oldblock, oldtile;

    if (!pwallstate)
        return;

    oldblock = pwallstate / 128;

    pwallstate += (word)tics;

    if (pwallstate / 128 != oldblock) {
        // block crossed into a new block
        oldtile = pwalltile;

        //
        // the tile can now be walked into
        //
        tilemap[pwallx][pwally] = 0;
        actorat[pwallx][pwally] = 0;
        *(mapsegs[0] + (pwally << mapshift) + pwallx) = player->areanumber + AREATILE;

        int dx = dirs[static_cast<byte>(pwalldir)][0], dy = dirs[static_cast<byte>(pwalldir)][1];
        //
        // see if it should be pushed farther
        //
        if (pwallstate >= 256) // only move two tiles fix
        {
            //
            // the block has been pushed two tiles
            //
            pwallstate = 0;
            tilemap[pwallx + dx][pwally + dy] = oldtile;
            return;
        } else {
            int xl, yl, xh, yh;
            xl = (player->x - PLAYERSIZE) >> TILESHIFT;
            yl = (player->y - PLAYERSIZE) >> TILESHIFT;
            xh = (player->x + PLAYERSIZE) >> TILESHIFT;
            yh = (player->y + PLAYERSIZE) >> TILESHIFT;

            pwallx += dx;
            pwally += dy;

            if (actorat[pwallx + dx][pwally + dy]
                || xl <= pwallx + dx && pwallx + dx <= xh && yl <= pwally + dy && pwally + dy <= yh) {
                pwallstate = 0;
                tilemap[pwallx][pwally] = oldtile;
                return;
            }
            actorat[pwallx + dx][pwally + dy] = (objstruct*)(uintptr_t)(tilemap[pwallx + dx][pwally + dy] = oldtile);
            tilemap[pwallx + dx][pwally + dy] = 64;
        }
    }

    pwallpos = (pwallstate / 2) & 63;
}
