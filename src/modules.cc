#include <iostream>

#include <ConfigurationReader.h>
#include <MEMpp.h>

int main(int argc, char** argv) {

    ConfigurationReader configuration;

    // TODO:
    // configuration.parse("configuration.tcl");

    MEMpp weight(configuration);

    std::vector<std::pair<double, double>> weights = weight.computeWeights({});

    std::cout << "Result: " << std::endl;
    for (const auto& r: weights) {
        std::cout << r.first << " +- " << r.second << std::endl;
    }

    return 0;
}
