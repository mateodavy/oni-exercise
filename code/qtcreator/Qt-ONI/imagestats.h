#ifndef IMAGESTATS_H
#define IMAGESTATS_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

#include "config.h"
#include "types.h"



class ImageStats
{
public:
    ImageStats(const Mat& image, const string label);

    typedef SampleVector Histo;

    struct Stats
    {
        Stats() : min(1.0), max(0.0)    { }
        Sample  min;
        Sample  max;
    };

    struct Detail
    {
        CvPoint mouse;
        Mat     values;
//        float   value()                 { return values.at<float>(KERNEL_SIZE, KERNEL_SIZE); }
        float   value()                 { int kc = (KERNEL_SIZE - 1) / 2; return values.at<float>(kc, kc); }
    };

    Histo       getHisto()              { return _histo; }
    Stats       getStats()              { return _stats; }

    Result&     getResult()             { return _result; }
    Detail      getDetail(int x, int y);

    Mat         norm(const Mat& image);
    Mat         gray(const Mat& image);

    void        scan();
    void        stat();
    void        dump();
    void        clear();

    void        setThreshold();
//    void        setThreshold(int x, int y);
    bool        isValidPos(int x, int y);
    bool        isFreePos(int x, int y);
    CvRect      getRectXY(int x, int y);

protected:
    Mat         norm2(const Mat& image);
    void        dumpDetail();
    void        dumpResult();

private:
        string  _label;
const   Mat&    _image;
        Histo   _histo;
        Stats   _stats;
        Detail  _detail;
        Result  _result;
        Sample  _scanThreshold;
};

#endif // IMAGESTATS_H
