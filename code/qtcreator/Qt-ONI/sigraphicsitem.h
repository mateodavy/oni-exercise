#ifndef SIGRAPHICSITEM_H
#define SIGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QObject>

#include <QImage>

#include <QDropEvent>
#include <QMimeData>

#include <QGraphicsSceneMouseEvent>

#include <scanimage.h>

//class SIGraphicsItem : public QObject, public QGraphicsItem
class SIGraphicsItem : public QGraphicsItem
{
public:
    explicit SIGraphicsItem(oni::ScanImage& data);
            ~SIGraphicsItem();

protected:

    void    dragEnterEvent(QGraphicsSceneDragDropEvent *e);
    void    dropEvent(QGraphicsSceneDragDropEvent *e);

    QRectF  boundingRect() const;

    void    mousePressEvent(QGraphicsSceneMouseEvent* event);
    void    mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

    void    mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void    hoverMoveEvent(QGraphicsSceneMouseEvent* event);
    void    hoverEnterEvent(QGraphicsSceneMouseEvent* event);
    void    hoverLeaveEvent(QGraphicsSceneMouseEvent* event);

    void    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    bool    validImage(const Mat& image);
    bool    validCursor(const QRectF& cursor, int rows, int cols);

    void    drawHisto(QPainter *painter);
    void    drawCursor(QPainter *painter);
    void    drawResult(QPainter *painter);

private:
    oni::ScanImage&     _data;
    QRectF              _cursor;
};

#endif // SIGRAPHICSITEM_H
