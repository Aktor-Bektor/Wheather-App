#include <sstream>
#include <curl/curl.h>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <cstring>
#include <nlohmann/json.hpp>
#include <cmath>

namespace request
{
    struct WheatherData
    {
        std::string location;
        std::string condition;
        float temp;
        float wind;
        float humidity;
        float precipitation;
        float feels_like;
    };

    struct Response
    {
        char* string;
        size_t size;
    };

    size_t write_chunck(void *data, size_t size, size_t nmemb, std::string *userdata)
    {
        size_t real_size = size * nmemb;

        Response *response = (Response *) userdata;

        char *ptr = (char *) realloc(response -> string, response -> size + real_size + 1);

        if(ptr == NULL)
        {
            throw std::runtime_error("Memory reallocation failed");
            return CURLE_WRITE_ERROR;
        }

        response -> string = ptr;
        memcpy(&(response -> string[response -> size]), data, real_size);
        response -> size += real_size;
        response -> string[response -> size] = 0;

        return real_size;
    }

    WheatherData get_current(std::string city)
    {
        WheatherData data;

        std::stringstream adress;
        adress << "http://api.weatherapi.com/v1/current.json?key=53be16e6eef04200abc175333252703&q=" 
            << city 
            << "&appid=8";
        
        CURL *curl;
        CURLcode result;
        curl = curl_easy_init();

        if(!curl)
        {
            throw std::runtime_error("Curl initialization failed");
        }

        Response response;
        response.string = (char *) malloc(1);
        response.size = 0;

        curl_easy_setopt(curl, CURLOPT_URL, adress.str().c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_chunck);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &response);

        result = curl_easy_perform(curl);

        if(result != CURLE_OK)
        {
            throw std::runtime_error(curl_easy_strerror(result));
        }
        try
        {
            nlohmann::json json_response = nlohmann::json::parse(response.string);

            std::string location = json_response["location"]["name"];
            std::string condition = json_response["current"]["condition"]["text"];
            float wind = json_response["current"]["wind_kph"];
            float temp = json_response["current"]["temp_c"];
            float humidity = json_response["current"]["humidity"];
            float precipitation = json_response["current"]["precip_mm"];
            float feels_like = json_response["current"]["feelslike_c"];

            data.location = location;
            data.temp = temp;
            data.condition = condition;
            data.wind = wind;
            data.humidity = humidity;
            data.precipitation = precipitation;
            data.feels_like = feels_like;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }

        free(response.string);
        curl_easy_cleanup(curl);

        return data;
    }

    std::vector <WheatherData> forecast(std::string city, short days)
    {
        std::vector<WheatherData> data;

        std::stringstream adress;
        adress << "http://api.weatherapi.com/v1/forecast.json?key=53be16e6eef04200abc175333252703&q=" 
            << city 
            << "&days=" 
            << days 
            << "&aqi=no&alerts=no";

        CURL *curl;
        CURLcode result;
        curl = curl_easy_init();

        if(!curl)
        {
            throw std::runtime_error("Curl initialization failed");
        }

        Response response;
        response.string = (char *) malloc(1);
        response.size = 0;

        curl_easy_setopt(curl, CURLOPT_URL, adress.str().c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_chunck);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &response);

        result = curl_easy_perform(curl);

        if(result != CURLE_OK)
        {
            throw std::runtime_error(curl_easy_strerror(result));
        }
        try
        {
            nlohmann::json json_response = nlohmann::json::parse(response.string);

            for(int i = 0; i < days; i++)
            {
                WheatherData current;
                std::string location = json_response["location"]["name"];
                std::string condition = json_response["forecast"]["forecastday"][i]["day"]["condition"]["text"];
                float wind = json_response["forecast"]["forecastday"][i]["day"]["maxwind_kph"];
                float temp = json_response["forecast"]["forecastday"][i]["day"]["avgtemp_c"];
                float humidity = json_response["forecast"]["forecastday"][i]["day"]["avghumidity"];
                float precipitation = json_response["forecast"]["forecastday"][i]["day"]["totalprecip_mm"];
                float feels_like = json_response["forecast"]["forecastday"][i]["day"]["avgtemp_c"];

                current.location = location;
                current.temp = temp;
                current.condition = condition;
                current.wind = wind;
                current.humidity = humidity;
                current.precipitation = precipitation;
                current.feels_like = feels_like;

                data.push_back(current);
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }

        return data;
    }
}