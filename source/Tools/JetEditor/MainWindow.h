#pragma once

#include "wx/wx.h"
#include "wx/aui/aui.h"

class JetEditorMainFrame : public wxFrame
{
public:
	JetEditorMainFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE | wxSUNKEN_BORDER);
	virtual ~JetEditorMainFrame();

	void OnExit(wxCommandEvent& event);

protected:
	wxAuiManager m_AUIManager;
	wxAuiNotebook *m_pLevelNB;

private:
	wxAuiNotebook *CreateLevelDataView();

	wxDECLARE_EVENT_TABLE();
};
