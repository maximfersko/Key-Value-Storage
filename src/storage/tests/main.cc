#include <gtest/gtest-spi.h>
#include <gtest/gtest.h>

#include <chrono>
#include <thread>

#include "../controller.h"

const storage::value_t John("John", "Doe", 1995, "New York", 6789L, 4);
const storage::value_t Jane("Jane", "Doe", 1997, "Los Angeles", 3200L, 12);
const storage::value_t Alice("Alice", "Smith", 1990, "Chicago", 1500L, 6);
const storage::value_t Bob("Bob", "Johnson", 1985, "Houston", 2450L, 7);
const storage::value_t Mary("Mary", "Williams", 1982, "Philadelphia", 1234L,
                            95);

const storage::optional_value_t Jane_opt(std::nullopt, "Jane", std::nullopt,
                                         std::nullopt, 0, std::nullopt);
const storage::optional_value_t John_opt("John", "Doe", std::nullopt,
                                         "New York", 6789L, std::nullopt);
const storage::optional_value_t Alice_opt("Alice", "Smith", 1990, "Chicago",
                                          std::nullopt, 85);
const storage::optional_value_t Bob_opt("Bob", "Johnson", 1985, std::nullopt,
                                        std::nullopt, std::nullopt);
const storage::optional_value_t Mary_opt("Mary", "Williams", 1982,
                                         "Philadelphia", 1234L, 95);

const storage::key_t first_key = "doctor";
const storage::key_t second_key = "lawyer";
const storage::key_t third_key = "journalist";
const storage::key_t fourth_key = "programmer";
const storage::key_t fifth_key = "marketing specialist";

TEST(first_suite_key, set_get_tree) {
    storage::Controller storage(storage::TypeHashTable::kSelfBalancingTree);
    storage.Set(first_key, John);
    ASSERT_EQ((storage.Get(first_key).value()) == John, true);
}

TEST(third_suite, set_get_tree_suite) {
    storage::Controller storage(storage::TypeHashTable::kSelfBalancingTree);
    storage.Set(fifth_key, Bob);
    storage.Set(fourth_key, Alice);
    storage.Set(third_key, John);
    storage.Set(second_key, John);
    storage.Set(first_key, Mary);
    storage.Set("1", Bob);
    storage.Set("2", Alice);
    storage.Set("3", John);
    storage.Set("4", John);
    storage.Set("5", Mary);
    storage.Set("6", Mary);
    storage.Set("7", Mary);
    storage.Set("8", Alice);
    storage.Set("40", John);
    ASSERT_EQ((storage.Get(first_key).value()) == Mary, true);
    ASSERT_TRUE(storage.Get("7").value() == Mary);
}

TEST(three_suite, set_tree) {
    storage::Controller storage(storage::TypeHashTable::kSelfBalancingTree);
    ASSERT_TRUE(storage.Set(first_key, Bob));
    ASSERT_TRUE(storage.Set(second_key, Alice));
}

TEST(first_suite, get_tree) {
    storage::Controller storage(storage::TypeHashTable::kSelfBalancingTree);
    storage.Set(first_key, Jane);
    storage.Set(second_key, Bob);
    storage.Set(third_key, John);
    ASSERT_TRUE(storage.Exists(first_key));
    ASSERT_FALSE(storage.Exists(fifth_key));
    ASSERT_TRUE(storage.Exists(second_key));
    ASSERT_TRUE(storage.Exists(third_key));
}

TEST(delete_suite2, delete_elm_second_tree) {
    storage::Controller storage(storage::TypeHashTable::kSelfBalancingTree);
    storage.Set(fifth_key, Bob);
    storage.Set(fourth_key, Alice);
    storage.Set(third_key, John);
    for (auto key :
         std::vector<storage::key_t>({fifth_key, fourth_key, third_key})) {
        storage.Del(key);
        ASSERT_FALSE(storage.Exists(key));
    }
}

TEST(first_suite, get_all_keys) {
    storage::Controller storage;
    storage.Set(first_key, John);
    storage.Set(second_key, Jane);
    storage.Set(third_key, Alice);
    std::vector<storage::key_t> expected_keys = {first_key, second_key,
                                                 third_key};
    std::sort(expected_keys.begin(), expected_keys.end());
    ASSERT_EQ(storage.Keys(), expected_keys);
}

