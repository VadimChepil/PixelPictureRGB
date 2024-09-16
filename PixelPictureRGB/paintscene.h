#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QColor>
#include <QList>
#include <QPointF>
#include <QStack>
#include <QGraphicsPixmapItem>
#include <QSet>

class paintscene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit paintscene(QObject *parent = nullptr);
    ~paintscene();

    void setPenMode(bool enabled);
    void setEraserMode(bool enabled);
    void setColor(const QColor& color);
    void setSizePx(const int& size);
    void setPipetteMode(bool enabled);
    void setFillingMode(bool enabled);

signals:
    void isPaintingNow(bool isPainting);
    void colorPicked(const QColor &color);


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPointF previousPoint;
    bool isPenModeActive;
    bool isEraserModeActive;
    bool isPipetteModeActive;
    bool isFillingModeActive;
    bool isPainting;
    QList<QGraphicsItem *> drawnItems;
    QColor color;
    int sizePx;


    void eraseItemsAt(const QPointF &position, qreal radius);
    void floodFill(const QPointF &pos);
    QGraphicsPixmapItem* getPixmapItemAt(const QPointF &pos);
};

#endif // PAINTSCENE_H
