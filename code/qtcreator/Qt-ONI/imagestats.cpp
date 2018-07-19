#include "imagestats.h"

#include <stdio.h>

#include <algorithm>


ImageStats::ImageStats(const Mat& image, const string label) :
    _label(label),
    _image(image),
    _histo(HISTO_SIZE),
//      _noiseLevel(IMAGE_NOISE_LEVEL_DEFAULT)
    _noiseSigma(IMAGE_NOISE_SIGMA_DEFAULT),
    _scanThresholdLevel(SCAN_THRESHOLD_DEFAULT)
{

}

void ImageStats::stat()
{
    // this is one way... also compute min/max/histo
    // should assert input image is 3 channels/uchar
    _stats.min = 1.0;
    _stats.max = 0.0;

    // reset histo
    std::fill(_histo.begin(), _histo.end(), 0.0f);
    float histoScale = 1.0f;
    float maxHistoValue = 0.0;

    // build float image and compute image histo/stats
    for (int y = 0; y < _image.rows; y++)
    {
        for (int x = 0; x < _image.cols; x++)
        {
            float value = _image.at<float>(y, x);
            _stats.min = min(_stats.min, value);
            _stats.max = max(_stats.max, value);
            int histoIndex = int((HISTO_SIZE - 1) * value); // create histo index
            _histo[histoIndex] += histoScale;               // use normalized float
            maxHistoValue = max(_histo[histoIndex], maxHistoValue);
        }
    }

    // normalize histo
    // could apply map
    // or foreach image...
    if (maxHistoValue > 0.0)
    {
        for (int index = 0; index < _histo.size(); index++)
            _histo[index] /= maxHistoValue;
    }
}

Mat ImageStats::norm(const Mat& image)
{
    // build float image and compute image histo/stats
    // note that dymanic range of result is 768, i.e. almost 10 bits
    // might want to call this differently
    Mat result(image.rows, image.cols, CV_32FC1, 0.0);
    float valueScale = 1.0f / (3.0f * 255.0f);
    float lumaMax = 0;

    for (int y = 0; y < image.rows; y++)
    {
        for (int x = 0; x < image.cols; x++)
        {
            Vec3b pixel  = image.at<Vec3b>(y, x);
            float value  = valueScale * float(pixel[0] + pixel[1] + pixel[2]);
//            int luma = int(255.0 * value);
            result.at<float>(y, x) = value;
//            uchar luma = uchar(255.0 * value);
//            lumaMax = max(lumaMax, luma);
            lumaMax = max(lumaMax, value);
        }
    }

#if 1
    // normalize image (optional or separate process?)
//    fprintf(stdout, "lm: %f\n", lumaMax);
    if (lumaMax > 0)
    {
        for (int y = 0; y < image.rows; y++)
        {
            for (int x = 0; x < image.cols; x++)
            {
                float value = result.at<float>(y, x);
                result.at<float>(y, x) = value / lumaMax;
            }
        }
    }
#endif

    // done
    return result;
}

Mat ImageStats::gray(const Mat& image)
{
    // build float image and compute image histo/stats
    Mat result(image.rows, image.cols, CV_8UC3);

    for (int y = 0; y < image.rows; y++)
    {
        for (int x = 0; x < image.cols; x++)
        {
            float value  = image.at<float>(y, x);
            Vec3b pixel;
            pixel[0] = int(255.0 * value);
            pixel[1] = int(255.0 * value);
            pixel[2] = int(255.0 * value);
            result.at<Vec3b>(y, x) = pixel;
        }
    }

    // done
    return result;
}

Mat ImageStats::norm2(const Mat& image)
{
    // this is one way... also compute min/max/histo
    // should assert input image is 3 channels/uchar
    _stats.min = 1.0;
    _stats.max = 0.0;
    float valueScale = 1.0f / (3.0f * 255.0f);

    // reset histo
    std::fill(_histo.begin(), _histo.end(), 0.0f);
//    float histoScale = 1.0f / float(image.rows * image.cols);
//    float histoScale = 1.0f / float(image.cols);
    float histoScale = 1.0f;
    float maxHistoValue = 0.0;

    // build float image and compute image histo/stats
//    Mat result(image.rows, image.cols, CV_32FC1, 0.0);
#define USE_8UC1
//#define USE_8UC3
#ifdef USE_8UC3
    Mat result(image.rows, image.cols, CV_8UC3);
#else
    Mat result(image.rows, image.cols, CV_8UC1);
#endif
    uchar lumaMax = 0;

    for (int y = 0; y < image.rows; y++)
    {
        for (int x = 0; x < image.cols; x++)
        {
            Vec3b pixel  = image.at<Vec3b>(y, x);
            float value  = valueScale * (pixel[0] + pixel[1] + pixel[2]);
//            int luma = int(255.0 * value);
//            result.at<float>(y, x) = value;
            uchar luma = uchar(255.0 * value);
            lumaMax = max(lumaMax, luma);
#ifdef USE_8UC3
            pixel[0] = luma;
            pixel[1] = luma;
            pixel[2] = luma;
            result.at<Vec3b>(y, x) = pixel;
#else
            result.at<uchar>(y, x) = luma;
#endif
            _stats.min = min(_stats.min, value);
            _stats.max = max(_stats.max, value);
            int histoIndex = int((HISTO_SIZE - 1) * value); // create histo index
            _histo[histoIndex] += histoScale;               // use normalized float
            maxHistoValue = max(_histo[histoIndex], maxHistoValue);
        }
    }

    // normalize histo
    if (maxHistoValue > 0.0)
    {
        for (int index = 0; index < _histo.size(); index++)
            _histo[index] /= maxHistoValue;
    }

    // normalize image
//    fprintf(stdout, "min: %f: max: %f: lm: %d\n", _stats.min, _stats.max, lumaMax);
    if (lumaMax > 0)
    {
        for (int y = 0; y < image.rows; y++)
        {
            for (int x = 0; x < image.cols; x++)
            {
                Vec3b pixel  = result.at<Vec3b>(y, x);
                pixel[0] = uchar(255 * pixel[0] / lumaMax);
                pixel[1] = uchar(255 * pixel[1] / lumaMax);
                pixel[2] = uchar(255 * pixel[2] / lumaMax);
                result.at<Vec3b>(y, x) = pixel;
            }
        }
    }

    // done
    return result;
}

