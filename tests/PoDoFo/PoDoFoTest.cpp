//-----------------------------------------------------------------------------
/* 
 * PoDoFoTest.cpp
 */
//-----------------------------------------------------------------------------

#include "PoDoFoTest.h"

#include <podofo/podofo.h>

using namespace PoDoFo;

//-----------------------------------------------------------------------------
// class macros

    CPPUNIT_TEST_SUITE_REGISTRATION(CPoDoFoTest);

//-----------------------------------------------------------------------------
/**
 * Sets up this fixture.
 */
void CPoDoFoTest::setUp()
{
}
//-----------------------------------------------------------------------------
/**
 * Tears down this fixture.
 */
void CPoDoFoTest::tearDown()
{
}
//-----------------------------------------------------------------------------
/**
 * Tests creating a PDF document.
 */
void CPoDoFoTest::testCreate()
{
    PdfMemDocument document;



#if (IGNORE)
    /*
     * PdfStreamedDocument is the class that can actually write a PDF file.
     * PdfStreamedDocument is much faster than PdfDocument, but it is only
     * suitable for creating/drawing PDF files and cannot modify existing
     * PDF documents.
     *
     * The document is written directly to pszFilename while being created.
     */
    PdfStreamedDocument document( pszFilename );

    /*
     * PdfPainter is the class which is able to draw text and graphics
     * directly on a PdfPage object.
     */
    PdfPainter painter;

    /*
     * This pointer will hold the page object later. 
     * PdfSimpleWriter can write several PdfPage's to a PDF file.
     */
    PdfPage* pPage;

    /*
     * A PdfFont object is required to draw text on a PdfPage using a PdfPainter.
     * PoDoFo will find the font using fontconfig on your system and embedd truetype
     * fonts automatically in the PDF file.
     */     
    PdfFont* pFont;

    try {
        /*
         * The PdfDocument object can be used to create new PdfPage objects.
         * The PdfPage object is owned by the PdfDocument will also be deleted automatically
         * by the PdfDocument object.
         *
         * You have to pass only one argument, i.e. the page size of the page to create.
         * There are predefined enums for some common page sizes.
         */
        pPage = document.CreatePage( PdfPage::CreateStandardPageSize( ePdfPageSize_A4 ) );

        /*
         * If the page cannot be created because of an error (e.g. ePdfError_OutOfMemory )
         * a NULL pointer is returned.
         * We check for a NULL pointer here and throw an exception using the RAISE_ERROR macro.
         * The raise error macro initializes a PdfError object with a given error code and
         * the location in the file in which the error ocurred and throws it as an exception.
         */
        if( !pPage ) 
        {
            PODOFO_RAISE_ERROR( ePdfError_InvalidHandle );
        }

        /*
         * Set the page as drawing target for the PdfPainter.
         * Before the painter can draw, a page has to be set first.
         */
        painter.SetPage( pPage );

        /*
         * Create a PdfFont object using the font "Arial".
         * The font is found on the system using fontconfig and embedded into the
         * PDF file. If Arial is not available, a default font will be used.
         *
         * The created PdfFont will be deleted by the PdfDocument.
         */
        pFont = document.CreateFont( "Arial" );
        
        /*
         * If the PdfFont object cannot be allocated return an error.
         */
        if( !pFont )
        {
            PODOFO_RAISE_ERROR( ePdfError_InvalidHandle );
        }

        /*
         * Set the font size
         */
        pFont->SetFontSize( 18.0 );

        /*
         * Set the font as default font for drawing.
         * A font has to be set before you can draw text on
         * a PdfPainter.
         */
        painter.SetFont( pFont );

        /*
         * You could set a different color than black to draw
         * the text.
         *
         * SAFE_OP( painter.SetColor( 1.0, 0.0, 0.0 ) );
         */

        /*
         * Actually draw the line "Hello World!" on to the PdfPage at
         * the position 2cm,2cm from the top left corner. 
         * Please remember that PDF files have their origin at the 
         * bottom left corner. Therefore we substract the y coordinate 
         * from the page height.
         * 
         * The position specifies the start of the baseline of the text.
         *
         * All coordinates in PoDoFo are in PDF units.
         * You can also use PdfPainterMM which takes coordinates in 1/1000th mm.
         *
         */
        painter.DrawText( 56.69, pPage->GetPageSize().GetHeight() - 56.69, "Hello World!" );

        /*
         * Tell PoDoFo that the page has been drawn completely.
         * This required to optimize drawing operations inside in PoDoFo
         * and has to be done whenever you are done with drawing a page.
         */
        painter.FinishPage();

        /*
         * Set some additional information on the PDF file.
         */
        document.GetInfo()->SetCreator ( PdfString("examplahelloworld - A PoDoFo test application") );
        document.GetInfo()->SetAuthor  ( PdfString("Dominik Seichter") );
        document.GetInfo()->SetTitle   ( PdfString("Hello World") );
        document.GetInfo()->SetSubject ( PdfString("Testing the PoDoFo PDF Library") );
        document.GetInfo()->SetKeywords( PdfString("Test;PDF;Hello World;") );

        /*
         * The last step is to close the document.
         */
        document.Close();
    } catch ( const PdfError & e ) {
        /*
         * All PoDoFo methods may throw exceptions
         * make sure that painter.FinishPage() is called
         * or who will get an assert in its destructor
         */
        try {
            painter.FinishPage();
        } catch( ... ) {
            /*
             * Ignore errors this time
             */
        }
#endif
}
//-----------------------------------------------------------------------------
