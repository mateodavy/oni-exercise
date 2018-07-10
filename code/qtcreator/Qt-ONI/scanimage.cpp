#include "scanimage.h"

#include <string.h>


using namespace oni;

ScanImage::ScanImage() :
    _imageStat(_images[IMAGE_TYPE_NORM], "normalized")
{
    buildTestImage();
    _images[IMAGE_TYPE_PROC] = _images[IMAGE_TYPE_TEST].clone();
    _images[IMAGE_TYPE_LAST] = _images[IMAGE_TYPE_TEST].clone();
    rebuildImages();
}

void ScanImage::build()
{
    rebuildImages();
}

void ScanImage::reset()
{
    Mat& image =  _images[IMAGE_TYPE_TEST];
    if (_images[IMAGE_TYPE_FILE].data != nullptr)
    {
        image = _images[IMAGE_TYPE_FILE];
    }
    _images[IMAGE_TYPE_PROC] = image.clone();
    _images[IMAGE_TYPE_LAST] = image.clone();
    rebuildImages();
}

void ScanImage::blur()
{
    // set images
    backupImage();
    Mat& source = _images[IMAGE_TYPE_LAST];
    Mat& target = _images[IMAGE_TYPE_PROC];

    // blur
    int blurSize = 3;
//    int blurSize = KERNEL_SIZE;
    Size blurKernelSize(blurSize, blurSize);

    cv::blur(source, target, blurKernelSize, Point(-1,-1));
//    cv::GaussianBlur(source, target, blurKernelSize, 0, 0);
//    cv::medianBlur(source, target, KERNEL_SIZE);
//    cv::bilateralFilter(source, target, blurSize, blurSize * 2, blurSize / 2 );

    // done
    rebuildImages();
}

void ScanImage::erode()
{
    // set images
    backupImage();
    Mat& source = _images[IMAGE_TYPE_LAST];
    Mat& target = _images[IMAGE_TYPE_PROC];

    // erode
    int erosionSize = 1;
//    int erosion_size = KERNEL_SIZE;
    int erosionType = MORPH_RECT;
//    int erosion_type = MORPH_CROSS;
//    int erosion_type = MORPH_ELLIPSE;


    Mat element = getStructuringElement( erosionType,
                                         Size(2 * erosionSize + 1, 2 * erosionSize + 1),
                                         Point(erosionSize, erosionSize));

    cv::erode(source, target, element);

    // done
    rebuildImages();
}

void ScanImage::threshold()
{
    // set images
    backupImage();
    Mat& source = _images[IMAGE_TYPE_LAST];
    Mat& target = _images[IMAGE_TYPE_PROC];

    // threshold
    int thresholdType = 0;
    int thresholdValue = 30;
    int thresholdMaxValue = 255;

    /*
       0: Binary
       1: Binary Inverted
       2: Threshold Truncated
       3: Threshold to Zero
       4: Threshold to Zero Inverted
     */

    cv::threshold(source, target, thresholdValue, thresholdMaxValue, thresholdType);

    // done
    rebuildImages();
}

bool ScanImage::load(QString path)
{
    // load image
    std::string filePath(path.toStdString());
    Mat imageFile = cv::imread(filePath, CV_LOAD_IMAGE_COLOR);

    // check if OK
    if (imageFile.data == nullptr)
    {
        qDebug() << "error loading file: " << QString::fromStdString(filePath);
        return false;
    }

    // adjust channels: BGR->RGB
    cv::cvtColor(imageFile, _images[IMAGE_TYPE_FILE], cv::COLOR_BGR2RGB);
    _images[IMAGE_TYPE_PROC] = _images[IMAGE_TYPE_FILE].clone();
    _images[IMAGE_TYPE_LAST] = _images[IMAGE_TYPE_FILE].clone();

    // rebuild
    rebuildImages();

    // done
    return true;
}

Mat& ScanImage::getImage(ImageType type)
{
    // add bound checking...
    return _images[type];
}

