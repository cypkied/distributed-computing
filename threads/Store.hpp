#pragma once
#include <ostream>
#include <limits>
#include <mutex>

class Store
{
public:
    using type_t = unsigned char;
    static constexpr type_t NO_TYPES = 10;
    static constexpr type_t MAX_OF_TYPES = std::numeric_limits<type_t>::max();

    void buy(const type_t type, const type_t quantity)
    {
        std::lock_guard<std::mutex> lock(mutex);
        storage[type] += quantity;
    }

    type_t sell(const type_t type, const type_t quantity)
    {
        std::lock_guard<std::mutex> lock(mutex);
        type_t buy_quantity = quantity < storage[type] ? quantity : storage[type];
        storage[type] -= buy_quantity;
        return buy_quantity;
    }

    type_t operator[](type_t index) const
    {
        std::lock_guard<std::mutex> lock(mutex);
        return storage[index];
    }

protected:
    friend std::ostream &operator<<(std::ostream &out, const Store &store)
    {
        std::lock_guard<std::mutex> lock(store.mutex);
        out << "STAN MAGAZYNU\n";
        for (Store::type_t i = 0; i < Store::NO_TYPES; ++i)
        {
            out << static_cast<size_t>(i) << ": " << static_cast<size_t>(store.storage[i]) << '\n';
        }
        return out;
    }

private:
    type_t storage[NO_TYPES] = {};
    mutable std::mutex mutex;
};