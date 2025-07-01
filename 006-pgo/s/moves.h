// the various things each side can do on a turn. MOVE_WAIT can mean either
// a mandatory wait while the ongoing fast attack completes, or an optional
// wait while doing nothing.
typedef enum {
  MOVE_WAIT,
  MOVE_WAIT_SHIELD,
  MOVE_FAST,
  MOVE_FAST_SHIELD,
  MOVE_CHARGED1,
  MOVE_CHARGED1_SHIELD,
  MOVE_CHARGED2,
  MOVE_CHARGED2_SHIELD,
  MOVE_SUB1,    // sub in pokémon one down
  MOVE_SUB2,    // sub in pokémon two down
  MOVEMAX
} pgo_move_e;

static inline bool
charged_move_p(pgo_move_e m){
  return m == MOVE_CHARGED1 || m == MOVE_CHARGED2 ||
          m == MOVE_CHARGED1_SHIELD || m == MOVE_CHARGED2_SHIELD;
}

static inline bool
fast_move_p(pgo_move_e m){
  return m == MOVE_FAST || m == MOVE_FAST_SHIELD;
}

static inline bool
shielded_move_p(pgo_move_e m){
  return m == MOVE_WAIT_SHIELD || m == MOVE_FAST_SHIELD ||
          m == MOVE_CHARGED1_SHIELD || m == MOVE_CHARGED2_SHIELD;
}

static bool
sub_move_p(pgo_move_e m){
  return m == MOVE_SUB1 || m == MOVE_SUB2;
}
