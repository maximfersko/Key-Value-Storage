#pragma once

#include "base_storage.h"
#include "tree/stl_map.h"

namespace storage {
class SelfBalancingBinarySearchTree : public BaseStorage {
   public:
    SelfBalancingBinarySearchTree() = default;
    ~SelfBalancingBinarySearchTree() = default;
    SelfBalancingBinarySearchTree(const SelfBalancingBinarySearchTree &) =
        delete;
    SelfBalancingBinarySearchTree(const SelfBalancingBinarySearchTree &&) =
        delete;
    SelfBalancingBinarySearchTree &operator=(
        const SelfBalancingBinarySearchTree &) = delete;
    SelfBalancingBinarySearchTree &operator=(
        const SelfBalancingBinarySearchTree &&) = delete;

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

    stl::map<key_t, value_t> data_;
};

}  // namespace storage