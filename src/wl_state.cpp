// WL_STATE.C

#include "wl_def.h"
#pragma hdrstop

/*
=============================================================================

                            LOCAL CONSTANTS

=============================================================================
*/

/*
=============================================================================

                            GLOBAL VARIABLES

=============================================================================
*/

static const dirtype opposite[9] = { dirtype::west, dirtype::southwest, dirtype::south, dirtype::southeast, dirtype::east, dirtype::northeast, dirtype::north, dirtype::northwest, dirtype::nodir };

static const dirtype diagonal[9][9] = {
    /* dirtype::east */ { dirtype::nodir, dirtype::nodir, dirtype::northeast, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::southeast, dirtype::nodir, dirtype::nodir },
    { dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir },
    /* dirtype::north */ { dirtype::northeast, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::northwest, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir },
    { dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir },
    /* dirtype::west */ { dirtype::nodir, dirtype::nodir, dirtype::northwest, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::southwest, dirtype::nodir, dirtype::nodir },
    { dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir },
    /* dirtype::south */ { dirtype::southeast, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::southwest, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir },
    { dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir },
    { dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir, dirtype::nodir }
};

void SpawnNewObj(unsigned tilex, unsigned tiley, statetype* state);
void NewState(objstruct* ob, statetype* state);

boolean TryWalk(objstruct* ob);
void    MoveObj(objstruct* ob, int32_t move);

void KillActor(objstruct* ob);
void DamageActor(objstruct* ob, unsigned damage);

boolean CheckLine(objstruct* ob);
void    FirstSighting(objstruct* ob);
boolean CheckSight(objstruct* ob);

/*
=============================================================================

                                LOCAL VARIABLES

=============================================================================
*/

//===========================================================================

/*
===================
=
= SpawnNewObj
=
= Spaws a new actor at the given TILE coordinates, with the given state, and
= the given size in GLOBAL units.
=
= newobj = a pointer to an initialized new actor
=
===================
*/

void SpawnNewObj(unsigned tilex, unsigned tiley, statetype* state)
{
    GetNewActor();
    newobj->state = state;
    if (state->tictime)
        newobj->ticcount = DEMOCHOOSE_ORIG_SDL(
            US_RndT() % state->tictime,
            US_RndT() % state->tictime + 1); // Chris' moonwalk bugfix ;D
    else
        newobj->ticcount = 0;

    newobj->tilex = (short)tilex;
    newobj->tiley = (short)tiley;
    newobj->x = ((int32_t)tilex << TILESHIFT) + TILEGLOBAL / 2;
    newobj->y = ((int32_t)tiley << TILESHIFT) + TILEGLOBAL / 2;
    newobj->dir = dirtype::nodir;

    actorat[tilex][tiley] = newobj;
    newobj->areanumber = *(mapsegs[0] + (newobj->tiley << mapshift) + newobj->tilex) - AREATILE;
}

/*
===================
=
= NewState
=
= Changes ob to a new state, setting ticcount to the max for that state
=
===================
*/

void NewState(objstruct* ob, statetype* state)
{
    ob->state = state;
    ob->ticcount = state->tictime;
}

/*
=============================================================================

                        ENEMY TILE WORLD MOVEMENT CODE

=============================================================================
*/

/*
==================================
=
= TryWalk
=
= Attempts to move ob in its current (ob->dir) direction.
=
= If blocked by either a wall or an actor returns FALSE
=
= If move is either clear or blocked only by a door, returns TRUE and sets
=
= ob->tilex         = new destination
= ob->tiley
= ob->areanumber    = the floor tile number (0-(NUMAREAS-1)) of destination
= ob->distance      = TILEGLOBAl, or -doornumber if a door is blocking the way
=
= If a door is in the way, an OpenDoor call is made to start it opening.
= The actor code should wait until
=       doorobjlist[-ob->distance].action = dr_open, meaning the door has been
=       fully opened
=
==================================
*/

#define CHECKDIAG(x, y)                                                                          \
    {                                                                                            \
        temp = (uintptr_t)actorat[x][y];                                                         \
        if (temp) {                                                                              \
            if (temp < 256)                                                                      \
                return false;                                                                    \
            if (((objstruct*)temp)->flags & static_cast<std::uint32_t>(objflag_t::FL_SHOOTABLE)) \
                return false;                                                                    \
        }                                                                                        \
    }

#ifdef PLAYDEMOLIKEORIGINAL
#define DOORCHECK                    \
    if (DEMOCOND_ORIG)               \
        doornum = temp & 63;         \
    else {                           \
        doornum = (int)temp & 127;   \
        OpenDoor(doornum);           \
        ob->distance = -doornum - 1; \
        return true;                 \
    }
