#include "pgotypes.h"

struct form {
  form(const species *s, const attack *a1, const attack *a2, float pptsum) :
    S(s),
    A1(a1),
    A2(a2),
    Pptsum(pptsum) { }

  bool operator<(const form& f) const {
    if(Pptsum == f.Pptsum){
      return S->name < f.S->name;
    }
    return Pptsum < f.Pptsum;
  }

  const species *S;
  const attack *A1, *A2; // can be nullptr
  float Pptsum; // sum of ppts, including STAB
};

float ppe_with_stab(const species *s, const attack *a){
  float pow = a->powertrain;
  if(has_stab_p(s, a)){
    pow = calc_stab(pow);
  }
  return pow / -a->energytrain;
}

const attack *find_most_powerful(const species *s){
  const attack *a = nullptr;
  for(const attack *atk : s->attacks){
    if(!charged_attack_p(atk)){
      continue;
    }
    if(a == nullptr){
      a = atk;
    }else{
      if(ppe_with_stab(s, a) < ppe_with_stab(s, atk)){
        a = atk;
      }else if(ppe_with_stab(s, a) == ppe_with_stab(s, atk)){
        // pick the quicker one if PPE is equal
        if(a->energytrain < atk->energytrain){
          a = atk;
        }
      }
    }
  }
  // FIXME consider Return if it has a shadow type!
  return a;
}

// find the second strongest charged attack (a1 must be the strongest) that
// does *not* share its type with a1. return nullptr if no such attack exists.
const attack *find_second(const species *s, const attack *a1){
  const attack *a = nullptr;
  for(const attack *atk : s->attacks){
    if(!charged_attack_p(atk)){
      continue;
    }
    if(atk == a1){
      continue;
    }
    if(atk->type == a1->type){
      continue;
    }
    if(a == nullptr){
      a = atk;
    }else{
      if(ppe_with_stab(s, a) < ppe_with_stab(s, atk)){
        a = atk;
      }else if(ppe_with_stab(s, a) == ppe_with_stab(s, atk)){
        // pick the quicker one if PPE is equal
        if(a->energytrain < atk->energytrain){
          a = atk;
        }
      }
    }
  }
  // FIXME consider Return if it has a shadow type!
  return a;
}

// for each form, find its two most powerful (by PPT) charged attacks having
// different types. sort the forms by the sum of the PPTs.
int main(void){
  std::vector<form> forms;
  for(unsigned i = 0 ; i < SPECIESCOUNT ; ++i){
    const species* s = &sdex[i];
    float pptsum = 0;
    const attack *a1 = find_most_powerful(s);
    pptsum = a1->powertrain / static_cast<float>(-a1->energytrain); // FIXME stab!
    const attack *a2 = find_second(s, a1);
    if(a2){
      pptsum += a2->powertrain / static_cast<float>(-a2->energytrain); // FIXME stab!
    }
    forms.emplace_back(s, a1, a2, pptsum);
  }
  sort(forms.begin(), forms.end());
  for(const auto& f : forms){
    std::cout << '\"' << f.S->name << "\" \"" << f.A1->name << "\" ";
    if(f.A2){
      std::cout << '\"' << f.A2->name << "\" ";
    }
    std::cout << f.Pptsum << std::endl;
  }
  return EXIT_SUCCESS;
}
