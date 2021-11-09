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

#ifndef _META_IMAGE_H_
#define _META_IMAGE_H_

#include "common.h"

#define META_IMG_MAGIC 0xce1ad63c
#define META_PARTITION_NAME_SZ 72
#define MAX_IMAGES_IN_METAIMG 32

typedef struct meta_header {
  u4 magic;
  u2 major_version;     /* (0x1) - reject images with higher major versions */
  u2 minor_version;     /* (0x0) - allow images with higher minor versions */
  char img_version[64]; /* Top level version for images in this meta */
  u2 meta_hdr_sz;       /* size of this header */
  u2 img_hdr_sz;        /* size of img_header_entry list */
} meta_header_t;

typedef struct meta_img_header_entry {
  char ptn_name[META_PARTITION_NAME_SZ];
  u4 start_offset;
  u4 size;
} meta_img_header_entry_t;

bool meta_image_detect(u1 *, size_t);
bool meta_image_extract(u1 *, size_t, char *, char *);

#endif
