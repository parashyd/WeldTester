#include "previewscreen.h"
#include "testdetails.h"
#include "DataFile.h"
#include "ui_previewscreen.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QString>
#include <QPdfWriter>
#include <QPainter>
#include <QDateTime> // For unique file names
#include <sched.h>
#include "shared_data.h"
#include "matrix_keypad.h"
#include "testscreen.h"
#include "qcustomplot.h"

#include <QDir>
#include <QDebug>

extern SharedData* shared;

PreviewScreen::PreviewScreen(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PreviewScreen)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    ui->setupUi(this);

    setupPlotAppearance();
    ui->label_OPvalue->setText(OperatorName);
    ui->label_Divvalue->setText(Division);
    ui->label_Secvalue->setText(Section);
    ui->label_LineValue->setText(Line);
    ui->label_Railvalue->setText(Rail);
    ui->label_TestTypeVal->setText(Testtype);
    ui->label_ModeVal->setText(Mode);
    ui->label_Kmval->setText(km);
    ui->label_Mvalue->setText(M);
    ui->label_TpVal->setText(TP);
    ui->label_LocVal->setText(LocOfDefect);
    ui->label_StattionsVal->setText(stations);
    ui->label_RollMarkVal->setText(rollmark);
    ui->label_WeldVal->setText(weld);
    ui->label_ProbeVal->setText(Probe);
    ui->label_ClassVal->setText(classVal);
    ui->label_Fpval->setText(FPNFP);
    ui->label_remarksval->setText(OthersObservation);

    ui->label_Chval->setText(QString::number(entry.channel));
    ui->label_CalsetVal->setText(QString::number(entry.calset));


    // ui->lineEdit_Date->setTexttring(Date);
    // ui->lineEdit_time->setText(Time);

    ui->label_Gainval->setText(QString::number(entry.Gain/GAIN_FACTOR));
    ui->label_RJVal->setText(QString::number(entry.reject/REJECT_FACTOR));
    ui->label_G1stVal->setText(QString::number(entry.g1_start));
    ui->label_G1EDVal->setText(QString::number(entry.g1_end));
    ui->label_TH1Val->setText(QString::number(entry.th1));
    ui->label_G2stval->setText(QString::number(entry.g2_start));
    ui->label_G2EDVal->setText(QString::number(entry.g2_start));
    ui->label_TH2val->setText(QString::number(entry.th2));
    ui->label_angleval->setText(QString::number(entry.Angle));
    ui->label_Delyval->setText(QString::number(entry.delay));
    ui->label_RangeVal->setText(QString::number(entry.range));

    ui->label_G1->setStyleSheet("QLabel { color : #219601; }");
    ui->label_G2->setStyleSheet("QLabel { color : #0818ff; }");
    qDebug() <<"fetched data from previous screen";

    //ui->Plot->setOpenGl(true);
    plotGraphFromFilePS();
}


void PreviewScreen::handleSocketKey(quint8 key)
{

    qDebug() << "Received key of preview (hex):" << QString("0x%1").arg(key, 2, 16, QLatin1Char('0')).toUpper();

    switch (key)
    {
    case SAVE:
        qDebug() << "Save" << key;
        on_pushButton_save_clicked();
        emit requestTestScreen();

        break;

    default:
        break;
    }
}


PreviewScreen::~PreviewScreen()
{
    delete ui;
}

