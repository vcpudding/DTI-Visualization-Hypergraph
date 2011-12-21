#include "DicomImageSet.h"

int DicomImageSet::parseFolder(QString folderName)
{
	QStringList fileList = getAllFiles(folderName);
    HRESULT hr = CoInitialize(NULL);
    if (!SUCCEEDED(hr))
    {
		return NULL;
    }

    hr = S_OK;
    IXMLDOMDocument *pXMLDom = NULL;
    IXMLDOMElement *pRoot = NULL;

    BSTR bstrXML = NULL;
    VARIANT varFileName;
    VariantInit(&varFileName);

	QProgressDialog progress (QString("Parsing all files in %1").arg(folderName), QString("Cancel"), 0, fileList.count());

    CHK_HR(CreateAndInitDOM(&pXMLDom));

    // Create a processing instruction element.
    CHK_HR(CreateAndAddPINode(pXMLDom, L"xml", L"version='1.0'"));
    
    // Create the root element.
    CHK_HR(CreateElement(pXMLDom, L"root", &pRoot));

    // Create an attribute for the <root> element, with name "created" and value "using dom".
	CHK_HR(CreateAndAddAttributeNode(pXMLDom, L"path", folderName.toStdWString().c_str(), pRoot));

    // Add NEWLINE for identation before </root>.
    CHK_HR(CreateAndAddTextNode(pXMLDom, L"\n", pRoot));
    // add <root> to document
    CHK_HR(AppendChildToParent(pRoot, pXMLDom));

	progress.show();
	for (int i=0; i<fileList.count(); ++i)
	{
		insertFileItem(pXMLDom, pRoot, folderName, fileList[i]);
		progress.setValue(i);
	}

	CHK_HR(VariantFromString(QDir(folderName).filePath(DATASET_FILE_NAME).toStdWString().c_str(), varFileName));
    CHK_HR(pXMLDom->save(varFileName));

CleanUp:
    SAFE_RELEASE(pRoot);
    SysFreeString(bstrXML);
    VariantClear(&varFileName);

    CoUninitialize();
	if (FAILED(hr))
	{
		SAFE_RELEASE(pXMLDom);
		return 0;
	}
	return 1;
}

QStringList DicomImageSet::getAllFiles(const QString &folderName)
{
	QDir folder (folderName);
	QFileInfoList fileInfoList = folder.entryInfoList();
	QStringList fileList;
	for (int i=0; i<fileInfoList.count(); ++i)
	{
		QString fName = fileInfoList[i].fileName();
		if (!fileInfoList[i].isDir())
		{
			fileList.append(fileInfoList[i].absoluteFilePath());
		}		
		else if (fileInfoList[i].fileName().compare (".") && fileInfoList[i].fileName().compare (".."))
		{
			fileList = fileList + getAllFiles(fileInfoList[i].absoluteFilePath());
		}
	}
	return fileList;
}

