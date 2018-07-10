#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow* MainWindow::_me = nullptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _group(this),
    _gitem(_image),
    _cursor(0, 0, KERNEL_SIZE, KERNEL_SIZE),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    setAcceptDrops(true);

    setupButtons();
    setupTables();
    setupMenu();
    setupViewOptions();
    setupFileTypeActionGroup();
    setupGraphicScene();
    setupAppStatus();

    setWindowTitle(QString(APP_TITLE_PREFIX) + QString(APP_TITLE_DEFAULT));
//    loadFile("images/Wed_morningSession_nup555_647.1528885469405-2.tif");
}

MainWindow::~MainWindow()
{
    delete _ui;
//  delete _me;
}

MainWindow* MainWindow::getInstance()
{
    if (_me == nullptr)
    {
        _me = new MainWindow();
    }
    return _me;
}

void MainWindow::setCursorPos(int x, int y)
{
    int ox = KERNEL_SIZE / 2;
    int oy = KERNEL_SIZE / 2;
    _cursor.setLeft(x - ox);
    _cursor.setTop( y - oy);
    _cursor.setWidth(KERNEL_SIZE);
    _cursor.setHeight(KERNEL_SIZE);
}

QRectF MainWindow::getCursor()
{
    return _cursor;
}

bool MainWindow::isViewOptionEnabled(UIViewOption option)
{
    switch (option)
    {
        case UI_VIEW_OPTION_RESULT:     return _ui->action_Result->isChecked();
        case UI_VIEW_OPTION_CURSOR:     return _ui->action_Cursor->isChecked();
        case UI_VIEW_OPTION_HISTOGRAM:  return _ui->action_Histogram->isChecked();
        case UI_VIEW_OPTION_NONE:
        default:
            break;
    }
    return false;
}

oni::ScanImage::ImageType MainWindow::getViewImageType()
{
    if (_ui->action_File->isChecked())
        return oni::ScanImage::IMAGE_TYPE_FILE;
    if (_ui->action_Last->isChecked())
        return oni::ScanImage::IMAGE_TYPE_LAST;
    if (_ui->action_Proc->isChecked())
        return oni::ScanImage::IMAGE_TYPE_PROC;
    if (_ui->action_Gray->isChecked())
        return oni::ScanImage::IMAGE_TYPE_GRAY;
    if (_ui->action_Cmap->isChecked())
        return oni::ScanImage::IMAGE_TYPE_CMAP;
    return oni::ScanImage::IMAGE_TYPE_NONE;
}

Mat& MainWindow::getImage()
{
    return _image.getImage(getViewImageType());
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
//    qDebug() << "MainWindow::dragEnterEvent" << e->mimeData()->hasUrls() << endl;

    if (e->mimeData()->hasUrls())
    {
        e->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *e)
{
    foreach (const QUrl &url, e->mimeData()->urls())
    {
        QString fileName = url.toLocalFile();
//        qDebug() << "Dropped file:" << fileName;
        if (loadFile(fileName))
            break;
    }
}

#if 0
void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
//    qDebug() << "MainWindow: key release :" << event->key();

    switch (event->key())
    {
        case Qt::Key_F2:    _viewImageType = oni::ScanImage::IMAGE_TYPE_FILE; qDebug() << "A"; break;

//        QApplication::keyboardModifiers() && Qt::ControlModifier)
//        if ((event->state() & ControlButton))
//        if ((event->modifiers() & Qt::ControlModifier))

        case Qt::Key_G:
        {
//            if (event->modifiers() & Qt::ControlModifier)
            if (true)
            {
            }
            break;
        }

        default:    // ignore
            break;
    }

    refreshGV();
}
#endif

void MainWindow::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
//    qDebug() << "MainWindow: resize:" << event;
}

void MainWindow::setupButtons()
{
    connect(_ui->buttonImageScan, SIGNAL(clicked()), this, SLOT(scan()));
    connect(_ui->buttonListClear, SIGNAL(clicked()), this, SLOT(clear()));
}

