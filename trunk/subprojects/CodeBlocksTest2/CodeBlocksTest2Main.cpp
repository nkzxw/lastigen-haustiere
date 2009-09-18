/***************************************************************
 * Name:      CodeBlocksTest2Main.cpp
 * Purpose:   Code for Application Frame
 * Author:     ()
 * Created:   2009-09-17
 * Copyright:  ()
 * License:
 **************************************************************/

#include "CodeBlocksTest2Main.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(CodeBlocksTest2Dialog)
#include <wx/settings.h>
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(CodeBlocksTest2Dialog)
const long CodeBlocksTest2Dialog::ID_STATICTEXT1 = wxNewId();
const long CodeBlocksTest2Dialog::ID_LISTBOX1 = wxNewId();
const long CodeBlocksTest2Dialog::ID_BUTTON1 = wxNewId();
const long CodeBlocksTest2Dialog::ID_STATICLINE1 = wxNewId();
const long CodeBlocksTest2Dialog::ID_BUTTON2 = wxNewId();
const long CodeBlocksTest2Dialog::ID_GRID1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(CodeBlocksTest2Dialog,wxDialog)
    //(*EventTable(CodeBlocksTest2Dialog)
    //*)
END_EVENT_TABLE()

CodeBlocksTest2Dialog::CodeBlocksTest2Dialog(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(CodeBlocksTest2Dialog)
    Create(parent, wxID_ANY, _("wxWidgets app"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Welcome to\nwxWidgets"), wxDefaultPosition, wxSize(217,66), 0, _T("ID_STATICTEXT1"));
    wxFont StaticText1Font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    if ( !StaticText1Font.Ok() ) StaticText1Font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    StaticText1Font.SetPointSize(20);
    StaticText1->SetFont(StaticText1Font);
    BoxSizer1->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 10);
    ListBox1 = new wxListBox(this, ID_LISTBOX1, wxDefaultPosition, wxSize(79,84), 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX1"));
    ListBox1->Append(_("hola"));
    ListBox1->Append(_("como estas"));
    ListBox1->Append(_("todo bien"));
    ListBox1->Append(_("si, todo bien"));
    BoxSizer1->Add(ListBox1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer2 = new wxBoxSizer(wxVERTICAL);
    Button1 = new wxButton(this, ID_BUTTON1, _("Abouts"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    BoxSizer2->Add(Button1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
    BoxSizer2->Add(StaticLine1, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    Button2 = new wxButton(this, ID_BUTTON2, _("Quit"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    BoxSizer2->Add(Button2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    BoxSizer1->Add(BoxSizer2, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    Grid1 = new wxGrid(this, ID_GRID1, wxDefaultPosition, wxDefaultSize, 0, _T("ID_GRID1"));
    Grid1->CreateGrid(4,4);
    BoxSizer1->Add(Grid1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(BoxSizer1);
    BoxSizer1->Fit(this);
    BoxSizer1->SetSizeHints(this);

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CodeBlocksTest2Dialog::OnAbout);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CodeBlocksTest2Dialog::OnQuit);
    //*)
}

CodeBlocksTest2Dialog::~CodeBlocksTest2Dialog()
{
    //(*Destroy(CodeBlocksTest2Dialog)
    //*)
}

void CodeBlocksTest2Dialog::OnQuit(wxCommandEvent& event)
{
    Close();
}

void CodeBlocksTest2Dialog::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}
