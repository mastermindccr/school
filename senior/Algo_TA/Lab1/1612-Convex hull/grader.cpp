#include "1612.h"
#include <iostream>
#include <vector>
#include <string>
#include <cassert>

namespace {

	const int query_limit = 32;
	
	int query_cnt;
	std::vector<int> coords;
	
	void protocal_violation(std::string reason) {
		printf("Protocal Violation: %s\n", reason.data());
		exit(0);
	}
	
	std::string ordinal(int x) {
		int a = x % 10, b = x % 100;
		if (a == 1 and b != 11) return "st";
		if (a == 2 and b != 12) return "nd";
		if (a == 3 and b != 13) return "rd";
		return "th";
	}

	std::vector<std::pair<int, int>> tangent;

}

std::pair<int, int> query(int m) {
	query_cnt++;
	
	if (query_cnt > query_limit) {
		protocal_violation("too many queries");
	}
	
	if (m < -1'000'000'000 or m > 1'000'000'000) {
		protocal_violation("invalid parameters");
	}
	
	printf("%d%s query: slope = %d\n", query_cnt, ordinal(query_cnt).data(), m);
	
	for(auto &i: tangent){
		if(m >= i.first)
			return {i.second, coords[i.second]};
	}
	
	return {coords.size()-1, coords[coords.size()-1]};
}

int main() {
	
	int t;
	assert(1 == scanf("%d", &t));
	
	for (int test = 1; test <= t; ++test) {
		int L, k;
		assert(2 == scanf("%d %d", &L, &k));
		coords.resize(L+1);
		for (int i = 0; i <= L; ++i) assert(1 == scanf("%d", &coords[i]));

		tangent.clear();
		for(int i = 1;i<=L;i++){
			if(i!=1 && coords[i]-coords[i-1] == (*tangent.rbegin()).first) continue;
			tangent.push_back({coords[i]-coords[i-1],i-1});
		}
		query_cnt = 0;
		int res = convex_hull(L, k);
		printf("convex_hull: %d\n", res);
		if (res == coords[k]) {
			printf("Accepted: %d\n", query_cnt);
		}
		else {
			printf("Wrong Answer: wrong coordinate\n");
		}
	}
	
	return 0;
}
