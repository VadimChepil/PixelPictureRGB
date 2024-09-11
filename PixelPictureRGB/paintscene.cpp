#include "paintscene.h"

paintscene::paintscene(QObject *parent)
    : QGraphicsScene(parent), isDragging(false)
{

}

paintscene::~paintscene() {}

void paintscene::setPenMode(bool enabled)
{
    isPenModeActive = enabled;
}

void paintscene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (isPenModeActive && event->button() == Qt::LeftButton)
    {
        if (this->sceneRect().contains(event->scenePos()))
        {
            isDragging = true;

            QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(event->scenePos().x() - 5,
                                                                     event->scenePos().y() - 5,
                                                                     10,
                                                                     10);
            ellipse->setBrush(QBrush(Qt::red));
            this->addItem(ellipse);
            previousPoint = event->scenePos();
        }
    }
}

void paintscene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (isPenModeActive && isDragging)
    {
        if (this->sceneRect().contains(event->scenePos()))
        {
            QGraphicsLineItem *line = new QGraphicsLineItem(QLineF(previousPoint, event->scenePos()));
            line->setPen(QPen(Qt::red, 10, Qt::SolidLine, Qt::RoundCap));
            this->addItem(line);

            previousPoint = event->scenePos();
        }
    }
}

void paintscene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isDragging)
    {
        isDragging = false;
    }
}



