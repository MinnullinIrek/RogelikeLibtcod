#ifndef BODYPART_H
#define BODYPART_H
#include <memory>
#include <string>

#include "../eitems.h"
#include "../header.h"
#include "chars.h"

class WearingItem;

class Bodypart : public ToString {
 public:
  Bodypart(const std::string& name, float percent, CharType hp);
  ~Bodypart();
  std::string toString() const override;
  void setItem(std::weak_ptr<WearingItem> item);
  void removeItem(std::weak_ptr<WearingItem> item);
  std::weak_ptr<WearingItem> getWeapon();
  const std::map<EArmorItemTypes, std::weak_ptr<WearingItem>>& getArmour();
  const std::string m_name;
  float m_percent;
  CharType m_hp;

 private:
  std::map<EArmorItemTypes, std::weak_ptr<WearingItem>> m_items;
  std::weak_ptr<WearingItem> m_weapon;
};

class BodyParts {
 public:
  BodyParts(const std::map<EWearingSlot, std::shared_ptr<Bodypart>>& slots);
  ~BodyParts();
  std::shared_ptr<Bodypart> getBodyPart(EWearingSlot slot);
  void setBodyPart(EWearingSlot slot, std::shared_ptr<Bodypart> bodyPart);

 private:
  std::map<EWearingSlot, std::shared_ptr<Bodypart>> m_parts;
};

#endif
