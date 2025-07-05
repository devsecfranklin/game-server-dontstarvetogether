/****************************************************************************************
* Copyright © 2018-2023, Jovibor: https://github.com/jovibor/                           *
* libpe is a library for obtaining PE32 (x86) and PE32+ (x64) files' inner structure.   *
* Official git repository: https://github.com/jovibor/libpe                             *
* This software is available under the "MIT License".                                   *
****************************************************************************************/
#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wintrust.h> //WIN_CERTIFICATE struct.
#include <memory>
#include <span>
#include <string>
#include <unordered_map>
#include <vector>

#ifndef __cpp_lib_span
static_assert(false, "C++20 compiler is required for libpe, MSVS 16.11.14 or higher.");
#endif

#ifndef __x86_64__
#define __x86_64__
#endif

#ifndef _WIN64
#define _WIN64
#endif

// Make PE32+ related definitions that are missing from mingw's headers.
#define	IMAGE_FILE_MACHINE_UNKNOWN	0
#define	IMAGE_FILE_MACHINE_TARGET_HOST  0x0001
#define	IMAGE_FILE_MACHINE_I860		0x014d
#define	IMAGE_FILE_MACHINE_I386		0x014c
#define	IMAGE_FILE_MACHINE_R3000	0x0162
#define	IMAGE_FILE_MACHINE_R4000	0x0166
#define	IMAGE_FILE_MACHINE_R10000	0x0168
#define	IMAGE_FILE_MACHINE_WCEMIPSV2	0x0169
#define	IMAGE_FILE_MACHINE_ALPHA	0x0184
#define	IMAGE_FILE_MACHINE_SH3		0x01a2
#define	IMAGE_FILE_MACHINE_SH3DSP	0x01a3
#define	IMAGE_FILE_MACHINE_SH3E		0x01a4
#define	IMAGE_FILE_MACHINE_SH4		0x01a6
#define	IMAGE_FILE_MACHINE_SH5		0x01a8
#define	IMAGE_FILE_MACHINE_ARM		0x01c0
#define	IMAGE_FILE_MACHINE_THUMB	0x01c2
#define	IMAGE_FILE_MACHINE_ARMNT	0x01c4
#define	IMAGE_FILE_MACHINE_ARM64	0xaa64
#define	IMAGE_FILE_MACHINE_AM33		0x01d3
//#define	IMAGE_FILE_MACHINE_POWERPC	0x01f0
#define	IMAGE_FILE_MACHINE_POWERPCFP	0x01f1
#define	IMAGE_FILE_MACHINE_IA64		0x0200
#define	IMAGE_FILE_MACHINE_MIPS16	0x0266
#define	IMAGE_FILE_MACHINE_ALPHA64	0x0284
#define	IMAGE_FILE_MACHINE_MIPSFPU	0x0366
#define	IMAGE_FILE_MACHINE_MIPSFPU16	0x0466
#define	IMAGE_FILE_MACHINE_AXP64	IMAGE_FILE_MACHINE_ALPHA64
#define	IMAGE_FILE_MACHINE_TRICORE	0x0520
//#define	IMAGE_FILE_MACHINE_CEF		0x0cef
//#define	IMAGE_FILE_MACHINE_EBC		0x0ebc
#define	IMAGE_FILE_MACHINE_AMD64	0x8664
#define	IMAGE_FILE_MACHINE_M32R		0x9041
#define	IMAGE_FILE_MACHINE_CEE		0xc0ee

#define IMAGE_GUARD_CF_INSTRUMENTED                    0x00000100 // Module performs control flow integrity checks using system-supplied support
#define IMAGE_GUARD_CFW_INSTRUMENTED                   0x00000200 // Module performs control flow and write integrity checks
#define IMAGE_GUARD_CF_FUNCTION_TABLE_PRESENT          0x00000400 // Module contains valid control flow target metadata
#define IMAGE_GUARD_SECURITY_COOKIE_UNUSED             0x00000800 // Module does not make use of the /GS security cookie
#define IMAGE_GUARD_PROTECT_DELAYLOAD_IAT              0x00001000 // Module supports read only delay load IAT
#define IMAGE_GUARD_DELAYLOAD_IAT_IN_ITS_OWN_SECTION   0x00002000 // Delayload import table in its own .didat section (with nothing else in it) that can be freely reprotected
#define IMAGE_GUARD_CF_EXPORT_SUPPRESSION_INFO_PRESENT 0x00004000 // Module contains suppressed export information. This also infers that the address taken
// taken IAT table is also present in the load config.
#define IMAGE_GUARD_CF_ENABLE_EXPORT_SUPPRESSION       0x00008000 // Module enables suppression of exports
#define IMAGE_GUARD_CF_LONGJUMP_TABLE_PRESENT          0x00010000 // Module contains longjmp target information
#define IMAGE_GUARD_RF_INSTRUMENTED                    0x00020000 // Module contains return flow instrumentation and metadata
#define IMAGE_GUARD_RF_ENABLE                          0x00040000 // Module requests that the OS enable return flow protection
#define IMAGE_GUARD_RF_STRICT                          0x00080000 // Module requests that the OS enable return flow protection in strict mode
#define IMAGE_GUARD_RETPOLINE_PRESENT                  0x00100000 // Module was built with retpoline support
#define IMAGE_GUARD_CF_FUNCTION_TABLE_SIZE_MASK        0xF0000000 // Stride of Guard CF function table encoded in these bits (additional count of bytes per element)
#define IMAGE_GUARD_CF_FUNCTION_TABLE_SIZE_SHIFT       28         // Shift to right-justify Guard CF function table stride

#ifndef PROCESSOR_ARCHITECTURE_ARM64
#define PROCESSOR_ARCHITECTURE_ARM64 12
#endif

#ifndef PROCESSOR_ARCHITECTURE_ARM32_ON_WIN64
#define PROCESSOR_ARCHITECTURE_ARM32_ON_WIN64 13
#endif
#ifndef PROCESSOR_ARCHITECTURE_IA32_ON_ARM64
#define PROCESSOR_ARCHITECTURE_IA32_ON_ARM64 14
#endif

#ifndef IMAGE_FILE_MACHINE_TARGET_HOST
#define IMAGE_FILE_MACHINE_TARGET_HOST                                                             \
  0x0001 // Useful for indicating we want to interact with the host and not a
         // WoW guest.
#endif

/// #define PROCESSOR_ARCHITECTURE_ARM32_ON_WIN64   13
#ifndef IMAGE_FILE_MACHINE_ARM64
//// IMAGE_FILE_MACHINE_ARM64 is Windows
#define IMAGE_FILE_MACHINE_ARM64 0xAA64 // ARM64 Little-Endian
#endif

#ifndef IMAGE_FILE_MACHINE_RISCV32
#define IMAGE_FILE_MACHINE_RISCV32 0x5032
#endif
#ifndef IMAGE_FILE_MACHINE_RISCV64
#define IMAGE_FILE_MACHINE_RISCV64 0x5064
#endif
#ifndef IMAGE_FILE_MACHINE_RISCV128
#define IMAGE_FILE_MACHINE_RISCV128 0x5128
#endif

#ifndef IMAGE_FILE_MACHINE_CHPE_X86
#define IMAGE_FILE_MACHINE_CHPE_X86 0x3A64 /// defined in ntimage.h
#endif

#ifndef IMAGE_SUBSYSTEM_XBOX_CODE_CATALOG
#define IMAGE_SUBSYSTEM_XBOX_CODE_CATALOG 17 // XBOX Code Catalog
#endif

