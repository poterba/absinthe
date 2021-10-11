/**
 * GreenPois0n Absinthe - crashreporter.c
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

#include "crashreporter.hpp"
#include "crashreport.hpp"
#include "crashreportcopy.hpp"
#include "crashreportmover.hpp"
#include "debug.hpp"
#include "file.hpp"
#include "lockdown.hpp"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace absinthe
{
namespace crashreport
{

Reporter::Reporter(std::shared_ptr<util::Device> device)
    : _device(device), _mover(std::make_shared<Mover>(device)),
      _copier(std::make_shared<Copy>(device))
{
}

Reporter::Reporter(std::shared_ptr<util::Device> device, uint16_t port)
    : _device(device), _mover(std::make_shared<Mover>(device, port)),
      _copier(std::make_shared<Copy>(device, port))
{
}

std::unique_ptr<Report> Reporter::last_crash()
{
    char** list = NULL;
    afc_error_t err = AFC_E_SUCCESS;

    if (!_mover || !_copier)
    {
        return {};
    }

    err = afc_read_directory(_copier->client(), "/", &list);
    if (err != AFC_E_SUCCESS)
    {
        return {};
    }

    char* lastItem = NULL;

    int i = 0;
    int j = 0;

    time_t latest = 0;

    for (i = 0; list[i] != NULL; i++)
    {
        if (!(strstr(list[i], "BackupAgent_") && strstr(list[i], ".plist")))
            continue;

        char** info = NULL;
        if (afc_get_file_info(_copier->client(), list[i], &info) != AFC_E_SUCCESS)
            continue;
        if (!info)
            continue;
        time_t mtime = 0;
        for (j = 0; info[j]; j += 2)
        {
            if (!strcmp(info[j], "st_mtime"))
            {
                mtime = atoll(info[j + 1]) / 1000000000;
            }
            free(info[j]);
            free(info[j + 1]);
        }
        free(info);
        if (mtime >= latest)
        {
            latest = mtime;
            lastItem = list[i];
        }
    }

    printf("Copying '%s'", lastItem);
    if (lastItem)
    {
        lastItem = strdup(lastItem);
    }
    for (i = 0; list[i]; i++)
    {
        free(list[i]);
    }
    free(list);
    if (!lastItem)
    {
        printf("hmm.. could not get last item");
        return {};
    }

    uint64_t handle;
    char data[0x1000];

    err = afc_file_open(_copier->client(), lastItem, AFC_FOPEN_RDONLY, &handle);
    if (err != AFC_E_SUCCESS)
    {
        printf("Unable to open %s", lastItem);
        free(lastItem);
        return {};
    }

    char crash_file[1024];
    mkstemp(crash_file);

    FILE* output = fopen(crash_file, "wb");
    if (output == NULL)
    {
        printf("Unable to open local file %s", crash_file);
        free(lastItem);
        afc_file_close(_copier->client(), handle);
        return {};
    }

    uint32_t bytes_read = 0;
    err = afc_file_read(_copier->client(), handle, data, 0x1000, &bytes_read);
    while (err == AFC_E_SUCCESS && bytes_read > 0)
    {
        fwrite(data, 1, bytes_read, output);
        err = afc_file_read(_copier->client(), handle, data, 0x1000, &bytes_read);
    }
    afc_file_close(_copier->client(), handle);
    fclose(output);

    afc_remove_path(_copier->client(), lastItem);

    uint32_t size = 0;
    plist_t plist = NULL;
    int ferr = 0;
    unsigned char* datas = NULL;
    ferr = util::file_read(crash_file, &datas, &size);
    if (ferr < 0)
    {
        fprintf(stderr, "Unable to open %s", crash_file);
        free(lastItem);
        return {};
    }

    plist_from_xml(static_cast<const char*>(datas), size, &plist);
    free(datas);

    std::unique_ptr<Report> report = NULL;
    if (plist)
    {
        report = std::make_unique<Report>(plist);
        plist_free(plist);
        remove(crash_file);
    }
    else
    {
        throw std::runtime_error("Reading crash report as plist failed");
    }
    free(lastItem);
    return report;
}

} // namespace crashreport
} // namespace absinthe
