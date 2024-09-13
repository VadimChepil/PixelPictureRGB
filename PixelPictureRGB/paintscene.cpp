#include "paintscene.h"

paintscene::paintscene(QObject *parent)
    : QGraphicsScene(parent), isPainting(false){}

paintscene::~paintscene() {}

void paintscene::setPenMode(bool enabled)
{
    isPenModeActive = enabled;
}

void paintscene::setEraserMode(bool enabled)
{
    isEraserModeActive = enabled;
}

void paintscene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (isEraserModeActive && event->button() == Qt::LeftButton)
    {
        eraseItemsAt(event->scenePos(), 20);
        return;
    }

    if (isPenModeActive && event->button() == Qt::LeftButton)
    {
        if (this->sceneRect().contains(event->scenePos()))
        {
            isPainting = true;
            emit isPaintingNow(isPainting);
            QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(event->scenePos().x() - 5,
                                                                     event->scenePos().y() - 5,
                                                                     10,
                                                                     10);
            ellipse->setBrush(QBrush(Qt::red));
            this->addItem(ellipse);
            drawnItems.append(ellipse);
            previousPoint = event->scenePos();
        }
    }
}


void paintscene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (isPenModeActive && isPainting)
    {
        if (this->sceneRect().contains(event->scenePos()))
        {
            QGraphicsLineItem *line = new QGraphicsLineItem(QLineF(previousPoint, event->scenePos()));
            line->setPen(QPen(Qt::red, 10, Qt::SolidLine, Qt::RoundCap));
            this->addItem(line);
            drawnItems.append(line);
            previousPoint = event->scenePos();
        }
    }
}

void paintscene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isPainting)
    {
        isPainting = false;
        emit isPaintingNow(isPainting);
    }
}

void paintscene::eraseItemsAt(const QPointF &position, qreal radius)
{
    QRectF eraseRect(position.x() - radius, position.y() - radius, radius * 2, radius * 2);
    QList<QGraphicsItem *> itemsToErase = this->items(eraseRect);

    for (QGraphicsItem *item : itemsToErase)
    {
        if (drawnItems.contains(item))
        {
            this->removeItem(item);
            drawnItems.removeOne(item);
            delete item;
        }
    }
}


