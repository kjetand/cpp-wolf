#ifndef WL_DEF_H
#define WL_DEF_H

// Defines which version shall be built and configures supported extra features
#include "version.h"
#include <cassert>
#include <fcntl.h>
#include <cmath>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <array>
#include <utility>
// Win32
#ifdef _WIN32
#include <wtypes.h>
#include "SDL.h"
#include "SDL_syswm.h"
#endif
#if !defined(_WIN32)
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <SDL/SDL.h>
#endif

#if !defined O_BINARY
#define O_BINARY 0
#endif

#pragma pack(1)

#include "foreign.h"

#ifndef SPEAR
#include "audiowl6.h"
#ifdef UPLOAD
#include "gfxv_apo.h"
#else
#ifdef GOODTIMES
#include "gfxv_wl6.h"
#else
#include "gfxv_apo.h"
#endif
#endif
#else
#include "audiosod.h"
#include "gfxv_sod.h"
#include "f_spear.h"
#endif

#include <magic_enum.hpp>

using byte = std::uint8_t;
using word = std::uint16_t;
using fixed = std::int32_t;
using longword = std::uint32_t;

struct Point {
    int x, y;
};

struct Rect {
    Point ul, lr;
};

void Quit(const char* errorStr, ...);

#include "id_pm.h"
#include "id_sd.h"
#include "id_in.h"
#include "id_vl.h"
#include "id_vh.h"
#include "id_us.h"
#include "id_ca.h"

#include "wl_menu.h"

// TODO: Make constexpr, or move (mapshift?)
#define MAPSPOT(x, y, plane) (mapsegs[plane][((y) << mapshift) + (x)])

/*
=============================================================================

                            GLOBAL CONSTANTS

=============================================================================
*/

constexpr auto MAXTICS = 10;
constexpr auto DEMOTICS = 4;

constexpr auto MAXACTORS = 150;   // max number of nazis, etc / map
constexpr auto MAXSTATS = 400;    // max number of lamps, bonus, etc
constexpr auto MAXDOORS = 64;     // max number of sliding doors
constexpr auto MAXWALLTILES = 64; // max number of wall tiles

//
// tile constants
//

constexpr auto ICONARROWS = 90;
constexpr auto PUSHABLETILE = 98;
constexpr auto EXITTILE = 99;  // at end of castle
constexpr auto AREATILE = 107; // first of NUMAREAS floor tiles
constexpr auto NUMAREAS = 37;
constexpr auto ELEVATORTILE = 21;
constexpr auto AMBUSHTILE = 106;
constexpr auto ALTELEVATORTILE = 107;

constexpr auto NUMBERCHARS = 9;

//----------------

constexpr auto EXTRAPOINTS = 40000;
constexpr auto RUNSPEED = 6000;
constexpr auto SCREENBWIDE = 80;
constexpr auto HEIGHTRATIO = 0.50; // also defined in id_mm.c

#ifndef SPEAR
constexpr auto LRpack = 8; // # of levels to store in endgame
#else
constexpr auto LRpack = 20;
#endif

constexpr auto MINDIST = (0x5800l);
constexpr auto PLAYERSIZE = MINDIST;    // player radius
constexpr auto MINACTORDIST = 0x10000l; // minimum dist from player center \
                              // to any actor center

#undef M_PI
constexpr auto M_PI = 3.141592657;

constexpr auto GLOBAL1 = (1l << 16);
constexpr auto TILEGLOBAL = GLOBAL1;
constexpr auto TILESHIFT = 16l;
constexpr auto UNSIGNEDSHIFT = 8;

constexpr auto ANGLES = 360; // must be divisable by 4
constexpr auto ANGLEQUAD = (ANGLES / 4);
constexpr auto FINEANGLES = 3600;

constexpr auto mapshift = 6;
constexpr auto MAPSIZE = (1 << mapshift);
constexpr auto maparea = MAPSIZE * MAPSIZE;

constexpr auto mapheight = MAPSIZE;
constexpr auto mapwidth = MAPSIZE;

constexpr auto TEXTURESHIFT = 6;
constexpr auto TEXTURESIZE = (1 << TEXTURESHIFT);
constexpr auto TEXTUREFROMFIXEDSHIFT = 4;
constexpr auto TEXTUREMASK = (TEXTURESIZE * (TEXTURESIZE - 1));

constexpr auto SPRITESCALEFACTOR = 2;

constexpr auto NORTH = 0;

constexpr auto STATUSLINES = 40;

constexpr auto SCREENSIZE = (SCREENBWIDE * 208);
constexpr auto PAGE1START = 0;
constexpr auto PAGE2START = (SCREENSIZE);

constexpr auto STARTAMMO = 8;

// object flag values

enum class objflag_t : std::uint32_t {
    FL_SHOOTABLE = 0x00000001,
    FL_BONUS = 0x00000002,
    FL_NEVERMARK = 0x00000004,
    FL_VISABLE = 0x00000008,
    FL_ATTACKMODE = 0x00000010,
    FL_FIRSTATTACK = 0x00000020,
    FL_AMBUSH = 0x00000040,
    FL_NONMARK = 0x00000080,
    FL_FULLBRIGHT = 0x00000100,
    // next free bit is   0x00001000
};

//
// sprite constants
//
enum class sprite : unsigned {
    SPR_DEMO,
#ifndef APOGEE_1_0
    SPR_DEATHCAM,
#endif
    //
    // static sprites
    //
    SPR_STAT_0,
    SPR_STAT_1,
    SPR_STAT_2,
    SPR_STAT_3,
    SPR_STAT_4,
    SPR_STAT_5,
    SPR_STAT_6,
    SPR_STAT_7,

