#include <random>

#include <Module.h>

// A simple module generating random points
class Vegas: public Module {
    public:
        Vegas(const std::string& name): Module(name), random_generator(1), uniform_distribution(0., 1.) {

        }

        virtual void work() override {
            points->clear();

            for (size_t i = 0; i < 10; i++) {
                points->push_back(uniform_distribution(random_generator));
            }
        }

    private:
        std::mt19937 random_generator;
        std::uniform_real_distribution<> uniform_distribution;

        std::shared_ptr<std::vector<double>> points = produce<std::vector<double>>("ps_points");

};
REGISTER_MODULE(Vegas);
