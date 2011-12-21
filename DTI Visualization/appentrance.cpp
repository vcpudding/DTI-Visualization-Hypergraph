#include "appentrance.h"
#include "dtivisualization.h"
#include "tractvisualization.h"

AppEntrance::AppEntrance(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connectSignalSlots();
}

AppEntrance::~AppEntrance()
{

}

void AppEntrance::connectSignalSlots()
{
	connect(ui.btnDicom, SIGNAL(clicked()), this, SLOT(handleDicom()));
	connect(ui.btnTract, SIGNAL(clicked()), this, SLOT(handleTract()));
}

void AppEntrance::handleDicom()
{
	DTIVisualization *w = new DTIVisualization();
	w->showMaximized();
	this->hide();
}

void AppEntrance::handleTract()
{
	TractVisualization *w = new TractVisualization();
	w->showMaximized();
	this->hide();
}