    SPR_STAT_8,
    SPR_STAT_9,
    SPR_STAT_10,
    SPR_STAT_11,
    SPR_STAT_12,
    SPR_STAT_13,
    SPR_STAT_14,
    SPR_STAT_15,

    SPR_STAT_16,
    SPR_STAT_17,
    SPR_STAT_18,
    SPR_STAT_19,
    SPR_STAT_20,
    SPR_STAT_21,
    SPR_STAT_22,
    SPR_STAT_23,

    SPR_STAT_24,
    SPR_STAT_25,
    SPR_STAT_26,
    SPR_STAT_27,
    SPR_STAT_28,
    SPR_STAT_29,
    SPR_STAT_30,
    SPR_STAT_31,

    SPR_STAT_32,
    SPR_STAT_33,
    SPR_STAT_34,
    SPR_STAT_35,
    SPR_STAT_36,
    SPR_STAT_37,
    SPR_STAT_38,
    SPR_STAT_39,

    SPR_STAT_40,
    SPR_STAT_41,
    SPR_STAT_42,
    SPR_STAT_43,
    SPR_STAT_44,
    SPR_STAT_45,
    SPR_STAT_46,
    SPR_STAT_47,

#ifdef SPEAR
    SPR_STAT_48,
    SPR_STAT_49,
    SPR_STAT_50,
    SPR_STAT_51,
#endif

    //
    // guard
    //
    SPR_GRD_S_1,
    SPR_GRD_S_2,
    SPR_GRD_S_3,
    SPR_GRD_S_4,
    SPR_GRD_S_5,
    SPR_GRD_S_6,
    SPR_GRD_S_7,
    SPR_GRD_S_8,

    SPR_GRD_W1_1,
    SPR_GRD_W1_2,
    SPR_GRD_W1_3,
    SPR_GRD_W1_4,
    SPR_GRD_W1_5,
    SPR_GRD_W1_6,
    SPR_GRD_W1_7,
    SPR_GRD_W1_8,

    SPR_GRD_W2_1,
    SPR_GRD_W2_2,
    SPR_GRD_W2_3,
    SPR_GRD_W2_4,
    SPR_GRD_W2_5,
    SPR_GRD_W2_6,
    SPR_GRD_W2_7,
    SPR_GRD_W2_8,

    SPR_GRD_W3_1,
    SPR_GRD_W3_2,
    SPR_GRD_W3_3,
    SPR_GRD_W3_4,
    SPR_GRD_W3_5,
    SPR_GRD_W3_6,
    SPR_GRD_W3_7,
    SPR_GRD_W3_8,

    SPR_GRD_W4_1,
    SPR_GRD_W4_2,
    SPR_GRD_W4_3,
    SPR_GRD_W4_4,
    SPR_GRD_W4_5,
    SPR_GRD_W4_6,
    SPR_GRD_W4_7,
    SPR_GRD_W4_8,

    SPR_GRD_PAIN_1,
    SPR_GRD_DIE_1,
    SPR_GRD_DIE_2,
    SPR_GRD_DIE_3,
    SPR_GRD_PAIN_2,
    SPR_GRD_DEAD,

    SPR_GRD_SHOOT1,
    SPR_GRD_SHOOT2,
    SPR_GRD_SHOOT3,

    //
    // dogs
    //
    SPR_DOG_W1_1,
    SPR_DOG_W1_2,
    SPR_DOG_W1_3,
    SPR_DOG_W1_4,
    SPR_DOG_W1_5,
    SPR_DOG_W1_6,
    SPR_DOG_W1_7,
    SPR_DOG_W1_8,

    SPR_DOG_W2_1,
    SPR_DOG_W2_2,
    SPR_DOG_W2_3,
    SPR_DOG_W2_4,
    SPR_DOG_W2_5,
    SPR_DOG_W2_6,
    SPR_DOG_W2_7,
    SPR_DOG_W2_8,

    SPR_DOG_W3_1,
    SPR_DOG_W3_2,
    SPR_DOG_W3_3,
    SPR_DOG_W3_4,
    SPR_DOG_W3_5,
    SPR_DOG_W3_6,
    SPR_DOG_W3_7,
    SPR_DOG_W3_8,

    SPR_DOG_W4_1,
    SPR_DOG_W4_2,
    SPR_DOG_W4_3,
    SPR_DOG_W4_4,
    SPR_DOG_W4_5,
    SPR_DOG_W4_6,
    SPR_DOG_W4_7,
    SPR_DOG_W4_8,

    SPR_DOG_DIE_1,
    SPR_DOG_DIE_2,
    SPR_DOG_DIE_3,
    SPR_DOG_DEAD,
    SPR_DOG_JUMP1,
    SPR_DOG_JUMP2,
    SPR_DOG_JUMP3,

    //
    // ss
    //
    SPR_SS_S_1,
    SPR_SS_S_2,
    SPR_SS_S_3,
    SPR_SS_S_4,
    SPR_SS_S_5,
    SPR_SS_S_6,
    SPR_SS_S_7,
    SPR_SS_S_8,

    SPR_SS_W1_1,
    SPR_SS_W1_2,
    SPR_SS_W1_3,
    SPR_SS_W1_4,
    SPR_SS_W1_5,
    SPR_SS_W1_6,
    SPR_SS_W1_7,
    SPR_SS_W1_8,

    SPR_SS_W2_1,
    SPR_SS_W2_2,
    SPR_SS_W2_3,
    SPR_SS_W2_4,
    SPR_SS_W2_5,
    SPR_SS_W2_6,
    SPR_SS_W2_7,
    SPR_SS_W2_8,

    SPR_SS_W3_1,
    SPR_SS_W3_2,
    SPR_SS_W3_3,
    SPR_SS_W3_4,
    SPR_SS_W3_5,
    SPR_SS_W3_6,
    SPR_SS_W3_7,
    SPR_SS_W3_8,