int DicomImageSet::insertFileItem(IXMLDOMDocument *pDom, IXMLDOMNode * pRoot, const QString &folderName, const QString &fileName)
{
	DicomImage * dicomImage = new DicomImage ();
	if (!dicomImage->readHeader((QDir(folderName).absoluteFilePath(fileName)).toStdString().c_str()))
	{
		//fail to resolve header information
		return 0;
	}
	int nElement;

	char * patientsName = dicomImage->getCharTag(DicomImage::TAG_PATIENTS_NAME, nElement);
	char * patientsId = dicomImage->getCharTag(DicomImage::TAG_PATIENT_ID, nElement);
	char * protocolName = dicomImage->getCharTag(DicomImage::TAG_PROTOCOL_NAME, nElement);
	double * pInstanceNumber = dicomImage->getNumericTag(DicomImage::TAG_INSTANCE_NUMBER, nElement);
	double * pAcquisitionNumber = dicomImage->getNumericTag(DicomImage::TAG_ACQUISITION_NUMBER, nElement);
	double * pAcquisitionDate = dicomImage->getNumericTag(DicomImage::TAG_ACQUISITION_DATE, nElement);
	int instanceNumber = pInstanceNumber?(int)pInstanceNumber[0]:-1;
	int acquisitionNumber = pAcquisitionNumber?(int)pAcquisitionNumber[0]:-1;
	int acquisitionDate = pAcquisitionDate?(int)pAcquisitionDate[0]:0;

	IXMLDOMNode * pParent = findParentNode(pDom, pRoot, patientsName, patientsId, acquisitionDate, protocolName, acquisitionNumber);

	if (!pParent)
	{
		return 0;
	}

	HRESULT hr = S_OK;
    IXMLDOMElement *pNode = NULL;
    CHK_HR(CreateElement(pDom, L"File", &pNode));
	CHK_HR(CreateAndAddAttributeNode(pDom, L"path", fileName.toStdWString().c_str(), pNode));
	CHK_HR(CreateAndAddAttributeNode(pDom, L"instance_number", QString::number(instanceNumber).toStdWString().c_str(), pNode));

	IXMLDOMNode * pSibling = NULL;
	CHK_HR(pParent->get_firstChild(&pSibling));
	int sibInstanceNumber = -1;
	VARIANT varInstanceNumber;
	DOMNodeType nodeType;
	while (pSibling && instanceNumber >= sibInstanceNumber)
	{
		CHK_HR(pSibling->get_nodeType(&nodeType));
		if(nodeType == NODE_ELEMENT)
		{
			CHK_HR(GetAttributeFromNode(pSibling, L"instance_number", &varInstanceNumber));
			sibInstanceNumber = QString::fromWCharArray(_bstr_t(varInstanceNumber)).toInt();
			if (instanceNumber < sibInstanceNumber)
			{
				break;
			}
		}
		IXMLDOMNode * tmpNode = NULL;
		CHK_HR(pSibling->get_nextSibling(&tmpNode));
		SAFE_RELEASE(pSibling);
		pSibling = tmpNode;
	}
	if (pSibling)
	{
		IXMLDOMNode * tmpNode = NULL;
		CHK_HR(pSibling->get_previousSibling(&tmpNode));
		SAFE_RELEASE(pSibling);
		pSibling = tmpNode;
	}
    CHK_HR(CreateAndAddTextNodeBefore(pDom, L"\n", pParent, pSibling));
    CHK_HR(InsertChildToParent(pNode, pSibling, pParent));

CleanUp:
    SAFE_RELEASE(pNode);

	return 1;
}

IXMLDOMNode * DicomImageSet::findParentNode(IXMLDOMDocument *pXMLDom, IXMLDOMNode * pRoot, const char * patientsName, const char * patientsId, int acquisitionDate, const char * protocol, int acquisitionNumber)
{
	if (!pXMLDom)
	{
		return NULL;
	}
	
	HRESULT hr = S_OK;
    IXMLDOMElement *pPatient = NULL, *pDate = NULL, *pProtocol = NULL, *pCollection = NULL;
	BSTR bstrQuery = NULL;
	QString strQuery;
	bool nodeExist = false;

	//look for the patient node
	strQuery = QString("root/Patient[@patients_name = '%1' and @patient_id = '%2']").arg(patientsName).arg(patientsId);
	bstrQuery = SysAllocString(strQuery.toStdWString().c_str());
    CHK_ALLOC(bstrQuery);
    CHK_HR(pXMLDom->selectSingleNode(bstrQuery, (IXMLDOMNode**)&pPatient));
    SysFreeString(bstrQuery);
	nodeExist = pPatient;
	if (!nodeExist)
	{
		//create the patient node
		CHK_HR(CreateElement(pXMLDom, L"Patient", &pPatient));
		CHK_HR(CreateAndAddAttributeNode(pXMLDom, L"patients_name", QString(patientsName).toStdWString().c_str(), pPatient));
		CHK_HR(CreateAndAddAttributeNode(pXMLDom, L"patient_id", QString(patientsId).toStdWString().c_str(), pPatient));
		CHK_HR(CreateAndAddTextNode(pXMLDom, L"\n", pRoot));
		CHK_HR(AppendChildToParent(pPatient, pRoot));
	}
	if (nodeExist)
	{
		//look for the date node
		strQuery = QString("Date[@acquisition_date = '%1']").arg(acquisitionDate);
		bstrQuery = SysAllocString(strQuery.toStdWString().c_str());
		CHK_ALLOC(bstrQuery);
		CHK_HR(pPatient->selectSingleNode(bstrQuery, (IXMLDOMNode**)&pDate));
		SysFreeString(bstrQuery);
		nodeExist = pDate;
	}
	if (!nodeExist)
	{
		CHK_HR(CreateElement(pXMLDom, L"Date", &pDate));
		CHK_HR(CreateAndAddAttributeNode(pXMLDom, L"acquisition_date", QString::number(acquisitionDate).toStdWString().c_str(), pDate));
		CHK_HR(CreateAndAddTextNode(pXMLDom, L"\n", pPatient));
		CHK_HR(AppendChildToParent(pDate, pPatient));
	}
	if (nodeExist)
	{
		//look for the protocol node
		strQuery = QString("Protocol[@protocol_name = '%1']").arg(protocol);
		bstrQuery = SysAllocString(strQuery.toStdWString().c_str());
		CHK_ALLOC(bstrQuery);
		CHK_HR(pDate->selectSingleNode(bstrQuery, (IXMLDOMNode**)&pProtocol));
		SysFreeString(bstrQuery);
		nodeExist = pProtocol;
	}
	if (!nodeExist)
	{
		CHK_HR(CreateElement(pXMLDom, L"Protocol", &pProtocol));
		CHK_HR(CreateAndAddAttributeNode(pXMLDom, L"protocol_name", QString(protocol).toStdWString().c_str(), pProtocol));
		CHK_HR(CreateAndAddTextNode(pXMLDom, L"\n", pDate));
		CHK_HR(AppendChildToParent(pProtocol, pDate));
	}
	if (nodeExist)
	{
		//look for the collection node
		strQuery = QString("Collection[@acquisition_number = '%1']").arg(acquisitionNumber);
		bstrQuery = SysAllocString(strQuery.toStdWString().c_str());
		CHK_ALLOC(bstrQuery);
		CHK_HR(pProtocol->selectSingleNode(bstrQuery, (IXMLDOMNode**)&pCollection));
		SysFreeString(bstrQuery);
		nodeExist = pCollection;
	}
	if (!nodeExist)
	{
		CHK_HR(CreateElement(pXMLDom, L"Collection", &pCollection));
		CHK_HR(CreateAndAddAttributeNode(pXMLDom, L"acquisition_number", QString::number(acquisitionNumber).toStdWString().c_str(), pCollection));
		CHK_HR(CreateAndAddTextNode(pXMLDom, L"\n", pProtocol));
		CHK_HR(AppendChildToParent(pCollection, pProtocol));
	}

CleanUp:
	SAFE_RELEASE(pPatient);
	SAFE_RELEASE(pDate);
	SAFE_RELEASE(pProtocol);

	if(SUCCEEDED(hr))
	{
		return pCollection;
	}
	SAFE_RELEASE(pCollection);
	return NULL;
}

