#include "__hopt_.h"
#include "hopt.h"

int				hopt_nerr = 0;		// extern global var in 'hopt.h'
char			hopt_cerr[16] = {0};// extern global var in 'hopt.h'

BOOL			hopt_end_on_arg_v = FALSE;	// Capabilities of HOPT
BOOL			hopt_undef_allowed = FALSE;	// Capabilities of HOPT
BOOL			hopt_redef_allowed = FALSE;	// Capabilities of HOPT
BOOL			hopt_redef_overwrt = FALSE;	// Capabilities of HOPT
void**			hopt_mallocd		= NULL;	// Store all addresses malloc'd
unsigned int	hopt_c_mallocd		= 0U;		// Count addresses malloc'd
t_hopt_map*		hopt_maps[HOPT_MAX_OPTIONS] = {0};	// extern global var in '__hopt_.h'
unsigned int	hopt_c_maps = 0;					// extern global var in '__hopt_.h'

// Parse and interpret options for you :0
//
// @param ac Arguments' program count
// @param av Total arguments's program
// @param buffer Your buffer to stock validation
// @param optstr The options strings (in order of buffer bytes)
// @param optargcount The arguments options can handling (nullable if all options have ZERO arg)
int
hopt(int ac, char** av)
{
	t_hopt			h;

	memset(&h, 0, sizeof(h));
	h.ac = ac;
	h.av = av;
	h.flags = calloc(hopt_c_maps, sizeof(BOOL));
	FINDER(&h);
	SORT(ac, av, h.f.head);
	free(h.flags);
	if (hopt_nerr != HOPT_SUCCESS)
		return (-1);
	return (h.n_parsed);
}

static inline
t_hopt_map*
hopt_create_map(char* names, int argc, void* mem, va_list va)
{
	t_hopt_map* map = malloc(sizeof(t_hopt_map));
	map->names = names;
	map->argc = argc;
	map->mem = mem;
	map->type = HOPT_TYPE_STR;
	if (argc >= 1)
		map->type = va_arg(va, unsigned int);
	return (map);
}

// Create new option mapping
// @param name Option aliases (ex: "c=-count=j"), option '-c' has 2 others alises : '--count' and '-j'
// @param argc Count of option's arguments
// @param mem Address in memory to fill
// @param ... Type of each option's argument(s)
int
hopt_add_option(char* names, int argc, void* mem, ...)
{
#ifdef HOPT_DEBUG
	for (unsigned int i = 0 ; names[i] ; ++i)
		if (!isalnum(names[i]) && names[i] != '=' && names[i] != '-')
			return (-1);
#endif
	va_list	va;
	va_start(va, mem);
	hopt_maps[hopt_c_maps] = hopt_create_map(names, argc, mem, va);
	++hopt_c_maps;
	va_end(va);
	return (0);
}

void
hopt_free(void)
{
	if (hopt_mallocd)
	{
		for (unsigned int i = 0 ; i < hopt_c_mallocd ; ++i)
		{
			free(hopt_mallocd[i]);
			hopt_mallocd[i] = NULL;
		}
		free(hopt_mallocd);
		hopt_mallocd = NULL;
	}
	for (unsigned int i = 0 ; i < hopt_c_maps ; ++i)
	{
		free(hopt_maps[i]);
		hopt_maps[i] = NULL;
	}
}

char*
hopt_strerror(void)
{
	char*	test;
	int		size = strlen(hopt_cerr);

	switch (hopt_nerr)
	{
		case HOPT_MALLOCF:
			test = malloc((21 + 1) * sizeof(char));
			strncpy(test, "hopt: malloc failed.", 21);
			test[21] = 0;
			return (test);
		case HOPT_INVALID:
			test = malloc((34 + size + 1) * sizeof(char));
			snprintf(test, 34 + size, "hopt: option -%s arg count invalid.", hopt_cerr);
			test[34 + size] = 0;
			return (test);
		case HOPT_UNDEFINED:
			test = malloc((29 + size + 1) * sizeof(char));
			snprintf(test, 29 + size, "hopt: option -%s is undefined.", hopt_cerr);
			test[29 + size] = 0;
			return (test);
		case HOPT_REDEFINED:
			test = malloc((29 + size + 1) * sizeof(char));
			snprintf(test, 29 + size, "hopt: option -%s is redefined.", hopt_cerr);
			test[29 + size] = 0;
			return (test);
		case HOPT_BADOPTSTR:
			test = malloc((76 + size + 1) * sizeof(char));
			snprintf(test, 76 + size, "hopt: option -%s in bad string format, call hopt_help() to see string format.", hopt_cerr);
			test[76 + size] = 0;
			return (test);
		case HOPT_BADSORDER:
			test = malloc((111 + size + 1) * sizeof(char));
			snprintf(test, 111 + size, "hopt: option -%s is in bad order, he need argument(s) but is not in last position in a string of shorts options.", hopt_cerr);
			test[111 + size] = 0;
			return (test);
		case HOPT_MISSOARGC:
			test = malloc((33 + size + 1) * sizeof(char));
			snprintf(test, 33 + size, "hopt: option -%s miss argument(s).", hopt_cerr);
			test[33 + size] = 0;
			return (test);
		default:
			test = malloc((15 + 1) * sizeof(char));
			strncpy(test, "hopt: success.", 15);
			test[15] = 0;
			return (test);
	}
}

// @param overwrite The redefinition will overwrite the ancient definition [sorted] (1), OR just be ignored [unsorted] (0) ?
void
hopt_allow_redef(BOOL overwrite)
{
	hopt_redef_overwrt = overwrite;
	hopt_redef_allowed = TRUE;
}

void
hopt_allow_undef(void)
{
	hopt_undef_allowed = TRUE;
}

void
hopt_end_on_arg(void)
{
	hopt_end_on_arg_v = TRUE;
}

void
hopt_reset(void)
{
	memset(hopt_cerr, 0, 16);
	hopt_nerr = 0;
	hopt_undef_allowed = FALSE;
	hopt_redef_allowed = FALSE;
	hopt_redef_overwrt = FALSE;
	hopt_end_on_arg_v = FALSE;
}

char*
hopt_see_hopt_help(void)
{
	return ("\e[1mHOPT HELP\e[0m:\n\
\e[1mac:\e[0m Your program's arguments count\n\
\e[1mav:\e[0m Your program's arguments value\n\
\e[1mbuffer:\e[0m Your programmed structure/buffer, declared like that: \n\
	\"typedef struct opt\n\
	{\n\
		char*	c;	// 0 arg\n\
		char	e;	// 0 arg\n\
		char*	a;	// 1 arg\n\
		char	d;	// 0 arg\n\
		char*	b;	// 1 arg\n\
		char*	f[4];	// 4 args\n\
	} t_opt;\"\n\
\e[1moptstr:\e[0m A string that represent each options possibles (long or not), to follow the structure exemple :\n\
	\e[2mhopt(ac, av, &_opt, \"c=-count:1/e/a:1/d=-detach/b=-bytes:1/f=o=-files-out:4\");\e[0m\n\
	The OPTSTR must have the same order than your structure/buffer.\n\
	When there is an '=' char, this is to allow different writing for an unique option (in this exemple, '-f' is along possible than '-o' or than '--files-output').\n\
	Long option must begin with '-' (like: \"-option\", for --option), and shortcut must not begin with that and must have a size of 1 (like: \"f\", for -f).\n");
}