void MainWindow::setupTables()
{
    _ui->resultTableWidget->setColumnWidth(0, UI_RESULT_COLUMN_WIDTH);
    _ui->resultTableWidget->setColumnWidth(1, UI_RESULT_COLUMN_WIDTH);
    _ui->resultTableWidget->setColumnWidth(2, UI_RESULT_COLUMN_WIDTH);
    _ui->resultTableWidget->setColumnWidth(3, UI_RESULT_COLUMN_WIDTH);
    _ui->resultTableWidget->setColumnWidth(4, UI_RESULT_COLUMN_WIDTH);
    _ui->resultTableWidget->horizontalHeader()->setStretchLastSection(true);    // make it beatiful and/or set correct table width
}

void MainWindow::setupMenu()
{
    addAction(_ui->action_Load);
    connect(_ui->action_Load, SIGNAL(triggered(bool)), this, SLOT(load()));
    addAction(_ui->action_Save);
    connect(_ui->action_Save, SIGNAL(triggered(bool)), this, SLOT(save()));

    addAction(_ui->action_Scan);
    connect(_ui->action_Scan, SIGNAL(triggered(bool)), this, SLOT(scan()));
    addAction(_ui->action_Clear);
    connect(_ui->action_Clear, SIGNAL(triggered(bool)), this, SLOT(clear()));

    addAction(_ui->action_Blur);
    connect(_ui->action_Blur, SIGNAL(triggered(bool)), this, SLOT(blur()));
    addAction(_ui->action_Erode);
    connect(_ui->action_Erode, SIGNAL(triggered(bool)), this, SLOT(erode()));
    addAction(_ui->action_Threshold);
    connect(_ui->action_Threshold, SIGNAL(triggered(bool)), this, SLOT(threshold()));

    addAction(_ui->action_Reset);
    connect(_ui->action_Reset, SIGNAL(triggered(bool)), this, SLOT(reset()));

    addAction(_ui->action_Set_Threshold);
    connect(_ui->action_Set_Threshold, SIGNAL(triggered(bool)), this, SLOT(setThreshold()));
}

void MainWindow::setupViewOptions()
{
    addAction(_ui->action_File);
    connect(_ui->action_File, SIGNAL(triggered(bool)), this, SLOT(optionChanged(bool)));
    addAction(_ui->action_Last);
    connect(_ui->action_Last, SIGNAL(triggered(bool)), this, SLOT(optionChanged(bool)));
    addAction(_ui->action_Proc);
    connect(_ui->action_Proc, SIGNAL(triggered(bool)), this, SLOT(optionChanged(bool)));
    addAction(_ui->action_Gray);
    connect(_ui->action_Gray, SIGNAL(triggered(bool)), this, SLOT(optionChanged(bool)));
    addAction(_ui->action_Cmap);
    connect(_ui->action_Cmap, SIGNAL(triggered(bool)), this, SLOT(optionChanged(bool)));

    addAction(_ui->action_Result);
    connect(_ui->action_Result, SIGNAL(triggered(bool)), this, SLOT(optionChanged(bool)));
    addAction(_ui->action_Cursor);
    connect(_ui->action_Cursor, SIGNAL(triggered(bool)), this, SLOT(optionChanged(bool)));
    addAction(_ui->action_Histogram);
    connect(_ui->action_Histogram, SIGNAL(triggered(bool)), this, SLOT(optionChanged(bool)));
}

void MainWindow::setupFileTypeActionGroup()
{
    _group.addAction(_ui->action_File);
    _group.addAction(_ui->action_Last);
    _group.addAction(_ui->action_Proc);
    _group.addAction(_ui->action_Gray);
    _group.addAction(_ui->action_Cmap);

    _ui->action_Proc->setChecked(true);
    _ui->action_Cursor->setChecked(true);
    _ui->action_Result->setChecked(true);
}

