/***************************************************************
 * Name:      CodeBlocksTest2Main.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2009-09-17
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef CODEBLOCKSTEST2MAIN_H
#define CODEBLOCKSTEST2MAIN_H

//(*Headers(CodeBlocksTest2Dialog)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/listbox.h>
#include <wx/statline.h>
#include <wx/grid.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class CodeBlocksTest2Dialog: public wxDialog
{
    public:

        CodeBlocksTest2Dialog(wxWindow* parent,wxWindowID id = -1);
        virtual ~CodeBlocksTest2Dialog();

    private:

        //(*Handlers(CodeBlocksTest2Dialog)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        //*)

        //(*Identifiers(CodeBlocksTest2Dialog)
        static const long ID_STATICTEXT1;
        static const long ID_LISTBOX1;
        static const long ID_BUTTON1;
        static const long ID_STATICLINE1;
        static const long ID_BUTTON2;
        static const long ID_GRID1;
        //*)

        //(*Declarations(CodeBlocksTest2Dialog)
        wxButton* Button1;
        wxStaticText* StaticText1;
        wxBoxSizer* BoxSizer2;
        wxGrid* Grid1;
        wxButton* Button2;
        wxStaticLine* StaticLine1;
        wxBoxSizer* BoxSizer1;
        wxListBox* ListBox1;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // CODEBLOCKSTEST2MAIN_H
