#mod_mockphp - Apache HTTPD module that mocks mod_php
## Its primary goal is to ensure a swift transition from a server on which the PHP is executed via mod_php to a PHP executed via (F)CGI

This module register the mod_php configuration directives so they wont raise 500 errors in case they are used on the HTTPD config files (including htaccess).
The directives are not passed to PHP if executed via CGI but a possible evolution of the module would be to pass these to an environment variable that could be parsed by a wrapper to ensure full compatiblity.

Can be used along with mod_php loaded to log calls to these directives, it shouldnt alter mod_php functionalities.
It will not be able to log directives that are inside "<IfModule php5_module>" tags if the php module is not loaded.

mod_php configuration directives that are registered by this mod :
 * php_value
 * php_flag
 * php_admin_value
 * php_admin_flag
 * PHPINIDir

Configuration directive specific to this mod :
 * log_mockphp : If set to On, logs directives calls on the HTTPD error log with the loglevel warning


Configuration example :
```
LoadModule mockphp_module modules/mod_mockphp.so
<IfModule mockphp_module>
	log_mockphp On
</IfModule>
```

If you use a stricter loglevel than the default warning and want to log messages, for example with a global loglevel at error :
```
LogLevel error mockphp:warn
```

Example of logged calls (if log_mockphp is set) :
```
[servername] [Fri Nov 18 13:31:28.417251 2016] [mockphp:info] [pid 18766] Called a php_mod function on /etc/httpd/conf/httpd.conf (21) 'php_admin_value upload_max_filesize 666M'
[servername] [Fri Nov 18 13:57:39.408429 2016] [mockphp:info] [pid 19280] Called a php_mod function on /home/user/www/.htaccess (5) 'php_flag engine 0'
```
