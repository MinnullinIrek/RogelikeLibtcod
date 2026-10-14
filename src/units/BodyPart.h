#ifndef BODYPART_H
#define BODYPART_H
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../eitems.h"
#include "../header.h"
#include "../utils/subscriber.h"
#include "chars.h"

class WearingItem;
class Armour;
class Weapon;
class BodyParts;

// Одна часть тела (голова, шея, рука, кисть, грудь, спина, пояс, бедра, голень, ступня...).
// На частице может одновременно висеть по одному предмету каждого типа брони:
// одежда + легкая + средняя + тяжелая + экзоскелет (разные типы могут покрывать одну и ту же часть).
// В кисти рук дополнительно хранится экипированное оружие.
class Bodypart : public ToString {
 public:
  Bodypart(EWearingSlot slot, const std::string& name, float percent, CharValueType hp);
  ~Bodypart();
  std::string toString() const override;

  EWearingSlot getSlot() const { return m_slot; }
  float getPercent() const { return m_percent; }
  CharValueType getHp() const { return m_hp; }

  // Экипировать предмет на эту часть тела. Возвращает false, если место занято
  // (тем же типом брони, а для оружия — кисть уже держит другое оружие).
  // weak_ptr<BodyParts> нужен, чтобы после изменения уведомить подписчиков окна инвентаря.
  bool equip(const std::weak_ptr<BodyParts>& body, std::shared_ptr<WearingItem> item);
  // Снять предмет с этой части тела.
  bool unequip(const std::weak_ptr<BodyParts>& body, std::shared_ptr<WearingItem> item);

  // Броня, надетая на часть тела, по типам.
  const std::map<EArmorItemTypes, std::weak_ptr<Armour>>& getArmour() const { return m_armour; }
  std::shared_ptr<Armour> getArmourByType(EArmorItemTypes type) const;

  // Оружие, держащееся в этой кисти (если это кисть).
  std::shared_ptr<Weapon> getWeapon() const { return m_weapon.lock(); }

 private:
  const EWearingSlot m_slot;
  const std::string m_name;
  float m_percent;
  CharValueType m_hp;

  std::map<EArmorItemTypes, std::weak_ptr<Armour>> m_armour;
  std::weak_ptr<Weapon> m_weapon;
};

// Набор частей тела юнита. Знает, какие части покрывает предмет,
// и умеет экипировать/снимать предметы целиком.
class BodyParts : public Publisher {
 public:
  BodyParts(const std::map<EWearingSlot, std::shared_ptr<Bodypart>>& slots);
  ~BodyParts();
  std::shared_ptr<Bodypart> getBodyPart(EWearingSlot slot);
  std::shared_ptr<Bodypart> getBodyPart(EWearingSlot slot) const;
  void setBodyPart(EWearingSlot slot, std::shared_ptr<Bodypart> bodyPart);

  // Все части тела, покрытые данным предметом.
  std::vector<std::shared_ptr<Bodypart>> coveredParts(const std::shared_ptr<WearingItem>& item) const;
  // Можно ли экипировать предмет (все требуемые части существуют и свободны).
  bool canEquip(const std::shared_ptr<WearingItem>& item) const;
  // Экипировать/снять предмет со всех покрываемых частей тела.
  // self — weak_ptr на этот же набор частей (нужен, чтобы части слали emit в окно инвентаря).
  bool equip(const std::weak_ptr<BodyParts>& self, std::shared_ptr<WearingItem> item);
  bool unequip(const std::weak_ptr<BodyParts>& self, std::shared_ptr<WearingItem> item);

  const std::map<EWearingSlot, std::shared_ptr<Bodypart>>& getParts() const { return m_parts; }

  // Владелец частей тела (юнит) — используется для каскадного уведомления подписчиков:
  // экипировка/снятие предмета шлёт emit() и в BodyParts, и во владельца.
  void setOwner(std::weak_ptr<Publisher> owner);

  // Список «предмет : где надет» для отображения в окне инвентаря.
  std::list<Text> showEquipped(Count count) const;

  // Полный набор частей тела humanoid'а: голова, шея, руки, кисти, грудь, спина,
  // пояс, бедра, голени, ступни.
  static std::shared_ptr<BodyParts> createHumanoid();

 private:
  std::map<EWearingSlot, std::shared_ptr<Bodypart>> m_parts;
  std::weak_ptr<Publisher> m_owner;
};

#endif
