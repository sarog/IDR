cmake_minimum_required(VERSION 3.21)

set(BCC32C TRUE)
set(_EMBT_CPP_PREPROCESSOR "cpp32c")
set(CLANG_BASED TRUE)

set(EMBT_TARGET Windows)

set(ROOTDIR "$ENV{USERPROFILE}\\Programs\\BCC1210")

#[[if(CMAKE_BASE_NAME STREQUAL "bcc32x")
    set(BCC32X TRUE)
    set(_EMBT_CPP_PREPROCESSOR "cpp32x")
    set(CLANG_BASED TRUE)
    set(CLANG_BASED_OPTS TRUE)
elseif(CMAKE_BASE_NAME STREQUAL "bcc64")
    set(BCC64 TRUE)
    set(_EMBT_CPP_PREPROCESSOR "cpp64")
    set(CLANG_BASED TRUE)
    set(CLANG_BASED_OPTS TRUE)
elseif(CMAKE_BASE_NAME STREQUAL "bcc32c")
    set(BCC32C TRUE)
    set(_EMBT_CPP_PREPROCESSOR "cpp32c")
    set(CLANG_BASED TRUE)
elseif(CMAKE_BASE_NAME STREQUAL "bcc32")
    set(BCC32 TRUE)
    set(_EMBT_CPP_PREPROCESSOR "cpp32")
    set(CLANG_BASED FALSE)
else()
    message(FATAL_ERROR "Unknown compiler specified ${CMAKE_BASE_NAME}")
endif()]]

# This module is shared by multiple languages; use include blocker.
if(__WINDOWS_EMBARCADERO)
    return()
endif()

set(__WINDOWS_EMBARCADERO 1)
set(BORLAND 1)

set(__pch_header_C "c-header")
set(__pch_header_CXX "c++-header")
set(__pch_header_OBJC "objective-c-header")
set(__pch_header_OBJCXX "objective-c++-header")

include(CMakePrintHelpers)

#[[
if("${CMAKE_${_lang}_COMPILER_VERSION}" VERSION_LESS 6.30)
    # Borland target type flags (bcc32 -h -t):
    set(_tW "-tW")       # -tW  GUI App         (implies -U__CONSOLE__)
    set(_tC "-tWC")      # -tWC Console App     (implies -D__CONSOLE__=1)
    set(_tD "-tWD")      # -tWD Build a DLL     (implies -D__DLL__=1 -D_DLL=1)
    set(_tM "-tWM")      # -tWM Enable threads  (implies -D__MT__=1 -D_MT=1)
    set(_tR "-tWR -tW-") # -tWR Use DLL runtime (implies -D_RTLDLL, and '-tW' too!!)
    # Notes:
    #  - The flags affect linking so we pass them to the linker.
    #  - The flags affect preprocessing so we pass them to the compiler.
    #  - Since '-tWR' implies '-tW' we use '-tWR -tW-' instead.
    #  - Since '-tW-' disables '-tWD' we use '-tWR -tW- -tWD' for DLLs.
else()]]
    set(EMBARCADERO 1)
    set(_tC "-tC") # Target is a console application
    set(_tD "-tD") # Target is a shared library
    set(_tM "-tM") # Target is multi-threaded
    set(_tR "-tR") # Target uses the dynamic RTL
    set(_tW "-tW") # Target is a Windows application
    set(_tV "-tV") # Target is a VCL application
    set(_tJ "-tJ") # Target uses the Delphi Runtime
    set(_tF "-tF") # Target is a FMX application
    set(_tP "-tP") # Target creates a Package
    set(_tU "-tU") # Target creates a Unicode
#endif()

set(CMAKE_BUILD_TYPE_INIT Debug)

## Set include paths
if(CLANG_BASED)
    set(CMAKE_INCLUDE_SYSTEM_FLAG_C "-isystem ")
    set(CMAKE_INCLUDE_SYSTEM_FLAG_CXX "-isystem ")
else()
    set(CMAKE_INCLUDE_SYSTEM_FLAG_C "-I")
    set(CMAKE_INCLUDE_SYSTEM_FLAG_CXX "-I")
endif()

if(CLANG_BASED_OPTS)
    if(NOT CMAKE_GENERATOR MATCHES "Borland Makefiles")
        set(CMAKE_DEPFILE_FLAGS_C "-MD -MT <OBJECT> -MF <DEP_FILE>")
        set(CMAKE_DEPFILE_FLAGS_CXX ${CMAKE_DEPFILE_FLAGS_C})
    endif()
else()
    if(NOT CMAKE_GENERATOR MATCHES "Borland Makefiles")
        set(CMAKE_DEPFILE_FLAGS_C "-md -mo<DEP_FILE>")
        set(CMAKE_DEPFILE_FLAGS_CXX ${CMAKE_DEPFILE_FLAGS_C})
    endif()
