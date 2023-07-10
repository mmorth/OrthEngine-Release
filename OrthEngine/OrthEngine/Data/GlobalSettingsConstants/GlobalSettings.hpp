#ifndef GLOBALSETTINGS_HPP
#define GLOBALSETTINGS_HPP

#include <iostream>
#include <fstream>

#include <g3log/g3log.hpp>
#include <g3log/loglevels.hpp>
#include <g3log/logworker.hpp>
#include <json/json.hpp>

class GlobalSettings 
{
public:
    static GlobalSettings& getInstance();

    void loadConfig(const std::string& filename);

    template<typename T>
    T getValue(const std::string& key, const T& defaultValue) const 
    {
        if (m_config.contains(key)) 
        {
            try 
            {
                return m_config[key].get<T>();
            }
            catch (const nlohmann::json::exception& e) 
            {
                std::cerr << "Failed to get value for key '" << key << "': " << e.what() << std::endl;
            }
        }

        return defaultValue;
    }

    template<typename T>
    void setValue(const std::string& key, const T& value) 
    {
        m_config[key] = value;
    }

private:
    nlohmann::json m_config;

    GlobalSettings();
    GlobalSettings(const GlobalSettings&) = delete;
    GlobalSettings& operator=(const GlobalSettings&) = delete;
};

#endif // GLOBALSETTINGS_HPP
