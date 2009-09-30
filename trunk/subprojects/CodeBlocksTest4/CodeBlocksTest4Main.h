/***************************************************************
 * Name:      CodeBlocksTest4Main.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2009-09-18
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef CODEBLOCKSTEST4MAIN_H
#define CODEBLOCKSTEST4MAIN_H

//(*Headers(CodeBlocksTest4Dialog)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class CodeBlocksTest4Dialog: public wxDialog
{
    public:

        CodeBlocksTest4Dialog(wxWindow* parent,wxWindowID id = -1);
        virtual ~CodeBlocksTest4Dialog();

    private:

        //(*Handlers(CodeBlocksTest4Dialog)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        //*)

        //(*Identifiers(CodeBlocksTest4Dialog)
        static const long ID_STATICTEXT1;
        static const long ID_BUTTON1;
        static const long ID_STATICLINE1;
        static const long ID_BUTTON2;
        //*)

        //(*Declarations(CodeBlocksTest4Dialog)
        wxButton* Button1;
        wxStaticText* StaticText1;
        wxBoxSizer* BoxSizer2;
        wxButton* Button2;
        wxStaticLine* StaticLine1;
        wxBoxSizer* BoxSizer1;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // CODEBLOCKSTEST4MAIN_H
