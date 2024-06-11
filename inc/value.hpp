#pragma once

#include <string>
#include <memory>

struct LoxValue
{
    friend std::ostream& operator<<(std::ostream& o, LoxValue& value);

    virtual std::ostream& operator<<(std::ostream& o) = 0;
    virtual ~LoxValue() = default;
};

using LoxValuePtr = std::shared_ptr<LoxValue>;

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

struct LoxBool: public LoxValue
{
    bool value_;

    LoxBool(bool value);
    ~LoxBool() = default;

    std::ostream& operator<<(std::ostream& o) override;
};

struct LoxNil: public LoxValue
{
    ~LoxNil() = default;

    std::ostream& operator<<(std::ostream& o) override;
};

