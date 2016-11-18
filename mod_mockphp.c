/* ====================================================================
 * Copyright (c) 2016 Jean Weisbuch for PHPNET
 *
 * Register php_module directives to avoid 500 errors in case PHP
 * module specific directives are set on configurations/htaccess
 * while php_module is not loaded.
 *
 * Logs calls to these directives if "log_mockphp" is set
 * Can be loaded along with mod_php (it should not interfere with it)
 * ==================================================================== */

#include "httpd.h"
#include "http_config.h"
#include "http_core.h"
#include "http_log.h"
#include "http_request.h"	// For ap_hook_fixups()

module AP_MODULE_DECLARE_DATA mockphp_module;
#ifdef APLOG_USE_MODULE
APLOG_USE_MODULE(mockphp);
#endif

typedef struct {
	int log_enabled;
} mod_mockphp_config_rec;

static mod_mockphp_config_rec config;

static int hook_fixup(request_rec *r) {
	return OK;
}

static void register_hooks(apr_pool_t * p) {
	// These modules will be loaded before mod_mockphp
	static const char * const aszPre[] = { "mod_php.c", "mod_php5.c", "mod_php7.c", "mpm_itk.c", "mod_vhs.c", NULL };
	ap_hook_fixups(hook_fixup, aszPre, NULL, APR_HOOK_FIRST);
}

// Enables logging of PHP directives call if "log_mockphp" is set to "On"
static const char *set_logging(cmd_parms *cmd, void *dummy, const int flag) {
	if(flag)	config.log_enabled = 1;
	else		config.log_enabled = 0;
	return NULL;
}

// If log_mockphp is set, logs calls to PHP directives as a warning on the errorlog
const char *log_action(cmd_parms *cmd, void *my_config, const char *arg1, const char *arg2) {

	if(!config.log_enabled) {
		// Logging is not enabled
		return NULL;
	}

	if(arg2 != "") {
		// Logging a directive that have two arguments (ex: php_value max_execution_time 30)
		ap_log_error(APLOG_MARK, APLOG_WARNING, 0, cmd->server, "Called a php_mod function on %s (%i) '%s %s %s'", cmd->directive->filename, cmd->directive->line_num, cmd->cmd->name, arg1, arg2);
	} else {
		// Logging a directive that has only one argument (ex: PHPINIDir /etc/php.ini)
		ap_log_error(APLOG_MARK, APLOG_WARNING, 0, cmd->server, "Called a php_mod function on %s (%i) '%s %s'", cmd->directive->filename, cmd->directive->line_num, cmd->cmd->name, arg1);
	}
	return NULL;
}

// Module configuration directives
const command_rec register_directives[] = {
	// Identical to php_dir_cmds[] from mod_php
        AP_INIT_TAKE2("php_value", log_action, NULL, OR_OPTIONS, "PHP Value Modifier"),
        AP_INIT_TAKE2("php_flag", log_action, NULL, OR_OPTIONS, "PHP Flag Modifier"),
        AP_INIT_TAKE2("php_admin_value", log_action, NULL, ACCESS_CONF|RSRC_CONF, "PHP Value Modifier (Admin)"),
        AP_INIT_TAKE2("php_admin_flag", log_action, NULL, ACCESS_CONF|RSRC_CONF, "PHP Flag Modifier (Admin)"),
        AP_INIT_TAKE12("PHPINIDir", log_action, NULL, RSRC_CONF, "Directory containing the php.ini file"),

	// Specific to mockphp
	AP_INIT_FLAG("log_mockphp", set_logging, NULL, RSRC_CONF, "Enable logging of PHP parameters used"),
        {NULL}
};

module AP_MODULE_DECLARE_DATA mockphp_module = {
	STANDARD20_MODULE_STUFF,
	NULL,			/* Per-directory configuration handler */
	NULL,			/* Merge handler for per-directory configurations */
	NULL,			/* Per-server configuration handler */
	NULL,			/* Merge handler for per-server configurations */
	register_directives,	/* Any directives we may have for HTTPD */
	register_hooks		/* Our hook registering function */
};
