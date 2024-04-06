#include "1622.h"
#include <bits/stdc++.h>

namespace {

struct TestCase {
    int n, k;
    std::vector<Number> a, b;
};

int cost, max_cost;
bool called_get_value;

void reset() {
    cost = 0;
}

void WA(const char *str) {
    printf("Wrong Answer: %s\n", str);
    exit(0);
}

}

Number::Number() : info(0) {}
Number::Number(uint32_t x) : info(x) {}

bool Number::operator <  (const Number &rhs) const { ++cost; return info <  rhs.info; }
bool Number::operator >  (const Number &rhs) const { ++cost; return info >  rhs.info; }
bool Number::operator <= (const Number &rhs) const { ++cost; return info <= rhs.info; }
bool Number::operator >= (const Number &rhs) const { ++cost; return info >= rhs.info; }
bool Number::operator == (const Number &rhs) const {         return info == rhs.info; }
bool Number::operator != (const Number &rhs) const {         return info != rhs.info; }

uint32_t Number::get_value() const {
    return info;
}

int main() {
    int t;
    assert(1 == scanf("%d", &t));
    
    std::vector<TestCase> tcs;
    
    while (t--) {
        TestCase tc;
        assert(2 == scanf("%d %d", &tc.n, &tc.k));
        tc.a.resize(tc.n);
        tc.b.resize(tc.n);
        for (Number &num : tc.a) {
            int x; assert(1 == scanf("%d", &x));
            num = Number(x);
        }
        for (Number &num : tc.b) {
            int x; assert(1 == scanf("%d", &x));
            num = Number(x);
        }
        tcs.emplace_back(tc);
    }
    
    fclose(stdin);
    
    for (const TestCase tc : tcs) {
        reset();
        Number res = two_increasing_arrays(tc.n, tc.k, tc.a, tc.b);
        max_cost = std::max(max_cost, cost);
        
        int smaller_equal_count = 0;
        for (const Number &num : tc.a) smaller_equal_count += (num <= res);
        for (const Number &num : tc.b) smaller_equal_count += (num <= res);
        
        if (smaller_equal_count != tc.k) {
            WA("Returned Value is Incorrect");
        }
    }
    
    if (called_get_value) {
        printf("Warning: You have called <get_value()>, which is not available in the Formosa OJ.");
    }
    
    printf("Accepted: %d\n", max_cost);
    
    fclose(stdout);
    
    return 0;
}
