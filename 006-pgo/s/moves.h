// the various things each side can do on a turn. MOVE_WAIT can mean either
// a mandatory wait while the ongoing fast attack completes, or an optional
// wait while doing nothing.
enum pgo_move_e {
  MOVE_WAIT             = 0x001,
  MOVE_FAST             = 0x002,
  MOVE_CHARGED1         = 0x004,
  MOVE_CHARGED2         = 0x008,
  MOVE_SUB1             = 0x010, // sub in pokémon one down
  MOVE_SUB2             = 0x020, // sub in pokémon two down
  MOVEMAX               = 0x040
};

static inline bool charged_move_p(pgo_move_e m){
  return m & (MOVE_CHARGED1 | MOVE_CHARGED2);
}

static inline bool fast_move_p(pgo_move_e m){
  return m & MOVE_FAST;
}

static bool sub_move_p(pgo_move_e m){
  return m & (MOVE_SUB1 | MOVE_SUB2);
}
