#pragma once

#include <memory>
#include <unordered_map>

#include <boost/any.hpp>

using InputTag = std::pair<std::string, std::string>;

namespace std {
    template<>
        struct hash<InputTag> {
            size_t operator()(const InputTag& tag) const {
                return string_hash(tag.first) + string_hash(tag.second);
            }

            std::hash<std::string> string_hash;
        };
};

// A simple memory pool
class Pool {
    public:
        template<typename T> std::shared_ptr<const T> get(const InputTag& tag) {
            boost::any& v = m_pool[tag];
            std::shared_ptr<T>& ptr = boost::any_cast<std::shared_ptr<T>&>(v);

            return std::const_pointer_cast<const T>(ptr);
        }

        template<typename T> std::shared_ptr<T> put(const InputTag& tag) {
            std::shared_ptr<T> ptr(new T());
            m_pool.emplace(tag, boost::any(ptr));

            return ptr;
        }

        static Pool& get();

        static void create();
        static void destroy();

    private:
        Pool() = default;
        static Pool*& instance();

        std::unordered_map<InputTag, boost::any> m_pool;
};

