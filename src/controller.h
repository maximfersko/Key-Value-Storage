#pragma once

#include "base_storage.h"
#include "hash_table.h"
#include "self_balancing_binary_search_tree.h"

namespace storage {

enum class TypeHashTable { kHashTable = 0, kSelfBalancingTree };
class Controller {
   public:
    explicit Controller(TypeHashTable type = TypeHashTable::kHashTable);
    Controller(const Controller &) = delete;
    Controller(const Controller &&) = delete;
    Controller &operator=(const Controller &) = delete;
    Controller &operator=(const Controller &&) = delete;
    ~Controller() = default;

    bool Set(const key_t &key, const value_t &value);
    [[nodiscard]] std::optional<value_t> Get(const key_t &key);
    bool Rename(const key_t &old_key, const key_t &new_key);
    bool Del(const key_t &key);
    [[nodiscard]] std::vector<key_t> Keys() const;
    bool Update(const key_t &key, const optional_value_t &value);
    bool Exists(const key_t &key);
    [[nodiscard]] std::vector<std::string> Find(const optional_value_t &value);
    [[nodiscard]] std::string TTL(const key_t &key);
    unsigned int Upload(const std::string &filename);
    unsigned int Export(const std::string &filename);
    void ShowAll() const;
    void DeleteOldData();

   private:
    std::unique_ptr<BaseStorage> key_value_storage_;
};

}  // namespace storage