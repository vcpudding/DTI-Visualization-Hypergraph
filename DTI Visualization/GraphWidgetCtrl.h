#ifndef GRAPHWIDGETCTRL_H
#define GRAPHWIDGETCTRL_H

#include <QWidget>
#include "ui_GraphWidgetCtrl.h"

#include <QAction>
#include <QSignalMapper>
#include "common.h"

class GraphWidgetCtrl : public QWidget
{
	Q_OBJECT

public:
	GraphWidgetCtrl(QWidget *parent = 0);
	~GraphWidgetCtrl();

private:
	Ui::GraphWidgetCtrlClass ui;

public:
	QAction *		actionGestureClickDrag;
	QAction *		actionGestureLinkage;
	QAction *		actionModeNew;
	QAction *		actionModeIntersect;
	QAction *		actionModeUnion;
	QAction *		actionModeDiff;
	QAction *		actionModeRemove;

	QSignalMapper * gestureMapper;
	QSignalMapper * selModeMapper;
	QSignalMapper * dispModeMapper;
	QPushButton *	btnLinkage;
	QPushButton *	btnAssign;
};

#endif // GRAPHWIDGETCTRL_H
