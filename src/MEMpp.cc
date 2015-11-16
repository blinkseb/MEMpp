#include <iostream>

#include <MEMpp.h>
#include <Pool.h>

MEMpp::MEMpp(const ConfigurationReader& configuration) {
    // Initialize shared memory pool for modules
    Pool::create();

    // Construct modules from configuration
    // Do it manually here since we don't have a configuration parser yet
    
    // First, load all shared libraries containing modules
    m_libraries.push_back(std::make_shared<SharedLibrary>("libmodules.so"));
    
    m_modules.push_back(ModuleFactory::get().create("Vegas", "vegas"));
    m_modules.push_back(ModuleFactory::get().create("Flatter", "top1_flatter"));
    m_modules.push_back(ModuleFactory::get().create("Flatter", "top2_flatter"));

}

MEMpp::~MEMpp() {
    // Destroy memory pool. Must be done before unloading libraries
    Pool::destroy();
}

std::vector<float> MEMpp::computeWeights(const std::vector<float>& particules) {
    for (size_t i = 0; i < 10; i++) {
        for (auto& module: m_modules)
            module->work();

        std::cout << "Top1 flatter: " << std::sqrt(*Pool::get().get<double>({"top1_flatter", "s"})) << std::endl;
        std::cout << "Top2 flatter: " << std::sqrt(*Pool::get().get<double>({"top2_flatter", "s"})) << std::endl;
    }

    return std::vector<float>();
}
