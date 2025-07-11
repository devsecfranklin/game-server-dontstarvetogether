// Originally writen by Erik-JS
// https://github.com/Erik-JS/proxy-dll-64/blob/master/winmm/main.cpp
// Adapted to DST and introduced declspec(named) to avoid generating
// function prologue.
// LICENSE: MIT per original license

#define _INC_MMSYSTEM

#include <windows.h>
#include <string.h>

#include <funchook.h>

#include "signatures.h"
#include "sigscan.h"

#include "memutil/module.hh"

typedef void (*t_mod_run)();

HMODULE h_fyp_main = 0;
HMODULE originalWINMM = NULL;
UINT64 p[180];

size_t SCAN_START;
size_t SCAN_END;

int handle_dst_client_main(int argc, char** argv);
decltype(&handle_dst_client_main) orig_dst_client_main;
decltype(&handle_dst_client_main) orig_dst_client_main_cpy;
const char* msg_err_fatal = "fyp - fatal error";
funchook_t* hook_ctx;

void loadOriginalWINMM();

void load_fyp() {
    HMODULE h_fyp_main = LoadLibrary("fyp_main.dll");
    t_mod_run fn = (t_mod_run)GetProcAddress(h_fyp_main, "mod_main");
    if (0 == fn)
        MessageBoxA(0, "Could not find mod_main in fyp_main.dll (did it not get injected?)", msg_err_fatal, 0);
    fn();
}

int handle_dst_client_main(int argc, char** argv) {
    funchook_uninstall(hook_ctx, 0);
    //funchook_destroy(hook_ctx);
    load_fyp();
    return orig_dst_client_main_cpy(argc, argv);
}

void hook_main() {
    auto bounds = SectionBounds(L"dontstarve_steam_x64.exe", ".text");
    SCAN_START = std::get<0>(bounds);
    SCAN_END   = std::get<1>(bounds) + SCAN_START;
    
    hook_ctx = funchook_create();

    orig_dst_client_main = (decltype(&handle_dst_client_main))find_signature(SCAN_START, SCAN_END, SIG_MAIN);
    if (0 == orig_dst_client_main) {
        MessageBoxA(0, "Could not find main function", msg_err_fatal, 0);
        return;

    }
    orig_dst_client_main_cpy = orig_dst_client_main;
    funchook_prepare(hook_ctx,
        (void**)&orig_dst_client_main,
        (void*)handle_dst_client_main);

    funchook_install(hook_ctx, 0);

    // TODO: Do we need to destroy hook_ctx if proc exits?
}

DWORD WINAPI Start(LPVOID lpParam) {
    loadOriginalWINMM();
    load_fyp();
   // hook_main();
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        Start(0);
        break;
    case DLL_PROCESS_DETACH:
        if (originalWINMM)
            FreeLibrary(originalWINMM);
        break;
    }
    return TRUE;
}