    SPR_SS_W4_1,
    SPR_SS_W4_2,
    SPR_SS_W4_3,
    SPR_SS_W4_4,
    SPR_SS_W4_5,
    SPR_SS_W4_6,
    SPR_SS_W4_7,
    SPR_SS_W4_8,

    SPR_SS_PAIN_1,
    SPR_SS_DIE_1,
    SPR_SS_DIE_2,
    SPR_SS_DIE_3,
    SPR_SS_PAIN_2,
    SPR_SS_DEAD,

    SPR_SS_SHOOT1,
    SPR_SS_SHOOT2,
    SPR_SS_SHOOT3,

    //
    // mutant
    //
    SPR_MUT_S_1,
    SPR_MUT_S_2,
    SPR_MUT_S_3,
    SPR_MUT_S_4,
    SPR_MUT_S_5,
    SPR_MUT_S_6,
    SPR_MUT_S_7,
    SPR_MUT_S_8,

    SPR_MUT_W1_1,
    SPR_MUT_W1_2,
    SPR_MUT_W1_3,
    SPR_MUT_W1_4,
    SPR_MUT_W1_5,
    SPR_MUT_W1_6,
    SPR_MUT_W1_7,
    SPR_MUT_W1_8,

    SPR_MUT_W2_1,
    SPR_MUT_W2_2,
    SPR_MUT_W2_3,
    SPR_MUT_W2_4,
    SPR_MUT_W2_5,
    SPR_MUT_W2_6,
    SPR_MUT_W2_7,
    SPR_MUT_W2_8,

    SPR_MUT_W3_1,
    SPR_MUT_W3_2,
    SPR_MUT_W3_3,
    SPR_MUT_W3_4,
    SPR_MUT_W3_5,
    SPR_MUT_W3_6,
    SPR_MUT_W3_7,
    SPR_MUT_W3_8,

    SPR_MUT_W4_1,
    SPR_MUT_W4_2,
    SPR_MUT_W4_3,
    SPR_MUT_W4_4,
    SPR_MUT_W4_5,
    SPR_MUT_W4_6,
    SPR_MUT_W4_7,
    SPR_MUT_W4_8,

    SPR_MUT_PAIN_1,
    SPR_MUT_DIE_1,
    SPR_MUT_DIE_2,
    SPR_MUT_DIE_3,
    SPR_MUT_PAIN_2,
    SPR_MUT_DIE_4,
    SPR_MUT_DEAD,

    SPR_MUT_SHOOT1,
    SPR_MUT_SHOOT2,
    SPR_MUT_SHOOT3,
    SPR_MUT_SHOOT4,

    //
    // officer
    //
    SPR_OFC_S_1,
    SPR_OFC_S_2,
    SPR_OFC_S_3,
    SPR_OFC_S_4,
    SPR_OFC_S_5,
    SPR_OFC_S_6,
    SPR_OFC_S_7,
    SPR_OFC_S_8,

    SPR_OFC_W1_1,
    SPR_OFC_W1_2,
    SPR_OFC_W1_3,
    SPR_OFC_W1_4,
    SPR_OFC_W1_5,
    SPR_OFC_W1_6,
    SPR_OFC_W1_7,
    SPR_OFC_W1_8,

    SPR_OFC_W2_1,
    SPR_OFC_W2_2,
    SPR_OFC_W2_3,
    SPR_OFC_W2_4,
    SPR_OFC_W2_5,
    SPR_OFC_W2_6,
    SPR_OFC_W2_7,
    SPR_OFC_W2_8,

    SPR_OFC_W3_1,
    SPR_OFC_W3_2,
    SPR_OFC_W3_3,
    SPR_OFC_W3_4,
    SPR_OFC_W3_5,
    SPR_OFC_W3_6,
    SPR_OFC_W3_7,
    SPR_OFC_W3_8,

    SPR_OFC_W4_1,
    SPR_OFC_W4_2,
    SPR_OFC_W4_3,
    SPR_OFC_W4_4,
    SPR_OFC_W4_5,
    SPR_OFC_W4_6,
    SPR_OFC_W4_7,
    SPR_OFC_W4_8,

    SPR_OFC_PAIN_1,
    SPR_OFC_DIE_1,
    SPR_OFC_DIE_2,
    SPR_OFC_DIE_3,
    SPR_OFC_PAIN_2,
    SPR_OFC_DIE_4,
    SPR_OFC_DEAD,

    SPR_OFC_SHOOT1,
    SPR_OFC_SHOOT2,
    SPR_OFC_SHOOT3,

#ifndef SPEAR
    //
    // ghosts
    //
    SPR_BLINKY_W1,
    SPR_BLINKY_W2,
    SPR_PINKY_W1,
    SPR_PINKY_W2,
    SPR_CLYDE_W1,
    SPR_CLYDE_W2,
    SPR_INKY_W1,
    SPR_INKY_W2,

    //
    // hans
    //
    SPR_BOSS_W1,
    SPR_BOSS_W2,
    SPR_BOSS_W3,
    SPR_BOSS_W4,
    SPR_BOSS_SHOOT1,
    SPR_BOSS_SHOOT2,
    SPR_BOSS_SHOOT3,
    SPR_BOSS_DEAD,

    SPR_BOSS_DIE1,
    SPR_BOSS_DIE2,
    SPR_BOSS_DIE3,

    //
    // schabbs
    //
    SPR_SCHABB_W1,
    SPR_SCHABB_W2,
    SPR_SCHABB_W3,
    SPR_SCHABB_W4,
    SPR_SCHABB_SHOOT1,
    SPR_SCHABB_SHOOT2,

