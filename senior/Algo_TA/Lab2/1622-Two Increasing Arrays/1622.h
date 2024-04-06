#include <vector>
#include <cstdint>

class Number {
protected:
    uint32_t info;
public:
    bool operator <  (const Number &rhs) const;
    bool operator >  (const Number &rhs) const;
    bool operator <= (const Number &rhs) const;
    bool operator >= (const Number &rhs) const;
    bool operator == (const Number &rhs) const;
    bool operator != (const Number &rhs) const;
    Number();
    Number(uint32_t);
    uint32_t get_value() const;
};

Number two_increasing_arrays(
    int n,
    int k,
    std::vector<Number> a,
    std::vector<Number> b
);
