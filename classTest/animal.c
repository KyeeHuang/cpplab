#include "animal.h"

static void animal_act(animal_t* this);

static animal_vtable_t __g_animal_vtable = 
{
  .act = animal_act,
};

animal_t * animal_born(void) {
  animal_t * this = malloc(sizeof(animal_t));
  
  this->type_name = malloc(sizeof("animal_t"));
  memcpy(this->type_name, "animal_t", sizeof(this->type_name));
  this->class_size = sizeof(animal_t);
  
  this->vt = &__g_animal_vtable;
  this->stamina = 100;
  
  printf("animal_born: stamina = 100\n");
  return this;
}

void act(void * this) {
  if (0 == strcmp(((animal_t*) this)->type_name, "animal_t")) {
    return ((animal_t*) this)->vt->act(this);
  }
  printf("can't act\n");
}

static void animal_act(animal_t* this) {
  if (this->stamina >= 30) {
    this->stamina -= 5;
    printf("animal act: stamina - 5\n");
    printf("stamina = %d\n", this->stamina);
  } else {
    this->stamina += 10;
    printf("animal rest : stamina + 10\n");
    printf("stamina = %d\n", this->stamina);
  }
}

void animal_die(animal_t* this) {
  free(this->type_name);
  free(this);
  printf("animal_die\n");
}

