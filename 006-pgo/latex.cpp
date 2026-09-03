#include "pgotypes.h"

static int
escape_filename(const char *s){
  for(const char* curs = s ; *curs ; ++curs){
    if(*curs != '\'' && *curs != '%'){
      if(printf("%c", *curs) < 0){
        return -1;
      }
    } // simply drop apostrophes and percents?
  }
  return 0;
}

static int
print_icons(const species *s, bool doprint, bool ismega){
  int count = 0;
  if(has_mega(s) && !ismega){
    ++count;
    if(doprint){
      printf(" \\calign{\\includegraphics[height=1.5em,keepaspectratio]{images/" IMAGECOLOR "mega.png}}");
    }
  }
  if(has_dmax(s)){
    ++count;
    if(doprint){
      printf(" \\calign{\\includegraphics[height=1.5em,keepaspectratio]{images/" IMAGECOLOR "dynamax.png}}");
    }
  }
  if(has_gmax(s)){
    ++count;
    if(doprint){
      printf(" \\calign{\\includegraphics[height=1.5em,keepaspectratio]{images/" IMAGECOLOR "gigantamax.png}}");
    }
  }
  return count;
}

// we're in the species card context. generate a table of the species'
// evolutionary line, using multiple rows for any fork. we only print
// forks above us, not behind. abbreviate regional prefixes to save space.
static void
print_evolution_table(const species* s){
  const species *devol = get_previous_evolution(s);
  int evolidx = 0;
  std::vector<const species*> evols;
  int rows = get_evolution_count(s, evols);
  if(devol || rows){
    // we need a table because the evolution can fan out
    if(rows == 0){
      rows = 1;
    }
    int immindex = -1; // see comment below
    std::vector<const species*> immevols;
    get_persistent_evolutions(s, immevols);
    for(int r = 0 ; r < rows ; ++r){
      if(r){
        printf("\\\\");
      }
      printf("\\hfill{}");
      // first, print previous step(s) (with page numbers)
      if(devol){
        print_previous_species(devol);
      }
      // next, print ourselves, in bold (no page number)
      printf("\\textbf{");
      escape_string(s->name.c_str());
      printf("}");
      // now, the next evolutionary step(s), if they exist. we do only one row.
      // this requires knowing our index in the immevols array and the next
      // entry in the evols array. when we come across the next immevols entry
      // in evols, update immindex and pop. then print any successor and pop.
      if(evols.size()){
        printf(" → ");
        if(immevols[immindex + 1] == evols[evolidx]){
          ++immindex;
          ++evolidx;
        }
        const auto imm = immevols[immindex];
        escape_abbr_string(imm->name.c_str());
        printf(" (\\pageref{species:");
        label_string(imm->name.c_str());
        printf("})");
        std::vector<const species*> waste;
        if(get_persistent_evolutions(imm, waste)){
          printf(" → ");
          escape_abbr_string(evols[evolidx]->name.c_str());
          printf(" (\\pageref{species:");
          label_string(evols[evolidx]->name.c_str());
          printf("})");
          ++evolidx;
          ++r;
        }
      }
    }
  /*}else{
    printf("No evolution");*/
  }
}

// returns maximum actual cp subject to the ceiling
static unsigned
print_optimal_latex_cp(const species* sp, int maxcp){
  stats* s = find_optimal_set(sp, maxcp, 0, false, calc_pok_gmean);
  unsigned foundcp = 0;
  unsigned cp = 0;
  unsigned printed = 0;
  while(s){
    stats* tmp = s->next;
    if((cp = s->cp) > foundcp){
      foundcp = cp;
    }
    if(++printed < 3){
      unsigned half;
      unsigned l = halflevel_to_level(s->hlevel, &half);
      printf("\\ivlev{%u}{%u}{%u}{%2u%s}", s->ia, s->id, s->is, l, half ? ".5" : "");
      printf(" (%u) ", s->cp);
    }
    delete s;
    s = tmp;
  }
  if(printed >= 3){
    printf("(%u more)", printed - 2);
  }
  return foundcp;
}

// used for species cards, always wants geometric mean
static void
print_optimal_latex(const species* sp){
  printf("\\raggedleft{}");
  unsigned maxcp;
  maxcp = print_optimal_latex_cp(sp, 0);
  if(maxcp > ULCPCAP){
    printf("\\\\");
    maxcp = print_optimal_latex_cp(sp, ULCPCAP);
  }
  if(maxcp > GLCPCAP){
    printf("\\\\");
    maxcp = print_optimal_latex_cp(sp, GLCPCAP);
  }
}

