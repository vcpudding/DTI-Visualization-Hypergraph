#include <stdio.h>
#include <tchar.h>
#include <msxml6.h>
#include <comutil.h>

#ifndef XML_HELPER_H
#define XML_HELPER_H

// Macro that calls a COM method returning HRESULT value.
#define CHK_HR(stmt)        do { hr=(stmt); if (FAILED(hr)) goto CleanUp; } while(0)

// Macro to verify memory allcation.
#define CHK_ALLOC(p)        do { if (!(p)) { hr = E_OUTOFMEMORY; goto CleanUp; } } while(0)

// Macro that releases a COM object if not NULL.
#define SAFE_RELEASE(p)     do { if ((p)) { (p)->Release(); (p) = NULL; } } while(0)

namespace XMLHelper
{
	// Helper function to create a VT_BSTR variant from a null terminated string. 
	HRESULT VariantFromString(PCWSTR wszValue, VARIANT &Variant);

	// Helper function to create a DOM instance. 
	HRESULT CreateAndInitDOM(IXMLDOMDocument **ppDoc);

	// Helper that allocates the BSTR param for the caller.
	HRESULT CreateElement(IXMLDOMDocument *pXMLDom, PCWSTR wszName, IXMLDOMElement **ppElement);

	// Helper function to append a child to a parent node.
	HRESULT AppendChildToParent(IXMLDOMNode *pChild, IXMLDOMNode *pParent);
	HRESULT InsertChildToParent(IXMLDOMNode *pChild, IXMLDOMNode *pRefChild, IXMLDOMNode *pParent);

	// Helper function to create and add a processing instruction to a document node.
	HRESULT CreateAndAddPINode(IXMLDOMDocument *pDom, PCWSTR wszTarget, PCWSTR wszData);

	// Helper function to create and add a comment to a document node.
	HRESULT CreateAndAddCommentNode(IXMLDOMDocument *pDom, PCWSTR wszComment);

	// Helper function to create and add an attribute to a parent node.
	HRESULT CreateAndAddAttributeNode(IXMLDOMDocument *pDom, PCWSTR wszName, PCWSTR wszValue, IXMLDOMElement *pParent);
	HRESULT SetAttribute(PCWSTR wszName, PCWSTR wszValue, IXMLDOMElement *pParent);

	// Helper function to create and append a text node to a parent node.
	HRESULT CreateAndAddTextNode(IXMLDOMDocument *pDom, PCWSTR wszText, IXMLDOMNode *pParent);

	// Helper function to create and insert a text node to a parent node before a specific node.
	HRESULT CreateAndAddTextNodeBefore(IXMLDOMDocument *pDom, PCWSTR wszText, IXMLDOMNode *pParent, IXMLDOMNode *pPosition);

	// Helper function to create and append a CDATA node to a parent node.
	HRESULT CreateAndAddCDATANode(IXMLDOMDocument *pDom, PCWSTR wszCDATA, IXMLDOMNode *pParent);

	// Helper function to create and append an element node to a parent node, and pass the newly created
	// element node to caller if it wants.
	HRESULT CreateAndAddElementNode(IXMLDOMDocument *pDom, PCWSTR wszName, PCWSTR wszNewline, IXMLDOMNode *pParent, IXMLDOMElement **ppElement = NULL);

	// Helper function to display parse error.
	// It returns error code of the parse error.
	HRESULT ReportParseError(IXMLDOMDocument *pDoc, char *szDesc);

	// Helper function to get attribute from a node.
	HRESULT GetAttributeFromNode(IXMLDOMNode *pNode, BSTR bstrAttrName, VARIANT * varAttrValue);
}

#endif