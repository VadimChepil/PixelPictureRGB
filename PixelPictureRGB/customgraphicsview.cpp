#include "customgraphicsview.h"
#include <QDebug>
#include <QScrollBar>
#include <QApplication>

CustomGraphicsView::CustomGraphicsView(QWidget *parent)
    : QGraphicsView(parent), isDragging(false) {}

void CustomGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isDragging = true;
        startPos = event->pos();
    }
}

void CustomGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (isDragging)
    {
        QPoint currentPos = event->pos();

        QPoint delta = currentPos - startPos;

        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());

        startPos = currentPos;
        QApplication::processEvents();
    }
}

void CustomGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isDragging)
    {
        isDragging = false;
    }
}


