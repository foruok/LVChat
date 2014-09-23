#include "pointSizeToPixelSize.h"
#include <QGuiApplication>
#include <QScreen>
#include <QFont>
#include <QFontMetrics>
#include <QDebug>

int PointSizeToPixelSize::pixelSize(int pointSize)
{
    QScreen *screen = qApp->primaryScreen();
    int ps = (pointSize * screen->logicalDotsPerInch()) / 72;
    qDebug() << "pointSize " << pointSize << " - pixelSize " << ps;
    return ps;
}

int PointSizeToPixelSize::height(int pointSize)
{
    QFont font = qApp->font();
    font.setPointSize(pointSize);
    QFontMetrics fm(font);
    return fm.height();
}

int PointSizeToPixelSize::width(int pointSize, QString text)
{
    QFont font = qApp->font();
    font.setPointSize(pointSize);
    QFontMetrics fm(font);
    return fm.width(text);
}
