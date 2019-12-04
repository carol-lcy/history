#include <stdio.h>
#include <limits.h>
#include "supergraph.h"

query_helper* engine_setup(size_t n_processors) {
	return NULL;
}

int length;//the length of result
int* position;
int *arr_reposts;
result* find_all_reposts(post* posts, size_t count, uint64_t post_id, query_helper* helper) {
	length = 0;
	int inque = 0;
	int deque = 0;
	//position to store every index of post
	if(position == NULL)
	{
		inque = 0;
		deque = 0;
		position = (int*)calloc(count,sizeof(int));
		arr_reposts = (int*)calloc(count,sizeof(int));
	}
	
	int find = 0;
	int index = 0; //record the index of post id in posts array 
	result* ret = malloc(sizeof(result));
	for(int i = 0; i < count; i++)
	{
		if(posts[i].pst_id == post_id)
		{
			find = 1;
			index = i;
			length = 1;
			break;
		}
	}
	if(find == 0)
	{
		free(arr_reposts);
		free(position);
		position = NULL;
		ret->elements = NULL;
		ret->n_elements = 0;
		return ret;
	}
	//enqueue
	position[index] = 1;
	arr_reposts[inque] = index;
	inque = 1;
	deque = 0;
	
	while(deque < inque)
	{
		//deque
		int cur = arr_reposts[deque];
		deque++;
		if(posts[cur].n_reposted != 0)
		{
			for(int i = 0; i<posts[cur].n_reposted; i++)
			{
				if(position[posts[cur].reposted_idxs[i]] != 1)
				{
					position[posts[cur].reposted_idxs[i]] = 1;
					length = length+1;
				}
				arr_reposts[inque] = posts[cur].reposted_idxs[i];
				inque++;
			}
		}
	}
	//get all corresponding post of index in position array, and set to result
	struct post** arr = malloc(sizeof(post*)*length);
	int j = 0;
	for(int i = 0; i<count; i++)
	{
		if(position[i] == 1)
		{
			// printf("at postion: %d\n",i);
			arr[j] = &posts[i];
			j++;
		}
	}
	free(arr_reposts);
	free(position);
	position = NULL;
	ret->elements = (void **)arr;
	ret->n_elements = length;
	return ret;
}

//--------------------------

int origin;//store the original post index
result* find_original(post* posts, size_t count, uint64_t post_id, query_helper* helper) {
	int find = 0;
	int index = 0; //record the index of post id in posts array 
	result* ret = malloc(sizeof(result));
	for(int i = 0; i < count; i++)
	{
		if(posts[i].pst_id == post_id)
		{
			find = 1;
			index = i;
			break;
		}
	}
	if(find == 0)
	{
		ret->elements = NULL;
		ret->n_elements = 0;
		return ret;
	}
	origin = index;
	// int rewind = 0;
	for(int i = 0; i<count; i++)
	{
		if(posts[i].n_reposted > 0)
		{
			for(int j = 0; j < posts[i].n_reposted; j++)
			{
				if(posts[i].reposted_idxs[j] == origin)
				{
					origin = i;
					i = -1;
					break;
				}
			}
		}
	}

	struct post** arr = malloc(sizeof(post*));
	arr[0] = &posts[origin];
	ret->elements = (void **)arr;
	ret->n_elements = 1;
	return ret;
}

//-----------------------------