//#define IMAGE_DEBUG_TYPE_UNKNOWN            UINT32_C(0x00)
//#define IMAGE_DEBUG_TYPE_COFF               UINT32_C(0x01)
//#define IMAGE_DEBUG_TYPE_CODEVIEW           UINT32_C(0x02)
//#define IMAGE_DEBUG_TYPE_FPO                UINT32_C(0x03)
//#define IMAGE_DEBUG_TYPE_MISC               UINT32_C(0x04)
//#define IMAGE_DEBUG_TYPE_EXCEPTION          UINT32_C(0x05)
//#define IMAGE_DEBUG_TYPE_FIXUP              UINT32_C(0x06)
//#define IMAGE_DEBUG_TYPE_OMAP_TO_SRC        UINT32_C(0x07)
//#define IMAGE_DEBUG_TYPE_OMAP_FROM_SRC      UINT32_C(0x08)
//#define IMAGE_DEBUG_TYPE_BORLAND            UINT32_C(0x09)
//#define IMAGE_DEBUG_TYPE_RESERVED10         UINT32_C(0x0a)
//#define IMAGE_DEBUG_TYPE_CLSID              UINT32_C(0x0b)
#define IMAGE_DEBUG_TYPE_VC_FEATURE         UINT32_C(0x0c)
#define IMAGE_DEBUG_TYPE_POGO               UINT32_C(0x0d)
#define IMAGE_DEBUG_TYPE_ILTCG              UINT32_C(0x0e)
#define IMAGE_DEBUG_TYPE_MPX                UINT32_C(0x0f)
#define IMAGE_DEBUG_TYPE_REPRO              UINT32_C(0x10)

#define IMAGE_REL_BASED_ABSOLUTE              0
#define IMAGE_REL_BASED_HIGH                  1
#define IMAGE_REL_BASED_LOW                   2
#define IMAGE_REL_BASED_HIGHLOW               3
#define IMAGE_REL_BASED_HIGHADJ               4
#define IMAGE_REL_BASED_MACHINE_SPECIFIC_5    5
#define IMAGE_REL_BASED_RESERVED              6
#define IMAGE_REL_BASED_MACHINE_SPECIFIC_7    7
#define IMAGE_REL_BASED_MACHINE_SPECIFIC_8    8
#define IMAGE_REL_BASED_MACHINE_SPECIFIC_9    9
#define IMAGE_REL_BASED_DIR64                 10

namespace libpe
{
	//Rich.
	struct PERICHHDR {
		DWORD dwOffset; //File's raw offset of this entry.
		WORD  wId;      //Entry Id.
		WORD  wVersion; //Entry version.
		DWORD dwCount;  //Amount of occurrences.
	};
	using PERICHHDR_VEC = std::vector<PERICHHDR>;