TEST(first_suite_tree, rename_tree) {
    storage::Controller storage(storage::TypeHashTable::kSelfBalancingTree);
    storage.Set(first_key, John);
    storage.Set(second_key, Jane);
    storage.Set(third_key, Alice);
    ASSERT_TRUE(storage.Rename(second_key, fifth_key));
    ASSERT_TRUE(*storage.Get(fifth_key) == Jane);
}

TEST(first_suite_tree, TTL_tree) {
    storage::Controller storage(storage::TypeHashTable::kSelfBalancingTree);
    storage.Set(first_key, John);
    storage.Set(second_key, Jane);
    storage.Set(third_key, storage::value_t("Mary", "Williams", 1982,
                                            "Philadelphia", 1234L, 5L));
    std::this_thread::sleep_for(std::chrono::seconds(5));
    ASSERT_EQ(storage.TTL(third_key), "null");
}

TEST(first_suite, set_get_hash) {
    storage::Controller storage;
    storage.Set(first_key, John);
    ASSERT_EQ((storage.Get(first_key).value()) == John, true);
}

TEST(third_suite, set_get_hash_suite) {
    storage::Controller storage;
    storage.Set(fifth_key, Bob);
    storage.Set(fourth_key, Alice);
    storage.Set(third_key, John);
    storage.Set(second_key, John);
    storage.Set(first_key, Mary);
    storage.Set("1", Bob);
    storage.Set("2", Alice);
    storage.Set("3", John);
    storage.Set("4", John);
    storage.Set("5", Mary);
    storage.Set("6", Mary);
    storage.Set("7", Mary);
    storage.Set("8", Alice);
    storage.Set("9", John);
    storage.Set("40", John);
    ASSERT_EQ((storage.Get(first_key).value()) == Mary, true);
    ASSERT_TRUE(storage.Get("7").value() == Mary);
}

TEST(second_suite, set_get1) {
    storage::Controller storage;
    storage.Set(first_key, John);
    storage.Set(second_key, Mary);
    ASSERT_EQ((storage.Get(first_key).value()) == John, true);
}

TEST(delete_suite2, delete_elm_second) {
    storage::Controller storage;
    storage.Set(fifth_key, Bob);
    storage.Set(fourth_key, Alice);
    storage.Set(third_key, John);
    for (auto key :
         std::vector<storage::key_t>({fifth_key, fourth_key, third_key})) {
        storage.Del(key);
        ASSERT_FALSE(storage.Exists(key));
    }
}

TEST(FindTest, FindBySurname) {
    storage::Controller storage;
    storage.Set("1",
                storage::value_t("Smith", "John", 1990, "New York", 100, 10));
    storage.Set("2",
                storage::value_t("Johnson", "Jane", 1985, "London", 200, 20));
    storage.Set("3",
                storage::value_t("Smith", "Alice", 1995, "Paris", 150, 15));
    storage::optional_value_t value;
    value.surname = "Smith";
    std::vector<std::string> result = storage.Find(value);
    std::vector<std::string> expected = {"1", "3"};
    ASSERT_EQ(result, expected);
}

TEST(update_sutie, update) {
    storage::Controller storage;
    storage.Set("1", Bob);
    storage.Set("2", Alice);
    storage.Set("3", John);
    storage.Set("4", John);
    storage.Update("1", Mary_opt);
    ASSERT_TRUE(storage.Get("1").value() == Mary);
}

TEST(first_suite, set_get_rename) {
    storage::Controller storage;
    storage.Set(first_key, John);
    storage.Rename(first_key, second_key);
    ASSERT_EQ((storage.Get(second_key).value()) == John, true);
}

TEST(first_suite, set_get_delete) {
    storage::Controller storage;
    ASSERT_EQ(storage.Set(first_key, John), true);
    ASSERT_EQ((storage.Get(first_key).value()) == John, true);
    ASSERT_EQ(storage.Del(first_key), true);
    ASSERT_EQ(storage.Exists(first_key), false);
}

TEST(first_suite, set_get_update) {
    storage::Controller storage;
    storage.Set(first_key, John);
    storage.Update(first_key, Mary_opt);
    ASSERT_TRUE((storage.Get(first_key).value()) == Mary);
}

