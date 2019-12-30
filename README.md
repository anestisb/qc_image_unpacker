# Android Qualcomm Image Unpacker

Tool to extract meta & packed Qualcomm images used in Android devices. These type of images are
mostly used from bootloader and modem images. List of currently supported magic headers are:

1. 0xce1ad63c: msm8996
2. 0x4b504246: sdm845, sdm855, sdm670


## Compile

* Clone this repository
* Install Android NDK if you want to cross-compile for Android devices
* Invoke `make.sh` bash script with desired build target
  * `$ ./make.sh` - if CC not defined from env, gcc is used by default
  * `$ ./make.sh gcc` - compile with gcc
  * `$ ./make.sh clang` - compile with clang
  * `$ ./make.sh cross-android` - cross-compile (arm64-v8a, x86 & x86_64) for Android with NDK
* Executables are copied under the `bin` directory
* For debug builds use `$ DEBUG=true ./make.sh`


## Usage

```
             qc_image_unpacker - ver. 0.1.0
       Anestis Bechtsoudis <anestis@census-labs.com>
  Copyright 2019 - 2020 by CENSUS S.A. All Rights Reserved.

 -i, --input=<path>   : input path (directory to search recursively or single file)
 -o, --output=<path>  : output path (default is same as input)
 -f, --file-override  : allow output file override if already exists (default: false)
 -v, --debug=LEVEL    : log level (0 - FATAL ... 4 - DEBUG), default: '3' (INFO)
 -h, --help           : this help
 ```

### Example of Pixel 3 bootloader unpack