int DicomImageSet::readDatasetFile(const QString &datasetFileName, QTreeWidget *treeWidget)
{
    HRESULT hr = S_OK;
    IXMLDOMDocument *pXMLDom = NULL;
    IXMLDOMNodeList *pPatients = NULL;
    IXMLDOMNode *pPatient = NULL;
	DOMNodeType nodeType;
    VARIANT_BOOL varStatus;
    VARIANT varFileName;
	VARIANT varValue;
	BSTR bstrQuery;

    VariantInit(&varFileName);
    VariantInit(&varValue);

	QString queryPatient, queryDate, queryProtocol, queryCollection;

    CHK_HR(CreateAndInitDOM(&pXMLDom));

	CHK_HR(VariantFromString(datasetFileName.toStdWString().c_str(), varFileName));
    CHK_HR(pXMLDom->load(varFileName, &varStatus));
    if (varStatus != VARIANT_TRUE)
    {
        CHK_HR(ReportParseError(pXMLDom, "Failed to load DOM from stocks.xml."));
    }

	treeWidget->clear();
	treeWidget->setColumnCount(1);
	CHK_HR(pXMLDom->getElementsByTagName(L"Patient", &pPatients));
	if (pPatients)
	{
		long nPatients;
		CHK_HR(pPatients->get_length(&nPatients));
		for (long i=0; i<nPatients; ++i)
		{
			CHK_HR(pPatients->get_item(i, &pPatient));
			CHK_HR(GetAttributeFromNode(pPatient, L"patients_name", &varValue));
			queryPatient = QString::fromWCharArray(_bstr_t(varValue));
			QTreeWidgetItem * patientItem = new QTreeWidgetItem(treeWidget, QStringList(queryPatient));
			patientItem->setExpanded(true);

			IXMLDOMNodeList * pDates = NULL;
			CHK_HR(pPatient->get_childNodes(&pDates));
			long nDates;
			CHK_HR(pDates->get_length(&nDates));
			for (long j=0; j<nDates; ++j)
			{
				IXMLDOMNode * pDate = NULL;
				CHK_HR(pDates->get_item(j, &pDate));
				CHK_HR(pDate->get_nodeType(&nodeType));
				if(nodeType!=NODE_ELEMENT)
				{
					continue;
				}
				CHK_HR(GetAttributeFromNode(pDate, L"acquisition_date", &varValue));
				queryDate = QString::fromWCharArray(_bstr_t(varValue));
				int intDate = queryDate.toInt();
				QDate date = (QDate::fromString("1900-01-01", "yyyy-MM-dd")).addDays(intDate-693962);
				QTreeWidgetItem * dateItem = new QTreeWidgetItem(patientItem, QStringList(date.toString("yyyy-MM-dd")));
				dateItem->setExpanded(true);

				IXMLDOMNodeList * pProtocols = NULL;
				CHK_HR(pDate->get_childNodes(&pProtocols));
				long nProtocols;
				CHK_HR(pProtocols->get_length(&nProtocols));
				for (long j=0; j<nProtocols; ++j)
				{
					IXMLDOMNode * pProtocol = NULL;
					CHK_HR(pProtocols->get_item(j, &pProtocol));
					CHK_HR(pProtocol->get_nodeType(&nodeType));
					if(nodeType!=NODE_ELEMENT)
					{
						continue;
					}
					CHK_HR(GetAttributeFromNode(pProtocol, L"protocol_name", &varValue));
					queryProtocol = QString::fromWCharArray(_bstr_t(varValue));
					QTreeWidgetItem * protocolItem = new QTreeWidgetItem(dateItem, QStringList(queryProtocol));
					protocolItem->setExpanded(true);

					IXMLDOMNodeList * pCollections = NULL;
					CHK_HR(pProtocol->get_childNodes(&pCollections));
					long nCollections;
					CHK_HR(pCollections->get_length(&nCollections));
					for (long j=0; j<nCollections; ++j)
					{
						IXMLDOMNode * pCollection = NULL;
						CHK_HR(pCollections->get_item(j, &pCollection));
						CHK_HR(pCollection->get_nodeType(&nodeType));
						if(nodeType!=NODE_ELEMENT)
						{
							continue;
						}
						CHK_HR(GetAttributeFromNode(pCollection, L"acquisition_number", &varValue));
						queryCollection = QString::fromWCharArray(_bstr_t(varValue));
						QTreeWidgetItem * collectionItem = new QTreeWidgetItem(protocolItem);
						QString queryStr = QString("root/Patient[@patients_name = '%1']/Date[@acquisition_date = '%2']/Protocol[@protocol_name = '%3']/Collection[@acquisition_number = '%4']")
							.arg(queryPatient).arg(queryDate).arg(queryProtocol).arg(queryCollection);

						IXMLDOMElement * pDtiNode = NULL;
						bstrQuery = SysAllocString(QString(queryStr+"/DTI").toStdWString().c_str());
						CHK_ALLOC(bstrQuery);
						CHK_HR(pXMLDom->selectSingleNode(bstrQuery, (IXMLDOMNode**)&pDtiNode));
						SysFreeString(bstrQuery);
						if (pDtiNode)
						{
							collectionItem->setText(0,queryCollection+"*");
						} else
						{
							collectionItem->setText(0, queryCollection);
						}

						collectionItem->setData(0, Qt::UserRole, queryStr);
						collectionItem->setExpanded(true);


						protocolItem->addChild(collectionItem);
						SAFE_RELEASE(pCollection);
					}

					dateItem->addChild(protocolItem);
					SAFE_RELEASE(pProtocol);
				}

				patientItem->addChild(dateItem);
				SAFE_RELEASE(pDate);
			}
			treeWidget->insertTopLevelItem(i, patientItem);
			SAFE_RELEASE(pPatient);
		}
	}

CleanUp:
    SAFE_RELEASE(pXMLDom);
    SAFE_RELEASE(pPatients);
    SAFE_RELEASE(pPatient);
    VariantClear(&varFileName);

	return SUCCEEDED(hr);
}

