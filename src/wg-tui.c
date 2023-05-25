/* SPDX-License-Identifier: GPL-2.0 */
/* wg-tui.c: Run application.
 *
 * Copyright (C) 2015-2020 Jason A. Donenfeld <Jason@zx2c4.com>. All Rights Reserved.
 * Copyright (C) 2023 Saveliy Pototskiy (savalione.com) <monologuesplus@gmail.com>
 */

#include <stddef.h>
#include <string.h>
#include <getopt.h>

#include <configure.h>
#include <wg-tui.h>

#include "subcommands.h"

const char *PROG_NAME;

static const struct {
	const char *subcommand;
	int (*function)(int, const char**);
	const char *description;
} subcommands[] = {
	{ "show", show_main, "Shows the current configuration and device information" },
	{ "showconf", showconf_main, "Shows the current configuration of a given WireGuard interface, for use with `setconf'" },
	{ "set", set_main, "Change the current configuration, add peers, remove peers, or change peers" },
	{ "setconf", setconf_main, "Applies a configuration file to a WireGuard interface" },
	{ "addconf", setconf_main, "Appends a configuration file to a WireGuard interface" },
	{ "syncconf", setconf_main, "Synchronizes a configuration file to a WireGuard interface" },
	{ "genkey", genkey_main, "Generates a new private key and writes it to stdout" },
	{ "genpsk", genkey_main, "Generates a new preshared key and writes it to stdout" },
	{ "pubkey", pubkey_main, "Reads a private key from stdin and writes a public key to stdout" }
};

static void show_usage(FILE *file)
{
	fprintf(file, "Usage: %s <cmd> [<args>]\n\n", PROG_NAME);
	fprintf(file, "Available subcommands:\n");
	for (size_t i = 0; i < sizeof(subcommands) / sizeof(subcommands[0]); ++i)
		fprintf(file, "  %s: %s\n", subcommands[i].subcommand, subcommands[i].description);
	fprintf(file, "You may pass `--help' to any of these subcommands to view usage.\n");
}

