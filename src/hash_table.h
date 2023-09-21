#pragma once

#include "base_storage.h"

namespace storage {

class HashTable : public BaseStorage {
   public:
    HashTable();
    HashTable(const HashTable &other) = delete;
    HashTable(const HashTable &&other) = delete;
    HashTable &operator=(const HashTable &other) = delete;
    ~HashTable();

    bool Set(const key_t &key, const value_t &value) override final;
    std::optional<value_t> Get(const key_t &key) override final;
    bool Rename(const key_t &old_key, const key_t &new_key) override final;
    bool Del(const key_t &key) override final;
    std::vector<key_t> Keys() const override final;
    bool Update(const key_t &key, const optional_value_t &value) override final;
    bool Exists(const key_t &key) override final;
    std::vector<std::string> Find(const optional_value_t &value) override final;
    std::string TTL(const key_t &key) override final;
    unsigned int Upload(const std::string &filename) override final;
    unsigned int Export(const std::string &filename) override final;
    void ShowAll() const override final;
    void DeleteOldData() override final;

   private:
    void Print() const;
    hash_t GetHash(const key_t &key) const;
    void Rehash();

    unsigned int size_;
    unsigned int count_structs_;
    std::vector<std::list<std::pair<key_t, value_t>>> data_;
};

}  // namespace storage
