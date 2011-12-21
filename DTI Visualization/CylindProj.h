#ifndef TEMPLATE_TRACTS_H
#define TEMPLATE_TRACTS_H

#include "FiberData.h"
#include "fibervis.h"

CylindPoint		cylindProjection (const FloatPoint &pt);
CylindPoint		clusterCenter (FiberData *fiberData, int clusterIndex);

//for template making
FiberVis *		editTemplateClusters ();
void			embedTemplateClusters ();
void			zAxisNonUniformScaling (vector<CylindPoint> &embBuf);
void			zAxisNonUniformScaling (vector<CylindPoint> &embBuf, vector<int> &clusterBuf);
float			zAxisDensity (vector<CylindPoint> &embBuf, float z);

#endif