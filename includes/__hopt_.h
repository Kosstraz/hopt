#ifndef __HOPT__H
#define __HOPT__H

# define _XOPEN_SOURCE 700
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <ctype.h>
# include <assert.h>
# include <stddef.h>
# include <stdarg.h>
# include "hopt.h"

# define BOOL	char
# define TRUE	1
# define FALSE	0

# define CASSERT(condition) ((void)sizeof(char[-!(condition)]))

//extern BOOL (*hopt_cb_earlyquit)(const char*, unsigned int) = NULL;
extern BOOL			hopt_end_on_arg_v;	// Capabilities of HOPT
extern BOOL			hopt_undef_allowed;	// Capabilities of HOPT
extern BOOL			hopt_redef_allowed;	// Capabilities of HOPT
extern BOOL			hopt_redef_overwrt;	// Capabilities of HOPT
extern void**		hopt_mallocd;	// Store all addresses malloc'd
extern unsigned int	hopt_c_mallocd;		// Count addresses malloc'd
extern t_hopt_map*	hopt_maps[HOPT_MAX_OPTIONS];
extern unsigned int	hopt_c_maps;

//static char*		hopt_help_str		= NULL;	// !WORK IN PROGRESS!

typedef struct hopt_sort
{
	unsigned int		index;
	unsigned int		argc;
	struct hopt_sort*	next;
}	t_hopt_sort;

typedef struct FINDER
{
	BOOL			error;
	BOOL			found;
	BOOL			strso;		// Know if is a STRing of Shorts Options
	unsigned int	last_i;		// Last index, to know the begin position of an option with argument(s)
	t_hopt_sort*	head;
	unsigned int	addrs_idx;
}	t_FINDER;

typedef struct hopt
{
	int				ac;			// Program arguments count
	char**			av;			// Program arguments value
	BOOL*			flags;		// To check if an option is redefined
	unsigned int	oac;		// Option Argument Count
	unsigned int	offset;		// Offset in the buffer
	unsigned int	n_parsed;	// Count of element parsed (included options' arguments)
	t_FINDER		f;			// FINDER(...) infos
}	t_hopt;

// test.c
void			hopt_test_alignment(void);

// lst.c
t_hopt_sort*	hopt_new_node(unsigned int index, unsigned int argc);
void			hopt_add_front(t_hopt_sort** head, t_hopt_sort* node);
void			hopt_add_back(t_hopt_sort** head, t_hopt_sort* node);
void			hopt_free_lstsort(t_hopt_sort* head);

// utils.c
char**			strsplit(const char* restrict s, char sep);
void			free2(void**  ptr2);
unsigned int	strlen2(char* restrict* restrict s);
unsigned int	optorder_total(unsigned int* optorder, unsigned int count);

// core.c
int
SORT(int ac, /*const*/ char** av, t_hopt_sort* head);
void
FINDER(t_hopt* restrict h);

#endif
