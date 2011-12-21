#ifndef APPENTRANCE_H
#define APPENTRANCE_H

#include <QWidget>
#include "ui_appentrance.h"

class AppEntrance : public QWidget
{
	Q_OBJECT

public:
	AppEntrance(QWidget *parent = 0);
	~AppEntrance();

private:
	Ui::AppEntranceClass ui;

	void			connectSignalSlots();

private slots:
	void			handleDicom ();
	void			handleTract ();
};

#endif // APPENTRANCE_H
