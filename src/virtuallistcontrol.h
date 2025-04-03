#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>

#include <vector>

#include "request.h"

class VirtualListControl : public wxListCtrl 
{
public:
    VirtualListControl(wxWindow *parent, const wxWindowID id, const wxPoint &pos, const wxSize &size)
        : wxListCtrl(parent, id, pos, size, wxLC_REPORT | wxLC_VIRTUAL)
    {
        this -> AppendColumn("Day", wxLIST_FORMAT_CENTER, 80);
        this -> AppendColumn("Condition", wxLIST_FORMAT_CENTER, 120);
        this -> AppendColumn("Temperature", wxLIST_FORMAT_CENTER, 80);
        this -> AppendColumn("Wind"), wxLIST_FORMAT_CENTER, 80;
        this -> AppendColumn("Humidity", wxLIST_FORMAT_CENTER, 80);
        this -> AppendColumn("Precipitation", wxLIST_FORMAT_CENTER, 80);
        this -> AppendColumn("Feels Like", wxLIST_FORMAT_CENTER, 80);
    }

    virtual wxString OnGetItemText(long index, long column) const wxOVERRIDE 
    {
        request::WheatherData day = days[index];
        switch(column) {
            case 0: return day.location;
            case 1: return day.condition;
            case 2: return wxString::Format("%.2f", day.temp);
            case 3: return wxString::Format("%.2f", day.wind);
            case 4: return wxString::Format("%.2f", day.humidity);
            case 5: return wxString::Format("%.2f", day.precipitation);
            case 6: return wxString::Format("%.2f", day.feels_like);
            default: return "";
        }
    }

    void RefreshAfterUpdate() {
        this -> SetItemCount(days.size());
        this -> Refresh();
    }

    std::vector <request::WheatherData> days;
};