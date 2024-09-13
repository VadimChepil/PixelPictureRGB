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

private:
    Ui::MainWindow *ui;
    paintscene *scene;
    QPixmap originalPix;
    double factor;


    void imageSelection();
    void displayImage(const QString &path);
    void updateScaleLabel(int percent);
    void scaleImage();
    void imageScaling(double scaleFactor);
    void saveImage();

private slots:
    void on_cb_scrolling_stateChanged(int arg1);
    void onComboBoxTextChanged(const QString &text);
};

#endif // MAINWINDOW_H