endif()

include_directories(SYSTEM "${ROOTDIR}/include/windows/crtl")
include_directories(SYSTEM "${ROOTDIR}/include/windows/sdk")
include_directories(SYSTEM "${ROOTDIR}/include/windows/rtl")

if(BCC32)
    include_directories(SYSTEM "${ROOTDIR}/include/dinkumware")
else()
    include_directories(SYSTEM "${ROOTDIR}/include/dinkumware64")
endif()

## Set library paths
if(BCC64)
    link_directories("${ROOTDIR}/lib/win64/$<IF:$<CONFIG:Debug>,debug,release>")
    link_directories("${ROOTDIR}/lib/win64/release")
    link_directories("${ROOTDIR}/lib/win64/release/psdk")
elseif(BCC32X OR BCC32C)
    link_directories("${ROOTDIR}/lib/win32c/$<IF:$<CONFIG:Debug>,debug,release>")
    link_directories("${ROOTDIR}/lib/win32c/release")
    link_directories("${ROOTDIR}/lib/win32/$<IF:$<CONFIG:Debug>,debug,release>")
    link_directories("${ROOTDIR}/lib/win32/release")
    link_directories("${ROOTDIR}/lib/win32/release/psdk")
elseif(BCC32)
    link_directories("${ROOTDIR}/lib/win32/$<IF:$<CONFIG:Debug>,debug,release>")
    link_directories("${ROOTDIR}/lib/win32/release")
    link_directories("${ROOTDIR}/lib/win32/release/psdk")
else()
    message(FATAL_ERROR "Unknown compiler")
endif()


if(BCC32X OR BCC32C OR BCC32)
    if(CMAKE_BUILD_TYPE MATCHES ".*DEB.*")
        set(linker_Path1 "${ROOTDIR}/lib/win32c/debug")
        set(linker_Path2 "${ROOTDIR}/lib/win32/debug")
        set(linker_Path3 "${ROOTDIR}/lib/win32/debug/psdk")
        set(windows_LIBRARY "${windows_LIBRARY} -L \"${linker_Path1}\"")
        set(windows_LIBRARY "${windows_LIBRARY} -L \"${linker_Path2}\"")
        set(windows_LIBRARY "${windows_LIBRARY} -L \"${linker_Path3}\"")
    endif()
    set(linker_Path1 "${ROOTDIR}/lib/win32c/release")
    set(linker_Path2 "${ROOTDIR}/lib/win32/release")
    set(linker_Path3 "${ROOTDIR}/lib/win32/release/psdk")
    set(windows_LIBRARY "${windows_LIBRARY} -L \"${linker_Path1}\"")
    set(windows_LIBRARY "${windows_LIBRARY} -L \"${linker_Path2}\"")
    set(windows_LIBRARY "${windows_LIBRARY} -L \"${linker_Path3}\"")
endif()

if(CLANG_BASED)
    if(BCC32X)
        set(CMAKE_C_COMPILER ${ROOTDIR}/bin/bcc32x.exe)
        set(CMAKE_CXX_COMPILER ${ROOTDIR}/bin/bcc32x.exe)
    endif()
    if(BCC64)
        if(CMAKE_BUILD_TYPE MATCHES ".*DEB.*")
            set(linker_Path1 "${ROOTDIR}/lib/win64/debug")
            set(linker_Path2 "${ROOTDIR}/lib/win64/debug/psdk")
            set(windows_LIBRARY "${windows_LIBRARY} -L \"${linker_Path1}\"")
            set(windows_LIBRARY "${windows_LIBRARY} -L \"${linker_Path2}\"")
        endif()
        set(linker_Path1 "${ROOTDIR}/lib/win64/release")
        set(linker_Path2 "${ROOTDIR}/lib/win64/release/psdk")
        set(CMAKE_C_COMPILER ${ROOTDIR}/bin/bcc64.exe)
        set(CMAKE_CXX_COMPILER ${ROOTDIR}/bin/bcc64.exe)
        set(windows_LIBRARY "${windows_LIBRARY} -L \"${linker_Path1}\"")
        set(windows_LIBRARY "${windows_LIBRARY} -L \"${linker_Path2}\"")
    endif()

    # Setting the Link Library Path in flag
    set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS ${windows_LIBRARY})
    set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS ${windows_LIBRARY})
    # setting the link library flag
    set(link_flags ${windows_LIBRARY})
    set(CMAKE_EXE_LINKER_FLAGS ${link_flags} CACHE INTERNAL "exe link flags")
    set(CMAKE_MODULE_LINKER_FLAGS ${link_flags} CACHE INTERNAL "module link flags")
    set(CMAKE_SHARED_LINKER_FLAGS ${link_flags} CACHE INTERNAL "shared lnk flags")
