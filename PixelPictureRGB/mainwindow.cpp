#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "paintscene.h"
#include "customgraphicsview.h"

#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new paintscene(this);
    ui->graphicsView->setScene(scene);

    QButtonGroup *toolButtonGroup = new QButtonGroup(this);
    toolButtonGroup->addButton(ui->b_cursor);
    toolButtonGroup->addButton(ui->b_pen);
    toolButtonGroup->addButton(ui->b_eraser);
    toolButtonGroup->addButton(ui->b_pipette);
    toolButtonGroup->addButton(ui->b_filling);

    toolButtonGroup->setExclusive(true);

    ui->graphicsView->setCursorMode(true);
    ui->graphicsView->setCursor(Qt::OpenHandCursor);
    ui->b_cursor->setChecked(true);

    connect(ui->b_selectImage, &QPushButton::clicked, this, &MainWindow::imageSelection);
    connect(ui->b_downSkaling, &QPushButton::clicked, this, &MainWindow::scaleImage);
    connect(ui->b_scalingUp, &QPushButton::clicked, this, &MainWindow::scaleImage);
    connect(ui->b_saveImage, &QPushButton::clicked, this, &MainWindow::saveImage);
    connect(ui->graphicsView, &CustomGraphicsView::scaleChanged, this, &MainWindow::updateScaleLabel);
    connect(scene, &paintscene::isPaintingNow, ui->graphicsView, &CustomGraphicsView::onPaintingStateChanged);
    connect(this, &MainWindow::sendScaleFactor, ui->graphicsView, &CustomGraphicsView::getScaleFactor);
    connect(scene, &paintscene::colorPicked, this, &MainWindow::setPipetteColor);

    connect(ui->b_black, &QPushButton::clicked, this, &MainWindow::on_b_colorButton_clicked);
    connect(ui->b_darkGray, &QPushButton::clicked, this, &MainWindow::on_b_colorButton_clicked);
    connect(ui->b_gray, &QPushButton::clicked, this, &MainWindow::on_b_colorButton_clicked);
    connect(ui->b_white, &QPushButton::clicked, this, &MainWindow::on_b_colorButton_clicked);
    connect(ui->b_darkRed, &QPushButton::clicked, this, &MainWindow::on_b_colorButton_clicked);
    connect(ui->b_red, &QPushButton::clicked, this, &MainWindow::on_b_colorButton_clicked);
    connect(ui->b_orange, &QPushButton::clicked, this, &MainWindow::on_b_colorButton_clicked);
    connect(ui->b_yellow, &QPushButton::clicked, this, &MainWindow::on_b_colorButton_clicked);
    connect(ui->b_darkGreen, &QPushButton::clicked, this, &MainWindow::on_b_colorButton_clicked);
    connect(ui->b_green, &QPushButton::clicked, this, &MainWindow::on_b_colorButton_clicked);
    connect(ui->b_lime, &QPushButton::clicked, this, &MainWindow::on_b_colorButton_clicked);
    connect(ui->b_lightGreen, &QPushButton::clicked, this, &MainWindow::on_b_colorButton_clicked);
    connect(ui->b_darkBlue, &QPushButton::clicked, this, &MainWindow::on_b_colorButton_clicked);
    connect(ui->b_blue, &QPushButton::clicked, this, &MainWindow::on_b_colorButton_clicked);
    connect(ui->b_cyan, &QPushButton::clicked, this, &MainWindow::on_b_colorButton_clicked);
    connect(ui->b_lightBlue, &QPushButton::clicked, this, &MainWindow::on_b_colorButton_clicked);
    connect(ui->b_brown, &QPushButton::clicked, this, &MainWindow::on_b_colorButton_clicked);
    connect(ui->b_purple, &QPushButton::clicked, this, &MainWindow::on_b_colorButton_clicked);
    connect(ui->b_magenta, &QPushButton::clicked, this, &MainWindow::on_b_colorButton_clicked);
    connect(ui->b_pink, &QPushButton::clicked, this, &MainWindow::on_b_colorButton_clicked);

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

    scene->clear();
    scene->addPixmap(pix);
    scene->setSceneRect(0, 0, pix.width(), pix.height());
}

void MainWindow::updateScaleLabel(double percent, double scaleFactor)
{
    ui->l_percentScale->setText(QString::number(percent) + "%");
    imageScaling(scaleFactor);
}

