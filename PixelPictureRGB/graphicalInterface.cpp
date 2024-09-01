#include "graphicalInterface.h"
#include "ui_graphicalInterface.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->b_selectImage, &QPushButton::clicked, this, &MainWindow::imageSelection);
    connect(ui->b_downSkaling, &QPushButton::clicked, this, &MainWindow::scaleImage);
    connect(ui->b_scalingUp, &QPushButton::clicked, this, &MainWindow::scaleImage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    int percentScale = ui->l_percentScale->text().remove('%').toInt();
    if (event->angleDelta().y() > 0 && percentScale < 200)
    {
        percentScale += 5;
        factor += 0.1;
    }
    else if (event->angleDelta().y() < 0 && percentScale > 0 && factor > 0.2)
    {
        percentScale -= 5;
        factor -= 0.1;
    }
    ui->l_percentScale->setText(QString::number(percentScale) + "%");
    imageScaling(factor);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isDragging = true;
        startPos = event->globalPosition().toPoint();
        scrollStartPos = ui->scrollArea->widget()->mapToGlobal(ui->scrollArea->widget()->pos());
        qDebug() << "startPos: " << startPos;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (isDragging)
    {
        QPoint currentPos = event->globalPosition().toPoint();
        QPoint delta = currentPos - startPos;

        ui->scrollArea->horizontalScrollBar()->setValue(ui->scrollArea->horizontalScrollBar()->value() - delta.x());
        ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->value() - delta.y());

        startPos = currentPos;
        QApplication::processEvents();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isDragging)
    {
        isDragging = false;
        qDebug() << "Mouse released at position: " << event->globalPosition().toPoint();;
    }
}

void MainWindow::imageSelection()
{
    QString imagePath = QFileDialog::getOpenFileName(this, "Select file", "");

    if (imagePath.isEmpty() || !(imagePath.right(5) == ".jpeg" || imagePath.right(4) == ".png"))
    {
        QMessageBox::warning(this, "Invalid data", "The editor supports only jpeg and png image format");
        return;
    }
    ui->stackedWidget->setCurrentIndex(1);
    displayImage(imagePath);
}

void MainWindow::displayImage(const QString &path)
{
    QPixmap pix;
    if (!pix.load(path))
    {
        QMessageBox::warning(this, "Error", "Failed to load image");
        return;
    }
    originalPix = pix;
    factor = 1.0;
    imageScaling(factor);
}

void MainWindow::scaleImage()
{
    QPushButton *button = (QPushButton *)sender();
    int percentScale = ui->l_percentScale->text().remove('%').toInt();

    if (button->text() == "+" && percentScale < 200)
    {
        percentScale += 5;
        factor += 0.1;
    }
    else if (button->text() == "-" && percentScale > 0 && factor > 0.2)
    {
        percentScale -= 5;
        factor -= 0.1;
    }
    ui->l_percentScale->setText(QString::number(percentScale) + "%");
    imageScaling(factor);
}

void MainWindow::imageScaling(double scaleFactor)
{
    QPixmap scaledPixmap = originalPix.scaled(originalPix.size() * scaleFactor, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->l_image->setPixmap(scaledPixmap);
    ui->l_image->setAlignment(Qt::AlignCenter);
}

void MainWindow::on_cb_scrolling_stateChanged(int arg1)
{
    if (arg1 == 0)
    {
        ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->scrollArea->setWidgetResizable(false);
        imageScaling(factor);
    }
    else
    {
        ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        ui->scrollArea->setWidgetResizable(true);
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    if (ui->scrollArea->verticalScrollBarPolicy() == Qt::ScrollBarAlwaysOff &&
        ui->scrollArea->horizontalScrollBarPolicy() == Qt::ScrollBarAlwaysOff)
    {
        imageScaling(factor);
    }
}






