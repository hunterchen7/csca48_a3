/**
 * CSC A48 - Intro to Computer Science II, Summer 2020
 * 
 * Assignment 3 - Graffit
 * 
 * Graphs &
 * Recursion
 * Assignment
 * For
 * Freshmen
 * In
 * Toronto
 *
 * (I am so proud of that initialism.)
 * 
 * This is the program file where you will implement your solution for
 * assignment 3. Please make sure you read through this file carefully
 * and that you understand what is provided and what you need to complete.
 * 
 * You will also need to have read the handout carefully. 
 * 
 * Parts where you have to implement functionality are clearly labeled TODO
 * 
 * Be sure to test your work thoroughly, our testing will be extensive
 * and will check that your solution is *correct*, not only that it
 * provides functionality.
 * 
 * (c) 2020 William Song, Mustafa Quraish
 **/

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_STR_LEN 1024
#ifndef __testing
#define MAT_SIZE 3	// A small graph
#endif

typedef struct user_struct {
    char name[MAX_STR_LEN];
    struct friend_node_struct* friends; //linked list
    struct brand_node_struct* brands; //linked list
    bool visited;
} User;

typedef struct friend_node_struct { //linked list
    User* user; //contains a linked list of friends and a linked list of brands
    struct friend_node_struct* next;
} FriendNode;

typedef struct brand_node_struct { //linked list
    char brand_name[MAX_STR_LEN];
    struct brand_node_struct* next;
} BrandNode;


/** Note: We are reusing the FriendNode here as a Linked List for allUsers.
  * This is usually bad coding practice but it will allow us to reuse the
  * helper functions.
  **/
FriendNode* allUsers; 

int brand_adjacency_matrix[MAT_SIZE][MAT_SIZE];
char brand_names[MAT_SIZE][MAX_STR_LEN];

/**
 * Checks if a user is inside a FriendNode LL.
 **/
bool in_friend_list(FriendNode *head, User *node) {
  if (!node) return false;
  for (FriendNode *cur = head; cur != NULL; cur = cur->next) {
    if (strcmp(cur->user->name, node->name) == 0) {
      return true;
    }
  }
  return false;
}

/**
 * Checks if a brand is inside a BrandNode LL.
 **/
bool in_brand_list(BrandNode *head, char *name) {
  for (BrandNode *cur = head; cur != NULL; cur = cur->next) {
    if (strcmp(cur->brand_name, name) == 0) {
      return true;
    }
  }
  return false;
}

/**
 * Inserts a User into a FriendNode LL in sorted position. If the user 
 * already exists, nothing is done. Returns the new head of the LL.
 **/
FriendNode *insert_into_friend_list(FriendNode *head, User *node) {
  if (node == NULL) return head;

  if (in_friend_list(head, node)) {
    printf("User already in list\n");
    return head;
  }
  FriendNode *fn = calloc(1, sizeof(FriendNode));
  fn->user = node;
  fn->next = NULL;

  if (head == NULL)
    return fn;
    
  if (strcmp(head->user->name, node->name) > 0) {
    fn->next = head;
    return fn;
  } 

  FriendNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->user->name, node->name) < 0;
       cur = cur->next)
    ;
  fn->next = cur->next;
  cur->next = fn;
  return head;
}

/**
 * Inserts a brand into a BrandNode LL in sorted position. If the brand 
 * already exists, nothing is done. Returns the new head of the LL.
 **/
BrandNode *insert_into_brand_list(BrandNode *head, char *node) {
  if (node == NULL) return head;

  if (in_brand_list(head, node)) {
    printf("Brand already in list\n");
    return head;
  }
  BrandNode *fn = calloc(1, sizeof(BrandNode));
  strcpy(fn->brand_name, node);
  fn->next = NULL;

  if (head == NULL)
    return fn;
    
  if (strcmp(head->brand_name, node) > 0) {
    fn->next = head;
    return fn;
  } 

  BrandNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->brand_name, node) < 0;
       cur = cur->next)
    ;
  fn->next = cur->next;
  cur->next = fn;
  return head;
}

/**
 * Deletes a User from FriendNode LL. If the user doesn't exist, nothing is 
 * done. Returns the new head of the LL.
 **/
FriendNode *delete_from_friend_list(FriendNode *head, User *node) {
  if (node == NULL) return head;

  if (!in_friend_list(head, node)) {
    printf("User not in list\n");
    return head;
  }

  if (strcmp(head->user->name, node->name) == 0) {
    FriendNode *temp = head->next;
    free(head);
    return temp;
  }

  FriendNode *cur;
  for (cur = head; cur->next->user != node; cur = cur->next)
    ;

  FriendNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);
  return head;
}

