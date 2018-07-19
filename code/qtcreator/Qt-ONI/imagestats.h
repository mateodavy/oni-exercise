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
        CvPoint cursor;
        Mat     values;
        float   value(int row, int col) { return values.at<float>(row, col); }
        float   peakValue()             { int kc = (KERNEL_SIZE - 1) / 2; return values.at<float>(kc, kc); }
        float   meanValue()             { return values.at<float>(KERNEL_SIZE, KERNEL_SIZE); }
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

    void        setThresholdLevel();
    void        setNoiseLevel();
//    float       getNoiseLevel()                 { return _noiseLevel; }
    float       getNoiseSigma()                 { return _noiseSigma; }
//    void        setThreshold(int x, int y);
    bool        isValidPos(int x, int y);
    bool        isFreePos(int x, int y);
    CvRect      getRectXY(int x, int y);

//    float       getSNRdB(float value)           { float noise = getNoiseLevel(); return (noise > 0.0f) ? DB_20_LOG(value / noise) : 0.0f; }
    float       getSNRdB(float value)           { float sigma = getNoiseSigma(); return (sigma > 0.0f) ? DB_20_LOG(value / sigma) : 0.0f; }

protected:
    Mat         norm2(const Mat& image);
    void        dumpDetail();
    void        dumpResult();
    void        dumpValues();

private:
        string  _label;
const   Mat&    _image;
        Histo   _histo;
        Stats   _stats;
        Detail  _detail;
        Result  _result;
//        float   _noiseLevel;
        float   _noiseSigma;
        Sample  _scanThresholdLevel;
};

#endif // IMAGESTATS_H
