#include "MainWindow.h"
#include "wx/wx.h"
#include "wx/artprov.h"
#include "wx/splitter.h"

wxBEGIN_EVENT_TABLE(JetEditorMainFrame, wxFrame)
	EVT_MENU(wxID_EXIT, JetEditorMainFrame::OnExit)
wxEND_EVENT_TABLE()

JetEditorMainFrame::JetEditorMainFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style)
	: wxFrame(parent, id, title, pos, size, style)
{
	m_AUIManager.SetManagedWindow(this);
	
	wxMenuBar *pMenuBar = new wxMenuBar;

	wxMenu *pFileMenu = new wxMenu;
	pFileMenu->Append(wxID_EXIT);

	pMenuBar->Append(pFileMenu, _("&File"));
	SetMenuBar(pMenuBar);

	// Setup material and level data view
	m_AUIManager.AddPane(CreateLevelDataView(), wxAuiPaneInfo().Caption(wxT("Level Data View")).Left().LeftDockable().Dock().CloseButton(false).MaximizeButton(false).MinSize(wxSize(300, 200)));
	
	// Setup CAD View
	wxSplitterWindow *pHorizontalSplit = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D | wxSP_LIVE_UPDATE | wxCLIP_CHILDREN);
	wxSplitterWindow *pVerticalSplitTop = new wxSplitterWindow(pHorizontalSplit, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D | wxSP_LIVE_UPDATE | wxCLIP_CHILDREN);
	wxSplitterWindow *pVerticalSplitBottom = new wxSplitterWindow(pHorizontalSplit, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D | wxSP_LIVE_UPDATE | wxCLIP_CHILDREN);

	pHorizontalSplit->SetSashGravity(0.5);
	pHorizontalSplit->SplitHorizontally(pVerticalSplitTop, pVerticalSplitBottom);

	wxPanel *panelTopRight = new wxPanel(pVerticalSplitTop, wxID_ANY);
	wxPanel *panelTopLeft = new wxPanel(pVerticalSplitTop, wxID_ANY);
	wxPanel *panelBottomLeft = new wxPanel(pVerticalSplitBottom, wxID_ANY);
	wxPanel *panelBottomRight = new wxPanel(pVerticalSplitBottom, wxID_ANY);

	wxStaticText *ptxtTR = new wxStaticText(panelTopRight, wxID_ANY, wxT("Top Right"));
	wxStaticText *ptxtTL = new wxStaticText(panelTopLeft, wxID_ANY, wxT("Top Left"));
	wxStaticText *ptxtBR = new wxStaticText(panelBottomRight, wxID_ANY, wxT("Bottom Right"));
	wxStaticText *ptxtBL = new wxStaticText(panelBottomLeft, wxID_ANY, wxT("Bottom Left"));

	pVerticalSplitBottom->SetSashGravity(0.5);
	pVerticalSplitTop->SetSashGravity(0.5);
	
	pVerticalSplitTop->SplitVertically(panelTopLeft, panelTopRight);
	pVerticalSplitBottom->SplitVertically(panelBottomLeft, panelBottomRight);

	m_AUIManager.AddPane(pHorizontalSplit, wxAuiPaneInfo().Caption(wxT("CAD View")).Center().CloseButton(false).MaximizeButton(false));

	// Setup properties view

	m_AUIManager.Update();

	this->Maximize();
}

JetEditorMainFrame::~JetEditorMainFrame()
{
	m_AUIManager.UnInit();
}

void JetEditorMainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

wxAuiNotebook *JetEditorMainFrame::CreateLevelDataView()
{
	wxSize clientSize = GetClientSize();

	wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16, 16));

	wxAuiNotebook *ctrl = new wxAuiNotebook(this, wxID_ANY, wxPoint(clientSize.x, clientSize.y), wxSize(500, 500), wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER);

	ctrl->Freeze();
	ctrl->AddPage(new wxPanel(ctrl, wxID_ANY), wxT("Materials"), false, page_bmp);
	ctrl->SetPageToolTip(0, wxT("Materials"));

	ctrl->Thaw();
	return ctrl;
}
