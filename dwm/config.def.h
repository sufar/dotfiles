/* See LICENSE file for copyright and license details. */
#include "gaplessgrid.c"
/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
#define ICONSIZE 16   /* icon size */
#define ICONSPACING 5 /* space between icon and title */
// static const char *fonts[]          = { "monospace:size=18",
static const char *fonts[]          = { "Source Code Pro:size=12",
					"WenQuanYi Micro Hei:size=12:type=Regular:antialias=true:autohit=true",
					"Symbols Nerd Font:pixelsize=16:type=2048-em:antialias=true:autohit=true"};
static const char dmenufont[]       = "monospace:size=14";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";


/* colors catppuccin-frappe */
static const char rosewater[]               = "#f2d5cf";
static const char flamingo[]                = "#eebebe";
static const char pink[]                    = "#f4b8e4";
static const char mauve[]                   = "#ca9ee6";
static const char red[]                     = "#e78284";
static const char maroon[]                  = "#ea999c";
static const char peach[]                   = "#ef9f76";
static const char yellow[]                  = "#e5c890";
static const char green[]                   = "#a6d189";
static const char teal[]                    = "#81c8be";
static const char sky[]                     = "#99d1db";
static const char sapphire[]                = "#85c1dc";
static const char blue[]                    = "#8caaee";
static const char lavender[]                = "#babbf1";
static const char text[]                    = "#c6d0f5";
static const char subtext1[]                = "#b5bfe2";
static const char subtext0[]                = "#a5adce";
static const char overlay2[]                = "#949cbb";
static const char overlay1[]                = "#838ba7";
static const char overlay0[]                = "#737994";
static const char surface2[]                = "#626880";
static const char surface1[]                = "#51576d";
static const char surface0[]                = "#414559";
static const char base[]                    = "#303446";
static const char mantle[]                  = "#292c3c";
static const char crust[]                   = "#232634";


static const char *colors[][3] = {
    /*                    fg     bg      border */
    [SchemeNorm]      = { text, base,     mantle   },
    [SchemeSel]       = { text, surface0, subtext0 },
    [SchemeStatus]    = { text, base,     base     }, // Statusbar right {text,background,not used but cannot be empty}
    [SchemeTagsSel]   = { base, lavender, base     }, // Tagbar left selected {text,background,not used but cannot be empty}
    [SchemeTagsNorm]  = { text, base,     base     }, // Tagbar left unselected {text,background,not used but cannot be empty}
    [SchemeInfoSel]   = { text, base,     base     }, // infobar middle  selected {text,background,not used but cannot be empty}
    [SchemeInfoNorm]  = { text, base,     base     }, // infobar middle  unselected {text,background,not used but cannot be empty}
};

/* tagging */
static const char *tags[] = { "", "", "", "﬏", "", "", "", "", "" };
static const char *defaulttagapps[] = { "alacritty", "google-chrome-stable", "intellij-idea-ultimate-edition", "code", "VirtualBox", "obsidian", "wps", "dbeaver", "netease-cloud-music" };

/* Lockfile */
static char lockfile[] = "/tmp/dwm.lock";

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
  { "###",      gaplessgrid },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
//static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
//static const char *dmenucmd[] = { "rofi", "-show", "drun", NULL };
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "rofi", "-no-lazy-grab","-show", "drun", "-modi", "drun", "-show-icons", NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *volup[]  = { "/home/zugle/.config/dwm/scripts/volup.sh", NULL };
static const char *voldown[]  = { "/home/zugle/.config/dwm/scripts/voldown.sh", NULL };
static const char *voltoggle[]  = { "/home/zugle/.config/dwm/scripts/voltoggle.sh", NULL };
static const char *mictoggle[]  = { "/home/zugle/.config/dwm/scripts/micmute.sh", NULL };
static const char *lightup[]  = { "/home/zugle/.config/dwm/scripts/lightup.sh", NULL };
static const char *lightdown[]  = { "/home/zugle/.config/dwm/scripts/lightdown.sh", NULL };
static const char *toggletouchpad[]  = { "/home/zugle/.config/dwm/scripts/toggle-touchpad.sh", NULL };
static const char *lockscreen[]  = { "/home/zugle/.config/dwm/scripts/lockscreen.sh", NULL };
static const char *screenshot[]  = { "flameshot", "gui", NULL };
static const char scratchpadname[] = "scratchpad";
// 120*34
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "160x52", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ 0,                       	0x1008FF11,     spawn,         {.v = voldown } },
	{ 0,                       	0x1008FF13,     spawn,         {.v = volup } },
	{ 0,                       	0x1008FF12,     spawn,         {.v = voltoggle } },
	// { 0,                       	0x1008ffb2,     spawn,          {.v = mictoggle } },
	{ 0,                            0x1008FF03,     spawn,     {.v = lightdown } },
	{ 0,                            0x1008FF02,     spawn,     {.v = lightup } },
	{ 0,                            0x1008ff58,     spawn,     {.v = screenshot } },
	{ 0,                            0x1008ffa9,     spawn,     {.v = toggletouchpad } },
  { MODKEY|ShiftMask,             XK_l,      spawn,          {.v = lockscreen }  },
	{ MODKEY|ShiftMask,             XK_a,      spawn,          {.v = screenshot } },
 	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_grave,  togglescratch,  {.v = scratchpadcmd } },
  { MODKEY,                       XK_g,      setlayout,      {.v = &layouts[3] } },
	{ MODKEY,                       XK_s,      spawndefault,   {0} },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_j,      aspectresize,   {.i = +24} },
	{ MODKEY|ShiftMask,             XK_k,      aspectresize,   {.i = -24} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_f,      fullscreen,     {0} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglealwaysontop, {0} },
	// { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_x,      movecenter,     {0} },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
  { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },

	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
