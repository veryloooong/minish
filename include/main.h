#pragma once

#define __UNUSED __attribute__((unused))

/**
 * @brief Represents a path in a linked list.
 *
 * The `path_t` struct is used to store a path as a string and a pointer to the
 * next path in the linked list.
 */
typedef struct path_t {
  char *path;          /** The path string. */
  struct path_t *next; /** Pointer to the next path in the linked list. */
} Path;

/**
 * @brief Represents a linked list of paths.
 *
 * The `path_list_t` struct is used to store a linked list of paths.
 */
typedef struct path_list_t {
  Path *head; /** Pointer to the first path in the linked list. */
} PathList;
