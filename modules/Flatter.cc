#include <Module.h>

class Flatter: public Module {
    public:

        Flatter(const std::string& name): Module(name) {
            static size_t counter = 0;
            ps_index = counter;
            counter++;
        };

        virtual void work() {

            double psPoint = ps_points->at(ps_index);
            const double range = M_PI / 2. + std::atan(mass / width);
            const double y = - std::atan(mass / width) + range * psPoint;

            *s = mass * width * std::tan(y) + (mass * mass);
            *jacobian = range * mass * width / (std::cos(y) * std::cos(y));
        }

    private:
        size_t ps_index = 0; // Will be configurable
        const float mass = 173.; // Will be configurable
        const float width = 3.; // Will be configurable

        std::string vegas_module_name = "cuba"; // Will be configurable

        std::shared_ptr<const std::vector<double>> ps_points = get<std::vector<double>>(vegas_module_name, "ps_points");

        std::shared_ptr<double> s = produce<double>("s");
        std::shared_ptr<double> jacobian = produce<double>("jacobian");


};
REGISTER_MODULE(Flatter);

