#include <wx/wx.h>

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

	return true;
}
