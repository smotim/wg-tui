/* SPDX-License-Identifier: GPL-2.0 */
/* wg-tui.h: Definitions for wg-tui.
 *
 * Copyright (C) 2023 Saveliy Pototskiy (savalione.com) <monologuesplus@gmail.com>
 */
#ifndef WG_TUI_H
#define WG_TUI_H

#include <stdio.h>

/* Display help information */
int print_help(FILE *file);

/* Display build information */
int print_build_info(FILE *file);

/* Display version information */
int print_version(FILE *file);

#endif // WG_TUI_H