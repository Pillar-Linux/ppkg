/* Pillar package-script(ppkg) header */
#ifndef PPKG_H
#define PPKG_H

#include "version.h"

#define ARG_MAX _POSIX_ARG_MAX

/* PACKAGE_PATH */
#define PACKAGE_PATH_DEFAULT "/usr/src/packages/"   /* default package path: /usr/local/src/packages */

/* PACKAGE_VERSION */
#define PACKAGE_VERSION_OUTPUT printf("ppkg version: %s\n", PACKAGE_VERSION_RAW);

/* PACKAGE_CONF */
#define PACKAGE_CONF_ARRAY_SIZE 32                                              /* sets arbitrary limit for number of lines in .conf toml arrays */
#define PACKAGE_CONF_ARRAY_WIDTH 128                                            /* sets arbitrary limit for width of lines in .conf toml arrays */

/* PACKAGE_BUILD */
#define PACKAGE_BUILD_SRC "src"

/* DEBUG */
#define DEBUG_SEGFAULT_PRINT if (debug_flag == 1){printf("debug> segfault line: %d\n", __LINE__);}
#define DEBUG_FUNCTION_PRINT if (debug_flag == 1){printf("\ndebug> ----%s----\n", __func__);}

#endif
