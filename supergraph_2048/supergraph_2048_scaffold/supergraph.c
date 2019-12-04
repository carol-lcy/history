#include "supergraph.h"

query_helper* engine_setup(size_t n_processors) {
	return NULL;
}

result* find_all_reposts(post* posts, size_t count, uint64_t post_id, query_helper* helper) {
	return NULL;
}

result* find_original(post* posts, size_t count, uint64_t post_id, query_helper* helper) {
	return NULL;
}

result* shortest_user_link(user* users, size_t count, uint64_t userA, uint64_t userB, query_helper* helper) {
	return NULL;
}

result* find_bots(user* users, size_t user_count, post* posts, size_t post_count, criteria* crit, query_helper* helper) {
	return NULL;
}

void engine_cleanup(query_helper* helpers) {
	//Clean up your engine
}


