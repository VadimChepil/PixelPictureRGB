#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QPushButton>
#include <QMouseEvent>
#include <QScrollBar>
#include <QGraphicsPixmapItem>
#include <QButtonGroup>

#include "customgraphicsview.h"
#include "paintscene.h"

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

signals:
    void sendScaleFactor(double scaleFactor);

private:
    Ui::MainWindow *ui;
    paintscene *scene;
    QPixmap originalPix;


    void imageSelection();
    void displayImage(const QString &path);
    void scaleImage();
    void imageScaling(double scaleFactor);
    void saveImage();
    void changeSetCheking(QPushButton *clickedButton);

private slots:
    void on_cb_scrolling_stateChanged(int arg1);
    void updateScaleLabel(double percent, double scaleFactor);
    void on_b_chouseAnotherImage_clicked();
    void on_hs_changePx_sliderMoved(int position);
    void on_b_pen_toggled(bool checked);
    void on_b_cursor_toggled(bool checked);
    void on_b_eraser_toggled(bool checked);
    void on_b_colorButton_clicked();
};

#endif // MAINWINDOW_H

