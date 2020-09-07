#pragma once
#include "types.h"

namespace CTRPluginFramework {
#define SHORT_DESCRIPTOR 64
#define LONG_DESCRIPTOR 128
#define PUBLISHER 64

struct SMDHTitle {
    utf16 ShortDescription[SHORT_DESCRIPTOR];
    utf16 LongDescription[LONG_DESCRIPTOR];
    utf16 Publisher[PUBLISHER];
};

struct SMDH {
    char        Magic[4];
    u16         Version;
    u16         Reserved1;
    SMDHTitle   Titles[16];
    u8          Ratings[16];
    u32         Region;
    u32         MatchMakerId;
    u64         MatchMakerBitId;
    u32         Flags;
    u16         EULAVersion;
    u16         Reserved;
    u32         OptimalBannerFrame;
    u32         StreetpassId;
    u64         Rreserved2;
    u8          SmallIcon[0x480];
    u8          LargeIcon[0x1200];

    static void Init(void);

    /**
     * @brief Read whole SMDH
     * @param tid Title ID
     * @param smdh Pointer to SMDH, reads only title if not provided
     */
    static void Read(u64 tid, SMDH* smdh = nullptr);
    static void ExtractSMDHTitle(SMDHTitle& title);
    static char ShortDescription[SHORT_DESCRIPTOR];
    static char LongDescription[LONG_DESCRIPTOR];
};
} // CTRPluginFramework