	//NT header.
	struct PENTHDR {
		DWORD dwOffset;   //File's raw offset of this header.
		union UNPENTHDR { //Union of either x86 or x64 NT header.
			IMAGE_NT_HEADERS32 stNTHdr32; //x86 Header.
			IMAGE_NT_HEADERS64 stNTHdr64; //x64 Header.
		} unHdr;
	};
	inline const std::unordered_map<DWORD, std::wstring_view> MapFileHdrMachine {
		{ IMAGE_FILE_MACHINE_UNKNOWN, L"IMAGE_FILE_MACHINE_UNKNOWN" },
		{ IMAGE_FILE_MACHINE_TARGET_HOST, L"IMAGE_FILE_MACHINE_TARGET_HOST" },
		{ IMAGE_FILE_MACHINE_I386, L"IMAGE_FILE_MACHINE_I386" },
		{ IMAGE_FILE_MACHINE_R3000, L"IMAGE_FILE_MACHINE_R3000" },
		{ IMAGE_FILE_MACHINE_R4000, L"IMAGE_FILE_MACHINE_R4000" },
		{ IMAGE_FILE_MACHINE_R10000, L"IMAGE_FILE_MACHINE_R10000" },
		{ IMAGE_FILE_MACHINE_WCEMIPSV2, L"IMAGE_FILE_MACHINE_WCEMIPSV2" },
		{ IMAGE_FILE_MACHINE_ALPHA, L"IMAGE_FILE_MACHINE_ALPHA" },
		{ IMAGE_FILE_MACHINE_SH3, L"IMAGE_FILE_MACHINE_SH3" },
		{ IMAGE_FILE_MACHINE_SH3DSP, L"IMAGE_FILE_MACHINE_SH3DSP" },
		{ IMAGE_FILE_MACHINE_SH3E, L"IMAGE_FILE_MACHINE_SH3E" },
		{ IMAGE_FILE_MACHINE_SH4, L"IMAGE_FILE_MACHINE_SH4" },
		{ IMAGE_FILE_MACHINE_SH5, L"IMAGE_FILE_MACHINE_SH5" },
		{ IMAGE_FILE_MACHINE_ARM, L"IMAGE_FILE_MACHINE_ARM" },
		{ IMAGE_FILE_MACHINE_THUMB, L"IMAGE_FILE_MACHINE_THUMB" },
		{ IMAGE_FILE_MACHINE_ARMNT, L"IMAGE_FILE_MACHINE_ARMNT" },
		{ IMAGE_FILE_MACHINE_AM33, L"IMAGE_FILE_MACHINE_AM33" },
		{ IMAGE_FILE_MACHINE_POWERPC, L"IMAGE_FILE_MACHINE_POWERPC" },
		{ IMAGE_FILE_MACHINE_POWERPCFP, L"IMAGE_FILE_MACHINE_POWERPCFP" },
		{ IMAGE_FILE_MACHINE_IA64, L"IMAGE_FILE_MACHINE_IA64" },
		{ IMAGE_FILE_MACHINE_MIPS16, L"IMAGE_FILE_MACHINE_MIPS16" },
		{ IMAGE_FILE_MACHINE_ALPHA64, L"IMAGE_FILE_MACHINE_ALPHA64" },
		{ IMAGE_FILE_MACHINE_MIPSFPU, L"IMAGE_FILE_MACHINE_MIPSFPU" },
		{ IMAGE_FILE_MACHINE_MIPSFPU16, L"IMAGE_FILE_MACHINE_MIPSFPU16" },
		{ IMAGE_FILE_MACHINE_TRICORE, L"IMAGE_FILE_MACHINE_TRICORE" },
		{ IMAGE_FILE_MACHINE_CEF, L"IMAGE_FILE_MACHINE_CEF" },
		{ IMAGE_FILE_MACHINE_EBC, L"IMAGE_FILE_MACHINE_EBC" },
		{ IMAGE_FILE_MACHINE_AMD64, L"IMAGE_FILE_MACHINE_AMD64" },
		{ IMAGE_FILE_MACHINE_M32R, L"IMAGE_FILE_MACHINE_M32R" },
		{ IMAGE_FILE_MACHINE_ARM64, L"IMAGE_FILE_MACHINE_ARM64" },
		{ IMAGE_FILE_MACHINE_CEE, L"IMAGE_FILE_MACHINE_CEE" },
	};
	inline const std::unordered_map<DWORD, std::wstring_view> MapFileHdrCharact {
		{ IMAGE_FILE_RELOCS_STRIPPED, L"IMAGE_FILE_RELOCS_STRIPPED" },
		{ IMAGE_FILE_EXECUTABLE_IMAGE, L"IMAGE_FILE_EXECUTABLE_IMAGE" },
		{ IMAGE_FILE_LINE_NUMS_STRIPPED, L"IMAGE_FILE_LINE_NUMS_STRIPPED" },
		{ IMAGE_FILE_LOCAL_SYMS_STRIPPED, L"IMAGE_FILE_LOCAL_SYMS_STRIPPED" },
		{ IMAGE_FILE_AGGRESIVE_WS_TRIM, L"IMAGE_FILE_AGGRESIVE_WS_TRIM" },
		{ IMAGE_FILE_LARGE_ADDRESS_AWARE, L"IMAGE_FILE_LARGE_ADDRESS_AWARE" },
		{ IMAGE_FILE_BYTES_REVERSED_LO, L"IMAGE_FILE_BYTES_REVERSED_LO" },
		{ IMAGE_FILE_32BIT_MACHINE, L"IMAGE_FILE_32BIT_MACHINE" },
		{ IMAGE_FILE_DEBUG_STRIPPED, L"IMAGE_FILE_DEBUG_STRIPPED" },
		{ IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP, L"IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP" },
		{ IMAGE_FILE_NET_RUN_FROM_SWAP, L"IMAGE_FILE_NET_RUN_FROM_SWAP" },
		{ IMAGE_FILE_SYSTEM, L"IMAGE_FILE_SYSTEM" },
		{ IMAGE_FILE_DLL, L"IMAGE_FILE_DLL" },
		{ IMAGE_FILE_UP_SYSTEM_ONLY, L"IMAGE_FILE_UP_SYSTEM_ONLY" },
		{ IMAGE_FILE_BYTES_REVERSED_HI, L"IMAGE_FILE_BYTES_REVERSED_HI" }
	};
	inline const std::unordered_map<DWORD, std::wstring_view> MapOptHdrMagic {
		{ IMAGE_NT_OPTIONAL_HDR32_MAGIC, L"IMAGE_NT_OPTIONAL_HDR32_MAGIC" },
		{ IMAGE_NT_OPTIONAL_HDR64_MAGIC, L"IMAGE_NT_OPTIONAL_HDR64_MAGIC" },
		{ IMAGE_ROM_OPTIONAL_HDR_MAGIC, L"IMAGE_ROM_OPTIONAL_HDR_MAGIC" }
	};
	inline const std::unordered_map<DWORD, std::wstring_view> MapOptHdrSubsystem {
		{ IMAGE_SUBSYSTEM_UNKNOWN, L"IMAGE_SUBSYSTEM_UNKNOWN" },
		{ IMAGE_SUBSYSTEM_NATIVE, L"IMAGE_SUBSYSTEM_NATIVE" },
		{ IMAGE_SUBSYSTEM_WINDOWS_GUI, L"IMAGE_SUBSYSTEM_WINDOWS_GUI" },
		{ IMAGE_SUBSYSTEM_WINDOWS_CUI, L"IMAGE_SUBSYSTEM_WINDOWS_CUI" },
		{ IMAGE_SUBSYSTEM_OS2_CUI, L"IMAGE_SUBSYSTEM_OS2_CUI" },
		{ IMAGE_SUBSYSTEM_POSIX_CUI, L"IMAGE_SUBSYSTEM_POSIX_CUI" },
		{ IMAGE_SUBSYSTEM_NATIVE_WINDOWS, L"IMAGE_SUBSYSTEM_NATIVE_WINDOWS" },
		{ IMAGE_SUBSYSTEM_WINDOWS_CE_GUI, L"IMAGE_SUBSYSTEM_WINDOWS_CE_GUI" },
		{ IMAGE_SUBSYSTEM_EFI_APPLICATION, L"IMAGE_SUBSYSTEM_EFI_APPLICATION" },
		{ IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER, L"IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER" },
		{ IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER, L"IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER" },
		{ IMAGE_SUBSYSTEM_EFI_ROM, L"IMAGE_SUBSYSTEM_EFI_ROM" },
		{ IMAGE_SUBSYSTEM_XBOX, L"IMAGE_SUBSYSTEM_XBOX" },
		{ IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION, L"IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION" },
		{ IMAGE_SUBSYSTEM_XBOX_CODE_CATALOG, L"IMAGE_SUBSYSTEM_XBOX_CODE_CATALOG" }
	};
	inline const std::unordered_map<DWORD, std::wstring_view> MapOptHdrDllCharact {
		{ 0x0001, L"IMAGE_LIBRARY_PROCESS_INIT" },
		{ 0x0002, L"IMAGE_LIBRARY_PROCESS_TERM" },
		{ 0x0004, L"IMAGE_LIBRARY_THREAD_INIT" },
		{ 0x0008, L"IMAGE_LIBRARY_THREAD_TERM" },
		{ IMAGE_DLLCHARACTERISTICS_HIGH_ENTROPY_VA, L"IMAGE_DLLCHARACTERISTICS_HIGH_ENTROPY_VA" },
		{ IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE, L"IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE" },
		{ IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY, L"IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY" },
		{ IMAGE_DLLCHARACTERISTICS_NX_COMPAT, L"IMAGE_DLLCHARACTERISTICS_NX_COMPAT" },
		{ IMAGE_DLLCHARACTERISTICS_NO_ISOLATION, L"IMAGE_DLLCHARACTERISTICS_NO_ISOLATION" },
		{ IMAGE_DLLCHARACTERISTICS_NO_SEH, L"IMAGE_DLLCHARACTERISTICS_NO_SEH" },
		{ IMAGE_DLLCHARACTERISTICS_NO_BIND, L"IMAGE_DLLCHARACTERISTICS_NO_BIND" },
		{ IMAGE_DLLCHARACTERISTICS_APPCONTAINER, L"IMAGE_DLLCHARACTERISTICS_APPCONTAINER" },
		{ IMAGE_DLLCHARACTERISTICS_WDM_DRIVER, L"IMAGE_DLLCHARACTERISTICS_WDM_DRIVER" },
		{ IMAGE_DLLCHARACTERISTICS_GUARD_CF, L"IMAGE_DLLCHARACTERISTICS_GUARD_CF" },
		{ IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE, L"IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE" }
	};

	//Data directories.
	struct PEDATADIR {
		IMAGE_DATA_DIRECTORY stDataDir;  //Standard header.
		std::string          strSection; //Name of the section this directory resides in (points to).
	};
	using PEDATADIR_VEC = std::vector<PEDATADIR>;

