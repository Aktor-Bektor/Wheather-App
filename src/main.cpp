#include <stdexcept>
#include <iostream>
#include <sstream>
#include <wx-3.2/wx/wx.h>
#include <wx-3.2/wx/listctrl.h>
#include <curl/curl.h>

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
//wxIMPLEMENT_APP(App);

int main()
{
    std::string city, feature;
    std::stringstream adress;
    
    //Feature Input
    std::cout << "Enter the feature you want to see (current, forecast): ";
    std::cin >> feature;


    if(feature == "current")
    {
        //City Input
        std::cout << "Enter the city name: ";
        std::cin >> city;

        adress << "http://api.weatherapi.com/v1/current.json?key=53be16e6eef04200abc175333252703&q=" 
               << city 
               << "&appid=8";
    }
    else if(feature == "forecast")
    {
        //City Input
        std::cout << "Enter the city name: ";
        std::cin >> city;

        int days;
        std::string aqi;

        //Getting the number of days and the air quality index
        std::cout << "Enter the number of days you want to see: ";
        std::cin >> days;

        if(typeid(days) != typeid(int) || days < 1 || days > 14)
        {
            throw std::runtime_error("Invalid number of days");
            return -1;
        }

        std::cout << "Do you want to see the air quality index? (yes, no): ";
        std::cin >> aqi;

        if(aqi == "yes")
            adress << "http://api.weatherapi.com/v1/forecast.json?key=53be16e6eef04200abc175333252703&q=" 
                   << city 
                   << "&days=" 
                   << days 
                   << "1&aqi=yes&alerts=no";
        else if(aqi == "no")
            adress << "http://api.weatherapi.com/v1/forecast.json?key=53be16e6eef04200abc175333252703&q=" 
                   << city 
                   << "&days=" 
                   << days 
                   << "&aqi=no&alerts=no";
        else
        {
            throw std::runtime_error("Invalid input for air quality index");
            return -1;
        }
    }
    else
    {
        throw std::runtime_error("Invalid feature");
        return -1;
    }


    

    std::cout << adress.str() << std::endl;

    CURL *curl;
    CURLcode result;

    curl = curl_easy_init();

    if(curl == NULL)
    {
        throw std::runtime_error("HTTP request failed");
        return -1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, adress.str().c_str());

    result = curl_easy_perform(curl);

    if(result != CURLE_OK)
    {
        throw std::runtime_error(curl_easy_strerror(result));
        return -1;
    }

    curl_easy_cleanup(curl);

    return 0;
}