int *shortest;
int *visit;
int *path;
int *arr_shortest;
int *len_shortest;
result* shortest_user_link(user* users, size_t user_count, uint64_t userA, uint64_t userB, query_helper* helper) {
	result* ret = malloc(sizeof(result));
	//check A and B, and get their index
	if(userA == userB)
	{
		ret->elements = NULL;
		ret->n_elements = 0;
		return ret;
	}
	int check = 0;
	int A_index;
	int B_index;
	for(int i = 0; i<user_count; i++)
	{
		if(users[i].user_id == userA)
		{
			check++;
			A_index = i;
		}
		else if(users[i].user_id == userB)
		{
			check++;
			B_index = i;
		}
	}
	//if any of them cannot find in the user array return null
	if(check != 2)
	{
		ret->elements = NULL;
		ret->n_elements = 0;
		return ret;
	}
	
	/*
	to both check A-->B and B-->A
	outgoing is to store the path from A to B
	n_outgoing is the length of path
	ingoing is to store the path from B to A
	n_ingoing is the length of path
	*/
	user** outgoing = NULL;
	int n_outgoing = 0;
	user** ingoing = NULL;
	int n_ingoing = 0;
	
	int inque = 0;
	int deque = 0;
	
	//inital Queue, and set visit and path array
	if(arr_shortest == NULL)
	{
		visit = (int *)calloc(user_count,sizeof(int));
		path = (int *)calloc(user_count,sizeof(int));
		arr_shortest = (int *)calloc(user_count,sizeof(int));
		len_shortest = (int *)calloc(user_count,sizeof(int));
		inque = 0;
		deque = 0;
	}
	//put a in queue
	arr_shortest[inque] = A_index;
	inque = 1;
	deque = 0;
	len_shortest[A_index] = 1;
	visit[A_index] = 1;
	path[A_index] = A_index;

	int find = 0;
	while( deque < inque )
	{
		if(find == 1)
		{
			break;
		}
		//deque, get current use
		int cur = arr_shortest[deque];
		deque++;
		//check the following(out edges) are visited or not
		for(int i = 0; i<users[cur].n_following;i++)
		{
			//not visited
			if(visit[users[cur].following_idxs[i]] != 1)
			{
				//if the out going edge is B
				if(users[cur].following_idxs[i] == B_index)
				{
					//get the number of path in current, +1 for B
					n_outgoing = len_shortest[cur]+1;
					visit[B_index] = 1;
					path[B_index] = cur;
					outgoing = malloc(sizeof(user*)*n_outgoing);
					
					int start = B_index;
					for(int j = n_outgoing-1; j>= 0; j--)
					{
						outgoing[j] = &users[start];
						start = path[start];
					}
					find = 1;
					break;
				}
				else
				{
					arr_shortest[inque] = users[cur].following_idxs[i];
					inque++;
					len_shortest[users[cur].following_idxs[i]] = len_shortest[cur]+1;
					visit[users[cur].following_idxs[i]] = 1;
					path[users[cur].following_idxs[i]] = cur;
				}
			}
		}
	}
	// visit[0] = '\0';
	// visit[user_count-1] = '\0';
	free(visit);
	free(path);
	free(len_shortest);
	free(arr_shortest);
	arr_shortest = NULL;
	//inital Q queue
	if(arr_shortest == NULL)
	{
		visit = (int *)calloc(user_count,sizeof(int));
		path = (int *)calloc(user_count,sizeof(int));
		arr_shortest = (int *)calloc(user_count,sizeof(int));
		len_shortest = (int *)calloc(user_count,sizeof(int));
		inque = 0;
		deque = 0;
	}
	//put b in queue
	arr_shortest[inque] = B_index;
	inque = 1;
	deque = 0;
	len_shortest[B_index] = 1;
	visit[B_index] = 1;
	path[B_index] = B_index;

	find = 0;
	while( deque < inque )
	{
		if(find == 1)
		{
			break;
		}
		//deque, get current use
		int cur = arr_shortest[deque];
		deque++;
		//check the following(out edges) are visited or not
		for(int i = 0; i<users[cur].n_following;i++)
		{
			//not visited
			if(visit[users[cur].following_idxs[i]] != 1)
			{
				//if the out going edge is A
				if(users[cur].following_idxs[i] == A_index)
				{
					//get the number of path in current, +1 for A
					n_ingoing = len_shortest[cur]+1;
					visit[A_index] = 1;
					path[A_index] = cur;
					ingoing = malloc(sizeof(user*)*n_ingoing);
					
					int start = A_index;
					for(int j = n_ingoing-1; j>= 0; j--)
					{
						ingoing[j] = &users[start];
						start = path[start];
					}
					find = 1;
					break;
				}
				else
				{
					arr_shortest[inque] = users[cur].following_idxs[i];
					inque++;
					len_shortest[users[cur].following_idxs[i]] = len_shortest[cur]+1;
					visit[users[cur].following_idxs[i]] = 1;
					path[users[cur].following_idxs[i]] = cur;
				}
			}
		}
	}
	free(visit);
	free(path);
	free(arr_shortest);
	free(len_shortest);
	arr_shortest = NULL;
	if(n_outgoing == 1 && n_ingoing == 1)
	{
		ret->elements = NULL;
		ret->n_elements = 0;
		free(outgoing);
		free(ingoing);
		return ret;
	}
	if(n_outgoing == 0 || n_outgoing == 1)
	{
		ret->elements = (void**)ingoing;
		ret->n_elements = n_ingoing;
		free(outgoing);
	}
	if(n_ingoing == 0 || n_ingoing == 1)
	{
		ret->elements = (void**)outgoing;
		ret->n_elements = n_outgoing;
		free(ingoing);
	}
	else if(n_outgoing > n_ingoing)
	{
		ret->elements = (void**)ingoing;
		ret->n_elements = n_ingoing;
		free(outgoing);
	}
	else
	{
		ret->elements = (void**)outgoing;
		ret->n_elements = n_outgoing;
		free(ingoing);
	}
	return ret;
}


