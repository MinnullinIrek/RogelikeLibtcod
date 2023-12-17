#ifndef CHAR_TYPES_H
#define CHAR_TYPES_H

using CharValueType = float;

enum class ESetting {
  baseChar,
  skills,
  abilities,
  damage,
  resistance,
  protection,
  damage1,
  damage2,
  damage3,
  addBaseChar,
  addAttribute,

};

enum class ECharTypes {
  strength,
  constitution,
  intelligence,
  dexterity,
  charisma,
  wisdom,
  perception,

  hp,
  energy,
  manna,
  weight,

  count
};

enum class EDamageType {
  cut = static_cast<int>(ECharTypes::count),
  chop,
  spike,
  crush,
  fire,
  ice,
  electricity,
  sound,

  count
};
enum class ESkills {
  parry = static_cast<int>(EDamageType::count),
};

#endif
