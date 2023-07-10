#include "GlobalSettings.hpp"

// ------------------------------------------------------------------------
GlobalSettings& GlobalSettings::getInstance()
{
    static GlobalSettings instance;
    return instance;
}

// ------------------------------------------------------------------------
GlobalSettings::GlobalSettings()
{
    loadConfig("GlobalConfig.json");

    LOG(INFO) << "Global settings loaded";
}

// ------------------------------------------------------------------------
void GlobalSettings::loadConfig(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Failed to open config file: " + filename);

    try 
    {
        file >> m_config;
    }
    catch (const nlohmann::json::parse_error& e) 
    {
        throw std::runtime_error("Failed to parse config file: " + std::string(e.what()));
    }

    file.close();
}