#else
#define DOORCHECK                \
    doornum = (int)temp & 127;   \
    OpenDoor(doornum);           \
    ob->distance = -doornum - 1; \
    return true;
#endif

#define CHECKSIDE(x, y)                                                                                 \
    {                                                                                                   \
        temp = (uintptr_t)actorat[x][y];                                                                \
        if (temp) {                                                                                     \
            if (temp < 128)                                                                             \
                return false;                                                                           \
            if (temp < 256) {                                                                           \
                DOORCHECK                                                                               \
            } else if (((objstruct*)temp)->flags & static_cast<std::uint32_t>(objflag_t::FL_SHOOTABLE)) \
                return false;                                                                           \
        }                                                                                               \
    }

boolean TryWalk(objstruct* ob)
{
    int       doornum = -1;
    uintptr_t temp;

    if (ob->obclass == classtype::inertobj) {
        switch (ob->dir) {
        case dirtype::north:
            ob->tiley--;
            break;

        case dirtype::northeast:
            ob->tilex++;
            ob->tiley--;
            break;

        case dirtype::east:
            ob->tilex++;
            break;

        case dirtype::southeast:
            ob->tilex++;
            ob->tiley++;
            break;

        case dirtype::south:
            ob->tiley++;
            break;

        case dirtype::southwest:
            ob->tilex--;
            ob->tiley++;
            break;

        case dirtype::west:
            ob->tilex--;
            break;

        case dirtype::northwest:
            ob->tilex--;
            ob->tiley--;
            break;
        }
    } else {
        switch (ob->dir) {
        case dirtype::north:
            if (ob->obclass == classtype::dogobj || ob->obclass == classtype::fakeobj
                || ob->obclass == classtype::ghostobj || ob->obclass == classtype::spectreobj) {
                CHECKDIAG(ob->tilex, ob->tiley - 1);
            } else {
                CHECKSIDE(ob->tilex, ob->tiley - 1);
            }
            ob->tiley--;
            break;

        case dirtype::northeast:
            CHECKDIAG(ob->tilex + 1, ob->tiley - 1);
            CHECKDIAG(ob->tilex + 1, ob->tiley);
            CHECKDIAG(ob->tilex, ob->tiley - 1);
            ob->tilex++;
            ob->tiley--;
            break;

        case dirtype::east:
            if (ob->obclass == classtype::dogobj || ob->obclass == classtype::fakeobj
                || ob->obclass == classtype::ghostobj || ob->obclass == classtype::spectreobj) {
                CHECKDIAG(ob->tilex + 1, ob->tiley);
            } else {
                CHECKSIDE(ob->tilex + 1, ob->tiley);
            }
            ob->tilex++;
            break;

        case dirtype::southeast:
            CHECKDIAG(ob->tilex + 1, ob->tiley + 1);
            CHECKDIAG(ob->tilex + 1, ob->tiley);
            CHECKDIAG(ob->tilex, ob->tiley + 1);
            ob->tilex++;
            ob->tiley++;
            break;

        case dirtype::south:
            if (ob->obclass == classtype::dogobj || ob->obclass == classtype::fakeobj
                || ob->obclass == classtype::ghostobj || ob->obclass == classtype::spectreobj) {
                CHECKDIAG(ob->tilex, ob->tiley + 1);
            } else {
                CHECKSIDE(ob->tilex, ob->tiley + 1);
            }
            ob->tiley++;
            break;

        case dirtype::southwest:
            CHECKDIAG(ob->tilex - 1, ob->tiley + 1);
            CHECKDIAG(ob->tilex - 1, ob->tiley);
            CHECKDIAG(ob->tilex, ob->tiley + 1);
            ob->tilex--;
            ob->tiley++;
            break;

        case dirtype::west:
            if (ob->obclass == classtype::dogobj || ob->obclass == classtype::fakeobj
                || ob->obclass == classtype::ghostobj || ob->obclass == classtype::spectreobj) {
                CHECKDIAG(ob->tilex - 1, ob->tiley);
            } else {
                CHECKSIDE(ob->tilex - 1, ob->tiley);
            }
            ob->tilex--;
            break;

        case dirtype::northwest:
            CHECKDIAG(ob->tilex - 1, ob->tiley - 1);
            CHECKDIAG(ob->tilex - 1, ob->tiley);
            CHECKDIAG(ob->tilex, ob->tiley - 1);
            ob->tilex--;
            ob->tiley--;
            break;

        case dirtype::nodir:
            return false;

        default:
            Quit("Walk: Bad dir");
        }
    }

#ifdef PLAYDEMOLIKEORIGINAL
    if (DEMOCOND_ORIG && doornum != -1) {
        OpenDoor(doornum);
        ob->distance = -doornum - 1;
        return true;
    }
#endif

    ob->areanumber = *(mapsegs[0] + (ob->tiley << mapshift) + ob->tilex) - AREATILE;

    ob->distance = TILEGLOBAL;
    return true;
}

