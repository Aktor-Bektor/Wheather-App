#include <fstream>
#include <vector>

#include <wx/wx.h>
#include <wx/listctrl.h>

#include "request.cpp"
#include "virtuallistcontrol.h"


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
        VirtualListControl *listView;

        wxTextCtrl *searchBarField;

        wxString city, country;

        void SetupSearchBar(wxBoxSizer* parentSizer);

        void PopulateListView(std::string city, char days);
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
    auto mainSizer = new wxBoxSizer(wxVERTICAL);

    auto searchBarSizer = new wxBoxSizer(wxVERTICAL);
    SetupSearchBar(searchBarSizer);
    mainSizer -> Add(searchBarSizer, 0, wxEXPAND | wxALL, 5);

    wxPanel *panel = new wxPanel(this);
    listView = new VirtualListControl(panel, wxID_ANY, wxDefaultPosition, wxSize(800, 500));

    auto listSizer = new wxBoxSizer(wxVERTICAL);
    listSizer->Add(listView, 1, wxALL | wxEXPAND, 5);
    panel->SetSizerAndFit(listSizer);

    mainSizer->Add(panel, 1, wxEXPAND | wxALL, 5); 

    this -> SetSizerAndFit(mainSizer);
}

void Frame::SetupSearchBar(wxBoxSizer* parentSizer)
{
    auto centeringSizer = new wxBoxSizer(wxHORIZONTAL);

    auto panel = new wxPanel(this);
    auto panelSizer = new wxBoxSizer(wxVERTICAL);

    SetBackgroundColour(panel -> GetBackgroundColour());
    
    auto searchBarLabel = new wxStaticText(panel, wxID_ANY, "Enter City:");
    searchBarField = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(FromDIP(300), wxDefaultSize.GetHeight()));
    auto searchButton = new wxButton(panel, wxID_ANY, "Search");

    std::vector<wxWindow*> items {
        searchBarLabel,
        searchBarField,
        searchButton
    };

    for(auto item : items) {
        panelSizer -> Add(item, 0, wxEXPAND | wxALL, FromDIP(3));
    }

    panel -> SetSizerAndFit(panelSizer);

    centeringSizer -> Add(panel, 1, wxALIGN_CENTER | wxALL, FromDIP(10));

    parentSizer -> Add(centeringSizer, 1, wxEXPAND);

    searchButton -> Bind(wxEVT_BUTTON, [this](wxCommandEvent&) { PopulateListView(searchBarField -> GetValue().ToStdString(), 0); });
}

void Frame::PopulateListView(std::string city, char days = 0)
{
    if(days == 0)
    {
        request::WheatherData data;

        data = request::get_current(city);

        listView -> days.push_back({data.location, data.condition, data.temp, data.wind, data.humidity, data.precipitation, data.feels_like});

        listView -> RefreshAfterUpdate();
    }
}

//Main Function
wxIMPLEMENT_APP(App);