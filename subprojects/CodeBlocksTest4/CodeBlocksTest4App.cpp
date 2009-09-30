/***************************************************************
 * Name:      CodeBlocksTest4App.cpp
 * Purpose:   Code for Application Class
 * Author:     ()
 * Created:   2009-09-18
 * Copyright:  ()
 * License:
 **************************************************************/

#include "CodeBlocksTest4App.h"

//(*AppHeaders
#include "CodeBlocksTest4Main.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(CodeBlocksTest4App);

bool CodeBlocksTest4App::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	CodeBlocksTest4Dialog Dlg(0);
    	SetTopWindow(&Dlg);
    	Dlg.ShowModal();
    	wxsOK = false;
    }
    //*)
    return wxsOK;

}
