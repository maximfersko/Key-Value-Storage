#include "hash_table.h"

#include <algorithm>
namespace storage {

HashTable::HashTable() : size_(10), count_structs_(0) {
    data_.resize(size_, std::list<std::pair<key_t, value_t>>{});
}

hash_t HashTable::GetHash(const key_t &key) const {
    return std::hash<std::string>{}(key) % size_;
}

bool HashTable::Exists(const key_t &key) {
    hash_t index = GetHash(key);
    return std::any_of(data_[index].begin(), data_[index].end(),
                       [&](const auto &elm) { return elm.first == key; });
}

void HashTable::Rehash() {
    size_ *= 2;
    std::vector<std::list<std::pair<key_t, value_t>>> new_data(
        size_, std::list<std::pair<key_t, value_t>>{});
    for (auto &list : data_) {
        for (auto &[key, value] : list) {
            const auto new_hash = GetHash(key);
            auto &new_list = new_data[new_hash];
            new_list.emplace_back(key, value);
        }
    }
    data_ = std::move(new_data);
}

bool HashTable::Set(const key_t &key, const value_t &value) {
    if (Exists(key)) return false;
    ++count_structs_;
    hash_t index = GetHash(key);
    if (index >= data_.size()) data_.resize(size_);
    auto &list = data_[index];
    list.emplace_back(key, value);
    if (count_structs_ > size_ * 0.75) Rehash();
    return true;
}

std::optional<value_t> HashTable::Get(const key_t &key) {
    if (!Exists(key)) return std::nullopt;
    hash_t index = GetHash(key);
    for (const auto &[current_key, value] : data_[index])
        if (current_key == key) return value;
    return std::nullopt;
}

std::vector<std::string> HashTable::Find(const optional_value_t &value) {
    std::vector<std::string> result;
    for (const auto &list : data_) {
        for (const auto &[key, data] : list) {
            if ((value.name && data.GetName() == *value.name) ||
                (value.surname && data.GetSurname() == *value.surname) ||
                (value.birth_year &&
                 data.GetBirthYear() == *value.birth_year) ||
                (value.city && data.GetCity() == *value.city) ||
                (value.count_coins &&
                 data.GetCountCoins() == *value.count_coins)) {
                result.push_back(key);
            }
        }
    }
    return result;
}

bool HashTable::Del(const key_t &key) {
    if (!Exists(key)) return false;
    hash_t index = GetHash(key);
    auto &list = data_[index];
    auto elm = std::find_if(list.begin(), list.end(), [&](const auto &pair) {
        return pair.first == key;
    });
    list.erase(elm);
    --count_structs_;
    return true;
}

std::vector<key_t> HashTable::Keys() const {
    std::vector<key_t> keys;
    keys.reserve(data_.size());
    for (const auto &list : data_)
        std::transform(list.begin(), list.end(), std::back_inserter(keys),
                       [](const auto &elm) { return elm.first; });
    return keys;
}

bool HashTable::Rename(const key_t &old_key, const key_t &new_key) {
    if (!Exists(old_key)) return false;
    hash_t old_index = GetHash(old_key);
    hash_t new_index = GetHash(new_key);
    auto &old_list = data_[old_index];
    auto old_it = std::find_if(
        old_list.begin(), old_list.end(),
        [&old_key](const auto &pair) { return pair.first == old_key; });
    old_it->first = new_key;
    if (old_index != new_index) {
        auto &new_list = data_[new_index];
        new_list.push_back(std::move(*old_it));
        old_list.erase(old_it);
    }
    return true;
}

void HashTable::DeleteOldData() {
    for (const auto &list : data_)
        for (const auto &[key, value] : list)
            if (!value.TTL()) Del(key);
}

bool HashTable::Update(const key_t &key, const optional_value_t &value) {
    if (!Exists(key)) return false;
    hash_t index = GetHash(key);
    auto &list = data_[index];
    for (auto &[current_key, current_value] : list) {
        if (current_key == key) {
            if (value.surname) current_value.SetSurname(*value.surname);
            if (value.name) current_value.SetName(*value.name);
            if (value.city) current_value.SetCity(*value.city);
            if (value.birth_year) current_value.SetBirthYear(*value.birth_year);
            if (value.count_coins)
                current_value.SetCountCoins(*value.count_coins);
            if (value.expiry_time)
                current_value.SetTimeLife(*value.expiry_time);
        }
    }
    return true;
}

unsigned int HashTable::Upload(const std::string &filename) {
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

std::string HashTable::TTL(const key_t &key) {
    if (!Exists(key)) return "null";
    hash_t index = GetHash(key);
    const auto &list = data_[index];
    auto elm = std::find_if(list.begin(), list.end(), [&](const auto &pair) {
        return pair.first == key;
    });
    return elm->second.TTL() ? std::to_string(*elm->second.TTL()) : "null";
}

unsigned int HashTable::Export(const std::string &filename) {
    std::ofstream file(filename);
    if (!file.is_open()) throw std::invalid_argument("File Error!");
    for (const auto &list : data_) {
        for (const auto &[key, value] : list) {
            file << key << " ";
            file << "\"" << value.GetSurname() << "\" ";
            file << "\"" << value.GetName() << "\" ";
            file << value.GetBirthYear() << " ";
            file << "\"" << value.GetCity() << "\" ";
            file << value.GetCountCoins() << " ";
            if (value.GetTimeLife()) file << *value.GetTimeLife() << std::endl;
        }
    }
    return count_structs_;
}

void HashTable::ShowAll() const {
    std::cout << std::setw(5) << "№"
              << " | " << std::setw(13) << "Фамилия"
              << " | " << std::setw(13) << "Имя"
              << " | " << std::setw(5) << "Год"
              << " | " << std::setw(13) << "Город"
              << " | " << std::setw(14) << "Количество коинов"
              << " |" << std::endl;
    Print();
}

void HashTable::Print() const {
    for (const auto &list : data_)
        for (const auto &[key, value] : list) value.Print(key);
}

HashTable::~HashTable() { data_.clear(); }

}  // namespace storage