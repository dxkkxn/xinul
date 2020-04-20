/*
 * Ensimag - Projet système
 * Copyright (C) 2012 - Damien Dejean <dam.dejean@gmail.com>
 *
 * Defines structure to find user space programs.
 */

#include "string.h"
#include "stdio.h"
#include "assert.h"

#include "userspace_apps.h"

const struct uapps *find_app(const char *name)
{
#ifdef STUDENT
	assert(0 && "Not yet implemented.");
	return NULL;
#else // END STUDENT
        const struct uapps *app = symbols_table;

        while (app->name != NULL) {
                if (strcmp(name, app->name) == 0) {
                        return app;
                }
                app++;
        }
        return NULL;
#endif // STUDENT
}

