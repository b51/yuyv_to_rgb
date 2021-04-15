### Usage

```bash
$ mkdir build
$ cd build && cmake ..
$ make -j
$ ./yuv_to_rgb -yuv ../pics/lena.yuv -w 512 -h 512 # for yuyv
$ ./yuv_to_rgb -yuv ../pics/suzie_qcif_176x144_nv12.yuv -w 176 -h 144 -is_nv12 # for nv12
```

### Generate yuyv/uyvy picture

```bash
$ ffmpeg -pix_fmts # get all supported pixel formats
$ ffmpeg -i lena.png -s 512x512 -pix_fmt yuyv422 lena.yuv
```
