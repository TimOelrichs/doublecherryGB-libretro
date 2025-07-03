cmake_minimum_required(VERSION 3.15)
project(DoubleCherryGB LANGUAGES CXX)

# Basiseinstellungen
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Plattformauswahl
option(MIYOO_BUILD "Build for Miyoo Mini" OFF)
option(TRIMUI_BUILD "Build for Trimui Smart Pro" OFF)
option(BRICK_BUILD "Build for Trimui Brick" OFF)

# Toolchain-Konfiguration
if(MIYOO_BUILD)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchains/miyoo.cmake)
    add_definitions(-DMIYOO_MINI)
elseif(TRIMUI_BUILD)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchains/trimui.cmake)
    add_definitions(-DTRIMUI_SMART_PRO)
elseif(BRICK_BUILD)
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchains/brick.cmake)
    add_definitions(-DTRIMUI_BRICK)
endif()

# Quell-Dateien (organisiert nach Modulen)
set(CORE_DIR ${CMAKE_SOURCE_DIR})
set(SOURCES
    ${CORE_DIR}/gb_core/apu.cpp
    ${CORE_DIR}/gb_core/cheat.cpp
    ${CORE_DIR}/gb_core/cpu.cpp
    ${CORE_DIR}/gb_core/gb.cpp
    ${CORE_DIR}/gb_core/lcd.cpp
    ${CORE_DIR}/gb_core/mbc.cpp
    ${CORE_DIR}/gb_core/rom.cpp
    
    ${CORE_DIR}/libretro/dmy_renderer.cpp
    ${CORE_DIR}/libretro/libretro.cpp
    
    ${CORE_DIR}/gb_core/GbPalettes/GBPaletteManager.cpp
    
    ${CORE_DIR}/gb_core/linkcable/alleyway_link_controller.cpp
    ${CORE_DIR}/gb_core/linkcable/link_master_device.cpp
    ${CORE_DIR}/gb_core/linkcable/linkcable_hub.cpp
    ${CORE_DIR}/gb_core/linkcable/dmg07.cpp
    ${CORE_DIR}/gb_core/linkcable/dmg07x4.cpp
    ${CORE_DIR}/gb_core/linkcable/hack_4p_tetris.cpp
    ${CORE_DIR}/gb_core/linkcable/hack_4p_kwirk.cpp
    ${CORE_DIR}/gb_core/linkcable/hack_4p_burger_time_deluxe.cpp
    ${CORE_DIR}/gb_core/linkcable/barcodeboy.cpp
    ${CORE_DIR}/gb_core/linkcable/barcode_taisen_bardigun.cpp
    ${CORE_DIR}/gb_core/linkcable/faceball2000_cable.cpp
    ${CORE_DIR}/gb_core/linkcable/gameboy_printer.cpp
    ${CORE_DIR}/gb_core/linkcable/game_and_watch_gallery_unlocker.cpp
    ${CORE_DIR}/gb_core/linkcable/power_antenna.cpp
    ${CORE_DIR}/gb_core/linkcable/pokebuddy_gen1.cpp
    
    ${CORE_DIR}/gb_core/infrared/pikachu_2_gs.cpp
    ${CORE_DIR}/gb_core/infrared/ubikey_unlocker.cpp
    ${CORE_DIR}/gb_core/infrared/tv_remote.cpp
    ${CORE_DIR}/gb_core/infrared/full_changer.cpp
    
    ${CORE_DIR}/services/printer/bmp_printer.cpp
    ${CORE_DIR}/services/printer/png_printer.cpp
    ${CORE_DIR}/services/printer/image_scaler.cpp
    ${CORE_DIR}/services/printer/printer_registry.cpp
)

# Bibliothek erstellen
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Plattformspezifische Compiler-Flags
if(MIYOO_BUILD)
    target_compile_options(${PROJECT_NAME} PRIVATE
        -mcpu=cortex-a7
        -mfpu=neon-vfpv4
        -mfloat-abi=hard
        -marm
        -Os
    )
elseif(TRIMUI_BUILD)
    target_compile_options(${PROJECT_NAME} PRIVATE
        -mcpu=cortex-a35
        -march=armv8-a
        -mtune=cortex-a35
        -O2
    )
elseif(BRICK_BUILD)
    target_compile_options(${PROJECT_NAME} PRIVATE
        -march=mips32
        -mtune=x1000
        -msoft-float
        -Os
    )
elseif(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4
        /WX-
        /wd4244
        /wd4018
    )
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Wno-missing-field-initializers
        -Wno-unused-parameter
    )
endif()

# Include-Verzeichnisse
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CORE_DIR}
    ${CORE_DIR}/gb_core
    ${CORE_DIR}/libretro
    ${CORE_DIR}/libretro-common/include
)

# Libretro-spezifische Einstellungen
target_compile_definitions(${PROJECT_NAME} PRIVATE __LIBRETRO__)

# Ausgabeformat
set_target_properties(${PROJECT_NAME} PROPERTIES
    PREFIX ""
    OUTPUT_NAME "${PROJECT_NAME}"
    SUFFIX "_libretro${CMAKE_SHARED_LIBRARY_SUFFIX}"
)

# Installation
install(TARGETS ${PROJECT_NAME} DESTINATION cores)

# Automatische Versionsnummer aus Git
execute_process(
  COMMAND git rev-parse --short HEAD
  OUTPUT_VARIABLE GIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
add_definitions(-DBUILD_VERSION="${GIT_HASH}")