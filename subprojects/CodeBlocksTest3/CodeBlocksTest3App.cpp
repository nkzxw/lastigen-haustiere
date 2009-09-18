/***************************************************************
 * Name:      CodeBlocksTest3App.cpp
 * Purpose:   Code for Application Class
 * Author:     ()
 * Created:   2009-09-17
 * Copyright:  ()
 * License:
 **************************************************************/

#include "CodeBlocksTest3App.h"

//(*AppHeaders
#include "CodeBlocksTest3Main.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(CodeBlocksTest3App);

bool CodeBlocksTest3App::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	CodeBlocksTest3Frame* Frame = new CodeBlocksTest3Frame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
