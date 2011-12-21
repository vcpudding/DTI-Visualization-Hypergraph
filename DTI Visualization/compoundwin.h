#ifndef COMPOUNDWIN_H
#define COMPOUNDWIN_H

#include <QWidget>
#include "ui_compoundwin.h"
#include "fiberglwidget.h"
#include "graphglwidget.h"
#include "FiberData.h"
#include "Hypergraph.h"

class CompoundWin : public QWidget
{
	Q_OBJECT

public:
	CompoundWin(FiberData * fibData, Hypergraph * graph, QWidget *parent = 0);
	~CompoundWin();

private:
	Ui::CompoundWinClass ui;
	FiberGLWidget *			_fibWidget;
	GraphGLWidget *			_graphWidget;
	//ColorTable *			_clusterColorer;
};

#endif // COMPOUNDWIN_H
