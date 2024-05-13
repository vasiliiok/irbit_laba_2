#include <ctime>
#include "big_integer.h"
// TODO Деление через битовые сдвиги
// TODO std::optional<T>
// TODO std::variant
// TODO Вывод не по одной цифре
std::pair<BigInt, BigInt> BigInt::divide(BigInt const &divisor) const {
    //TODO: Как это работает??
    if (divisor.digits_.size() == 1) {
        BigInt copy = *this;
        uint64_t temp;
        uint32_t carry = 0;

        for (int i = copy.digits_.size() - 1; i >= 0; --i) {

            temp = (BIG_INTEGER_BASE *
                    static_cast<uint64_t>(carry) +
                    static_cast<uint64_t>(copy.digits_[i]));

            copy.digits_[i] = (temp /
                    static_cast<uint64_t>(divisor.digits_.front()));

            carry = (temp %
                    static_cast<uint64_t>(divisor.digits_.front()));
        }

        removeExtraZeros(copy);

        return {copy, carry};
    }

    // Divisor's length GT 1
    BigInt start_range;
    BigInt end_range = *this;
    BigInt potential_result;
    BigInt result;
    BigInt carry;
    bool carry_less_divisor;

    do {
        potential_result = (start_range + end_range) / 2;
        result = potential_result * divisor;
        carry = *this - result;
        carry_less_divisor = (carry >= 0) && (carry < divisor);

        // Correct: Array: 1215752192 23

        if (carry_less_divisor) {
            return {potential_result, carry};
        }
        if (result > *this) {
            end_range = potential_result;
        } else {
            start_range = potential_result;
        }
    } while (potential_result != 0);

    throw std::logic_error("Error in divide function!");
}

void BigInt::removeExtraZeros(BigInt &obj) {
    size_t size = obj.digits_.size();
    while (obj.digits_[size - 1] == 0) {
        size--;
    }
    if (size == 0) {
        size = 1;
    }
    obj.digits_.resize(size);
    obj.digits_.shrink_to_fit();
}

BigInt::BigInt(uint32_t number) {
    digits_.front() = number;
}

BigInt::BigInt(int32_t number) {
    if (number < 0) {
        negative_ = true;
        number = -number;
    }

    digits_.front() = static_cast<uint32_t>(number);
}

BigInt::BigInt(
        const unsigned int *digits_arr,
        size_t size) {

    if (digits_arr == nullptr) {
        throw std::logic_error("Pointer to digits array must not be nullptr");
    }

    if (size == 0) {
        throw std::logic_error("Digits array length must be GT 0");
    }

    digits_.reserve(size);
    for (int i = 0; i < size - 1; ++i) {
        digits_.push_back(digits_arr[i]);
    }
}

BigInt::BigInt(
        const std::vector<unsigned int> &digits) {

    if (digits.empty()) {
        throw std::logic_error("Vector must not be empty");
    }

    digits_ = digits;
}

BigInt::BigInt(
        std::string const &string,
        uint32_t base) {

    // When we get num-string from which we need to get BigInt,
    // we else must consider num-string's base system.
    // max_base_characters - maximum number of characters,
    // we can take at a time, to add to BigInt.
    const short max_base_characters =
            std::floor(std::log10(base));

    std::string buffer;
    for (char i : string) {
        buffer.push_back(i);

        if (buffer.size() == max_base_characters) {

            *this *= base;
            *this += static_cast<uint32_t>(
                    stoi(buffer));

            buffer.clear();
        }
    }
    if (!buffer.empty()) {
        *this *= base;
        *this += static_cast<uint32_t>(
                stoi(buffer));
    }
}

BigInt &BigInt::operator+=(
    BigInt const &other) {

    if (*this == 0 && other == 0) {
        return *this;
    }

    if (*this == 0) {
        return *this = other;
    }

    if (other == 0) {
        return *this;
    }

    if (negative_ && other.negative_) {
        negative_ = false;
        *this += -other;
        negative_ = true;
        return *this;
    }

    if (negative_ && !other.negative_) {
        return
            *this = other - (-(*this));
    }

    if (!negative_ && other.negative_) {
        return *this -= -other;
    }

    size_t other_size = other.digits_.size();
    bool other_bigger_equal = other_size >= digits_.size();

    if (other_bigger_equal) {
        digits_.reserve(other_size + 1);
        digits_.resize(other_size);
    }

    uint64_t temp;
    short carry = 0;
    for (size_t i = 0; i < other_size; ++i) {
        temp = (static_cast<uint64_t>(digits_[i]) +
                static_cast<uint64_t>(other.digits_[i]) +
                static_cast<uint64_t>(carry));

        if (temp >= BIG_INTEGER_BASE) {
            digits_[i] = temp - BIG_INTEGER_BASE;
            carry = 1;
        } else {
            digits_[i] = temp;
            carry = 0;
        }
    }

    if (carry) {
        if (!other_bigger_equal) {
            for (size_t i = other_size; i < digits_.size(); ++i) {
                if (digits_[i] == BIG_INTEGER_BASE - 1) {
                    digits_[i] = 0;
                } else {
                    digits_[i] += carry;
                    return *this;
                }
            }
            digits_.reserve(digits_.size() + 1);
        }
        digits_.resize(digits_.size() + 1);
        digits_[digits_.size() - 1] = carry;
    }

    return *this;
}

