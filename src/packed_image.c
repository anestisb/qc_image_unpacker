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

#include "packed_image.h"

#include "common.h"
#include "utils.h"

static bool process_fbpt(u1 *buf, size_t bufSz) {
  fbpt_header_t *pFBPT = (fbpt_header_t *)buf;
  if (pFBPT->signature != FBPT_SIGNATURE) {
    LOGMSG(l_ERROR, "Unknown partition table format");
    return false;
  }
  LOGMSG(l_DEBUG, "type:0x%x lun:0x%x num_of_partitions:0x%x", pFBPT->type, pFBPT->lun,
         pFBPT->num_of_partitions);

  if (pFBPT->num_of_partitions >= MAX_PARTITIONS) {
    LOGMSG(l_ERROR, "Number of partitions is too big (0x%x)", pFBPT->num_of_partitions);
    return false;
  }

  if (pFBPT->num_of_partitions * sizeof(fbpt_entry_t) + sizeof(fbpt_header_t) != bufSz) {
    LOGMSG(l_ERROR, "Corrupted partition table");
    return false;
  }

  for (u4 i = 0; i < pFBPT->num_of_partitions; i++) {
    fbpt_entry_t *pEntry = (fbpt_entry_t *)(buf + sizeof(fbpt_header_t) + i * sizeof(fbpt_entry_t));
    LOGMSG_RAW(l_INFO, "  %i: %s (size:0x%x ?:0x%x attributes:0x%x type:%s id:%s)\n", i,
               pEntry->partition_name, pEntry->size, pEntry->unknown2, pEntry->attributes,
               pEntry->type_guid, pEntry->partition_guid);
    // utils_hexDump(pEntry->partition_name, (u1*)pEntry, sizeof(fbpt_entry_t));
  }

  return true;
}

bool packed_image_extract(u1 *buf, size_t bufSz, char *filePath, char *outputDir) {
  packed_header_t *pPackedHeader;
  packed_img_header_entry_t *pImgHeaderEntry;
  u1 *pLastValidEntry, *pImageEnd;
  u4 i = 0, entries = 0;
  char outPath[PATH_MAX], outFile[PATH_MAX];
  int dstFD = -1;

  if (bufSz < sizeof(packed_header_t)) {
    LOGMSG(l_ERROR, "Invalid input size (%zu < %zu)", bufSz, sizeof(packed_header_t));
    return false;
  }

  pPackedHeader = (packed_header_t *)buf;
  if (pPackedHeader->magic != PACKED_IMG_MAGIC) {
    LOGMSG(l_ERROR, "Invalid magic header (0x%x != 0x%x)", pPackedHeader->magic, PACKED_IMG_MAGIC);
    return false;
  }

  if (pPackedHeader->version > MIN_SUPPORTED_VERSION) {
    LOGMSG(l_ERROR, "Unsupported version (0x%x > 0x%x)", pPackedHeader->version,
           MIN_SUPPORTED_VERSION);
    return false;
  }

  if (pPackedHeader->total_file_sz != bufSz) {
    LOGMSG(l_ERROR, "Pack image corrupted, expected size %zu but got %zu",
           pPackedHeader->total_file_sz, bufSz);
    return false;
  }

  LOGMSG(l_DEBUG, "Image version: %s", pPackedHeader->img_version);

  pImgHeaderEntry = (packed_img_header_entry_t *)(buf + sizeof(packed_header_t));
  entries = pPackedHeader->num_of_entries;

  if ((size_t)bufSz < pPackedHeader->total_file_sz) {
    LOGMSG(l_ERROR, "Unexpected file size (0x%x < 0x%x)", bufSz, pPackedHeader->total_file_sz);
    return false;
  }

  pImageEnd = buf + bufSz;
  pLastValidEntry = pImageEnd - sizeof(packed_img_header_entry_t);

  // Create output root directory to place extracted images
  memset(outPath, 0, sizeof(outPath));
  snprintf(outPath, sizeof(outPath), "%s/%s_images", outputDir, utils_fileBasename(filePath));
  if (mkdir(outPath, 0755) && errno != EEXIST) {
    LOGMSG_P(l_ERROR, "mkdir(%s) failed", outPath);
    return false;
  }

  LOGMSG(l_DEBUG, "Processing '%u' entries in packed image", entries);
  for (i = 0; i < entries; i++) {
    u8 expandedSize = (u8)(pImgHeaderEntry->partition_sz) | ((u8)(pImgHeaderEntry->pad1) << 32);
    u8 expandedNextOffet = (u8)(pImgHeaderEntry->next_offset) | ((u8)(pImgHeaderEntry->pad2) << 32);

    LOGMSG_RAW(l_INFO, "%u: %s (type:0x%x size:0x%llx next:0x%llx)\n", i,
               pImgHeaderEntry->partition_name, pImgHeaderEntry->type, expandedSize,
               expandedNextOffet);
    // Overflow checks
    if (((u1 *)pImgHeaderEntry > pLastValidEntry) || (expandedSize & 0x8000000000000000ull) ||
        ((u1 *)(pImgHeaderEntry) + expandedSize + sizeof(packed_img_header_entry_t) > pImageEnd)) {
      LOGMSG(l_ERROR, "Corrupted packed image");
      return false;
    }

    // Ensure null termination of description string (not an issue as pad1 MSB is always zero)
    *(u1 *)(pImgHeaderEntry + 39) = 0x0;

    // In case of partition table entry simply print information, nothing to extract
    if (pImgHeaderEntry->type == 0x0) {
      // pImgHeaderEntry->partition_sz);
      if (!process_fbpt((u1 *)pImgHeaderEntry + sizeof(packed_img_header_entry_t), expandedSize)) {
        LOGMSG(l_ERROR, "Partition table processing failed");
        return false;
      }
      goto next;
    }

    // Write output file
    memset(outFile, 0, sizeof(outFile));
    if (snprintf(outFile, sizeof(outFile), "%s/%s", outPath, pImgHeaderEntry->partition_name) < 0) {
      LOGMSG(l_ERROR, "Failed to construct output path");
      return false;
    }
    dstFD = open(outFile, O_CREAT | O_RDWR, 0644);
    if (dstFD == -1) {
      LOGMSG_P(l_ERROR, "Couldn't create output file '%s' in input directory", outFile);
      return false;
    }

    if (!utils_writeToFd(dstFD, (u1 *)pImgHeaderEntry + sizeof(packed_img_header_entry_t),
                         expandedSize)) {
      LOGMSG_P(l_ERROR, "Write to '%s' failed", outFile);
      close(dstFD);
      dstFD = -1;
      return false;
    }

    close(dstFD);
    dstFD = -1;

    // Point to next entry
  next:
    pImgHeaderEntry = (packed_img_header_entry_t *)(buf + expandedNextOffet);
  }

  return true;
}
