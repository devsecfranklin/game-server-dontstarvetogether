#ifndef FYP_FMT_HH_HH
#define FYP_FMT_HH_HH

#include <iostream>
#include <memory>
#include <sstream>

namespace fyp {

namespace Colors {
    static const char* CYAN = "\x1b[36m";
    static const char* BLUE = "\x1b[34m";
    static const char* RED = "\x1b[31m";
    static const char* WHITE = "\x1b[97m";
    static const char* GREEN = "\x1b[32m";
    static const char* NORM = "\x1b[0m";
}

struct Pout {
    template <typename T>
    Pout& operator<<(const T& x) {
        std::cout << x;
        return *this;
    }

    inline Pout(std::string const& color, std::string const& prefix) {
        std::cout << color << prefix << " ";
    }

    inline Pout(std::string const& color) {
        std::cout << color;
    }

    inline ~Pout() {
        std::cout << Colors::NORM;
    }

    inline Pout& operator<<(std::ostream& (*f)(std::ostream&)) {
        std::cout << f;
        return *this;
    }

    inline Pout& operator<<(std::ostream& (*f)(std::ios&)) {
        std::cout << f;
        return *this;
    }

    inline Pout& operator<<(std::ostream& (*f)(std::ios_base&)) {
        std::cout << f;
        return *this;
    }
};

// template <>
// inline Pout& Pout::operator << (const Pout &x) {
//     std::cout << x;
//     return *this;
// }

struct PoutFactory {
    inline PoutFactory(std::string const& prefix)
        : m_prefix(prefix) {
    }

    inline Pout red() { return Pout(Colors::RED, m_prefix); }
    inline Pout cyan() { return Pout(Colors::CYAN, m_prefix); }
    inline Pout blue() { return Pout(Colors::BLUE, m_prefix); }
    inline Pout white() { return Pout(Colors::WHITE, m_prefix); }
    inline Pout green() { return Pout(Colors::GREEN, m_prefix); }
    inline Pout norm() { return Pout(Colors::NORM, m_prefix); }

    std::string m_prefix;
    std::ios_base::Init _ios_base;
};

} // namespace fyp

#endif /* FYP_FMT_HH_HH */
