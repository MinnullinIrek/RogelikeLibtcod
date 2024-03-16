#ifndef RANDOM_SELCTOR
#define RANDOM_SELCTOR

#include <algorithm>
#include <vector>

#include "random.h"


int select(const std::vector<int>& selectingValues) {
  int r = random(1, 100);
  auto it = std::lower_bound(std::begin(selectingValues), std::end(selectingValues), r);
  return std::distance(std::begin(selectingValues), it);
}

#endif
