/***************************************************************
 *  @author: b51 <b51live@gmail.com>
 *  @file  : main.cc
 *  Licensed under The MIT License [see LICENSE for details]
 ***************************************************************/

#include <glog/logging.h>
#include <stdlib.h>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

DEFINE_string(yuv, "../pics/lena.yuv", "yuv image to show");
DEFINE_int32(w, 512, "yuv image width");
DEFINE_int32(h, 512, "yuv image height");
DEFINE_bool(is_nv12, false, "Is image is nv12?");

static inline float clamp(float val, float mn, float mx) {
    return (val >= mn) ? ((val <= mx) ? val : mx) : mn;
}

static void YUVToRGB(const unsigned char y, const unsigned char u,
                     const unsigned char v, unsigned char* r, unsigned char* g,
                     unsigned char* b) {
    const int y1 = (int) y - 16;
    const int u1 = (int) u - 128;
    const int v1 = (int) v - 128;

    *r = clamp(1.164f * y1 + 2.018f * v1, 0.0f, 255.0f);
    *g = clamp(1.164f * y1 - 0.813f * u1 - 0.391f * v1, 0.0f, 255.0f);
    *b = clamp(1.164f * y1 + 1.596f * u1, 0.0f, 255.0f);
}

static void NV12ToRGB(unsigned char* yuv, int w, int h) {
    int num_pixels = w * h;
    unsigned char* bgr = (unsigned char*) calloc(num_pixels * 3, sizeof(unsigned char));
    memset(bgr, 0, num_pixels * 3 * sizeof(unsigned char));

    // for nv12, [w*h]Y, [w*h/2]U, [w*h/2]V
    int u_slide = h * w;
    int yuyv_stride = w;
    int bgr_stride = w * 3;

    unsigned char y00, y01, y10, y11, u, v;
    unsigned char r, g, b;
    for (int j = 0; j < h; j+=2) {
        for (int i = 0, ii = 0; i < w; i+=2, ii+=6) {
            int y_index = j * yuyv_stride + i;
            int uv_index = u_slide + j / 2 * w + i;
            y00 = yuv[y_index + 0];
            y01 = yuv[y_index + 1];
            y10 = yuv[y_index + yuyv_stride + 0];
            y11 = yuv[y_index + yuyv_stride + 1];
            u   = yuv[uv_index + 0];
            v   = yuv[uv_index + 1];

            int bgr_index = j * bgr_stride + ii;
            // bgr00
            YUVToRGB(y00, u, v, &r, &g, &b);
            bgr[bgr_index + 0] = b;
            bgr[bgr_index + 1] = g;
            bgr[bgr_index + 2] = r;

            // bgr01
            YUVToRGB(y01, u, v, &r, &g, &b);
            bgr[bgr_index + 3] = b;
            bgr[bgr_index + 4] = g;
            bgr[bgr_index + 5] = r;

            // bgr10
            YUVToRGB(y10, u, v, &r, &g, &b);
            bgr[bgr_index + bgr_stride + 0] = b;
            bgr[bgr_index + bgr_stride + 1] = g;
            bgr[bgr_index + bgr_stride + 2] = r;

            // bgr11
            YUVToRGB(y11, u, v, &r, &g, &b);
            bgr[bgr_index + bgr_stride + 3] = b;
            bgr[bgr_index + bgr_stride + 4] = g;
            bgr[bgr_index + bgr_stride + 5] = r;
        }
    }

    cv::Mat img(h, w, CV_8UC3, bgr);
    cv::imshow("img", img);
    cv::waitKey();
    delete[] bgr;
}

static void YUYVToRGB(unsigned char* yuv, int w, int h) {
    int num_pixels = w * h;
    unsigned char* bgr = (unsigned char*)calloc(num_pixels * 3, sizeof(unsigned char));
    memset(bgr, 0, num_pixels * 3 * sizeof(char));

    int i, j;
    unsigned char y0, y1, u, v;
    unsigned char r, g, b;
    for (i = 0, j = 0; i < (num_pixels << 1); i += 4, j += 6) {
        y0 = yuv[i + 0];
        u =  yuv[i + 1];
        y1 = yuv[i + 2];
        v =  yuv[i + 3];

        YUVToRGB(y0, u, v, &r, &g, &b);
        bgr[j + 0] = b;
        bgr[j + 1] = g;
        bgr[j + 2] = r;

        YUVToRGB(y1, u, v, &r, &g, &b);
        bgr[j + 3] = b;
        bgr[j + 4] = g;
        bgr[j + 5] = r;
    }
    cv::Mat img(h, w, CV_8UC3, bgr);
    cv::imshow("img", img);
    cv::waitKey();
    delete[] bgr;
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

        LOG(INFO) << "Reading " << length << " characters... ";
        // read data as a block:
        is.read(buffer, length);

        if (is)
            LOG(INFO) << "all characters read successfully.";
        else
            LOG(ERROR) << "error: only " << is.gcount() << " could be read";
        is.close();

        LOG(INFO) << FLAGS_is_nv12;
        if (FLAGS_is_nv12)
            NV12ToRGB((unsigned char*)buffer, FLAGS_w, FLAGS_h);
        else
            YUYVToRGB((unsigned char*)buffer, FLAGS_w, FLAGS_h);

        delete[] buffer;
    }
}