void loadOriginalWINMM() {
    char dllpath[MAX_PATH];
    GetSystemDirectory(dllpath, MAX_PATH);
    strncat(dllpath, "\\winmm.dll", MAX_PATH-1);
    originalWINMM = LoadLibrary(dllpath);
    if (!originalWINMM)
        return;
    p[0] = (UINT64)GetProcAddress(originalWINMM, "CloseDriver");
    p[1] = (UINT64)GetProcAddress(originalWINMM, "DefDriverProc");
    p[2] = (UINT64)GetProcAddress(originalWINMM, "DriverCallback");
    p[3] = (UINT64)GetProcAddress(originalWINMM, "DrvGetModuleHandle");
    p[4] = (UINT64)GetProcAddress(originalWINMM, "GetDriverModuleHandle");
    p[5] = (UINT64)GetProcAddress(originalWINMM, "OpenDriver");
    p[6] = (UINT64)GetProcAddress(originalWINMM, "PlaySound");
    p[7] = (UINT64)GetProcAddress(originalWINMM, "PlaySoundA");
    p[8] = (UINT64)GetProcAddress(originalWINMM, "PlaySoundW");
    p[9] = (UINT64)GetProcAddress(originalWINMM, "SendDriverMessage");
    p[10] = (UINT64)GetProcAddress(originalWINMM, "WOWAppExit");
    p[11] = (UINT64)GetProcAddress(originalWINMM, "auxGetDevCapsA");
    p[12] = (UINT64)GetProcAddress(originalWINMM, "auxGetDevCapsW");
    p[13] = (UINT64)GetProcAddress(originalWINMM, "auxGetNumDevs");
    p[14] = (UINT64)GetProcAddress(originalWINMM, "auxGetVolume");
    p[15] = (UINT64)GetProcAddress(originalWINMM, "auxOutMessage");
    p[16] = (UINT64)GetProcAddress(originalWINMM, "auxSetVolume");
    p[17] = (UINT64)GetProcAddress(originalWINMM, "joyConfigChanged");
    p[18] = (UINT64)GetProcAddress(originalWINMM, "joyGetDevCapsA");
    p[19] = (UINT64)GetProcAddress(originalWINMM, "joyGetDevCapsW");
    p[20] = (UINT64)GetProcAddress(originalWINMM, "joyGetNumDevs");
    p[21] = (UINT64)GetProcAddress(originalWINMM, "joyGetPos");
    p[22] = (UINT64)GetProcAddress(originalWINMM, "joyGetPosEx");
    p[23] = (UINT64)GetProcAddress(originalWINMM, "joyGetThreshold");
    p[24] = (UINT64)GetProcAddress(originalWINMM, "joyReleaseCapture");
    p[25] = (UINT64)GetProcAddress(originalWINMM, "joySetCapture");
    p[26] = (UINT64)GetProcAddress(originalWINMM, "joySetThreshold");
    p[27] = (UINT64)GetProcAddress(originalWINMM, "mciDriverNotify");
    p[28] = (UINT64)GetProcAddress(originalWINMM, "mciDriverYield");
    p[29] = (UINT64)GetProcAddress(originalWINMM, "mciExecute");
    p[30] = (UINT64)GetProcAddress(originalWINMM, "mciFreeCommandResource");
    p[31] = (UINT64)GetProcAddress(originalWINMM, "mciGetCreatorTask");
    p[32] = (UINT64)GetProcAddress(originalWINMM, "mciGetDeviceIDA");
    p[33] = (UINT64)GetProcAddress(originalWINMM, "mciGetDeviceIDFromElementIDA");
    p[34] = (UINT64)GetProcAddress(originalWINMM, "mciGetDeviceIDFromElementIDW");
    p[35] = (UINT64)GetProcAddress(originalWINMM, "mciGetDeviceIDW");
    p[36] = (UINT64)GetProcAddress(originalWINMM, "mciGetDriverData");
    p[37] = (UINT64)GetProcAddress(originalWINMM, "mciGetErrorStringA");
    p[38] = (UINT64)GetProcAddress(originalWINMM, "mciGetErrorStringW");
    p[39] = (UINT64)GetProcAddress(originalWINMM, "mciGetYieldProc");
    p[40] = (UINT64)GetProcAddress(originalWINMM, "mciLoadCommandResource");
    p[41] = (UINT64)GetProcAddress(originalWINMM, "mciSendCommandA");
    p[42] = (UINT64)GetProcAddress(originalWINMM, "mciSendCommandW");
    p[43] = (UINT64)GetProcAddress(originalWINMM, "mciSendStringA");
    p[44] = (UINT64)GetProcAddress(originalWINMM, "mciSendStringW");
    p[45] = (UINT64)GetProcAddress(originalWINMM, "mciSetDriverData");
    p[46] = (UINT64)GetProcAddress(originalWINMM, "mciSetYieldProc");
    p[47] = (UINT64)GetProcAddress(originalWINMM, "midiConnect");
    p[48] = (UINT64)GetProcAddress(originalWINMM, "midiDisconnect");
    p[49] = (UINT64)GetProcAddress(originalWINMM, "midiInAddBuffer");
    p[50] = (UINT64)GetProcAddress(originalWINMM, "midiInClose");
    p[51] = (UINT64)GetProcAddress(originalWINMM, "midiInGetDevCapsA");
    p[52] = (UINT64)GetProcAddress(originalWINMM, "midiInGetDevCapsW");
    p[53] = (UINT64)GetProcAddress(originalWINMM, "midiInGetErrorTextA");
    p[54] = (UINT64)GetProcAddress(originalWINMM, "midiInGetErrorTextW");
    p[55] = (UINT64)GetProcAddress(originalWINMM, "midiInGetID");
    p[56] = (UINT64)GetProcAddress(originalWINMM, "midiInGetNumDevs");
    p[57] = (UINT64)GetProcAddress(originalWINMM, "midiInMessage");
    p[58] = (UINT64)GetProcAddress(originalWINMM, "midiInOpen");
    p[59] = (UINT64)GetProcAddress(originalWINMM, "midiInPrepareHeader");
    p[60] = (UINT64)GetProcAddress(originalWINMM, "midiInReset");
    p[61] = (UINT64)GetProcAddress(originalWINMM, "midiInStart");
    p[62] = (UINT64)GetProcAddress(originalWINMM, "midiInStop");
    p[63] = (UINT64)GetProcAddress(originalWINMM, "midiInUnprepareHeader");
    p[64] = (UINT64)GetProcAddress(originalWINMM, "midiOutCacheDrumPatches");
    p[65] = (UINT64)GetProcAddress(originalWINMM, "midiOutCachePatches");
    p[66] = (UINT64)GetProcAddress(originalWINMM, "midiOutClose");
    p[67] = (UINT64)GetProcAddress(originalWINMM, "midiOutGetDevCapsA");
    p[68] = (UINT64)GetProcAddress(originalWINMM, "midiOutGetDevCapsW");
    p[69] = (UINT64)GetProcAddress(originalWINMM, "midiOutGetErrorTextA");
    p[70] = (UINT64)GetProcAddress(originalWINMM, "midiOutGetErrorTextW");
    p[71] = (UINT64)GetProcAddress(originalWINMM, "midiOutGetID");
    p[72] = (UINT64)GetProcAddress(originalWINMM, "midiOutGetNumDevs");
    p[73] = (UINT64)GetProcAddress(originalWINMM, "midiOutGetVolume");
    p[74] = (UINT64)GetProcAddress(originalWINMM, "midiOutLongMsg");
    p[75] = (UINT64)GetProcAddress(originalWINMM, "midiOutMessage");
    p[76] = (UINT64)GetProcAddress(originalWINMM, "midiOutOpen");
    p[77] = (UINT64)GetProcAddress(originalWINMM, "midiOutPrepareHeader");
    p[78] = (UINT64)GetProcAddress(originalWINMM, "midiOutReset");
    p[79] = (UINT64)GetProcAddress(originalWINMM, "midiOutSetVolume");
    p[80] = (UINT64)GetProcAddress(originalWINMM, "midiOutShortMsg");
    p[81] = (UINT64)GetProcAddress(originalWINMM, "midiOutUnprepareHeader");
    p[82] = (UINT64)GetProcAddress(originalWINMM, "midiStreamClose");
    p[83] = (UINT64)GetProcAddress(originalWINMM, "midiStreamOpen");
    p[84] = (UINT64)GetProcAddress(originalWINMM, "midiStreamOut");
    p[85] = (UINT64)GetProcAddress(originalWINMM, "midiStreamPause");
    p[86] = (UINT64)GetProcAddress(originalWINMM, "midiStreamPosition");
    p[87] = (UINT64)GetProcAddress(originalWINMM, "midiStreamProperty");
    p[88] = (UINT64)GetProcAddress(originalWINMM, "midiStreamRestart");
    p[89] = (UINT64)GetProcAddress(originalWINMM, "midiStreamStop");
    p[90] = (UINT64)GetProcAddress(originalWINMM, "mixerClose");
    p[91] = (UINT64)GetProcAddress(originalWINMM, "mixerGetControlDetailsA");
    p[92] = (UINT64)GetProcAddress(originalWINMM, "mixerGetControlDetailsW");
    p[93] = (UINT64)GetProcAddress(originalWINMM, "mixerGetDevCapsA");
    p[94] = (UINT64)GetProcAddress(originalWINMM, "mixerGetDevCapsW");
    p[95] = (UINT64)GetProcAddress(originalWINMM, "mixerGetID");
    p[96] = (UINT64)GetProcAddress(originalWINMM, "mixerGetLineControlsA");
    p[97] = (UINT64)GetProcAddress(originalWINMM, "mixerGetLineControlsW");
    p[98] = (UINT64)GetProcAddress(originalWINMM, "mixerGetLineInfoA");
    p[99] = (UINT64)GetProcAddress(originalWINMM, "mixerGetLineInfoW");
    p[100] = (UINT64)GetProcAddress(originalWINMM, "mixerGetNumDevs");
    p[101] = (UINT64)GetProcAddress(originalWINMM, "mixerMessage");
    p[102] = (UINT64)GetProcAddress(originalWINMM, "mixerOpen");
    p[103] = (UINT64)GetProcAddress(originalWINMM, "mixerSetControlDetails");
    p[104] = (UINT64)GetProcAddress(originalWINMM, "mmDrvInstall");
    p[105] = (UINT64)GetProcAddress(originalWINMM, "mmGetCurrentTask");
    p[106] = (UINT64)GetProcAddress(originalWINMM, "mmTaskBlock");
    p[107] = (UINT64)GetProcAddress(originalWINMM, "mmTaskCreate");
    p[108] = (UINT64)GetProcAddress(originalWINMM, "mmTaskSignal");
    p[109] = (UINT64)GetProcAddress(originalWINMM, "mmTaskYield");
    p[110] = (UINT64)GetProcAddress(originalWINMM, "mmioAdvance");
    p[111] = (UINT64)GetProcAddress(originalWINMM, "mmioAscend");
    p[112] = (UINT64)GetProcAddress(originalWINMM, "mmioClose");
    p[113] = (UINT64)GetProcAddress(originalWINMM, "mmioCreateChunk");
    p[114] = (UINT64)GetProcAddress(originalWINMM, "mmioDescend");
    p[115] = (UINT64)GetProcAddress(originalWINMM, "mmioFlush");
    p[116] = (UINT64)GetProcAddress(originalWINMM, "mmioGetInfo");
    p[117] = (UINT64)GetProcAddress(originalWINMM, "mmioInstallIOProcA");
    p[118] = (UINT64)GetProcAddress(originalWINMM, "mmioInstallIOProcW");
    p[119] = (UINT64)GetProcAddress(originalWINMM, "mmioOpenA");
    p[120] = (UINT64)GetProcAddress(originalWINMM, "mmioOpenW");
    p[121] = (UINT64)GetProcAddress(originalWINMM, "mmioRead");
    p[122] = (UINT64)GetProcAddress(originalWINMM, "mmioRenameA");
    p[123] = (UINT64)GetProcAddress(originalWINMM, "mmioRenameW");
    p[124] = (UINT64)GetProcAddress(originalWINMM, "mmioSeek");
    p[125] = (UINT64)GetProcAddress(originalWINMM, "mmioSendMessage");
    p[126] = (UINT64)GetProcAddress(originalWINMM, "mmioSetBuffer");
    p[127] = (UINT64)GetProcAddress(originalWINMM, "mmioSetInfo");
    p[128] = (UINT64)GetProcAddress(originalWINMM, "mmioStringToFOURCCA");
    p[129] = (UINT64)GetProcAddress(originalWINMM, "mmioStringToFOURCCW");
    p[130] = (UINT64)GetProcAddress(originalWINMM, "mmioWrite");
    p[131] = (UINT64)GetProcAddress(originalWINMM, "mmsystemGetVersion");
    p[132] = (UINT64)GetProcAddress(originalWINMM, "sndPlaySoundA");
    p[133] = (UINT64)GetProcAddress(originalWINMM, "sndPlaySoundW");
    p[134] = (UINT64)GetProcAddress(originalWINMM, "timeBeginPeriod");
    p[135] = (UINT64)GetProcAddress(originalWINMM, "timeEndPeriod");
    p[136] = (UINT64)GetProcAddress(originalWINMM, "timeGetDevCaps");
    p[137] = (UINT64)GetProcAddress(originalWINMM, "timeGetSystemTime");
    p[138] = (UINT64)GetProcAddress(originalWINMM, "timeGetTime");
    p[139] = (UINT64)GetProcAddress(originalWINMM, "timeKillEvent");
    p[140] = (UINT64)GetProcAddress(originalWINMM, "timeSetEvent");
    p[141] = (UINT64)GetProcAddress(originalWINMM, "waveInAddBuffer");
    p[142] = (UINT64)GetProcAddress(originalWINMM, "waveInClose");
    p[143] = (UINT64)GetProcAddress(originalWINMM, "waveInGetDevCapsA");
    p[144] = (UINT64)GetProcAddress(originalWINMM, "waveInGetDevCapsW");
    p[145] = (UINT64)GetProcAddress(originalWINMM, "waveInGetErrorTextA");
    p[146] = (UINT64)GetProcAddress(originalWINMM, "waveInGetErrorTextW");
    p[147] = (UINT64)GetProcAddress(originalWINMM, "waveInGetID");
    p[148] = (UINT64)GetProcAddress(originalWINMM, "waveInGetNumDevs");
    p[149] = (UINT64)GetProcAddress(originalWINMM, "waveInGetPosition");
    p[150] = (UINT64)GetProcAddress(originalWINMM, "waveInMessage");
    p[151] = (UINT64)GetProcAddress(originalWINMM, "waveInOpen");
    p[152] = (UINT64)GetProcAddress(originalWINMM, "waveInPrepareHeader");
    p[153] = (UINT64)GetProcAddress(originalWINMM, "waveInReset");
    p[154] = (UINT64)GetProcAddress(originalWINMM, "waveInStart");
    p[155] = (UINT64)GetProcAddress(originalWINMM, "waveInStop");
    p[156] = (UINT64)GetProcAddress(originalWINMM, "waveInUnprepareHeader");
    p[157] = (UINT64)GetProcAddress(originalWINMM, "waveOutBreakLoop");
    p[158] = (UINT64)GetProcAddress(originalWINMM, "waveOutClose");
    p[159] = (UINT64)GetProcAddress(originalWINMM, "waveOutGetDevCapsA");
    p[160] = (UINT64)GetProcAddress(originalWINMM, "waveOutGetDevCapsW");
    p[161] = (UINT64)GetProcAddress(originalWINMM, "waveOutGetErrorTextA");
    p[162] = (UINT64)GetProcAddress(originalWINMM, "waveOutGetErrorTextW");
    p[163] = (UINT64)GetProcAddress(originalWINMM, "waveOutGetID");
    p[164] = (UINT64)GetProcAddress(originalWINMM, "waveOutGetNumDevs");
    p[165] = (UINT64)GetProcAddress(originalWINMM, "waveOutGetPitch");
    p[166] = (UINT64)GetProcAddress(originalWINMM, "waveOutGetPlaybackRate");
    p[167] = (UINT64)GetProcAddress(originalWINMM, "waveOutGetPosition");
    p[168] = (UINT64)GetProcAddress(originalWINMM, "waveOutGetVolume");
    p[169] = (UINT64)GetProcAddress(originalWINMM, "waveOutMessage");
    p[170] = (UINT64)GetProcAddress(originalWINMM, "waveOutOpen");
    p[171] = (UINT64)GetProcAddress(originalWINMM, "waveOutPause");
    p[172] = (UINT64)GetProcAddress(originalWINMM, "waveOutPrepareHeader");
    p[173] = (UINT64)GetProcAddress(originalWINMM, "waveOutReset");
    p[174] = (UINT64)GetProcAddress(originalWINMM, "waveOutRestart");
    p[175] = (UINT64)GetProcAddress(originalWINMM, "waveOutSetPitch");
    p[176] = (UINT64)GetProcAddress(originalWINMM, "waveOutSetPlaybackRate");
    p[177] = (UINT64)GetProcAddress(originalWINMM, "waveOutSetVolume");
    p[178] = (UINT64)GetProcAddress(originalWINMM, "waveOutUnprepareHeader");
    p[179] = (UINT64)GetProcAddress(originalWINMM, "waveOutWrite");
}