    SPR_SCHABB_DIE1,
    SPR_SCHABB_DIE2,
    SPR_SCHABB_DIE3,
    SPR_SCHABB_DEAD,
    SPR_HYPO1,
    SPR_HYPO2,
    SPR_HYPO3,
    SPR_HYPO4,

    //
    // fake
    //
    SPR_FAKE_W1,
    SPR_FAKE_W2,
    SPR_FAKE_W3,
    SPR_FAKE_W4,
    SPR_FAKE_SHOOT,
    SPR_FIRE1,
    SPR_FIRE2,

    SPR_FAKE_DIE1,
    SPR_FAKE_DIE2,
    SPR_FAKE_DIE3,
    SPR_FAKE_DIE4,
    SPR_FAKE_DIE5,
    SPR_FAKE_DEAD,

    //
    // hitler
    //
    SPR_MECHA_W1,
    SPR_MECHA_W2,
    SPR_MECHA_W3,
    SPR_MECHA_W4,
    SPR_MECHA_SHOOT1,
    SPR_MECHA_SHOOT2,
    SPR_MECHA_SHOOT3,
    SPR_MECHA_DEAD,

    SPR_MECHA_DIE1,
    SPR_MECHA_DIE2,
    SPR_MECHA_DIE3,

    SPR_HITLER_W1,
    SPR_HITLER_W2,
    SPR_HITLER_W3,
    SPR_HITLER_W4,
    SPR_HITLER_SHOOT1,
    SPR_HITLER_SHOOT2,
    SPR_HITLER_SHOOT3,
    SPR_HITLER_DEAD,

    SPR_HITLER_DIE1,
    SPR_HITLER_DIE2,
    SPR_HITLER_DIE3,
    SPR_HITLER_DIE4,
    SPR_HITLER_DIE5,
    SPR_HITLER_DIE6,
    SPR_HITLER_DIE7,

    //
    // giftmacher
    //
    SPR_GIFT_W1,
    SPR_GIFT_W2,
    SPR_GIFT_W3,
    SPR_GIFT_W4,
    SPR_GIFT_SHOOT1,
    SPR_GIFT_SHOOT2,

    SPR_GIFT_DIE1,
    SPR_GIFT_DIE2,
    SPR_GIFT_DIE3,
    SPR_GIFT_DEAD,
#endif
    //
    // Rocket, smoke and small explosion
    //
    SPR_ROCKET_1,
    SPR_ROCKET_2,
    SPR_ROCKET_3,
    SPR_ROCKET_4,
    SPR_ROCKET_5,
    SPR_ROCKET_6,
    SPR_ROCKET_7,
    SPR_ROCKET_8,

    SPR_SMOKE_1,
    SPR_SMOKE_2,
    SPR_SMOKE_3,
    SPR_SMOKE_4,
    SPR_BOOM_1,
    SPR_BOOM_2,
    SPR_BOOM_3,

//
// Angel of Death's DeathSparks(tm)
//
#ifdef SPEAR
    SPR_HROCKET_1,
    SPR_HROCKET_2,
    SPR_HROCKET_3,
    SPR_HROCKET_4,
    SPR_HROCKET_5,
    SPR_HROCKET_6,
    SPR_HROCKET_7,
    SPR_HROCKET_8,

    SPR_HSMOKE_1,
    SPR_HSMOKE_2,
    SPR_HSMOKE_3,
    SPR_HSMOKE_4,
    SPR_HBOOM_1,
    SPR_HBOOM_2,
    SPR_HBOOM_3,

    SPR_SPARK1,
    SPR_SPARK2,
    SPR_SPARK3,
    SPR_SPARK4,
#endif

#ifndef SPEAR
    //
    // gretel
    //
    SPR_GRETEL_W1,
    SPR_GRETEL_W2,
    SPR_GRETEL_W3,
    SPR_GRETEL_W4,
    SPR_GRETEL_SHOOT1,
    SPR_GRETEL_SHOOT2,
    SPR_GRETEL_SHOOT3,
    SPR_GRETEL_DEAD,

    SPR_GRETEL_DIE1,
    SPR_GRETEL_DIE2,
    SPR_GRETEL_DIE3,

    //
    // fat face
    //
    SPR_FAT_W1,
    SPR_FAT_W2,
    SPR_FAT_W3,
    SPR_FAT_W4,
    SPR_FAT_SHOOT1,
    SPR_FAT_SHOOT2,
    SPR_FAT_SHOOT3,
    SPR_FAT_SHOOT4,

    SPR_FAT_DIE1,
    SPR_FAT_DIE2,
    SPR_FAT_DIE3,
    SPR_FAT_DEAD,

//
// bj
//
#ifdef APOGEE_1_0
    SPR_BJ_W1 = 360,
#elif defined(APOGEE_1_1) && defined(UPLOAD)
    SPR_BJ_W1 = 406,
#else
    SPR_BJ_W1,
#endif
    SPR_BJ_W2,
    SPR_BJ_W3,
    SPR_BJ_W4,
    SPR_BJ_JUMP1,
    SPR_BJ_JUMP2,
    SPR_BJ_JUMP3,
    SPR_BJ_JUMP4,
#else
    //
    // THESE ARE FOR 'SPEAR OF DESTINY'
    //

    //
    // Trans Grosse
    //
    SPR_TRANS_W1,
    SPR_TRANS_W2,
    SPR_TRANS_W3,
    SPR_TRANS_W4,
    SPR_TRANS_SHOOT1,
    SPR_TRANS_SHOOT2,
    SPR_TRANS_SHOOT3,
    SPR_TRANS_DEAD,

    SPR_TRANS_DIE1,
    SPR_TRANS_DIE2,
    SPR_TRANS_DIE3,

