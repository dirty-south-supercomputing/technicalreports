// we always do something related to fast attacks (launch one, or wait for one
// to complete). these are other things we can do.
enum pgo_move_e {
  MOVE_FAST     = 0x001, MOVE_CHARGED1 = 0x002, MOVE_CHARGED2 = 0x004,
  MOVE_SUB1     = 0x008, MOVE_SUB2     = 0x010, MOVEMAX       = 0x020
};

static inline bool charged_move_p(pgo_move_e m){
  return m & (MOVE_CHARGED1 | MOVE_CHARGED2);
}

static inline bool sub_move_p(pgo_move_e m){
  return m & (MOVE_SUB1 | MOVE_SUB2);
}
