#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QPixmap>
#include <QMessageBox>
#include <QPushButton>

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

private slots:
    void imageSelection();
    void displayImage(const QString &path);
    void scaleImage();
    void imageScaling(double scaleFactor);

private:
    Ui::MainWindow *ui;
    QPixmap originalPix;
    double factor;
};
#endif // MAINWINDOW_H
