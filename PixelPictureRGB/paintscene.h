#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsScene>
#include <QPointF>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QMouseEvent>


class paintscene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit paintscene(QObject *parent = nullptr);
    ~paintscene();

    void setPenMode(bool enabled);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPointF previousPoint;
    bool isPenModeActive = false;
    bool isDragging;
};

#endif // PAINTSCENE_H