/*
==================================
=
= SelectDodgeDir
=
= Attempts to choose and initiate a movement for ob that sends it towards
= the player while dodging
=
= If there is no possible move (ob is totally surrounded)
=
= ob->dir           =      dirtype::nodir
=
= Otherwise
=
= ob->dir           = new direction to follow
= ob->distance      = TILEGLOBAL or -doornumber
= ob->tilex         = new destination
= ob->tiley
= ob->areanumber    = the floor tile number (0-(NUMAREAS-1)) of destination
=
==================================
*/

void SelectDodgeDir(objstruct* ob)
{
    int      deltax, deltay, i;
    unsigned absdx, absdy;
    dirtype  dirtry[5];
    dirtype  turnaround, tdir;

    if (ob->flags & static_cast<std::uint32_t>(objflag_t::FL_FIRSTATTACK)) {
        //
        // turning around is only ok the very first time after noticing the
        // player
        //
        turnaround = dirtype::nodir;
        ob->flags &= ~static_cast<std::uint32_t>(objflag_t::FL_FIRSTATTACK);
    } else
        turnaround = opposite[static_cast<byte>(ob->dir)];

    deltax = player->tilex - ob->tilex;
    deltay = player->tiley - ob->tiley;

    //
    // arange 5 direction choices in order of preference
    // the four cardinal directions plus the diagonal straight towards
    // the player
    //

    if (deltax > 0) {
        dirtry[1] = dirtype::east;
        dirtry[3] = dirtype::west;
    } else {
        dirtry[1] = dirtype::west;
        dirtry[3] = dirtype::east;
    }

    if (deltay > 0) {
        dirtry[2] = dirtype::south;
        dirtry[4] = dirtype::north;
    } else {
        dirtry[2] = dirtype::north;
        dirtry[4] = dirtype::south;
    }

    //
    // randomize a bit for dodging
    //
    absdx = abs(deltax);
    absdy = abs(deltay);

    if (absdx > absdy) {
        tdir = dirtry[1];
        dirtry[1] = dirtry[2];
        dirtry[2] = tdir;
        tdir = dirtry[3];
        dirtry[3] = dirtry[4];
        dirtry[4] = tdir;
    }

    if (US_RndT() < 128) {
        tdir = dirtry[1];
        dirtry[1] = dirtry[2];
        dirtry[2] = tdir;
        tdir = dirtry[3];
        dirtry[3] = dirtry[4];
        dirtry[4] = tdir;
    }

    dirtry[0] = diagonal[static_cast<byte>(dirtry[1])][static_cast<byte>(dirtry[2])];

    //
    // try the directions util one works
    //
    for (i = 0; i < 5; i++) {
        if (dirtry[i] == dirtype::nodir || dirtry[i] == turnaround)
            continue;

        ob->dir = dirtry[i];
        if (TryWalk(ob))
            return;
    }

    //
    // turn around only as a last resort
    //
    if (turnaround != dirtype::nodir) {
        ob->dir = turnaround;

        if (TryWalk(ob))
            return;
    }

    ob->dir = dirtype::nodir;
}

/*
============================
=
= SelectChaseDir
=
= As SelectDodgeDir, but doesn't try to dodge
=
============================
*/

