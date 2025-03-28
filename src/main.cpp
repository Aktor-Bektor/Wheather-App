#include <stdexcept>
#include <iostream>
#include <sstream>
#include <wx-3.2/wx/wx.h>
#include <wx-3.2/wx/listctrl.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <stdlib.h>
#include <cstring>
#include "request.cpp"

//App Class
class App : public wxApp 
{
    public:
        virtual bool OnInit();
};

//Frame Class
class Frame : public wxFrame
{
    public:
        Frame(const wxString& title, const wxPoint& pos, const wxSize& size);

    private:
        wxListView * basicListView;
        void populateListView();
};

//Initialization Things
bool App::OnInit()
{
    Frame * frame = new Frame("Wheather App", wxDefaultPosition, wxSize(800, 600));
    frame -> Show(true);
    return true;
}

//Frame Constructor
Frame::Frame(const wxString &title, const wxPoint &pos, const wxSize &size) : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    //Panel Declaration
    wxPanel * panel = new wxPanel(this);

    //ListView Declaration
    basicListView = new wxListView(panel);
    basicListView -> AppendColumn("ID");
    basicListView -> AppendColumn("Name");
    basicListView -> AppendColumn("Description");
    basicListView -> SetColumnWidth(0, 80);
    basicListView -> SetColumnWidth(1, 120);
    basicListView -> SetColumnWidth(2, 600);

    //Button Declaration

    //Sizer Shananighans
    auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer -> Add(basicListView, 1, wxEXPAND | wxALL, 0);
    
    //Existance
    panel -> SetSizerAndFit(sizer);

    //Event Handling
    populateListView();
}

void Frame::populateListView()
{
    basicListView -> InsertItem(0, "1");
    basicListView -> SetItem(0, 1, "Item 1");
    basicListView -> SetItem(0, 2, "This is an item");

    basicListView -> InsertItem(1, "2");
    basicListView -> SetItem(1, 1, "Item 2");
    basicListView -> SetItem(1, 2, "This is another item");

    basicListView -> InsertItem(2, "3");
    basicListView -> SetItem(2, 1, "Item 3");
    basicListView -> SetItem(2, 2, "This is yet another item");
}

//Main Function
wxIMPLEMENT_APP(App);