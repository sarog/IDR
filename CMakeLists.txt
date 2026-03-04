# https://docwiki.embarcadero.com/RADStudio/Athens/en/Using_CMake_with_C%2B%2B_Builder
# https://docwiki.embarcadero.com/RADStudio/Athens/en/CMake_Macros_and_Variables

# Target Win32: cmake -DCMAKE_C_COMPILER=bcc32x.exe -DCMAKE_CXX_COMPILER=bcc32x.exe -G Ninja <directory> [1]
# Target Win64: cmake -DCMAKE_C_COMPILER=bcc64.exe -DCMAKE_CXX_COMPILER=bcc64.exe -G Ninja <directory> [

cmake_minimum_required(VERSION 3.21)

set(APP "IDR")

set(CMAKE_BUILD_TYPE "DEBUG")
set(CMAKE_CXX_COMPILER bcc32)
# set(CMAKE_MAKE_PROGRAM Ninja)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_DEPFILE_FLAGS_CXX "-md -mo\"<DEPFILE>\"")


# https://docwiki.embarcadero.com/RADStudio/Tokyo/en/BCC32.EXE,_the_C%2B%2B_32-bit_Command-Line_Compiler
# https://docwiki.embarcadero.com/RADStudio/Sydney/en/Message_Options
# -w-csu -w-aus -w-mcc -w-
# -DUNICODE -D_UNICODE
add_compile_options(-y -Q -k -r- -c -w-par -vi- -C8 -Od -D_DEBUG -DUSEPACKAGES -H -n.\\Win32\\Debug\\Obj -H=.\\IdrPCH1.h)
# add_compile_options(-y -Q -k -r- -c -w-par -w-mcc -w-csu -vi- -D_DEBUG -DUSEPACKAGES)

project(${APP} CXX)

# -tR -eidr.exe -tM -lS:1048576 -lSc:4098 -lH:1048576 -lHc:8192 -v -tC -lGl -lGpr -tM -Od -v @MAKE0000.@@@
file(GLOB SOURCES
        Idr.cpp
        "*.h"
        "*.cpp"
        Idr.rc
)

## .DFM
file(GLOB SOURCES_DFM
        "*.dfm"
)

__embarcadero_language(CXX)
set_embt_target(VCL DynamicRuntime) # Unicode

## Resource compiler:
# $(BDSINCLUDE)\windows\vcl;Sources\Libs\;Sources\Forms\;Sources\;$(BDSINCLUDE)\windows\vcl;Sources\Libs\;Sources\Forms\;Sources\
# set(SYSTEM SOURCES ${SYSTEM SOURCES} idr.vrc)

# Syntax: ILINK32 objfiles, exefile, mapfile, libfiles, deffile, resfiles
# -lGl  = Static Package
# -lGpr = Runtime-only package
# -lGF LARGEADDRESSAWARE
# https://docwiki.embarcadero.com/RADStudio/Tokyo/en/Using_ILINK32_and_ILINK64_on_the_Command_Line
add_link_options(-tV -tW -lr -lGl -lGpr -laa -lTpe) #-lL.\\Win32\\Debug\\Obj

add_executable(${APP} WIN32 ${SOURCES}) # Idr.rc

# bin\cgrc.exe -c65001 .\Win32\Release\Obj\Idr.vrc -fo.\Win32\Release\Obj\Idr.res
target_precompile_headers(${APP} PUBLIC IdrPCH1.h)
#target_include_directories(${APP} PRIVATE Sources/Libs Sources/Forms Sources)
target_link_directories(${APP} PRIVATE
        "${CMAKE_SOURCE_DIR}"
)

set(EMBT_BPI_DIR "${ROOTDIR}/lib/win32/release")

target_link_libraries(${APP} PRIVATE
        "${EMBT_BPI_DIR}/vcl.bpi"
        "${EMBT_BPI_DIR}/rtl.bpi"
        "${EMBT_BPI_DIR}/bcbsmp.bpi"
        "${EMBT_BPI_DIR}/vclx.bpi"
        "${EMBT_BPI_DIR}/vclimg.bpi"
        "Idr.res"
)

add_custom_command(
        TARGET ${APP}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/dis.dll"
        "$<TARGET_FILE_DIR:${APP}>/dis.dll"
        VERBATIM
)

add_custom_command(
        TARGET ${APP}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Icons.dll"
        "$<TARGET_FILE_DIR:${APP}>/Icons.dll"
        VERBATIM
)

# Optional: keep .dfm files attached to the target for IDE visibility only
set_source_files_properties(${SOURCES_DFM} PROPERTIES HEADER_FILE_ONLY TRUE)
target_sources(${APP} PRIVATE ${SOURCES_DFM})

## Generate a manifest:
#add_custom_command(
#        TARGET ${PROJECT_NAME}
#        POST_BUILD
#        COMMAND "mt.exe" -manifest \"${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.manifest\" -outputresource:\"$<TARGET_FILE:${PROJECT_NAME}>\")

install(TARGETS ${APP}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)
