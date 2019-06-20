#include "builtin.h"
#include "config.h"
#include "parse-options.h"

static char const * const env__helper_usage[] = {
	N_("git env--helper [--mode-bool | --mode-ulong] --env-variable=<VAR> --env-default=<DEF> [<options>]"),
	NULL
};

int cmd_env__helper(int argc, const char **argv, const char *prefix)
{
	enum {
		ENV_HELPER_BOOL = 1,
		ENV_HELPER_ULONG,
	} cmdmode = 0;
	int exit_code = 0;
	int quiet = 0;
	const char *env_variable = NULL;
	const char *env_default = NULL;
	int ret;
	int ret_int, tmp_int;
	unsigned long ret_ulong, tmp_ulong;
	struct option opts[] = {
		OPT_CMDMODE(0, "mode-bool", &cmdmode,
			    N_("invoke git_env_bool(...)"), ENV_HELPER_BOOL),
		OPT_CMDMODE(0, "mode-ulong", &cmdmode,
			    N_("invoke git_env_ulong(...)"), ENV_HELPER_ULONG),
		OPT_STRING(0, "variable", &env_variable, N_("name"),
			   N_("which environment variable to ask git_env_*(...) about")),
		OPT_STRING(0, "default", &env_default, N_("value"),
			   N_("what default value does git_env_*(...) fall back on?")),
		OPT_BOOL(0, "exit-code", &exit_code,
			 N_("exit code determined by truth of the git_env_*() function")),
		OPT_BOOL(0, "quiet", &quiet,
			 N_("don't print the git_env_*() return value")),
		OPT_END(),
	};

	if (parse_options(argc, argv, prefix, opts, env__helper_usage, 0))
		usage_with_options(env__helper_usage, opts);
	if (!env_variable || !env_default ||
	    !*env_variable || !*env_default)
		usage_with_options(env__helper_usage, opts);

	switch (cmdmode) {
	case ENV_HELPER_BOOL:
		tmp_int = strtol(env_default, (char **)&env_default, 10);
		if (*env_default) {
			error(_("option `--default' expects a numerical value with `--mode-bool`"));
			usage_with_options(env__helper_usage, opts);
		}
		ret_int = git_env_bool(env_variable, tmp_int);
		if (!quiet)
			printf("%d\n", ret_int);
		ret = ret_int;
		break;
	case ENV_HELPER_ULONG:
		tmp_ulong = strtoll(env_default, (char **)&env_default, 10);
		if (*env_default) {
			error(_("option `--default' expects a numerical value with `--mode-ulong`"));
			usage_with_options(env__helper_usage, opts);
		}
		ret_ulong = git_env_ulong(env_variable, tmp_ulong);
		if (!quiet)
			printf("%lu\n", ret_ulong);
		ret = ret_ulong;
		break;
	}

	if (exit_code)
		return !ret;

	return 0;
}
