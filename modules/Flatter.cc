#include <ArrayEntry.h>
#include <ConfigurationSet.h>
#include <Module.h>

#include <iostream>

class Flatter: public Module {
    public:

        Flatter(const ConfigurationSet& parameters): Module(parameters.getModuleName()),
            mass(parameters.get<double>("mass")),
            width(parameters.get<double>("width")),
            m_ps_point(parameters.get<ArrayEntry>("input")) {

        };

        virtual void work() override {

            double psPoint = m_ps_point.get<double>();
            const double range = M_PI / 2. + std::atan(mass / width);
            const double y = - std::atan(mass / width) + range * psPoint;

            *s = mass * width * std::tan(y) + (mass * mass);
            *jacobian = range * mass * width / (std::cos(y) * std::cos(y));
        }

        virtual size_t dimensions() const override {
            return 1;
        }

    private:
        const float mass;
        const float width;
        ArrayEntry m_ps_point;

        std::shared_ptr<double> s = produce<double>("s");
        std::shared_ptr<double> jacobian = produce<double>("jacobian");


};
REGISTER_MODULE(Flatter);
