#ifndef TRACKINGCONFIGDLG_H
#define TRACKINGCONFIGDLG_H

#include <QDialog>
#include "ui_trackingconfigdlg.h"
#include "FiberData.h"

class TrackingConfigDlg : public QDialog
{
	Q_OBJECT

public:
	TrackingConfigDlg(QWidget *parent = 0);
	~TrackingConfigDlg();

private:
	Ui::TrackingConfigDlgClass ui;

public:
	static TrackingConfig*		getConfig(QWidget* parent, const QString &caption);
};

#endif // TRACKINGCONFIGDLG_H
