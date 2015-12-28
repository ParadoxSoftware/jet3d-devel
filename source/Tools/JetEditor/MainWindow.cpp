#include "MainWindow.h"
#include "wx/wx.h"

wxBEGIN_EVENT_TABLE(JetEditorMainFrame, wxFrame)
	EVT_MENU(wxID_EXIT, JetEditorMainFrame::OnExit)
wxEND_EVENT_TABLE()

JetEditorMainFrame::JetEditorMainFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style)
	: wxFrame(parent, id, title, pos, size, style)
{
	m_AUIManager.SetManagedWindow(this);
	this->ShowFullScreen(true);

	wxMenuBar *pMenuBar = new wxMenuBar;

	wxMenu *pFileMenu = new wxMenu;
	pFileMenu->Append(wxID_EXIT);

	pMenuBar->Append(pFileMenu, _("&File"));
	SetMenuBar(pMenuBar);
}

JetEditorMainFrame::~JetEditorMainFrame()
{

}

void JetEditorMainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}