void SelectChaseDir(objstruct* ob)
{
    int     deltax, deltay;
    dirtype d[3];
    dirtype tdir, olddir, turnaround;

    olddir = ob->dir;
    turnaround = opposite[static_cast<byte>(olddir)];

    deltax = player->tilex - ob->tilex;
    deltay = player->tiley - ob->tiley;

    d[1] = dirtype::nodir;
    d[2] = dirtype::nodir;

    if (deltax > 0)
        d[1] = dirtype::east;
    else if (deltax < 0)
        d[1] = dirtype::west;
    if (deltay > 0)
        d[2] = dirtype::south;
    else if (deltay < 0)
        d[2] = dirtype::north;

    if (abs(deltay) > abs(deltax)) {
        tdir = d[1];
        d[1] = d[2];
        d[2] = tdir;
    }

    if (d[1] == turnaround)
        d[1] = dirtype::nodir;
    if (d[2] == turnaround)
        d[2] = dirtype::nodir;

    if (d[1] != dirtype::nodir) {
        ob->dir = d[1];
        if (TryWalk(ob))
            return; /*either moved forward or attacked*/
    }

    if (d[2] != dirtype::nodir) {
        ob->dir = d[2];
        if (TryWalk(ob))
            return;
    }

    /* there is no direct path to the player, so pick another direction */

    if (olddir != dirtype::nodir) {
        ob->dir = olddir;
        if (TryWalk(ob))
            return;
    }

    if (US_RndT() > 128) /*randomly determine direction of search*/
    {
        for (tdir = dirtype::north; tdir <= dirtype::west; tdir = (dirtype)(static_cast<byte>(tdir) + 1)) {
            if (tdir != turnaround) {
                ob->dir = tdir;
                if (TryWalk(ob))
                    return;
            }
        }
    } else {
        for (tdir = dirtype::west; tdir >= dirtype::north; tdir = (dirtype)(static_cast<byte>(tdir) - 1)) {
            if (tdir != turnaround) {
                ob->dir = tdir;
                if (TryWalk(ob))
                    return;
            }
        }
    }

    if (turnaround != dirtype::nodir) {
        ob->dir = turnaround;
        if (ob->dir != dirtype::nodir) {
            if (TryWalk(ob))
                return;
        }
    }

    ob->dir = dirtype::nodir; // can't move
}

/*
============================
=
= SelectRunDir
=
= Run Away from player
=
============================
*/

void SelectRunDir(objstruct* ob)
{
    int     deltax, deltay;
    dirtype d[3];
    dirtype tdir;

    deltax = player->tilex - ob->tilex;
    deltay = player->tiley - ob->tiley;

    if (deltax < 0)
        d[1] = dirtype::east;
    else
        d[1] = dirtype::west;
    if (deltay < 0)
        d[2] = dirtype::south;
    else
        d[2] = dirtype::north;

    if (abs(deltay) > abs(deltax)) {
        tdir = d[1];
        d[1] = d[2];
        d[2] = tdir;
    }

    ob->dir = d[1];
    if (TryWalk(ob))
        return; /*either moved forward or attacked*/

    ob->dir = d[2];
    if (TryWalk(ob))
        return;

    /* there is no direct path to the player, so pick another direction */

    if (US_RndT() > 128) /*randomly determine direction of search*/
    {
        for (tdir = dirtype::north; tdir <= dirtype::west; tdir = (dirtype)(static_cast<byte>(tdir) + 1)) {
            ob->dir = tdir;
            if (TryWalk(ob))
                return;
        }
    } else {
        for (tdir = dirtype::west; tdir >= dirtype::north; tdir = (dirtype)(static_cast<byte>(tdir) - 1)) {
            ob->dir = tdir;
            if (TryWalk(ob))
                return;
        }
    }

    ob->dir = dirtype::nodir; // can't move
}

/*
=================
=
= MoveObj
=
= Moves ob be move global units in ob->dir direction
= Actors are not allowed to move inside the player
= Does NOT check to see if the move is tile map valid
=
= ob->x                 = adjusted for new position
= ob->y
=
=================
*/

void MoveObj(objstruct* ob, int32_t move)
{
    int32_t deltax, deltay;

    switch (ob->dir) {
    case dirtype::north:
        ob->y -= move;
        break;
    case dirtype::northeast:
        ob->x += move;
        ob->y -= move;
        break;
    case dirtype::east:
        ob->x += move;
        break;
    case dirtype::southeast:
        ob->x += move;
        ob->y += move;
        break;
    case dirtype::south:
        ob->y += move;
        break;
    case dirtype::southwest:
        ob->x -= move;
        ob->y += move;
        break;
    case dirtype::west:
        ob->x -= move;
        break;
    case dirtype::northwest:
        ob->x -= move;
        ob->y -= move;
        break;

    case dirtype::nodir:
        return;

    default:
        Quit("MoveObj: bad dir!");
    }

    //
    // check to make sure it's not on top of player
    //
    if (areabyplayer[ob->areanumber]) {
        deltax = ob->x - player->x;
        if (deltax < -MINACTORDIST || deltax > MINACTORDIST)
            goto moveok;
        deltay = ob->y - player->y;
        if (deltay < -MINACTORDIST || deltay > MINACTORDIST)
            goto moveok;

        if (ob->hidden) // move closer until he meets CheckLine
            goto moveok;

        if (ob->obclass == classtype::ghostobj || ob->obclass == classtype::spectreobj)
            TakeDamage(tics * 2, ob);

        //
        // back up
        //
        switch (ob->dir) {
        case dirtype::north:
            ob->y += move;
            break;
        case dirtype::northeast:
            ob->x -= move;
            ob->y += move;
            break;
        case dirtype::east:
            ob->x -= move;
            break;
        case dirtype::southeast:
            ob->x -= move;
            ob->y -= move;
            break;
        case dirtype::south:
            ob->y -= move;
            break;
        case dirtype::southwest:
            ob->x += move;
            ob->y -= move;
            break;
        case dirtype::west:
            ob->x += move;
            break;
        case dirtype::northwest:
            ob->x += move;
            ob->y += move;
            break;

        case dirtype::nodir:
            return;
        }
        return;
    }
moveok:
    ob->distance -= move;
}

