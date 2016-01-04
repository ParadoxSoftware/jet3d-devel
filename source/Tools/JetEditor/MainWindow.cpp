#include "MainWindow.h"
#include "wx/wx.h"
#include "wx/artprov.h"

wxBEGIN_EVENT_TABLE(JetEditorMainFrame, wxFrame)
	EVT_MENU(ID_MODE_MOVESCALE, JetEditorMainFrame::OnMode)
	EVT_MENU(ID_MODE_ROTATESHEAR, JetEditorMainFrame::OnMode)
	EVT_MENU(ID_MODE_VERTEXMANIPULATION, JetEditorMainFrame::OnMode)
	EVT_MENU(ID_VIEW_SNAPTOGRID, JetEditorMainFrame::OnSnapToGrid)
	EVT_UPDATE_UI(ID_MODE_MOVESCALE, JetEditorMainFrame::OnUpdateUI)
	EVT_UPDATE_UI(ID_MODE_ROTATESHEAR, JetEditorMainFrame::OnUpdateUI)
	EVT_UPDATE_UI(ID_MODE_VERTEXMANIPULATION, JetEditorMainFrame::OnUpdateUI)
	EVT_UPDATE_UI(ID_VIEW_SNAPTOGRID, JetEditorMainFrame::OnUpdateUI)
	EVT_MENU(wxID_EXIT, JetEditorMainFrame::OnExit)
wxEND_EVENT_TABLE()

JetEditorMainFrame::JetEditorMainFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style)
	: wxFrame(parent, id, title, pos, size, style)
{
	m_AUIManager.SetManagedWindow(this);
	
	// Set the app icon
	SetIcon(wxIcon("icons\\jwe.ico", wxBITMAP_TYPE_ICO));

	// Setup the menus
	SetupMenus();

	// Setup material and level data view
	m_AUIManager.AddPane(CreateBrowserView(), wxAuiPaneInfo().Caption(wxT("Browsers")).Left().LeftDockable().Dock().CloseButton(false).MaximizeButton(false).MinSize(wxSize(300, 200)));
	
	// Setup CAD View
	m_AUIManager.AddPane(CreateCADView(), wxAuiPaneInfo().Caption(wxT("CAD View")).Center().CloseButton(false).MaximizeButton(false));

	// Setup properties view
	m_AUIManager.AddPane(CreatePropertyView(), wxAuiPaneInfo().Caption(wxT("Property View")).Right().RightDockable().Dock().CloseButton(false).MaximizeButton(false).MinSize(wxSize(300, 200)));

	// Update the AUI Manager
	m_AUIManager.Update();

	this->Maximize();
}

JetEditorMainFrame::~JetEditorMainFrame()
{
	m_AUIManager.UnInit();
}

void JetEditorMainFrame::OnMode(wxCommandEvent& event)
{

}

void JetEditorMainFrame::OnSnapToGrid(wxCommandEvent& event)
{

}

void JetEditorMainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

wxAuiNotebook *JetEditorMainFrame::CreateBrowserView()
{
	wxSize clientSize = GetClientSize();
	wxAuiNotebook *ctrl = new wxAuiNotebook(this, wxID_ANY, wxPoint(clientSize.x, clientSize.y), wxSize(500, 500), wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxNO_BORDER);

	ctrl->Freeze();
	
	ctrl->AddPage(new wxPanel(ctrl, wxID_ANY), wxT("Materials"), false, NULL);
	ctrl->SetPageToolTip(0, wxT("Materials"));

	ctrl->AddPage(new wxPanel(ctrl, wxID_ANY), wxT("Lists"), false, NULL);
	ctrl->SetPageToolTip(1, wxT("Lists"));

	ctrl->AddPage(new wxPanel(ctrl, wxID_ANY), wxT("Model"), false, NULL);
	ctrl->SetPageToolTip(2, wxT("Model"));

	ctrl->AddPage(new wxPanel(ctrl, wxID_ANY), wxT("Group"), false, NULL);
	ctrl->SetPageToolTip(3, wxT("Group"));

	ctrl->Thaw();
	return ctrl;
}

wxSplitterWindow *JetEditorMainFrame::CreateCADView()
{
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

	return pHorizontalSplit;
}

wxPropertyGrid *JetEditorMainFrame::CreatePropertyView()
{
	wxPropertyGrid *pPropGrid = new wxPropertyGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_AUTO_SORT | wxPG_SPLITTER_AUTO_CENTER | wxPG_DEFAULT_STYLE);
	return pPropGrid;
}

