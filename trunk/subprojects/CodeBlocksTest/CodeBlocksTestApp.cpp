/***************************************************************
 * Name:      CodeBlocksTestApp.cpp
 * Purpose:   Code for Application Class
 * Author:     ()
 * Created:   2009-09-17
 * Copyright:  ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "CodeBlocksTestApp.h"
#include "CodeBlocksTestMain.h"

//(*AppHeaders
#include <wx/image.h>
//*)

IMPLEMENT_APP(CodeBlocksTestApp);

bool CodeBlocksTestApp::OnInit()
{
	//(*AppInitialize
	bool wxsOK = true;
	wxInitAllImageHandlers();
	//*)
	return wxsOK;
	
    
    CodeBlocksTestDialog* dlg = new CodeBlocksTestDialog(0L, _("wxWidgets Application Template"));
    dlg->SetIcon(wxICON(aaaa)); // To Set App Icon
    dlg->Show();
    return true;
}
