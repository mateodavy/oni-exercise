#ifndef CONFIG_H
#define CONFIG_H


//
// Application constants
//
#define APP_NAME            "oni"
#define APP_TITLE_PREFIX    "oni: "
#define APP_TITLE_DEFAULT   "<load file from menu or drop a file into window>"

//
// UI Constants
//
#define UI_RESULT_COLUMN_WIDTH  40


//
// Program limits (could be define as const ints as well
//
#define NUM_RECTANGLES_MIN      3
#define NUM_RECTANGLES_MAX      10
#define NUM_RECTANGLES_DEFAULT  8

#define RECTANGLE_SET_COUNT     2
#define RECTANGLE_SET_SOURCE    0
#define RECTANGLE_SET_TARGET    1

#define RECTANGLE_MAX_W         10
#define RECTANGLE_MAX_H         10


//
// Constants (would eventually separate UI in another file)
//
#define UI_GRAPHICS_W   1200
#define UI_GRAPHICS_H   1200

#define UI_RECTANGLE_W  20
#define UI_RECTANGLE_H  20

#define UI_SCALE        40
#define UI_SCALE_W      UI_SCALE
#define UI_SCALE_H      UI_SCALE

#define UI_IMAGE_MARGIN_W   20
#define UI_IMAGE_MARGIN_H   20

#define UI_HISTO_OFFSET_X   -200
#define UI_HISTO_OFFSET_Y    240

#define UI_VALUE_OFFSET_X   30
#define UI_VALUE_OFFSET_Y   15

#define UI_ZOOM_SCALE   16
#define UI_ZOOM_MARGIN  48

//#define HISTO_SIZE      1024
#define HISTO_SIZE      512
//#define HISTO_SIZE      256

#define SCAN_SOLUTIONS_MAX          512
#define SCAN_THRESHOLD_DEFAULT      0.200f

#define IMAGE_NOISE_LEVEL_DEFAULT   0.1f
#define IMAGE_NOISE_SIGMA_DEFAULT   0.1f

//#define DB_20_LOG(v)    (20.0f * logf((v)))
#define DB_20_LOG(v)    (20.0f * log10((v)))

#define KERNEL_SIZE     7

#define IS_MODE_SET(mode, flag) ((mode)  &  (flag))

#define SET_MODE(mode, flag)    (mode) |=  (flag)
#define CLR_MODE(mode, flag)    (mode) &= ~(flag)

#define ALT_MODE(mode, flag)    { IS_MODE_SET(mode, flag) ? CLR_MODE(mode, flag) : SET_MODE(mode, flag); }


#endif // CONFIG_H
