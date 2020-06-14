#ifndef __ID_CA__
#define __ID_CA__

//===========================================================================

#define NUMMAPS   60
#define MAPPLANES 2

void UnCacheGrChunk(int chunk) noexcept;
void UnCacheAudioChunk(int chunk) noexcept;

//===========================================================================

typedef struct
{
    int32_t planestart[3];
    word    planelength[3];
    word    width, height;
    char    name[16];
} maptype;

//===========================================================================

extern int mapon;

extern word* mapsegs[MAPPLANES];
extern byte* audiosegs[NUMSNDCHUNKS];
extern byte* grsegs[NUMCHUNKS];

extern char extension[5];
extern char graphext[5];
extern char audioext[5];

//===========================================================================

bool CA_LoadFile(const char* filename, void** ptr);
bool CA_WriteFile(const char* filename, void* ptr, int32_t length);

int32_t CA_RLEWCompress(word* source, int32_t length, word* dest, word rlewtag);

void CA_RLEWexpand(word* source, word* dest, int32_t length, word rlewtag);

void CA_Startup(void);
void CA_Shutdown(void);

int32_t CA_CacheAudioChunk(int chunk);
void    CA_LoadAllSounds(void);

void CA_CacheGrChunk(int chunk);
void CA_CacheMap(int mapnum);

void CA_CacheScreen(int chunk);

void CA_CannotOpen(const char* name);

#endif
