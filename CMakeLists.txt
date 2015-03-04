########################################
# General setup
#
cmake_minimum_required (VERSION 2.8)

# Update compiler before calling project()
if (APPLE)
	# Use clang compiler
	set(CMAKE_C_COMPILER "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang")
	set(CMAKE_CXX_COMPILER "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++")
	if (NOT EXISTS "${CMAKE_CXX_COMPILER}")
		set(CMAKE_C_COMPILER "clang")
		set(CMAKE_CXX_COMPILER "clang++")
	endif()
endif()

PROJECT (openatomic)
set(OPENATOMIC_IS_STABLE FALSE)

# Set up paths
if((${CMAKE_SYSTEM_NAME} MATCHES "Darwin"))
	# The gettext module will install the translations unconditionally.
	# Redirect the installation to a build directory where it does no harm.
	set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install-dummy)
else()
	set(bindir  ${CMAKE_INSTALL_PREFIX}/bin              CACHE PATH "bindir")
	set(datadir ${CMAKE_INSTALL_PREFIX}/share/openatomic CACHE PATH "datadir")
	add_definitions(-DDATA_DIR="${datadir}/")
endif()
set(userdir ".OpenAtomic" CACHE STRING "User directory")
add_definitions(-DUSER_DIR="${userdir}")

# Set where the binary files will be built.  The program will not execute from
# here.  You must run "make install" to install these to the proper location
# as defined above.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# REQUIRED does not work in CMake <=2.4.6 for SDL
find_package( SDL REQUIRED )
find_package( SDL_image REQUIRED )
find_package( SDL_mixer REQUIRED )
find_package( SDL_net REQUIRED )
find_package( X11 REQUIRED )
find_package( Threads REQUIRED )
#find_package( Guichan REQUIRED )
find_package( Boost COMPONENTS system regex thread filesystem REQUIRED )

# Workaround for the non-working REQUIRED flag
if( NOT SDL_FOUND )
    message( FATAL_ERROR "SDL(-dev) not found!" )
endif ( NOT SDL_FOUND )

if( NOT X11_FOUND )
    message(FATAL_ERROR "X11(-dev) not found")
endif( NOT X11_FOUND )

if( NOT Threads_FOUND )
    message(FATAL_ERROR "PTHREAD(-dev) not found")
endif( NOT Threads_FOUND )

if( NOT Boost_FOUND )
    message(FATAL_ERROR "Boost(-dev) not found")
endif( NOT Boost_FOUND )


# version number
set(OPENATOMIC_VERSION_MAJOR "0")
set(OPENATOMIC_VERSION_MINOR "5")
if(OPENATOMIC_IS_STABLE)
	set(OPENATOMIC_VERSION_PATCH "0")
else()
	set(OPENATOMIC_VERSION_PATCH ${OPENATOMIC_WC_REVISION})
endif()

# Various compile flags
add_definitions(-msse2)
#####add_definitions(-m32)
add_definitions(-O3)
#add_definitions(-rdynamic)
add_definitions(-D_GNU_SOURCE=1)
add_definitions(-D_THREAD_SAFE)
add_definitions(-D_REENTRANT)
####add_definitions(-DHAVE_OPENGL)
add_definitions(-I/usr/include/SDL/)

# Enabling all warnings in MSVC spams too much
if(NOT MSVC)
	add_definitions(-Wall)
endif(NOT MSVC)

# gcc uses some optimizations which might break stuff without this flag
add_definitions(-fno-strict-aliasing)
add_definitions(-Wno-unknown-pragmas)
# -fno-exceptions)

# includes
include(CheckCXXCompilerFlag)
include_directories(${Boost_INCLUDE_DIR})

link_libraries (
    ${CMAKE_THREAD_LIBS_INIT}
    ${X11_LIBRARIES}
    ${SDL_LIBRARY}
    ${SDLIMAGE_LIBRARY}
    ${SDLMIXER_LIBRARY}
    ${SDLNET_LIBRARY}
    guichan_sdl
    guichan
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_REGEX_LIBRARY}
    SDLmain # Sadly not included in SDL_LIBRARY variable
)

