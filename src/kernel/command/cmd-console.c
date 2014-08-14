/*
 * kernel/command/cmd-console.c
 *
 * Copyright(c) 2007-2014 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <xboot.h>
#include <command/command.h>


#if	defined(CONFIG_COMMAND_CONSOLE) && (CONFIG_COMMAND_CONSOLE > 0)

static int console(int argc, char ** argv)
{
	if(argc != 2)
		return 0;

	if(strcmp(argv[1], "on") == 0)
	{
		console_onoff(console_get_stdin(), TRUE);
		console_onoff(console_get_stdout(), TRUE);
		console_onoff(console_get_stderr(), TRUE);
	}
	else if(strcmp(argv[1], "off") == 0)
	{
		console_onoff(console_get_stdin(), FALSE);
		console_onoff(console_get_stdout(), FALSE);
		console_onoff(console_get_stderr(), FALSE);
	}

	return 0;
}

static struct command_t console_cmd = {
	.name		= "console",
	.func		= console,
	.desc		= "turn on/off the standard console\r\n",
	.usage		= "console <on|off>\r\n",
	.help		= "    turn on/off the stdin,stdout and stderr console\r\n"
				  "    the options as below for details\r\n"
				  "    on      turn on the standard console\r\n"
				  "    off     turn off the standard console\r\n"
};

static __init void console_cmd_init(void)
{
	if(command_register(&console_cmd))
		LOG("Register command 'console'");
	else
		LOG("Failed to register command 'console'");
}

static __exit void console_cmd_exit(void)
{
	if(command_unregister(&console_cmd))
		LOG("Unegister command 'console'");
	else
		LOG("Failed to unregister command 'console'");
}

command_initcall(console_cmd_init);
command_exitcall(console_cmd_exit);

#endif