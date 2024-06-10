#include <stddef.h>
#include <stdio.h>

typedef size_t Entity;

#define MAX_ENTITIES 10
struct EntityManager {
  Entity entity_count_;
  Entity free_usigs_[MAX_ENTITIES];

  unsigned long component_tracker_[MAX_ENTITIES]; // 64 possible components

  size_t idx_to_entity_[MAX_ENTITIES];
  size_t entity_to_idx_[MAX_ENTITIES];
};

void init(struct EntityManager* mgr) {
  for (int i = 0; i < MAX_ENTITIES; i++) {
    mgr->free_usigs_[i] = i;
    mgr->idx_to_entity_[i] = -1;
    mgr->entity_to_idx_[i] = -1;
  }
}

void debug(struct EntityManager* mgr) {
  printf("Entity Count\n%zu\n", mgr->entity_count_);

  printf("Free universal signatures\n");
  for (int i = 0; i < MAX_ENTITIES; i++) { printf("%02zu ", mgr->free_usigs_[i]); }
  printf("\n");

  printf("Idx -> Entity\n");
  for (int i = 0; i < MAX_ENTITIES; i++) { printf("%02ld ", (mgr->idx_to_entity_[i] == (size_t)(-1) ? (long)-1 : (long)mgr->idx_to_entity_[i])); }
  printf("\n");

  printf("Entity -> Idx\n");
  for (int i = 0; i < MAX_ENTITIES; i++) { printf("%02ld ", (mgr->entity_to_idx_[i] == (size_t)(-1) ? (long)-1 : (long)mgr->entity_to_idx_[i])); }
  printf("\n\n");
}

void addEntity(struct EntityManager* mgr, long component_tracker) {
  if (mgr->entity_count_ == MAX_ENTITIES) return;

  // get available universal signature
  Entity usig = mgr->free_usigs_[0];
  // update signature array to reflect the now unavailable signature
  mgr->free_usigs_[0] = mgr->free_usigs_[MAX_ENTITIES - mgr->entity_count_ - 1];
  mgr->free_usigs_[MAX_ENTITIES - mgr->entity_count_ - 1] = usig;

  // set values
  mgr->component_tracker_[mgr->entity_count_] = component_tracker;
  mgr->idx_to_entity_[mgr->entity_count_] = usig;
  mgr->entity_to_idx_[usig] = mgr->entity_count_;

  // update entity count
  mgr->entity_count_++;
}

void removeEntity(struct EntityManager* mgr, Entity entity) {
  printf("removing %zu\n", entity);

  mgr->entity_count_--;

  // pack array data
  size_t pack_idx = mgr->entity_to_idx_[entity];
  mgr->idx_to_entity_[pack_idx] = mgr->idx_to_entity_[mgr->entity_count_];
  mgr->component_tracker_[pack_idx] = mgr->idx_to_entity_[mgr->entity_count_];

  mgr->entity_to_idx_[mgr->idx_to_entity_[pack_idx]] = pack_idx;


  // remove moved / removed data
  mgr->entity_to_idx_[entity] = -1;
  mgr->idx_to_entity_[mgr->entity_count_] = -1;


  // update universal signatures to reflect freed signature
  int entity_usig_idx = -1;
  for (int i = MAX_ENTITIES; i >= 0; i--) {
    if (mgr->free_usigs_[i] != entity) continue;
    entity_usig_idx = i;
  }
  Entity last_used_usig = mgr->free_usigs_[MAX_ENTITIES - mgr->entity_count_ - 1];
  mgr->free_usigs_[MAX_ENTITIES - mgr->entity_count_ - 1] = mgr->free_usigs_[entity_usig_idx];
  mgr->free_usigs_[entity_usig_idx] = last_used_usig;
}

// <<<<<<< TESTING >>>>>>>
int main() {
  struct EntityManager mgr = {0, {0}, {0}, {0}, {0}};
  init(&mgr);
  debug(&mgr);

  for (int i = 0; i < 3; i++) {
    addEntity(&mgr, 0);
    debug(&mgr);
  }

  removeEntity(&mgr, 0);
  debug(&mgr);

  removeEntity(&mgr, 9);
  debug(&mgr);
}

// void removeEntity(struct EntityManager* mgr, Entity e) { }