/**
 * Deletes a brand from BrandNode LL. If the user doesn't exist, nothing is 
 * done. Returns the new head of the LL.
 **/
BrandNode *delete_from_brand_list(BrandNode *head, char *node) {
  if (node == NULL) return head;

  if (!in_brand_list(head, node)) {
    printf("Brand not in list\n");
    return head;
  }

  if (strcmp(head->brand_name, node) == 0) {
    BrandNode *temp = head->next;
    free(head);
    return temp;
  }

  BrandNode *cur;
  for (cur = head; strcmp(cur->next->brand_name, node) != 0; cur = cur->next)
    ;

  BrandNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);
  return head;
}

/**
 * Prints out the user data.
 **/
void print_user_data(User *user) {
  printf("User name: %s\n", user->name);
  printf("Friends:\n");
  for (FriendNode *f = user->friends; f != NULL; f = f->next) {
    printf("   %s\n", f->user->name);
  }
  printf("Brands:\n");
  for (BrandNode *b = user->brands; b != NULL; b = b->next) {
    printf("   %s\n", b->brand_name);
  }
}

/**
 * Get the index into brand_names for the given brand name. If it doesn't
 * exist in the array, return -1
 **/
int get_brand_index(char *name) {
  for (int i = 0; i < MAT_SIZE; i++) {
    if (strcmp(brand_names[i], name) == 0) {
      return i;
    }
  }
  printf("brand '%s' not found\n", name);
  return -1; // Not found
}

/**
 * Print out brand name, index and similar brands.
 **/
void print_brand_data(char *brand_name) {
  int idx = get_brand_index(brand_name);
  if (idx < 0) {
    printf("Brand '%s' not in the list.\n", brand_name);
    return;
  }
  printf("Brand name: %s\n", brand_name);
  printf("Brand idx: %d\n", idx);
  printf("Similar brands:\n");
  for (int i = 0; i < MAT_SIZE; i++) {
    if (brand_adjacency_matrix[idx][i] == 1 && strcmp(brand_names[i], "") != 0) {
      printf("   %s\n", brand_names[i]);
    }
  }
}

/**
 * Read from a given file and populate a the brand list and brand matrix.
 **/
void populate_brand_matrix(char* file_name) {
    // Read the file
    char buff[MAX_STR_LEN];
    FILE* f = fopen(file_name, "r");
    fscanf(f, "%s", buff);
    char* line = buff;
    // Load up the brand_names matrix
    for (int i = 0; i < MAT_SIZE; i++) {
        if (i == MAT_SIZE - 1) {
            strcpy(brand_names[i], line);
            break;
        }
        int index = strchr(line, ',') - line;
        strncpy(brand_names[i], line, index);
        line = strchr(line, ',') + sizeof(char);
    }
    // Load up the brand_adjacency_matrix
    for (int x = 0; x < MAT_SIZE; x++) {
        fscanf(f, "%s", buff);
        for (int y = 0; y < MAT_SIZE; y++) {
            int value = (int) buff[y*2];
            if (value == 48) { value = 0; }
            else {value = 1;}
            brand_adjacency_matrix[x][y] = value;
        }
    }
}

// Users
/**TODO: Complete this function
 * Creates and returns a user. Returns NULL on failure.
 **/
User* create_user(char* name) {
  User *user = (User*)malloc(sizeof(User));
  if (name != NULL) {
    strcpy(user->name, name);
    user->friends = NULL;
    user->brands = NULL;
    user->visited = false;
    if (!in_friend_list(allUsers, user)) { //name doesn't yet exist
      allUsers = insert_into_friend_list(allUsers, user);
      return user;
    }
  }
  return NULL;
}

/**TODO: Complete this function
 * Deletes a given user. 
 * Returns 0 on success, -1 on failure.
 **/
int delete_user(User* user) {
  if (!user) return -1;
  for (FriendNode *curr = allUsers; curr != NULL; curr = curr->next) {
    if (!strcmp(curr->user->name,user->name)) { //if next is user we're looking for then the allUsers list now skips it
      //FriendNode *temp = curr;
      for (FriendNode *friend = curr->user->friends; friend != NULL; friend = friend->next) {
        friend->user->friends = delete_from_friend_list(friend->user->friends, user);
      }
      allUsers = delete_from_friend_list(allUsers, user);
      return 0;
    }
  }
  return -1;
}

/**TODO: Complete this function
 * Create a friendship between user and friend.
 * Returns 0 on success, -1 on failure.
 **/
int add_friend(User* user, User* friend) {
  if (!user || !friend) return -1;
  if (!in_friend_list(user->friends, friend)) { //not a friend
    user->friends = insert_into_friend_list(user->friends, friend);
    friend->friends = insert_into_friend_list(friend->friends, user);
    return 0;
  } else {
    return -1;
  }
}

