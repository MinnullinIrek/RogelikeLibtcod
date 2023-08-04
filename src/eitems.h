#ifndef EITEMS_H
#define EITEMS_H
enum class EWearingSlot {
  larm,
  rarm,
  lhend,
  rhand,
  lforearm,
  rforearm,
  head,
  neck,
  brest,
  hip,
  lleg,
  rleg,
  lfoot,
  rfoot,

  count
};

enum class EArmorItemTypes {
  clothes,
  light,
  medium,
  heavy,
  overcoat,
  robe,
  exoArmour,

};

enum class EWeaponType {
  blade,
  axe,
  spike,
  bow,
  crossBow,
  sling,
  halberd,
  shield,
  teeth,
};

enum class EWeaponSize {
  small,
  medium,
  heavy,
  giant,
};

enum class EWeaponRangedClose { melee, ranged, magik, thrown, close };

enum class EWeaponOneTwoHand { oneHand, twoHand };

#endif
