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

void paintscene::setSizePx(const int &size)
{
    sizePx = size;
}
void paintscene::setPipetteMode(bool enabled)
{
    isPipetteModeActive = enabled;
}

/*void paintscene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (isEraserModeActive && event->button() == Qt::LeftButton)
    {
        eraseItemsAt(event->scenePos(), sizePx);
        return;
    }

    if (isPenModeActive && event->button() == Qt::LeftButton)
    {
        if (this->sceneRect().contains(event->scenePos()))
        {
            isPainting = true;
            emit isPaintingNow(isPainting);
            QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(event->scenePos().x() - (sizePx / 2),
                                                                     event->scenePos().y() - (sizePx / 2),
                                                                     sizePx,
                                                                     sizePx);
            ellipse->setBrush(QBrush(QColor(penColor)));
            ellipse->setPen(Qt::NoPen);
            this->addItem(ellipse);
            drawnItems.append(ellipse);
            previousPoint = event->scenePos();
        }
    }
}*/
void paintscene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (isPenModeActive)
    {
        isPainting = true;
        previousPoint = event->scenePos();
        emit isPaintingNow(isPainting);
    }
    else if (isEraserModeActive)
    {
        eraseItemsAt(event->scenePos(), sizePx);
    }
    else if (isPipetteModeActive)
    {
        QGraphicsPixmapItem *pixmapItem = nullptr;
        foreach (QGraphicsItem *item, items(event->scenePos()))
        {
            pixmapItem = dynamic_cast<QGraphicsPixmapItem*>(item);
            if (pixmapItem)
                break;
        }

        if (pixmapItem)
        {
            QPixmap pixmap = pixmapItem->pixmap();
            QImage image = pixmap.toImage();

            QPointF imagePos = pixmapItem->mapFromScene(event->scenePos());
            int x = static_cast<int>(imagePos.x());
            int y = static_cast<int>(imagePos.y());

            if (x >= 0 && y >= 0 && x < image.width() && y < image.height())
            {
                QColor color = image.pixelColor(x, y);
                emit colorPicked(color);
            }
        }
    }
    QGraphicsScene::mousePressEvent(event);
}


void paintscene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (isPenModeActive && isPainting)
    {
        if (this->sceneRect().contains(event->scenePos()))
        {
            QPen pen(QColor(penColor), sizePx, Qt::SolidLine, Qt::RoundCap);
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