void MainWindow::scaleImage()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    int percentScale = ui->l_percentScale->text().remove('%').toInt();
    double factor = double(percentScale) / 100;
    emit sendScaleFactor(factor);

    if (button->text() == "+" && factor <= 1.95)
    {
        percentScale += 5;
        factor += 0.05;
    }
    else if (button->text() == "-" && factor > 0.2)
    {
        percentScale -= 5;
        factor -= 0.05;
    }
    ui->l_percentScale->setText(QString::number(percentScale) + "%");
    imageScaling(factor);
}


void MainWindow::imageScaling(double scaleFactor)
{
    ui->graphicsView->resetTransform();
    ui->graphicsView->scale(scaleFactor, scaleFactor);
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

void MainWindow::saveImage()
{
    if (!scene) return;

    QRectF sceneRect = scene->sceneRect();
    QSizeF sceneSize = sceneRect.size().toSize();

    QPixmap pixmap(sceneSize.toSize());
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    scene->render(&painter, QRectF(), sceneRect);

    QString fileName = QFileDialog::getSaveFileName(this, "Save Image", "", "Image Files (*.png *.jpg *.jpeg)");

    if (!fileName.isEmpty())
    {
        pixmap.save(fileName);
    }
}

void MainWindow::changeSetCheking(QPushButton *clickedButton)
{
    if (ui->b_cursor != clickedButton)
        ui->b_cursor->setChecked(false);
    if (ui->b_pen != clickedButton)
        ui->b_pen->setChecked(false);
    if (ui->b_eraser != clickedButton)
        ui->b_eraser->setChecked(false);
    if (ui->b_pipette != clickedButton)
        ui->b_pipette->setChecked(false);
    if (ui->b_filling != clickedButton)
        ui->b_filling->setChecked(false);
}

void MainWindow::on_b_chouseAnotherImage_clicked()
{
    scene->clear();
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_hs_changePx_sliderMoved(int position)
{
    ui->le_countPx->setText(QString::number(position));
    scene->setSizePx(position);
}

void MainWindow::on_b_pen_toggled(bool checked)
{
    if (checked)
    {
        changeSetCheking(ui->b_pen);
        scene->setPenMode(true);
        QString colorName = ui->l_showColorNow->styleSheet();
        QColor color = colorName.remove("background-color:").trimmed();
        scene->setPenColor(color);
        ui->graphicsView->setCursor(Qt::PointingHandCursor);
    }
    else
    {
        scene->setPenMode(false);
    }
}

void MainWindow::on_b_cursor_toggled(bool checked)
{
    if (checked)
    {
        changeSetCheking(ui->b_cursor);
        ui->graphicsView->setCursorMode(true);
        ui->graphicsView->setCursor(Qt::OpenHandCursor);
    }
    else
    {
        ui->graphicsView->setCursorMode(false);
    }
}

void MainWindow::on_b_eraser_toggled(bool checked)
{
    if (checked)
    {
        changeSetCheking(ui->b_eraser);
        scene->setEraserMode(true);
        ui->graphicsView->setCursor(Qt::CrossCursor);
    }
    else
    {
        scene->setEraserMode(false);
    }
}

void MainWindow::on_b_colorButton_clicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button)
    {
        QString buttonStyle = button->styleSheet();
        QColor colorName;
        ui->l_showColorNow->setStyleSheet(buttonStyle);
        if (buttonStyle.contains("rgb(139, 0, 0)"))
        {
            colorName = QColor(139, 0, 0);
        }
        else if (buttonStyle.contains("rgb(98, 98, 98)"))
        {
            colorName = QColor(98, 98, 98);
        }
        else
        {
            colorName = buttonStyle.remove("background-color:").trimmed();
        }
        QColor color = colorName;
        scene->setPenColor(color);
    }
}

void MainWindow::on_b_pipette_toggled(bool checked)
{
    if (checked)
    {
        changeSetCheking(ui->b_pipette);
        scene->setPipetteMode(true);
        ui->graphicsView->setCursor(Qt::UpArrowCursor);
    }
    else
    {
        scene->setPipetteMode(false);
    }
}

void MainWindow::setPipetteColor(const QColor &color)
{
    QString colorName = color.name();
    ui->l_showColorNow->setStyleSheet(QString("background-color: %1").arg(colorName));
}