    //
    // Wilhelm
    //
    SPR_WILL_W1,
    SPR_WILL_W2,
    SPR_WILL_W3,
    SPR_WILL_W4,
    SPR_WILL_SHOOT1,
    SPR_WILL_SHOOT2,
    SPR_WILL_SHOOT3,
    SPR_WILL_SHOOT4,

    SPR_WILL_DIE1,
    SPR_WILL_DIE2,
    SPR_WILL_DIE3,
    SPR_WILL_DEAD,

    //
    // UberMutant
    //
    SPR_UBER_W1,
    SPR_UBER_W2,
    SPR_UBER_W3,
    SPR_UBER_W4,
    SPR_UBER_SHOOT1,
    SPR_UBER_SHOOT2,
    SPR_UBER_SHOOT3,
    SPR_UBER_SHOOT4,

    SPR_UBER_DIE1,
    SPR_UBER_DIE2,
    SPR_UBER_DIE3,
    SPR_UBER_DIE4,
    SPR_UBER_DEAD,

    //
    // Death Knight
    //
    SPR_DEATH_W1,
    SPR_DEATH_W2,
    SPR_DEATH_W3,
    SPR_DEATH_W4,
    SPR_DEATH_SHOOT1,
    SPR_DEATH_SHOOT2,
    SPR_DEATH_SHOOT3,
    SPR_DEATH_SHOOT4,

    SPR_DEATH_DIE1,
    SPR_DEATH_DIE2,
    SPR_DEATH_DIE3,
    SPR_DEATH_DIE4,
    SPR_DEATH_DIE5,
    SPR_DEATH_DIE6,
    SPR_DEATH_DEAD,

    //
    // Ghost
    //
    SPR_SPECTRE_W1,
    SPR_SPECTRE_W2,
    SPR_SPECTRE_W3,
    SPR_SPECTRE_W4,
    SPR_SPECTRE_F1,
    SPR_SPECTRE_F2,
    SPR_SPECTRE_F3,
    SPR_SPECTRE_F4,

    //
    // Angel of Death
    //
    SPR_ANGEL_W1,
    SPR_ANGEL_W2,
    SPR_ANGEL_W3,
    SPR_ANGEL_W4,
    SPR_ANGEL_SHOOT1,
    SPR_ANGEL_SHOOT2,
    SPR_ANGEL_TIRED1,
    SPR_ANGEL_TIRED2,

    SPR_ANGEL_DIE1,
    SPR_ANGEL_DIE2,
    SPR_ANGEL_DIE3,
    SPR_ANGEL_DIE4,
    SPR_ANGEL_DIE5,
    SPR_ANGEL_DIE6,
    SPR_ANGEL_DIE7,
    SPR_ANGEL_DEAD,

#endif

    //
    // player attack frames
    //
    SPR_KNIFEREADY,
    SPR_KNIFEATK1,
    SPR_KNIFEATK2,
    SPR_KNIFEATK3,
    SPR_KNIFEATK4,

    SPR_PISTOLREADY,
    SPR_PISTOLATK1,
    SPR_PISTOLATK2,
    SPR_PISTOLATK3,
    SPR_PISTOLATK4,

    SPR_MACHINEGUNREADY,
    SPR_MACHINEGUNATK1,
    SPR_MACHINEGUNATK2,
    MACHINEGUNATK3,
    SPR_MACHINEGUNATK4,

    SPR_CHAINREADY,
    SPR_CHAINATK1,
    SPR_CHAINATK2,
    SPR_CHAINATK3,
    SPR_CHAINATK4,

};

/*
=============================================================================

                               GLOBAL TYPES

=============================================================================
*/

enum class controldir_t : byte {
    di_north,
    di_east,
    di_south,
    di_west
};

enum class door_t : byte {
    dr_normal,
    dr_lock1,
    dr_lock2,
    dr_lock3,
    dr_lock4,
    dr_elevator
};

enum class activetype {
    ac_badobject = -1,
    ac_no,
    ac_yes,
    ac_allways
};

enum class classtype : byte {
    nothing,
    playerobj,
    inertobj,
    guardobj,
    officerobj,
    ssobj,
    dogobj,
    bossobj,
    schabbobj,
    fakeobj,
    mechahitlerobj,
    mutantobj,
    needleobj,
    fireobj,
    bjobj,
    ghostobj,
    realhitlerobj,
    gretelobj,
    giftobj,
    fatobj,
    rocketobj,

    spectreobj,
    angelobj,
    transobj,
    uberobj,
    willobj,
    deathobj,
    hrocketobj,
    sparkobj
};

enum class wl_stat_t : byte {
    none,
    block,
    bo_gibs,
    bo_alpo,
    bo_firstaid,
    bo_key1,
    bo_key2,
    bo_key3,
    bo_key4,
    bo_cross,
    bo_chalice,
    bo_bible,
    bo_crown,
    bo_clip,
    bo_clip2,
    bo_machinegun,
    bo_chaingun,
    bo_food,
    bo_fullheal,
    bo_25clip,
    bo_spear
};

enum class dirtype {
    east,
    northeast,
    north,
    northwest,
    west,
    southwest,
    south,
    southeast,
    nodir
};

enum class enemy_t : byte {
    en_guard,
    en_officer,
    en_ss,
    en_dog,
    en_boss,
    en_schabbs,
    en_fake,
    en_hitler,
    en_mutant,
    en_blinky,
    en_clyde,
    en_pinky,
    en_inky,
    en_gretel,
    en_gift,
    en_fat,
    en_spectre,
    en_angel,
    en_trans,
    en_uber,
    en_will,
    en_death
};

constexpr auto NUMENEMIES = magic_enum::enum_count<enemy_t>();

typedef void (*statefunc)(void*);

struct statetype {
    bool  rotate;
    short shapenum; // a shapenum of -1 means get from ob->temp1
    short tictime;
    void (*think)(void*), (*action)(void*);
    struct statetype* next;
};

