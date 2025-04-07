#include <fstream>
#include <vector>

#include <wx/wx.h>
#include <wx/listctrl.h>

#include "request.cpp"
#include "virtuallistcontrol.h"

class App : public wxApp 
{
    public:
        virtual bool OnInit();
};

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

bool App::OnInit()
{
    Frame * frame = new Frame("Wheather App", wxDefaultPosition, wxSize(800, 600));
    frame -> Show(true);
    return true;
}

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

    SetBackgroundColour(panel->GetBackgroundColour());

    auto searchBarLabel = new wxStaticText(panel, wxID_ANY, "Enter City:");
    searchBarField = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(FromDIP(300), wxDefaultSize.GetHeight()));
    auto searchButton = new wxButton(panel, wxID_ANY, "Search");

    auto toggleCheckBox = new wxCheckBox(panel, wxID_ANY, "Forecast", wxDefaultPosition, wxSize(FromDIP(100), FromDIP(30))); // Adjusted size for better alignment

    wxArrayString daysOptions;
    for (int i = 1; i <= 5; ++i) {
        daysOptions.Add(wxString::Format("%d days", i));
    }
    auto daysDropdown = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxSize(FromDIP(200), FromDIP(30)), daysOptions);
    daysDropdown->Hide();

    auto searchBarSizer = new wxBoxSizer(wxHORIZONTAL);
    searchBarSizer->Add(searchBarLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, FromDIP(3));
    searchBarSizer->Add(searchBarField, 1, wxALIGN_CENTER_VERTICAL | wxALL, FromDIP(3));
    panelSizer->Add(searchBarSizer, 0, wxEXPAND | wxALL, FromDIP(3));

    auto forecastSizer = new wxBoxSizer(wxHORIZONTAL);
    forecastSizer->Add(toggleCheckBox, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, FromDIP(5));
    forecastSizer->Add(daysDropdown, 0, wxALIGN_CENTER_VERTICAL);
    panelSizer->Add(forecastSizer, 0, wxEXPAND | wxALL, FromDIP(3));

    panelSizer->Add(searchButton, 0, wxEXPAND | wxALL, FromDIP(3));

    panel->SetSizerAndFit(panelSizer);

    centeringSizer->Add(panel, 1, wxALIGN_CENTER | wxALL, FromDIP(10));

    parentSizer->Add(centeringSizer, 1, wxEXPAND);

    toggleCheckBox->Bind(wxEVT_CHECKBOX, [daysDropdown, forecastSizer](wxCommandEvent& event) {
        if (event.IsChecked()) {
            daysDropdown->Show();
        } else {
            daysDropdown->Hide();
        }
        forecastSizer->Layout();
    });

    searchButton->Bind(wxEVT_BUTTON, [this, toggleCheckBox, daysDropdown](wxCommandEvent&) {
        int days = 0;

        if (toggleCheckBox->IsChecked()) {
            int selectedIndex = daysDropdown->GetSelection();
            if (selectedIndex != wxNOT_FOUND) {
                days = selectedIndex + 1;
            } else {
                wxMessageBox("Please select the number of forecast days.", "Error", wxOK | wxICON_ERROR);
                return;
            }
        }

        PopulateListView(searchBarField->GetValue().ToStdString(), days);
    });
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
    else
    {
        std::vector<request::WheatherData> data;

        data = request::get_forecast(city, days);

        for(const auto& day : data)
        {
            listView -> days.push_back({day.location, day.condition, day.temp, day.wind, day.humidity, day.precipitation, day.feels_like});
        }

        listView -> RefreshAfterUpdate();
    }
}

wxIMPLEMENT_APP(App);