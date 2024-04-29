#include "human.h"

static void human_act(human_t * this);
static void human_learn(human_t * this);

static animal_vtable_t __g_animal_vtable;
static human_vtable_t __g_human_vtable = 
{
  .learn = human_learn,
};

human_t* human_born(void) {
  human_t* this = malloc(sizeof(human_t));
  
  this->p_ancestor = animal_born();
  memcpy(&(this->ancestor), this->p_ancestor, sizeof(animal_t));
  
  this->ancestor.class_size = sizeof(human_t);
  
  memcpy(&__g_animal_vtable, this->ancestor.vt, sizeof(animal_vtable_t));
  this->ancestor.vt = &__g_animal_vtable;
  this->ancestor.vt->act = human_act;
  
  this->vt = &__g_human_vtable;
  this->intelligence = 60;
  
  printf("human_born: intelligence = %d\n", this->intelligence);
  
  return this;
}

void learn(void* this) {
  char* type_name = ((human_t*) this)->ancestor.type_name;
  unsigned int class_size = ((human_t*) this)->ancestor.class_size;
  
  if ((0 == strcmp(type_name, "animal_t")) && (class_size >= sizeof(human_t))) {
    return ((human_t *) this)->vt->learn(this);
  }

  printf("can't learn\n");
}

static void human_act(human_t* this) {
  if (this->ancestor.stamina >= 30) {
    this->ancestor.stamina -= 1;
    printf("human act: stamina - 1\n");
    printf("stamina = %d\n", this->ancestor.stamina);
  } else {
    this->ancestor.stamina += 5;
    printf("human act: stamina + 5\n");
    printf("stamina = %d\n", this->ancestor.stamina);
  }
}

static void human_learn(human_t* this) {
  this->intelligence += 1;
  printf("human_learn: intelligence +1\n");
  printf("intelligence = %d\n", this->intelligence);
}

void human_die(human_t* this) {
  printf("human_die\n");
  
  animal_die(this->p_ancestor);
  free(this);
}

