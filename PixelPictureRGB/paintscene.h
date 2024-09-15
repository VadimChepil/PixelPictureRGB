#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsScene>
#include <QPointF>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QPainterPath>

class paintscene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit paintscene(QObject *parent = nullptr);
    ~paintscene();

    void setPenMode(bool enabled);
    void setEraserMode(bool enabled);
    void setPenColor(const QColor& color);
    void setSizePx(const int& size);
    void setPipetteMode(bool enabled);

signals:
    void isPaintingNow(bool isPainting);
    void colorPicked(const QColor &color);


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPointF previousPoint;
    bool isPenModeActive = false;
    bool isEraserModeActive = false;
    bool isPipetteModeActive = false;
    bool isPainting;
    QList<QGraphicsItem *> drawnItems;
    QColor penColor;
    int sizePx;


    void eraseItemsAt(const QPointF &position, qreal radius);
};

#endif // PAINTSCENE_H
