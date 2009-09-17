/***************************************************************
 * Name:      CodeBlocksTestMain.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2009-09-17
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef CODEBLOCKSTESTMAIN_H
#define CODEBLOCKSTESTMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "CodeBlocksTestApp.h"


#include <wx/button.h>
#include <wx/statline.h>
class CodeBlocksTestDialog: public wxDialog
{
    public:
        CodeBlocksTestDialog(wxDialog *dlg, const wxString& title);
        ~CodeBlocksTestDialog();

    protected:
        enum
        {
            idBtnQuit = 1000,
            idBtnAbout
        };
        wxStaticText* m_staticText1;
        wxButton* BtnAbout;
        wxStaticLine* m_staticline1;
        wxButton* BtnQuit;

    private:
        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        DECLARE_EVENT_TABLE()
};

#endif // CODEBLOCKSTESTMAIN_H