	//Sections headers.
	//For more info check:
	//docs.microsoft.com/en-us/windows/desktop/api/winnt/ns-winnt-_image_section_header#members
	//«An 8-byte, null-padded UTF-8 string. For longer names, this member contains a forward slash (/) 
	//followed by an ASCII representation of a decimal number that is an offset into the string table.»
	struct PESECHDR {
		DWORD                dwOffset;   //File's raw offset of this section header descriptor.
		IMAGE_SECTION_HEADER stSecHdr;   //Standard section header.
		std::string          strSecName; //Section full name.
	};
	using PESECHDR_VEC = std::vector<PESECHDR>;
	inline const std::unordered_map<DWORD, std::wstring_view> MapSecHdrCharact {
		{ 0x00000000, L"IMAGE_SCN_TYPE_REG (Reserved)" },
		{ 0x00000001, L"IMAGE_SCN_TYPE_DSECT (Reserved)" },
		{ 0x00000002, L"IMAGE_SCN_TYPE_NOLOAD (Reserved)" },
		{ 0x00000004, L"IMAGE_SCN_TYPE_GROUP (Reserved)" },
		{ IMAGE_SCN_TYPE_NO_PAD, L"IMAGE_SCN_TYPE_NO_PAD (Reserved)" },
		{ 0x00000010, L"IMAGE_SCN_TYPE_COPY (Reserved)" },
		{ IMAGE_SCN_CNT_CODE, L"IMAGE_SCN_CNT_CODE (Section contains code)" },
		{ IMAGE_SCN_CNT_INITIALIZED_DATA, L"IMAGE_SCN_CNT_INITIALIZED_DATA (Section contains initialized data)" },
		{ IMAGE_SCN_CNT_UNINITIALIZED_DATA, L"IMAGE_SCN_CNT_UNINITIALIZED_DATA (Section contains uninitialized data)" },
		{ IMAGE_SCN_LNK_OTHER, L"IMAGE_SCN_LNK_OTHER (Reserved)" },
		{ IMAGE_SCN_LNK_INFO, L"IMAGE_SCN_LNK_INFO (Section contains comments or some other type of information)" },
		{ 0x00000400, L"IMAGE_SCN_TYPE_OVER (Reserved)" },
		{ IMAGE_SCN_LNK_REMOVE, L"IMAGE_SCN_LNK_REMOVE (Section contents will not become part of image)" },
		{ IMAGE_SCN_LNK_COMDAT, L"IMAGE_SCN_LNK_COMDAT (Section contents comdat)" },
		{ IMAGE_SCN_NO_DEFER_SPEC_EXC, L"IMAGE_SCN_NO_DEFER_SPEC_EXC (Reset speculative exceptions handling bits in the TLB entries for this section)" },
		{ IMAGE_SCN_GPREL, L"IMAGE_SCN_GPREL (Section content can be accessed relative to GP)" },
		{ 0x00010000, L"IMAGE_SCN_MEM_SYSHEAP (Obsolete)" },
		{ IMAGE_SCN_MEM_PURGEABLE, L"IMAGE_SCN_MEM_PURGEABLE" },
		{ IMAGE_SCN_MEM_LOCKED, L"IMAGE_SCN_MEM_LOCKED" },
		{ IMAGE_SCN_MEM_PRELOAD, L"IMAGE_SCN_MEM_PRELOAD" },
		{ IMAGE_SCN_ALIGN_1BYTES, L"IMAGE_SCN_ALIGN_1BYTES" },
		{ IMAGE_SCN_ALIGN_2BYTES, L"IMAGE_SCN_ALIGN_2BYTES" },
		{ IMAGE_SCN_ALIGN_4BYTES, L"IMAGE_SCN_ALIGN_4BYTES" },
		{ IMAGE_SCN_ALIGN_8BYTES, L"IMAGE_SCN_ALIGN_8BYTES" },
		{ IMAGE_SCN_ALIGN_16BYTES, L"IMAGE_SCN_ALIGN_16BYTES (Default alignment if no others are specified)" },
		{ IMAGE_SCN_ALIGN_32BYTES, L"IMAGE_SCN_ALIGN_32BYTES" },
		{ IMAGE_SCN_ALIGN_64BYTES, L"IMAGE_SCN_ALIGN_64BYTES" },
		{ IMAGE_SCN_ALIGN_128BYTES, L"IMAGE_SCN_ALIGN_128BYTES" },
		{ IMAGE_SCN_ALIGN_256BYTES, L"IMAGE_SCN_ALIGN_256BYTES" },
		{ IMAGE_SCN_ALIGN_512BYTES, L"IMAGE_SCN_ALIGN_512BYTES" },
		{ IMAGE_SCN_ALIGN_1024BYTES, L"IMAGE_SCN_ALIGN_1024BYTES" },
		{ IMAGE_SCN_ALIGN_2048BYTES, L"IMAGE_SCN_ALIGN_2048BYTES" },
		{ IMAGE_SCN_ALIGN_4096BYTES, L"IMAGE_SCN_ALIGN_4096BYTES" },
		{ IMAGE_SCN_ALIGN_8192BYTES, L"IMAGE_SCN_ALIGN_8192BYTES" },
		{ IMAGE_SCN_ALIGN_MASK, L"IMAGE_SCN_ALIGN_MASK" },
		{ IMAGE_SCN_LNK_NRELOC_OVFL, L"IMAGE_SCN_LNK_NRELOC_OVFL (Section contains extended relocations)" },
		{ IMAGE_SCN_MEM_DISCARDABLE, L"IMAGE_SCN_MEM_DISCARDABLE (Section can be discarded)" },
		{ IMAGE_SCN_MEM_NOT_CACHED, L"IMAGE_SCN_MEM_NOT_CACHED (Section is not cachable)" },
		{ IMAGE_SCN_MEM_NOT_PAGED, L"IMAGE_SCN_MEM_NOT_PAGED (Section is not pageable)" },
		{ IMAGE_SCN_MEM_SHARED, L"IMAGE_SCN_MEM_SHARED (Section is shareable)" },
		{ IMAGE_SCN_MEM_EXECUTE, L"IMAGE_SCN_MEM_EXECUTE (Section is executable)" },
		{ IMAGE_SCN_MEM_READ, L"IMAGE_SCN_MEM_READ (Section is readable)" },
		{ IMAGE_SCN_MEM_WRITE, L"IMAGE_SCN_MEM_WRITE (Section is writeable)" }
	};

	//Export table.
	struct PEEXPORTFUNC {
		DWORD       dwFuncRVA;        //Function RVA.
		DWORD       dwOrdinal;        //Function ordinal.
		DWORD       dwNameRVA;        //Name RVA.
		std::string strFuncName;      //Function name.
		std::string strForwarderName; //Function forwarder name.
	};
	struct PEEXPORT {
		DWORD                     dwOffset;      //File's raw offset of the Export header descriptor.
		IMAGE_EXPORT_DIRECTORY    stExportDesc;  //Standard export header descriptor.
		std::string               strModuleName; //Actual module name.
		std::vector<PEEXPORTFUNC> vecFuncs;      //Array of the exported functions struct.	
	};

	//Import table:
	struct PEIMPORTFUNC {
		union UNPEIMPORTTHUNK {
			IMAGE_THUNK_DATA32 stThunk32; //x86 standard thunk.
			IMAGE_THUNK_DATA64 stThunk64; //x64 standard thunk.
		} unThunk;
		IMAGE_IMPORT_BY_NAME stImpByName; //Standard IMAGE_IMPORT_BY_NAME struct
		std::string          strFuncName; //Function name.
	};
	struct PEIMPORT {
		DWORD                     dwOffset;      //File's raw offset of this Import descriptor.
		IMAGE_IMPORT_DESCRIPTOR   stImportDesc;  //Standard Import descriptor.
		std::string               strModuleName; //Imported module name.
		std::vector<PEIMPORTFUNC> vecImportFunc; //Array of imported functions.
	};
	using PEIMPORT_VEC = std::vector<PEIMPORT>;

	/**************************************Resources by Levels*******************************************
	* There are 3 levels of resources: 1. Type 2. Name 3. Language.										*
	* https://docs.microsoft.com/en-us/windows/desktop/Debug/pe-format#the-rsrc-section					*
	* Highest (root) resource structure is PERESROOT. It's a struct	that includes:                      *
	* an IMAGE_RESOURCE_DIRECTORY of root resource directory itself and vector<PERESROOTDATA>,          *
	* that contains structs of all IMAGE_RESOURCE_DIRECTORY_ENTRY structures of the root resource       *
	* directory. It also includes: wstring(Resource name), IMAGE_RESOURCE_DATA_ENTRY,                   *
	* vector<byte> (RAW resource data), and PERESLVL2 that is a struct of the next, second, resource    *
	* level, that replicates struct of root resource level.	PERESLVL2 includes IMAGE_RESOURCE_DIRECTORY *
	* of the second resource level, and vector<PERESLVL2DATA> that includes PERESLVL3 that is a struct  *
	* of the last, third, level of resources. Like previous two, this last level's struct consist of    *
	* IMAGE_RESOURCE_DIRECTORY and vector<PERESLVL3DATA>, that is again a vector of structs of all      *
	* IMAGE_RESOURCE_DIRECTORY_ENTRY of the last, third, level of resources.                            *
	****************************************************************************************************/

