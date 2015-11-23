#pragma once

#include <boost/any.hpp>
#include <lua.hpp>
#include <map>
#include <string>

class ConfigurationReader;

class ConfigurationSet {
    public:
        template<typename T> const T& get(const std::string& name) const {
            auto value = m_set.find(name);
            if (value == m_set.end())
                throw not_found_error("Parameter '" + name + "' not found.");

            return boost::any_cast<const T&>(m_set.at(name)); 
        }

        template<typename T> const T& get(const std::string& name, const T& defaultValue) const {
            auto value = m_set.find(name);
            if (value == m_set.end())
                return defaultValue;

            return value->second;
        }

        void parse(lua_State* L, int index);

        std::string getModuleName() const {
            return m_module_name;
        }

        std::string getModuleType() const {
            return m_module_type;
        }

    private:
        class not_found_error: public std::runtime_error {
            using std::runtime_error::runtime_error;
        };

        friend class ConfigurationReader;
        ConfigurationSet(const std::string& module_type, const std::string& module_name):
            m_module_type(module_type),
            m_module_name(module_name) {}

        std::string m_module_type;
        std::string m_module_name;
        std::map<std::string, boost::any> m_set;
};
