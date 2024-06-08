#pragma once

#include <string>

struct LoxValue
{
    friend std::ostream& operator<<(std::ostream& o, LoxValue& value);

    virtual std::ostream& operator<<(std::ostream& o) = 0;
    virtual ~LoxValue() = default;
};

struct LoxString: public LoxValue
{
    std::string value_;

    LoxString(std::string value);
    ~LoxString() = default;

    std::ostream& operator<<(std::ostream& o) override;
};

struct LoxInteger: public LoxValue
{
    int64_t value_;

    LoxInteger(int64_t value);
    ~LoxInteger() = default;

    std::ostream& operator<<(std::ostream& o) override;
};

struct LoxFloat: public LoxValue
{
    double value_;

    LoxFloat(double value);
    ~LoxFloat() = default;

    std::ostream& operator<<(std::ostream& o) override;
};