//---------------------
//
// trivial actor structure
//
//---------------------

struct statstruct {
    byte      tilex, tiley;
    short     shapenum; // if shapenum == -1 the obj has been removed
    byte*     visspot;
    uint32_t  flags;
    wl_stat_t itemnumber;
};

//---------------------
//
// door actor structure
//
//---------------------

enum class doortype {
    dr_open,
    dr_closed,
    dr_opening,
    dr_closing
};

struct doorstruct {
    byte     tilex, tiley;
    bool     vertical;
    door_t   lock;
    doortype action;
    short    ticcount;
};

//--------------------
//
// thinking actor structure
//
//--------------------

struct objstruct {
    activetype active;
    short      ticcount;
    classtype  obclass;
    statetype* state;

    uint32_t flags; // FL_SHOOTABLE, etc

    int32_t distance; // if negative, wait for that door to open
    dirtype dir;

    fixed x, y;
    word  tilex, tiley;
    byte  areanumber;

    short viewx;
    word  viewheight;
    fixed transx, transy; // in global coord

    short   angle;
    short   hitpoints;
    int32_t speed;

    short             temp1, temp2, hidden;
    struct objstruct *next, *prev;
};

enum class button_t : int {
    bt_nobutton = -1,
    bt_attack = 0,
    bt_strafe,
    bt_run,
    bt_use,
    bt_readyknife,
    bt_readypistol,
    bt_readymachinegun,
    bt_readychaingun,
    bt_nextweapon,
    bt_prevweapon,
    bt_esc,
    bt_pause,
    bt_strafeleft,
    bt_straferight,
    bt_moveforward,
    bt_movebackward,
    bt_turnleft,
    bt_turnright,
};

constexpr auto NUMBUTTONS = magic_enum::enum_count<button_t>();

enum class weapontype : byte {
    wp_knife,
    wp_pistol,
    wp_machinegun,
    wp_chaingun
};

constexpr auto NUMWEAPONS = magic_enum::enum_count<weapontype>();

enum class difficulty_t : byte {
    gd_baby,
    gd_easy,
    gd_medium,
    gd_hard
};

//---------------
//
// gamestate structure
//
//---------------

typedef struct
{
    difficulty_t difficulty;
    short        mapon;
    int32_t      oldscore, score, nextextra;
    short        lives;
    short        health;
    short        ammo;
    short        keys;
    weapontype   bestweapon, weapon, chosenweapon;

    short faceframe;
    short attackframe, attackcount, weaponframe;

    short episode, secretcount, treasurecount, killcount,
        secrettotal, treasuretotal, killtotal;
    int32_t TimeCount;
    int32_t killx, killy;
    bool    victoryflag; // set during victory animations
} gametype;

enum class exit_t : byte {
    ex_stillplaying,
    ex_completed,
    ex_died,
    ex_warped,
    ex_resetgame,
    ex_loadedgame,
    ex_victorious,
    ex_abort,
    ex_demodone,
    ex_secretlevel
};

extern word* mapsegs[MAPPLANES];
extern int   mapon;

/*
=============================================================================

                             WL_MAIN DEFINITIONS

=============================================================================
*/

extern bool    loadedgame;
extern fixed   focallength;
extern int     viewscreenx, viewscreeny;
extern int     viewwidth;
extern int     viewheight;
extern short   centerx;
extern int32_t heightnumerator;
extern fixed   scale;

extern int dirangle[9];

extern int      mouseadjustment;
extern int      shootdelta;
extern unsigned screenofs;

extern bool startgame;
extern char str[80];
extern char configdir[256];
extern char configname[13];

//
// Command line parameter variables
//
extern bool         param_debugmode;
extern bool         param_nowait;
extern difficulty_t param_difficulty;
extern int          param_tedlevel;
extern int          param_joystickindex;
extern int          param_joystickhat;
extern int          param_samplerate;
extern int          param_audiobuffer;
extern int          param_mission;
extern bool         param_goodtimes;
extern bool         param_ignorenumchunks;

void NewGame(difficulty_t difficulty, int episode);
void CalcProjection(int32_t focal);
void NewViewSize(int width);
bool SetViewSize(unsigned width, unsigned height);
bool LoadTheGame(FILE* file, int x, int y);
bool SaveTheGame(FILE* file, int x, int y);
void ShowViewSize(int width);
void ShutdownId();

/*
=============================================================================

                         WL_GAME DEFINITIONS

=============================================================================
*/

extern gametype     gamestate;
extern byte         bordercol;
extern SDL_Surface* latchpics[NUMLATCHPICS];
extern char         demoname[13];

void SetupGameLevel();
void GameLoop();
void DrawPlayBorder();
void DrawStatusBorder(byte color);
void DrawPlayScreen();
void DrawPlayBorderSides();
void ShowActStatus();

void PlayDemo(int demonumber);
void RecordDemo();

#ifdef SPEAR
extern int32_t  spearx, speary;
extern unsigned spearangle;
extern bool     spearflag;
#endif

#define ClearMemory SD_StopDigitized

// JAB
#define PlaySoundLocTile(s, tx, ty) PlaySoundLocGlobal(s, (((int32_t)(tx) << TILESHIFT) + (1L << (TILESHIFT - 1))), (((int32_t)ty << TILESHIFT) + (1L << (TILESHIFT - 1))))
#define PlaySoundLocActor(s, ob)    PlaySoundLocGlobal(s, (ob)->x, (ob)->y)
void PlaySoundLocGlobal(word s, fixed gx, fixed gy);
void UpdateSoundLoc();

/*
=============================================================================

                            WL_PLAY DEFINITIONS

=============================================================================
*/

