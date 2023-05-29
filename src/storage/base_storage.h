#pragma once
#include <fstream>
#include <iostream>
#include <list>
#include <optional>
#include <sstream>
#include <vector>

#include "data.h"

namespace storage {

using value_t = Data;
using optional_value_t = OptionalData;

class BaseStorage {
   public:
    BaseStorage() = default;
    virtual ~BaseStorage() = default;

    virtual bool Set(const key_t &key, const value_t &value) = 0;
    [[nodiscard]] virtual std::optional<value_t> Get(const key_t &key) = 0;
    virtual bool Rename(const key_t &old_key, const key_t &new_key) = 0;
    virtual bool Del(const key_t &key) = 0;
    [[nodiscard]] virtual std::vector<key_t> Keys() const = 0;
    virtual bool Update(const key_t &key, const optional_value_t &value) = 0;
    virtual bool Exists(const key_t &key) = 0;
    [[nodiscard]] virtual std::vector<std::string> Find(
        const optional_value_t &value) = 0;
    [[nodiscard]] virtual std::string TTL(const key_t &key) = 0;
    virtual unsigned int Upload(const std::string &filename) = 0;
    virtual unsigned int Export(const std::string &filename) = 0;
    virtual void DeleteOldData() = 0;
    virtual void ShowAll() const = 0;
};

}  // namespace storage