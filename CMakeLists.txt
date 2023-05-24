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

set(WG_TUI_PREFIX "/usr" CACHE STRING "Prefix directory for installing wg-tui")
set(WG_TUI_BINDIR "${WG_TUI_PREFIX}/bin" CACHE STRING "Directory for binaries")
set(WG_TUI_LIBDIR "${WG_TUI_PREFIX}/lib" CACHE STRING "Directory for libraries")
set(WG_TUI_MANDIR "${WG_TUI_PREFIX}/share/man" CACHE STRING "Directory for man")
set(WG_TUI_BASHCOMPDIR "${WG_TUI_PREFIX}/share/bash-completion/completions" CACHE STRING "Directory for bash completion files")
set(WG_TUI_RUNSTATEDIR "/var/lib" CACHE STRING "Run state dir")

option(WG_TUI_WITH_BASHCOMPLETION "Decides whether or not bash completion files for the tools are installed" OFF)
option(WG_TUI_WITH_WGQUICK "Decides whether or not the wg-quick(8) script is installed" OFF)
option(WG_TUI_WITH_SYSTEMDUNITS "Decides whether or not systemd units are installed for wg-quick(8)" OFF)

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
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		INCLUDES DESTINATION "include"
	)

	install(FILES "src/ctype.h" DESTINATION "include")
    install(FILES "src/config.h" DESTINATION "include")
    install(FILES "src/ipc-linux.h" DESTINATION "include")
    install(FILES "src/configure.h" DESTINATION "include")
    install(FILES "src/encoding.h" DESTINATION "include")
    install(FILES "src/ipc.h" DESTINATION "include")
    install(FILES "src/curve25519-fiat32.h" DESTINATION "include")
    install(FILES "src/ipc-windows.h" DESTINATION "include")
    install(FILES "src/curve25519-hacl64.h" DESTINATION "include")
    install(FILES "src/containers.h" DESTINATION "include")
    install(FILES "src/ipc-uapi.h" DESTINATION "include")
    install(FILES "src/ipc-openbsd.h" DESTINATION "include")
    install(FILES "src/terminal.h" DESTINATION "include")
    install(FILES "src/ipc-uapi-windows.h" DESTINATION "include")
    install(FILES "src/ipc-uapi-unix.h" DESTINATION "include")
    install(FILES "src/version.h" DESTINATION "include")
    install(FILES "src/netlink.h" DESTINATION "include")
    install(FILES "src/ipc-freebsd.h" DESTINATION "include")
    install(FILES "src/subcommands.h" DESTINATION "include")
    install(FILES "src/curve25519.h" DESTINATION "include")

    # install(
    # 	EXPORT "wg-tuiTargets"
    # 	DESTINATION "lib/cmake/wg-tui"
	# )
endif()