	//Level 3/Lang (the lowest) resources.
	struct PERESLVL3DATA {
		IMAGE_RESOURCE_DIRECTORY_ENTRY stResDirEntry;  //Level 3 (Lang) standard IMAGE_RESOURCE_DIRECTORY_ENTRY struct.
		std::wstring                   wstrResName;    //Level 3 (Lang) resource name.
		IMAGE_RESOURCE_DATA_ENTRY      stResDataEntry; //Level 3 (Lang) standard IMAGE_RESOURCE_DATA_ENTRY struct.
		std::vector<std::byte>         vecRawResData;  //Level 3 (Lang) resource raw data.
	};
	using PERESLANGDATA = PERESLVL3DATA;

	struct PERESLVL3 {
		DWORD                      dwOffset;   //File's raw offset of this level 3 IMAGE_RESOURCE_DIRECTORY descriptor.
		IMAGE_RESOURCE_DIRECTORY   stResDir;   //Level 3 standard IMAGE_RESOURCE_DIRECTORY header.
		std::vector<PERESLVL3DATA> vecResData; //Array of level 3 resource entries.
	};
	using PERESLANG = PERESLVL3;

	//Level 2/Name resources — Includes Lang resourses.
	struct PERESLVL2DATA {
		IMAGE_RESOURCE_DIRECTORY_ENTRY stResDirEntry;  //Level 2 (Name) standard IMAGE_RESOURCE_DIRECTORY_ENTRY struct.
		std::wstring                   wstrResName;    //Level 2 (Name) resource name.
		IMAGE_RESOURCE_DATA_ENTRY      stResDataEntry; //Level 2 (Name) standard IMAGE_RESOURCE_DATA_ENTRY struct.
		std::vector<std::byte>         vecRawResData;  //Level 2 (Name) resource raw data.
		PERESLVL3                      stResLvL3;      //Level 3 (Lang) resource struct.
	};
	using PERESNAMEDATA = PERESLVL2DATA;

	struct PERESLVL2 {
		DWORD                      dwOffset;   //File's raw offset of this level 2 IMAGE_RESOURCE_DIRECTORY descriptor.
		IMAGE_RESOURCE_DIRECTORY   stResDir;   //Level 2 standard IMAGE_RESOURCE_DIRECTORY header.
		std::vector<PERESLVL2DATA> vecResData; //Array of level 2 resource entries.
	};
	using PERESNAME = PERESLVL2;

	//Level 1/Type resources — Includes Name Resources.
	struct PERESROOTDATA {
		IMAGE_RESOURCE_DIRECTORY_ENTRY stResDirEntry;  //Level root (Type) standard IMAGE_RESOURCE_DIRECTORY_ENTRY struct.
		std::wstring                   wstrResName;	   //Level root (Type) resource name.
		IMAGE_RESOURCE_DATA_ENTRY      stResDataEntry; //Level root (Type) standard IMAGE_RESOURCE_DATA_ENTRY struct.
		std::vector<std::byte>         vecRawResData;  //Level root (Type) resource raw data.
		PERESLVL2                      stResLvL2;      //Level 2 (Name) resource struct.
	};
	using PERESTYPEDATA = PERESROOTDATA;

	struct PERESROOT {
		DWORD                      dwOffset;   //File's raw offset of this level root IMAGE_RESOURCE_DIRECTORY descriptor.
		IMAGE_RESOURCE_DIRECTORY   stResDir;   //Level root standard IMAGE_RESOURCE_DIRECTORY header.
		std::vector<PERESROOTDATA> vecResData; //Array of level root resource entries.
	};
	using PERESTYPE = PERESROOT;

	//Flattened resources.
	struct PERESFLAT {
		std::span<const std::byte> spnData { };    //Resource data.
		std::wstring_view          wsvTypeStr { }; //Resource Type name.
		std::wstring_view          wsvNameStr { }; //Resource Name name (resource itself name).
		std::wstring_view          wsvLangStr { }; //Resource Lang name.
		WORD                       wTypeID { };    //Resource Type ID (RT_CURSOR, RT_BITMAP, etc...).
		WORD                       wNameID { };    //Resource Name ID (resource itself ID).
		WORD                       wLangID { };    //Resource Lang ID.
	};
	using PERESFLAT_VEC = std::vector<PERESFLAT>;
	inline const std::unordered_map<DWORD, std::wstring_view> MapResID {
		{ 1, L"RT_CURSOR" },
		{ 2, L"RT_BITMAP" },
		{ 3, L"RT_ICON" },
		{ 4, L"RT_MENU" },
		{ 5, L"RT_DIALOG" },
		{ 6, L"RT_STRING" },
		{ 7, L"RT_FONTDIR" },
		{ 8, L"RT_FONT" },
		{ 9, L"RT_ACCELERATOR" },
		{ 10, L"RT_RCDATA" },
		{ 11, L"RT_MESSAGETABLE" },
		{ 12, L"RT_GROUP_CURSOR" },
		{ 14, L"RT_GROUP_ICON" },
		{ 16, L"RT_VERSION" },
		{ 17, L"RT_DLGINCLUDE" },
		{ 19, L"RT_PLUGPLAY" },
		{ 20, L"RT_VXD" },
		{ 21, L"RT_ANICURSOR" },
		{ 22, L"RT_ANIICON" },
		{ 23, L"RT_HTML" },
		{ 24, L"RT_MANIFEST" },
		{ 28, L"RT_RIBBON_XML" },
		{ 240, L"RT_DLGINIT" },
		{ 241, L"RT_TOOLBAR" }
	};
	/*********************************Resources End*****************************************/

	//Exception table.
	struct PEEXCEPTION {
		DWORD                         dwOffset;           //File's raw offset of this exception's descriptor.
		_IMAGE_RUNTIME_FUNCTION_ENTRY stRuntimeFuncEntry; //Standard _IMAGE_RUNTIME_FUNCTION_ENTRY header.
	};
	using PEEXCEPTION_VEC = std::vector<PEEXCEPTION>;

	//Security table.
	struct PESECURITY {
		DWORD           dwOffset;  //File's raw offset of this security descriptor.
		WIN_CERTIFICATE stWinSert; //Standard WIN_CERTIFICATE header.
	};
	using PESECURITY_VEC = std::vector<PESECURITY>;
	inline const std::unordered_map<DWORD, std::wstring_view> MapWinCertRevision {
		{ WIN_CERT_REVISION_1_0, L"WIN_CERT_REVISION_1_0" },
		{ WIN_CERT_REVISION_2_0, L"WIN_CERT_REVISION_2_0" }
	};
	inline const std::unordered_map<DWORD, std::wstring_view> MapWinCertType {
		{ WIN_CERT_TYPE_X509, L"WIN_CERT_TYPE_X509" },
		{ WIN_CERT_TYPE_PKCS_SIGNED_DATA, L"WIN_CERT_TYPE_PKCS_SIGNED_DATA" },
		{ WIN_CERT_TYPE_RESERVED_1, L"WIN_CERT_TYPE_RESERVED_1" },
		{ WIN_CERT_TYPE_TS_STACK_SIGNED, L"WIN_CERT_TYPE_TS_STACK_SIGNED" },
	};

