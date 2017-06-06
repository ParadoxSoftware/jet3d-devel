/*
	@file MainWindow.h
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
#pragma once

#include "wx/wx.h"
#include "wx/aui/aui.h"
#include "wx/splitter.h"
#include "wx/propgrid/propgrid.h"

class ActorStudioMainFrame : public wxFrame
{
public:
	ActorStudioMainFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER);
	virtual ~ActorStudioMainFrame();

	void OnFileOpen(wxCommandEvent& event);
	void OnFileClose(wxCommandEvent& event);
	void OnFileSave(wxCommandEvent& event);
	void OnFileSaveAs(wxCommandEvent& event);
	void OnFileExit(wxCommandEvent& event);

	void OnUpdateUI(wxUpdateUIEvent &event);

protected:
	wxAuiManager m_AUIManager;
	wxAuiNotebook *m_pLevelNB;
	wxMenu *m_pObjectMenu;

	enum
	{
		ID_FILE_PROPERTIES = wxID_HIGHEST + 1,
		ID_FILE_IMPORT,
		ID_FILE_PREFERENCES,

		ID_EDIT_CLONESELECTED,
		ID_EDIT_SELECTALL,
		ID_EDIT_SELECTNONE,
		ID_EDIT_SELECTINVERT,

		ID_ACTOR_COMPILE,
		ID_ACTOR_DECOMPILE
	};

private:
	wxAuiNotebook *CreateBrowserView();
	void CreateModelView();
	wxPropertyGrid *CreatePropertyView();

	void SetupMenus();

	wxDECLARE_EVENT_TABLE();
};
