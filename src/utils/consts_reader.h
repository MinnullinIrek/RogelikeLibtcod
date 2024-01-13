#ifndef CONSTS_READER_H
#define CONSTS_READER_H

#include <string>
#include <unordered_map>

class MagicConsts {
 public:
  static MagicConsts& instance() {
    static MagicConsts magicConsts;
    return magicConsts;
  }
  void readJson();
  float getValue(std::string&& name);

 private:
  MagicConsts();
  std::unordered_map<std::string, float> m_consts;
};

#define SF MagicConsts::instance().getValue
#define SI (int)MagicConsts::instance().getValue

#endif