	//Relocation table.
	struct PERELOCDATA {
		DWORD dwOffset;     //File's raw offset of this Relocation data descriptor.
		WORD  wRelocType;   //Relocation type.
		WORD  wRelocOffset; //Relocation offset (Offset the relocation must be applied to.)
	};
	inline const std::unordered_map<DWORD, std::wstring_view> MapRelocType {
		{ IMAGE_REL_BASED_ABSOLUTE, L"IMAGE_REL_BASED_ABSOLUTE" },
		{ IMAGE_REL_BASED_HIGH, L"IMAGE_REL_BASED_HIGH" },
		{ IMAGE_REL_BASED_LOW, L"IMAGE_REL_BASED_LOW" },
		{ IMAGE_REL_BASED_HIGHLOW, L"IMAGE_REL_BASED_HIGHLOW" },
		{ IMAGE_REL_BASED_HIGHADJ, L"IMAGE_REL_BASED_HIGHADJ" },
		{ IMAGE_REL_BASED_MACHINE_SPECIFIC_5, L"IMAGE_REL_BASED_MACHINE_SPECIFIC_5" },
		{ IMAGE_REL_BASED_RESERVED, L"IMAGE_REL_BASED_RESERVED" },
		{ IMAGE_REL_BASED_MACHINE_SPECIFIC_7, L"IMAGE_REL_BASED_MACHINE_SPECIFIC_7" },
		{ IMAGE_REL_BASED_MACHINE_SPECIFIC_8, L"IMAGE_REL_BASED_MACHINE_SPECIFIC_8" },
		{ IMAGE_REL_BASED_MACHINE_SPECIFIC_9, L"IMAGE_REL_BASED_MACHINE_SPECIFIC_9" },
		{ IMAGE_REL_BASED_DIR64, L"IMAGE_REL_BASED_DIR64" }
	};
	struct PERELOC {
		DWORD                    dwOffset;     //File's raw offset of this Relocation descriptor.
		IMAGE_BASE_RELOCATION    stBaseReloc;  //Standard IMAGE_BASE_RELOCATION header.
		std::vector<PERELOCDATA> vecRelocData; //Array of the Relocation data struct.
	};
	using PERELOC_VEC = std::vector<PERELOC>;


	//Debug table.
	struct PEDEBUGDBGHDR {
		//dwHdr[6] is an array of the first six DWORDs of IMAGE_DEBUG_DIRECTORY::PointerToRawData data (Debug info header).
		//Their meaning varies depending on dwHdr[0] (Signature) value.
		//If dwHdr[0] == 0x53445352 (Ascii "RSDS") it's PDB 7.0 file:
		// Then dwHdr[1]-dwHdr[4] is GUID (*((GUID*)&dwHdr[1])). dwHdr[5] is Counter/Age.
		//If dwHdr[0] == 0x3031424E (Ascii "NB10") it's PDB 2.0 file:
		// Then dwHdr[1] is Offset. dwHdr[2] is Time/Signature. dwHdr[3] is Counter/Age.
		DWORD       dwHdr[6];
		std::string strPDBName; //PDB file name/path.
	};
	struct PEDEBUG {
		DWORD                 dwOffset;       //File's raw offset of this Debug descriptor.
		IMAGE_DEBUG_DIRECTORY stDebugDir;     //Standard IMAGE_DEBUG_DIRECTORY header.
		PEDEBUGDBGHDR         stDebugHdrInfo; //Debug info header.
	};
	using PEDEBUG_VEC = std::vector<PEDEBUG>;
	inline const std::unordered_map<DWORD, std::wstring_view> MapDbgType {
		{ IMAGE_DEBUG_TYPE_UNKNOWN, L"IMAGE_DEBUG_TYPE_UNKNOWN" },
		{ IMAGE_DEBUG_TYPE_COFF, L"IMAGE_DEBUG_TYPE_COFF" },
		{ IMAGE_DEBUG_TYPE_CODEVIEW, L"IMAGE_DEBUG_TYPE_CODEVIEW" },
		{ IMAGE_DEBUG_TYPE_FPO, L"IMAGE_DEBUG_TYPE_FPO" },
		{ IMAGE_DEBUG_TYPE_MISC, L"IMAGE_DEBUG_TYPE_MISC" },
		{ IMAGE_DEBUG_TYPE_EXCEPTION, L"IMAGE_DEBUG_TYPE_EXCEPTION" },
		{ IMAGE_DEBUG_TYPE_FIXUP, L"IMAGE_DEBUG_TYPE_FIXUP" },
		{ IMAGE_DEBUG_TYPE_OMAP_TO_SRC, L"IMAGE_DEBUG_TYPE_OMAP_TO_SRC" },
		{ IMAGE_DEBUG_TYPE_OMAP_FROM_SRC, L"IMAGE_DEBUG_TYPE_OMAP_FROM_SRC" },
		{ IMAGE_DEBUG_TYPE_BORLAND, L"IMAGE_DEBUG_TYPE_BORLAND" },
		{ IMAGE_DEBUG_TYPE_RESERVED10, L"IMAGE_DEBUG_TYPE_RESERVED10" },
		{ IMAGE_DEBUG_TYPE_CLSID, L"IMAGE_DEBUG_TYPE_CLSID" },
		{ IMAGE_DEBUG_TYPE_VC_FEATURE, L"IMAGE_DEBUG_TYPE_VC_FEATURE" },
		{ IMAGE_DEBUG_TYPE_POGO, L"IMAGE_DEBUG_TYPE_POGO" },
		{ IMAGE_DEBUG_TYPE_ILTCG, L"IMAGE_DEBUG_TYPE_ILTCG" },
		{ IMAGE_DEBUG_TYPE_MPX, L"IMAGE_DEBUG_TYPE_MPX" },
		{ IMAGE_DEBUG_TYPE_REPRO, L"IMAGE_DEBUG_TYPE_REPRO" }
	};

	//TLS table.
	struct PETLS {
		DWORD              dwOffset;          //File's raw offset of the TLS header descriptor.
		union UNPETLS {
			IMAGE_TLS_DIRECTORY32 stTLSDir32; //x86 standard TLS header.
			IMAGE_TLS_DIRECTORY64 stTLSDir64; //x64 TLS header.
		} unTLS;
		std::vector<DWORD> vecTLSCallbacks;   //Array of the TLS callbacks.
	};
	inline const std::unordered_map<DWORD, std::wstring_view> MapTLSCharact {
		{ IMAGE_SCN_ALIGN_1BYTES, L"IMAGE_SCN_ALIGN_1BYTES" },
		{ IMAGE_SCN_ALIGN_2BYTES, L"IMAGE_SCN_ALIGN_2BYTES" },
		{ IMAGE_SCN_ALIGN_4BYTES, L"IMAGE_SCN_ALIGN_4BYTES" },
		{ IMAGE_SCN_ALIGN_8BYTES, L"IMAGE_SCN_ALIGN_8BYTES" },
		{ IMAGE_SCN_ALIGN_16BYTES, L"IMAGE_SCN_ALIGN_16BYTES" },
		{ IMAGE_SCN_ALIGN_32BYTES, L"IMAGE_SCN_ALIGN_32BYTES" },
		{ IMAGE_SCN_ALIGN_64BYTES, L"IMAGE_SCN_ALIGN_64BYTES" },
		{ IMAGE_SCN_ALIGN_128BYTES, L"IMAGE_SCN_ALIGN_128BYTES" },
		{ IMAGE_SCN_ALIGN_256BYTES, L"IMAGE_SCN_ALIGN_256BYTES" },
		{ IMAGE_SCN_ALIGN_512BYTES, L"IMAGE_SCN_ALIGN_512BYTES" },
		{ IMAGE_SCN_ALIGN_1024BYTES, L"IMAGE_SCN_ALIGN_1024BYTES" },
		{ IMAGE_SCN_ALIGN_2048BYTES, L"IMAGE_SCN_ALIGN_2048BYTES" },
		{ IMAGE_SCN_ALIGN_4096BYTES, L"IMAGE_SCN_ALIGN_4096BYTES" },
		{ IMAGE_SCN_ALIGN_8192BYTES, L"IMAGE_SCN_ALIGN_8192BYTES" },
		{ IMAGE_SCN_ALIGN_MASK, L"IMAGE_SCN_ALIGN_MASK" }
	};