void PreviewScreen::plotGraphFromFilePS()
{
    if (!shared) return;

    QVector<QPointF> filteredPoints;
    if (getFilteredPoints(config, filteredPoints) == 0) return;


    QVector<double> xData, yData;
    maxX = 0;
    maxY = 0;

    for (const QPointF& point : filteredPoints)
    {
        double address = point.x();  // Already normalized address
        double scaledY = (point.y() / 255.0) * 100.0;

        xData.append(address);
        yData.append(scaledY);

        maxX = qMax<int>(maxX, static_cast<int>(address));
        maxY = qMax<double>(maxY, scaledY);

        //qDebug() << "x-axis" << address << "y-axis" << scaledY;
    }

    if (xData.isEmpty()) return;

    // No need to subtract delay * DELAY_FACTOR here, already done
    QVector<double> xDataNormalized;
    for (double addr : xData)
        xDataNormalized.append(addr / RANGE_FACTOR );

    double normG1Start = (g1_start * config.range)/100;
    double normG1End   = (g1_end * config.range)/100;

    double normG2Start = (g2_start * config.range) / 100.0;
    double normG2End   = (g2_end * config.range) / 100.0;

    double ph1Value = 0, ph2Value = 0, phValue=0;
    int peakIndex1 = -1, peakIndex2 = -1,peakIndex=-1;

    for (int i = 0; i < xDataNormalized.size(); i++)
    {
        double x = xDataNormalized[i];
        if(freeze)
        {
            if (yDataFreeze[i] > phValue)
            {
                peakIndex = i;
                phValue = qMax(phValue, yDataFreeze[i]);
            }
        }
        else
        {
            if (x >= normG1Start && x <= normG1End)
            {
                if (yData[i] > ph1Value) peakIndex1 = i;
                ph1Value = qMax(ph1Value, yData[i]);
            }
            if (x >= normG2Start && x <= normG2End)
            {
                if (yData[i] > ph2Value) peakIndex2 = i;
                ph2Value = qMax(ph2Value, yData[i]);
            }
        }
    }

    double bp1Value = 0, bp2Value = 0;

    if(!freeze)
    {
        for (int i = peakIndex1; i >= 0; i--)
        {
            if (yData[i] == 0)
            {
                bp1Value = xDataNormalized[i];
                break;
            }
        }
        for (int i = peakIndex2; i >= 0; i--)
        {
            if (yData[i] == 0)
            {
                bp2Value = xDataNormalized[i];
                break;
            }

        }
    }

    if(freeze)
    {
        int Lm=0,Rm=0, count = (int)config.range / 20;
        for(int i = peakIndex;i>=0;i--)
        {
            if(yDataFreeze[i] == 0)
            {
                if(count == ((int)config.range /20))
                {
                    Lm = xDataNormalized[i] * 100 / config.range;
                }
                else if(count == 0){
                    break;
                }
                count--;
            }
            else{
                count = config.range / 20;
            }
        }

        count = config.range /20;

        for(int i =peakIndex;i<xDataNormalized.size();i++)
        {
            if(yDataFreeze[i] == 0)
            {
                if(count == ((int)config.range /20))
                {
                    Rm = xDataNormalized[i] * 100 / config.range;
                }
                else if(count == 0){
                    break;
                }
                count--;
            }
            else{
                count = config.range / 20;
            }
        }
        ui->label_Hmval->setText(QString::number(Lm) + "-" +QString::number(Rm) );
    }
    else
    {
        ui->label_HM->hide();
        ui->label_Hmval->hide();
    }

    if(freeze)
    {
        ui->label_G1->hide();
        ui->label_G2->hide();

        ui->lineEdit_PHG2->hide();
        ui->lineEdit_BPG2->hide();
        ui->lineEdit_SDG2->hide();
        ui->lineEdit_DG2->hide();

        ui->lineEdit_PHG1->setText(QString::number(phValue, 'd', 0));
        ui->lineEdit_BPG1->setText(QString::number(xDataNormalized[peakIndex],'d', 0));
        ui->lineEdit_DG1->setText(QString::number(xDataNormalized[peakIndex] * cos(config.Angle * M_PI / 180.0), 'd', 0));
        ui->lineEdit_SDG1->setText(QString::number(xDataNormalized[peakIndex] * sin(config.Angle * M_PI / 180.0), 'd', 0));
    }
    // Update UI
    else{
        ui->lineEdit_PHG1->setText(QString::number(ph1Value, 'd', 0));
        ui->lineEdit_PHG2->setText(QString::number(ph2Value, 'd', 0));

        ui->lineEdit_BPG1->setText(QString::number(bp1Value, 'd', 0));
        ui->lineEdit_BPG2->setText(QString::number(bp2Value, 'd', 0));

        ui->lineEdit_DG1->setText(QString::number(bp1Value * cos(config.Angle * M_PI / 180.0), 'd', 0));
        ui->lineEdit_SDG1->setText(QString::number(bp1Value * sin(config.Angle * M_PI / 180.0), 'd', 0));

        ui->lineEdit_DG2->setText(QString::number(bp2Value * cos(config.Angle * M_PI / 180.0), 'd', 0));
        ui->lineEdit_SDG2->setText(QString::number(bp2Value * sin(config.Angle * M_PI / 180.0), 'd', 0));
    }



    // ui->lineEdit_P2P->setText(QString::number(
    //     abs(ui->lineEdit_DG1->text().toInt() - ui->lineEdit_DG2->text().toInt()), 'd', 0));

    // ------------------ Plot Drawing ------------------
    ui->Plot->clearGraphs();

    // Threshold lines (no delay offset needed anymore)
    ui->Plot->addGraph();
    ui->Plot->graph(0)->setData(QVector<double>{normG1Start,normG1End},
                                QVector<double>{th1, th1});
    //qDebug() <<"Gs"<<normG1Start <<"GE"<<normG1End;
    ui->Plot->graph(0)->setPen(QPen(QColor("#219601"), 2)); // Green

    ui->Plot->addGraph();
    ui->Plot->graph(1)->setData(QVector<double>{normG2Start , normG2End},
                                QVector<double>{th2, th2});
    ui->Plot->graph(1)->setPen(QPen(QColor("#0818ff"), 2)); // Blue

    ui->Plot->xAxis->setRange(0, config.range);

    ui->Plot->addGraph();
    if (config.channel == 1){
         ui->Plot->graph(2)->setPen(QPen(Qt::magenta, 2));
    }
    else{
         ui->Plot->graph(2)->setPen(QPen(Qt::yellow, 2));
    }

    if(freeze){
        ui->Plot->graph(2)->setData(xDataNormalized, yDataFreeze);
        ui->Plot->graph(2)->setLineStyle(QCPGraph::lsImpulse);
    }
    else{
        ui->Plot->graph(2)->setData(xDataNormalized, yData);
        ui->Plot->graph(2)->setLineStyle(QCPGraph::lsLine);
    }

    ui->Plot->replot();
}


