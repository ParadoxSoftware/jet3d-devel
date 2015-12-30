#include <wx/wx.h>
#include "MainWindow.h"

class JetEditorApp : public wxApp
{
public:
	bool OnInit() wxOVERRIDE;
};

wxDECLARE_APP(JetEditorApp);
wxIMPLEMENT_APP(JetEditorApp);

bool JetEditorApp::OnInit()
{
	if (!wxApp::OnInit())
		return false;

	JetEditorMainFrame *pFrame = new JetEditorMainFrame(NULL, wxID_ANY, _("Jet3D World Editor"));
	SetTopWindow(pFrame);
	pFrame->Show();

	return true;
}
