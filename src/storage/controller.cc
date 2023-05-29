#include "controller.h"

namespace storage {

Controller::Controller(TypeHashTable type) {
    if (type == TypeHashTable::kHashTable) {
        key_value_storage_ = std::make_unique<HashTable>();
    } else if (type == TypeHashTable::kSelfBalancingTree) {
        key_value_storage_ = std::make_unique<SelfBalancingBinarySearchTree>();
    }
}

bool Controller::Set(const key_t &key, const value_t &value) {
    return key_value_storage_->Set(key, value);
}

std::optional<value_t> Controller::Get(const key_t &key) {
    return key_value_storage_->Get(key);
}

bool Controller::Rename(const key_t &old_key, const key_t &new_key) {
    return key_value_storage_->Rename(old_key, new_key);
}

bool Controller::Del(const key_t &key) { return key_value_storage_->Del(key); }

std::vector<key_t> Controller::Keys() const {
    return key_value_storage_->Keys();
}

bool Controller::Update(const key_t &key, const optional_value_t &value) {
    return key_value_storage_->Update(key, value);
}

bool Controller::Exists(const key_t &key) {
    return key_value_storage_->Exists(key);
}

std::vector<std::string> Controller::Find(const optional_value_t &value) {
    return key_value_storage_->Find(value);
}

std::string Controller::TTL(const key_t &key) {
    return key_value_storage_->TTL(key);
}

unsigned int Controller::Upload(const std::string &filename) {
    unsigned int str_cout = 0;
    try {
        str_cout = key_value_storage_->Upload(filename);
    } catch (const std::invalid_argument &e) {
        std::cerr << e.what() << '\n';
    }
    return str_cout;
}

unsigned int Controller::Export(const std::string &filename) {
    unsigned int str_cout = 0;
    try {
        str_cout = key_value_storage_->Export(filename);
    } catch (const std::invalid_argument &e) {
        std::cerr << e.what() << '\n';
    }
    return str_cout;
}

void Controller::DeleteOldData() { key_value_storage_->DeleteOldData(); }

void Controller::ShowAll() const { key_value_storage_->ShowAll(); }

}  // namespace storage