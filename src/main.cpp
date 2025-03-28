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
        void populateListView(std::string city = "", short days = 0);
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

    basicListView = new wxListView(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);

    std::cout << "Enter City: ";
    std::string city;
    std::cin >> city;
    if(typeid(city) != typeid(std::string) || city.empty())
    {
        throw std::invalid_argument("City must be a string");
    }
    std::cout << "Enter Days (0 for current data): ";
    short days;
    std::cin >> days;
    if(typeid(days) != typeid(short) || days < 0 || days > 14)
    {
        throw std::invalid_argument("Days must be a number between 0 and 14");
    }

    //ListView Declaration
    wxListItem column;
    column.SetAlign(wxLIST_FORMAT_CENTER);

    column.SetText("Day");
    basicListView->InsertColumn(0, column);

    column.SetText("Condition");
    basicListView->InsertColumn(1, column);

    column.SetText("Temperature");
    basicListView->InsertColumn(2, column);

    column.SetText("Wind Speed");
    basicListView->InsertColumn(3, column);

    column.SetText("Humidity");
    basicListView->InsertColumn(4, column);

    column.SetText("Precipitation");
    basicListView->InsertColumn(5, column);

    column.SetText("Feels Like");
    basicListView->InsertColumn(6, column);

    basicListView -> SetColumnWidth(0, 80);
    basicListView -> SetColumnWidth(1, 200);
    basicListView -> SetColumnWidth(2, 100);
    basicListView -> SetColumnWidth(3, 100);
    basicListView -> SetColumnWidth(4, 100);
    basicListView -> SetColumnWidth(5, 100);
    basicListView -> SetColumnWidth(6, 100);

    //Button Declaration

    //Sizer Shananighans
    auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer -> Add(basicListView, 1, wxEXPAND | wxALL, 0);
    
    //Existance
    panel -> SetSizerAndFit(sizer);

    //Event Handling
    populateListView(city, days);
}

void Frame::populateListView(std::string city, short days)
{
    if(days == 0)
    {
        request::WheatherData data = request::get_current(city);

        basicListView -> InsertItem(0, wxString::FromUTF8("Current"));
        basicListView -> SetItem(0, 1, data.condition);

        std::stringstream tempStream, windStream, humidityStream, precipStream, feelsLikeStream;
        tempStream << std::fixed << std::setprecision(2) << data.temp;
        windStream << std::fixed << std::setprecision(2) << data.wind;
        humidityStream << std::fixed << std::setprecision(2) << data.humidity;
        precipStream << std::fixed << std::setprecision(2) << data.precipitation;
        feelsLikeStream << std::fixed << std::setprecision(2) << data.feels_like;

        basicListView->SetItem(0, 2, wxString::FromUTF8(tempStream.str()));
        basicListView->SetItem(0, 3, wxString::FromUTF8(windStream.str()));
        basicListView->SetItem(0, 4, wxString::FromUTF8(humidityStream.str()));
        basicListView->SetItem(0, 5, wxString::FromUTF8(precipStream.str()));
        basicListView->SetItem(0, 6, wxString::FromUTF8(feelsLikeStream.str()));
    }
    else
    {
        std::vector<request::WheatherData> data = request::get_forecast(city, days);
        for(size_t i = 0; i < data.size(); ++i)
        {
            basicListView -> InsertItem(i, wxString::FromUTF8(std::to_string(i + 1)));
            basicListView -> SetItem(i, 1, data[i].condition);
            
            std::stringstream tempStream, windStream, humidityStream, precipStream, feelsLikeStream;
            tempStream << std::fixed << std::setprecision(2) << data[i].temp;
            windStream << std::fixed << std::setprecision(2) << data[i].wind;
            humidityStream << std::fixed << std::setprecision(2) << data[i].humidity;
            precipStream << std::fixed << std::setprecision(2) << data[i].precipitation;
            feelsLikeStream << std::fixed << std::setprecision(2) << data[i].feels_like;

            basicListView->SetItem(i, 2, wxString::FromUTF8(tempStream.str()));
            basicListView->SetItem(i, 3, wxString::FromUTF8(windStream.str()));
            basicListView->SetItem(i, 4, wxString::FromUTF8(humidityStream.str()));
            basicListView->SetItem(i, 5, wxString::FromUTF8(precipStream.str()));
            basicListView->SetItem(i, 6, wxString::FromUTF8(feelsLikeStream.str()));
        }
    }
    
}

//Main Function
wxIMPLEMENT_APP(App);