#ifndef INITPLOT_H
#define INITPLOT_H

#include "qcustomplot.h"

static void defaultPlotInit(QCustomPlot *plot)
{
    // set some pens, brushes and backgrounds:
    plot->xAxis->setBasePen(QPen(Qt::white, 1));
    plot->yAxis->setBasePen(QPen(Qt::white, 1));
    plot->yAxis2->setBasePen(QPen(Qt::white, 1));
    plot->xAxis->setTickPen(QPen(Qt::white, 1));
    plot->yAxis->setTickPen(QPen(Qt::white, 1));
    plot->yAxis2->setTickPen(QPen(Qt::white, 1));
    plot->xAxis->setSubTickPen(QPen(Qt::white, 1));
    plot->yAxis->setSubTickPen(QPen(Qt::white, 1));
    plot->yAxis2->setSubTickPen(QPen(Qt::white, 1));
    plot->xAxis->setTickLabelColor(Qt::white);
    plot->yAxis->setTickLabelColor(Qt::white);
    plot->yAxis2->setTickLabelColor(Qt::white);
    plot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    plot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    plot->yAxis2->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    plot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    plot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    plot->yAxis2->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    plot->xAxis->grid()->setSubGridVisible(true);
    plot->yAxis->grid()->setSubGridVisible(true);
    plot->yAxis2->grid()->setSubGridVisible(true);
    plot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    plot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    plot->yAxis2->grid()->setZeroLinePen(Qt::NoPen);
//    plot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
//    plot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    plot->setBackground(plotGradient);
//    QLinearGradient axisRectGradient;
//    axisRectGradient.setStart(0, 0);
//    axisRectGradient.setFinalStop(0, 350);
//    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
//    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
//    plot->axisRect()->setBackground(axisRectGradient);
    // Note: we could have also just called plot->rescaleAxes(); instead
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables  | QCP::iMultiSelect);
}

static QCPPolarAxisAngular* setPloatGraphPolar(QCustomPlot *plot)
{
    plot->plotLayout()->clear();
    QCPPolarAxisAngular* angularAxis = new QCPPolarAxisAngular(plot);
    plot->plotLayout()->addElement(0, 0, angularAxis);


    angularAxis->setRangeDrag(false);
    angularAxis->setTickLabelMode(QCPPolarAxisAngular::lmUpright);

    angularAxis->radialAxis()->setTickLabelMode(QCPPolarAxisRadial::lmUpright);
    angularAxis->radialAxis()->setTickLabelRotation(0);
    angularAxis->radialAxis()->setAngle(45);
    angularAxis->radialAxis()->setBasePen(QPen(Qt::white, 1));
    angularAxis->radialAxis()->setTickPen(QPen(Qt::white, 1));
    angularAxis->radialAxis()->setSubTickPen(QPen(Qt::white, 1));
    angularAxis->radialAxis()->setLabelColor(Qt::white);
    angularAxis->radialAxis()->setTickLabelColor(Qt::white);

    angularAxis->setBasePen(QPen(Qt::white, 1));
    angularAxis->setTickPen(QPen(Qt::white, 1));
    angularAxis->setSubTickPen(QPen(Qt::white, 1));
    angularAxis->setLabelColor(Qt::white);
    angularAxis->setTickLabelColor(Qt::white);


    angularAxis->grid()->setAngularPen(QPen(QColor(140, 140, 140), 0, Qt::DotLine));
    angularAxis->grid()->setSubGridType(QCPPolarGrid::gtAll);

    angularAxis->setRange(0, 360);

    return angularAxis;

}

#endif // INITPLOT_H
