#include "customgraphicsview.h"
#include <QDebug>
#include <QScrollBar>
#include <QApplication>

CustomGraphicsView::CustomGraphicsView(QWidget *parent)
    : QGraphicsView(parent), isDragging(false){}

void CustomGraphicsView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    if (!isCursorModeActive) return;

    if (event->button() == Qt::LeftButton)
    {
        isDragging = true;
        startPos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
}

void CustomGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
    if (!isCursorModeActive || !isDragging) return;

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
    if (!isCursorModeActive) return;

    if (event->button() == Qt::LeftButton && isDragging)
    {
        isDragging = false;
        setCursor(Qt::OpenHandCursor);
    }
}

void CustomGraphicsView::setCursorMode(bool enabled)
{
    isCursorModeActive = enabled;
}