TEST(FindTest, FindByCity) {
    storage::Controller storage;
    storage.Set("1",
                storage::value_t("Smith", "John", 1990, "New York", 100, 10));
    storage.Set("2",
                storage::value_t("Johnson", "Jane", 1985, "London", 200, 20));
    storage.Set("3",
                storage::value_t("Smith", "Alice", 1995, "Paris", 150, 15));
    storage::optional_value_t value;
    value.city = "Paris";
    std::vector<std::string> result = storage.Find(value);
    std::vector<std::string> expected = {"3"};
    ASSERT_EQ(result, expected);
}

TEST(delete_suite, delete_elm) {
    storage::Controller storage;
    storage.Set(fifth_key, Bob);
    storage.Set(fourth_key, Alice);
    storage.Set(third_key, John);
    storage.Del(fourth_key);
    for (auto key : storage.Keys()) {
        if (key == fifth_key)
            ASSERT_EQ(fifth_key, key);
        else
            ASSERT_EQ(third_key, key);
    }
}

TEST(first_suite, get) {
    storage::Controller storage;
    storage.Set(first_key, Jane);
    storage.Set(second_key, Bob);
    storage.Set(third_key, John);
    ASSERT_TRUE(storage.Exists(first_key));
    ASSERT_FALSE(storage.Exists(fifth_key));
    ASSERT_TRUE(storage.Exists(second_key));
    ASSERT_TRUE(storage.Exists(third_key));
}

TEST(three_suite, set) {
    storage::Controller storage;
    ASSERT_TRUE(storage.Set(first_key, Bob));
    ASSERT_TRUE(storage.Set(second_key, Alice));
    ASSERT_FALSE(storage.Set(first_key, John));
}

TEST(HashTableTest, Rehash) {
    storage::Controller table;
    for (int i = 0; i < 100; ++i) {
        storage::key_t keys = "key" + std::to_string(i);
        table.Set(keys,
                  storage::value_t("Mary" + std::to_string(i),
                                   "Williams" + std::to_string(i), 1982 + i,
                                   "Philadelphia" + std::to_string(i),
                                   1234L + i, 95 + i));
    }
    ASSERT_TRUE(table.Get("key55").value() ==
                storage::value_t("Mary55", "Williams55", 2037, "Philadelphia55",
                                 1289L, 150));
    ASSERT_TRUE(table.Get("key78").value() ==
                storage::value_t("Mary78", "Williams78", 2060, "Philadelphia78",
                                 1312L, 173));

    ASSERT_TRUE(table.Get("key32").value() ==
                storage::value_t("Mary32", "Williams32", 2014, "Philadelphia32",
                                 1266L, 127));
    ASSERT_TRUE(table.Get("key11").value() ==
                storage::value_t("Mary11", "Williams11", 1993, "Philadelphia11",
                                 1245L, 106));
    ASSERT_TRUE(table.Get("key65").value() ==
                storage::value_t("Mary65", "Williams65", 2047, "Philadelphia65",
                                 1299L, 160));
}

TEST(first_suite, import_export) {
    storage::Controller storage;
    ASSERT_TRUE(storage.Set(first_key, John));
    ASSERT_EQ(storage.Export("ex.dat"), 1);
    storage::Controller storage_new;
    ASSERT_EQ(storage_new.Upload("ex.dat"), 1);
}

TEST(first_suite_hash, rename_hash) {
    storage::Controller storage;
    storage.Set(first_key, John);
    storage.Set(second_key, Jane);
    storage.Set(third_key, Alice);
    ASSERT_TRUE(storage.Rename(second_key, fifth_key));
    ASSERT_TRUE(*storage.Get(fifth_key) == Jane);
}

TEST(first_suite_tree, TTL_hash_table) {
    storage::Controller storage;
    storage.Set(first_key, John);
    storage.Set(second_key, Jane);
    storage.Set(third_key, storage::value_t("Mary", "Williams", 1982,
                                            "Philadelphia", 1234L, 5L));
    std::this_thread::sleep_for(std::chrono::seconds(5));
    ASSERT_EQ(storage.TTL(third_key), "null");
}

TEST(first_suite_tree, TTL_hash_table_delete_old_data) {
    storage::Controller storage;
    std::vector<storage::key_t> keys = {first_key, second_key, third_key};
    storage.Set(keys[0], John);
    storage.Set(keys[1], Jane);
    storage.Set(keys[2], John);
    std::this_thread::sleep_for(std::chrono::seconds(8));
    storage.DeleteOldData();
    for (const auto &key : keys) ASSERT_FALSE(storage.Exists(key));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
