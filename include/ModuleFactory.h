#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// Forward declaration
class Module;

// Plugin system
template<typename T> class PluginFactory;

// FIXME: This need some error protection

template<typename Interface, typename... Args>
class PluginFactory<Interface* (Args...)> {
    public:
        using type = Interface* (Args...);
        using Factory = PluginFactory<Interface*(Args...)>;

        struct PMakerBase {
            virtual std::shared_ptr<Interface> create(Args...) const = 0;
            virtual ~PMakerBase() {}
        };

        template<class ModuleType>
            struct PMaker: public PMakerBase {
                PMaker(const std::string& name) {
                    Factory::get().registerPMaker(this, name);
                }

                virtual std::shared_ptr<Interface> create(Args... args) const override {
                    return std::shared_ptr<ModuleType>(new ModuleType(std::forward<Args>(args)...));
                }
            };

        std::shared_ptr<Interface> create(const std::string& name, Args... args) const {
            return findPMaker(name)->create(std::forward<Args>(args)...);
        }

        static PluginFactory<Interface* (Args...)>& get();

        void registerPMaker(PMakerBase* pMaker, const std::string& name) {
            m_plugins.emplace(name, pMaker);
        }

        PMakerBase* findPMaker(const std::string& name) const {
            return m_plugins.at(name);
        }

        std::vector<std::string> getPluginsList() const {
            std::vector<std::string> result;
            for (const auto& plugin: m_plugins) {
                result.push_back(plugin.first);
            }

            return result;
        }

    private:
        PluginFactory() {
        }

        PluginFactory(const PluginFactory&) = delete; // stop default
        const PluginFactory& operator=(const PluginFactory&) = delete; // stop default

        std::unordered_map<std::string, PMakerBase*> m_plugins;
};

using ModuleFactory = PluginFactory<Module* (const std::string&)>;

#define MODULE_UNIQUE_NAME2(x, y) x ## y
#define MODULE_UNIQUE_NAME(x, y) MODULE_UNIQUE_NAME2(x, y)
#define REGISTER_MODULE(type) \
    static const ModuleFactory::PMaker<type> MODULE_UNIQUE_NAME(s_module , __LINE__)(#type)
