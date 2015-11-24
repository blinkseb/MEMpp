#pragma once

#include <vector>
#include <string>

#include <InputTag.h>
#include <lua/utils.h>
#include <Pool.h>

/*!
 *  Custom type describe an entry in an array. It's defined by an input tag and an index.
 */

struct ArrayEntry {
    public:

    ArrayEntry() = default;

    static bool isArrayEntry(lua_State* L, int index, std::string* input_tag = nullptr, size_t* array_index = nullptr) {
    
        size_t absolute_index = lua::get_index(L, index);

         if (lua_type(L, absolute_index) != LUA_TTABLE)
            return false;

         lua_len(L, absolute_index);
         size_t len = lua_tointeger(L, -1);
         lua_pop(L, 1);

         if (len != 2)
             return false;

         // Get first element of the table
         int first_type = lua_rawgeti(L, absolute_index, 1);
         if (first_type != LUA_TSTRING) {
             lua_pop(L, 1);
             return false;
         }
         int second_type = lua_rawgeti(L, absolute_index, 2);
         if (second_type != LUA_TNUMBER) {
             lua_pop(L, 2);
             return false;
         }

         std::string input_tag_str = lua_tostring(L, -2);
         int local_array_index = lua_tointeger(L, -1);

         lua_pop(L, 2);

         if (! InputTag::isInputTag(input_tag_str))
             return false;

         if (local_array_index < 0)
             return false;

         if (input_tag)
             *input_tag = input_tag_str;

         if (array_index)
             *array_index = local_array_index;

         return true;
    }

    static bool fromTable(lua_State* L, int index, ArrayEntry& entry) {

         std::string input_tag_str;
         size_t array_index;

         if (! isArrayEntry(L, index, &input_tag_str, &array_index))
             return false;

         entry = {InputTag::fromString(input_tag_str), array_index};

         return true;
    }

    template<typename T> const T get() {
        ensure_resolved();

        auto ptr = boost::any_cast<std::shared_ptr<std::vector<T>>>(value);
        return (*ptr)[index];
    }

    private:

    ArrayEntry(const InputTag& tag, size_t index):
        inputTag(tag), index(index) {
    }

    inline void ensure_resolved() {
        if (resolved)
            return;
        
        value = Pool::get().raw_get(inputTag);
        resolved = true;
    }

    InputTag inputTag;
    size_t index;

    bool resolved = false;
    boost::any value;
};

