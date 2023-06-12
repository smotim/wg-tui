cmake_minimum_required (VERSION 3.13.1)

project(wg-tui
    VERSION "1.0.20210914"
    DESCRIPTION "WireGuard text user interface"
    HOMEPAGE_URL "https://github.com/smotim/wg-tui"
    LANGUAGES C
)

include(GNUInstallDirs)

# Determine if wg-tui is built as a subproject (using add_subdirectory)
# or if it is the master project.
if(NOT DEFINED WG_TUI_MASTER_PROJECT)
	set(WG_TUI_MASTER_PROJECT OFF)
	if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    	set(WG_TUI_MASTER_PROJECT ON)
    	message(STATUS "CMake version: ${CMAKE_VERSION}")
  	endif ()
endif ()

# Options
option(WG_TUI_INSTALL "Generate the install target" ${WG_TUI_MASTER_PROJECT})

set(WG_TUI_BINDIR "${CMAKE_INSTALL_BINDIR}" CACHE STRING "Directory for binaries")
set(WG_TUI_LIBDIR "${CMAKE_INSTALL_LIBDIR}" CACHE STRING "Directory for libraries")
set(WG_TUI_INCDIR "${CMAKE_INSTALL_PREFIX}/include" CACHE STRING "Directory for includes")
set(WG_TUI_MANDIR "${CMAKE_INSTALL_PREFIX}/share/man" CACHE STRING "Directory for man pages")
set(WG_TUI_BASHCOMPDIR "${CMAKE_INSTALL_PREFIX}/share/bash-completion/completions" CACHE STRING "Directory for bash completion files")
set(WG_TUI_SYSTEMD "${CMAKE_INSTALL_PREFIX}/systemd" CACHE STRING "Directory for systemd files")
set(WG_TUI_RUNSTATEDIR "${CMAKE_INSTALL_LIBDIR}" CACHE STRING "Run state dir")

option(WG_TUI_WITH_INCLUDE "Decides whether or not include files for the tools are installed" ON)
option(WG_TUI_WITH_BASHCOMPLETION "Decides whether or not bash completion files for the tools are installed" ON)
option(WG_TUI_WITH_MAN "Decides whether or not man files for the tools are installed" ON)
option(WG_TUI_WITH_WGQUICK "Decides whether or not the wg-tui script is installed" ON)
option(WG_TUI_WITH_SYSTEMDUNITS "Decides whether or not systemd units are installed for wg-tui" ON)

# Set default build to release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()

message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})

# It creates export commands for future ide dependency resolution
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# CMake configure
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/configure.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/configure.h)

# Sources
set(WG_TUI_SRC
    src/setconf.c
    src/genkey.c
    src/encoding.c
    src/config.c
    src/ipc.c
    src/terminal.c
    src/showconf.c
    src/curve25519.c
    src/set.c
    src/wg.c
    src/show.c
    src/pubkey.c
    src/configure.c
)

# Executable
add_executable(${PROJECT_NAME} ${WG_TUI_SRC})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)

if(WG_TUI_INSTALL)
	install(
		TARGETS ${PROJECT_NAME}
		EXPORT "wg-tuiTargets"
        LIBRARY DESTINATION ${WG_TUI_LIBDIR}
        ARCHIVE DESTINATION ${WG_TUI_LIBDIR}
        RUNTIME DESTINATION ${WG_TUI_BINDIR}
		INCLUDES DESTINATION ${WG_TUI_INCDIR}
	)

    if(WG_TUI_WITH_INCLUDE)
	    install(FILES "src/ctype.h" DESTINATION ${WG_TUI_INCDIR})
        install(FILES "src/config.h" DESTINATION ${WG_TUI_INCDIR})
        install(FILES "src/ipc-linux.h" DESTINATION ${WG_TUI_INCDIR})
        install(FILES "src/configure.h" DESTINATION ${WG_TUI_INCDIR})
        install(FILES "src/encoding.h" DESTINATION ${WG_TUI_INCDIR})
        install(FILES "src/ipc.h" DESTINATION ${WG_TUI_INCDIR})
        install(FILES "src/curve25519-fiat32.h" DESTINATION ${WG_TUI_INCDIR})
        install(FILES "src/ipc-windows.h" DESTINATION ${WG_TUI_INCDIR})
        install(FILES "src/curve25519-hacl64.h" DESTINATION ${WG_TUI_INCDIR})
        install(FILES "src/containers.h" DESTINATION ${WG_TUI_INCDIR})
        install(FILES "src/ipc-uapi.h" DESTINATION ${WG_TUI_INCDIR})
        install(FILES "src/ipc-openbsd.h" DESTINATION ${WG_TUI_INCDIR})
        install(FILES "src/terminal.h" DESTINATION ${WG_TUI_INCDIR})
        install(FILES "src/ipc-uapi-windows.h" DESTINATION ${WG_TUI_INCDIR})
        install(FILES "src/ipc-uapi-unix.h" DESTINATION ${WG_TUI_INCDIR})
        install(FILES "src/version.h" DESTINATION ${WG_TUI_INCDIR})
        install(FILES "src/netlink.h" DESTINATION ${WG_TUI_INCDIR})
        install(FILES "src/ipc-freebsd.h" DESTINATION ${WG_TUI_INCDIR})
        install(FILES "src/subcommands.h" DESTINATION ${WG_TUI_INCDIR})
        install(FILES "src/curve25519.h" DESTINATION ${WG_TUI_INCDIR})
    endif()

    if(WG_TUI_WITH_BASHCOMPLETION)
        install(FILES "src/completion/wg-quick.bash-completion" DESTINATION ${WG_TUI_BASHCOMPDIR})
        install(FILES "src/completion/wg.bash-completion" DESTINATION ${WG_TUI_BASHCOMPDIR})
    endif()

    if(WG_TUI_WITH_MAN)
        install(FILES "src/man/wg-quick.8" DESTINATION ${WG_TUI_MANDIR})
        install(FILES "src/man/wg.8" DESTINATION ${WG_TUI_MANDIR})
    endif()

    if(WG_TUI_WITH_WGQUICK)
        set(WG_TUI_BIN_PATH "${CMAKE_INSTALL_PREFIX}/${WG_TUI_BINDIR}/${PROJECT_NAME}")
        set(WG_TUI_LINK_PATH "${CMAKE_INSTALL_PREFIX}/${WG_TUI_BINDIR}/wg")
        install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${WG_TUI_BIN_PATH} ${WG_TUI_LINK_PATH} )")
    endif()

    if(WG_TUI_WITH_SYSTEMDUNITS)
        install(FILES "src/systemd/wg-quick.target" DESTINATION ${WG_TUI_SYSTEMD})
        install(FILES "src/systemd/wg-quick@.service" DESTINATION ${WG_TUI_SYSTEMD})
    endif()

    # install(
    # 	EXPORT "wg-tuiTargets"
    # 	DESTINATION "lib/cmake/wg-tui"
	# )
endif()