void PreviewScreen::setupPlotAppearance()
{
    ui->Plot->setFixedSize(420, 270);
    ui->Plot->setBackground(Qt::white);         // Changed the color

    // Configure a fixed ticker for the X-axis
    QSharedPointer<QCPAxisTickerFixed> xTicker(new QCPAxisTickerFixed);

    xTicker->setTickStep(entry.range/(10));
    xTicker->setScaleStrategy(QCPAxisTickerFixed::ssNone);
    ui->Plot->xAxis->setTicker(xTicker);
    ui->Plot->xAxis->setBasePen(QPen(Qt::black));
    ui->Plot->xAxis->setTickPen(QPen(Qt::black));
    ui->Plot->xAxis->setTickLabelColor(Qt::black);
    ui->Plot->xAxis->grid()->setPen(QPen(Qt::gray));
    ui->Plot->xAxis->grid()->setSubGridPen(QPen(Qt::darkGray));

    // Configure a fixed ticker for the Y-axis
    QSharedPointer<QCPAxisTickerFixed> yTicker(new QCPAxisTickerFixed);
    yTicker->setTickStep(10);  // Set step for 10 intervals
    yTicker->setScaleStrategy(QCPAxisTickerFixed::ssNone);
    ui->Plot->yAxis->setTicker(yTicker);
    ui->Plot->yAxis->setBasePen(QPen(Qt::black));
    ui->Plot->yAxis->setTickPen(QPen(Qt::black));
    ui->Plot->yAxis->setTickLabelColor(Qt::black);
    ui->Plot->yAxis->grid()->setPen(QPen(Qt::gray));
    ui->Plot->yAxis->grid()->setSubGridPen(QPen(Qt::darkGray));

    ui->Plot->yAxis->setTickLabelSide(QCPAxis::lsInside);
    ui->Plot->xAxis->setTickLabelSide(QCPAxis::lsOutside);

    // Set range to ensure the grid fits within the plot
    ui->Plot->xAxis->setRange(0, 100);
    ui->Plot->yAxis->setRange(0, 100);

    ui->Plot->replot();
}


