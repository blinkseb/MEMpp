#pragma once

#include <string>
#include <vector>

struct LightModule {
    std::string name;
    std::string type;
};

class ConfigurationReader {
    public:
        ConfigurationReader();

        void addModule(const std::string& type, const std::string& name);

    private:
        std::vector<LightModule> m_modules;
};
