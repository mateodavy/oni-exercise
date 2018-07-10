#include "sigraphicsitem.h"

#include "mainwindow.h"

//SIGraphicsItem::SIGraphicsItem(oni::ScanImage& data) : QObject(), QGraphicsItem(), _data(data)
SIGraphicsItem::SIGraphicsItem(oni::ScanImage& data) : QGraphicsItem(), _data(data)
{
    setAcceptDrops(true);
    setAcceptHoverEvents(true);
    setAcceptTouchEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

SIGraphicsItem::~SIGraphicsItem()
{

}

void SIGraphicsItem::dragEnterEvent(QGraphicsSceneDragDropEvent *e)
{
    if (e->mimeData()->hasUrls())
    {
        e->acceptProposedAction();
    }
}

void SIGraphicsItem::dropEvent(QGraphicsSceneDragDropEvent *e)
{
    foreach (const QUrl &url, e->mimeData()->urls())
    {
        QString filePath = url.toLocalFile();
//        emit droppedFile(filePath);
        MainWindow* main = MainWindow::getInstance();
        main->loadFile(filePath);
        break;  // only first... no way to check here
    }
}

void SIGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    const QPointF& pos = event->pos();
    qDebug() << "SIGraphicsItem: mouse press: " << pos.x() << ", " << pos.y();

}

void SIGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    const QPointF& pos = event->pos();
    qDebug() << "SIGraphicsItem: mouse release: " << pos.x() << ", " << pos.y();

}

void SIGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    const QPointF& pos = event->pos();
    qDebug() << "SIGraphicsItem: mouse move: " << pos.x() << ", " << pos.y();

}

void SIGraphicsItem::hoverEnterEvent(QGraphicsSceneMouseEvent* event)
{
    const QPointF& pos = event->pos();
    qDebug() << "SIGraphicsItem: mouse hover enter: " << pos.x() << ", " << pos.y();

}

void SIGraphicsItem::hoverLeaveEvent(QGraphicsSceneMouseEvent* event)
{
    const QPointF& pos = event->pos();
    qDebug() << "SIGraphicsItem: mouse hover leave: " << pos.x() << ", " << pos.y();

}

void SIGraphicsItem::hoverMoveEvent(QGraphicsSceneMouseEvent* event)
{
    const QPointF& pos = event->pos();
    qDebug() << "SIGraphicsItem: mouse hover move: " << pos.x() << ", " << pos.y();

}

#if 0
void SIGraphicsItem::mouseMove(QGraphicsSceneMouseEvent* event)
{
//    const QPointF& pos = event->pos();
    const QPointF& pos = event->scenePos();

    qDebug() << "SIGraphicsItem: MY mouse move: " << pos.x() << ", " << pos.y();

#if 1
    int ks = KERNEL_SIZE;
//    QPointF rectPos(-ks / 2.0f, -ks / 2.0f);
    QPointF rectPos(pos.x() - ks / 2.0f, pos.y() - ks / 2.0f);
    QSizeF rectSize(ks, ks);
//    QRectF mouseRect(pos + rectPos, rectSize);
//    QRectF mouseRect(rectPos, rectSize);

//    _cursor = mouseRect;
    _cursor.setTopLeft(rectPos);
    _cursor.setSize(rectSize);
//    _cursor.setTopLeft(rectPos);
//    _cursor.setSize(rectSize);
    _cursorP = rectPos;
    _cursorS = rectSize;

    _cursorP.setX(pos.x() - ks / 2.0f);
    _cursorP.setY(pos.y() - ks / 2.0f);
    _cursorS.setWidth(ks);
    _cursorS.setHeight(ks);
#ifdef DEBUG
    qDebug() << "SIGraphicsItem: MY CURSOR  : " << _cursor << endl;
    qDebug() << "SIGraphicsItem: MY CURSOR P: " << _cursorP << endl;
    qDebug() << "SIGraphicsItem: MY CURSOR S: " << _cursorS << endl;
#endif
#endif

}
#endif

QRectF SIGraphicsItem::boundingRect() const
{
    int bw = UI_GRAPHICS_W;
    int bh = UI_GRAPHICS_H;

    return QRectF(0, 0, bw, bh); // We are limiting the area of triangle
}

void SIGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // get main window and associated data
    MainWindow* main = MainWindow::getInstance();
