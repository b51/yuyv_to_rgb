### Usage

```bash
mkdir build
cd build && cmake ..
make -j
./test_yuyv -yuv ../pics/lena.yuv -w 512 -h 512  
```

### Generate yuyv/uyvy picture

```bash
$ ffmpeg -pix_fmts # get all supported pixel formats
$ ffmpeg -i lena.png -s 512x512 -pix_fmt yuyv422 lena.yuv
```
