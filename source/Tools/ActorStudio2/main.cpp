/*
@file main.cpp
@author paradoxnj
@brief ActorStudio2 main entry point

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
#include <wx/wx.h>

class ActorStudioApp : public wxApp
{
public:
	bool OnInit() wxOVERRIDE;
};

wxDECLARE_APP(ActorStudioApp);
wxIMPLEMENT_APP(ActorStudioApp);

bool ActorStudioApp::OnInit()
{
	if (!wxApp::OnInit())
		return false;

	//JetEditorMainFrame *pFrame = new JetEditorMainFrame(NULL, wxID_ANY, _("Jet3D World Editor"));
	//SetTopWindow(pFrame);
	//pFrame->Show();

	return true;
}
