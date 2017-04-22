/**
 * Implements a dictionary's functionality.
 */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

//Counter for number of words in dictionary.
int *word_count = 0;
node *root = NULL;

int char_index(const char *c)
{
    int index_result = (int)c - (int)'a';
    return index_result;
}

//Returns a new trie node
struct node *new_node(void)
{
  struct node *tNode = NULL;

  tNode = (struct node *)malloc(sizeof(node));

  if (tNode)
  {
    tNode->is_word = false;

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
      tNode->children[i] = NULL;
    }
  }

  return tNode;
}

void insert_node(node *root, char cur_word[LENGTH])
{
    int level;
    int length = strlen(cur_word);
    int cur_child;

    node *crawl_temp = root;

	for (level = 0; level < length; level++)
	{
		cur_child = char_index(&cur_word[level]);
		if (crawl_temp->children[cur_child] == NULL)
		{
			crawl_temp->children[cur_child] = new_node();
		}
		crawl_temp = crawl_temp->children[cur_child];
	}
	crawl_temp->is_word = true;
	word_count++;
}

void free_trie(node *root)
{
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		if (root->children[i] != NULL)
		{
			free_trie(root->children[i]);
		}
	}
	free(root);
	root = NULL;
}



/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
	int level;
	int length = strlen(word);
	int cur_child;

	node *crawl_temp = root;

	for (level = 0; level < length; level++)
	{
		cur_child = char_index(&word[level]);
		if (crawl_temp->children[cur_child] == NULL)
		{
			return false;
		}

		crawl_temp = crawl_temp->children[cur_child];
	}
    if (crawl_temp != NULL && crawl_temp->is_word == true)
    {
        return true;
    }
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
  //Open dictionary file, error if does not open.
  FILE* dict_in = fopen(dictionary, "rb");
  if (dict_in == NULL)
	{
		fprintf(stderr, "Could not open dictionary file.\n");
		return false;
	}

  char cur_word[LENGTH];
  root = new_node();

	while(1)
	{
		//Gets characters one at a time from dictionary file.
		int c = fgetc(dict_in);
		int word_index = 0;

  	if (c != '\n')
  	{
    	//Create a string for the current word being stored.
    	cur_word[word_index] = (char)c;
    	word_index++;
  	}
  	else
  	{
    	//Check for the word in the trie, insert if doesn't exist.
		insert_node(root, cur_word);
  	}
	}
  return false;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
	if (root != NULL)
		{
			return *word_count;
		}
    return 0;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
	free_trie(root);
	if (root == NULL)
	{
		return true;
	}
	else
	{
  	return false;
	}
}
