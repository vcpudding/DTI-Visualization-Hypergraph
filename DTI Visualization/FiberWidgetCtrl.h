#ifndef FIBERWIDGETCTRL_H
#define FIBERWIDGETCTRL_H

#include <QWidget>
#include "ui_FiberWidgetCtrl.h"
#include <QSignalMapper>
#include "common.h"

class FiberWidgetCtrl : public QWidget
{
	Q_OBJECT

public:
	FiberWidgetCtrl(QWidget *parent = 0);
	~FiberWidgetCtrl();

private:
	Ui::FiberWidgetCtrlClass ui;

public:
	QSignalMapper *		fibOpMapper;
	QSignalMapper *		selModeMapper;
	QSignalMapper *		clrModeMapper;
	QSignalMapper *		dispModeMapper;
};

#endif // FIBERWIDGETCTRL_H
