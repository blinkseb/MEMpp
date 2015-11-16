#include <Pool.h>

Pool& Pool::get() {
    return *instance();
}

void Pool::create() {
    instance() = new Pool();
}

void Pool::destroy() {
    delete instance();
    instance() = nullptr;
}

Pool*& Pool::instance() {
    static Pool* instance = nullptr;
    return instance;
}
