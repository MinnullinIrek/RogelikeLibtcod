#ifndef IITEMS_H
#define IITEMS_H
#include <unordered_map>

#include "../header.h"

class IItems {
 private:
  /* data */
 public:
  std::unordered_map<ESettings, Setting> m_settings;
  ItemType m_type;
};

#endif
