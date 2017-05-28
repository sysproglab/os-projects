//
// Created by kivi on 17.05.17.
//
#include <string.h>
#include <stdlib.h>

#include "ini.h"

typedef struct {
    char* statusbar;
    char* desktop;
} configuration;

configuration config = { NULL, NULL };

static int handler(void* user, const char* section, const char* name,
                   const char* value)
{
    configuration* pconfig = (configuration*)user;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    if (MATCH("statusbar", "exe")) {
        pconfig->statusbar = strdup(value);
    } else if (MATCH("desktop", "exe")) {
        pconfig->desktop = strdup(value);
    } else {
        return 0;  /* unknown section/name, error */
    }
    return 1;
}

void init_config(const char* config_file) {
    if (ini_parse(config_file, handler, &config) < 0) {
        printf("Can't load %s\n", config_file);
        exit(1);
    }
}

const char* get_statusbar() {
    return config.statusbar;
}

const char* get_desktop() {
    return config.desktop;
}