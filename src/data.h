#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>
#include <optional>
#include <string>

namespace storage {

using key_t = std::string;
using hash_t = unsigned long;

class Data {
   public:
    Data() = default;
    Data(const std::string &surname, const std::string &name, int birth_year,
         const std::string &city, long count_coins,
         std::optional<unsigned long> time_life = std::nullopt);
    Data(const Data &other);
    Data(const Data &&other) { (*this) = std::move(other); }
    Data &operator=(const Data &other);
    bool operator==(const storage::Data &other) const;

    ~Data() = default;

    std::string GetSurname() const { return surname_; }
    void SetSurname(const std::string &surname) { surname_ = surname; }
    std::string GetName() const { return name_; }
    void SetName(const std::string &name) { name_ = name; }
    int GetBirthYear() const { return birth_year_; }
    void SetBirthYear(int birth_year) { birth_year_ = birth_year; }
    long GetCountCoins() const { return count_coins_; }
    void SetCountCoins(long count_coins) { count_coins_ = count_coins; }
    std::optional<long> GetTimeLife() const { return expiry_time_; }
    std::optional<long> TTL() const;
    void SetTimeLife(unsigned long time_life);
    std::string GetCity() const { return city_; }
    void SetCity(const std::string &city) { city_ = city; }
    void Clear();
    void Print(const key_t &key) const;

   private:
    std::string surname_;
    std::string name_;
    int birth_year_;
    std::string city_;
    long count_coins_;
    std::chrono::time_point<std::chrono::system_clock> created_time_;
    std::optional<unsigned long> expiry_time_;
};

struct OptionalData {
    OptionalData() = default;
    OptionalData(const std::optional<std::string> &surname,
                 const std::optional<std::string> &name,
                 const std::optional<int> &birth_year,
                 const std::optional<std::string> &city,
                 const std::optional<long> &count_coins,
                 const std::optional<unsigned long> &expiry_time)
        : surname(surname),
          name(name),
          birth_year(birth_year),
          city(city),
          count_coins(count_coins),
          expiry_time(expiry_time) {}

    Data ToValue() {
        return (Data(surname.value(), name.value(), birth_year.value(),
                     city.value(), count_coins.value(), expiry_time.value()));
    }

    std::optional<std::string> surname;
    std::optional<std::string> name;
    std::optional<int> birth_year;
    std::optional<std::string> city;
    std::optional<long> count_coins;
    std::optional<unsigned long> expiry_time;
};

}  // namespace storage