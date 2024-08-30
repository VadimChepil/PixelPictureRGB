#include "graphicalInterface.h"
#include "ui_graphicalInterface.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->b_selectImage, &QPushButton::clicked, this, &MainWindow::selectFile);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selectFile()
{
    QString imagePath = QFileDialog::getOpenFileName(this, "Select file", "");

    if (imagePath.isEmpty() || !(imagePath.right(5) == ".jpeg" || imagePath.right(4) == ".png"))
    {
        QMessageBox::warning(this, "Invalid data", "The editor supports only jpeg and png image format");
        return;
    }

    ui->stackedWidget->setCurrentIndex(1);
    showImage(imagePath);
}

void MainWindow::showImage(const QString &path)
{
    QPixmap pix;
    if (!pix.load(path))
    {
        QMessageBox::warning(this, "Error", "Failed to load image");
        return;
    }

    ui->l_image->setPixmap(pix);
    ui->l_image->setScaledContents(true);
    int labelWidth = ui->l_image->width();
    double factor = double(labelWidth) / pix.width();
    ui->l_image->setFixedSize(pix.size() * factor);
}







