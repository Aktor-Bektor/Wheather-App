#include <bits/stdc++.h>
#include <wx-3.2/wx/wx.h>
#include <curl/curl.h>

class App : public wxApp 
{
    public:
        virtual bool OnInit();
};

class Frame : public wxFrame
{
    public:
        Frame(const wxString& title, const wxPoint& pos, const wxSize& size);
};

bool App::OnInit()
{
    Frame * frame = new Frame("Wheather App", wxDefaultPosition, wxDefaultSize);
    frame -> Show(true);
    return true;
}

Frame::Frame(const wxString &title, const wxPoint &pos, const wxSize &size) : wxFrame(nullptr, wxID_ANY, title, pos, size)
{
    wxPanel * panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
    panel->SetBackgroundColour(wxColour(75, 79, 94));

    wxBoxSizer * sizer = new wxBoxSizer(wxVERTICAL);
    sizer -> Add(panel, 1, wxEXPAND);

    this -> SetSizerAndFit(sizer);
}

wxIMPLEMENT_APP(App);