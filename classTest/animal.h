#ifndef _ANIMAL_H_
#define _ANIMAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _animal_t animal_t;
typedef void (*animal_act_t) (animal_t*);

typedef struct _animal_vtable_t {
  animal_act_t act;
} animal_vtable_t;

struct _animal_t {
  char* type_name;
  unsigned int class_size;
  animal_vtable_t* vt;
  unsigned int stamina;
};

animal_t * animal_born(void);
void act(void * this);
void animal_die(animal_t* this);


#endif