// the various things each side can do on a turn. MOVE_WAIT can mean either
// a mandatory wait while the ongoing fast attack completes, or an optional
// wait while doing nothing.
typedef enum {
  MOVE_WAIT             = 0x001,
  MOVE_WAIT_SHIELD      = 0x002,
  MOVE_FAST             = 0x004,
  MOVE_FAST_SHIELD      = 0x010,
  MOVE_CHARGED1         = 0x020,
  MOVE_CHARGED1_SHIELD  = 0x040,
  MOVE_CHARGED2         = 0x080,
  MOVE_CHARGED2_SHIELD  = 0x100,
  MOVE_SUB1             = 0x200, // sub in pokémon one down
  MOVE_SUB2             = 0x400, // sub in pokémon two down
  MOVEMAX               = 0x800
} pgo_move_e;

static inline bool
charged_move_p(pgo_move_e m){
  return m & (MOVE_CHARGED1 | MOVE_CHARGED2 |
      MOVE_CHARGED1_SHIELD | MOVE_CHARGED2_SHIELD);
}

static inline bool
fast_move_p(pgo_move_e m){
  return m & (MOVE_FAST | MOVE_FAST_SHIELD);
}

static inline bool
shielded_move_p(pgo_move_e m){
  return m & (MOVE_WAIT_SHIELD | MOVE_FAST_SHIELD |
              MOVE_CHARGED1_SHIELD | MOVE_CHARGED2_SHIELD);
}

static bool
sub_move_p(pgo_move_e m){
  return m & (MOVE_SUB1 | MOVE_SUB2);
}
