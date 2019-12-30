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

#ifndef _PACKED_IMAGE_H_
#define _PACKED_IMAGE_H_

#include "common.h"

#define PACKED_IMG_MAGIC 0x4b504246  // "FBPK"
#define MIN_SUPPORTED_VERSION 0x1
#define FBPT_SIGNATURE 0x54504246  // "FBPT"
#define MAX_PARTITIONS 0x101
#define GUID_STR_SIZE 8 + 4 + 4 + 4 + 16 + 1  // Includes null termination

typedef struct packed_header {
  u4 magic;
  u4 version;
  char img_version[68];
  u4 num_of_entries;
  u4 total_file_sz;
} packed_header_t;

typedef struct packed_img_header_entry {
  u4 type;
  char partition_name[32];
  u4 pad1;  // Most likely used for images with size > UINT32_MAX
  u4 partition_sz;
  u4 pad2;  // Most likely used for images with size > UINT32_MAX (offset needs bigger ranges)
  u4 next_offset;
  u4 checksum;
} packed_img_header_entry_t;

#if 0
  0000  46 42 50 54 01 00 00 00 01 00 00 00 00 00 00 00  FBPT............
  0010  03 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
  0020  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
  0030  00 00 00 00 00 00 00 00 00 30 30 30 00 00 38 00  .........000..8.
  0040  00 00 00 00 04 00 00 00 78 62 6c 5f 61 00 00 00  ........xbl_a...
  0050  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
  0060  00 00 00 00 00 00 00 00 00 00 00 00 44 45 41 30  ............DEA0
  0070  42 41 32 43 2d 43 42 44 44 2d 34 38 30 35 2d 42  BA2C-CBDD-4805-B
  0080  34 46 39 2d 46 34 32 38 32 35 31 43 33 45 39 38  4F9-F428251C3E98
  0090  00 31 61 37 37 37 31 64 37 2d 62 34 30 66 2d 34  .1a7771d7-b40f-4
  00a0  65 64 33 2d 39 32 65 31 2d 34 35 35 63 33 36 35  ed3-92e1-455c365
  00b0  61 39 32 36 31 00 30 30 00 00 02 00 00 00 00 00  a9261.00........
  00c0  00 00 00 00 78 62 6c 5f 63 6f 6e 66 69 67 5f 61  ....xbl_config_a
  00d0  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
  00e0  00 00 00 00 00 00 00 00 35 41 33 32 35 41 45 34  ........5A325AE4
  00f0  2d 34 32 37 36 2d 42 36 36 44 2d 30 41 44 44 2d  -4276-B66D-0ADD-
  0100  33 34 39 34 44 46 32 37 37 30 36 41 00 33 34 34  3494DF27706A.344
  0110  66 61 33 63 32 2d 31 61 62 36 2d 34 62 39 37 2d  fa3c2-1ab6-4b97-
  0120  61 64 36 35 2d 66 35 63 31 32 63 63 62 39 61 36  ad65-f5c12ccb9a6
  0130  30 00 30 30 00 00 00 00 00 00 00 00 14 00 00 00  0.00............
  0140  6c 61 73 74 5f 70 61 72 74 69 00 00 00 00 00 00  last_parti......
  0150  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
  0160  00 00 00 00 30 30 30 30 30 30 30 30 2d 30 30 30  ....00000000-000
  0170  30 2d 30 30 30 30 2d 30 30 30 30 2d 30 30 30 30  0-0000-0000-0000
  0180  30 30 30 30 30 30 30 30 00 37 64 33 39 32 30 31  00000000.7d39201
  0190  37 2d 36 34 35 31 2d 34 38 34 64 2d 39 33 30 38  7-6451-484d-9308
  01a0  2d 66 38 63 32 39 32 35 65 31 65 35 39 00 30 30  -f8c2925e1e59.00
#endif

typedef struct fbpt_header {
  u4 signature;
  u4 type;  // 0 = MBR, 1 = GPT, 2 = GPT-backup
  u4 lun;
  u4 unknown;
  u4 num_of_partitions;
  char unknown2[37];
  char pad[3];    // usually is 0x303030
} fbpt_header_t;  // sizeof == 60

#if 0
  0000  00 00 08 00 00 00 00 00 04 00 00 00 61 6f 70 5f  ............aop_
  0010  61 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  a...............
  0020  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
  0030  44 36 39 45 39 30 41 35 2d 34 43 41 42 2d 30 30  D69E90A5-4CAB-00
  0040  37 31 2d 46 36 44 46 2d 41 42 39 37 37 46 31 34  71-F6DF-AB977F14
  0050  31 41 37 46 00 65 35 66 39 32 30 33 63 2d 33 61  1A7F.e5f9203c-3a
  0060  64 33 2d 34 32 39 31 2d 61 38 34 33 2d 39 38 64  d3-4291-a843-98d
  0070  33 63 34 65 38 34 30 36 39 00 30 30              3c4e84069.00
#endif

typedef struct fbpt_entry {
  u4 size;
  u4 unknown2;
  u4 attributes;
  char partition_name[36];
  char type_guid[GUID_STR_SIZE];
  char partition_guid[GUID_STR_SIZE];
  char pad[2];   // Usually is 0x3030
} fbpt_entry_t;  // sizeof == 124

bool packed_image_extract(u1 *, size_t, char *, char *);

#endif