QStringList DicomImageSet::getOrderedDcmFiles(const QString &datasetFileName, const QString & queryString)
{
    HRESULT hr = S_OK;
    IXMLDOMDocument *pXMLDom = NULL;
    IXMLDOMNodeList *pFileNodes = NULL;
    VARIANT_BOOL varStatus;
    VARIANT varFileName;
	BSTR bstrQuery;

	QStringList fileList;

    CHK_HR(CreateAndInitDOM(&pXMLDom));

	CHK_HR(VariantFromString(datasetFileName.toStdWString().c_str(), varFileName));
    CHK_HR(pXMLDom->load(varFileName, &varStatus));
    if (varStatus != VARIANT_TRUE)
    {
        CHK_HR(ReportParseError(pXMLDom, "Failed to load DOM from stocks.xml."));
    }

    bstrQuery = SysAllocString(queryString.toStdWString().c_str());
    CHK_ALLOC(bstrQuery);
    CHK_HR(pXMLDom->selectNodes(bstrQuery, &pFileNodes));
	if (pFileNodes)
	{
		long nFiles;
		CHK_HR(pFileNodes->get_length(&nFiles));
		for (long i=0; i<nFiles; ++i)
		{
			VARIANT varValue;
			IXMLDOMNode * pFile = NULL;
			CHK_HR(pFileNodes->get_item(i, &pFile));
			CHK_HR(GetAttributeFromNode(pFile, L"path", &varValue));
			fileList <<QString::fromWCharArray(_bstr_t(varValue));
			SAFE_RELEASE(pFile);
		}
	}

CleanUp:
	SAFE_RELEASE(pXMLDom);
	SAFE_RELEASE(pFileNodes);
	return fileList;
}