void print_species_latex(const species* s, bool overzoom, bool bg, bool mainform){
  printf("\\vfill\n");
  const auto gma = lookup_gmax_attack(s);
  bool gmax = !overzoom && gma;
  const mega* meg = lookup_mega(s->name.c_str());
  // just because we *have* a mega doesn't mean we *are* a mega
  bool ismega = ismega_p(s);
  printf("\\begin{speciesbox}[title=\\#%04u ", s->idx);
  if(gmax){
    printf("Gigantamax ");
  }
  escape_string(s->name.c_str());
  if(ismega){
    printf(" (%'u)", meg->initialcost);
  }
  if(mainform){
    printf(",before title={\\phantomsection\\label{species:");
    label_string(s->name.c_str());
    printf("}}");
  }
  printf(",title style={left color=%s,right color=%s},after title={",
          TNames[s->t1], s->t2 == TYPECOUNT ? TNames[s->t1] : TNames[s->t2]);
  if(s->shiny){
    printf("\\calign{\\includegraphics[height=1em,keepaspectratio]{images/" IMAGECOLOR "shiny.png}}");
  }
  float avg = calc_amean(s->atk, s->def, s->sta);
  printf("\\hfill%u %u %u %.1f %.1f}", s->atk, s->def, s->sta, avg, calc_gmean(s->atk, s->def, s->sta));
  // background image is zoomed and flipped at low opacity
  printf(",interior style={fill overzoom image=images/highres/" IMAGECOLOR);
  escape_filename(s->name.c_str());
  printf(",fill image opacity=0.2}");
  printf("]{\\footnotesize");

  if(bg){
    printf("\\pagecolor{%s!50!white}", TNames[s->t1]);
  }
  // the table containing image and attack data
  printf("\\begin{tabularx}{\\linewidth}{@{}c X @{}}");
  printf("\\scalebox{-1}[1]{\\includegraphics[width=0.3\\linewidth,valign=c,keepaspectratio]{images/highres/" IMAGECOLOR);
  if(gmax){ // get the gmax image
    printf("Gmax ");
  }
  for(const char* curs = s->name.c_str() ; *curs ; ++curs){
    if(*curs != '%' && *curs != '\''){
      printf("%c", *curs);
    }
  }
  printf(".png}} &\\begingroup\\setlength{\\tabcolsep}{4pt}\\begin{tabular}{lrrrrr}\n");
  for(const auto &a : s->attacks){
    print_attack_latex(s, a);
  }
  /*if(ismega && meg->plusatk){
    print_plus_attack_latex(s, meg->plusatk);
  }*/
  if(mainform && s->shadow){
    print_attack_latex(s, &ATK_Return);
  }
  printf("\\end{tabular}\\endgroup\\end{tabularx}\n");

  // the minipages with icons and cp data
  printf("\\noindent\\begin{minipage}{0.%d\\linewidth}", gmax ? 4 : 3);
  print_types_big(s->t1, s->t2);
  // for the gmax cards, don't print the max icons --- we know it's max-capable
  if(!gmax){
    print_icons(s, true, ismega);
  }
  printf("\\end{minipage}\n");
  if(mainform || ismega){ // optimal IVs and evolutionary lineage (not used for gmax)
    printf("\\begin{minipage}{0.%d\\linewidth}\\scriptsize\\raggedleft{}", gmax ? 6 : 7);
    print_optimal_latex(s);
    printf("\\end{minipage}\\\\");

    printf("\\scriptsize{}");
    if(!ismega){
      printf("%u ", stardust_reward(s));
      if(s->categorystr() && strcmp(s->categorystr(), "")){
        printf("%s\n", s->categorystr());
      }else{
        printf("CG %d", a2cost_to_cgroup(s->a2cost));
      }
      printf(" Gen %s %s\\hfill{}",
            idx_to_generation(s->idx), idx_to_region(s->idx));
      printf("\\begin{minipage}{0.%d\\linewidth}\\scriptsize\\raggedleft{}", gmax ? 6 : 7);
      print_evolution_table(s);
      printf("\\end{minipage}");
    }
  }else{ // other than main/mega forms
    if(gmax){
      printf("\\hfill");
      print_type(s->t1);
      printf(" G-Max %s", gma->name.c_str());
    }
  }

  // shadow is implemented as subtitle
  if(mainform && s->shadow){
    printf("\\tcbsubtitle{Shadow ");
    escape_string(s->name.c_str());
    printf("\\hfill{}");
    const float atk = s->atk * 6 / 5.0;
    const float def = s->def * 5 / 6.0;
    const float savg = calc_amean(atk, def, s->sta);
    // we don't show geometric mean as it'll always be the same as the non-shadow
    // form. instead, show delta for arithmetic mean.
    printf("%.1f %.1f %u %.1f ", atk, def, s->sta, savg);
    if(avg > savg){
      printf("(-%.1f\\%%)", (avg - savg) * 100 / avg);
    }else if(avg < savg){
      printf("(+%.1f\\%%)", (savg - avg) * 100 / avg);
    }else{
      printf("(no change)");
    }
    printf("}\n");
  }

  printf("}");
  printf("\\end{speciesbox}\n");
}

// print those entries containing type(s). pass TYPECOUNT for a wildcard on t2.
// pass the same type twice for only that base type. LaTeX output.
void filter_by_types(int t1, int t2, const species* dex, unsigned count, bool overzoom, bool mainform){
  for(unsigned i = 0 ; i < count ; ++i){
    bool printit = false;
    if(dex[i].t1 == t1){
      if(dex[i].t2 == t2 || t2 == TYPECOUNT || (t1 == t2 && dex[i].t2 == TYPECOUNT)){
        printit = true;
      }
    }
    if(printit){
      print_species_latex(&dex[i], overzoom, true, mainform);
    }
  }
}