Mat ScanImage::getTestImage()
{
#define TEST_VERSION_2

#ifdef TEST_VERSION_1
    Mat image(256, 512, CV_8UC3);

    for (int y = 0; y < image.rows; y++)
    {
        for (int x = 0; x < image.cols; x++)
        {
            Vec3b pixel;
#ifdef BLUE_ONLY
            pixel[0] = 0x00;   // r
            pixel[1] = 0x00;   // g
            pixel[2] = 0xff;   // b
#else
            pixel[0] =        (x / 2);     // r
            pixel[1] =        (y / 1);     // g
            pixel[2] = 0xff - (y / 1);     // b
//            pixel[2] =        (y / 1);     // b - test for min 0 (ugly!) - confirmed
#endif
            image.at<Vec3b>(y, x) = pixel;
        }
    }
#endif

#ifdef TEST_VERSION_2
    Mat image(1024, 1024, CV_8UC3);

    for (int y = 0; y < image.rows; y++)
    {
        for (int x = 0; x < image.cols; x++)
        {
            Vec3b pixel;
            uchar r = 0x00;
            uchar g = 0x00;
            uchar b = 0x00;
            if (x < 512)
            {
                if (y < 512)
                {
//                    r = ((1024 - x - y) / 2);
                    r = ((1024 - x - y) / 2);
                    g = r;
                    b = g;
                }
                else            // (y >= 512)
                {
                    if (y < 768)
                    {
                        r = (x >= 256) ? ((((x - 256) % 2) == 1) ? 0xff : 0x00) : 0x00;
                        g = r;
                        b = g;
                    }
                    else        // (y >= 768)
                    {
                        r = (x < 256) ? (((y % 2) == 1) ? 0xff : 0x00) : 0xff;
                        g = r;
                        b = g;
                    }
                }
            }
            else                // (x >= 512)
            {
                if (y < 512)
                {
                    r =       ((x - 512) / 2);
                    g =        (y / 2);
                    b = 0xff - (y / 2);
                }
                else            // (y >= 512)
                {
                    if (x < 768)
                    {
                        r = (y < 768) ? 0xff : 0x00;
                        g = (y < 768) ? 0x00 : 0xff;
                        b = (y < 768) ? 0x00 : 0x00;
                    }
                    else        // (x >= 768)
                    {
                        r = (y < 768) ? 0x00 : 0x80;
                        g = (y < 768) ? 0x00 : 0x80;
                        b = (y < 768) ? 0xff : 0x80;
                    }
                }
            }
            pixel[0] = r;     // r
            pixel[1] = g;     // g
            pixel[2] = b;     // b
            image.at<Vec3b>(y, x) = pixel;
        }
    }
#endif
    return image;
}

void ScanImage::buildTestImage()
{
    const Mat& testImage = getTestImage();
    _images[IMAGE_TYPE_TEST] = testImage.clone();
}

void ScanImage::rebuildImages()
{
    // set blur image - nor used
//    _images[IMAGE_TYPE_BLUR] = _images[IMAGE_TYPE_FILE].clone();

    // test - override with test image
//    _images[IMAGE_TYPE_FILE] = getTestImage();
//    dump("file", _images[IMAGE_TYPE_FILE]);

#if 0
    cv::Mat abgr(bgra.size(), bgra.type());
    int from_to[] = { 0,3, 1,1, 2,2, 3,0 };
    cv::mixChannels(&bgra,1,&abgr,1,from_to,4);
#endif

    _images[IMAGE_TYPE_NORM] = _imageStat.norm(_images[IMAGE_TYPE_PROC]).clone();
    _imageStat.stat();
//    _imageStat.dump();

    _images[IMAGE_TYPE_GRAY] = _imageStat.gray(_images[IMAGE_TYPE_NORM]).clone();
    _images[IMAGE_TYPE_TEMP] = _images[IMAGE_TYPE_GRAY].clone();
    applyColorMap(_images[IMAGE_TYPE_TEMP], _images[IMAGE_TYPE_CMAP], COLORMAP_RAINBOW);

//    dump("cmap", _imageCmap);
}

void ScanImage::backupImage()
{
    // copy current: HOLD -> TEMP
    _images[IMAGE_TYPE_TEMP] = _images[IMAGE_TYPE_PROC];

    // copy current: PROC -> HOLD
    _images[IMAGE_TYPE_LAST] = _images[IMAGE_TYPE_PROC];
}


string ScanImage::getImageTypeString(int type)
{
    switch (type)
    {
        case CV_8UC1:       return "CV_8UC1";
        case CV_8UC2:       return "CV_8UC2";
        case CV_8UC3:       return "CV_8UC3";
        case CV_8UC4:       return "CV_8UC4";
        case CV_32FC1:      return "CV_32FC1";
        case CV_64FC1:      return "CV_64FC1";
        default:
            break;
    }

    string s = to_string(type);
//    return "CV_??";
    return s;
}

void ScanImage::dump()
{
//    fprintf(stdout, "image:\t[%s]\n", _imagePath.toStdString().c_str());
    fprintf(stdout, "image[%s]:\n", _imagePath.toStdString().c_str());
    Mat& image = getImage();
    dump(image);
}

void ScanImage::dump(Mat& image)
{
#if 0
    fprintf(stdout, "\tw: %d\n", image.cols);
    fprintf(stdout, "\th: %d\n", image.rows);
    fprintf(stdout, "\tt: %d\n", image.type());
    fprintf(stdout, "\td: %d\n", image.channels());
    fprintf(stdout, "\tf: %x\n", image.flags);
#else
    string imageTypeString = getImageTypeString(image.type());
    fprintf(stdout, "\t%d x %d x %d %s\n", image.cols, image.rows, image.channels(), imageTypeString.c_str());
#ifdef WAIT
    if (image.channels() == 1)
    {
        CvPoint minP;
        CvPoint maxP;
        double minV;
        double maxV;
//        cvMinMaxLoc(image.data, &minV, &maxV, &minP, &minP);
        fprintf(stderr, "\tmin  : %f @ (%d, %d)\n", minV, minP.x, minP.y);
        fprintf(stderr, "\tmax  : %f @ (%d, %d)\n", maxV, maxP.x, maxP.y);
    }
#endif
#endif
}

void ScanImage::dump(const string& prefix)
{
    fprintf(stdout, "image[%s]:", prefix.c_str());
    Mat& image = getImage();
    dump(image);
}

void ScanImage::dump(const string& prefix, Mat& image)
{
    fprintf(stdout, "image[%s]:", prefix.c_str());
    dump(image);
}