int DicomImageSet::insertDTIFileItem(const QString &datasetFolderName, const QString &queryString, const QString &dtiFileName)
{
	HRESULT hr = CoInitialize(NULL);
	if (!SUCCEEDED(hr))
	{
		return 0;
	}
	QString datasetFileName = datasetFolderName+DATASET_FILE_NAME;
	IXMLDOMDocument *pXMLDom = NULL;
	IXMLDOMElement *pNode = NULL, *pParent = NULL;
	BSTR bstrQuery = NULL;
	VARIANT varFileName;
	VARIANT_BOOL varStatus;
	QString queryDTIFile;

	CHK_HR(CreateAndInitDOM(&pXMLDom));
	CHK_HR(VariantFromString(datasetFileName.toStdWString().c_str(), varFileName));
	CHK_HR(pXMLDom->load(varFileName, &varStatus));

	/*queryDTIFile = queryString+QString("/DTI[@file_name = '%1']").arg(dtiFileName);
	bstrQuery = SysAllocString(queryString.toStdWString().c_str());
	CHK_ALLOC(bstrQuery);
	CHK_HR(pXMLDom->selectSingleNode(bstrQuery, (IXMLDOMNode**)&pNode));
	SysFreeString(bstrQuery);
	if (pNode)
	{
		return 1;
	}*/	

	bstrQuery = SysAllocString(queryString.toStdWString().c_str());
	CHK_ALLOC(bstrQuery);
	CHK_HR(pXMLDom->selectSingleNode(bstrQuery, (IXMLDOMNode**)&pParent));
	SysFreeString(bstrQuery);

	if (pParent)
	{
		CHK_HR(CreateElement(pXMLDom, L"DTI", &pNode));
		CHK_HR(CreateAndAddAttributeNode(pXMLDom, L"file_name", dtiFileName.toStdWString().c_str(), pNode));
		CHK_HR(CreateAndAddTextNode(pXMLDom, L"\n", pParent));
		CHK_HR(AppendChildToParent(pNode, pParent));
		CHK_HR(pXMLDom->save(varFileName));
		return 1;
	}

CleanUp:
	SAFE_RELEASE(pXMLDom);
	SAFE_RELEASE(pNode);
	return 0;
}

QString DicomImageSet::getDTIFileName(const QString &datasetFolderName, const QString &queryString)
{
	QString datasetFileName = datasetFolderName+DATASET_FILE_NAME;
	HRESULT hr = S_OK;
	IXMLDOMDocument *pXMLDom = NULL;
	IXMLDOMNode *pFileNode = NULL;
	VARIANT_BOOL varStatus;
	VARIANT varFileName;
	BSTR bstrQuery;

	QStringList fileList;

	CHK_HR(CreateAndInitDOM(&pXMLDom));

	CHK_HR(VariantFromString(datasetFileName.toStdWString().c_str(), varFileName));
	CHK_HR(pXMLDom->load(varFileName, &varStatus));
	if (varStatus != VARIANT_TRUE)
	{
		CHK_HR(ReportParseError(pXMLDom, "Failed to load DOM from stocks.xml."));
	}

	bstrQuery = SysAllocString(QString(queryString+"/DTI").toStdWString().c_str());
	CHK_ALLOC(bstrQuery);
	CHK_HR(pXMLDom->selectSingleNode(bstrQuery, &pFileNode));
	if (pFileNode)
	{
		VARIANT varValue;
		CHK_HR(GetAttributeFromNode(pFileNode, L"file_name", &varValue));
		return QString::fromWCharArray(_bstr_t(varValue));
	}

CleanUp:
	SAFE_RELEASE(pXMLDom);
	SAFE_RELEASE(pFileNode);
	return QString();
}