void ImageStats::scan()
{
    int limit = SCAN_SOLUTIONS_MAX;
    _result.clear();
    for (int y = 0; y < _image.rows; y++)
    {
        for (int x = 0; x < _image.cols; x++)
        {
            if (isValidPos(x, y))
            {
                Detail detail = getDetail(x, y);
                float value = detail.values.at<float>(KERNEL_SIZE, KERNEL_SIZE);
                if (value > _scanThresholdLevel)
                {
                    // try to add point
                    CvRect rect;
                    rect.x = x;
                    rect.y = y;
                    rect.width  = KERNEL_SIZE;
                    rect.height = KERNEL_SIZE;
                    if (isFreePos(x, y))
                    {
                        // limit # of entries?
                        if (--limit >= 0)
                            _result.push_back(rect);
                    }
                }
            }
            else
            {
                if (false)
                {
                    fprintf(stderr, "scan: invalid pos: %d,%d\n", y, x);
                }
            }
        }
    }
#ifdef DEBUG
    dumpResult();
#endif
}

//#define DUMP_IMAGE_INFO
//#define DUMP_CURSOR_DETAIL
//#define DUMP_CURSOR_RESULT
#define DUMP_CURSOR_VALUES

void ImageStats::dump()
{
#ifdef DUMP_IMAGE_INFO
    // dump
    fprintf(stderr, "ImageStats::dump: [%s]\n", _label.c_str());
    fprintf(stderr, "\tdim  : %dx%d\n", _image.cols, _image.rows);
    fprintf(stderr, "\tmin  : %f\n", _stats.min);
    fprintf(stderr, "\tmax  : %f\n", _stats.max);
    fprintf(stderr, "\thisto: %d\n", _histo.size());
#endif

#ifdef DUMP_CURSOR_DETAIL
    dumpDetail();
#endif

#ifdef DUMP_CURSOR_RESULT
    dumpResult();
#endif

#ifdef DUMP_CURSOR_VALUES
    dumpValues();
#endif
}

void ImageStats::clear()
{
    _result.clear();
}

ImageStats::Detail ImageStats::getDetail(int x, int y)
{
    _detail.cursor.x = x;
    _detail.cursor.y = y;

//    fprintf(stderr, "ImageStats::getDetail: type: %d\n", _image.type());

    // check image
    if (_image.data == nullptr)
    {
        fprintf(stderr, "ImageStats::getDetail: image: %.3dx%.3d\n", _image.cols, _image.rows);
        fprintf(stderr, "ImageStats::getDetail: mouse: %.3d,%.3d\n", x, y);
        return _detail; // invalid!
    }

    int x1 = x - KERNEL_SIZE / 2;
    int y1 = y - KERNEL_SIZE / 2;
//    int x2 = x + KERNEL_SIZE / 2;
//    int y2 = y + KERNEL_SIZE / 2;

    Rect roi(x1, y1, KERNEL_SIZE + 1, KERNEL_SIZE + 1); // add 1 row/col

//    _detail.values = _image(roi);
    _detail.values = _image(roi).clone();
    // copy center value to bottom/right
//    _detail.values.at<float>(KERNEL_SIZE, KERNEL_SIZE) = _image.at<float>(y, x);

    float scale = 1.0f / float(KERNEL_SIZE);

    // many ways to do this...
    // use CvDot (dot product)
    // or Reduce (or Blur box)
    // or ...

    for (int y = 0; y < (_detail.values.rows - 1); y++)
    {
        _detail.values.at<float>(y, KERNEL_SIZE) = 0.0f;
        for (int x = 0; x < (_detail.values.cols - 1); x++)
        {
            _detail.values.at<float>(y, KERNEL_SIZE) += scale * _detail.values.at<float>(y, x);
        }
    }
    for (int x = 0; x < (_detail.values.cols - 1); x++)
    {
        _detail.values.at<float>(KERNEL_SIZE, x) = 0.0f;
        for (int y = 0; y < (_detail.values.rows - 1); y++)
        {
            _detail.values.at<float>(KERNEL_SIZE, x) += scale * _detail.values.at<float>(y, x);
        }
    }

    _detail.values.at<float>(KERNEL_SIZE, KERNEL_SIZE) = 0.0f;
    for (int y = 0; y < (_detail.values.rows - 1); y++)
        _detail.values.at<float>(KERNEL_SIZE, KERNEL_SIZE) += scale * _detail.values.at<float>(y, KERNEL_SIZE);

#ifdef DEBUG
    dumpDetail();
#endif

    return _detail;
}