/*
=============================================================================

                                STUFF

=============================================================================
*/

/*
===============
=
= DropItem
=
= Tries to drop a bonus item somewhere in the tiles surrounding the
= given tilex/tiley
=
===============
*/

void DropItem(wl_stat_t itemtype, int tilex, int tiley)
{
    int x, y, xl, xh, yl, yh;

    //
    // find a free spot to put it in
    //
    if (!actorat[tilex][tiley]) {
        PlaceItemType(itemtype, tilex, tiley);
        return;
    }

    xl = tilex - 1;
    xh = tilex + 1;
    yl = tiley - 1;
    yh = tiley + 1;

    for (x = xl; x <= xh; x++) {
        for (y = yl; y <= yh; y++) {
            if (!actorat[x][y]) {
                PlaceItemType(itemtype, x, y);
                return;
            }
        }
    }
}

/*
===============
=
= KillActor
=
===============
*/

void KillActor(objstruct* ob)
{
    int tilex, tiley;

    tilex = ob->tilex = (word)(ob->x >> TILESHIFT); // drop item on center
    tiley = ob->tiley = (word)(ob->y >> TILESHIFT);

    switch (ob->obclass) {
    case classtype::guardobj:
        GivePoints(100);
        NewState(ob, &s_grddie1);
        PlaceItemType(wl_stat_t::bo_clip2, tilex, tiley);
        break;

    case classtype::officerobj:
        GivePoints(400);
        NewState(ob, &s_ofcdie1);
        PlaceItemType(wl_stat_t::bo_clip2, tilex, tiley);
        break;

    case classtype::mutantobj:
        GivePoints(700);
        NewState(ob, &s_mutdie1);
        PlaceItemType(wl_stat_t::bo_clip2, tilex, tiley);
        break;

    case classtype::ssobj:
        GivePoints(500);
        NewState(ob, &s_ssdie1);
        if (static_cast<byte>(gamestate.bestweapon) < static_cast<byte>(weapontype::wp_machinegun))
            PlaceItemType(wl_stat_t::bo_machinegun, tilex, tiley);
        else
            PlaceItemType(wl_stat_t::bo_clip2, tilex, tiley);
        break;

    case classtype::dogobj:
        GivePoints(200);
        NewState(ob, &s_dogdie1);
        break;

#ifndef SPEAR
    case classtype::bossobj:
        GivePoints(5000);
        NewState(ob, &s_bossdie1);
        PlaceItemType(wl_stat_t::bo_key1, tilex, tiley);
        break;

    case classtype::gretelobj:
        GivePoints(5000);
        NewState(ob, &s_greteldie1);
        PlaceItemType(wl_stat_t::bo_key1, tilex, tiley);
        break;

    case classtype::giftobj:
        GivePoints(5000);
        gamestate.killx = player->x;
        gamestate.killy = player->y;
        NewState(ob, &s_giftdie1);
        break;

    case classtype::fatobj:
        GivePoints(5000);
        gamestate.killx = player->x;
        gamestate.killy = player->y;
        NewState(ob, &s_fatdie1);
        break;

    case classtype::schabbobj:
        GivePoints(5000);
        gamestate.killx = player->x;
        gamestate.killy = player->y;
        NewState(ob, &s_schabbdie1);
        break;
    case classtype::fakeobj:
        GivePoints(2000);
        NewState(ob, &s_fakedie1);
        break;

    case classtype::mechahitlerobj:
        GivePoints(5000);
        NewState(ob, &s_mechadie1);
        break;
    case classtype::realhitlerobj:
        GivePoints(5000);
        gamestate.killx = player->x;
        gamestate.killy = player->y;
        NewState(ob, &s_hitlerdie1);
        break;
#else
    case classtype::spectreobj:
        if (ob->flags & static_cast<std::uint32_t>(objflag_t::FL_BONUS)) {
            GivePoints(200); // Get points once for each
            ob->flags &= ~static_cast<std::uint32_t>(objflag_t::FL_BONUS);
        }
        NewState(ob, &s_spectredie1);
        break;

    case angelobj:
        GivePoints(5000);
        NewState(ob, &s_angeldie1);
        break;

    case transobj:
        GivePoints(5000);
        NewState(ob, &s_transdie0);
        PlaceItemType(bo_key1, tilex, tiley);
        break;

    case uberobj:
        GivePoints(5000);
        NewState(ob, &s_uberdie0);
        PlaceItemType(bo_key1, tilex, tiley);
        break;

    case willobj:
        GivePoints(5000);
        NewState(ob, &s_willdie1);
        PlaceItemType(bo_key1, tilex, tiley);
        break;

    case deathobj:
        GivePoints(5000);
        NewState(ob, &s_deathdie1);
        PlaceItemType(bo_key1, tilex, tiley);
        break;
#endif
    }

    gamestate.killcount++;
    ob->flags &= ~static_cast<std::uint32_t>(objflag_t::FL_SHOOTABLE);
    actorat[ob->tilex][ob->tiley] = NULL;
    ob->flags |= static_cast<std::uint32_t>(objflag_t::FL_NONMARK);
}