void PreviewScreen::on_pushButton_save_clicked()
{
    // QString jsonFile = "testdetails.json";
    // QFile file(jsonFile);
    // if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //     qWarning() << "Cannot open JSON file:" << jsonFile;
    //     return;
    // }

    // QByteArray data = file.readAll();
    // file.close();

    // QJsonDocument doc = QJsonDocument::fromJson(data);
    // if (doc.isNull() || !doc.isObject()) {
    //     qWarning() << "Invalid JSON format in" << jsonFile;
    //     return;
    // }

    // QJsonObject frameData = doc.object();

    // QString wheelNo = frameData["AxleWheelNo"].toString();
    // QString frameNo = frameData["FrameNo"].toString();
    // QString operatorName = frameData["OperatorName"].toString();

    QString baseDir = "SavedData";

    // --- Ensure directories exist ---
    QDir dir;
    if (!dir.exists(baseDir))
        dir.mkpath(baseDir);

    QString dateFolder = QString("%1/%2%3").arg(baseDir).arg(QDate::currentDate().toString("dd-MM-yyyy")).arg(Line);
    if (!dir.exists(dateFolder))
        dir.mkpath(dateFolder);

    // --- Capture preview of screen ---
    QPixmap pixmap = this->grab();

    // --- Adjust JPG output size ---
    QSize jpgTargetSize(800, 600); // You can change this (640x480, 1024x768, etc.)
    QPixmap scaledJpg = pixmap.scaled(jpgTargetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // QString jpgFileName = QString("%1/Wheel_%2_Frame_%3.jpg")
    //                           .arg(dateFolder)
    //                           .arg(wheelNo)
    //                           .arg(frameNo);

    QString jpgFileName = QString("%1/CH-%2 TP-%3 KM-%4 M-%5.jpg").arg(dateFolder).arg(entry.channel).arg(TP).arg(km).arg(M);

    if (!scaledJpg.save(jpgFileName, "JPG", 90))
        qWarning() << "Failed to save JPG:" << jpgFileName;
    else
        qDebug() << "[INFO] Saved JPG:" << jpgFileName;

    // --- Prepare PDF file path ---
    /*QString pdfFileName = QString("%1/Wheel_%2.pdf").arg(baseDir).arg(wheelNo);

    // --- Setup PDF writer ---
    QPdfWriter pdfWriter(pdfFileName);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(300); // 300 DPI for clear print quality

    QPainter painter(&pdfWriter);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    int pdfWidth = pdfWriter.width();
    int pdfHeight = pdfWriter.height();
    int topMargin = 200;
    int sideMargin = 80;

    // --- Scale image to fit nicely within PDF page ---
    int availableWidth = pdfWidth - (2 * sideMargin);
    int availableHeight = pdfHeight - (topMargin + 200);
    QPixmap scaledPdf = pixmap.scaled(availableWidth, availableHeight,
                                      Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // --- Header text ---
    QFont headerFont;
    headerFont.setPointSize(14);
    headerFont.setBold(true);
    painter.setFont(headerFont);

    QString header = QString("Wheel No: %1 | Frame No: %2 | Operator: %3 | Time: %4")
                         .arg(wheelNo)
                         .arg(frameNo)
                         .arg(operatorName)
                         .arg(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss"));

    painter.drawText(QRect(0, 0, pdfWidth, topMargin), Qt::AlignCenter, header);

    // --- Draw scaled image centered on page ---
    int x = (pdfWidth - scaledPdf.width()) / 2;
    int y = topMargin;
    painter.drawPixmap(x, y, scaledPdf.width(), scaledPdf.height(), scaledPdf);

    painter.end();
    qDebug() << "[INFO] Saved PDF:" << pdfFileName;

    // --- Update TestDetails.txt ---
    QFile detailsFile(baseDir + "/TestDetails.txt");
    if (detailsFile.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&detailsFile);
        out << QString("Wheel:%1, Frame:%2, Operator:%3, Time:%4\n")
                   .arg(wheelNo)
                   .arg(frameNo)
                   .arg(operatorName)
                   .arg(QDateTime::currentDateTime().toString("hh:mm:ss dd-MM-yyyy"));
        detailsFile.close();
    }

    // --- Update SummaryLog.txt when last frame is completed ---
    const int maxFramesPerWheel = 6; // change if needed
    if (frameNo.toInt() >= maxFramesPerWheel) {
        QFile summaryFile(baseDir + "/SummaryLog.txt");
        if (summaryFile.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&summaryFile);
            out << QString("Wheel %1 completed with %2 frames: %3\n")
                       .arg(wheelNo)
                       .arg(frameNo)
                       .arg(pdfFileName);
            summaryFile.close();
        }
        qDebug() << "[INFO] Wheel" << wheelNo << "completed.";
    }*/

}

void PreviewScreen::setDetails(const QString &ModelNo, const QString &FilePath)
{
    // ui->lineEdit_ModelMcNo->setText(ModelNo);
    // ui->lineEdit_FilePath->setText(FilePath);
}



