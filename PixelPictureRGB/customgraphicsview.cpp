#include "customgraphicsview.h"
#include <QDebug>
#include <QScrollBar>
#include <QApplication>

CustomGraphicsView::CustomGraphicsView(QWidget *parent)
    : QGraphicsView(parent), isDragging(false), isPaintingActive(false), scaleFactor(1.0)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
}

void CustomGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (!isCursorModeActive || event->button() != Qt::LeftButton)
    {
        QGraphicsView::mousePressEvent(event);
        return;
    }
    isDragging = true;
    startPos = event->pos();
    setCursor(Qt::ClosedHandCursor);
}

void CustomGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (!isCursorModeActive || !isDragging)
    {
        QGraphicsView::mouseMoveEvent(event);
        return;
    }

    if (isDragging)
    {
        QPoint currentPos = event->pos();
        QPoint delta = currentPos - startPos;

        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());

        startPos = currentPos;
    }
}

void CustomGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (!isCursorModeActive)
    {
        QGraphicsView::mouseReleaseEvent(event);
        return;
    }

    if (event->button() == Qt::LeftButton && isDragging)
    {
        isDragging = false;
        setCursor(Qt::OpenHandCursor);
    }
}

void CustomGraphicsView::wheelEvent(QWheelEvent *event)
{
    if (isPaintingActive) return;

    if (event->angleDelta().y() > 0 && scaleFactor < 2.0)
    {
        scaleFactor += 0.05;
    }
    else if (event->angleDelta().y() < 0 && scaleFactor > 0.2)
    {
        scaleFactor -= 0.05;
    }
    resetTransform();
    scale(scaleFactor, scaleFactor);

    int percentScale = static_cast<int>(scaleFactor * 100);
    emit scaleChanged(percentScale);
}

void CustomGraphicsView::setCursorMode(bool enabled)
{
    isCursorModeActive = enabled;
}

void CustomGraphicsView::onPaintingStateChanged(bool isPainting)
{
    isPaintingActive = isPainting;
}