/*
===================
=
= DamageActor
=
= Called when the player succesfully hits an enemy.
=
= Does damage points to enemy ob, either putting it into a stun frame or
= killing it.
=
===================
*/

void DamageActor(objstruct* ob, unsigned damage)
{
    madenoise = true;

    //
    // do double damage if shooting a non attack mode actor
    //
    if (!(ob->flags & static_cast<std::uint32_t>(objflag_t::FL_ATTACKMODE)))
        damage <<= 1;

    ob->hitpoints -= (short)damage;

    if (ob->hitpoints <= 0)
        KillActor(ob);
    else {
        if (!(ob->flags & static_cast<std::uint32_t>(objflag_t::FL_ATTACKMODE)))
            FirstSighting(ob); // put into combat mode

        switch (ob->obclass) // dogs only have one hit point
        {
        case classtype::guardobj:
            if (ob->hitpoints & 1)
                NewState(ob, &s_grdpain);
            else
                NewState(ob, &s_grdpain1);
            break;

        case classtype::officerobj:
            if (ob->hitpoints & 1)
                NewState(ob, &s_ofcpain);
            else
                NewState(ob, &s_ofcpain1);
            break;

        case classtype::mutantobj:
            if (ob->hitpoints & 1)
                NewState(ob, &s_mutpain);
            else
                NewState(ob, &s_mutpain1);
            break;

        case classtype::ssobj:
            if (ob->hitpoints & 1)
                NewState(ob, &s_sspain);
            else
                NewState(ob, &s_sspain1);

            break;
        }
    }
}

/*
=============================================================================

                                CHECKSIGHT

=============================================================================
*/

/*
=====================
=
= CheckLine
=
= Returns true if a straight line between the player and ob is unobstructed
=
=====================
*/

boolean CheckLine(objstruct* ob)
{
    int      x1, y1, xt1, yt1, x2, y2, xt2, yt2;
    int      x, y;
    int      xdist, ydist, xstep, ystep;
    int      partial, delta;
    int32_t  ltemp;
    int      xfrac, yfrac, deltafrac;
    unsigned value, intercept;

    x1 = ob->x >> UNSIGNEDSHIFT; // 1/256 tile precision
    y1 = ob->y >> UNSIGNEDSHIFT;
    xt1 = x1 >> 8;
    yt1 = y1 >> 8;

    x2 = plux;
    y2 = pluy;
    xt2 = player->tilex;
    yt2 = player->tiley;

    xdist = abs(xt2 - xt1);

    if (xdist > 0) {
        if (xt2 > xt1) {
            partial = 256 - (x1 & 0xff);
            xstep = 1;
        } else {
            partial = x1 & 0xff;
            xstep = -1;
        }

        deltafrac = abs(x2 - x1);
        delta = y2 - y1;
        ltemp = ((int32_t)delta << 8) / deltafrac;
        if (ltemp > 0x7fffl)
            ystep = 0x7fff;
        else if (ltemp < -0x7fffl)
            ystep = -0x7fff;
        else
            ystep = ltemp;
        yfrac = y1 + (((int32_t)ystep * partial) >> 8);

        x = xt1 + xstep;
        xt2 += xstep;
        do {
            y = yfrac >> 8;
            yfrac += ystep;

            value = (unsigned)tilemap[x][y];
            x += xstep;

            if (!value)
                continue;

            if (value < 128 || value > 256)
                return false;

            //
            // see if the door is open enough
            //
            value &= ~0x80;
            intercept = yfrac - ystep / 2;

            if (intercept > doorposition[value])
                return false;

        } while (x != xt2);
    }

    ydist = abs(yt2 - yt1);

    if (ydist > 0) {
        if (yt2 > yt1) {
            partial = 256 - (y1 & 0xff);
            ystep = 1;
        } else {
            partial = y1 & 0xff;
            ystep = -1;
        }

        deltafrac = abs(y2 - y1);
        delta = x2 - x1;
        ltemp = ((int32_t)delta << 8) / deltafrac;
        if (ltemp > 0x7fffl)
            xstep = 0x7fff;
        else if (ltemp < -0x7fffl)
            xstep = -0x7fff;
        else
            xstep = ltemp;
        xfrac = x1 + (((int32_t)xstep * partial) >> 8);

        y = yt1 + ystep;
        yt2 += ystep;
        do {
            x = xfrac >> 8;
            xfrac += xstep;

            value = (unsigned)tilemap[x][y];
            y += ystep;

            if (!value)
                continue;

            if (value < 128 || value > 256)
                return false;

            //
            // see if the door is open enough
            //
            value &= ~0x80;
            intercept = xfrac - xstep / 2;

            if (intercept > doorposition[value])
                return false;
        } while (y != yt2);
    }

    return true;
}

