#include "XMLHelper.h"

// Helper function to create a VT_BSTR variant from a null terminated string. 
HRESULT XMLHelper::VariantFromString(PCWSTR wszValue, VARIANT &Variant)
{
    HRESULT hr = S_OK;
    BSTR bstr = SysAllocString(wszValue);
    CHK_ALLOC(bstr);
    
    V_VT(&Variant)   = VT_BSTR;
    V_BSTR(&Variant) = bstr;

CleanUp:
    return hr;
}

// Helper function to create a DOM instance. 
HRESULT XMLHelper::CreateAndInitDOM(IXMLDOMDocument **ppDoc)
{
    HRESULT hr = CoCreateInstance(__uuidof(DOMDocument60), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(ppDoc));
    if (SUCCEEDED(hr))
    {
        // these methods should not fail so don't inspect result
        (*ppDoc)->put_async(VARIANT_FALSE);  
        (*ppDoc)->put_validateOnParse(VARIANT_FALSE);
        (*ppDoc)->put_resolveExternals(VARIANT_FALSE);
        (*ppDoc)->put_preserveWhiteSpace(VARIANT_TRUE);
    }
    return hr;
}

// Helper that allocates the BSTR param for the caller.
HRESULT XMLHelper::CreateElement(IXMLDOMDocument *pXMLDom, PCWSTR wszName, IXMLDOMElement **ppElement)
{
    HRESULT hr = S_OK;
    *ppElement = NULL;

    BSTR bstrName = SysAllocString(wszName);
    CHK_ALLOC(bstrName);
    CHK_HR(pXMLDom->createElement(bstrName, ppElement));

CleanUp:
    SysFreeString(bstrName);
    return hr;
}

// Helper function to append a child to a parent node.
HRESULT XMLHelper::AppendChildToParent(IXMLDOMNode *pChild, IXMLDOMNode *pParent)
{
    HRESULT hr = S_OK;
    IXMLDOMNode *pChildOut = NULL;
    CHK_HR(pParent->appendChild(pChild, &pChildOut));

CleanUp:
    SAFE_RELEASE(pChildOut);
    return hr;
}

// Helper function to append a child to a parent node.
HRESULT XMLHelper::InsertChildToParent(IXMLDOMNode *pChild, IXMLDOMNode *pRefChild, IXMLDOMNode *pParent)
{
    HRESULT hr = S_OK;
    IXMLDOMNode *pChildOut = NULL;
	_variant_t varSiblingNodeRef = pRefChild;
    CHK_HR(pParent->insertBefore(pChild, varSiblingNodeRef, &pChildOut));

CleanUp:
    SAFE_RELEASE(pChildOut);
    return hr;
}

// Helper function to create and add a processing instruction to a document node.
HRESULT XMLHelper::CreateAndAddPINode(IXMLDOMDocument *pDom, PCWSTR wszTarget, PCWSTR wszData)
{
    HRESULT hr = S_OK;
    IXMLDOMProcessingInstruction *pPI = NULL;

    BSTR bstrTarget = SysAllocString(wszTarget);
    BSTR bstrData = SysAllocString(wszData);
    CHK_ALLOC(bstrTarget && bstrData);
    
    CHK_HR(pDom->createProcessingInstruction(bstrTarget, bstrData, &pPI));
    CHK_HR(AppendChildToParent(pPI, pDom));

CleanUp:
    SAFE_RELEASE(pPI);
    SysFreeString(bstrTarget);
    SysFreeString(bstrData);
    return hr;
}

// Helper function to create and add a comment to a document node.
HRESULT XMLHelper::CreateAndAddCommentNode(IXMLDOMDocument *pDom, PCWSTR wszComment)
{
    HRESULT hr = S_OK;
    IXMLDOMComment *pComment = NULL;

    BSTR bstrComment = SysAllocString(wszComment);
    CHK_ALLOC(bstrComment);
    
    CHK_HR(pDom->createComment(bstrComment, &pComment));
    CHK_HR(AppendChildToParent(pComment, pDom));

CleanUp:
    SAFE_RELEASE(pComment);
    SysFreeString(bstrComment);
    return hr;
}

// Helper function to create and add an attribute to a parent node.
HRESULT XMLHelper::CreateAndAddAttributeNode(IXMLDOMDocument *pDom, PCWSTR wszName, PCWSTR wszValue, IXMLDOMElement *pParent)
{
    HRESULT hr = S_OK;
    IXMLDOMAttribute *pAttribute = NULL;
    IXMLDOMAttribute *pAttributeOut = NULL; // Out param that is not used

    BSTR bstrName = NULL;
    VARIANT varValue;
    VariantInit(&varValue);

    bstrName = SysAllocString(wszName);
    CHK_ALLOC(bstrName);
    CHK_HR(VariantFromString(wszValue, varValue));

    CHK_HR(pDom->createAttribute(bstrName, &pAttribute));
    CHK_HR(pAttribute->put_value(varValue));
    CHK_HR(pParent->setAttributeNode(pAttribute, &pAttributeOut));

CleanUp:
    SAFE_RELEASE(pAttribute);
    SAFE_RELEASE(pAttributeOut);
    SysFreeString(bstrName);
    VariantClear(&varValue);
    return hr;
}

