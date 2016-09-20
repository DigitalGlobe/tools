#include "XercesRoutines.h"
#include<iostream>
#include<vector>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <time.h>
#include <string>
#include <sstream>
 
using namespace std;
//below macro gives all the required namespaces for Xerces
XERCES_CPP_NAMESPACE_USE

void Serialiser_XML_writer(xercesc::DOMDocument* pDOMDocument, char* FullFilePath);

int XercesRoutines::XercesTest()
{
	XMLPlatformUtils::Initialize();

	// Pointer to our DOMImplementation.
	DOMImplementation * pDOMImplementation = NULL;

	// Get the DOM Implementation (used for creating DOMDocuments).
	pDOMImplementation =
		DOMImplementationRegistry::getDOMImplementation(XMLString::transcode("core"));

	DOMDocument * pDOMDocument = NULL;

	pDOMDocument = pDOMImplementation->createDocument(L"schemas.example.com/2008/",
		L"ex:Hello_World", 0);


	DOMElement * pRootElement = NULL;
	pRootElement = pDOMDocument->getDocumentElement();


	// Create a Comment node, and then append this to the root element.
	DOMComment * pDOMComment = NULL;
	pDOMComment = pDOMDocument->createComment(L"Dates are formatted mm/dd/yy."
		L" Don't forget XML is case-sensitive.");
	pRootElement->appendChild(pDOMComment);


	// Create an Element node, then fill in some attributes,
	// and append this to the root element.
	DOMElement * pDataElement = NULL;
	pDataElement = pDOMDocument->createElement(L"data");

	// Copy the current system date to a buffer, then set/create the attribute.
	wchar_t wcharBuffer[128];
	_wstrdate_s(wcharBuffer, 9);
	pDataElement->setAttribute(L"date", wcharBuffer);

	// Convert an integer to a string, then set/create the attribute.
	_itow_s(65536, wcharBuffer, 128, 10);
	pDataElement->setAttribute(L"integer", wcharBuffer);

	// Convert a floating-point number to a wstring,
	// then set/create the attribute.
	std::wstringstream    myStream;
	myStream.precision(8);
	myStream.setf(std::ios_base::fixed, std::ios_base::floatfield);
	myStream << 3.1415926535897932384626433832795;
	const std::wstring ws(myStream.str());
	pDataElement->setAttribute(L"float", ws.c_str());

	// Append 'pDataElement' to 'pRootElement'.
	pRootElement->appendChild(pDataElement);


	// Create an Element node, then fill in some attributes, add some text,
	// then append this to the 'pDataElement' element.
	DOMElement * pRow = NULL;
	pRow = pDOMDocument->createElement(L"tow");

	// Create some sample data.
	_itow_s(1, wcharBuffer, 128, 10);
	pRow->setAttribute(L"index", wcharBuffer);

	/*
	Create a text node and append this as well. Some people
	prefer to place their data in the text node
	which is perfectly valid, others prefer to use
	the attributes. A combination of the two is quite common.
	*/
	DOMText* pTextNode = NULL;
	pTextNode = pDOMDocument->createTextNode(L"Comments and"
		L" data can also go in text nodes.");
	pRow->appendChild(pTextNode);
	pDataElement->appendChild(pRow);


	// Create a new  row (this time putting data and descriptions into different places).
	pRow = pDOMDocument->createElement(L"ConstantPI");
	pRow->setAttribute(L"description", L"The value of PI");
	pTextNode = pDOMDocument->createTextNode(L"3.1415");
	pRow->appendChild(pTextNode);
	pDataElement->appendChild(pRow);


	// Create a new row.
	pRow = pDOMDocument->createElement(L"UsefulLinks");
	pRow->setAttribute(L"website", L"http://www.programminggeeksinchrysalis.blogspot.com/");
	pTextNode = pDOMDocument->createTextNode(L"Blog worth Reading.");
	pRow->appendChild(pTextNode);
	pDataElement->appendChild(pRow);
	char* path = "c:\\temp\\sample.xml";
	Serialiser_XML_writer(pDOMDocument, path);

	//Release The Document after the xml file has been written
	pDOMDocument->release();
	XMLPlatformUtils::Terminate();

	return 0;
}

void Serialiser_XML_writer(xercesc::DOMDocument* pDOMDocument, char* FullFilePath)
{
	DOMImplementation *pImplement = NULL;
	DOMLSSerializer *pSerializer = NULL; // @DOMWriter
	LocalFileFormatTarget *pTarget = NULL;

	//Return the first registered implementation that has the desired features. In this case, we are after
	//a DOM implementation that has the LS feature... or Load/Save.
	pImplement = DOMImplementationRegistry::getDOMImplementation(L"LS");

	//From the DOMImplementation, create a DOMWriter.
	//DOMWriters are used to serialize a DOM tree [back] into an XML document.
	pSerializer = ((DOMImplementationLS*)pImplement)->createLSSerializer();
	DOMLSOutput *pOutput = ((DOMImplementationLS*)pImplement)->createLSOutput();
	DOMConfiguration *pConfiguration = pSerializer->getDomConfig();

	// Have a nice output
	if (pConfiguration->canSetParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true))
		pConfiguration->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
	pTarget = new LocalFileFormatTarget(FullFilePath);
	pOutput->setByteStream(pTarget);

	pSerializer->write(pDOMDocument, pOutput);

	delete pTarget;
	pOutput->release();
	pSerializer->release();
}
