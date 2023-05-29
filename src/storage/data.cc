#include "data.h"

namespace storage {

Data::Data(const std::string &surname, const std::string &name, int birth_year,
           const std::string &city, long count_coins,
           std::optional<unsigned long> time_life)
    : surname_(surname),
      name_(name),
      birth_year_(birth_year),
      city_(city),
      count_coins_(count_coins) {
    if (time_life) SetTimeLife(*time_life);
}

Data::Data(const Data &other)
    : surname_(other.surname_),
      name_(other.name_),
      birth_year_(other.birth_year_),
      city_(other.city_),
      count_coins_(other.count_coins_),
      created_time_(other.created_time_),
      expiry_time_(other.expiry_time_) {}

Data &Data::operator=(const Data &other) {
    surname_ = other.surname_;
    name_ = other.name_;
    birth_year_ = other.birth_year_;
    city_ = other.city_;
    count_coins_ = other.count_coins_;
    expiry_time_ = other.expiry_time_;
    created_time_ = other.created_time_;
    return *this;
}

void Data::Clear() {
    surname_ = "";
    name_ = "";
    birth_year_ = 0;
    city_ = "";
    count_coins_ = 0L;
    expiry_time_ = std::nullopt;
}

void Data::SetTimeLife(unsigned long time_life) {
    created_time_ = std::chrono::system_clock::now();
    auto expiry_time = created_time_ + std::chrono::seconds(time_life);
    expiry_time_ =
        std::chrono::time_point_cast<std::chrono::seconds>(expiry_time)
            .time_since_epoch()
            .count();
}

std::optional<long> Data::TTL() const {
    if (!expiry_time_) return std::nullopt;
    auto current_time = std::chrono::system_clock::now();
    auto expiry_time = std::chrono::time_point<std::chrono::system_clock>(
        std::chrono::seconds(*expiry_time_));
    auto remaining_time = std::chrono::duration_cast<std::chrono::seconds>(
                              expiry_time - current_time)
                              .count();
    return (remaining_time > 0) ? std::optional<long>(remaining_time)
                                : std::nullopt;
}

void Data::Print(const key_t &key) const {
    const int num_width = 2;
    const int name_width = 10;
    const int city_width = 10;
    const int coins_width = 20;
    const char fill_char = '-';
    std::cout << std::setfill(fill_char) << std::setw(num_width + 3) << ""
              << std::setw(name_width + 3) << "" << std::setw(name_width + 3)
              << "" << std::setw(num_width + 3) << ""
              << std::setw(city_width + 3) << "" << std::setw(coins_width + 3)
              << "" << std::setfill(' ') << std::endl;
    std::cout << std::setw(num_width) << key << " | " << std::setw(name_width)
              << surname_ << " | " << std::setw(name_width) << name_ << " | "
              << std::setw(num_width) << birth_year_ << " | "
              << std::setw(city_width) << city_ << " | "
              << std::setw(coins_width) << count_coins_ << " |" << std::endl;
}

bool Data::operator==(const storage::Data &other) const {
    return (this->surname_ == other.GetSurname() &&
            this->name_ == other.GetName() &&
            this->birth_year_ == other.GetBirthYear() &&
            this->city_ == other.GetCity() &&
            this->count_coins_ == other.GetCountCoins());
}

}  // namespace storage