constexpr auto BASEMOVE = 35;
constexpr auto RUNMOVE = 70;
constexpr auto BASETURN = 35;
constexpr auto RUNTURN = 70;

constexpr auto JOYSCALE = 2;

extern byte       tilemap[MAPSIZE][MAPSIZE]; // wall values only
extern byte       spotvis[MAPSIZE][MAPSIZE];
extern objstruct* actorat[MAPSIZE][MAPSIZE];

extern objstruct* player;

extern unsigned tics;
extern int      viewsize;

extern int lastgamemusicoffset;

//
// current user input
//
extern int         controlx, controly; // range from -100 to 100
extern bool        buttonstate[NUMBUTTONS];
extern objstruct   objlist[MAXACTORS];
extern bool        buttonheld[NUMBUTTONS];
extern exit_t      playstate;
extern bool        madenoise;
extern statstruct  statobjlist[MAXSTATS];
extern statstruct* laststatobj;
extern objstruct * newobj, *killerobj;
extern doorstruct  doorobjlist[MAXDOORS];
extern doorstruct* lastdoorobj;
extern int         godmode;

extern bool    demorecord, demoplayback;
extern int8_t *demoptr, *lastdemoptr;
extern void*   demobuffer;

//
// control info
//
extern bool                        mouseenabled, joystickenabled;
extern std::array<int, 4>          dirscan;
extern std::array<int, NUMBUTTONS> buttonscan;
extern std::array<button_t, 4>     buttonmouse;
extern std::array<button_t, 32>    buttonjoy;

void InitActorList();
void GetNewActor();
void PlayLoop();

void CenterWindow(word w, word h);

void InitRedShifts();
void FinishPaletteShifts();

void RemoveObj(objstruct* gone);
void PollControls();
int  StopMusic();
void StartMusic();
void ContinueMusic(int offs);
void StartDamageFlash(int damage);
void StartBonusFlash();

#ifdef SPEAR
extern int32_t funnyticount; // FOR FUNNY BJ FACE
#endif

extern objstruct* objfreelist; // *obj,*player,*lastobj,

extern bool noclip, ammocheat;
extern int  singlestep, extravbls;

/*
=============================================================================

                                WL_INTER

=============================================================================
*/

void IntroScreen();
void PG13();
void DrawHighScores();
void CheckHighScore(int32_t score, word other);
void Victory();
void LevelCompleted();
void ClearSplitVWB();

void PreloadGraphics();

/*
=============================================================================

                                WL_DEBUG

=============================================================================
*/

int DebugKeys();

/*
=============================================================================

                            WL_DRAW DEFINITIONS

=============================================================================
*/

//
// math tables
//
extern short*  pixelangle;
extern int32_t finetangent[FINEANGLES / 4];
extern fixed   sintable[];
extern fixed*  costable;
extern int*    wallheight;
extern word    horizwall[], vertwall[];
extern int32_t lasttimecount;
extern int32_t frameon;

extern unsigned screenloc[3];

extern bool fizzlein, fpscounter;

extern fixed viewx, viewy; // the focal point
extern fixed viewsin, viewcos;

void ThreeDRefresh();
void CalcTics();

struct t_compshape {
    word leftpix, rightpix;
    word dataofs[64];
    // table data after dataofs[rightpix-leftpix+1]
};

/*
=============================================================================

                             WL_STATE DEFINITIONS

=============================================================================
*/
constexpr auto TURNTICS = 10;
constexpr auto SPDPATROL = 512;
constexpr auto SPDDOG = 1500;

void InitHitRect(objstruct* ob, unsigned radius);
void SpawnNewObj(unsigned tilex, unsigned tiley, statetype* state);
void NewState(objstruct* ob, statetype* state);

bool TryWalk(objstruct* ob);
void SelectChaseDir(objstruct* ob);
void SelectDodgeDir(objstruct* ob);
void SelectRunDir(objstruct* ob);
void MoveObj(objstruct* ob, int32_t move);
bool SightPlayer(objstruct* ob);

void KillActor(objstruct* ob);
void DamageActor(objstruct* ob, unsigned damage);

bool CheckLine(objstruct* ob);
bool CheckSight(objstruct* ob);

/*
=============================================================================

                             WL_AGENT DEFINITIONS

=============================================================================
*/

extern short      anglefrac;
extern int        facecount, facetimes;
extern word       plux, pluy; // player coordinates scaled to unsigned
extern int32_t    thrustspeed;
extern objstruct* LastAttacker;

void Thrust(int angle, int32_t speed);
void SpawnPlayer(int tilex, int tiley, int dir);
void TakeDamage(int points, objstruct* attacker);
void GivePoints(int32_t points);
void GetBonus(statstruct* check);
void GiveWeapon(weapontype weapon);
void GiveAmmo(int ammo);
void GiveKey(int key);

//
// player state info
//

void StatusDrawFace(unsigned picnum);
void DrawFace();
void DrawHealth();
void HealSelf(int points);
void DrawLevel();
void DrawLives();
void GiveExtraMan();
void DrawScore();
void DrawWeapon();
void DrawKeys();
void DrawAmmo();

/*
=============================================================================

                             WL_ACT1 DEFINITIONS

=============================================================================
*/

extern doorstruct  doorobjlist[MAXDOORS];
extern doorstruct* lastdoorobj;
extern short       doornum;

extern word doorposition[MAXDOORS];

extern byte areaconnect[NUMAREAS][NUMAREAS];

extern bool areabyplayer[NUMAREAS];

extern word         pwallstate;
extern word         pwallpos; // amount a pushable wall has been moved (0-63)
extern word         pwallx, pwally;
extern controldir_t pwalldir;
extern byte         pwalltile;