	//LoadConfigDirectory.
	struct PELOADCONFIG {
		DWORD dwOffset;                            //File's raw offset of the LCD descriptor.
		union UNPELOADCONFIG {
			IMAGE_LOAD_CONFIG_DIRECTORY32 stLCD32; //x86 LCD descriptor.
			IMAGE_LOAD_CONFIG_DIRECTORY64 stLCD64; //x64 LCD descriptor.
		} unLCD;
	};
	inline const std::unordered_map<DWORD, std::wstring_view> MapLCDGuardFlags {
		{ IMAGE_GUARD_CF_INSTRUMENTED, L"IMAGE_GUARD_CF_INSTRUMENTED (Module performs control flow integrity checks using system-supplied support)" },
		{ IMAGE_GUARD_CFW_INSTRUMENTED, L"IMAGE_GUARD_CFW_INSTRUMENTED (Module performs control flow and write integrity checks)" },
		{ IMAGE_GUARD_CF_FUNCTION_TABLE_PRESENT, L"IMAGE_GUARD_CF_FUNCTION_TABLE_PRESENT (Module contains valid control flow target metadata)" },
		{ IMAGE_GUARD_SECURITY_COOKIE_UNUSED, L"IMAGE_GUARD_SECURITY_COOKIE_UNUSED (Module does not make use of the /GS security cookie)" },
		{ IMAGE_GUARD_PROTECT_DELAYLOAD_IAT, L"IMAGE_GUARD_PROTECT_DELAYLOAD_IAT (Module supports read only delay load IAT)" },
		{ IMAGE_GUARD_DELAYLOAD_IAT_IN_ITS_OWN_SECTION, L"IMAGE_GUARD_DELAYLOAD_IAT_IN_ITS_OWN_SECTION (Delayload import table in its own .didat section (with nothing else in it) that can be freely reprotected)" },
		{ IMAGE_GUARD_CF_EXPORT_SUPPRESSION_INFO_PRESENT, L"IMAGE_GUARD_CF_EXPORT_SUPPRESSION_INFO_PRESENT (Module contains suppressed export information. This also infers that the address taken IAT table is also present in the load config.)" },
		{ IMAGE_GUARD_CF_ENABLE_EXPORT_SUPPRESSION, L"IMAGE_GUARD_CF_ENABLE_EXPORT_SUPPRESSION (Module enables suppression of exports)" },
		{ IMAGE_GUARD_CF_LONGJUMP_TABLE_PRESENT, L"IMAGE_GUARD_CF_LONGJUMP_TABLE_PRESENT (Module contains longjmp target information)" },
		{ IMAGE_GUARD_RF_INSTRUMENTED, L"IMAGE_GUARD_RF_INSTRUMENTED (Module contains return flow instrumentation and metadata)" },
		{ IMAGE_GUARD_RF_ENABLE, L"IMAGE_GUARD_RF_ENABLE (Module requests that the OS enable return flow protection)" },
		{ IMAGE_GUARD_RF_STRICT, L"IMAGE_GUARD_RF_STRICT (Module requests that the OS enable return flow protection in strict mode)" },
		{ IMAGE_GUARD_CF_FUNCTION_TABLE_SIZE_MASK, L"IMAGE_GUARD_CF_FUNCTION_TABLE_SIZE_MASK (Stride of Guard CF function table encoded in these bits (additional count of bytes per element))" },
		{ IMAGE_GUARD_CF_FUNCTION_TABLE_SIZE_SHIFT, L"IMAGE_GUARD_CF_FUNCTION_TABLE_SIZE_SHIFT (Shift to right-justify Guard CF function table stride)" }
	};

	//Bound import table.
	struct PEBOUNDFORWARDER {
		DWORD                     dwOffset;              //File's raw offset of this Bound Forwarder descriptor.
		IMAGE_BOUND_FORWARDER_REF stBoundForwarder;      //Standard IMAGE_BOUND_FORWARDER_REF struct.
		std::string               strBoundForwarderName; //Bound forwarder name.
	};
	struct PEBOUNDIMPORT {
		DWORD                         dwOffset;          //File's raw offset of this Bound Import descriptor.
		IMAGE_BOUND_IMPORT_DESCRIPTOR stBoundImpDesc;    //Standard IMAGE_BOUND_IMPORT_DESCRIPTOR struct.
		std::string                   strBoundName;      //Bound Import name.
		std::vector<PEBOUNDFORWARDER> vecBoundForwarder; //Array of the Bound Forwarder structs.
	};
	using PEBOUNDIMPORT_VEC = std::vector<PEBOUNDIMPORT>;

	//Delay import table.
	struct PEDELAYIMPORTFUNC {
		union UNPEDELAYIMPORTTHUNK
		{
			struct x32 {
				IMAGE_THUNK_DATA32 stImportAddressTable;      //x86 Import Address Table struct.
				IMAGE_THUNK_DATA32 stImportNameTable;         //x86 Import Name Table struct.
				IMAGE_THUNK_DATA32 stBoundImportAddressTable; //x86 Bound Import Address Table struct.
				IMAGE_THUNK_DATA32 stUnloadInformationTable;  //x86 Unload Information Table struct.
			} st32;
			struct x64 {
				IMAGE_THUNK_DATA64 stImportAddressTable;      //x64 Import Address Table struct.
				IMAGE_THUNK_DATA64 stImportNameTable;         //x64 Import Name Table struct.
				IMAGE_THUNK_DATA64 stBoundImportAddressTable; //x64 Bound Import Address Table struct
				IMAGE_THUNK_DATA64 stUnloadInformationTable;  //x64 Unload Information Table struct.
			} st64;
		} unThunk;
		IMAGE_IMPORT_BY_NAME stImpByName; //Standard IMAGE_IMPORT_BY_NAME struct.
		std::string          strFuncName; //Function name.
	};
	struct PEDELAYIMPORT {
		DWORD                          dwOffset;        //File's raw offset of this Delay Import descriptor.
		IMAGE_DELAYLOAD_DESCRIPTOR     stDelayImpDesc;  //Standard IMAGE_DELAYLOAD_DESCRIPTOR struct.
		std::string                    strModuleName;   //Import module name.
		std::vector<PEDELAYIMPORTFUNC> vecDelayImpFunc; //Array of the Delay Import module functions.
	};
	using PEDELAYIMPORT_VEC = std::vector<PEDELAYIMPORT>;

