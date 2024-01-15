#ifdef EFFECT_H
#define EFFECT_H

// Albert
struct CoordSymb {
  Coord cd;
  Symbol s;
}

class Effect {
 public:
  void showEfect();

  std::vector<std::vector<CoordSymb>> effectList = {{{{9, 10}, 'a'}, {11, 10}, 'a'}}, { {{8, 10}, 'b'}, {12, 10}, 'b' }
}, {
  {{7, 10}, 'c'}, {13, 10}, 'c'
}
}
,
}
;
}
;

#endif  // EFFECT_H
