/*
	@file MainWindow.cpp
	@author paradoxnj
	@brief The editor's main window

	@par license
	The contents of this file are subject to the Jet3D Public License
	Version 1.02 (the "License"); you may not use this file except in
	compliance with the License. You may obtain a copy of the License at
	http://www.jet3d.com

	@par
	Software distributed under the License is distributed on an "AS IS"
	basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
	the License for the specific language governing rights and limitations
	under the License.

	@par
	The Original Code is Jet3D, released December 12, 1999.
	Copyright (C) 1996-1999 Eclipse Entertainment, L.L.C. All Rights Reserved
*/

#include "MainWindow.h"
#include "wx/wx.h"
#include "wx/artprov.h"

wxBEGIN_EVENT_TABLE(ActorStudioMainFrame, wxFrame)
EVT_MENU(wxID_OPEN, ActorStudioMainFrame::OnFileOpen)
EVT_MENU(wxID_CLOSE, ActorStudioMainFrame::OnFileClose)
EVT_MENU(wxID_SAVE, ActorStudioMainFrame::OnFileSave)
EVT_MENU(wxID_SAVEAS, ActorStudioMainFrame::OnFileSaveAs)
EVT_MENU(wxID_EXIT, ActorStudioMainFrame::OnFileExit)
wxEND_EVENT_TABLE()

ActorStudioMainFrame::ActorStudioMainFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style)
: wxFrame(parent, id, title, pos, size, style)
{
	m_AUIManager.SetManagedWindow(this);

	// Set the app icon
	SetIcon(wxIcon("icons\\AStudio.ico", wxBITMAP_TYPE_ICO));

	// Setup the menus
	SetupMenus();

	// Setup material and level data view
	m_AUIManager.AddPane(CreateBrowserView(), wxAuiPaneInfo().Caption(wxT("Browsers")).Left().LeftDockable().Dock().CloseButton(false).MaximizeButton(false).MinSize(wxSize(300, 200)));

	// Setup CAD View
	m_AUIManager.AddPane(CreateModelView(), wxAuiPaneInfo().Caption(wxT("Model View")).Center().CloseButton(false).MaximizeButton(false));

	// Setup properties view
	m_AUIManager.AddPane(CreatePropertyView(), wxAuiPaneInfo().Caption(wxT("Property View")).Right().RightDockable().Dock().CloseButton(false).MaximizeButton(false).MinSize(wxSize(300, 200)));

	// Update the AUI Manager
	m_AUIManager.Update();

	this->Maximize();
}

ActorStudioMainFrame::~ActorStudioMainFrame()
{
	m_AUIManager.UnInit();
}

void ActorStudioMainFrame::OnFileOpen(wxCommandEvent& WXUNUSED(event))
{

}

void ActorStudioMainFrame::OnFileClose(wxCommandEvent& WXUNUSED(event))
{

}

void ActorStudioMainFrame::OnFileSave(wxCommandEvent& WXUNUSED(event))
{

}

void ActorStudioMainFrame::OnFileSaveAs(wxCommandEvent& WXUNUSED(event))
{

}

void ActorStudioMainFrame::OnFileExit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

wxAuiNotebook *ActorStudioMainFrame::CreateBrowserView()
{
	wxSize clientSize = GetClientSize();
	wxAuiNotebook *ctrl = new wxAuiNotebook(this, wxID_ANY, wxPoint(clientSize.x, clientSize.y), wxSize(500, 500), wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER);

	ctrl->Freeze();

	ctrl->AddPage(new wxPanel(ctrl, wxID_ANY), wxT("Actor"), false, NULL);
	ctrl->SetPageToolTip(0, wxT("Actor"));

	ctrl->Thaw();
	return ctrl;
}

void ActorStudioMainFrame::CreateModelView()
{

}

wxPropertyGrid *ActorStudioMainFrame::CreatePropertyView()
{
	wxPropertyGrid *pPropGrid = new wxPropertyGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_AUTO_SORT | wxPG_SPLITTER_AUTO_CENTER | wxPG_DEFAULT_STYLE);
	return pPropGrid;
}

void ActorStudioMainFrame::SetupMenus()
{
	wxMenuBar *pMenuBar = new wxMenuBar;

	wxMenu *pFileMenu = new wxMenu;
	pFileMenu->Append(wxID_NEW, wxT("&New"));
	pFileMenu->Append(wxID_OPEN, wxT("&Open..."));
	pFileMenu->Append(wxID_CLOSE, wxT("&Close"));
	pFileMenu->Append(wxID_SAVE, wxT("&Save"));
	pFileMenu->Append(wxID_SAVEAS, wxT("Save &As..."));
	pFileMenu->AppendSeparator();
	pFileMenu->Append(ID_FILE_IMPORT, wxT("&Import..."));
	pFileMenu->AppendSeparator();
	pFileMenu->Append(wxID_EXIT, wxT("E&xit"));

	wxMenu *pEditMenu = new wxMenu;
	pEditMenu->Append(wxID_UNDO, wxT("&Undo"));
	pEditMenu->Append(wxID_REDO, wxT("&Redo"));
	pEditMenu->AppendSeparator();
	pEditMenu->Append(wxID_CUT, wxT("Cu&t"));
	pEditMenu->Append(wxID_COPY, wxT("&Copy"));
	pEditMenu->Append(wxID_PASTE, wxT("&Paste"));
	pEditMenu->AppendSeparator();
	pEditMenu->Append(wxID_DELETE, wxT("&Delete"));
	pEditMenu->AppendSeparator();
	pEditMenu->Append(ID_EDIT_SELECTALL, wxT("Select &All"));
	pEditMenu->Append(ID_EDIT_SELECTNONE, wxT("Select None"));
	pEditMenu->Append(ID_EDIT_SELECTINVERT, wxT("Select Invert"));
	pEditMenu->Append(ID_EDIT_CLONESELECTED, wxT("Clone &Selected"));

	wxMenu *pActorMenu = new wxMenu;
	pActorMenu->Append(ID_ACTOR_COMPILE, wxT("&Compile..."));
	pActorMenu->Append(ID_ACTOR_DECOMPILE, wxT("&Decompile..."));

	wxMenu *pHelpMenu = new wxMenu;
	pHelpMenu->Append(wxID_ABOUT, wxT("About Jet Editor"));

	pMenuBar->Append(pFileMenu, _("&File"));
	pMenuBar->Append(pEditMenu, _("&Edit"));
	pMenuBar->Append(pActorMenu, _("&Actor"));
	pMenuBar->Append(pHelpMenu, _("&Help"));

	SetMenuBar(pMenuBar);
}

void ActorStudioMainFrame::OnUpdateUI(wxUpdateUIEvent &event)
{

}