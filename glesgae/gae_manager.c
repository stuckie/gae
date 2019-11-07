#include "gae_manager.h"


gae_manager_t* gae_manager_init(gae_manager_t* manager, unsigned int dataSize)
{
	gae_map_init(manager, sizeof(gae_hashstring), dataSize, gae_hashstring_compare);
	
	return manager;
}

void* gae_manager_add(gae_manager_t* manager, gae_hashstring name)
{
	unsigned int index = gae_array_length(&(manager->ids)) - 1;
	
	if (manager->values.used == manager->values.allocated) {
		gae_array_resize(&(manager->values), gae_array_length(&(manager->values)) + 1);
		index += 1;
	}
	
	gae_array_push(&(manager->ids), &name);
	manager->values.used += manager->values.size;
	return gae_array_get(&(manager->values), index);
}

void* gae_manager_get(gae_manager_t* manager, gae_hashstring name)
{
	return gae_map_get(manager, &name);
}

gae_manager_t* gae_manager_delete(gae_manager_t* manager, gae_hashstring name)
{
	gae_map_remove(manager, &name);
	return manager;
}

unsigned int gae_manager_count(gae_manager_t* manager)
{
	return gae_map_length(manager);
}

gae_manager_t* gae_manager_destroy(gae_manager_t* manager)
{
	gae_map_destroy(manager);
	return manager;
}