endif()

if(BCC64)
    set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
    set(CMAKE_IMPORT_LIBRARY_SUFFIX ".a")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    set(CMAKE_LINK_LIBRARY_SUFFIX ".a")

    ## Doesn't seem to always work here. Also set in compiler configuration modules.
    set(CMAKE_C_OUTPUT_EXTENSION ".o")
    set(CMAKE_CXX_OUTPUT_EXTENSION ".o")
else()
    set(CMAKE_FIND_LIBRARY_SUFFIXES "-bcc.lib" ".lib")
endif()

set(_COMPILE_C "-c")
set(_COMPILE_CXX "-P -c")
set(CMAKE_LIBRARY_PATH_FLAG "-L")

if(CLANG_BASED)
    if(CLANG_BASED_OPTS)
        set(FIX_QUOTING "--rsp-quoting=windows ")
    else()
        set(FIX_QUOTING "-Xdriver --rsp-quoting=windows ")
    endif()
endif()
set(CMAKE_LINK_LIBRARY_FLAG "${FIX_QUOTING}")

# uncomment these out to debug makefiles
#set(CMAKE_START_TEMP_FILE "")
#set(CMAKE_END_TEMP_FILE "")
#set(CMAKE_VERBOSE_MAKEFILE 1)

# Linker cannot handle + in the file name, so mangle object file name
set (CMAKE_MANGLE_OBJECT_FILE_NAMES "ON")

# extra flags for a win32 exe
set(CMAKE_CREATE_WIN32_EXE "${_tW}" )
# extra flags for a console app
set(CMAKE_CREATE_CONSOLE_EXE "${_tC}" )

foreach(t EXE SHARED MODULE)
    string(APPEND CMAKE_${t}_LINKER_FLAGS_INIT " ${_tM} ${_tR}")
    string(APPEND CMAKE_${t}_LINKER_FLAGS_DEBUG_INIT " -v")
    string(APPEND CMAKE_${t}_LINKER_FLAGS_RELWITHDEBINFO_INIT " -v")
endforeach()

# Resource files
if(NOT CMAKE_RC_COMPILER_INIT)
  #   bin\brcc32.exe -d_DEBUG -i"\include\windows\vcl" IdcDialog.rc -fo.\Win32\Debug\Obj\IdcDialog.res
  set(CMAKE_RC_COMPILER_INIT "brcc32.exe")
  set(CMAKE_RC_FLAGS "-i\"${ROOTDIR}/include/windows/vcl/\"")
  set(CMAKE_RC_COMPILE_OBJECT
          "<CMAKE_RC_COMPILER> <FLAGS> <SOURCE> -fo <OBJECT>")
endif()
enable_language(RC)

# Older tools do not support multiple concurrent invocations within a
# single working directory.
if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 7.50 OR BCC32)
    if(NOT DEFINED CMAKE_JOB_POOL_LINK)
        set(CMAKE_JOB_POOL_LINK BCC32LinkPool)
        get_property(_bccjp GLOBAL PROPERTY JOB_POOLS)
        if(NOT _bccjp MATCHES "BCC32LinkPool=")
            set_property(GLOBAL APPEND PROPERTY JOB_POOLS BCC32LinkPool=1)
        endif()
        unset(_bccjp)
    endif()
endif()