//    oni::ScanImage::ImageType imageType = main->getViewImageType();
//    oni::ScanImage::ImageType imageType = oni::ScanImage::IMAGE_TYPE_CMAP;
//    qDebug() << "SIGraphicsItem::paint: I: " << imageType << endl;
    bool resultViewEnabled = main->isViewOptionEnabled(MainWindow::UI_VIEW_OPTION_RESULT);
    bool cursorViewEnabled = main->isViewOptionEnabled(MainWindow::UI_VIEW_OPTION_CURSOR);
    bool histogramViewEnabled = main->isViewOptionEnabled(MainWindow::UI_VIEW_OPTION_HISTOGRAM);
//    qDebug() << "SIGraphicsItem::paint: C: " << cursorViewEnabled << endl;
//    qDebug() << "SIGraphicsItem::paint: H: " << histogramViewEnabled << endl;

    // set brush
    QPen rectPen(Qt::white);
    rectPen.setWidthF(1.0 / UI_SCALE);
    rectPen.setJoinStyle(Qt::MiterJoin);
    painter->setPen(rectPen);
//    painter->setPen(Qt::white);
//    painter->setBrush(Qt::white);
    painter->setBrush(Qt::transparent);

#ifdef DRAW_TEXT
    // draw title
//    QPointF textPos(0.0, 20.0);
    QPointF textPos(0.0, 30.0);
//    QString textVal(_data.getName());
    QString textVal("Hello!");
    painter->drawText(textPos, textVal);
#endif

    // transform
//    painter->scale(UI_SCALE_W, -UI_SCALE_H);

    // draw image
    cv::Mat& siImage = main->getImage();
    if (!validImage(siImage))
        return;
    int bpp = 3; // bytes per pixel (assume)
//    qDebug() << "painting image: " << siImage.cols << "x" << siImage.rows << endl;
    QImage qtImage(siImage.data, siImage.cols, siImage.rows, siImage.cols * bpp, QImage::Format_RGB888);

    float ioX = UI_IMAGE_MARGIN_W;
    float ioY = UI_IMAGE_MARGIN_H;
    QPointF imageOrigin(ioX, ioY);
    painter->drawImage(imageOrigin, qtImage);

    // adjust cursor - workaround for now
    _cursor = main->getCursor();

    // draw histo
    bool histoOK = validCursor(_cursor, siImage.rows, siImage.cols);
//    qDebug() << "drawing histo: OK: " << histoOK << endl;
    if (histoOK && histogramViewEnabled)
    {
        drawHisto(painter);
    }

    // draw cursor
    bool cursorOK = validCursor(_cursor, siImage.rows, siImage.cols);
//    qDebug() << "drawing cursor: OK: " << cursorOK << endl;
    if (cursorOK && cursorViewEnabled)
    {
        drawCursor(painter);
    }

    // draw result
    bool resultOK = true;
//    qDebug() << "drawing result: OK: " << resultOK << endl;
    if (resultOK && resultViewEnabled)
    {
        drawResult(painter);
    }
}

bool SIGraphicsItem::validImage(const Mat& image)
{
    return ((image.rows != 0) && (image.cols != 0));
}

bool SIGraphicsItem::validCursor(const QRectF& cursor, int rows, int cols)
{
    float ioX = UI_IMAGE_MARGIN_W;
    float ioY = UI_IMAGE_MARGIN_H;

    bool cursorOK = ((ioX < cursor.left())
                    && (cursor.right()  < (cols + ioX))
                    && (ioY < cursor.top())
                    && (cursor.bottom() < (rows + ioY)));

    return cursorOK;
}

