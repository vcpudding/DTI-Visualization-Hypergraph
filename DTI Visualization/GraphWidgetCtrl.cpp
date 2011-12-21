#include "GraphWidgetCtrl.h"
#include <QMenu>
#include <QMessageBox>

GraphWidgetCtrl::GraphWidgetCtrl(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//btnLinkage = ui.btnLinkage;
	btnAssign = ui.btnAssignCluster;

	QActionGroup *gestureGroup = new QActionGroup(this);
	gestureGroup->setExclusive(true);
	actionGestureClickDrag = new QAction("Click / Drag", gestureGroup);
	actionGestureLinkage = new QAction("Linkage", gestureGroup);
	actionGestureClickDrag->setCheckable(true);
	actionGestureLinkage->setCheckable(true);
	actionGestureClickDrag->setChecked(true);

	gestureMapper = new QSignalMapper();
	connect(actionGestureClickDrag, SIGNAL(triggered()), gestureMapper, SLOT(map()));
	connect(actionGestureLinkage, SIGNAL(triggered()), gestureMapper, SLOT(map()));
	gestureMapper->setMapping(actionGestureClickDrag, (int)GEST_CLICKDRAG);
	gestureMapper->setMapping(actionGestureLinkage, (int)GEST_LINKAGE);

	/*QMenu *gestureMenu = new QMenu(this);
	gestureMenu->addActions(gestureGroup->actions());
	ui.btnGesture->setMenu(gestureMenu);*/

	QActionGroup *selModeGroup = new QActionGroup(this);
	actionModeNew = new QAction("New", selModeGroup);
	actionModeIntersect = new QAction("Intersect", selModeGroup);
	actionModeUnion = new QAction("Union", selModeGroup);
	actionModeDiff = new QAction("Difference", selModeGroup);
	actionModeRemove = new QAction("Remove", selModeGroup);
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
	ui.btnMode->setMenu(selModeMenu);

	QActionGroup *dispModeGroup = new QActionGroup(this);
	QAction * actionDispGraph = new QAction("Graph", dispModeGroup);
	QAction * actionDispPinWeight = new QAction("Pin weights", dispModeGroup);
	QAction * actionDispPartitions = new QAction("Partitions", dispModeGroup);
	actionDispGraph->setCheckable(true);
	actionDispPinWeight->setCheckable(true);
	actionDispPartitions->setCheckable(true);
	actionDispGraph->setChecked(true);

	dispModeMapper = new QSignalMapper();
	connect(actionDispGraph, SIGNAL(triggered()), dispModeMapper, SLOT(map()));
	connect(actionDispPinWeight, SIGNAL(triggered()), dispModeMapper, SLOT(map()));
	connect(actionDispPartitions, SIGNAL(triggered()), dispModeMapper, SLOT(map()));
	dispModeMapper->setMapping(actionDispGraph, (int)DISP_GRAPH);
	dispModeMapper->setMapping(actionDispPinWeight, (int)DISP_PINWEIGHTS);
	dispModeMapper->setMapping(actionDispPartitions, (int)DISP_PARTITIONS);

	QMenu *dispModeMenu = new QMenu();
	dispModeMenu->addActions(dispModeGroup->actions());
	ui.btnDispMode->setMenu(dispModeMenu);
}

GraphWidgetCtrl::~GraphWidgetCtrl()
{

}