void MainWindow::setupGraphicScene()
{
    // gprahics setup
    _ui->graphicsView->setScene(&_scene);
    _ui->graphicsView->setRenderHint(QPainter::Antialiasing);
//    _ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    _ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // set scene reference
    _scene.setSceneRect(0, 0, 1200, 1200);
//    _scene->setBackgroundBrush(QBrush(QColor("dark blue")));
    _scene.setBackgroundBrush(QBrush(QColor("dark gray")));

//#define DRAW_LINES
#ifdef DRAW_LINES   // check origin position
    _scene->addLine(-10, 0, 10, 0, QPen(Qt::red)); // Add horizontal line via center
    _scene->addLine(0, -10, 0, 10, QPen(Qt::green)); // Add vertical line via center
#endif
    _scene.addItem(&_gitem);

    // connect drop event
//    connect(&_gitem, SIGNAL(droppedFile(QString)), this, SLOT(loadFile(QString)));

    // adjust view tranform
    QTransform gvTransform;
    gvTransform.translate(-20.0, -20.0);
    _ui->graphicsView->setTransform(gvTransform);
}

void MainWindow::setupAppStatus()
{
    // show status
    char const* status = "Application started...";
    _ui->statusBar->showMessage(status);
}

void MainWindow::refreshGV()
{
    _ui->graphicsView->viewport()->repaint();
}

void MainWindow::optionChanged(bool state)
{
//    qDebug() << "MainWindow::optionChanged: state: " << state;
#if 0
    QObject* object = sender();
    if (object == _ui->action_File)
    {
        QAction* action = qobject_cast<QAction*>(object);
        qDebug() << "MainWindow::optionChanged: file: " << action->isChecked();
    }
#endif
    refreshGV();
}

bool MainWindow::loadFile(QString path)
{
    bool statusOK = _image.load(path);

    if (statusOK)
    {
        setWindowTitle(APP_TITLE_PREFIX + path);
        refreshGV();
//        _image.dump();
    }

    return statusOK;
}

void MainWindow::loadFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Load Image"), "",
            tr("Images (*.tif);;All Files (*)"));
    if (!fileName.isEmpty())
        loadFile(fileName);
}

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save Result"), "",
            tr("Result (*.oni);;All Files (*)"));
    if (fileName.isEmpty())
        return;
    // else save result
}

void MainWindow::load()
{
    loadFile();
}

void MainWindow::save()
{
    saveFile();
}

void MainWindow::scan()
{
    // clean first...
    clear();

    // re-scan
    _image.getStats().scan();
    refreshGV();

    // fill list
    Result& result = _image.getStats().getResult();
    for (CvRect item : result)
    {
        int numRows = _ui->resultTableWidget->rowCount();
        _ui->resultTableWidget->insertRow(numRows);

        ImageStats::Detail detail = _image.getStats().getDetail(item.x, item.y);
        float value = detail.values.at<float>(KERNEL_SIZE, KERNEL_SIZE);

        QTableWidgetItem* xPosItem = new QTableWidgetItem(QString::number(item.x));
        QTableWidgetItem* yPosItem = new QTableWidgetItem(QString::number(item.y));
        QTableWidgetItem* ampItem = new QTableWidgetItem(QString::number(value));

        _ui->resultTableWidget->setItem(numRows, 0, xPosItem);
        _ui->resultTableWidget->setItem(numRows, 1, yPosItem);
        _ui->resultTableWidget->setItem(numRows, 2, ampItem);
    }
}

void MainWindow::clear()
{
    // clear data
    _image.getStats().clear();
    refreshGV();

    // clear list
    int numRows = _ui->resultTableWidget->rowCount();
    for (int count = 0; count < numRows; count++)
        _ui->resultTableWidget->removeRow(0);
}

void MainWindow::exit()
{
    close();
}

void MainWindow::blur()
{
    _image.blur();
    refreshGV();
}

void MainWindow::erode()
{
    _image.erode();
    refreshGV();
}

void MainWindow::reset()
{
    _image.reset();
    refreshGV();
}

void MainWindow::threshold()
{
    // argument?
    _image.threshold();
    refreshGV();
}

void MainWindow::setThreshold()
{
    _image.getStats().setThreshold();
}