void JetEditorMainFrame::SetupMenus()
{
	wxMenuBar *pMenuBar = new wxMenuBar;

	wxMenu *pFileMenu = new wxMenu;
	pFileMenu->Append(wxID_NEW, wxT("&New"));
	pFileMenu->Append(wxID_OPEN, wxT("&Open..."));
	pFileMenu->Append(wxID_CLOSE, wxT("&Close"));
	pFileMenu->Append(wxID_SAVE, wxT("&Save"));
	pFileMenu->Append(wxID_SAVEAS, wxT("Save &As..."));
	pFileMenu->AppendSeparator();
	pFileMenu->Append(ID_FILE_EXPORT, wxT("&Export..."));
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
	pEditMenu->Append(ID_EDIT_CLONESELECTED, wxT("Clone &Selected"));
	pEditMenu->AppendSeparator();
	pEditMenu->Append(ID_EDIT_SELECTALL, wxT("Select &All"));

	wxMenu *pEditSelectAll = new wxMenu;
	pEditSelectAll->Append(ID_EDIT_SELECTALLBYTYPE_BRUSHES, wxT("Brushes"));
	pEditSelectAll->Append(ID_EDIT_SELECTALLBYTYPE_CAMERAS, wxT("Cameras"));
	pEditSelectAll->Append(ID_EDIT_SELECTALLBYTYPE_LIGHTS, wxT("Lights"));
	pEditSelectAll->Append(ID_EDIT_SELECTALLBYTYPE_MODELS, wxT("Models"));
	pEditMenu->AppendSubMenu(pEditSelectAll, wxT("Select All By Type"));

	pEditMenu->Append(ID_EDIT_SELECTNONE, wxT("Select None"));
	pEditMenu->Append(ID_EDIT_SELECTINVERT, wxT("Select Invert"));

	wxMenu *pViewMenu = new wxMenu;
	pViewMenu->Append(ID_VIEW_ZOOMIN, wxT("Zoom In"));
	pViewMenu->Append(ID_VIEW_ZOOMOUT, wxT("Zoom Out"));
	pViewMenu->Append(ID_VIEW_CENTERONSELECTION, wxT("Center On Selection"));
	pViewMenu->AppendSeparator();
	pViewMenu->AppendCheckItem(ID_VIEW_SNAPTOGRID, wxT("Snap to Grid"));

	wxMenu *pModeMenu = new wxMenu;
	pModeMenu->AppendRadioItem(ID_MODE_MOVESCALE, wxT("Move/Scale"));
	pModeMenu->AppendRadioItem(ID_MODE_ROTATESHEAR, wxT("Rotate/Shear"));
	pModeMenu->AppendRadioItem(ID_MODE_VERTEXMANIPULATION, wxT("Vertex Manipulation"));

	wxMenu *pBuildMenu = new wxMenu;
	pBuildMenu->Append(ID_BUILD_UPDATESELECTION, wxT("Update Selection"));
	pBuildMenu->Append(ID_BUILD_UPDATEALL, wxT("Update All"));
	pBuildMenu->Append(ID_BUILD_REBUILDALL, wxT("Rebuild All..."));

	wxMenu *pObjectMenu = new wxMenu;
	pObjectMenu->Append(ID_OBJECT_CUBE, wxT("Cube"));
	pObjectMenu->Append(ID_OBJECT_CYLINDER, wxT("Cylinder"));
	pObjectMenu->Append(ID_OBJECT_SPHERE, wxT("Sphere"));
	pObjectMenu->Append(ID_OBJECT_ARCH, wxT("Arch"));
	pObjectMenu->Append(ID_OBJECT_SHEET, wxT("Sheet"));
	pObjectMenu->AppendSeparator();
	pObjectMenu->Append(ID_OBJECT_LIGHT, wxT("Light"));
	pObjectMenu->Append(ID_OBJECT_CAMERA, wxT("Camera"));
	pObjectMenu->AppendSeparator();

	m_pObjectMenu = new wxMenu;
	pObjectMenu->AppendSubMenu(m_pObjectMenu, wxT("Jet3D Objects"));

	wxMenu *pHelpMenu = new wxMenu;
	pHelpMenu->Append(wxID_ABOUT, wxT("About Jet Editor"));

	pMenuBar->Append(pFileMenu, _("&File"));
	pMenuBar->Append(pEditMenu, _("&Edit"));
	pMenuBar->Append(pViewMenu, _("&View"));
	pMenuBar->Append(pModeMenu, _("&Mode"));
	pMenuBar->Append(pBuildMenu, _("&Build"));
	pMenuBar->Append(pObjectMenu, _("&Object"));
	pMenuBar->Append(pHelpMenu, _("&Help"));

	SetMenuBar(pMenuBar);
}

void JetEditorMainFrame::OnUpdateUI(wxUpdateUIEvent &event)
{

}