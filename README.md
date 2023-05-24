# wg-tui
wg-tui is WireGuard text user interface.

Based on [wireguard-tools](https://git.zx2c4.com/wireguard-tools)

## Compile and install
```sh
$ git clone https://github.com/smotim/wg-tui
$ cd wg-tui
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Build options
* ``WG_TUI_INSTALL`` Generate the install target
* ``WG_TUI_PREFIX`` (default: ``/usr``): 
* ``WG_TUI_BINDIR`` (default: ``${WG_TUI_PREFIX}/bin``): 
* ``WG_TUI_LIBDIR`` (default: ``${WG_TUI_PREFIX}/lib``): 
* ``WG_TUI_MANDIR`` (default: ``${WG_TUI_PREFIX}/share/man``): 
* ``WG_TUI_BASHCOMPDIR`` (default: ``${WG_TUI_PREFIX}/share/bash-completion/completions``): 
* ``WG_TUI_RUNSTATEDIR`` (default: ``/var/lib``): 
* ``WG_TUI_WITH_BASHCOMPLETION`` (default: ``OFF``): Decides whether or not bash completion files for the tools are installed
* ``WG_TUI_WITH_WGQUICK`` (default: ``OFF``): Decides whether or not the wg-quick(8) script is installed
* ``WG_TUI_WITH_SYSTEMDUNITS`` (default: ``OFF``): Decides whether or not systemd units are installed for wg-quick(8)

## Additional information
* Authors:
  * Saveliy Pototskiy ([savalione.com](https://savalione.com))
  * Timofey Smolin ([github.com/smotim](https://github.com/smotim))
* Link to Github: https://github.com/smotim/wg-tui
* Date of creation: 2023-05-25