BigInt BigInt::operator+(
    BigInt const &other) const {
    return BigInt(*this) += other;
}

BigInt BigInt::operator-() const {
    BigInt temp = *this;
    temp.negative_ = !temp.negative_;
    return temp;
}

BigInt &BigInt::operator-=(
    BigInt const &other) {

    // If smt negative - do it positive
    // and use +=
    if (negative_ && other.negative_) {
        return *this += -other;
    }

    if (negative_ && !other.negative_) {
        negative_ = false;
        *this += other;
        negative_ = true;
        return *this;
    }

    if (!negative_ && other.negative_) {
        return *this += other;
    }

    // Check is smt EQ zero
    if (*this == 0 && other == 0) {
        return *this;
    }

    if (*this == 0) {
        *this = other;
        if (negative_) {
            negative_ = false;
        }
        return *this;
    }

    if (other == 0) {
        return *this;
    }

    if (*this < other) {
        *this = other - *this;
        negative_ = true;
        return *this;
    }

    if (*this == other) {
        return *this = 0;
    }

    // positive - positive
    int64_t temp;
    short carry = 0;
    size_t other_size = other.digits_.size();
    for (int i = 0; i < digits_.size(); i++) {

        temp = (int64_t)digits_[i] + carry;
        if (i < other_size) {
            temp -= other.digits_[i];
        }

        if (temp < 0) {
            temp += BIG_INTEGER_BASE;
            carry = -1;
        } else {
            carry = 0;
        }

        digits_[i] = temp;
    }

    removeExtraZeros(*this);

    return *this;
}

BigInt BigInt::operator-(
    BigInt const &other) const {
    return BigInt(*this) -= other;
}

BigInt &BigInt::operator*=(
    BigInt const &other) {

    if (!negative_ && other.negative_) {
        negative_ = true;
    } else if (negative_ && other.negative_) {
        negative_ = false;
    }

    if (*this == 0 || other == 0) {
        *this = 0;
        return *this;
    }

    std::vector<uint32_t> result;
    size_t max_digit = digits_.size() + other.digits_.size();
    result.reserve(max_digit);
    result.resize(max_digit - 1);

    uint64_t temp;
    uint64_t carry = 0;
    size_t position;
    for (size_t i = 0; i < other.digits_.size(); ++i) {
        for (size_t j = 0; j < digits_.size(); ++j) {
            position = i + j;

            temp = (static_cast<uint64_t>(other.digits_[i]) *
                    static_cast<uint64_t>(digits_[j]) +
                    carry);

            if (temp >= BIG_INTEGER_BASE) {
                carry = temp / BIG_INTEGER_BASE;
            } else {
                carry = 0;
            }

            result[position] += temp - BIG_INTEGER_BASE * carry;
        }
        if (carry) {
            if (position + 1 == result.size()) {
                result.resize(max_digit);
            }
            result[position + 1] += carry;
            carry = 0;
        }
    }

    digits_ = result;

    return *this;
}

BigInt BigInt::operator*(
    BigInt const &other) const {
    return BigInt(*this) *= other;
}

BigInt &BigInt::operator/=(
    BigInt const &other) {

    if (other == 0) {
        throw std::logic_error("Division by zero!");
    }

    if (*this == 0) {
        return *this;
    }

    if (!negative_ && other.negative_) {
        negative_ = true;
    } else {
        negative_ = false;
    }

    if (*this == other) {
        return *this = 1;
    }

    if (*this < other) {
        return *this = 0;
    }

    return (*this = divide(other).first);
}


BigInt BigInt::operator/(
    BigInt const &other) const {
    return divide(other).first;
}

BigInt &BigInt::operator%=(
    BigInt const &other) {
    return *this = divide(other).second;
}

BigInt BigInt::operator%(
    BigInt const &other) const {
    BigInt temp = *this;
    return temp.divide(other).second;
}

bool BigInt::operator==(
    BigInt const &other) const {

    if (negative_ != other.negative_) {
        return false;
    }

    if (digits_.size() != other.digits_.size()) {
        return false;
    }

    for (int i = 0; i < digits_.size(); ++i) {
        if (digits_[i] != other.digits_[i]) {
            return false;
        }
    }

    return true;
}

bool BigInt::operator!=(
    BigInt const &other) const {
    return !(other == *this);
}

