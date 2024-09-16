#include "paintscene.h"

paintscene::paintscene(QObject *parent)
    : QGraphicsScene(parent), isPenModeActive(false), isEraserModeActive(false),
    isPipetteModeActive(false), isFillingModeActive(false), isPainting(false) {}

paintscene::~paintscene() {}

void paintscene::setPenMode(bool enabled)
{
    isPenModeActive = enabled;
}

void paintscene::setEraserMode(bool enabled)
{
    isEraserModeActive = enabled;
}

void paintscene::setColor(const QColor &color)
{
    this->color = color;
}

void paintscene::setSizePx(const int &size)
{
    sizePx = size;
}
void paintscene::setPipetteMode(bool enabled)
{
    isPipetteModeActive = enabled;
}

void paintscene::setFillingMode(bool enabled)
{
    isFillingModeActive = enabled;
}

void paintscene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
    {
        return;
    }
    if (isPenModeActive)
    {
        if (this->sceneRect().contains(event->scenePos()))
        {
            isPainting = true;
            emit isPaintingNow(isPainting);
            QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(event->scenePos().x() - (sizePx / 2),
                                                                     event->scenePos().y() - (sizePx / 2),
                                                                     sizePx,
                                                                     sizePx);
            ellipse->setBrush(QBrush(QColor(color)));
            ellipse->setPen(Qt::NoPen);
            this->addItem(ellipse);
            drawnItems.append(ellipse);
            previousPoint = event->scenePos();
        }
    }
    else if (isEraserModeActive)
    {
        eraseItemsAt(event->scenePos(), sizePx);
    }
    else if (isPipetteModeActive || isFillingModeActive)
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
                if (isPipetteModeActive)
                {
                    QColor color = image.pixelColor(x, y);
                    emit colorPicked(color);
                }
                else
                {
                    floodFill(event->scenePos());
                }
            }
        }
    }
}


void paintscene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (isPenModeActive && isPainting)
    {
        if (this->sceneRect().contains(event->scenePos()))
        {
            QPen pen(QColor(color), sizePx, Qt::SolidLine, Qt::RoundCap);
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

void paintscene::floodFill(const QPointF &pos)
{
    QGraphicsPixmapItem *pixmapItem = nullptr;
    foreach (QGraphicsItem *item, items(pos))
    {
        pixmapItem = dynamic_cast<QGraphicsPixmapItem*>(item);
        if (pixmapItem)
            break;
    }

    if (!pixmapItem) return;

    QPixmap pixmap = pixmapItem->pixmap();
    QImage image = pixmap.toImage();

    QPoint imagePos(static_cast<int>(pixmapItem->mapFromScene(pos).x()),
                    static_cast<int>(pixmapItem->mapFromScene(pos).y()));

    QColor clickedColor = image.pixelColor(imagePos);

    if (clickedColor == color) return;

    QStack<QPoint> stack;
    stack.push(imagePos);

    while (!stack.isEmpty()) {
        QPoint point = stack.pop();
        int x = point.x();
        int y = point.y();

        if (x < 0 || x >= image.width() || y < 0 || y >= image.height()) continue;

        QColor currentColor = image.pixelColor(x, y);

        if (currentColor != clickedColor) continue;

        image.setPixelColor(x, y, color);

        stack.push(QPoint(x + 1, y));
        stack.push(QPoint(x - 1, y));
        stack.push(QPoint(x, y + 1));
        stack.push(QPoint(x, y - 1));
    }
    pixmapItem->setPixmap(QPixmap::fromImage(image));
    this->update();
}