/*
================
=
= CheckSight
=
= Checks a straight line between player and current object
=
= If the sight is ok, check alertness and angle to see if they notice
=
= returns true if the player has been spoted
=
================
*/

#define MINSIGHT 0x18000l

boolean CheckSight(objstruct* ob)
{
    int32_t deltax, deltay;

    //
    // don't bother tracing a line if the area isn't connected to the player's
    //
    if (!areabyplayer[ob->areanumber])
        return false;

    //
    // if the player is real close, sight is automatic
    //
    deltax = player->x - ob->x;
    deltay = player->y - ob->y;

    if (deltax > -MINSIGHT && deltax < MINSIGHT
        && deltay > -MINSIGHT && deltay < MINSIGHT)
        return true;

    //
    // see if they are looking in the right direction
    //
    switch (ob->dir) {
    case dirtype::north:
        if (deltay > 0)
            return false;
        break;

    case dirtype::east:
        if (deltax < 0)
            return false;
        break;

    case dirtype::south:
        if (deltay < 0)
            return false;
        break;

    case dirtype::west:
        if (deltax > 0)
            return false;
        break;

        // check diagonal moving guards fix

    case dirtype::northwest:
        if (DEMOCOND_SDL && deltay > -deltax)
            return false;
        break;

    case dirtype::northeast:
        if (DEMOCOND_SDL && deltay > deltax)
            return false;
        break;

    case dirtype::southwest:
        if (DEMOCOND_SDL && deltax > deltay)
            return false;
        break;

    case dirtype::southeast:
        if (DEMOCOND_SDL && -deltax > deltay)
            return false;
        break;
    }

    //
    // trace a line to check for blocking tiles (corners)
    //
    return CheckLine(ob);
}

/*
===============
=
= FirstSighting
=
= Puts an actor into attack mode and possibly reverses the direction
= if the player is behind it
=
===============
*/

