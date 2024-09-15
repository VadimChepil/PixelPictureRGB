#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QWheelEvent>

class CustomGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit CustomGraphicsView(QWidget *parent = nullptr);

    void setCursorMode(bool enabled);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event);

signals:
    void scaleChanged(double percent, double scaleFactor);

public slots:
    void onPaintingStateChanged(bool isPainting);
    void getScaleFactor(double scaleFactor);

private:
    bool isDragging;
    QPoint startPos;
    bool isPaintingModeActive;
    bool isCursorModeActive;
    double scaleFactor;
};

#endif // CUSTOMGRAPHICSVIEW_H
