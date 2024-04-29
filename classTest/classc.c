#include "animal.h"
#include "human.h"

int main() {
  printf("\nanimal monkey:\n");
  animal_t* monkey = animal_born();
  act(monkey);

  printf("\nanimal monkey learn:\n");
  learn(monkey);

  animal_die(monkey);

  printf("\nhuman qaq:\n");
  human_t* qaq = human_born();
  act(qaq);
  learn(qaq);
  human_die(qaq);

  system("pause");

  return 0;
}