extern "C" __declspec(dllexport) __declspec(naked) void CloseDriver() {
    asm("jmp *p + 0 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void DefDriverProc() {
    asm("jmp *p + 1 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void DriverCallback() {
    asm("jmp *p + 2 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void DrvGetModuleHandle() {
    asm("jmp *p + 3 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void GetDriverModuleHandle() {
    asm("jmp *p + 4 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void OpenDriver() {
    asm("jmp *p + 5 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void PlaySound() {
    asm("jmp *p + 6 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void PlaySoundA() {
    asm("jmp *p + 7 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void PlaySoundW() {
    asm("jmp *p + 8 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void SendDriverMessage() {
    asm("jmp *p + 9 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void WOWAppExit() {
    asm("jmp *p + 10 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void auxGetDevCapsA() {
    asm("jmp *p + 11 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void auxGetDevCapsW() {
    asm("jmp *p + 12 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void auxGetNumDevs() {
    asm("jmp *p + 13 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void auxGetVolume() {
    asm("jmp *p + 14 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void auxOutMessage() {
    asm("jmp *p + 15 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void auxSetVolume() {
    asm("jmp *p + 16 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void joyConfigChanged() {
    asm("jmp *p + 17 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void joyGetDevCapsA() {
    asm("jmp *p + 18 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void joyGetDevCapsW() {
    asm("jmp *p + 19 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void joyGetNumDevs() {
    asm("jmp *p + 20 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void joyGetPos() {
    asm("jmp *p + 21 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void joyGetPosEx() {
    asm("jmp *p + 22 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void joyGetThreshold() {
    asm("jmp *p + 23 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void joyReleaseCapture() {
    asm("jmp *p + 24 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void joySetCapture() {
    asm("jmp *p + 25 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void joySetThreshold() {
    asm("jmp *p + 26 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mciDriverNotify() {
    asm("jmp *p + 27 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mciDriverYield() {
    asm("jmp *p + 28 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mciExecute() {
    asm("jmp *p + 29 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mciFreeCommandResource() {
    asm("jmp *p + 30 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mciGetCreatorTask() {
    asm("jmp *p + 31 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mciGetDeviceIDA() {
    asm("jmp *p + 32 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mciGetDeviceIDFromElementIDA() {
    asm("jmp *p + 33 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mciGetDeviceIDFromElementIDW() {
    asm("jmp *p + 34 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mciGetDeviceIDW() {
    asm("jmp *p + 35 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mciGetDriverData() {
    asm("jmp *p + 36 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mciGetErrorStringA() {
    asm("jmp *p + 37 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mciGetErrorStringW() {
    asm("jmp *p + 38 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mciGetYieldProc() {
    asm("jmp *p + 39 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mciLoadCommandResource() {
    asm("jmp *p + 40 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mciSendCommandA() {
    asm("jmp *p + 41 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mciSendCommandW() {
    asm("jmp *p + 42 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mciSendStringA() {
    asm("jmp *p + 43 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mciSendStringW() {
    asm("jmp *p + 44 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mciSetDriverData() {
    asm("jmp *p + 45 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mciSetYieldProc() {
    asm("jmp *p + 46 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiConnect() {
    asm("jmp *p + 47 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiDisconnect() {
    asm("jmp *p + 48 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiInAddBuffer() {
    asm("jmp *p + 49 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiInClose() {
    asm("jmp *p + 50 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiInGetDevCapsA() {
    asm("jmp *p + 51 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiInGetDevCapsW() {
    asm("jmp *p + 52 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiInGetErrorTextA() {
    asm("jmp *p + 53 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiInGetErrorTextW() {
    asm("jmp *p + 54 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiInGetID() {
    asm("jmp *p + 55 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiInGetNumDevs() {
    asm("jmp *p + 56 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiInMessage() {
    asm("jmp *p + 57 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiInOpen() {
    asm("jmp *p + 58 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiInPrepareHeader() {
    asm("jmp *p + 59 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiInReset() {
    asm("jmp *p + 60 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiInStart() {
    asm("jmp *p + 61 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiInStop() {
    asm("jmp *p + 62 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiInUnprepareHeader() {
    asm("jmp *p + 63 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiOutCacheDrumPatches() {
    asm("jmp *p + 64 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiOutCachePatches() {
    asm("jmp *p + 65 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiOutClose() {
    asm("jmp *p + 66 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiOutGetDevCapsA() {
    asm("jmp *p + 67 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiOutGetDevCapsW() {
    asm("jmp *p + 68 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiOutGetErrorTextA() {
    asm("jmp *p + 69 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiOutGetErrorTextW() {
    asm("jmp *p + 70 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiOutGetID() {
    asm("jmp *p + 71 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiOutGetNumDevs() {
    asm("jmp *p + 72 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiOutGetVolume() {
    asm("jmp *p + 73 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiOutLongMsg() {
    asm("jmp *p + 74 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiOutMessage() {
    asm("jmp *p + 75 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiOutOpen() {
    asm("jmp *p + 76 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiOutPrepareHeader() {
    asm("jmp *p + 77 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiOutReset() {
    asm("jmp *p + 78 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiOutSetVolume() {
    asm("jmp *p + 79 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiOutShortMsg() {
    asm("jmp *p + 80 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiOutUnprepareHeader() {
    asm("jmp *p + 81 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiStreamClose() {
    asm("jmp *p + 82 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiStreamOpen() {
    asm("jmp *p + 83 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiStreamOut() {
    asm("jmp *p + 84 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiStreamPause() {
    asm("jmp *p + 85 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiStreamPosition() {
    asm("jmp *p + 86 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiStreamProperty() {
    asm("jmp *p + 87 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiStreamRestart() {
    asm("jmp *p + 88 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void midiStreamStop() {
    asm("jmp *p + 89 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mixerClose() {
    asm("jmp *p + 90 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mixerGetControlDetailsA() {
    asm("jmp *p + 91 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mixerGetControlDetailsW() {
    asm("jmp *p + 92 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mixerGetDevCapsA() {
    asm("jmp *p + 93 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mixerGetDevCapsW() {
    asm("jmp *p + 94 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mixerGetID() {
    asm("jmp *p + 95 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mixerGetLineControlsA() {
    asm("jmp *p + 96 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mixerGetLineControlsW() {
    asm("jmp *p + 97 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mixerGetLineInfoA() {
    asm("jmp *p + 98 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mixerGetLineInfoW() {
    asm("jmp *p + 99 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mixerGetNumDevs() {
    asm("jmp *p + 100 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mixerMessage() {
    asm("jmp *p + 101 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mixerOpen() {
    asm("jmp *p + 102 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mixerSetControlDetails() {
    asm("jmp *p + 103 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mmDrvInstall() {
    asm("jmp *p + 104 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mmGetCurrentTask() {
    asm("jmp *p + 105 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mmTaskBlock() {
    asm("jmp *p + 106 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mmTaskCreate() {
    asm("jmp *p + 107 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mmTaskSignal() {
    asm("jmp *p + 108 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mmTaskYield() {
    asm("jmp *p + 109 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mmioAdvance() {
    asm("jmp *p + 110 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mmioAscend() {
    asm("jmp *p + 111 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mmioClose() {
    asm("jmp *p + 112 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mmioCreateChunk() {
    asm("jmp *p + 113 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mmioDescend() {
    asm("jmp *p + 114 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mmioFlush() {
    asm("jmp *p + 115 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mmioGetInfo() {
    asm("jmp *p + 116 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mmioInstallIOProcA() {
    asm("jmp *p + 117 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mmioInstallIOProcW() {
    asm("jmp *p + 118 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mmioOpenA() {
    asm("jmp *p + 119 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mmioOpenW() {
    asm("jmp *p + 120 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mmioRead() {
    asm("jmp *p + 121 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mmioRenameA() {
    asm("jmp *p + 122 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mmioRenameW() {
    asm("jmp *p + 123 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mmioSeek() {
    asm("jmp *p + 124 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mmioSendMessage() {
    asm("jmp *p + 125 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mmioSetBuffer() {
    asm("jmp *p + 126 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mmioSetInfo() {
    asm("jmp *p + 127 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mmioStringToFOURCCA() {
    asm("jmp *p + 128 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mmioStringToFOURCCW() {
    asm("jmp *p + 129 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mmioWrite() {
    asm("jmp *p + 130 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void mmsystemGetVersion() {
    asm("jmp *p + 131 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void sndPlaySoundA() {
    asm("jmp *p + 132 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void sndPlaySoundW() {
    asm("jmp *p + 133 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void timeBeginPeriod() {
    asm("jmp *p + 134 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void timeEndPeriod() {
    asm("jmp *p + 135 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void timeGetDevCaps() {
    asm("jmp *p + 136 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void timeGetSystemTime() {
    asm("jmp *p + 137 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void timeGetTime() {
    asm("jmp *p + 138 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void timeKillEvent() {
    asm("jmp *p + 139 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void timeSetEvent() {
    asm("jmp *p + 140 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveInAddBuffer() {
    asm("jmp *p + 141 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveInClose() {
    asm("jmp *p + 142 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveInGetDevCapsA() {
    asm("jmp *p + 143 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveInGetDevCapsW() {
    asm("jmp *p + 144 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveInGetErrorTextA() {
    asm("jmp *p + 145 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveInGetErrorTextW() {
    asm("jmp *p + 146 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveInGetID() {
    asm("jmp *p + 147 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveInGetNumDevs() {
    asm("jmp *p + 148 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveInGetPosition() {
    asm("jmp *p + 149 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveInMessage() {
    asm("jmp *p + 150 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveInOpen() {
    asm("jmp *p + 151 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveInPrepareHeader() {
    asm("jmp *p + 152 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveInReset() {
    asm("jmp *p + 153 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveInStart() {
    asm("jmp *p + 154 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveInStop() {
    asm("jmp *p + 155 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveInUnprepareHeader() {
    asm("jmp *p + 156 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveOutBreakLoop() {
    asm("jmp *p + 157 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveOutClose() {
    asm("jmp *p + 158 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveOutGetDevCapsA() {
    asm("jmp *p + 159 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveOutGetDevCapsW() {
    asm("jmp *p + 160 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveOutGetErrorTextA() {
    asm("jmp *p + 161 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveOutGetErrorTextW() {
    asm("jmp *p + 162 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveOutGetID() {
    asm("jmp *p + 163 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveOutGetNumDevs() {
    asm("jmp *p + 164 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveOutGetPitch() {
    asm("jmp *p + 165 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveOutGetPlaybackRate() {
    asm("jmp *p + 166 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveOutGetPosition() {
    asm("jmp *p + 167 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveOutGetVolume() {
    asm("jmp *p + 168 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveOutMessage() {
    asm("jmp *p + 169 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveOutOpen() {
    asm("jmp *p + 170 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveOutPause() {
    asm("jmp *p + 171 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveOutPrepareHeader() {
    asm("jmp *p + 172 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveOutReset() {
    asm("jmp *p + 173 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveOutRestart() {
    asm("jmp *p + 174 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveOutSetPitch() {
    asm("jmp *p + 175 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveOutSetPlaybackRate() {
    asm("jmp *p + 176 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveOutSetVolume() {
    asm("jmp *p + 177 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveOutUnprepareHeader() {
    asm("jmp *p + 178 * 8");
}

extern "C" __declspec(dllexport) __declspec(naked) void waveOutWrite() {
    asm("jmp *p + 179 * 8");
}
