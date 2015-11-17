#include <iostream>

#include <ConfigurationReader.h>
#include <MEMpp.h>
#include <Types.h>

int main(int argc, char** argv) {

    ConfigurationReader configuration;

    // TODO:
    // configuration.parse("configuration.tcl");

    MEMpp weight(configuration);

    // Electron
    LorentzVector p3(1.617190e+01, -1.379191e+01, -3.429975e+00, 2.152932e+01);
    // b-quark
    LorentzVector p4(-5.579083e+01, -1.115929e+02, -1.221447e+02, 1.746626e+02);
    // Muon
    LorentzVector p5(-1.890186e+01, 1.008961e+01, -6.029266e-01, 2.143464e+01);
    // Anti b-quark
    LorentzVector p6(7.327256e+01, 9.854132e+01, -7.928848e+01, 1.462464e+02);

    std::vector<std::pair<double, double>> weights = weight.computeWeights({p3, p4, p5, p6});

    std::cout << "Result: " << std::endl;
    for (const auto& r: weights) {
        std::cout << r.first << " +- " << r.second << std::endl;
    }

    return 0;
}
