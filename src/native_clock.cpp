#include "native_clock.hpp"

#include <chrono>

#include "interpreter.hpp"

int NativeClock::arity()
{
    return 0;
}

LoxValuePtr NativeClock::call(Interpreter& interpreter, std::vector<LoxValuePtr>& args)
{
    auto now = std::chrono::system_clock::now();
    auto epoch = now.time_since_epoch();
    auto ms_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);

    return std::make_shared<LoxFloat>(((double)ms_since_epoch.count()) / 1000.0F);
}
    
std::ostream& NativeClock::operator<<(std::ostream& o)
{
    o << "<native-fn>";
    return o;
}

