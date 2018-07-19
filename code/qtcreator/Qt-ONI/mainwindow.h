#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDropEvent>
#include <QMimeData>

#include <QActionGroup>
#include <QGraphicsView>

#include <QString>

#include <QImage>
#include <QDebug>

#include "scanimage.h"

#include "sigraphicscene.h"
#include "sigraphicsitem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
static  MainWindow*     getInstance();

    explicit MainWindow(QWidget *parent = 0);
            ~MainWindow();

    typedef enum
    {
        UI_VIEW_OPTION_NONE         = 0x0000,
        UI_VIEW_OPTION_RESULT       = 0x0001,
        UI_VIEW_OPTION_CURSOR       = 0x0002,
        UI_VIEW_OPTION_VALUES       = 0x0004,
        UI_VIEW_OPTION_HISTOGRAM    = 0x0008
    } UIViewOption;

    bool                        isViewOptionEnabled(UIViewOption option);
    oni::ScanImage::ImageType   getViewImageType();
    Mat&                        getImage();

public Q_SLOTS:

    void    setCursorPos(int x, int y);
    QRectF  getCursor();

    void    optionChanged(bool state);
    bool    loadFile(QString path);
    void    loadFile();
    void    saveFile();
    void    load();
    void    save();
    void    scan();
    void    clear();
    void    exit();

    void    blur();
    void    erode();
    void    reset();
    void    threshold();
    void    setThresholdLevel();
    void    setNoiseLevel();

protected:

    void    dragEnterEvent(QDragEnterEvent *e);
    void    dropEvent(QDropEvent *e);

#if 0
    void    keyReleaseEvent(QKeyEvent* event);
#endif

    void    resizeEvent(QResizeEvent* event);

    void    refreshGV();

    void    setupButtons();
    void    setupTables();
    void    setupMenu();
    void    setupViewOptions();
    void    setupFileTypeActionGroup();
    void    setupGraphicScene();
    void    setupAppStatus();

    bool    saveResultFile(QString& path);

private:
        QActionGroup        _group;
        oni::ScanImage      _image;
        SIGraphicScene      _scene;
        SIGraphicsItem      _gitem;
        QRectF              _cursor;

        Ui::MainWindow*     _ui;
static  MainWindow*         _me;

};

#endif // MAINWINDOW_H
