#pragma once

#include <ModuleFactory.h>
#include <Pool.h>

class Module {
    public:
        Module(const std::string& name):
            m_name(name) {
            // Empty    
        }

        virtual void configure() { };
        virtual void work() { };
        virtual void finish() { };

    protected:
        template<typename T> std::shared_ptr<T> produce(const std::string& name) {
            return Pool::get().put<T>({m_name, name});
        }

        template<typename T> std::shared_ptr<const T> get(const std::string& module, const std::string& name) {
            return Pool::get().get<T>({module, name});
        }

    private:
        std::string m_name;
};

using ModulePtr = std::shared_ptr<Module>;
