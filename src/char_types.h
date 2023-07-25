#ifndef CHAR_TYPES_H
#define CHAR_TYPES_H

using CharType = float;

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
};

enum class ECharTypes {
  strength,
  constitution,
  intelligence,
  dexterity,
  charisma,
  wisdom,

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

#endif