void FirstSighting(objstruct* ob)
{
    //
    // react to the player
    //
    switch (ob->obclass) {
    case classtype::guardobj:
        PlaySoundLocActor(HALTSND, ob);
        NewState(ob, &s_grdchase1);
        ob->speed *= 3; // go faster when chasing player
        break;

    case classtype::officerobj:
        PlaySoundLocActor(SPIONSND, ob);
        NewState(ob, &s_ofcchase1);
        ob->speed *= 5; // go faster when chasing player
        break;

    case classtype::mutantobj:
        NewState(ob, &s_mutchase1);
        ob->speed *= 3; // go faster when chasing player
        break;

    case classtype::ssobj:
        PlaySoundLocActor(SCHUTZADSND, ob);
        NewState(ob, &s_sschase1);
        ob->speed *= 4; // go faster when chasing player
        break;

    case classtype::dogobj:
        PlaySoundLocActor(DOGBARKSND, ob);
        NewState(ob, &s_dogchase1);
        ob->speed *= 2; // go faster when chasing player
        break;

#ifndef SPEAR
    case classtype::bossobj:
        SD_PlaySound(GUTENTAGSND);
        NewState(ob, &s_bosschase1);
        ob->speed = SPDPATROL * 3; // go faster when chasing player
        break;

#ifndef APOGEE_1_0
    case classtype::gretelobj:
        SD_PlaySound(KEINSND);
        NewState(ob, &s_gretelchase1);
        ob->speed *= 3; // go faster when chasing player
        break;

    case classtype::giftobj:
        SD_PlaySound(EINESND);
        NewState(ob, &s_giftchase1);
        ob->speed *= 3; // go faster when chasing player
        break;

    case classtype::fatobj:
        SD_PlaySound(ERLAUBENSND);
        NewState(ob, &s_fatchase1);
        ob->speed *= 3; // go faster when chasing player
        break;
#endif

    case classtype::schabbobj:
        SD_PlaySound(SCHABBSHASND);
        NewState(ob, &s_schabbchase1);
        ob->speed *= 3; // go faster when chasing player
        break;

    case classtype::fakeobj:
        SD_PlaySound(TOT_HUNDSND);
        NewState(ob, &s_fakechase1);
        ob->speed *= 3; // go faster when chasing player
        break;

    case classtype::mechahitlerobj:
        SD_PlaySound(DIESND);
        NewState(ob, &s_mechachase1);
        ob->speed *= 3; // go faster when chasing player
        break;

    case classtype::realhitlerobj:
        SD_PlaySound(DIESND);
        NewState(ob, &s_hitlerchase1);
        ob->speed *= 5; // go faster when chasing player
        break;

    case classtype::ghostobj:
        NewState(ob, &s_blinkychase1);
        ob->speed *= 2; // go faster when chasing player
        break;
#else
    case classtype::spectreobj:
        SD_PlaySound(GHOSTSIGHTSND);
        NewState(ob, &s_spectrechase1);
        ob->speed = 800; // go faster when chasing player
        break;

    case classtype::angelobj:
        SD_PlaySound(ANGELSIGHTSND);
        NewState(ob, &s_angelchase1);
        ob->speed = 1536; // go faster when chasing player
        break;

    case classtype::transobj:
        SD_PlaySound(TRANSSIGHTSND);
        NewState(ob, &s_transchase1);
        ob->speed = 1536; // go faster when chasing player
        break;

    case classtype::uberobj:
        NewState(ob, &s_uberchase1);
        ob->speed = 3000; // go faster when chasing player
        break;

    case classtype::willobj:
        SD_PlaySound(WILHELMSIGHTSND);
        NewState(ob, &s_willchase1);
        ob->speed = 2048; // go faster when chasing player
        break;

    case classtype::deathobj:
        SD_PlaySound(KNIGHTSIGHTSND);
        NewState(ob, &s_deathchase1);
        ob->speed = 2048; // go faster when chasing player
        break;
#endif
    }

    if (ob->distance < 0)
        ob->distance = 0; // ignore the door opening command

    ob->flags |= static_cast<std::uint32_t>(objflag_t::FL_ATTACKMODE) | static_cast<std::uint32_t>(objflag_t::FL_FIRSTATTACK);
}

/*
===============
=
= SightPlayer
=
= Called by actors that ARE NOT chasing the player.  If the player
= is detected (by sight, noise, or proximity), the actor is put into
= it's combat frame and true is returned.
=
= Incorporates a random reaction delay
=
===============
*/

boolean SightPlayer(objstruct* ob)
{
    if (ob->flags & static_cast<std::uint32_t>(objflag_t::FL_ATTACKMODE))
        Quit("An actor in ATTACKMODE called SightPlayer!");

    if (ob->temp2) {
        //
        // count down reaction time
        //
        ob->temp2 -= (short)tics;
        if (ob->temp2 > 0)
            return false;
        ob->temp2 = 0; // time to react
    } else {
        if (!areabyplayer[ob->areanumber])
            return false;

        if (ob->flags & static_cast<std::uint32_t>(objflag_t::FL_AMBUSH)) {
            if (!CheckSight(ob))
                return false;
            ob->flags &= ~static_cast<std::uint32_t>(objflag_t::FL_AMBUSH);
        } else {
            if (!madenoise && !CheckSight(ob))
                return false;
        }

        switch (ob->obclass) {
        case classtype::guardobj:
            ob->temp2 = 1 + US_RndT() / 4;
            break;
        case classtype::officerobj:
            ob->temp2 = 2;
            break;
        case classtype::mutantobj:
            ob->temp2 = 1 + US_RndT() / 6;
            break;
        case classtype::ssobj:
            ob->temp2 = 1 + US_RndT() / 6;
            break;
        case classtype::dogobj:
            ob->temp2 = 1 + US_RndT() / 8;
            break;

        case classtype::bossobj:
        case classtype::schabbobj:
        case classtype::fakeobj:
        case classtype::mechahitlerobj:
        case classtype::realhitlerobj:
        case classtype::gretelobj:
        case classtype::giftobj:
        case classtype::fatobj:
        case classtype::spectreobj:
        case classtype::angelobj:
        case classtype::transobj:
        case classtype::uberobj:
        case classtype::willobj:
        case classtype::deathobj:
            ob->temp2 = 1;
            break;
        }
        return false;
    }

    FirstSighting(ob);

    return true;
}