bool BigInt::operator<(
    BigInt const &other) const {

    if (negative_ && !other.negative_) {
        return true;
    }

    if (!negative_ && other.negative_) {
        return false;
    }

    if (digits_.size() < other.digits_.size()) {
        return true;
    }

    if (digits_.size() == other.digits_.size()) {
        for (int i = 0; i < digits_.size(); ++i) {
            size_t index = digits_.size() - i - 1;

            if (digits_[index] < other.digits_[index]) {
                return true;
            }
            else if (digits_[index] > other.digits_[index]) {
                return false;
            }
        }
    }

    return false;
}

bool BigInt::operator<=(
    BigInt const &other) const {

    if (negative_ && !other.negative_) {
        return true;
    }

    if (!negative_ && other.negative_) {
        return false;
    }

    if (digits_.size() < other.digits_.size()) {
        return true;
    }

    if (digits_.size() == other.digits_.size()) {
        for (int i = 0; i < digits_.size(); ++i) {
            size_t index = digits_.size() - i - 1;

            if (digits_[index] < other.digits_[index]) {
                return true;
            }
            else if (digits_[index] > other.digits_[index]) {
                return false;
            }
        }
        return true;
    }

    return false;
}

bool BigInt::operator>(
    BigInt const &other) const {
    return other < *this;
}

bool BigInt::operator>=(
    BigInt const &other) const {
    return other <= *this;
}

BigInt BigInt::operator~() const {
    BigInt copy = *this;
    for (auto &digit : copy.digits_) {
        digit = ~digit;
    }
    return copy;
}

BigInt BigInt::operator&(
    BigInt const &other) const {

    BigInt result;
    size_t max_size = std::max(other.digits_.size(), digits_.size());
    result.digits_.reserve(max_size);
    result.digits_.resize(max_size);

    for (auto i = 0; i < max_size; ++i) {
        uint32_t first_digit = digits_[i] ? i < digits_.size() : 0;
        uint32_t second_digit = other.digits_[i] ? i < other.digits_.size() : 0;

        result.digits_[i] = first_digit & second_digit;
    }

    return result;
}

BigInt BigInt::operator|(
    BigInt const &other) const {

    BigInt result;
    size_t max_size = std::max(other.digits_.size(), digits_.size());
    result.digits_.reserve(max_size);
    result.digits_.resize(max_size);

    for (auto i = 0; i < max_size; ++i) {
        uint32_t first_digit = digits_[i] ? i < digits_.size() : 0;
        uint32_t second_digit = other.digits_[i] ? i < other.digits_.size() : 0;

        result.digits_[i] = first_digit | second_digit;
    }

    return result;
}

BigInt BigInt::operator^(
    BigInt const &other) const {

    BigInt result;
    size_t max_size = std::max(other.digits_.size(), digits_.size());
    result.digits_.reserve(max_size);
    result.digits_.resize(max_size);

    for (auto i = 0; i < max_size; ++i) {
        uint32_t first_digit = digits_[i] ? i < digits_.size() : 0;
        uint32_t second_digit = other.digits_[i] ? i < other.digits_.size() : 0;

        result.digits_[i] = first_digit ^ second_digit;
    }

    return result;
}

BigInt BigInt::operator<<(
    size_t shift_value) const {

    BigInt copy = *this;
    for (uint32_t &digit : copy.digits_) {
        digit <<= shift_value;
    }

    return copy;
}

BigInt BigInt::operator>>(
    size_t shift_value) const {

    BigInt copy = *this;
    for (uint32_t &digit : copy.digits_) {
        shift_value >>= digit;
    }

    return copy;
}

std::ostream &operator<<(
    std::ostream &stream,
    const BigInt &value) {

    stream << "Array: ";
    for (auto num : value.digits_) {
        stream << num << " ";
    }
    stream << "\n";

    //---------------------------------
    //uint64_t start_time = std::clock();
    //---------------------------------

    if (value == 0) {
        stream << 0;
        return stream;
    }

    BigInt temp = value;
    if (temp < 0) {
        temp.negative_ = false;
    }

    std::ostringstream result_stream;
    std::string result;

    while(temp > 0) {
        auto divide_result = temp.divide(10);
        temp = divide_result.first;
        result_stream << uint32_t(divide_result.second);
    }
    if (value < 0) {
        result_stream << '-';
    }
    result = result_stream.str();
    std::reverse(result.begin(), result.end());

    //---------------------------------
    //stream << "Conversion to 10 base takes: " << std::clock() - start_time << " milliseconds"  << std::endl  << std::endl;
    //---------------------------------

    stream << result;

    return stream;
}

// TODO: Что оно должно делать?
std::istream &operator>>(
    std::istream &stream,
    BigInt &value) {

    std::string input;
    stream >> input;
    value = BigInt(input);

    return stream;
}