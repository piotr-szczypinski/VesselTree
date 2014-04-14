#ifndef RENDERITQT_H
#define RENDERITQT_H

#include <QWidget>
#include <QVTKWidget.h>
#include "renderit.h"

/*! RenderITQt tworzy widget wyświetlania dla klasy RenderIT, po której dziedziczy.
Widget mozna wykorzystać w oknach z interfejsem graficznym Qt.*/

class RenderITQt:public RenderIT, QVTKWidget
{
public:
    RenderITQt(QWidget* parent = NULL, Qt::WFlags f = 0);
};

#endif // RENDERITQT_H
