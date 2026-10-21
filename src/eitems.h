#ifndef EITEMS_H
#define EITEMS_H
// Части тела под слоты одежды/оружия.
// Предмет может покрывать сразу несколько частей (vector<EWearingSlot> в ArmourStruct),
// либо одну часть. Оружие занимает одну руку (lhand ИЛИ rhand) или две руки (lhand+rhand).
enum class EWearingSlot {
  head,     // голова
  neck,     // шея
  larm,     // левая рука (плечо)
  rarm,     // правая рука (плечо)
  lforearm, // левое предплечье
  rforearm, // правое предплечье
  lhand,    // левая кисть
  rhand,    // правая кисть
  chest,    // грудь
  back,     // спина
  waist,    // пояс
  hips,     // бедра
  lshin,    // левая голень
  rshin,    // правая голень
  lfoot,    // левая ступня
  rfoot,    // правая ступня

  count
};

// Типы брони. Разные типы могут покрывать одну и ту же часть тела одновременно
// (например cloth shirt + leather cuirass на груди),
// поэтому в Bodypart хранится map<EArmorItemTypes, item>.
enum class EArmorItemTypes {
  clothes,   // одежда
  light,     // легкая броня
  medium,    // средняя броня
  heavy,     // тяжелая броня
  exoArmour, // экзоскелет

  count
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

inline const char* slotName(EWearingSlot slot) {
  switch (slot) {
    case EWearingSlot::head: return "head";
    case EWearingSlot::neck: return "neck";
    case EWearingSlot::larm: return "larm";
    case EWearingSlot::rarm: return "rarm";
    case EWearingSlot::lforearm: return "lforearm";
    case EWearingSlot::rforearm: return "rforearm";
    case EWearingSlot::lhand: return "lhand";
    case EWearingSlot::rhand: return "rhand";
    case EWearingSlot::chest: return "chest";
    case EWearingSlot::back: return "back";
    case EWearingSlot::waist: return "waist";
    case EWearingSlot::hips: return "hips";
    case EWearingSlot::lshin: return "lshin";
    case EWearingSlot::rshin: return "rshin";
    case EWearingSlot::lfoot: return "lfoot";
    case EWearingSlot::rfoot: return "rfoot";
    default: return "unknown";
  }
}

inline const char* armourTypeName(EArmorItemTypes type) {
  switch (type) {
    case EArmorItemTypes::clothes: return "clothes";
    case EArmorItemTypes::light: return "light";
    case EArmorItemTypes::medium: return "medium";
    case EArmorItemTypes::heavy: return "heavy";
    case EArmorItemTypes::exoArmour: return "exoArmour";
    default: return "unknown";
  }
}

// Кисти рук — сюда экипируется оружие.
inline bool isHandSlot(EWearingSlot slot) { return slot == EWearingSlot::lhand || slot == EWearingSlot::rhand; }

#endif