```
$ bin/qc_image_unpacker -i ~/Desktop/tmp/crosshatch-pd1a.180720.030/bootloader-crosshatch-b1c1-0.1-4948814.img
[INFO] Processing 1 file(s) from /Users/anestisb/Desktop/tmp/crosshatch-pd1a.180720.030/bootloader-crosshatch-b1c1-0.1-4948814.img
0: partition table (type:0x0 size:0xa68 next:0xb00)
  0: ssd (size:0x2000 ?:0x0 attributes:0x0 type:2C86E742-745E-4FDD-BFD8-B6A7AC638772 id:71616e45-a9e6-4af3-a5b9-e5310b65509f)
  1: misc (size:0x100000 ?:0x0 attributes:0x0 type:82ACC91F-357C-4A68-9C8F-689E1B1A23A1 id:8c89c720-2202-4513-952a-fe58ebf6fc57)
  2: keystore (size:0x80000 ?:0x0 attributes:0x0 type:DE7D4029-0F5B-41C8-AE7E-F6C023A02B33 id:a8cac5a1-7611-4352-a725-c289440d3420)
  3: frp (size:0x80000 ?:0x0 attributes:0x0 type:91B72D4D-71E0-4CBF-9B8E-236381CFF17A id:ec3b9549-713b-45c8-82ad-c9c7a3b7f803)
  4: system_a (size:0xb0000000 ?:0x0 attributes:0x0 type:97D7B011-54DA-4835-B3C4-917AD6E73D74 id:8077038c-7869-4b9a-b9e2-95fd830e7f53)
  5: system_b (size:0xb0000000 ?:0x0 attributes:0x0 type:77036CD4-03D5-42BB-8ED1-37E5A88BAA34 id:23d330b1-36ee-4fa2-8ee3-6963fbaaf813)
  6: product_a (size:0x12c00000 ?:0x0 attributes:0x0 type:CAED77DB-A3D0-4A16-8D9B-16A3258D9405 id:eb70f1c6-39f4-4ffd-948e-bac8085264ce)
  7: product_b (size:0x12c00000 ?:0x0 attributes:0x0 type:77036CD4-03D5-42BB-8ED1-37E5A88BAA34 id:2b1c749c-9157-4422-9e66-d47024f8d512)
  8: vendor_a (size:0x30000000 ?:0x0 attributes:0x0 type:97D7B011-54DA-4835-B3C4-917AD6E73D74 id:445b1620-0339-4176-bf1f-4ebbc1a42f93)
  9: vendor_b (size:0x30000000 ?:0x0 attributes:0x0 type:77036CD4-03D5-42BB-8ED1-37E5A88BAA34 id:e3826f7e-a90d-4897-bdcb-00bee60a2a35)
  10: boot_a (size:0x4000000 ?:0x0 attributes:0x0 type:20117F86-E985-4357-B9EE-374BC1D8487D id:3187b997-2da3-4e8b-adb2-59809e84a13c)
  11: boot_b (size:0x4000000 ?:0x0 attributes:0x0 type:77036CD4-03D5-42BB-8ED1-37E5A88BAA34 id:f395b5f3-068f-4a8b-9214-9ca48371345e)
  12: modem_a (size:0x5000000 ?:0x0 attributes:0x4 type:EBD0A0A2-B9E5-4433-87C0-68B6B72699C7 id:9773821c-7272-4f48-bb6b-29e26fceb022)
  13: modem_b (size:0x5000000 ?:0x0 attributes:0x4 type:77036CD4-03D5-42BB-8ED1-37E5A88BAA34 id:2d2cfeb7-4ab7-4de1-a950-1e4dbe1780b5)
  14: apdp_a (size:0x40000 ?:0x0 attributes:0x0 type:E6E98DA2-E22A-4D12-AB33-169E7DEAA507 id:e5546e9a-77c7-45f8-84af-d7a4e48dc7e3)
  15: apdp_b (size:0x40000 ?:0x0 attributes:0x0 type:77036CD4-03D5-42BB-8ED1-37E5A88BAA34 id:34e4e0a0-e56e-423f-a178-2765a8d0cb33)
  16: msadp_a (size:0x40000 ?:0x0 attributes:0x0 type:ED9E8101-05FA-46B7-82AA-8D58770D200B id:1baa0f9a-cc3c-4242-9a82-209eb6e2c524)
  17: msadp_b (size:0x40000 ?:0x0 attributes:0x0 type:77036CD4-03D5-42BB-8ED1-37E5A88BAA34 id:73eeb0d3-f179-40e7-a9ab-698d17e588e3)
  18: klog (size:0x400000 ?:0x0 attributes:0x0 type:5AF80809-AABB-4943-9168-CDFC38742598 id:55877eeb-04bc-4859-84d7-44c10edc0318)
  19: metadata (size:0x1000000 ?:0x0 attributes:0x0 type:A6C01AE4-9E89-4D18-A8DA-5BEB269B209A id:42e6545f-c2b8-437a-bcd2-a8350e0d2b45)
  20: userdata (size:0x0 ?:0x3 attributes:0x30 type:1B81E7E6-F50D-419B-A739-2AEEF8DA3335 id:825a5e5b-2768-47ef-97c1-42ccdec6f681)
1: partition table (type:0x0 size:0x1b0 next:0xcf0)
  0: xbl_a (size:0x380000 ?:0x0 attributes:0x4 type:DEA0BA2C-CBDD-4805-B4F9-F428251C3E98 id:1a7771d7-b40f-4ed3-92e1-455c365a9261)
  1: xbl_config_a (size:0x20000 ?:0x0 attributes:0x0 type:5A325AE4-4276-B66D-0ADD-3494DF27706A id:344fa3c2-1ab6-4b97-ad65-f5c12ccb9a60)
  2: last_parti (size:0x0 ?:0x0 attributes:0x14 type:00000000-0000-0000-0000-000000000000 id:7d392017-6451-484d-9308-f8c2925e1e59)
2: partition table (type:0x0 size:0x1b0 next:0xee0)
  0: xbl_b (size:0x380000 ?:0x0 attributes:0x4 type:DEA0BA2C-CBDD-4805-B4F9-F428251C3E98 id:bc3788ea-91eb-49c8-8668-74e7cdcd9da3)
  1: xbl_config_b (size:0x20000 ?:0x0 attributes:0x0 type:5A325AE4-4276-B66D-0ADD-3494DF27706A id:01b8a326-5a82-4f7d-b327-00893e98a3c8)
  2: last_parti (size:0x0 ?:0x0 attributes:0x14 type:00000000-0000-0000-0000-000000000000 id:8764a772-f7fa-4900-8243-f7eac6699360)
3: partition table (type:0x0 size:0xc58 next:0x1b70)
  0: aop_a (size:0x80000 ?:0x0 attributes:0x4 type:D69E90A5-4CAB-0071-F6DF-AB977F141A7F id:2b976ffc-4e96-46ca-b952-bf3014e1a783)
  1: tz_a (size:0x200000 ?:0x0 attributes:0x4 type:A053AA7F-40B8-4B1C-BA08-2F68AC71A4F4 id:cd24c7e8-95c1-4e53-b691-1c450fe87fe8)
  2: hyp_a (size:0x80000 ?:0x0 attributes:0x0 type:E1A6A689-0C8D-4CC6-B4E8-55A4320FBD8A id:5524ebe8-3835-4aea-b3fe-60cb6da5bca1)
  3: abl_a (size:0x200000 ?:0x0 attributes:0x4 type:BD6928A1-4CE0-A038-4F3A-1495E3EDDFFB id:74c9508f-eaa1-4361-843b-de9fcae05454)
  4: keymaster_a (size:0x80000 ?:0x0 attributes:0x4 type:A11D2A7C-D82A-4C2F-8A01-1805240E6626 id:5355f785-e3c3-469a-be3f-cf0303f0ef8a)
  5: cmnlib_a (size:0x80000 ?:0x0 attributes:0x4 type:73471795-AB54-43F9-A847-4F72EA5CBEF5 id:50176365-24ac-4f0d-814c-8e6b3ab0497d)
  6: cmnlib64_a (size:0x80000 ?:0x0 attributes:0x4 type:8EA64893-1267-4A1B-947C-7C362ACAAD2C id:cff20a4c-4ee9-4a68-8e40-170ed84841e7)
  7: devcfg_a (size:0x20000 ?:0x0 attributes:0x0 type:F65D4B16-343D-4E25-AAFC-BE99B6556A6D id:77b49964-e044-4d01-9688-55d32ed3dcbd)
  8: qupfw_a (size:0x10000 ?:0x0 attributes:0x0 type:21d1219f-2ed1-4ab4-930a-41a16ae75f7f id:bd832198-b242-4329-9fae-9806af6c5b65)
  9: vbmeta_a (size:0x10000 ?:0x0 attributes:0x4 type:4b7a15d6-322c-42ac-8110-88b7da0c5d77 id:810d295c-0690-408f-964e-570876f97995)
  10: dtbo_a (size:0x800000 ?:0x0 attributes:0x0 type:24d0d418-d31d-4d8d-ac2c-4d4305188450 id:7deb4687-0699-4e5e-acab-7ab297b6c536)
  11: storsec_a (size:0x20000 ?:0x0 attributes:0x0 type:02DB45FE-AD1B-4CB6-AECC-0042C637DEFA id:094bd422-49a0-4289-8d68-e4d12b6b9502)
  12: aop_b (size:0x80000 ?:0x0 attributes:0x0 type:77036CD4-03D5-42BB-8ED1-37E5A88BAA34 id:26dfb2ed-3a4d-4829-9692-ad794db2ce6b)
  13: tz_b (size:0x200000 ?:0x0 attributes:0x0 type:77036CD4-03D5-42BB-8ED1-37E5A88BAA34 id:590514c1-255f-424c-b6b4-daa3798cea76)
  14: hyp_b (size:0x80000 ?:0x0 attributes:0x0 type:77036CD4-03D5-42BB-8ED1-37E5A88BAA34 id:70f42b66-6976-4678-b83d-445298d6f692)
  15: abl_b (size:0x200000 ?:0x0 attributes:0x4 type:77036CD4-03D5-42BB-8ED1-37E5A88BAA34 id:8a3c3069-0b95-4cd7-80c3-69c8232b7917)
  16: keymaster_b (size:0x80000 ?:0x0 attributes:0x4 type:77036CD4-03D5-42BB-8ED1-37E5A88BAA34 id:560334e8-db5c-46a8-822d-551c1f3f27df)
  17: cmnlib_b (size:0x80000 ?:0x0 attributes:0x4 type:77036CD4-03D5-42BB-8ED1-37E5A88BAA34 id:6ef528b0-a46e-4915-a5c4-0db38c9d9dc3)
  18: cmnlib64_b (size:0x80000 ?:0x0 attributes:0x4 type:77036CD4-03D5-42BB-8ED1-37E5A88BAA34 id:58297aa1-8050-403e-a795-82dffbd32b7f)
  19: devcfg_b (size:0x20000 ?:0x0 attributes:0x0 type:77036CD4-03D5-42BB-8ED1-37E5A88BAA34 id:ea92f80d-2515-4528-912e-7ee56398b0d6)
  20: qupfw_b (size:0x10000 ?:0x0 attributes:0x0 type:77036CD4-03D5-42BB-8ED1-37E5A88BAA34 id:634d61aa-4d28-4b05-8a89-486e18330e36)
  21: vbmeta_b (size:0x10000 ?:0x0 attributes:0x4 type:77036CD4-03D5-42BB-8ED1-37E5A88BAA34 id:8d38b3c1-aaa5-4b56-8451-d26faf804dbf)
  22: dtbo_b (size:0x800000 ?:0x0 attributes:0x0 type:77036CD4-03D5-42BB-8ED1-37E5A88BAA34 id:82fa4e2a-8978-4b07-a822-4a5beb5c0734)
  23: storsec_b (size:0x20000 ?:0x0 attributes:0x0 type:77036CD4-03D5-42BB-8ED1-37E5A88BAA34 id:71f17687-5082-4d8f-a082-9d2e2b55e1aa)
  24: last_parti (size:0x0 ?:0x0 attributes:0x14 type:00000000-0000-0000-0000-000000000000 id:23b5f9fc-e6ca-4f3f-b043-92784c86f47d)
4: partition table (type:0x0 size:0x7fc next:0x23b0)
  0: ALIGN_TO_128K_2 (size:0x1a000 ?:0x0 attributes:0x4 type:6891a3b7-0ccc-4705-bb53-2673cac193bd id:4ba09905-d110-4beb-85a1-24291b258d78)
  1: persist (size:0x2800000 ?:0x0 attributes:0x0 type:6C95E238-E343-4BA8-B489-8681ED22AD0B id:82f03a4c-cbf9-495d-8df3-c38d388cf4e0)
  2: modemst1 (size:0x200000 ?:0x0 attributes:0x0 type:EBBEADAF-22C9-E33B-8F5D-0E81686A68CB id:bd44af79-86e1-44b3-a3cf-ee46c0055eea)
  3: modemst2 (size:0x200000 ?:0x0 attributes:0x0 type:0A288B1F-22C9-E33B-8F5D-0E81686A68CB id:65b83d51-30eb-46e8-b3d4-604313611d09)
  4: fsg (size:0x200000 ?:0x0 attributes:0x4 type:638FF8E2-22C9-E33B-8F5D-0E81686A68CB id:86dc26ed-f5a1-4b3e-8fc7-e516d2b11d41)
  5: fsc (size:0x20000 ?:0x0 attributes:0x0 type:57B90A16-22C9-E33B-8F5D-0E81686A68CB id:4c13e845-d02c-4d19-806b-96230bc09451)
  6: devinfo (size:0x1000 ?:0x0 attributes:0x4 type:65ADDCF4-0C5C-4D9A-AC2D-D90B5CBFCD03 id:dd47f7cc-29e8-4bfd-9c2f-0a46366ee36f)
  7: dip (size:0x100000 ?:0x0 attributes:0x4 type:4114B077-005D-4E12-AC8C-B493BDA684FB id:b6c8d7e5-8f57-48e5-932d-0b4a04ffe8e1)
  8: spunvm (size:0x800000 ?:0x0 attributes:0x0 type:e42e2b4c-33b0-429b-b1ef-d341c547022c id:8d7ddcc7-70b2-42d4-b9f6-eb6c871fa254)
  9: limits (size:0x1000 ?:0x0 attributes:0x4 type:10A0C19C-516A-5444-5CE3-664C3226A794 id:728e50e1-5eb8-4969-9d00-b758f448eb4a)
  10: toolsfv (size:0x100000 ?:0x0 attributes:0x4 type:97745ABA-135A-44C3-9ADC-05616173C24C id:ea2bb4b3-0c05-4693-aebb-906ad8487b76)
  11: logfs (size:0x800000 ?:0x0 attributes:0x0 type:BC0330EB-3410-4951-A617-03898DBE3372 id:fe7a3419-7850-42d7-843b-e6ef1aa52294)
  12: sti (size:0x200000 ?:0x0 attributes:0x0 type:AA9A5C4C-4F1F-7D3A-014A-22BD33BF7191 id:9eb99e3b-1cff-4dd5-b1be-3c9e468cf6f7)
  13: ImageFv (size:0x200000 ?:0x0 attributes:0x0 type:17911177-C9E6-4372-933C-804B678E666F id:a8384106-40d7-4c01-8a40-f385700d5c48)
  14: splash (size:0x4000 ?:0x0 attributes:0x0 type:AD99F201-DC71-4E30-9630-E19EEF553D1B id:a89e8da7-8986-4a2e-96a9-b42ebe6cbea7)
  15: last_parti (size:0x0 ?:0x0 attributes:0x14 type:00000000-0000-0000-0000-000000000000 id:b313e512-d232-4daa-913d-1be5c3c44fa6)
5: msadp (type:0x1 size:0x4000 next:0x63f0)
6: xbl (type:0x1 size:0x37c000 next:0x382430)
7: xbl_config (type:0x1 size:0x18000 next:0x39a470)
8: aop (type:0x1 size:0x2d000 next:0x3c74b0)
9: tz (type:0x1 size:0x1f6000 next:0x5bd4f0)
10: hyp (type:0x1 size:0x5f000 next:0x61c530)
11: abl (type:0x1 size:0x100000 next:0x71c570)
12: keymaster (type:0x1 size:0x35000 next:0x7515b0)
13: cmnlib (type:0x1 size:0x5c000 next:0x7ad5f0)
14: cmnlib64 (type:0x1 size:0x79000 next:0x826630)
15: devcfg (type:0x1 size:0xa000 next:0x830670)
16: qupfw (type:0x1 size:0x10000 next:0x8406b0)
17: storsec (type:0x1 size:0x6000 next:0x8466f0)
18: logfs (type:0x1 size:0x4000 next:0x84a730)
[INFO] 1 known images found from 1 input files
[INFO] Extracted images files are available in '/Users/anestisb/Desktop/tmp/crosshatch-pd1a.180720.030'
```


## Changelog

* __0.2.0__ - TBC
  * Print size when traversing partition table entries
  * Various code cleanups
* __0.1.0__ - 29 December 2019
  * Initial release


## License

```
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
```
