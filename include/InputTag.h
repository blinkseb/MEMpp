#pragma once

#include <string>
#include <vector>

std::vector<std::string> split(const std::string& s, const std::string& delimiters);

struct InputTag {
    public:
        InputTag(const std::string& module, const std::string& parameter):
            module(module), parameter(parameter) {
            string_representation = module + "::" + parameter;
        }

        InputTag() = default;

        /*!
         *  Check if a given string is an input tag. Expected format is
         *      Module::Parameter
         *
         *  Delimiter is '::'.
         */
        static bool isInputTag(const std::string& tag) {
            return tag.find("::") != std::string::npos;
        }

        /*!
         * Create a input tag from a string. No check is performed to ensure that
         * the string is an input tag. Use `isInputTag` first.
         */
        static InputTag fromString(const std::string& tag) {
            auto tags = split(tag, "::");

            return InputTag(tags[0], tags[1]);
        }

        bool operator==(const InputTag& rhs) const {
            return ((module == rhs.module) && (parameter == rhs.parameter));
        }

        std::string toString() const {
            return string_representation;
        }

        std::string module;
        std::string parameter;

        std::string string_representation;
};

namespace std {
    template<>
        struct hash<InputTag> {
            size_t operator()(const InputTag& tag) const {
                return string_hash(tag.module) + string_hash(tag.parameter);
            }

            std::hash<std::string> string_hash;
        };
};