/**TODO: Complete this function
 * Removes a friendship between user and friend.
 * Returns 0 on success, -1 on faliure.
 **/
int remove_friend(User* user, User* friend) {
  if (in_friend_list(user->friends, friend)) {
    delete_from_friend_list(user->friends, friend);
    delete_from_friend_list(friend->friends, user);
    return 0;
  } else {
    return -1;
  }
}

/**TODO: Complete this function
 * Creates a follow relationship, the user follows the brand.
 * Returns 0 on success, -1 on faliure.
 **/
int follow_brand(User* user, char* brand_name) {
  if (!brand_name || !user) return -1;
  if (!in_brand_list(user->brands, brand_name)) {
    user->brands = insert_into_brand_list(user->brands, brand_name);
    return 0;
  } else {
    return -1;
  }
}

/**TODO: Complete this function
 * Removes a follow relationship, the user unfollows the brand.
 * Returns 0 on success, -1 on faliure.
 **/
int unfollow_brand(User* user, char* brand_name) {
  if (in_brand_list(user->brands, brand_name)) {
    delete_from_brand_list(user->brands, brand_name);
    return 0;
  } else {
    return -1;
  }
}

/**TODO: Complete this function
 * Return the number of mutual friends between two users.
 **/
int get_mutual_friends(User* a, User* b) {
  int mutual_friends = 0;
  for (FriendNode *a_friend = a->friends; a_friend != NULL; a_friend = a_friend->next) {
    if (in_friend_list(b->friends, a_friend->user)) mutual_friends++;
  }
  return mutual_friends;
}

/**TODO: Complete this function
 * A degree of connection is the number of steps it takes to get from
 * one user to another.
 * 
 * For example, if X & Y are friends, then we expect to recieve 1 when calling
 * this on (X,Y). Continuing on, if Y & Z are friends, then we expect to
 * recieve 2 when calling this on (X,Z).
 * 
 * Returns a non-negative integer representing the degrees of connection
 * between two users, -1 on failure.
 **/

int get_index(FriendNode *head, User *user) {
  int i = 0;
  FriendNode *curr = allUsers;
  while (curr != NULL) {
    if (!strcmp(curr->user->name, user->name)) return i;
    curr = curr->next;
    i++;
  }
  return -1; //strcmp got 0;
}

int get_num_users(FriendNode *head) {
  int num_users = 0;
  FriendNode *curr = allUsers;
  while (curr != NULL) {
    num_users++;
    curr = curr->next;
  }
  return num_users;
}

typedef struct node {
  char name[MAX_STR_LEN];
  int prev; //index
} Node;

void print_arr(short *arr, int size) {
  for (int i = 0; i < size; i++) {
    short x = arr[i];
    if (x < 0 || x > 10) {
      printf(" %d ",arr[i]);
    } else {
      printf("  %d ",arr[i]);
    }
    
  }
  printf("\n");
}

int get_degrees_of_connection(User* a, User* b) {
  //printf("--------------start-------------\n");
  if (a == NULL || b == NULL) return -1; //one or both is NULL
  if (!strcmp(a->name, b->name)) return 0; //base case, a is b

  FriendNode *head = (FriendNode*) malloc(sizeof(FriendNode));
  head->next = NULL;
  head->user = a;
  FriendNode *tail = head; //tail is initially head because it's 1 item

  int num_users = get_num_users(allUsers);

  int index_a = get_index(allUsers, a); //"index" of user a, indicies of course don't actually exist in this sense because it's a linked list
  int index_b = get_index(allUsers, b); //index of b, i.e. target index

  if (index_b == -1 || index_a == -1) return -1; //one or the other doesn't exist

  //using shorts because 255 isn't enough and int_max is probably overkill
  short *prevNode = (short *) calloc(num_users, sizeof(short)); //index of the node it came from
  short *userDist = (short *) calloc(num_users, sizeof(short)); //distance of each node from "origin"

  //set all distances in userDist to -1, that way we can check if a node has been visited yet
  for (int i = 0; i < num_users; i++) {
    userDist[i] = -1;
    prevNode[i] = -1;
  }

  prevNode[index_a] = -5; //user a has no prior node as it is the origin
  userDist[index_a] = 0; //distance from origin to origin is 0

  while (head != NULL) { //loop while the queue is not empty
    int index = get_index(allUsers, head->user); //index in the userDist array, this value will be changed
    
    if (index == index_b) { //reached target
      int dist = userDist[prevNode[index]] + 1; //prevNode[index] is the index of the prior node
      free(prevNode); //free my arrays
      free(userDist);
      return dist;
    }

    for (FriendNode *curr = head->user->friends; curr != NULL; curr = curr->next) { //iterate through friends of queue head
      int curr_index = get_index(allUsers, curr->user);
      if (userDist[curr_index] == -1 && prevNode[curr_index] == -1) { //a node is unvisited if neither of it's arrays have been touched
        prevNode[curr_index] = index; //the index of the previous node is stored in the prevNode array
        //enque
        FriendNode *new_tail = (FriendNode*) malloc(sizeof(FriendNode));
        tail->next = new_tail;
        new_tail->user = curr->user;
        new_tail->next = NULL;
        tail = new_tail;
        //int placeholder = 0;
      }
    }
  
    if (index != index_a) userDist[index] = userDist[prevNode[index]] + 1; //the node at this index is now visited because it has a distance
    //however, we don't want to change the value at index_a to anything else since that is a base case

    //deque
    FriendNode *temp = head;
    head = head->next;
    free(temp);
  }

  return -1; //no path was found in all nodes

}

