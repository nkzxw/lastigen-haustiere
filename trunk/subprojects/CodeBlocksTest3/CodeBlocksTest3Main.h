/***************************************************************
 * Name:      CodeBlocksTest3Main.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2009-09-17
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef CODEBLOCKSTEST3MAIN_H
#define CODEBLOCKSTEST3MAIN_H

//(*Headers(CodeBlocksTest3Frame)
#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
//*)

class CodeBlocksTest3Frame: public wxFrame
{
    public:

        CodeBlocksTest3Frame(wxWindow* parent,wxWindowID id = -1);
        virtual ~CodeBlocksTest3Frame();

    private:

        //(*Handlers(CodeBlocksTest3Frame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        //*)

        //(*Identifiers(CodeBlocksTest3Frame)
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(CodeBlocksTest3Frame)
        wxStatusBar* StatusBar1;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // CODEBLOCKSTEST3MAIN_H
