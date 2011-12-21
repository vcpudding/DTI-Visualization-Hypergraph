#include "FiberWidgetCtrl.h"

#include <QMenu>

FiberWidgetCtrl::FiberWidgetCtrl(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QActionGroup *fibOpGroup = new QActionGroup(this);
	fibOpGroup->setExclusive(true);
	QAction *actionOpNav = new QAction("Navigate", fibOpGroup);
	QAction *actionOpSel = new QAction("Select", fibOpGroup);
	actionOpNav->setCheckable(true);
	actionOpSel->setCheckable(true);
	actionOpNav->setChecked(true);

	fibOpMapper = new QSignalMapper();
	connect(actionOpNav, SIGNAL(triggered()), fibOpMapper, SLOT(map()));
	connect(actionOpSel, SIGNAL(triggered()), fibOpMapper, SLOT(map()));
	fibOpMapper->setMapping(actionOpNav, (int)OP_NAV);
	fibOpMapper->setMapping(actionOpSel, (int)OP_SEL);

	QMenu *fibOpMenu = new QMenu();
	fibOpMenu->addActions(fibOpGroup->actions());
	ui.btnOperation->setMenu(fibOpMenu);

	QActionGroup *selModeGroup = new QActionGroup(this);
	QAction *actionModeNew = new QAction("New", selModeGroup);
	QAction *actionModeIntersect = new QAction("Intersect", selModeGroup);
	QAction *actionModeUnion = new QAction("Union", selModeGroup);
	QAction *actionModeDiff = new QAction("Difference", selModeGroup);
	QAction *actionModeRemove = new QAction("Remove", selModeGroup);
	actionModeNew->setCheckable(true);
	actionModeIntersect->setCheckable(true);
	actionModeUnion->setCheckable(true);
	actionModeDiff->setCheckable(true);
	actionModeRemove->setCheckable(true);
	actionModeNew->setChecked(true);

	selModeMapper = new QSignalMapper();
	connect(actionModeNew, SIGNAL(triggered()), selModeMapper, SLOT(map()));
	connect(actionModeIntersect, SIGNAL(triggered()), selModeMapper, SLOT(map()));
	connect(actionModeUnion, SIGNAL(triggered()), selModeMapper, SLOT(map()));
	connect(actionModeDiff, SIGNAL(triggered()), selModeMapper, SLOT(map()));
	connect(actionModeRemove, SIGNAL(triggered()), selModeMapper, SLOT(map()));
	selModeMapper->setMapping(actionModeNew, (int)SEL_NEW);
	selModeMapper->setMapping(actionModeIntersect, (int)SEL_INTERSECT);
	selModeMapper->setMapping(actionModeUnion, (int)SEL_UNION);
	selModeMapper->setMapping(actionModeDiff, (int)SEL_DIFF);
	selModeMapper->setMapping(actionModeRemove, (int)SEL_REMOVE);

	QMenu *selModeMenu = new QMenu();
	selModeMenu->addActions(selModeGroup->actions());
	ui.btnSelMode->setMenu(selModeMenu);

	QActionGroup *clrModeGroup = new QActionGroup(this);
	QAction *actionClrDir = new QAction("Direction", clrModeGroup);
	QAction *actionClrCluster = new QAction("Cluster", clrModeGroup);
	actionClrDir->setCheckable(true);
	actionClrCluster->setCheckable(true);
	actionClrCluster->setChecked(true);

	clrModeMapper = new QSignalMapper();
	connect(actionClrDir, SIGNAL(triggered()), clrModeMapper, SLOT(map()));
	connect(actionClrCluster, SIGNAL(triggered()), clrModeMapper, SLOT(map()));
	clrModeMapper->setMapping(actionClrDir, (int)COLOR_DIR);
	clrModeMapper->setMapping(actionClrCluster, (int)COLOR_CLUSTER);

	QMenu *clrModeMenu = new QMenu();
	clrModeMenu->addActions(clrModeGroup->actions());
	ui.btnClrMode->setMenu(clrModeMenu);

	QActionGroup *dispModeGroup = new QActionGroup(this);
	QAction *actionDispFiber = new QAction("Fibers", dispModeGroup);
	QAction *actionDispClusterCenters = new QAction("Cluster centers", dispModeGroup);
	QAction *actionDispMatchings = new QAction("Matchings", dispModeGroup);
	actionDispFiber->setCheckable(true);
	actionDispClusterCenters->setCheckable(true);
	actionDispMatchings->setCheckable(true);
	actionDispFiber->setChecked(true);

	dispModeMapper = new QSignalMapper();
	connect(actionDispFiber, SIGNAL(triggered()), dispModeMapper, SLOT(map()));
	connect(actionDispClusterCenters, SIGNAL(triggered()), dispModeMapper, SLOT(map()));
	connect(actionDispMatchings, SIGNAL(triggered()), dispModeMapper, SLOT(map()));
	dispModeMapper->setMapping(actionDispFiber, (int)DISP_FIBERS);
	dispModeMapper->setMapping(actionDispClusterCenters, (int)DISP_CLUSTER_CENTERS);
	dispModeMapper->setMapping(actionDispMatchings, (int)DISP_MATCHINGS);

	QMenu *dispModeMenu = new QMenu();
	dispModeMenu->addActions(dispModeGroup->actions());
	ui.btnDispMode->setMenu(dispModeMenu);
}

FiberWidgetCtrl::~FiberWidgetCtrl()
{

}