// Brands
/**TODO: Complete this function
 * Marks two brands as similar.
 **/
void connect_similar_brands(char* brandNameA, char* brandNameB) {
  brand_adjacency_matrix[get_brand_index(brandNameA)][get_brand_index(brandNameB)] = 1;
  brand_adjacency_matrix[get_brand_index(brandNameB)][get_brand_index(brandNameA)] = 1;
}

/**TODO: Complete this function
 * Marks two brands as not similar.
 **/
void remove_similar_brands(char* brandNameA, char* brandNameB) {
  brand_adjacency_matrix[get_brand_index(brandNameA)][get_brand_index(brandNameB)] = 0;
  brand_adjacency_matrix[get_brand_index(brandNameB)][get_brand_index(brandNameA)] = 0;
}

// Harder ones
/**TODO: Complete this function
 * Returns a suggested friend for the given user, returns NULL on failure.
 * See the handout for how we define a suggested friend.
 **/

int mutual_brands_count(User *a, User *b) {
  int mutual = 0;
  for (BrandNode *a_brand = a->brands; a_brand != NULL; a_brand = a_brand->next) {
    if (in_brand_list(b->brands, a_brand->brand_name)) mutual++;
  }
  return mutual;
}

User* get_suggested_friend(User* user) {
  User *suggested_friend = NULL;
  int max_mutual = 0;
  for (FriendNode *temp = allUsers; temp != NULL; temp = temp->next) {
    if (!in_friend_list(user->friends, temp->user) && strcmp(user->name, temp->user->name) != 0) { 
      //temp is not in user's friend list and temp is not the user
      if (mutual_brands_count(user, temp->user) > max_mutual) {
        suggested_friend = temp->user;
        max_mutual = mutual_brands_count(user, temp->user);
      } else if (mutual_brands_count(user, temp->user) == max_mutual) {
        if (suggested_friend == NULL || strcmp(temp->user->name, suggested_friend->name) > 0) suggested_friend = temp->user;
        //e.g. user == "z" and temp->user == "a"
        //ascii of z == 122, ascii of a == 97, 122 > 97 so strcmp returns > 0
      }
    }
  }
  return suggested_friend;
}

/**TODO: Complete this function
 * Friends n suggested friends for the given user.
 * See the handout for how we define a suggested friend.
 * Returns how many friends were successfully followed.
 **/
int add_suggested_friends(User* user, int n) {
  int success = 0;
  for (int i = 0; i < n; i++) {
    if (!add_friend(user, get_suggested_friend(user))) success++;
  }
  return success;
}

char *get_suggested_brand(User *user) {
  if (!user) return NULL;
  char *suggested_brand = NULL;
  int max_similarity = 0;
  
  for (int i = 0; i < MAT_SIZE; i++) {
    int similar = 0;
    for (BrandNode *temp = user->brands; temp != NULL; temp = temp->next) {
      int j = get_brand_index(temp->brand_name);
      if (brand_adjacency_matrix[i][j] && !in_brand_list(user->brands, brand_names[i])) similar++;
    }
    if (similar > max_similarity) {
      max_similarity = similar;
      suggested_brand = brand_names[i];
    } else if (similar == max_similarity) {
      if (suggested_brand == NULL || strcmp(suggested_brand, brand_names[i]) > 0) {
        suggested_brand = brand_names[i];
      }
    }
  }
  return suggested_brand;
}

/**TODO: Complete this function
 * Follows n suggested brands for the given user.
 * See the handout for how we define a suggested brand.     
 * Returns how many brands were successfully followed. 	  	
 **/
int follow_suggested_brands(User* user, int n) {
  int success = 0;
  for (int i = 0; i < n; i++) {
    if (!follow_brand(user, get_suggested_brand(user))) success++;
  }
  return success;
}
