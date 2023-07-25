#include "IItems.h"

IItems::IItems(
    std::string_view name, std::string_view description, const std::map<ESetting, std::shared_ptr<Chars>>& chars)
    : m_name(name), m_description(description), m_chars(chars) {}
IItems::~IItems() {}
std::string IItems::toString() const { return m_name; }
void IItems::setChars(ESetting setting, std::shared_ptr<Chars> chars) { m_chars[setting] = chars; }
std::shared_ptr<Chars> IItems::getChars(ESetting setting) {
  if (m_chars.find(setting) != m_chars.end()) {
    return m_chars.at(setting);
  }
  return nullptr;
}

WearingItem::WearingItem(const WearingStruct& wstr) :
  IItems(wstr.name, wstr.description, wstr.m_chars) {}

WearingItem::~WearingItem() {}

Armour::Armour(const ArmourStruct& str) : WearingItem(str) {

}

Armour::~Armour() {

}

Weapon::Weapon(const WeaponStruct& str) : WearingItem(str) {

}

Weapon::~Weapon() {


}
