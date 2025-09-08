#ifndef MAGIC_HOLDER
#define MAGIC_HOLDER

#include <cstdint>

/* Interface */
struct MagicHolder
{
    // for safty in case new  derived classes has any resources
    virtual ~MagicHolder() = default;

    [[nodiscard("PURE FUN")]] virtual uint64_t occupanciesMask(const int square) const noexcept = 0;

    // Generate all possible attacks from square
    [[nodiscard("PURE FUN")]] virtual uint64_t attacksMask(const int square, const uint64_t block) const noexcept = 0;
};

#endif
