/* See LICENSE file for copyright and license details. */

/* interval between updates (in ms) */
const unsigned int interval = 1000;

/* text to show if no value can be retrieved */
static const char unknown_str[] = "n/a";

/* maximum output string length */
#define MAXLEN 2048

/*
 * function            description                     argument (example)
 *
 * battery_perc        battery percentage              battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * battery_state       battery charging state          battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * battery_remaining   battery remaining HH:MM         battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * cpu_perc            cpu usage in percent            NULL
 * cpu_freq            cpu frequency in MHz            NULL
 * datetime            date and time                   format string (%F %T)
 * disk_free           free disk space in GB           mountpoint path (/)
 * disk_perc           disk usage in percent           mountpoint path (/)
 * disk_total          total disk space in GB          mountpoint path (/")
 * disk_used           used disk space in GB           mountpoint path (/)
 * entropy             available entropy               NULL
 * gid                 GID of current user             NULL
 * hostname            hostname                        NULL
 * ipv4                IPv4 address                    interface name (eth0)
 * ipv6                IPv6 address                    interface name (eth0)
 * kernel_release      `uname -r`                      NULL
 * keyboard_indicators caps/num lock indicators        format string (c?n?)
 *                                                     see keyboard_indicators.c
 * keymap              layout (variant) of current     NULL
 *                     keymap
 * load_avg            load average                    NULL
 * netspeed_rx         receive network speed           interface name (wlan0)
 * netspeed_tx         transfer network speed          interface name (wlan0)
 * num_files           number of files in a directory  path
 *                                                     (/home/foo/Inbox/cur)
 * ram_free            free memory in GB               NULL
 * ram_perc            memory usage in percent         NULL
 * ram_total           total memory size in GB         NULL
 * ram_used            used memory in GB               NULL
 * run_command         custom shell command            command (echo foo)
 * separator           string to echo                  NULL
 * swap_free           free swap in GB                 NULL
 * swap_perc           swap usage in percent           NULL
 * swap_total          total swap size in GB           NULL
 * swap_used           used swap in GB                 NULL
 * temp                temperature in degree celsius   sensor file
 *                                                     (/sys/class/thermal/...)
 *                                                     NULL on OpenBSD
 *                                                     thermal zone on FreeBSD
 *                                                     (tz0, tz1, etc.)
 * uid                 UID of current user             NULL
 * uptime              system uptime                   NULL
 * username            username of current user        NULL
 * vol_perc            OSS/ALSA volume in percent      mixer file (/dev/mixer)
 *                                                     NULL on OpenBSD
 * wifi_perc           WiFi signal in percent          interface name (wlan0)
 * wifi_essid          WiFi ESSID                      interface name (wlan0)
 */
static const struct arg args[] = {
	/* function format          argument */
	{ cpu_perc, "^b#a6d189^^c#414559^  ^b#303446^^c#98c379^ %s%% ", NULL},
	{ ram_used, "^b#81c8be^^c#303446^  ^b#303446^^c#81c8be^ %s ", NULL},
	{ load_avg, "^b#dd7878^^c#303446^  ^b#303446^^c#ef9f76^ %s ", NULL},
	// { temp, "^c#303446^^b#81c8be^%s°C|", "/sys/class/thermal/thermal_zone0/temp"},
	// { run_command, "ﯦ%s%%|","light -G | awk -F '.' '{ print $1 }'"},
	// { run_command, "ﯦ%s%%|","light -G | awk -F '.' '{ print ^c#ef9f76^$1 }'"},
	{ run_command, "^b#8caaee^^c#414559^  ^b#303446^^c#c6d0f5^ %s%% ", "pactl list sinks | grep $(pactl info | grep 'Default Sink' | awk '{print $3}') -A 7 | sed -n '8p' | awk '{printf int($5)}'"},
	//{ run_command, "%s|", "amixer sget Master | awk -F '[][]' '/Left:/ { print $2 }'"},
	// { run_command, "%s|", "amixer sget Capture | awk -F '[][]' '/Left:/ { print $2 }'"},
	{ battery_perc,	"%s%|", "BAT0"},
	{ datetime, "^b#f4b8e4^^c#303446^  ^b#303446^^c#f4b8e4^ %s",  "%m-%d %T" },
};
