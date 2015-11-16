#include <iostream>

#include <ConfigurationReader.h>
#include <MEMpp.h>

int main(int argc, char** argv) {

    ConfigurationReader configuration;

    // TODO:
    // configuration.parse("configuration.tcl");

    MEMpp weight(configuration);

    std::vector<float> weights = weight.computeWeights({});

    return 0;
}
