#include "self_balancing_binary_search_tree.h"

namespace storage {

bool SelfBalancingBinarySearchTree::Set(const key_t &key,
                                        const value_t &value) {
    return data_.insert(std::make_pair(key, value)).second;
}

std::optional<value_t> SelfBalancingBinarySearchTree::Get(const key_t &key) {
    if (!Exists(key)) return std::nullopt;
    return std::make_optional(data_[key]);
}

bool SelfBalancingBinarySearchTree::Exists(const key_t &key) {
    return data_.contains(key);
}

bool SelfBalancingBinarySearchTree::Rename(const key_t &old_key,
                                           const key_t &new_key) {
    if (!data_.contains(old_key) || data_.contains(new_key)) return false;
    auto node = data_[old_key];
    Del(old_key);
    data_.insert(std::make_pair(new_key, node));
    return true;
}

bool SelfBalancingBinarySearchTree::Del(const key_t &key) {
    auto [iterator, is_find] = data_.search(key);
    if (!is_find) return false;
    data_.erase(iterator);
    return true;
}

std::vector<key_t> SelfBalancingBinarySearchTree::Keys() const {
    std::vector<key_t> keys;
    for (auto node : data_) keys.push_back(node.first);
    return keys;
}

bool SelfBalancingBinarySearchTree::Update(const key_t &key,
                                           const optional_value_t &value) {
    auto [iterator, is_find] = data_.search(key);
    if (!is_find) return false;
    if (value.surname) (*iterator).second.SetSurname(*value.surname);
    if (value.name) (*iterator).second.SetName(*value.name);
    if (value.birth_year) (*iterator).second.SetBirthYear(*value.birth_year);
    if (value.city) (*iterator).second.SetCity(*value.city);
    if (value.count_coins) (*iterator).second.SetCountCoins(*value.count_coins);
    if (value.expiry_time) (*iterator).second.SetTimeLife(*value.expiry_time);
    return true;
}

unsigned int SelfBalancingBinarySearchTree::Upload(
    const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) throw std::invalid_argument("File Error!");
    std::string line;
    unsigned int count = 0;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key, surname, name, city;
        int birth_year;
        long count_coins, time_life;
        if (!(iss >> key)) continue;
        std::getline(iss >> std::ws, surname, ' ');
        std::getline(iss >> std::ws, name, ' ');
        iss >> birth_year >> std::ws;
        std::getline(iss >> std::ws, city, ' ');
        iss >> count_coins >> time_life;
        value_t value(surname, name, birth_year, city, count_coins, time_life);
        if (Set(key, value)) count++;
    }
    return count;
}

std::string SelfBalancingBinarySearchTree::TTL(const key_t &key) {
    auto [iterator, is_find] = data_.search(key);
    if (!is_find) return "null";
    return (*iterator).second.TTL()
               ? std::to_string(*((*iterator).second.TTL()))
               : "null";
}

unsigned int SelfBalancingBinarySearchTree::Export(
    const std::string &filename) {
    std::ofstream file(filename);
    if (!file.is_open()) throw std::invalid_argument("File Error!");
    for (const auto &[key, value] : data_) {
        file << key << " ";
        file << "\"" << value.GetSurname() << "\" ";
        file << "\"" << value.GetName() << "\" ";
        file << value.GetBirthYear() << " ";
        file << "\"" << value.GetCity() << "\" ";
        file << value.GetCountCoins() << " ";
        if (value.GetTimeLife()) file << *value.GetTimeLife() << std::endl;
    }
    return 0;
}

void SelfBalancingBinarySearchTree::Print() const {
    for (auto it = data_.begin(); it != data_.end(); ++it) {
        (*it).second.Print((*it).first);
    }
}

void SelfBalancingBinarySearchTree::ShowAll() const {
    std::cout << std::setw(5) << "№"
              << " | " << std::setw(13) << "Фамилия"
              << " | " << std::setw(13) << "Имя"
              << " | " << std::setw(5) << "Год"
              << " | " << std::setw(13) << "Город"
              << " | " << std::setw(14) << "Количество коинов"
              << " |" << std::endl;
    Print();
}

void SelfBalancingBinarySearchTree::DeleteOldData() {
    for (auto it = data_.begin(); it != data_.end(); ++it)
        if (!(*it).second.TTL()) Del((*it).first);
}

std::vector<std::string> SelfBalancingBinarySearchTree::Find(
    const optional_value_t &value) {
    std::vector<std::string> result;
    for (const auto &[key, data] : data_) {
        if ((value.surname && data.GetSurname() == *value.surname) ||
            (value.name && data.GetName() == *value.name) ||
            (value.birth_year && data.GetBirthYear() == *value.birth_year) ||
            (value.city && data.GetCity() == *value.city) ||
            (value.count_coins && data.GetCountCoins() == *value.count_coins)) {
            result.push_back(key);
        }
    }
    return result;
}

}  // namespace storage