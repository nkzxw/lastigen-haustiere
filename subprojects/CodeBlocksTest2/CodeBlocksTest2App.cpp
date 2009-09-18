/***************************************************************
 * Name:      CodeBlocksTest2App.cpp
 * Purpose:   Code for Application Class
 * Author:     ()
 * Created:   2009-09-17
 * Copyright:  ()
 * License:
 **************************************************************/

#include "CodeBlocksTest2App.h"

//(*AppHeaders
#include "CodeBlocksTest2Main.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(CodeBlocksTest2App);

bool CodeBlocksTest2App::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	CodeBlocksTest2Dialog Dlg(0);
    	SetTopWindow(&Dlg);
    	Dlg.ShowModal();
    	wxsOK = false;
    }
    //*)
    return wxsOK;

}
