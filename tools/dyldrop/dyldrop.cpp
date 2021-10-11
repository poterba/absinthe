/**
 * GreenPois0n Absinthe - dyldrop.c
 * Copyright (C) 2010 Chronic-Dev Team
 * Copyright (C) 2010 Joshua Hill
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "common.hpp"
#include "debug.hpp"
#include "dyldcache.hpp"
#include "macho.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum
{
    MODE_NONE,
    MODE_DYLIB_SYM,
    MODE_DYLIB_LIST,
    MODE_SYM_SEARCH,
    MODE_SYM_HEADER,
    MODE_SYMDB
};

static void print_sym(const char* name, uint32_t addr, void* userdata)
{
    printf("#define %s (void*)0x%08x", name, addr);
}

static void print_sym_struct_elem(const char* name, uint32_t address, void* userdata)
{
    if (userdata) {
        fprintf(userdata, "\t{ \"%s\", 0x%08x },", name, address);
    } else {
        printf("\t{ \"%s\", 0x%08x },", name, address);
    }
}

static char* c_safe_name(const char* name)
{
    char* outname = (char*) malloc(strlen(name) + 1);
    int i;
    for (i = 0; i < strlen(name) + 1; i++) {
        switch (name[i]) {
            case ' ':
            case '.':
            case '-':
                outname[i] = '_';
                break;
            default:
                outname[i] = name[i];
                break;
        }
    }
    return outname;
}

int main(int argc, char* argv[])
{
    int i = 0;
    int ret = 0;
    int lastidx = 0;
    char* path = NULL;
    char* dylib = NULL;
    char* symbol = NULL;
    char* outpath = NULL;
    uint32_t address = 0xFFFFFFFF;
    macho_t* macho = NULL;
    dyldimage_t* image = NULL;
    dyldcache_t* cache = NULL;

    if ((argc < 4) && (argc != 3)) {
        char* name = strrchr(argv[0], '/');
        name = name ? name + 1 : argv[0];
        info(
            "Usage: %s <dyldcache> <dylib> <symbol>"
            "       %s <dyldcache> -l <dylib>"
            "       %s <dyldcache> -s <symbol>"
            "       %s <dyldcache> -h PATH"
            "       %s <dyldcache> -S <symbol1> [<symbol2> ...]"
            "       %s <mach-o> -l"
            "       %s <mach-o> <symbol>",
            name, name, name, name, name, name, name);
        return 0;
    }

    if (argc >= 4) {
        int mode = MODE_NONE;
        path = strdup(argv[1]);
        if (!strcmp(argv[2], "-s")) {
            dylib = NULL;
            symbol = strdup(argv[3]);
            mode = MODE_SYM_SEARCH;
        } else if (!strcmp(argv[2], "-l")) {
            dylib = strdup(argv[3]);
            symbol = NULL;
            mode = MODE_DYLIB_LIST;
        } else if (!strcmp(argv[2], "-h")) {
            dylib = NULL;
            symbol = NULL;
            outpath = strdup(argv[3]);
            mode = MODE_SYM_HEADER;
        } else if (!strcmp(argv[2], "-S")) {
            dylib = NULL;
            symbol = NULL;
            mode = MODE_SYMDB;
        } else {
            dylib = strdup(argv[2]);
            symbol = strdup(argv[3]);
            mode = MODE_DYLIB_SYM;
        }

        debug("Creating dyldcache from %s", path);
        cache = dyldcache_open(path);
        if (cache == NULL) {
            throw std::runtime_error("Unable to allocate memory for dyldcache");
            goto panic;
        }

        if (outpath) {
            mkdir_with_parents(outpath, 0755);
        }

        for (i = 0; i < cache->header->images_count; i++) {
            image = cache->images[i];
            // debug("Found %s", image->name);
            if ((dylib == NULL) || (strcmp(dylib, image->name) == 0)) {
                macho = macho_load(image->data, image->size, cache);
                if (macho == NULL) {
                    debug("Unable to parse Mach-O file in cache");
                    continue;
                }
                // macho_debug(macho);

                if (symbol) {
                    address = macho_lookup(macho, symbol);
                    if (address != 0) {
                        if (!dylib) {
                            printf("// %s:", image->name);
                        }
                        print_sym(symbol, address, NULL);
                    }
                } else if (!symbol && (mode == MODE_SYMDB)) {
                    int j;
                    int symno = 0;
                    char** symnames = (char**) malloc(sizeof(char*) * argc - 3);
                    uint32_t* symaddrs = (uint32_t*) malloc(sizeof(uint32_t) * argc - 3);
                    for (j = 3; j < argc; j++) {
                        address = macho_lookup(macho, argv[j]);
                        if (address != 0) {
                            symnames[symno] = argv[j];
                            symaddrs[symno] = address;
                            symno++;
                        }
                    }
                    if (symno > 0) {
                        char* cn = c_safe_name(image->name);
                        printf("// %s", image->name);
                        printf("struct %s_syms {", cn);
                        for (j = 0; j < symno; j++) {
                            char* csn = c_safe_name(symnames[j]);
                            printf("\tvoid* %s;", csn);
                            free(csn);
                        }
                        printf("};");
                        printf("struct %s_syms %s = {", cn, cn);
                        for (j = 0; j < symno; j++) {
                            printf("\t(void*)0x%x", symaddrs[j]);
                            if (j == symno - 1) {
                                printf("");
                            } else {
                                printf(",");
                            }
                        }
                        printf("};");
                        printf("");
                        free(cn);
                    }
                    free(symnames);
                    free(symaddrs);
                } else {
                    if (dylib) {
                        printf("// %s:", image->name);
                        macho_list_symbols(macho, print_sym, NULL);
                    } else {
                        char* cn = c_safe_name(image->name);
                        char* cf =
                            (char*) malloc(strlen(outpath) + 1 + strlen(image->name) + 2 + 1);
                        strcpy(cf, outpath);
                        strcat(cf, "/");
                        strcat(cf, image->name);
                        strcat(cf, ".hpp");

                        FILE* f = fopen(cf, "wb");
                        if (f) {
                            fprintf(f, "// %s", image->name);
                            fprintf(f, "static struct symaddr %s_syms[] {", cn);
                            macho_list_symbols(macho, print_sym_struct_elem, f);
                            fprintf(f, "\t{ NULL, 0 }");
                            fprintf(f, "};");
                            fclose(f);
                        }
                        free(cf);
                        free(cn);
                    }
                }

                macho_free(macho);
                macho = NULL;
            }
        }

        dyldcache_free(cache);
        cache = NULL;
    } else if (argc == 3) {
        path = strdup(argv[1]);
        symbol = strdup(argv[2]);

        macho = macho_open(path);
        if (macho == NULL) {
            debug("Unable to parse Mach-O file");
        }
        // macho_debug(dylib);

        if (strcmp(symbol, "-l") == 0) {
            macho_list_symbols(macho, print_sym, NULL);
            address = 0;
        } else {
            address = macho_lookup(macho, symbol);
            if (address != 0) {
                printf("#define %s (void*)0x%08x", symbol, address);
            }
        }
    }

    if (address == 0xFFFFFFFF) {
        goto panic;
    }

    goto finish;

panic:
    throw std::runtime_error("ERROR: %d", ret == 0 ? -1 : ret);

finish:
    debug("Cleaning up");
    if (cache)
        dyldcache_free(cache);
    if (macho)
        macho_free(macho);
    if (path)
        free(path);
    if (symbol)
        free(symbol);
    if (dylib)
        free(dylib);
    if (outpath)
        free(outpath);
    return ret;
}
