#pragma once

#include <QDate>
#include <QDir>
#include <QProgressDialog>
#include <QStringList>
#include <QTextStream>
#include <QTreeWidget>
#include "DicomImage.h"
#include "XMLHelper.h"

using namespace XMLHelper;

namespace DicomImageSet
{
	//int		readDicomImages	(QStringList orderedFileList);
	static const char *	DATASET_FILE_NAME = "dicom_dataset.xml";
	int					parseFolder			(QString folderName);
	QStringList			getAllFiles			(const QString &folderName);
	int					insertFileItem		(IXMLDOMDocument *pDom, IXMLDOMNode * pRoot, const QString &folderName, const QString &fileName);
	IXMLDOMNode *		findParentNode		(IXMLDOMDocument *pXMLDom, IXMLDOMNode * pRoot, const char * patientsName, const char * patientsId, int acquisitionDate, const char * protocol, int acquisitionNumber);
	int					readDatasetFile		(const QString &datasetFileName, QTreeWidget * treeWidget);
	int					insertDTIFileItem	(const QString &datasetFolderName, const QString &queryString, const QString &dtiFileName);
	QString				getDTIFileName		(const QString &datasetFolderName, const QString &queryString);
	QStringList			getOrderedDcmFiles	(const QString &datasetFileName, const QString &queryString);
}
