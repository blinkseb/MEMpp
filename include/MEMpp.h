#pragma once

#include <vector>

#include <Module.h>
#include <SharedLibrary.h>

struct ConfigurationReader;

class MEMpp {
    public:
        MEMpp(const ConfigurationReader&);
        virtual ~MEMpp();

        std::vector<float> computeWeights(const std::vector<float>& particules);

    private:
        std::vector<ModulePtr> m_modules;

        using SharedLibraryPtr = std::shared_ptr<SharedLibrary>;
        std::vector<SharedLibraryPtr> m_libraries;
};