int main(int argc, const char *argv[])
{
	PROG_NAME = argv[0];

	if (argc == 2 && (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version") || !strcmp(argv[1], "version"))) {
		printf("wg-tui v%s - https://github.com/team4665/wg-tui/\n", WG_TUI_VERSION);
		return 0;
	}
	if (argc == 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help") || !strcmp(argv[1], "help"))) {
		show_usage(stdout);
		return 0;
	}

	if (argc == 1) {
		static const char *new_argv[] = { "show", NULL };
		return show_main(1, new_argv);
	}

	for (size_t i = 0; i < sizeof(subcommands) / sizeof(subcommands[0]); ++i) {
		if (!strcmp(argv[1], subcommands[i].subcommand))
			return subcommands[i].function(argc - 1, argv + 1);
	}

	fprintf(stderr, "Invalid subcommand: `%s'\n", argv[1]);
	show_usage(stderr);
	return 1;
}

int print_help(FILE *file)
{
	fprintf(file, "Usage: %s <cmd> [<args>]\n", PROG_NAME);
	printf("args:");
	printf("    -h, --help          Display help information and exit");
	printf("    -u, --build-info    Display build information end exit");
	return 0;
}

int print_build_info(FILE *file)
{
    char *use_bashcompletion = "NO";
    char *use_wgquick = "NO";
    char *use_systemdunits = "NO";

#ifdef WG_TUI_WITH_BASHCOMPLETION
    use_bashcompletion = "YES";
#endif

#ifdef WG_TUI_WITH_WGQUICK
    use_wgquick = "YES";
#endif

#ifdef WG_TUI_WITH_SYSTEMDUNITS
    use_systemdunits = "YES";
#endif

	fprintf(file, "Version: %s\n", WG_TUI_VERSION);
	fprintf(file, "  Major: %s\n", WG_TUI_VERSION_MAJOR);
	fprintf(file, "  Minor: %s\n", WG_TUI_VERSION_MINOR);
	fprintf(file, "  Patch: %s\n", WG_TUI_VERSION_PATCH);
	fprintf(file, "\n");
	fprintf(file, "Description: %s\n", WG_TUI_DESCRIPTION);
	fprintf(file, "Homepage url: %s\n", WG_TUI_HOMEPAGE_URL);
	fprintf(file, "\n");
	fprintf(file, "Project generator: %s\n", WG_TUI_GENERATOR);
	fprintf(file, "Project build type: %s\n", WG_TUI_BUILD_TYPE);
	fprintf(file, "\n");
	fprintf(file, "C compiler variables:\n");
	fprintf(file, "  Extensions: %s\n", WG_TUI_C_EXTENSIONS);
	fprintf(file, "  Standard: %s\n", WG_TUI_C_STANDARD);
	fprintf(file, "  Compiler: %s\n", WG_TUI_C_COMPILER);
	fprintf(file, "  Compiler id: %s\n", WG_TUI_C_COMPILER_ID);
	fprintf(file, "  Compiler predefines command: %s\n", WG_TUI_C_COMPILER_PREDEFINES_COMMAND);
	fprintf(file, "  Compiler target: %s\n", WG_TUI_C_COMPILER_TARGET);
    fprintf(file, "  Compiler version: %s\n", WG_TUI_C_COMPILER_VERSION);
    fprintf(file, "  Flags: %s\n", WG_TUI_C_FLAGS);
    fprintf(file, "  Simulate id: %s\n", WG_TUI_C_SIMULATE_ID);
    fprintf(file, "  Standard include derictories: %s\n", WG_TUI_C_STANDARD_INCLUDE_DIRECTORIES);
    fprintf(file, "  Standard libraries: %s\n", WG_TUI_C_STANDARD_LIBRARIES);
    fprintf(file, "  Standard required: %s\n", WG_TUI_C_STANDARD_REQUIRED);
    fprintf(file, "  Compiler ABI: %s\n", WG_TUI_C_COMPILER_ABI);
    fprintf(file, "  Compiler architecture id: %s\n", WG_TUI_C_COMPILER_ARCHITECTURE_ID);
    fprintf(file, "  Compiler compiler version internal: %s\n", WG_TUI_C_COMPILER_VERSION_INTERNAL);
    fprintf(file, "  Linker preference: %s\n", WG_TUI_C_LINKER_PREFERENCE);
    fprintf(file, "  Linker preference propagates: %s\n", WG_TUI_C_LINKER_PREFERENCE_PROPAGATES);
    fprintf(file, "  Platform id: %s\n", WG_TUI_C_PLATFORM_ID);
    fprintf(file, "\n");
    fprintf(file, "CMake version: %s\n", WG_TUI_CMAKE_VERSION);
    fprintf(file, "        Major: %s\n", WG_TUI_CMAKE_VERSION_MAJOR);
    fprintf(file, "        Minor: %s\n", WG_TUI_CMAKE_VERSION_MINOR);
    fprintf(file, "        Patch: %s\n", WG_TUI_CMAKE_VERSION_PATCH);
    fprintf(file, "\n");
    fprintf(file, "Application options:\n");
    fprintf(file, "  Prefix: %s\n", PREFIX);
    fprintf(file, "  Binary directory: %s\n", BINDIR);
    fprintf(file, "  Library directory: %s\n", LIBDIR);
    fprintf(file, "  Man pages directory: %s\n", MANDIR);
    fprintf(file, "  Bash completion scripts: %s\n", BASHCOMPDIR);
    fprintf(file, "  Run state directory: %s\n", RUNSTATEDIR);
    fprintf(file, "  Use bash completeon scripts: %s\n", use_bashcompletion);
    fprintf(file, "  Use wg quick: %s\n", use_wgquick);
    fprintf(file, "  Use systemd units: %s\n", use_systemdunits);

	return 0;
}