void ImageStats::setThresholdLevel()
{
    float value = _detail.meanValue();
    qDebug() << "ImageStats::setThresholdLevel: value: " << value;
    _scanThresholdLevel = value;
}

void ImageStats::setNoiseLevel()
{
    // compute standard dev
    float mean = _detail.meanValue();
    float diff = 0.0f;
    for (int y = 0; y < (_detail.values.rows - 1); y++)
    {
        _detail.values.at<float>(y, KERNEL_SIZE) = 0.0f;
        for (int x = 0; x < (_detail.values.cols - 1); x++)
        {
            float value = (_detail.values.at<float>(y, x) - mean);
            diff += value * value;
        }
    }

#define USE_STD_DEV
#ifdef USE_STD_DEV
    int count = KERNEL_SIZE * KERNEL_SIZE;
    _noiseSigma = sqrtf(diff / float(count - 1));   // corrected
//    _noiseSigma = sqrtf(diff / float(count));     // not corrected
#else
    _noiseSigma = mean;
#endif

#ifdef DEBUG
    qDebug() << "ImageStats::setNoiseLevel: diff : " << diff;
    qDebug() << "ImageStats::setNoiseLevel: mean : " << mean;
    qDebug() << "ImageStats::setNoiseLevel: sigma: " << _noiseSigma;
#endif
}

bool ImageStats::isValidPos(int x, int y)
{
    int ks2 = (KERNEL_SIZE - 1) / 2;
    bool validPos = ((ks2 < x) && (x < (_image.cols - ks2 - 1)) && (ks2 < y) && (y < (_image.rows - ks2 - 1)));
    return validPos;
}

bool ImageStats::isFreePos(int x, int y)
{
    // iterate over existing points and check if included...
    for (CvRect item : _result)
    {
        QRect rect(item.x, item.y, item.width, item.height);
//        qDebug() << "ImageStats::isFreePos: " << rect << "point: " << x << ", " << y << "- inside: " << rect.contains(x, y);
        if (rect.contains(x, y))
            return false;
    }

    // none found
    return true;
}

CvRect ImageStats::getRectXY(int x, int y)
{
    int ks1 =  KERNEL_SIZE;
    int ks2 = (KERNEL_SIZE - 1) / 2;
    CvRect result;
    result.x = x - ks2;
    result.y = y - ks2;
    result.width  = ks1;
    result.height = ks1;
    return result;
}

void ImageStats::dumpDetail()
{
    fprintf(stdout, "\tmouse  : %d,%d\n", _detail.cursor.x, _detail.cursor.y);
    fprintf(stdout, "\tdetail : %dx%d\n", _detail.values.cols, _detail.values.rows);

    fprintf(stdout, "      ");
    for (int x = 0; x < _detail.values.cols; x++)
        fprintf(stdout, "[%.3d] ", x);
    fprintf(stdout, "\n");

    for (int y = 0; y < _detail.values.rows; y++)
    {
        fprintf(stdout, "[%.3d] ", y);
        for (int x = 0; x < _detail.values.cols; x++)
        {
            fprintf(stdout, "%.3f ", _detail.values.at<float>(y, x));
        }
        fprintf(stdout, "\n");
    }
}

void ImageStats::dumpResult()
{
    fprintf(stdout, "result: threshold: %.3f -> %d items\n", _scanThresholdLevel, _result.size());
    int count = 0;
    for (CvRect item : _result)
    {
        Detail detail = getDetail(item.x, item.y);
        float value = detail.values.at<float>(KERNEL_SIZE, KERNEL_SIZE);
        fprintf(stdout, "\t[%.3d]: (%.3d, %.3d) -> %.3f\n", count, item.x, item.y, value);
        count++;
    }
}

void ImageStats::dumpValues()
{
    fprintf(stdout, "values: peak: %.3f: mean: %.3f: sigma: %.3f: SNR1: %.2f dB: SNR2: %.2f dB\n",
        _detail.peakValue(),
        _detail.meanValue(),
        _noiseSigma,
        getSNRdB(_detail.peakValue()),
        getSNRdB(_detail.meanValue())
    );
}


