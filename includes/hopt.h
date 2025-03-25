
#ifndef HOPT_H
#define HOPT_H 1

# define HOPT_VERSION	100

# define HOPT_SUCCESS	0
# define HOPT_MALLOCF	1
# define HOPT_INVALID	2
# define HOPT_UNDEFINED	3
# define HOPT_REDEFINED	4
# define HOPT_BADSORDER	5
# define HOPT_BADOPTSTR	6
# define HOPT_MISSOARGC	7

# ifndef HOPT_MAX_OPTIONS
// modifiable
#  define HOPT_MAX_OPTIONS	64
# endif

# define HOPT_TYPE_DEFAULT	0
# define HOPT_TYPE_STR		0
# define HOPT_TYPE_CHAR		1
# define HOPT_TYPE_SHORT	2
# define HOPT_TYPE_INT		3
# define HOPT_TYPE_LONG		4
# define HOPT_TYPE_FLOAT	5
# define HOPT_TYPE_DOUBLE	6

typedef struct hopt_map
{
	char*	names;
	int		argc;
	void*	mem;
	int		type;
}	t_hopt_map;

//void hopt_add_desc(...);
//void hopt_early_exit(BOOL (*fun)(char* option, int optlen));

// Error code to specify error (look at 'hopt_cerr' to see which option is causing the error)
extern int		hopt_nerr;
// The option that refers to the error stored in 'hopt_nerr'
extern char		hopt_cerr[16];

	/*
		CORE for HOPT
	*/

// Parse and interpret options for you :0
//
// @param ac Arguments' program count
// @param av Total arguments's program
// @param buffer Your buffer to stock validation
// @param optstr The options strings (in order of buffer bytes)
// @param optargcount The arguments options can handling (nullable if all options have ZERO arg)
int		hopt(int ac, char** av);
//int	hopt(int ac, char** av, const t_hopt_map* restrict map);

void	hopt_free(void);

// Create new option mapping
// @param name Option aliases (ex: "c=-count=j"), option '-c' has 2 others alises : '--count' and '-j'
// @param argc Count of option's arguments
// @param mem Address in memory to fill
// @param ... Type of each option's argument(s)
int		hopt_add_option(char* names, int argc, void* mem, ...);

	/*
		UTILS for HOPT
	*/

// Set description for options (format : "i=-flood(To flood network)/f=-files:2(Set out and in file)/...")
//// Return the HELP string associed to options
//char*	hopt_set_help(const char* restrict optstr_desc);
//// Return the HELP string associed to options (need to call HOPT_EST_HELP(str) before)
//char*	hopt_get_help(void);
// Return a string for the (potential) current error
// (needs to free)
char*	hopt_strerror(void);
char*	hopt_see_hopt_help(void);

	/*
		OPTIONS for HOPT
	*/

// Undef allowed is not sorted, same for undef unallowed
void	hopt_allow_undef(void);
// @param overwrite The redefinition will overwrite the ancient definition (1), OR just be ignored (0) ?
void	hopt_allow_redef(char overwrite);
void	hopt_end_on_arg(void);
void	hopt_reset(void);

#endif
