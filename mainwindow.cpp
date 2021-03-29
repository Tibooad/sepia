#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <QFileDialog>
#include <QImageReader>
#include <QMessageBox>
#include <iostream>
#include <QPainter>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->picture->setScaledContents(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "C:/Users/Max/Desktop/7BkiWW8qhP0.jpg", tr("Image Files (*.png *.jpg *.bmp *.webp)"));
    /*QImageReader reader(fileName);
    QImage image;
    if (image.isNull()) {
            QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                     tr("Cannot load %1: %2")
                                     .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
            return;
    }*/
    QPixmap pm(fileName);
    this->ui->picture->setPixmap(pm);

}

QImage MainWindow::grayscaled(const QImage & image)
{
    // TODO: compare speed of both methods
    if (1 == 1)
    {
        if (!image.isNull())
        {
            QImage gray(image.size(), QImage::Format_ARGB32);
            gray.fill(Qt::transparent);
            static QVector<QRgb> monoTable;
            if (monoTable.isEmpty())
            {
                for (int i = 0; i <= 255; i++)
                    monoTable.append(qRgb(i, i, i));
            }
            QPainter p(&gray);
            p.drawImage(0, 0, image.convertToFormat(QImage::Format_Indexed8, monoTable));
            p.end();
            return gray;
        }
        return image;
    }
    else
    {
        QImage img = image;
        if (!image.isNull())
        {
            int pixels = img.width() * img.height();
            if (pixels*(int)sizeof(QRgb) <= img.byteCount())
            {
                QRgb *data = (QRgb *)img.bits();
                for (int i = 0; i < pixels; i++)
                {
                    int val = qGray(data[i]);
                    data[i] = qRgba(val, val, val, qAlpha(data[i]));
                }
            }
        }
        return img;
    }
}

QImage MainWindow::colorized(const QImage & image, QColor color){
    if (!image.isNull())
    {
        QImage resultImage(image.size(), QImage::Format_ARGB32_Premultiplied);
        resultImage.fill(Qt::transparent);
        QPainter painter(&resultImage);
        painter.drawImage(0, 0, grayscaled(image));
        //painter.setCompositionMode(QPainter::CompositionMode_Screen);
        painter.setCompositionMode(QPainter::CompositionMode_ColorDodge);
        //painter.setCompositionMode(QPainter::CompositionMode_Clear);

        painter.fillRect(resultImage.rect(), color);
        painter.end();
        resultImage.setAlphaChannel(image.alphaChannel());
        return resultImage;
    }
    return QImage();
}

void MainWindow::on_pushButton_2_clicked()
{

    QPixmap pm=*this->ui->picture->pixmap();
    QImage image=pm.toImage();
    image=colorized(image,QColor(172,122,51));

    pm=QPixmap::fromImage(image);
    this->ui->picture->setPixmap(pm);
}
