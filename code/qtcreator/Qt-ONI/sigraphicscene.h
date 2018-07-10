#ifndef SIGRAPHICSCENE_H
#define SIGRAPHICSCENE_H

#include <QDebug>

#include <QGraphicsItem>
#include <QGraphicsScene>

#include <QGraphicsSceneMouseEvent>

#include "sigraphicsitem.h"


class SIGraphicScene : public QGraphicsScene
{
public:
    SIGraphicScene();

protected:

    void    mousePressEvent(QGraphicsSceneMouseEvent* event);
    void    mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

    void    mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void    hoverMoveEvent(QGraphicsSceneMouseEvent* event);

};

#endif // SIGRAPHICSCENE_H
