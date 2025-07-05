#include "sigscan.h"
const SIGNATURE SIG_MAIN = {
    (char*)"main_client",
    //(char*)"48 89 5c 24 08 57 48 81 ec 80 00 00 00 8b f9 b9 01 00 00 00 0f 29 74 24 70 0f 29 7c 24 60 48 8b da FF 15 ?? ?? ?? ?? 48 8d 8c 24 a8 00 00 00 E8 ?? ?? ?? ??"
    (char*)"40 57 48 83 ec 30 48 c7 44 24 28 fe ff ff ff 48 89 5c 24 40 48 8b da 8b f9"
};
