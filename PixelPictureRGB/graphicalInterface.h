#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "customgraphicsview.h"

#include <QMainWindow>
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QPushButton>
#include <QMouseEvent>
#include <QScrollBar>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QPixmap originalPix;
    double factor;

    void imageSelection();
    void displayImage(const QString &path);
    void scaleImage();
    void imageScaling(double scaleFactor);


private slots:
    void on_cb_scrolling_stateChanged(int arg1);

protected:
    void wheelEvent(QWheelEvent *event) override;
};

#endif // MAINWINDOW_H
