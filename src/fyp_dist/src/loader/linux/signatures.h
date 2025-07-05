#include "sigscan.h"

const SIGNATURE SIGS_ALL[] = {
    { (char*)"main_server",
        (char*)"41 55 41 54 49 89 f4 55 89 fd bf 04 00 00 00 53 48 81 ec ?? 01 00 00 48 8d 74 24 20 48 c7 44 24 28 ff ff ff ff 48 c7 44 24 20 ff ff ff ff" },
    { (char*)"main_client",
        (char*)"41 57 41 56 41 55 41 54 49 89 f4 55 89 fd bf 04 00 00 00 53 48 81 ec b8 01 00 00 48 8d 74 24 10" }
};
