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

#include "meta_image.h"

#include "common.h"
#include "utils.h"

bool meta_image_extract(u1 *buf, size_t bufSz, char *filePath, char *outputDir) {
  meta_header_t *pMetaHeader;
  meta_img_header_entry_t *pImgHeaderEntry;
  u4 i = 0, j = 0, images = 0;
  u1 *pImageEnd = NULL;
  bool PnameTerminated = false;
  char outPath[PATH_MAX], outFile[PATH_MAX];

  if (bufSz < sizeof(meta_header_t)) {
    LOGMSG(l_ERROR, "Invalid input size (%zu < %zu)", bufSz, sizeof(meta_header_t));
    return false;
  }

  pMetaHeader = (meta_header_t *)buf;
  if (pMetaHeader->magic != META_IMG_MAGIC) {
    LOGMSG(l_ERROR, "Invalid magic header (0x%x != 0x%x)", pMetaHeader->magic, META_IMG_MAGIC);
    return false;
  }

  LOGMSG(l_DEBUG, "Image version: %s", pMetaHeader->img_version);

  pImgHeaderEntry = (meta_img_header_entry_t *)(buf + sizeof(meta_header_t));
  images = pMetaHeader->img_hdr_sz / sizeof(meta_img_header_entry_t);
  if (images > MAX_IMAGES_IN_METAIMG) {
    LOGMSG(l_ERROR, "Number of images (%u) in meta_image are greater than expected", images);
    return false;
  }

  if ((size_t)bufSz <= sizeof(meta_header_t) + pMetaHeader->img_hdr_sz) {
    LOGMSG(l_ERROR, "The size is smaller than image header size + entry size");
    return false;
  }

  pImageEnd = buf + bufSz;

  // Create output root directory to place extracted images
  memset(outPath, 0, sizeof(outPath));
  snprintf(outPath, sizeof(outPath), "%s/%s_images", outputDir, utils_fileBasename(filePath));
  if (mkdir(outPath, 0755)) {
    LOGMSG_P(l_ERROR, "mkdir(%s) failed", outPath);
    return false;
  }

  LOGMSG(l_DEBUG, "Processing '%u' images", images);
  for (i = 0; i < images; i++) {
    int dstfd = -1;
    PnameTerminated = false;
    if (pImgHeaderEntry[i].ptn_name[0] == 0x00 || pImgHeaderEntry[i].start_offset == 0 ||
        pImgHeaderEntry[i].size == 0)
      break;

    if (pImageEnd < buf + pImgHeaderEntry[i].start_offset + pImgHeaderEntry[i].size) {
      LOGMSG(l_ERROR, "Image size mismatch");
      return false;
    }

    for (j = 0; j < META_PARTITION_NAME_SZ; j++) {
      if (!(pImgHeaderEntry[i].ptn_name[j])) {
        PnameTerminated = true;
        break;
      }
    }
    if (!PnameTerminated) {
      LOGMSG(l_ERROR, "ptn_name string not terminated properly");
      return false;
    }

    // Write output file
    memset(outFile, 0, sizeof(outFile));
    snprintf(outFile, sizeof(outFile), "%s/%s", outPath, pImgHeaderEntry[i].ptn_name);
    dstfd = open(outFile, O_CREAT | O_EXCL | O_RDWR, 0644);
    if (dstfd == -1) {
      LOGMSG_P(l_ERROR, "Couldn't create output file '%s' in input directory", outFile);
      return false;
    }

    if (!utils_writeToFd(dstfd, buf + pImgHeaderEntry[i].start_offset, pImgHeaderEntry[i].size)) {
      close(dstfd);
      return false;
    }

    close(dstfd);
  }

  return true;
}
