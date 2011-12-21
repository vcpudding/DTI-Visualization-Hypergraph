#include "trackingconfigdlg.h"

TrackingConfigDlg::TrackingConfigDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

TrackingConfigDlg::~TrackingConfigDlg()
{

}

TrackingConfig* TrackingConfigDlg::getConfig(QWidget *parent, const QString &caption)
{
	TrackingConfigDlg dlg(parent);
	dlg.setWindowTitle(caption);
	if (dlg.exec()==QDialog::Accepted)
	{
		TrackingConfig* config = new TrackingConfig[1];
		config->faThres = dlg.ui.faSpin->value();
		config->angleThres = dlg.ui.angleSpin->value();
		config->stepSize = dlg.ui.stepSpin->value();
		config->minLength = dlg.ui.minLengthSpin->value();
		config->maxLength = dlg.ui.maxLengthSpin->value();
		config->isFiberCount = dlg.ui.isFiberCount->currentIndex();
		config->nFibers = dlg.ui.nFiberSpin->value();
		return config;
	}
	return NULL;
}