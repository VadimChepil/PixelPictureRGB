#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QPushButton>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QScrollBar>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void imageSelection();
    void displayImage(const QString &path);
    void scaleImage();
    void imageScaling(double scaleFactor);
    void on_cb_scrolling_stateChanged(int arg1);
    void resizeEvent(QResizeEvent *event);

private:
    Ui::MainWindow *ui;
    QPixmap originalPix;
    double factor;
    bool isDragging;
    QPoint startPos;
    QPoint scrollStartPos;
    QPoint scrollCurrentPos;
};
#endif // MAINWINDOW_H
