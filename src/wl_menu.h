//
// WL_MENU.H
//
#ifdef SPEAR

constexpr auto BORDCOLOR = 0x99;
constexpr auto BORD2COLOR = 0x93;
constexpr auto DEACTIVE = 0x9b;
constexpr auto BKGDCOLOR = 0x9d;

#define MenuFadeOut() VL_FadeOut(0, 255, 0, 0, 51, 10)

#else

constexpr auto BORDCOLOR = 0x29;
constexpr auto BORD2COLOR = 0x23;
constexpr auto DEACTIVE = 0x2b;
constexpr auto BKGDCOLOR = 0x2d;
constexpr auto STRIPE = 0x2c;

#define MenuFadeOut() VL_FadeOut(0, 255, 43, 0, 0, 10)

#endif

constexpr auto READCOLOR = 0x4a;
constexpr auto READHCOLOR = 0x47;
constexpr auto VIEWCOLOR = 0x7f;
constexpr auto TEXTCOLOR = 0x17;
constexpr auto HIGHLIGHT = 0x13;
#define MenuFadeIn() VL_FadeIn(0, 255, gamepal, 10)

#define MENUSONG WONDERIN_MUS

#ifndef SPEAR
#define INTROSONG NAZI_NOR_MUS
#else
#define INTROSONG XTOWER2_MUS
#endif

constexpr auto SENSITIVE = 60;
#define CENTERX ((int)screenWidth / 2)
#define CENTERY ((int)screenHeight / 2)

#define MENU_X 76
#define MENU_Y 55
#define MENU_W 178
#ifndef SPEAR
#ifndef GOODTIMES
constexpr auto MENU_H = 13 * 10 + 6;
#else
constexpr auto MENU_H = 13 * 9 + 6;
#endif
#else
constexpr auto MENU_H = 13 * 9 + 6;
#endif

constexpr auto SM_X = 48;
constexpr auto SM_W = 250;

constexpr auto SM_Y1 = 20;
constexpr auto SM_H1 = 4 * 13 - 7;
constexpr auto SM_Y2 = SM_Y1 + 5 * 13;
constexpr auto SM_H2 = 4 * 13 - 7;
constexpr auto SM_Y3 = SM_Y2 + 5 * 13;
constexpr auto SM_H3 = 3 * 13 - 7;

constexpr auto CTL_X = 24;
constexpr auto CTL_Y = 86;
constexpr auto CTL_W = 284;
constexpr auto CTL_H = 60;

constexpr auto LSM_X = 85;
constexpr auto LSM_Y = 55;
constexpr auto LSM_W = 175;
constexpr auto LSM_H = 10 * 13 + 10;

constexpr auto NM_X = 50;
constexpr auto NM_Y = 100;
constexpr auto NM_W = 225;
constexpr auto NM_H = 13 * 4 + 15;

constexpr auto NE_X = 10;
constexpr auto NE_Y = 23;
constexpr auto NE_W = 320 - NE_X * 2;
constexpr auto NE_H = 200 - NE_Y * 2;

constexpr auto CST_X = 20;
constexpr auto CST_Y = 48;
constexpr auto CST_START = 60;
constexpr auto CST_SPC = 60;

//
// TYPEDEFS
//
enum class menuitems : short {
    undefined = -1,
    newgame,
    soundmenu,
    control,
    loadgame,
    savegame,
    changeview,

#ifndef GOODTIMES
#ifndef SPEAR
    readthis,
#endif
#endif

    viewscores,
    backtodemo,
    quit
};

struct CP_iteminfo {
    short     x, y;
    short     amount;
    menuitems curpos;
    short     indent;
};

struct CP_itemtype {
    short active;
    char  string[36];
    int (*routine)(int temp1);
};

struct CustomCtrls {
    short allowed[4];
};

extern CP_itemtype MainMenu[];
extern CP_iteminfo MainItems;

enum class input_t : byte { MOUSE,
    JOYSTICK,
    KEYBOARDBTNS,
    KEYBOARDMOVE }; // FOR INPUT TYPES

//
// FUNCTION PROTOTYPES
//

void US_ControlPanel(ScanCode);

void EnableEndGameMenuItem();

void SetupControlPanel();
void SetupSaveGames();
void CleanupControlPanel();

void      DrawMenu(CP_iteminfo* item_i, CP_itemtype* items);
menuitems HandleMenu(CP_iteminfo* item_i, CP_itemtype* items, void (*routine)(menuitems w));
void      ClearMScreen();
void      DrawWindow(int x, int y, int w, int h, int wcolor);
void      DrawOutline(int x, int y, int w, int h, int color1, int color2);
void      WaitKeyUp();
void      ReadAnyControl(ControlInfo* ci);
void      TicDelay(int count);
void      CacheLump(int lumpstart, int lumpend);
void      UnCacheLump(int lumpstart, int lumpend);
int       StartCPMusic(int song);
int       Confirm(const char* string);
void      Message(const char* string);
void      CheckPause();
void      ShootSnd();
void      CheckSecretMissions();
void      BossKey();

void DrawGun(CP_iteminfo* item_i, CP_itemtype* items, int x, int* y, menuitems which, int basey, void (*routine)(menuitems w));
void DrawHalfStep(int x, int y);
void EraseGun(CP_iteminfo* item_i, CP_itemtype* items, int x, int y, menuitems which);
void SetTextColor(CP_itemtype* items, int hlight);
void DrawMenuGun(CP_iteminfo* iteminfo);
void DrawStripes(int y);

void DefineMouseBtns();
void DefineJoyBtns();
void DefineKeyBtns();
void DefineKeyMove();
void EnterCtrlData(int index, CustomCtrls* cust, void (*DrawRtn)(int), void (*PrintRtn)(int), input_t type);

void DrawMainMenu();
void DrawSoundMenu();
void DrawLoadSaveScreen(int loadsave);
void DrawNewEpisode();
void DrawNewGame();
void DrawChangeView(int view);
void DrawMouseSens();
void DrawCtlScreen();
void DrawCustomScreen();
void DrawLSAction(int which);
void DrawCustMouse(int hilight);
void DrawCustJoy(int hilight);
void DrawCustKeybd(int hilight);
void DrawCustKeys(int hilight);
void PrintCustMouse(int i);
void PrintCustJoy(int i);
void PrintCustKeybd(int i);
void PrintCustKeys(int i);

void PrintLSEntry(int w, int color);
void TrackWhichGame(menuitems w);
void DrawNewGameDiff(menuitems w);
void FixupCustom(menuitems w);

int CP_NewGame(int);
int CP_Sound(int);
int CP_LoadGame(int quick);
int CP_SaveGame(int quick);
int CP_Control(int);
int CP_ChangeView(int);
int CP_ExitOptions(int);
int CP_Quit(int);
int CP_ViewScores(int);
int CP_EndGame(int);
int CP_CheckQuick(ScanCode scancode);
int CustomControls(int);
int MouseSensitivity(int);

void CheckForEpisodes();

void FreeMusic();

//
// WL_INTER
//
struct LRstruct {
    int     kill, secret, treasure;
    int32_t time;
};

extern LRstruct LevelRatios[];

void Write(int x, int y, const char* string);
void NonShareware();
int  GetYorN(int x, int y, int pic);
