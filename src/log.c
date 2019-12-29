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

#include "log.h"

#include <stdarg.h>
#include <sys/time.h>
#include <time.h>

#include "common.h"

static unsigned int log_minLevel;
static bool log_isTTY;
static int log_fd;
static bool inside_line;

__attribute__((constructor)) void log_init(void) {
  log_minLevel = l_INFO;
  log_fd = STDOUT_FILENO;
  log_isTTY = isatty(log_fd);
}

void log_setMinLevel(log_level_t dl) { log_minLevel = dl; }

void log_msg(log_level_t dl,
             bool perr,
             bool raw_print,
             const char *file,
             const char *func,
             int line,
             const char *fmt,
             ...) {
  struct {
    char *descr;
    char *prefix;
  } logLevels[] = { { "[FATAL]", "\033[1;31m" },
                    { "[ERROR]", "\033[1;35m" },
                    { "[WARNING]", "\033[1;33m" },
                    { "[INFO]", "\033[1m" },
                    { "[DEBUG]", "\033[0;37m" } };

  char strerr[512];
  if (perr) {
    snprintf(strerr, sizeof(strerr), "%s", strerror(errno));
  }

  if (dl > log_minLevel) return;

  struct tm tm;
  struct timeval tv;

  gettimeofday(&tv, NULL);
  localtime_r((const time_t *)&tv.tv_sec, &tm);

  if (inside_line && !raw_print) {
    dprintf(log_fd, "\n");
  }

  if (log_isTTY) {
    dprintf(log_fd, "%s", logLevels[dl].prefix);
  }

  if (raw_print) {
    int fmtLen = strlen(fmt);
    if (fmtLen > 0 && fmt[fmtLen - 1] == '\n') {
      inside_line = false;
    } else {
      inside_line = true;
    }
  } else {
    if (log_minLevel >= l_DEBUG || !log_isTTY) {
      dprintf(log_fd, "%s [%d] %d/%02d/%02d %02d:%02d:%02d (%s:%d %s) ", logLevels[dl].descr,
              getpid(), tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
              tm.tm_sec, file, line, func);
    } else {
      dprintf(log_fd, "%s ", logLevels[dl].descr);
    }
  }

  va_list args;
  va_start(args, fmt);
  vdprintf(log_fd, fmt, args);
  va_end(args);

  if (perr) {
    dprintf(log_fd, ": %s", strerr);
  }

  if (log_isTTY) {
    dprintf(log_fd, "\033[0m");
  }

  if (!raw_print) {
    dprintf(log_fd, "\n");
  }
  fflush(stdout);

  if (dl == l_FATAL) {
    exit(EXIT_FAILURE);
  }
}
