#ifndef _HUMAN_H_
#define _HUMAN_H_

#include "animal.h"

typedef struct _human_t human_t;
typedef void (*human_learn_t) (human_t*);

typedef struct _human_vtable_t {
  human_learn_t learn;
} human_vtable_t;

struct _human_t {
  animal_t ancestor;
  animal_t* p_ancestor;
  human_vtable_t* vt;
  unsigned int intelligence;
};

human_t* human_born(void);
void learn(void* this);
void human_die(human_t* this);

#endif