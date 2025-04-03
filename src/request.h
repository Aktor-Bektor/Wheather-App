#pragma once

#include <vector>
#include <string>

namespace request {
    struct WheatherData {
        std::string location;
        std::string condition;
        float temp;
        float wind;
        float humidity;
        float precipitation;
        float feels_like;
    };

    struct Response {
        char* string;
        size_t size;
    };

    size_t write_chunck(void *data, size_t size, size_t nmemb, std::string *userdata);
    WheatherData get_current(std::string city);
    std::vector <WheatherData> get_forecast(std::string city, short days);
}