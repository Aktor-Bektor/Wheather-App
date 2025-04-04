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
        this->AppendColumn("Day", wxLIST_FORMAT_CENTER, 80);

        int totalWidth = size.GetWidth();
        int fixedWidth = 80;
        int remainingWidth = totalWidth - fixedWidth;

        float conditionMultiplier = 1.5f;
        float otherMultiplier = 1.0f;
        float totalMultiplier = conditionMultiplier + 5 * otherMultiplier;

        int conditionWidth = static_cast<int>((remainingWidth * conditionMultiplier) / totalMultiplier);
        int otherWidth = static_cast<int>((remainingWidth * otherMultiplier) / totalMultiplier);

        this->AppendColumn("Condition", wxLIST_FORMAT_CENTER, conditionWidth);
        this->AppendColumn("Temperature", wxLIST_FORMAT_CENTER, otherWidth);
        this->AppendColumn("Wind", wxLIST_FORMAT_CENTER, otherWidth);
        this->AppendColumn("Humidity", wxLIST_FORMAT_CENTER, otherWidth);
        this->AppendColumn("Precipitation", wxLIST_FORMAT_CENTER, otherWidth);
        this->AppendColumn("Feels Like", wxLIST_FORMAT_CENTER, otherWidth);

        Bind(wxEVT_SIZE, &VirtualListControl::OnResize, this);
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
        days.clear();
    }

    std::vector <request::WheatherData> days;

private:
    void OnResize(wxSizeEvent& event)
    {
        int totalWidth = GetClientSize().GetWidth();
        int fixedWidth = 80;
        int remainingWidth = totalWidth - fixedWidth;

        float conditionMultiplier = 1.5f;
        float otherMultiplier = 1.0f;
        float totalMultiplier = conditionMultiplier + 5 * otherMultiplier;

        int conditionWidth = static_cast<int>((remainingWidth * conditionMultiplier) / totalMultiplier);
        int otherWidth = static_cast<int>((remainingWidth * otherMultiplier) / totalMultiplier);

        SetColumnWidth(0, fixedWidth);
        SetColumnWidth(1, conditionWidth);
        SetColumnWidth(2, otherWidth);
        SetColumnWidth(3, otherWidth);
        SetColumnWidth(4, otherWidth);
        SetColumnWidth(5, otherWidth);
        SetColumnWidth(6, otherWidth);

        event.Skip();
    }
};