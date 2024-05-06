#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <corecrt.h>
#include <string>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <cmath>
#include <ostream>
#include <sstream>
#include <algorithm>
#include <utility>

#define BIG_INTEGER_BASE 4'294'967'296

class BigInt final {

private:

    void removeExtraZeros(BigInt &);
    std::pair<BigInt, BigInt> divide(BigInt const &);

private:

    bool negative_ = false;
    std::vector<uint32_t> digits_ = {0};

public:

    bool eqZero() const;

    BigInt(
            const unsigned int *digits,
            size_t size);

    explicit BigInt(
            std::vector<unsigned int> const &digits);

    BigInt(
            uint32_t number);

    BigInt(
            int32_t number);

    explicit BigInt(
            std::string const &value,
            uint32_t base = 10);

    BigInt() = default;

public:
    explicit operator uint32_t() {
        return digits_.front();
    }

    BigInt &operator+=(
            const BigInt &obj);

    BigInt operator+(
            const BigInt &obj) const;

    BigInt &operator-=(
            const BigInt &obj);

    BigInt operator-(
            const BigInt &obj) const;

    BigInt operator-() const;

    BigInt &operator*=(
            const BigInt &obj);

    BigInt operator*(
            const BigInt &obj) const;

    BigInt &operator/=(
            const BigInt &obj);

    BigInt operator/(
            const BigInt &obj) const;

    BigInt &operator%=(
            const BigInt &obj);

    BigInt operator%(
            const BigInt &obj) const;

    bool operator==(
            const BigInt &obj) const;

    bool operator!=(
            const BigInt &obj) const;

    bool operator<(
            const BigInt &obj) const;

    bool operator<=(
            const BigInt &obj) const;

    bool operator>(
            const BigInt &obj) const;

    bool operator>=(
            const BigInt &obj) const;

    BigInt operator~() const;

    BigInt operator&(
            const BigInt &obj) const;

    BigInt operator|(
            const BigInt &obj) const;

    BigInt operator^(
            const BigInt &obj) const;

    BigInt operator<<(
            size_t shift_value) const;

    BigInt operator>>(
            size_t shift_value) const;

    friend std::ostream &operator<<(
            std::ostream &stream,
            const BigInt &value);

    friend std::istream &operator>>(
            std::istream &stream,
            BigInt &value);

};

#endif //BIG_INTEGER_H