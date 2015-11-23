#include <ConfigurationSet.h>
#include <Module.h>

class EmptyModule: public Module {
    public:

        EmptyModule(const ConfigurationSet& parameters): Module(parameters.getModuleName()) {
            // Empty
        };

        virtual void work() override {

        }
};
REGISTER_MODULE(EmptyModule);