void InitDoorList();
void InitStaticList();
void SpawnStatic(int tilex, int tiley, int type);
void SpawnDoor(int tilex, int tiley, bool vertical, door_t lock);
void MoveDoors();
void MovePWalls();
void OpenDoor(int door);
void PlaceItemType(wl_stat_t itemtype, int tilex, int tiley);
void PushWall(int checkx, int checky, controldir_t dir);
void OperateDoor(int door);
void InitAreas();

/*
=============================================================================

                             WL_ACT2 DEFINITIONS

=============================================================================
*/

extern statetype s_grddie1;
extern statetype s_dogdie1;
extern statetype s_ofcdie1;
extern statetype s_mutdie1;
extern statetype s_ssdie1;
extern statetype s_bossdie1;
extern statetype s_schabbdie1;
extern statetype s_fakedie1;
extern statetype s_mechadie1;
extern statetype s_hitlerdie1;
extern statetype s_greteldie1;
extern statetype s_giftdie1;
extern statetype s_fatdie1;

extern statetype s_spectredie1;
extern statetype s_angeldie1;
extern statetype s_transdie0;
extern statetype s_uberdie0;
extern statetype s_willdie1;
extern statetype s_deathdie1;

extern statetype s_grdchase1;
extern statetype s_dogchase1;
extern statetype s_ofcchase1;
extern statetype s_sschase1;
extern statetype s_mutchase1;
extern statetype s_bosschase1;
extern statetype s_schabbchase1;
extern statetype s_fakechase1;
extern statetype s_mechachase1;
extern statetype s_gretelchase1;
extern statetype s_giftchase1;
extern statetype s_fatchase1;

extern statetype s_spectrechase1;
extern statetype s_angelchase1;
extern statetype s_transchase1;
extern statetype s_uberchase1;
extern statetype s_willchase1;
extern statetype s_deathchase1;

extern statetype s_blinkychase1;
extern statetype s_hitlerchase1;

extern statetype s_grdpain;
extern statetype s_grdpain1;
extern statetype s_ofcpain;
extern statetype s_ofcpain1;
extern statetype s_sspain;
extern statetype s_sspain1;
extern statetype s_mutpain;
extern statetype s_mutpain1;

extern statetype s_deathcam;

extern statetype s_schabbdeathcam2;
extern statetype s_hitlerdeathcam2;
extern statetype s_giftdeathcam2;
extern statetype s_fatdeathcam2;

void SpawnStand(enemy_t which, int tilex, int tiley, int dir);
void SpawnPatrol(enemy_t which, int tilex, int tiley, int dir);
void KillActor(objstruct* ob);

void SpawnDeadGuard(int tilex, int tiley);
void SpawnBoss(int tilex, int tiley);
void SpawnGretel(int tilex, int tiley);
void SpawnTrans(int tilex, int tiley);
void SpawnUber(int tilex, int tiley);
void SpawnWill(int tilex, int tiley);
void SpawnDeath(int tilex, int tiley);
void SpawnAngel(int tilex, int tiley);
void SpawnSpectre(int tilex, int tiley);
void SpawnGhosts(enemy_t which, int tilex, int tiley);
void SpawnSchabbs(int tilex, int tiley);
void SpawnGift(int tilex, int tiley);
void SpawnFat(int tilex, int tiley);
void SpawnFakeHitler(int tilex, int tiley);
void SpawnHitler(int tilex, int tiley);

void A_DeathScream(objstruct* ob);
void SpawnBJVictory();

/*
=============================================================================

                             WL_TEXT DEFINITIONS

=============================================================================
*/

extern char helpfilename[], endfilename[];

extern void HelpScreens();
extern void EndText();

/*
=============================================================================

                             MISC DEFINITIONS

=============================================================================
*/

static inline fixed FixedMul(fixed a, fixed b)
{
    return (fixed)(((int64_t)a * b + 0x8000) >> 16);
}

#ifdef PLAYDEMOLIKEORIGINAL
#define DEMOCHOOSE_ORIG_SDL(orig, sdl) ((demorecord || demoplayback) ? (orig) : (sdl))
#define DEMOCOND_ORIG                  (demorecord || demoplayback)
#define DEMOIF_SDL                     if (DEMOCOND_SDL)
#else
#define DEMOCHOOSE_ORIG_SDL(orig, sdl) (sdl)
#define DEMOCOND_ORIG                  false
#define DEMOIF_SDL
#endif
#define DEMOCOND_SDL (!DEMOCOND_ORIG)

#define ISPOINTER(x) ((((uintptr_t)(x)) & ~0xffff) != 0)

#define CHECKMALLOCRESULT(x) \
    if (!(x))                \
    Quit("Out of memory at %s:%i", __FILE__, __LINE__)

// Mingw32 includes these definitions in string.h
#ifndef __MINGW32__
#ifdef _WIN32
#define strcasecmp  stricmp
#define strncasecmp strnicmp
#define snprintf    _snprintf
#else
static inline char* itoa(int value, char* string, int radix)
{
    sprintf(string, "%d", value);
    return string;
}

static inline char* ltoa(long value, char* string, int radix)
{
    sprintf(string, "%ld", value);
    return string;
}
#endif
#endif

#define lengthof(x) (sizeof(x) / sizeof(*(x)))
#define endof(x)    ((x) + lengthof(x))

static inline word READWORD(byte*& ptr)
{
    word val = ptr[0] | ptr[1] << 8;
    ptr += 2;
    return val;
}

static inline longword READLONGWORD(byte*& ptr)
{
    longword val = ptr[0] | ptr[1] << 8 | ptr[2] << 16 | ptr[3] << 24;
    ptr += 4;
    return val;
}

//By Fabien: This prevents SDL from drawing the surface, openGL does it instead.
#include "crt.h"
//Fab's CRT Hack
#define SDL_Flip(x) CRT_DAC()

#endif