	//COM descriptor table.
	struct PECOMDESCRIPTOR {
		DWORD              dwOffset; //File's raw offset of the IMAGE_COR20_HEADER descriptor.
		IMAGE_COR20_HEADER stCorHdr; //Standard IMAGE_COR20_HEADER struct.
	};
	inline const std::unordered_map<DWORD, std::wstring_view> MapCOR20Flags {
		{ ReplacesCorHdrNumericDefines::COMIMAGE_FLAGS_ILONLY, L"COMIMAGE_FLAGS_ILONLY" },
		{ ReplacesCorHdrNumericDefines::COMIMAGE_FLAGS_32BITREQUIRED, L"COMIMAGE_FLAGS_32BITREQUIRED" },
		{ ReplacesCorHdrNumericDefines::COMIMAGE_FLAGS_IL_LIBRARY, L"COMIMAGE_FLAGS_IL_LIBRARY" },
		{ ReplacesCorHdrNumericDefines::COMIMAGE_FLAGS_STRONGNAMESIGNED, L"COMIMAGE_FLAGS_STRONGNAMESIGNED" },
		{ 16, L"COMIMAGE_FLAGS_NATIVE_ENTRYPOINT" },
		{ ReplacesCorHdrNumericDefines::COMIMAGE_FLAGS_TRACKDEBUGDATA, L"COMIMAGE_FLAGS_TRACKDEBUGDATA" },
		{ 131072, L"COMIMAGE_FLAGS_32BITPREFERRED" }
	};

	//File information struct.
	struct PEFILEINFO {
		bool fIsPE32 : 1 {};
		bool fIsPE64 : 1 {};
		bool fHasDosHdr : 1 {};
		bool fHasRichHdr : 1 {};
		bool fHasNTHdr : 1 {};
		bool fHasDataDirs : 1 {};
		bool fHasSections : 1 {};
		bool fHasExport : 1 {};
		bool fHasImport : 1 {};
		bool fHasResource : 1 {};
		bool fHasException : 1 {};
		bool fHasSecurity : 1 {};
		bool fHasReloc : 1 {};
		bool fHasDebug : 1 {};
		bool fHasArchitect : 1 {};
		bool fHasGlobalPtr : 1 {};
		bool fHasTLS : 1 {};
		bool fHasLoadCFG : 1 {};
		bool fHasBoundImp : 1 {};
		bool fHasIAT : 1 {};
		bool fHasDelayImp : 1 {};
		bool fHasCOMDescr : 1 {};
	};

	//Pure abstract base class Ilibpe.
	class Ilibpe
	{
	public:
		virtual auto ParsePE(LPCWSTR pwszFile) -> int = 0;                   //Parse PE from disk file.
		virtual auto ParsePE(std::span<const std::byte> spnFile) -> int = 0; //Parse PE file from memory bytes.
		[[deprecated("Deprecated, use ParsePE instead.")]] virtual auto LoadPe(LPCWSTR pwszFile) -> int = 0;
		[[deprecated("Deprecated, use ParsePE instead.")]] virtual auto LoadPe(std::span<const std::byte> spnFile) -> int = 0;
		[[nodiscard]] virtual auto GetFileInfo()const->PEFILEINFO = 0;
		[[nodiscard]] virtual auto GetOffsetFromRVA(ULONGLONG ullRVA)const->DWORD = 0;
		[[nodiscard]] virtual auto GetOffsetFromVA(ULONGLONG ullVA)const->DWORD = 0;
		[[nodiscard]] virtual auto GetMSDOSHeader() -> IMAGE_DOS_HEADER* = 0;
		[[nodiscard]] virtual auto GetRichHeader() -> PERICHHDR_VEC* = 0;
		[[nodiscard]] virtual auto GetNTHeader() -> PENTHDR* = 0;
		[[nodiscard]] virtual auto GetDataDirs() -> PEDATADIR_VEC* = 0;
		[[nodiscard]] virtual auto GetSecHeaders() -> PESECHDR_VEC* = 0;
		[[nodiscard]] virtual auto GetExport() -> PEEXPORT* = 0;
		[[nodiscard]] virtual auto GetImport() -> PEIMPORT_VEC* = 0;
		[[nodiscard]] virtual auto GetResources() -> PERESROOT* = 0;
		[[nodiscard]] virtual auto GetExceptions() -> PEEXCEPTION_VEC* = 0;
		[[nodiscard]] virtual auto GetSecurity() -> PESECURITY_VEC* = 0;
		[[nodiscard]] virtual auto GetRelocations() -> PERELOC_VEC* = 0;
		[[nodiscard]] virtual auto GetDebug() -> PEDEBUG_VEC* = 0;
		[[nodiscard]] virtual auto GetTLS() -> PETLS* = 0;
		[[nodiscard]] virtual auto GetLoadConfig() -> PELOADCONFIG* = 0;
		[[nodiscard]] virtual auto GetBoundImport() -> PEBOUNDIMPORT_VEC* = 0;
		[[nodiscard]] virtual auto GetDelayImport() -> PEDELAYIMPORT_VEC* = 0;
		[[nodiscard]] virtual auto GetCOMDescriptor() -> PECOMDESCRIPTOR* = 0;
		virtual void Clear() = 0; //Clear all internal structs.
		virtual void Destroy() = 0;
		[[nodiscard]] static auto FlatResources(const PERESROOT& stResRoot) -> PERESFLAT_VEC;
	};

	//Return codes.
	constexpr auto PEOK = 0;
	constexpr auto ERR_FILE_OPEN = 0x01;
	constexpr auto ERR_FILE_SIZESMALL = 0x02;
	constexpr auto ERR_FILE_MAPPING = 0x03;
	constexpr auto ERR_FILE_NODOSHDR = 0x04;

#ifdef LIBPE_SHARED_DLL
#ifdef LIBPE_SHARED_DLL_EXPORT
#define ILIBPEAPI __declspec(dllexport)
#else  //LIBPE_SHARED_DLL_EXPORT^^
#define ILIBPEAPI __declspec(dllimport)
#ifdef _WIN64
#ifdef _DEBUG
#define LIBPE_LIB_NAME(x) x"64d.lib"
#else  //_DEBUG
#define LIBPE_LIB_NAME(x) x"64.lib"
#endif //_DEBUG
#else  //_WIN64
#ifdef _DEBUG
#define LIBPE_LIB_NAME(x) x"d.lib"
#else  //_DEBUG
#define LIBPE_LIB_NAME(x) x".lib"
#endif //_DEBUG
#endif //_WIN64
#pragma comment(lib, LIBPE_LIB_NAME("libpe"))
#endif //LIBPE_SHARED_DLL_EXPORT
#else  //LIBPE_SHARED_DLL^^
#define	ILIBPEAPI
#endif //LIBPE_SHARED_DLL

	/********************************************************************************************
	* Factory function Createlibpe, returns IlibpePtr - unique_ptr with custom deleter.         *
	* If you, for some reason, need a raw pointer, you can directly call CreateRawlibpe         *
	* function, which returns Ilibpe interface pointer, but in this case you will need to       *
	* call Ilibpe::Destroy method afterwards manually - to delete Ilibpe object.                *
	********************************************************************************************/
	extern "C" ILIBPEAPI Ilibpe * __cdecl CreateRawlibpe();
	using IlibpePtr = std::unique_ptr < Ilibpe, decltype([](Ilibpe* p) { p->Destroy(); }) > ;

	inline IlibpePtr Createlibpe() {
		return IlibpePtr { CreateRawlibpe() };
	};

	/********************************************
	* LIBPEINFO: service info structure.        *
	********************************************/
	struct LIBPEINFO {
		const wchar_t* pwszVersion { };        //wchar_t string Version.
		union {
			unsigned long long ullVersion { }; //long long number Version.
			struct {
				short wMajor;
				short wMinor;
				short wMaintenance;
				short wRevision;
			} stVersion;
		};
	};

	extern "C" ILIBPEAPI LIBPEINFO __cdecl GetLibInfo();
}
