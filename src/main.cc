/***************************************************************
 *  @author: b51 <b51live@gmail.com>
 *  @file  : main.cc
 *  Licensed under The MIT License [see LICENSE for details]
 ***************************************************************/

#include <iostream>
#include <stdlib.h>
#include <glog/logging.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

DEFINE_string(yuv, "../pics/lena.yuv", "yuv image to show");
DEFINE_int32(w, 512, "yuv image width");
DEFINE_int32(h, 512, "yuv image height");

const unsigned char uchar_clipping_table[] = {
    0,   0,   0,   0,   0,   0,   0,
    0,  // -128 - -121
    0,   0,   0,   0,   0,   0,   0,
    0,  // -120 - -113
    0,   0,   0,   0,   0,   0,   0,
    0,  // -112 - -105
    0,   0,   0,   0,   0,   0,   0,
    0,  // -104 -  -97
    0,   0,   0,   0,   0,   0,   0,
    0,  //  -96 -  -89
    0,   0,   0,   0,   0,   0,   0,
    0,  //  -88 -  -81
    0,   0,   0,   0,   0,   0,   0,
    0,  //  -80 -  -73
    0,   0,   0,   0,   0,   0,   0,
    0,  //  -72 -  -65
    0,   0,   0,   0,   0,   0,   0,
    0,  //  -64 -  -57
    0,   0,   0,   0,   0,   0,   0,
    0,  //  -56 -  -49
    0,   0,   0,   0,   0,   0,   0,
    0,  //  -48 -  -41
    0,   0,   0,   0,   0,   0,   0,
    0,  //  -40 -  -33
    0,   0,   0,   0,   0,   0,   0,
    0,  //  -32 -  -25
    0,   0,   0,   0,   0,   0,   0,
    0,  //  -24 -  -17
    0,   0,   0,   0,   0,   0,   0,
    0,  //  -16 -   -9
    0,   0,   0,   0,   0,   0,   0,
    0,  //   -8 -   -1
    0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,
    15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,
    30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,
    45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
    60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,
    75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,
    90,  91,  92,  93,  94,  95,  96,  97,  98,  99,  100, 101, 102, 103, 104,
    105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
    120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134,
    135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
    150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164,
    165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
    180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194,
    195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
    210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224,
    225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254,
    255, 255, 255, 255, 255, 255, 255, 255, 255,  // 256-263
    255, 255, 255, 255, 255, 255, 255, 255,       // 264-271
    255, 255, 255, 255, 255, 255, 255, 255,       // 272-279
    255, 255, 255, 255, 255, 255, 255, 255,       // 280-287
    255, 255, 255, 255, 255, 255, 255, 255,       // 288-295
    255, 255, 255, 255, 255, 255, 255, 255,       // 296-303
    255, 255, 255, 255, 255, 255, 255, 255,       // 304-311
    255, 255, 255, 255, 255, 255, 255, 255,       // 312-319
    255, 255, 255, 255, 255, 255, 255, 255,       // 320-327
    255, 255, 255, 255, 255, 255, 255, 255,       // 328-335
    255, 255, 255, 255, 255, 255, 255, 255,       // 336-343
    255, 255, 255, 255, 255, 255, 255, 255,       // 344-351
    255, 255, 255, 255, 255, 255, 255, 255,       // 352-359
    255, 255, 255, 255, 255, 255, 255, 255,       // 360-367
    255, 255, 255, 255, 255, 255, 255, 255,       // 368-375
    255, 255, 255, 255, 255, 255, 255, 255,       // 376-383
};
constexpr int clipping_table_offset = 128;

/** Clip a value to the range 0<val<255. For speed this is done using an
 * array, so can only cope with numbers in the range -128<val<383.
 */
static unsigned char CLIPVALUE(int val)
{
  return uchar_clipping_table[val + clipping_table_offset];
}

static void YUV2RGB(const unsigned char y, const unsigned char u,
                    const unsigned char v, unsigned char* r, unsigned char* g,
                    unsigned char* b) {
    const int y2 = (int)y;
    const int u2 = (int)u - 128;
    const int v2 = (int)v - 128;

    int r2 = y2 + ((v2 * 37221) >> 15);
    int g2 = y2 - (((u2 * 12975) + (v2 * 18949)) >> 15);
    int b2 = y2 + ((u2 * 66883) >> 15);

    *r = CLIPVALUE(r2);
    *g = CLIPVALUE(g2);
    *b = CLIPVALUE(b2);
}

static void YUYV2RGB(unsigned char* yuv, int w, int h) {
    int i, j;
    unsigned char y0, y1, u, v;
    unsigned char r, g, b;
    int num_pixels = w * h;

    unsigned char* rgb = (unsigned char*)calloc(num_pixels * 3, sizeof(unsigned char));
    memset(rgb, 0, num_pixels * 3 * sizeof(char));

    for (i = 0, j = 0; i < (num_pixels << 1); i += 4, j += 6) {
        y0 = yuv[i + 0];
        u =  yuv[i + 1];
        y1 = yuv[i + 2];
        v =  yuv[i + 3];
        YUV2RGB(y0, u, v, &r, &g, &b);
        rgb[j + 0] = b;
        rgb[j + 1] = g;
        rgb[j + 2] = r;
        YUV2RGB(y1, u, v, &r, &g, &b);
        rgb[j + 3] = b;
        rgb[j + 4] = g;
        rgb[j + 5] = r;
    }
    cv::Mat img(h, w, CV_8UC3, rgb);
    cv::imshow("img", img);
    cv::waitKey();
}

int main(int argc, char* argv[]) {
  FLAGS_minloglevel = google::INFO;
  FLAGS_logtostderr = true;
  FLAGS_colorlogtostderr = true;

  google::InitGoogleLogging(argv[0]);
  google::ParseCommandLineFlags(&argc, &argv, true);

  std::ifstream is(FLAGS_yuv, std::ifstream::binary);
  if (is) {
      // get length of file:
      is.seekg(0, is.end);
      int length = is.tellg();
      is.seekg(0, is.beg);

      char* buffer = new char[length];

      std::cout << "Reading " << length << " characters... ";
      // read data as a block:
      is.read(buffer, length);

      if (is)
          std::cout << "all characters read successfully.";
      else
          std::cout << "error: only " << is.gcount() << " could be read";
      is.close();

      YUYV2RGB((unsigned char*)buffer, FLAGS_w, FLAGS_h);

      delete[] buffer;
  }
}
