#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "paintscene.h"
#include "customgraphicsview.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new paintscene(this);
    ui->graphicsView->setScene(scene);

    ui->graphicsView->setCursorMode(true);
    ui->graphicsView->setCursor(Qt::OpenHandCursor);

    connect(ui->b_selectImage, &QPushButton::clicked, this, &MainWindow::imageSelection);
    connect(ui->b_downSkaling, &QPushButton::clicked, this, &MainWindow::scaleImage);
    connect(ui->b_scalingUp, &QPushButton::clicked, this, &MainWindow::scaleImage);
    connect(ui->b_saveImage, &QPushButton::clicked, this, &MainWindow::saveImage);
    connect(ui->comboBox, &QComboBox::currentTextChanged, this, &MainWindow::onComboBoxTextChanged);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::imageSelection()
{
    QString imagePath = QFileDialog::getOpenFileName(this, "Select file", "");

    if (imagePath.isEmpty() || !(imagePath.endsWith(".jpeg", Qt::CaseInsensitive) || imagePath.endsWith(".png", Qt::CaseInsensitive)))
    {
        QMessageBox::warning(this, "Invalid data", "The editor supports only JPEG and PNG image formats");
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
    //scene->clear();
    //scene->addPixmap(pix);
    if (imageItem)
    {
        scene->removeItem(imageItem);  // Видаляємо попереднє зображення
        delete imageItem;              // Очищаємо пам'ять
    }

    imageItem = scene->addPixmap(originalPix);  // Додаємо нове зображення
    imageScaling(factor);
}

void MainWindow::scaleImage()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    int percentScale = ui->l_percentScale->text().remove('%').toInt();

    if (button->text() == "+" && percentScale < 200)
    {
        percentScale += 5;
        factor += 0.05;
    }
    else if (button->text() == "-" && percentScale > 0 && factor > 0.2)
    {
        percentScale -= 5;
        factor -= 0.05;
    }
    ui->l_percentScale->setText(QString::number(percentScale) + "%");
    imageScaling(factor);
}

void MainWindow::imageScaling(double scaleFactor)
{
    if (!originalPix.isNull())
    {
        QPixmap scaledPixmap = originalPix.scaled(originalPix.size() * scaleFactor, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        scene->clear();
        scene->addPixmap(scaledPixmap);
        scene->setSceneRect(0, 0, scaledPixmap.width(), scaledPixmap.height());
    }
}

void MainWindow::on_cb_scrolling_stateChanged(int arg1)
{
    if (arg1 == 0)
    {
        ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
    else
    {
        ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    }
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    int percentScale = ui->l_percentScale->text().remove('%').toInt();
    if (event->angleDelta().y() > 0 && percentScale < 200)
    {
        percentScale += 5;
        factor += 0.05;
    }
    else if (event->angleDelta().y() < 0 && percentScale > 0 && factor > 0.2)
    {
        percentScale -= 5;
        factor -= 0.05;
    }
    ui->l_percentScale->setText(QString::number(percentScale) + "%");
    imageScaling(factor);
}

void MainWindow::saveImage()
{
    if (!scene) return;

    QRectF sceneRect = scene->sceneRect();
    QSizeF sceneSize = sceneRect.size().toSize();

    QPixmap pixmap(sceneSize.toSize());
    pixmap.fill(Qt::white);

    QPainter painter(&pixmap);
    scene->render(&painter, QRectF(), sceneRect);

    QString fileName = QFileDialog::getSaveFileName(this, "Save Image", "", "Image Files (*.png *.jpg *.jpeg)");

    if (!fileName.isEmpty())
    {
        pixmap.save(fileName);
    }
}


void MainWindow::onComboBoxTextChanged(const QString &text)
{
    if (text == "Pen")
    {
        scene->setPenMode(true);
        ui->graphicsView->setCursor(Qt::PointingHandCursor);
    }
    else
    {
        scene->setPenMode(false);
    }
    if (text == "Cursor")
    {
        ui->graphicsView->setCursorMode(true);
        ui->graphicsView->setCursor(Qt::OpenHandCursor);
    }
    else
    {
        ui->graphicsView->setCursorMode(false);
    }
}

