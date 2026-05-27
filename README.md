# key-value-storage

An in-memory key-value store with two interchangeable backends - a chaining hash table and a self-balancing BST - sharing a common interface. Values are typed records (surname, name, birth year, city, coin balance) with optional TTL expiry.

## Tech Stack

C++17, Google Test, custom `stl::map` (BST), `std::hash`

## Notable Implementation Details

**Dual backend via polymorphism.** `BaseStorage` defines the full interface as pure virtuals. `Controller` holds a `std::unique_ptr<BaseStorage>` and selects the backend at construction time via `TypeHashTable` enum - `kHashTable` or `kSelfBalancingTree`. All operations delegate transparently, making the backends fully substitutable.

**Custom BST instead of `std::map`.** `SelfBalancingBinarySearchTree` uses a hand-rolled `stl::map<K,T>` built on top of `Btree<K,T>` and `treeNode<K,T>`. The tree stores `std::pair<K,T>*` on the heap per node (allocated in the `treeNode` constructor), with `nextNode()`/`prevNode()` for in-order iterator traversal without a stack.

**Hash table rehashing at 75% load.** When `count_structs_ > size_ * 0.75`, `Rehash()` doubles the bucket count and re-inserts all elements. Uses `std::hash<std::string>{}(key) % size_` - standard library hash wrapped in a custom slot function.

**TTL via `std::chrono` epoch seconds.** `Data::SetTimeLife()` stores expiry as raw `long` seconds since epoch in `std::optional<unsigned long> expiry_time_`. `TTL()` subtracts `system_clock::now()` from the expiry point and returns `std::nullopt` if expired - zero allocation, no background thread needed.

**Partial update via `OptionalData`.** `Update()` accepts an `OptionalData` struct where each field is `std::optional` - only set fields overwrite the existing record. Avoids full replacement for sparse updates.

**`Rename` in hash table** moves the node between buckets in-place: finds the old entry, changes its key, then if the new hash maps to a different bucket, physically moves the `std::list` node via `push_back(std::move(*it))` + `erase`.

## Quick Start

```bash
cd src

# Build and run
make build

# Run tests
make tests

# Run with static analysis
make cppcheck
```

## API

| Method | Description |
|---|---|
| `Set(key, value)` | Insert; returns false if key exists |
| `Get(key)` | Returns `std::optional<Data>` |
| `Update(key, partial)` | Partial field update |
| `Del(key)` | Delete by key |
| `Rename(old, new)` | Rename key |
| `Exists(key)` | Key presence check |
| `Find(partial)` | Search by any field |
| `TTL(key)` | Remaining seconds or `"null"` |
| `Upload(file)` | Bulk import from `.dat` |
| `Export(file)` | Dump to `.dat` |
| `DeleteOldData()` | Evict all expired records |
