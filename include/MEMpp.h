#pragma once

#include <vector>

#include <Module.h>
#include <SharedLibrary.h>

struct ConfigurationReader;

class MEMpp {
    public:
        MEMpp(const ConfigurationReader&);
        virtual ~MEMpp();

        std::vector<std::pair<double, double>> computeWeights(const std::vector<float>& particules);

        double integrand(const double* psPoints, const double* weights);


    private:
        static int CUBAIntegrand(const int *nDim, const double* psPoint, const int *nComp, double *value, void *inputs, const int *nVec, const int *core, const double *weight);

        std::vector<ModulePtr> m_modules;

        using SharedLibraryPtr = std::shared_ptr<SharedLibrary>;
        std::vector<SharedLibraryPtr> m_libraries;

        size_t m_n_dimensions;
        std::shared_ptr<std::vector<double>> m_ps_points;
};
