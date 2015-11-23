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

boost::any Pool::raw_get(const InputTag& tag) {
    auto it = m_pool.find(tag);
    if (it == m_pool.end())
        throw tag_not_found_error("No such tag in pool: " + tag.toString());
    return it->second;
}
