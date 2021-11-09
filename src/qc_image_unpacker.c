/*

   qc_image_unpacker
   -----------------------------------------

   Anestis Bechtsoudis <anestis@census-labs.com>
   Copyright 2019 - 2020 by CENSUS S.A. All Rights Reserved.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

*/

#include <getopt.h>
#include <libgen.h>

#include "common.h"
#include "log.h"
#include "meta_image.h"
#include "packed_image.h"
#include "utils.h"

// clang-format off
static void usage(bool exit_success) {
  LOGMSG_RAW(l_INFO, "             " PROG_NAME " - ver. " PROG_VERSION "\n");
  LOGMSG_RAW(l_INFO, PROG_AUTHORS "\n\n");
  LOGMSG_RAW(l_INFO,"%s",
             " -i, --input=<path>   : input path (directory to search recursively or single file)\n"
             " -o, --output=<path>  : output path (default is same as input)\n"
             " -f, --file-override  : allow output file override if already exists (default: false)\n"
             " -v, --debug=LEVEL    : log level (0 - FATAL ... 4 - DEBUG), default: '3' (INFO)\n"
             " -h, --help           : this help\n");

  if (exit_success)
    exit(EXIT_SUCCESS);
  else
    exit(EXIT_FAILURE);
}
// clang-format on

int main(int argc, char **argv) {
  int c;
  int logLevel = l_INFO;
  runArgs_t pRunArgs = {
    .outputDir = NULL,
    .fileOverride = false,
  };
  infiles_t pFiles = {
    .inputFile = NULL,
    .files = NULL,
    .fileCnt = 0,
  };

  if (argc < 1) usage(true);

  struct option longopts[] = {
    { "input", required_argument, 0, 'i' },   { "output", required_argument, 0, 'o' },
    { "file-override", no_argument, 0, 'f' }, { "debug", required_argument, 0, 'v' },
    { "help", no_argument, 0, 'h' },          { 0, 0, 0, 0 }
  };

  while ((c = getopt_long(argc, argv, "i:o:fv:h?", longopts, NULL)) != -1) {
    switch (c) {
      case 'i':
        pFiles.inputFile = optarg;
        break;
      case 'o':
        pRunArgs.outputDir = optarg;
        break;
      case 'f':
        pRunArgs.fileOverride = true;
        break;
      case 'v':
        logLevel = atoi(optarg);
        break;
      case '?':
      case 'h':
        usage(true);
        break;
      default:
        exit(EXIT_FAILURE);
        break;
    }
  }

  // Adjust log level
  if (logLevel < 0 || logLevel >= l_MAX_LEVEL) {
    LOGMSG(l_FATAL, "Invalid debug level '%d'", logLevel);
  }
  log_setMinLevel(logLevel);

  // Initialize input files
  if (!utils_init_files(&pFiles)) {
    LOGMSG(l_FATAL, "Couldn't load input files");
    exit(EXIT_FAILURE);
  }

  // Check output directory
  if (pRunArgs.outputDir && !utils_isValidDir(pRunArgs.outputDir)) {
    LOGMSG(l_FATAL, "'%s' output directory is not valid", pRunArgs.outputDir);
    exit(EXIT_FAILURE);
  }

  // It output directory not set, put extracted images under input directory
  if (!pRunArgs.outputDir)
    pRunArgs.outputDir =
        utils_isValidDir(pFiles.inputFile) ? pFiles.inputFile : dirname(pFiles.inputFile);

  size_t processedImgs = 0;
  LOGMSG(l_INFO, "Processing %zu file(s) from %s", pFiles.fileCnt, pFiles.inputFile);

  for (size_t f = 0; f < pFiles.fileCnt; f++) {
    off_t fileSz = 0;
    int srcfd = -1;
    u1 *buf = NULL;

    LOGMSG(l_DEBUG, "Processing '%s'", pFiles.files[f]);

    buf = utils_mapFileToRead(pFiles.files[f], &fileSz, &srcfd);
    if (buf == NULL) {
      LOGMSG(l_ERROR, "Open & map failed - skipping '%s'", pFiles.files[f]);
      continue;
    }

    /*
     * Check only if we have something to detect here.
     * Individual _image_detect() functions check header size.
     */
    if ((size_t)fileSz < sizeof(u4)) {
      LOGMSG(l_ERROR, "Invalid input size - skipping '%s'", pFiles.files[f]);
      goto next_file;
    }

    if (meta_image_detect(buf, (size_t)fileSz)) {
      LOGMSG(l_DEBUG, "Meta image header found");
      if (!meta_image_extract(buf, (size_t)fileSz, pFiles.files[f], pRunArgs.outputDir)) {
        LOGMSG(l_ERROR, "Skipping '%s'", pFiles.files[f]);
        goto next_file;
      }
    } else if (packed_image_detect(buf, (size_t)fileSz)) {
      LOGMSG(l_DEBUG, "packed image header found");
      if (!packed_image_extract(buf, (size_t)fileSz, pFiles.files[f], pRunArgs.outputDir)) {
        LOGMSG(l_ERROR, "Skipping '%s'", pFiles.files[f]);
        goto next_file;
      }
    } else {
      LOGMSG(l_ERROR, "Invalid magic header 0x%x - skipping '%s'", *(u4*)buf,
             pFiles.files[f]);
      goto next_file;
    }

    processedImgs++;

  next_file:
    // Clean-up
    munmap(buf, fileSz);
    buf = NULL;
    close(srcfd);
  }

  LOGMSG(l_INFO, "%zu known images found from %u input files", processedImgs, pFiles.fileCnt);
  LOGMSG(l_INFO, "Extracted images files are available in '%s'", pRunArgs.outputDir);

  if (pFiles.fileCnt > 1) {
    for (size_t i = 0; i < pFiles.fileCnt; i++) {
      free(pFiles.files[i]);
    }
  }
  free(pFiles.files);
  exit(EXIT_SUCCESS);
}
