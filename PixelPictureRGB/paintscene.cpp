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

void paintscene::setPenColor(const QColor &color)
{
    penColor = color;
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
            ellipse->setBrush(QBrush(QColor(penColor)));
            ellipse->setPen(Qt::NoPen);
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
            QPen pen(QColor(penColor), 10, Qt::SolidLine, Qt::RoundCap);
            QGraphicsLineItem *line = new QGraphicsLineItem(QLineF(previousPoint, event->scenePos()));
            line->setPen(QPen(pen));
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