HRESULT XMLHelper::SetAttribute(PCWSTR wszName, PCWSTR wszValue, IXMLDOMElement *pParent)
{
	HRESULT hr = S_OK;

	BSTR bstrName = NULL;
	VARIANT varValue;
	VariantInit(&varValue);

	bstrName = SysAllocString(wszName);
	CHK_ALLOC(bstrName);
	CHK_HR(VariantFromString(wszValue, varValue));

	CHK_HR(pParent->setAttribute(bstrName, varValue));

CleanUp:
	SysFreeString(bstrName);
	VariantClear(&varValue);
	return hr;
}

// Helper function to create and append a text node to a parent node.
HRESULT XMLHelper::CreateAndAddTextNode(IXMLDOMDocument *pDom, PCWSTR wszText, IXMLDOMNode *pParent)
{
    HRESULT hr = S_OK;    
    IXMLDOMText *pText = NULL;

    BSTR bstrText = SysAllocString(wszText);
    CHK_ALLOC(bstrText);

    CHK_HR(pDom->createTextNode(bstrText, &pText));
    CHK_HR(AppendChildToParent(pText, pParent));

CleanUp:
    SAFE_RELEASE(pText);
    SysFreeString(bstrText);
    return hr;
}

// Helper function to create and append a CDATA node to a parent node.
HRESULT XMLHelper::CreateAndAddCDATANode(IXMLDOMDocument *pDom, PCWSTR wszCDATA, IXMLDOMNode *pParent)
{
    HRESULT hr = S_OK;
    IXMLDOMCDATASection *pCDATA = NULL;

    BSTR bstrCDATA = SysAllocString(wszCDATA);
    CHK_ALLOC(bstrCDATA);

    CHK_HR(pDom->createCDATASection(bstrCDATA, &pCDATA));
    CHK_HR(AppendChildToParent(pCDATA, pParent));

CleanUp:
    SAFE_RELEASE(pCDATA);
    SysFreeString(bstrCDATA);
    return hr;
}

// Helper function to create and append an element node to a parent node, and pass the newly created
// element node to caller if it wants.
HRESULT XMLHelper::CreateAndAddElementNode(IXMLDOMDocument *pDom, PCWSTR wszName, PCWSTR wszNewline, IXMLDOMNode *pParent, IXMLDOMElement **ppElement)
{
    HRESULT hr = S_OK;
    IXMLDOMElement* pElement = NULL;

    CHK_HR(CreateElement(pDom, wszName, &pElement));
    // Add NEWLINE+TAB for identation before this element.
    CHK_HR(CreateAndAddTextNode(pDom, wszNewline, pParent));
    // Append this element to parent.
    CHK_HR(AppendChildToParent(pElement, pParent));

CleanUp:
    if (ppElement)
        *ppElement = pElement;  // Caller is repsonsible to release this element.
    else
        SAFE_RELEASE(pElement); // Caller is not interested on this element, so release it.

    return hr;
}

// Helper function to display parse error.
// It returns error code of the parse error.
HRESULT XMLHelper::ReportParseError(IXMLDOMDocument *pDoc, char *szDesc)
{
    HRESULT hr = S_OK;
    HRESULT hrRet = E_FAIL; // Default error code if failed to get from parse error.
    IXMLDOMParseError *pXMLErr = NULL;
    BSTR bstrReason = NULL;

    CHK_HR(pDoc->get_parseError(&pXMLErr));
    CHK_HR(pXMLErr->get_errorCode(&hrRet));
    CHK_HR(pXMLErr->get_reason(&bstrReason));
    printf("%s\n%S\n", szDesc, bstrReason);

CleanUp:
    SAFE_RELEASE(pXMLErr);
    SysFreeString(bstrReason);
    return hrRet;
}

HRESULT XMLHelper::GetAttributeFromNode(IXMLDOMNode *pNode, BSTR bstrAttrName, VARIANT * varAttrValue)
{	
    HRESULT hr = S_OK;
	IXMLDOMNamedNodeMap *pAttributes = NULL;
    IXMLDOMNode *pAttrNode = NULL;

	CHK_HR(pNode->get_attributes(&pAttributes));
	if (pAttributes)
	{
		CHK_HR(pAttributes->getNamedItem(bstrAttrName, &pAttrNode));
		if (pAttrNode)
		{
			CHK_HR(pAttrNode->get_nodeValue(varAttrValue));
		} 
	}
	else
	{
		hr = S_FALSE;
	}

CleanUp:
	SAFE_RELEASE(pAttributes);
	SAFE_RELEASE(pAttrNode);

	return hr;
}
HRESULT XMLHelper::CreateAndAddTextNodeBefore(IXMLDOMDocument *pDom, PCWSTR wszText, IXMLDOMNode *pParent, IXMLDOMNode *pPosition)
{
    HRESULT hr = S_OK;    
    IXMLDOMText *pText = NULL;

    BSTR bstrText = SysAllocString(wszText);
    CHK_ALLOC(bstrText);

    CHK_HR(pDom->createTextNode(bstrText, &pText));
    CHK_HR(InsertChildToParent(pText, pPosition, pParent));

CleanUp:
    SAFE_RELEASE(pText);
    SysFreeString(bstrText);
    return hr;
}