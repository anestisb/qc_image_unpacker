/*

   qc_image_unpacker
   -----------------------------------------

   Dmitry Baryshkov <dmitry.baryshkov@linaro.org>
   Copyright 2019 - 2020 by CENSUS S.A. All Rights Reserved.
   Copyright 2021 by Linaro Ltd.

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

#ifndef _BOOTLDR_IMAGE_H_
#define _BOOTLDR_IMAGE_H_

#include "common.h"

#define BOOTLDR_IMG_MAGIC1 0x544f4f42
#define BOOTLDR_IMG_MAGIC2 0x2152444c
#define BOOTLDR_PARTITION_NAME_SZ 0x40
//#define MAX_IMAGES_IN_BOOTLDRIMG 32

typedef struct bootldr_header {
  u4 magic1;
  u4 magic2;
  u4 images;
  u4 start_offset;
  u4 full_size;
} bootldr_header_t;

typedef struct bootldr_img_header_entry {
  char ptn_name[BOOTLDR_PARTITION_NAME_SZ];
  u4 size;
} bootldr_img_header_entry_t;

bool bootldr_image_detect(u1 *, size_t);
bool bootldr_image_extract(u1 *, size_t, char *, char *);

#endif