void SIGraphicsItem::drawHisto(QPainter *painter)
{
    const ImageStats::Histo& histo = _data.getStats().getHisto();

//    qDebug() << "drawing histo: " << histo.size() << endl;

    // cursor pos - relative to image
    const QRectF& cursor = _cursor;
    QPointF center = cursor.center();
    int ix = int(center.x() - UI_IMAGE_MARGIN_W);
    int iy = int(center.y() - UI_IMAGE_MARGIN_H);

    // get image detail
    ImageStats::Detail detail = _data.getStats().getDetail(ix, iy);
//    qDebug() << "SIGraphicsItem::drawHisto: mouse: " << ix << " - " << iy;
//    qDebug() << "SIGraphicsItem::drawHisto: value: " << detail.value;

//    float histOffsetX = 344.0 + ix + UI_HISTO_OFFSET_X;
//    float histOffsetY = 860.0 + iy + UI_HISTO_OFFSET_Y;
    float histOffsetX = ix + UI_HISTO_OFFSET_X;
    float histOffsetY = iy + UI_HISTO_OFFSET_Y;

    int hX = histOffsetX;
    int hY = histOffsetY;
    int hW = 600.0;
    int hH = 300.0;
    QRectF rectH(hX, hY, hW, hH);
    QPen rectHPen(Qt::white);
    QPen rectLPen(Qt::lightGray);
    QBrush rectHBrush(Qt::darkGray);

    //    rectHPen.setWidthF(1.0 / UI_SCALE);
    float histScaleX =   1.0;
    float histScaleY = 255.0;
    float dataOffsetX = 44.0;
    float dataOffsetY =-22.0;
    // set transform??

    rectHPen.setJoinStyle(Qt::MiterJoin);
    painter->setBrush(rectHBrush);
    painter->drawRect(rectH);

    int pos = 0;
    int valuePos = int(2.0 * histScaleY * detail.value());
//    fprintf(stdout, "SIGraphicsItem::drawHisto: value: %f: pos: %d\n", detail.value(), valuePos);
    for (auto item : histo)
    {
        painter->setPen(rectHPen);
        float pX =    (pos  * histScaleX);
        float pY =  - (item * histScaleY);
        //        float pY = 300.0 - (item * histScaleY);
        QPointF point(dataOffsetX + histOffsetX + pX, dataOffsetY + hH + histOffsetY + pY);
        painter->drawPoint(point);
        if (valuePos == pos)
        {
            painter->setPen(rectHPen);
            int x1 = dataOffsetX + histOffsetX + pX;
            int y1 = dataOffsetY + histOffsetY + hH;
            int x2 = dataOffsetX + histOffsetX + pX;
            int y2 = dataOffsetY + histOffsetY + hH + pY;
            painter->drawLine(x1, y1, x2, y2);

            painter->setPen(rectLPen);
            y1 = dataOffsetY + histOffsetY + hH + pY;
            y2 = dataOffsetY + histOffsetY + hH - histScaleY;
            painter->drawLine(x1, y1, x2, y2);
        }
        pos++;
    }

    // draw current value line

    #if 0
    // draw rectangles
    for (RectangleSet::const_iterator iter = _item.cbegin(); iter < _item.cend(); iter++)
    {
        QRectF rect(*iter);
        painter->drawRect(rect);
    }
    #endif
}