macro(__embarcadero_language lang)
    set(CMAKE_${lang}_COMPILE_OPTIONS_DLL "${_tD}" ) # Note: This variable is a ';' separated list
    set(CMAKE_SHARED_LIBRARY_${lang}_FLAGS "${_tD}") # ... while this is a space separated string.
    set(CMAKE_${lang}_USE_RESPONSE_FILE_FOR_INCLUDES 1)

    # compile a source file into an object file
    # place <DEFINES> outside the response file because Borland refuses
    # to parse quotes from the response file.
    set(CMAKE_${lang}_COMPILE_OBJECT
            "<CMAKE_${lang}_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -o<OBJECT> ${_COMPILE_${lang}} <SOURCE>"
    )

    set(CMAKE_${lang}_LINK_EXECUTABLE
            "<CMAKE_${lang}_COMPILER> ${FIX_QUOTING} -o<TARGET> <LINK_FLAGS> <FLAGS> ${CMAKE_START_TEMP_FILE} <LINK_LIBRARIES> <OBJECTS>${CMAKE_END_TEMP_FILE}"
    )

    # Used for Makefile based builds
    set(CMAKE_${lang}_CREATE_PREPROCESSED_SOURCE
            "${_EMBT_CPP_PREPROCESSOR} <DEFINES> <INCLUDES> <FLAGS> -o<PREPROCESSED_SOURCE> ${_COMPILE_${lang}} <SOURCE>"
    )

    # Create a module library.
    set(CMAKE_${lang}_CREATE_SHARED_MODULE
            "<CMAKE_${lang}_COMPILER> ${FIX_QUOTING} ${_tD} ${CMAKE_START_TEMP_FILE}-o<TARGET> <LINK_FLAGS> <LINK_LIBRARIES> <OBJECTS>${CMAKE_END_TEMP_FILE}"
    )

    # Create an import library for another target.
    if(BCC64)
        set(CMAKE_${lang}_CREATE_IMPORT_LIBRARY
                "mkexp <TARGET_IMPLIB> <TARGET>"
        )
    else()
        set(CMAKE_${lang}_CREATE_IMPORT_LIBRARY
                "implib -c -w <TARGET_IMPLIB> <TARGET>"
        )
    endif()

    # Create a shared library.
    # First create a module and then its import library.
    set(CMAKE_${lang}_CREATE_SHARED_LIBRARY
            ${CMAKE_${lang}_CREATE_SHARED_MODULE}
            ${CMAKE_${lang}_CREATE_IMPORT_LIBRARY}
    )

    # create a static library
    if(BCC64)
        set(CMAKE_${lang}_CREATE_STATIC_LIBRARY
                "tlib64 ${CMAKE_START_TEMP_FILE}/p2048 /N /B <LINK_FLAGS> /u <TARGET_QUOTED> <OBJECTS>${CMAKE_END_TEMP_FILE}"
        )
    else()
        set(CMAKE_${lang}_CREATE_STATIC_LIBRARY
                "tlib ${CMAKE_START_TEMP_FILE}/p512 /N /B <LINK_FLAGS> /u <TARGET_QUOTED> <OBJECTS>${CMAKE_END_TEMP_FILE}"
        )
    endif()

    # Initial configuration flags.
    string(APPEND CMAKE_${lang}_FLAGS_INIT " ${_tM} ${_tR}")

    if(CLANG_BASED)
        set(CMAKE_INCLUDE_SYSTEM_FLAG_${lang} "-isystem ")
    endif()

    if(CLANG_BASED_OPTS)
        string(APPEND CMAKE_${lang}_FLAGS_DEBUG_INIT " -O0 -g")
        string(APPEND CMAKE_${lang}_FLAGS_MINSIZEREL_INIT " -O1 -DNDEBUG")
        string(APPEND CMAKE_${lang}_FLAGS_RELEASE_INIT " -O2 -DNDEBUG")
        string(APPEND CMAKE_${lang}_FLAGS_RELWITHDEBINFO_INIT " -O3")
    else()
        string(APPEND CMAKE_${lang}_FLAGS_DEBUG_INIT " -Od -v")
        string(APPEND CMAKE_${lang}_FLAGS_MINSIZEREL_INIT " -O1 -DNDEBUG")
        string(APPEND CMAKE_${lang}_FLAGS_RELEASE_INIT " -O2 -DNDEBUG")
        string(APPEND CMAKE_${lang}_FLAGS_RELWITHDEBINFO_INIT " -Od")
        set(CMAKE_${lang}_STANDARD_LIBRARIES_INIT "import32.lib")
    endif()
endmacro()

macro(set_embt_target)
    foreach(arg IN ITEMS ${ARGN})
        if(${arg} STREQUAL "FMX")
            set(CMAKE_${_lang}_FLAGS "${CMAKE_${_lang}_FLAGS} ${_tF}")
            include_directories(SYSTEM "${ROOTDIR}/include/windows/fmx")
        elseif(${arg} STREQUAL "VCL")
            set(CMAKE_${_lang}_FLAGS "${CMAKE_${_lang}_FLAGS} ${_tV}")
            include_directories(SYSTEM "${ROOTDIR}/include/windows/vcl")
        elseif(${arg} STREQUAL "DR")
            set(CMAKE_${_lang}_FLAGS "${CMAKE_${_lang}_FLAGS} ${_tJ}")
        elseif(${arg} STREQUAL "DynamicRuntime")
            set(CMAKE_${_lang}_FLAGS "${CMAKE_${_lang}_FLAGS} ${_tR}")
        elseif(${arg} STREQUAL "Package")
            set(CMAKE_${_lang}_FLAGS "${CMAKE_${_lang}_FLAGS} ${_tP}")
        elseif(${arg} STREQUAL "Unicode")
            set(CMAKE_${_lang}_FLAGS "${CMAKE_${_lang}_FLAGS} ${_tU}")
        else()
            message("Error in set_embt_target: unknown target specified \"${arg}\"")
        endif()
    endforeach()
endmacro()