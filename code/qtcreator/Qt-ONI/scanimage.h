#ifndef SCANIMAGE_H
#define SCANIMAGE_H

#include <iostream>

#include <QString>
#include <QDebug>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>

using namespace cv;
using namespace std;

#include "config.h"
#include "types.h"

#include "imagestats.h"


namespace oni
{

class ScanImage
{
public:
        ScanImage();

        typedef enum
        {
            IMAGE_TYPE_NONE = 0,
            IMAGE_TYPE_FILE = 1,
            IMAGE_TYPE_LAST = 2,
            IMAGE_TYPE_PROC = 3,
            IMAGE_TYPE_NORM = 4,
            IMAGE_TYPE_GRAY = 5,
            IMAGE_TYPE_CMAP = 6,
            IMAGE_TYPE_TEMP = 7,
            IMAGE_TYPE_TEST = 8,
//            IMAGE_TYPE_HOLD = 10,
//            IMAGE_TYPE_BLUR = 9,
            IMAGE_TYPE_MAX
        } ImageType;

        void        build();
        void        reset();

        void        blur();
        void        erode();
        void        threshold();
        void        setThreshold(float value);

        bool        load(QString path);

        Mat&        getImage(ImageType type = IMAGE_TYPE_PROC);
        string      getImageTypeString(int type);
        ImageStats& getStats()      { return _imageStat; }

        void        dump();
        void        dump(Mat& image);
        void        dump(const string& prefix);
        void        dump(const string& prefix, Mat& image);

protected:
        Mat         getTestImage();

        void        buildTestImage();
        void        rebuildImages();
        void        backupImage();

private:
        // use array for images
        QString         _imagePath;
        ImageStats      _imageStat;
        Mat             _images[IMAGE_TYPE_MAX];
};

}

#endif // SCANIMAGE_H
