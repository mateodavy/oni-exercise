#include "sigraphicscene.h"

#include "mainwindow.h"


SIGraphicScene::SIGraphicScene()
{

}

void SIGraphicScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);
//    const QPointF& pos = event->pos();
//    qDebug() << "SIGraphicScene: mouse press  : " << pos.x() << ", " << pos.y();

//    QGraphicsScene::mousePressEvent(event);
}

void SIGraphicScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);
//    const QPointF& pos = event->pos();
//    qDebug() << "SIGraphicScene: mouse release: " << pos.x() << ", " << pos.y();

//    QGraphicsScene::mouseReleaseEvent(event);
}

void SIGraphicScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
//    const QPointF& pos = event->pos();
    const QPointF& pos1 = event->scenePos();
//    const QPointF& pos2 = event->screenPos();

//    qDebug() << "SIGraphicScene: mouse move: scene : " << pos1.x() << ", " << pos1.y();
//    qDebug() << "SIGraphicScene: mouse move: screen: " << pos2.x() << ", " << pos2.y();

    // update cursor in main window
    MainWindow* main = MainWindow::getInstance();
    main->setCursorPos(pos1.x(), pos1.y());

#if 0
    // get children items
    QList<QGraphicsItem *> itemList = items();
//    qDebug() << "SIGraphicScene: mouse move: childs: " << childs.size();
    for (auto item : itemList)
    {

//        SIGraphicsItem* sigi = reinterpret_cast<SIGraphicsItem*>(item);
//        sigi->mouseMove(event);
    }
#endif

    update();
}

void SIGraphicScene::hoverMoveEvent(QGraphicsSceneMouseEvent* event)
{
    Q_UNUSED(event);
//    const QPointF& pos = event->pos();
//    qDebug() << "SIGraphicScene: mouse hover: " << pos.x() << ", " << pos.y();

}
