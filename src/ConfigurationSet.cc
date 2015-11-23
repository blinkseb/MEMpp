#include <ConfigurationSet.h>
#include <logging.h>
#include <lua/utils.h>

void ConfigurationSet::parse(lua_State* L, int index) {

    lua_pushnil(L);
    while (lua_next(L, -2) != 0) {

        std::string key = lua_tostring(L, -2);

        try {
            boost::any value = lua::to_any(L, -1);
            m_set.emplace(key, value);
        } catch(...) {
            LOG(emerg) << "Exception while trying to parse parameter " << m_module_type << "." << m_module_name << "::" << key;
            std::rethrow_exception(std::current_exception());
        }

        lua_pop(L, 1);
    }
}
