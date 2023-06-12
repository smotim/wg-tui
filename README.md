# wg-tui
wg-tui is WireGuard text user interface.

Based on [wireguard-tools](https://git.zx2c4.com/wireguard-tools)

## Compile and install
```sh
$ git clone https://github.com/team4665/wg-tui 
$ cd wg-tui
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Launch
```sh
$ python3 python/main.py
```

## Build options
* ``WG_TUI_INSTALL`` Generate the install target
* ``WG_TUI_BINDIR`` (default: ``${CMAKE_INSTALL_PREFIX}/bin``): Directory for binaries
* ``WG_TUI_LIBDIR`` (default: ``${CMAKE_INSTALL_PREFIX}/lib``): Directory for libraries
* ``WG_TUI_INCDIR`` (default: ``${CMAKE_INSTALL_PREFIX}/include``): Directory for includes
* ``WG_TUI_MANDIR`` (default: ``${CMAKE_INSTALL_PREFIX}/share/man``): Directory for man pages
* ``WG_TUI_BASHCOMPDIR`` (default: ``${CMAKE_INSTALL_PREFIX}/share/bash-completion/completions``): Directory for bash completion files
* ``WG_TUI_SYSTEMD`` (default: ``${CMAKE_INSTALL_PREFIX}/systemd``): Directory for systemd files
* ``WG_TUI_RUNSTATEDIR`` (default: ``${CMAKE_INSTALL_LIBDIR}``): Run state dir
* ``WG_TUI_WITH_INCLUDE`` (default: ``ON``): Decides whether or not include files for the tools are installed
* ``WG_TUI_WITH_BASHCOMPLETION`` (default: ``ON``): Decides whether or not bash completion files for the tools are installed
* ``WG_TUI_WITH_MAN`` (default: ``ON``): Decides whether or not man files for the tools are installed
* ``WG_TUI_WITH_WGQUICK`` (default: ``ON``): Decides whether or not the wg-tui script is installed
* ``WG_TUI_WITH_SYSTEMDUNITS`` (default: ``ON``): Decides whether or not systemd units are installed for wg-tui

## Additional information
* Authors:
  * Saveliy Pototskiy ([savalione.com](https://savalione.com))
  * Timofey Smolin ([github.com/smotim](https://github.com/smotim))
  * Timur Zhakeev (https://github.com/Zhakey567)
* Link to Github: https://github.com/smotim/wg-tui
* Date of creation: 2023-05-25