int* robot;
result* find_bots(user* users, size_t user_count, post* posts, size_t post_count, criteria* crit, query_helper* helper) {
	int robnum = 0;
	//initial
	if(robot == NULL)
	{
		robot = (int *)calloc(user_count,sizeof(int));
	}
	for(int i = 0; i<user_count; i++)
	{
		robot[i] = 0;
	}
	int count = 0;
	for(int i = 0; i<user_count; i++)
	{
		float num_repost = 0;
		for(int j = 0; j<users[i].n_posts; j++)
		{
			result* res = find_original(posts,post_count,posts[users[i].post_idxs[j]].pst_id,helper);
			if(res->elements[0] != &posts[users[i].post_idxs[j]] && res->elements!=NULL)
			{
				num_repost++;
			}
			free(res->elements);
			free(res);
		}
		if((num_repost/users[i].n_posts) > crit->oc_threshold)
		{
			if(robot[i] != 1)
			{
				count++;
				robot[i] = 1;
				robnum++;
				continue;
			}
		}

		if((float)users[i].n_followers / (users[i].n_followers + users[i].n_following) < crit->acc_rep_threshold)
		{
			if(robot[i] != 1)
			{
				robot[i] = 1;
				robnum++;
				continue;
			}
		}
	}

	

	for(int i = 0; i<user_count; i++)
	{
		if(crit->bot_net_threshold == 1)
		{
			break;
		}
		if(robot[i] == 1)
		{
			continue;
		}
		float robfollower = 0;
		for(int j = 0; j < users[i].n_followers; j++)
		{
			if(robot[users[i].follower_idxs[j]] == 1)
			{
				robfollower++;
			}
		}
		if(robfollower/users[i].n_followers > crit->bot_net_threshold)
		{
			if(robot[i] != 1)
			{
				robot[i] = 1;
				robnum++;
				i = -1;
			}
		}
	}
	
	result* ret = malloc(sizeof(result));
	if(robnum == 0)
	{
		free(robot);
		robot = NULL;
		ret->elements = NULL;
		ret->n_elements = 0;
		return ret;
	}
	struct user** arr = malloc(sizeof(user*)*robnum);
	int j = 0;
	for(int i = 0; i<user_count; i++)
	{
		if(robot[i] == 1)
		{
			arr[j] = &users[i];
			j++;
		}
	}
	free(robot);
	robot = NULL;
	ret->elements = (void **)arr;
	ret->n_elements = robnum;
	return ret;
}

void engine_cleanup(query_helper* helpers) {
	//Clean up your engine
}