add_executable (
    openatomic # Executable Name
    WIN32 # Only if you don't want the DOS prompt to appear in the background in Windows
    MACOSX_BUNDLE
    src/3rdparty/astar/mapsearchnode.cpp
    src/3rdparty/glSDL/glSDL.c
    src/3rdparty/xmlgui/xmlgui.cpp
    src/3rdparty/xml/tinyxml.cpp
    src/3rdparty/xml/tinyxmlerror.cpp
    src/3rdparty/xml/tinyxmlparser.cpp
    src/ai/aicontroller.cpp
    src/ai/aigamearea.cpp
    src/ai/aigameareafield.cpp
    src/ai/aimodel.cpp
    src/ai/aiplayer.cpp
    src/audio/playsound.cpp
    src/core/animation.cpp
    src/core/gameengine.cpp
    src/core/gameobject.cpp
    src/core/gameresults.cpp
    src/core/gamestate.cpp
    src/core/gamingarea.cpp
    src/core/go_bomb.cpp
    src/core/go_player.cpp
    src/core/message.cpp
    src/core/resources.cpp
    src/core/scheduler.cpp
    src/core/seqparser.cpp
    src/core/tokenizer.cpp
    src/debug/errorreporter.cpp
    src/game/introstate.cpp
    src/game/menustate.cpp
    src/game/playstate.cpp
    src/game/resultsstate.cpp
    src/network/debugnetwork.cpp
    src/network/localserver.cpp
    src/network/localclient.cpp
    src/network/networkclient.cpp
    src/readers/alireader.cpp
    src/readers/anireader.cpp
    src/readers/cfgreader.cpp
    src/readers/filereader.cpp
    src/readers/fonreader.cpp
    src/readers/pcxreader.cpp
    src/readers/resreader.cpp
    src/render/objectdraw.cpp
    src/render/render.cpp
    src/render/stringdraw.cpp
    src/singletons/fontsingleton.cpp
    src/main.cpp
    src/old_main.cpp
#    ${CMAKE_CURRENT_BINARY_DIR}/svnversion.h
)

# include the output directory, where the svnversion.h file is generated
#include_directories(${CMAKE_CURRENT_BINARY_DIR})

# a custom target that is always built
#add_custom_target(svnheader ALL)

# creates svnversion.h using cmake script
#add_custom_command(TARGET svnheader COMMAND ${CMAKE_COMMAND}
#   -DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR} 
#   -P ${CMAKE_CURRENT_SOURCE_DIR}/getsvn.cmake)

# svnversion.h is a generated file
#set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/svnversion.h
#    PROPERTIES GENERATED TRUE
#    HEADER_FILE_ONLY TRUE)
                         
# explicitly say that the executable depends on the svnheader
#add_dependencies (
#    openatomic # executable name (must be same as add_executable)
#    svnheader
#)

########################################
# Install shared data files
#
if((NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin"))
    install(DIRECTORY build/DATA DESTINATION ${datadir} PATTERN)
    install(DIRECTORY build/gui DESTINATION ${datadir} PATTERN)
    install(DIRECTORY build/locale DESTINATION ${datadir} PATTERN)
    install(DIRECTORY build/sounds DESTINATION ${datadir} PATTERN)
    install(FILES build/FONT6.FON DESTINATION ${datadir})
	install(TARGETS openatomic RUNTIME DESTINATION ${bindir})
endif()

include(FindGettext)
if(GETTEXT_FOUND AND NOT DISABLE_WX)
	file(GLOB LINGUAS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} locale/*) 
	GETTEXT_CREATE_TRANSLATIONS(openatomic.pot ALL ${LINGUAS})
endif()

if((NOT ${CMAKE_SYSTEM_NAME} MATCHES "Linux|FreeBSD|Darwin"))
	install(FILES license.txt DESTINATION ${datadir})
endif()

# packaging information
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "openatomic")
set(CPACK_PACKAGE_VENDOR "Tescoon Software")
set(CPACK_PACKAGE_VERSION_MAJOR ${OPENATOMIC_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${OPENATOMIC_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${OPENATOMIC_VERSION_PATCH})
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsdl1.2debian,libsdl-image1.2,libsdl-mixer1.2,libguichan-0.8.1-1,libpthread")
set(CPACK_PACKAGE_DESCRIPTION "OpenAtomic Game")
set(CPACK_PACKAGE_CONTACT "sobotka.mira@gmail.com")

set(CPACK_SET_DESTDIR ON)

# CPack must be included after the CPACK_* variables are set in order for those
# variables to take effect.
include(CPack)
