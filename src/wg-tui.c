/* SPDX-License-Identifier: GPL-2.0 */
/* wg-tui.c: Run application.
 *
 * Copyright (C) 2015-2020 Jason A. Donenfeld <Jason@zx2c4.com>. All Rights Reserved.
 * Copyright (C) 2023 Saveliy Pototskiy (savalione.com) <monologuesplus@gmail.com>
 */

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <getopt.h>

#include <configure.h>
#include <wg-tui.h>

#include "subcommands.h"

const char *PROG_NAME;

// static const struct {
// 	const char *subcommand;
// 	int (*function)(int, const char**);
// 	const char *description;
// } subcommands[] = {
// 	{ "show", show_main, "Shows the current configuration and device information" },
// 	{ "showconf", showconf_main, "Shows the current configuration of a given WireGuard interface, for use with `setconf'" },
// 	{ "set", set_main, "Change the current configuration, add peers, remove peers, or change peers" },
// 	{ "setconf", setconf_main, "Applies a configuration file to a WireGuard interface" },
// 	{ "addconf", setconf_main, "Appends a configuration file to a WireGuard interface" },
// 	{ "syncconf", setconf_main, "Synchronizes a configuration file to a WireGuard interface" },
// 	{ "genkey", genkey_main, "Generates a new private key and writes it to stdout" },
// 	{ "genpsk", genkey_main, "Generates a new preshared key and writes it to stdout" },
// 	{ "pubkey", pubkey_main, "Reads a private key from stdin and writes a public key to stdout" }
// };

// static void show_usage(FILE *file)
// {
// 	fprintf(file, "Usage: %s <cmd> [<args>]\n\n", PROG_NAME);
// 	fprintf(file, "Available subcommands:\n");
// 	for (size_t i = 0; i < sizeof(subcommands) / sizeof(subcommands[0]); ++i)
// 		fprintf(file, "  %s: %s\n", subcommands[i].subcommand, subcommands[i].description);
// 	fprintf(file, "You may pass `--help' to any of these subcommands to view usage.\n");
// }

/* getopt options */
// #define OPT_DEVICE      1000
#define OPT_SHOWCONF    1001
#define OPT_SET         1002
#define OPT_SETCONF     1003
#define OPT_ADDCONF     1004
#define OPT_SYNCCONF    1005
#define OPT_GENKEY      1006
#define OPT_GENPSK      1007
#define OPT_PUBKEY      1008


int main(int argc, char **argv)
{
	PROG_NAME = argv[0];

    if(argc <= 1)
        return print_help(stdout);

    /* Options */
    static struct option long_options[] =
        {
            {"help", no_argument, 0, 'h'},
            {"version", no_argument, 0, 'v'},
            {"build-info", no_argument, 0, 'b'},

            {"device", required_argument, 0, 'd'},
            {"show", optional_argument, 0, 's'},

            {"showconf", required_argument, 0, OPT_SHOWCONF},
            {"set", required_argument, 0, OPT_SET},
            {"setconf", required_argument, 0, OPT_SETCONF},
            {"addconf", required_argument, 0, OPT_ADDCONF},
            {"syncconf", required_argument, 0, OPT_SYNCCONF},
            {"genkey", required_argument, 0, OPT_GENKEY},
            {"genpsk", required_argument, 0, OPT_GENPSK},
            {"pubkey", required_argument, 0, OPT_PUBKEY},

            {0, 0, 0, 0}
        };
    
    char *device = "NULL";

    int opts;
    while((opts = getopt_long(argc, argv, "hvbs::d:", long_options, NULL)) != -1)
    {
        switch (opts)
        {
            case 'd':
                printf("device: %s\n", optarg);
                device = optarg;
                break;
            case 'h':
                return print_help(stdout);
                break;
            case 'v':
                return print_version(stdout);
                break;
            case 'b':
                return print_build_info(stdout);
                break;
            case 's':
                if(optarg == NULL)
                {
                    if(strcmp(device, "NULL"))
                        return show(device);
                    else
                    {
                        fprintf(stdout, "You need to specify device via --device [name]\n");
                        return -1;
                    }
                }
                else if(!strcmp(optarg, "interfaces"))
                    return show_interfaces();
                else
                {
                    fprintf(stdout, "-s, --show %s (key %s not found)\n", optarg, optarg);
                    return -1;
                }
                break;
            case OPT_SHOWCONF:
                printf("OPT_SHOWCONF: %s\n", optarg);
                return 0;
                break;
            case OPT_SET:
                return 0;
                break;
            case OPT_SETCONF:
                return 0;
                break;
            case OPT_ADDCONF:
                return 0;
                break;
            case OPT_SYNCCONF:
                return 0;
                break;
            case OPT_GENKEY:
                return 0;
                break;
            case OPT_GENPSK:
                return 0;
                break;
            case OPT_PUBKEY:
                return 0;
                break;
            default:
                return 0;
                break;
        }
    }

	// for (size_t i = 0; i < sizeof(subcommands) / sizeof(subcommands[0]); ++i) {
	// 	if (!strcmp(argv[1], subcommands[i].subcommand))
	// 		return subcommands[i].function(argc - 1, argv + 1);
	// }

	// fprintf(stderr, "Invalid subcommand: `%s'\n", argv[1]);
	// show_usage(stderr);
	return 1;
}

int print_help(FILE *file)
{
	fprintf(file, "Usage: %s [options]\n", PROG_NAME);
	printf("options:\n");
	printf("    -h, --help              Display help information and exit\n");
    printf("    -v, --version           Display version information and exit\n");
	printf("    -b, --build-info        Display build information end exit\n");
    printf("    -s, --show [options]    Shows the current configuration and device information\n");
    printf("        options:\n");
    printf("            none\n");
    printf("            all\n");
    printf("            interfaces\n");
    printf("        --showconf          Shows the current configuration of a given WireGuard interface, for use with --setconf\n");
    printf("        --set               Change the current configuration, add peers, remove peers, or change peers\n");
    printf("        --setconf           Applies a configuration file to a WireGuard interface\n");
    printf("        --addconf           Appends a configuration file to a WireGuard interface\n");
    printf("        --syncconf          Synchronizes a configuration file to a WireGuard interface\n");
    printf("        --genkey            Generates a new private key and writes it to stdout\n");
    printf("        --genpsk            Generates a new preshared key and writes it to stdout\n");
    printf("        --pubkey            Reads a private key from stdin and writes a public key to stdout\n");
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

int print_version(FILE *file)
{
    fprintf(file, "wg-tui version: %s\n", WG_TUI_VERSION);
    return 0;
}