void SIGraphicsItem::drawCursor(QPainter *painter)
{
    // get cursor position
    const QRectF& cursor = _cursor; // arg?

    // cursor pos - relative to image
    QPointF center = cursor.center();
    int ix = int(center.x() - UI_IMAGE_MARGIN_W);
    int iy = int(center.y() - UI_IMAGE_MARGIN_H);

    // get image detail
    ImageStats::Detail detail = _data.getStats().getDetail(ix, iy);
//    qDebug() << "SIGraphicsItem::drawCursor: mouse: " << ix << " - " << iy;
//    qDebug() << "SIGraphicsItem::drawCursor: value: " << detail.value;

    // setup graphics
//    QBrush rectCBrush(Qt::darkGray);
//    QColor cursorColor(int(detail.color[0]), int(detail.color[1]), int(detail.color[2]));
    float value = detail.value();
//    QColor cursorColor(value, value, value);
    QColor cursorColor;
    cursorColor.setRgbF(value, value, value);
//    QColor cursorColor(detail.color, detail.color, detail.color);
//    QColor cursorColor(detail.color[0], detail.color[1], detail.color[2]);
    QBrush rectCBrush(cursorColor);
    painter->setBrush(rectCBrush);
//    painter->fillRect(_cursor);
//    painter->fillRect(_cursor, rectCBrush);

    QPen rectCPen(Qt::white);
    painter->setPen(rectCPen);

    // draw cursor
    painter->drawRect(_cursor);

    // draw lines
    float ks = float(KERNEL_SIZE);
    float cx = _cursor.left() + ks / 2.0;
    float cy = _cursor.top()  + ks / 2.0;

    float zs = float(UI_ZOOM_SCALE);    // zoom scale
    float ls = ks * zs;                 // line size: kernel * zoom

    float mx = ks / 2.0;                // margin x
    float my = ks / 2.0;                // margin y

//    QLineF lines[8];
    QLineF l1;
    l1.setLine(cx + mx, cy, cx + mx + ls / 2.0, cy);
    QLineF l2;
    l2.setLine(cx, cy + my, cx, cy + my + ls / 2.0);
    QLineF l3;
    l3.setLine(cx + mx + ls / 2.0, cy - ls / 2.0, cx + mx + ls / 2.0, cy + ls / 2.0);
    QLineF l4;
    l4.setLine(cx - ls / 2.0, cy + my + ls / 2.0, cx + ls / 2.0, cy + my + ls / 2.0);
    QLineF l5;
    l5.setLine(cx + mx + ls / 2.0, cy - ls / 2.0, cx + mx + ls / 2.0 + mx, cy - ls / 2.0);
    QLineF l6;
    l6.setLine(cx + mx + ls / 2.0, cy + ls / 2.0, cx + mx + ls / 2.0 + mx, cy + ls / 2.0);
    QLineF l7;
    l7.setLine(cx - ls / 2.0, cy + my + ls / 2.0, cx - ls / 2.0, cy + my + ls / 2.0 + my);
    QLineF l8;
    l8.setLine(cx + ls / 2.0, cy + my + ls / 2.0, cx + ls / 2.0, cy + my + ls / 2.0 + my);
//    painter->drawLines(lines, 8);
    painter->drawLine(l1);
    painter->drawLine(l2);
    painter->drawLine(l3);
    painter->drawLine(l4);
    painter->drawLine(l5);
    painter->drawLine(l6);
    painter->drawLine(l7);
    painter->drawLine(l8);

    // draw side histos
    float offsetScale = 3.0;

    // adjust pen
    QPen rectHPen(Qt::lightGray);
    painter->setPen(rectHPen);

    // horizontal (y)
    for (int y = 0; y < (detail.values.rows - 1); y++)
    {
        float value = detail.values.at<float>(y, KERNEL_SIZE);
        float rx = cx - ls / 2.0;
        float ry = cy + offsetScale * my + ls / 2.0;
        float rw = UI_ZOOM_SCALE * KERNEL_SIZE;
        float rh = UI_ZOOM_SCALE;
        ry += y * UI_ZOOM_SCALE;
        QColor pointColor;
        pointColor.setRgbF(value, value, value);
        QBrush pointBrush(pointColor);
        painter->setBrush(pointBrush);
        QRectF pointRect(rx, ry, rw, rh);
        painter->drawRect(pointRect);
    }
    // vertical (x)
    for (int x = 0; x < (detail.values.cols - 1); x++)
    {
        float value = detail.values.at<float>(KERNEL_SIZE, x);
        float rx = cx + offsetScale * mx + ls / 2.0;
        float ry = cy - ls / 2.0;
        float rw = UI_ZOOM_SCALE;
        float rh = UI_ZOOM_SCALE * KERNEL_SIZE;
        rx += x * UI_ZOOM_SCALE;
        QColor pointColor;
        pointColor.setRgbF(value, value, value);
        QBrush pointBrush(pointColor);
        painter->setBrush(pointBrush);
        QRectF pointRect(rx, ry, rw, rh);
        painter->drawRect(pointRect);
    }

    // draw zoom box
    for (int y = 0; y < (detail.values.rows - 1); y++)
    {
        for (int x = 0; x < (detail.values.cols - 1); x++)
        {
            float value = detail.values.at<float>(y, x);
//            qDebug() << "value: " << value;
            float ox = cx + offsetScale * mx + ls / 2.0;
            float oy = cy + offsetScale * my + ls / 2.0;
            ox += x * UI_ZOOM_SCALE;
            oy += y * UI_ZOOM_SCALE;
            QColor pointColor;
            pointColor.setRgbF(value, value, value);
            QBrush pointBrush(pointColor);
            painter->setBrush(pointBrush);
            QRectF pointRect(ox, oy, UI_ZOOM_SCALE, UI_ZOOM_SCALE);
            painter->drawRect(pointRect);
        }
    }
}

void SIGraphicsItem::drawResult(QPainter *painter)
{
    // result list
    Result& result = _data.getStats().getResult();

    QPen resultPen(Qt::red);
    painter->setPen(resultPen);
    QBrush resultBrush(Qt::transparent);
    painter->setBrush(resultBrush);
    for (CvRect item : result)
    {
        float rx = UI_IMAGE_MARGIN_W + item.x;
        float ry = UI_IMAGE_MARGIN_H + item.y;
        float rw = item.width;
        float rh = item.height;
        QRectF resultRect(rx, ry, rw, rh);
        painter->drawRect(resultRect);
    }
}


