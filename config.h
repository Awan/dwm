/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappx     = 11;       /* gaps between tags */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char *fonts[]          = {"-wuncon-siji-medium-r-normal--10-100-75-75-c-80-iso10646-1","-*-tamsyn-medium-r-normal-*-12-*-*-*-*-*-*-1"};
static const char dmenufont[]       = { '-*-tamsyn-medium-r-normal-*-12-*-*-*-*-*-*-1','-wuncon-siji-medium-r-normal--10-100-75-75-c-80-iso10646-1' };
static const char col_gray1[]       = "#24292A";
static const char col_gray2[]       = "#ff00ff";
static const char col_gray3[]       = "#ccd2be";
static const char col_gray4[]       = "#ff0000";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "mpv",      NULL,       NULL,       0,            1,           -1 },
	{ "mpv",     "webcam",  "webcam",    0,            1,           -1 },
	{ "Google-chrome",     NULL,  "Facebook Call",    0,            1,           -1 },
	{ "feh",      NULL,       NULL,       0,            1,           -1 },
	{ "sxiv",      NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

#include "layouts.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
    { "|M|",      centeredmaster },
    { ">M>",      centeredfloatingmaster },
	{ "HHH",      grid },
};

/* key definitions */
#define MODKEY Mod4Mask
#define XF86AudioLowerVolume  0x1008ff11
#define XF86AudioRaiseVolume  0x1008ff13
#define XF86AudioMute         0x1008ff12
#define XF86MonBrightnessDown 0x1008ff03
#define XF86MonBrightnessUp   0x1008ff02
#define Insert                0xff63
#define XF86LaunchA           0x1008ff4a
#define XF86Back              0x1008ff26
#define XF86Forward           0x1008ff27
#define XF86Reload            0x1008ff73
#define XF86HomePage          0x1008ff18
#define XF86Search            0x1008ff1b
#define XF86Mail              0x1008ff19
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "urxvtc", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "urxvtc", "-geometry", "75x20", "-T", scratchpadname,  NULL };
static const char *brup[] = { "xbacklight", "+5",  NULL };
static const char *brdown[] = { "xbacklight", "-5",  NULL };
static const char *volup[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "+1000", NULL };
static const char *voldown[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "-1000", NULL };
static const char *voltoggle[] = { "pactl", "set-sink-mute", "@DEFAULT_SINK@", "toggle", NULL };
static const char *lockcmd[] = { "slock", NULL };
static const char *clipshow[] = { "clipmenu", "-i", "-fn", dmenufont, NULL };
static const char *clippaste[] = { "paste4dwm", NULL };
static const char *music[] = { "mpc", "toggle", NULL };
static const char *seekbackward[] =  { "mpc", "seek", "-100", NULL };
static const char *seekforward[] = { "mpc", "seek", "+100", NULL };
static const char *mail[] = { "urxvtc", "-e", "mutt", NULL};
static const char *tmenu[] = { "urxvtc", "-e", "tmux", "attach", NULL };

#include "focusurgent.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      incnmaster,          {.i = -1 } },
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_u,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_y,      focusurgent,    {0} },
	{ MODKEY,                       XK_i,      togglescratch,     {.v = scratchpadcmd } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_semicolon,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
  { MODKEY,                       XK_n,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[4]} },
  { MODKEY,                       XK_o,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_s,      togglesticky,   {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
  { MODKEY|ShiftMask,             XK_t,    spawn,          {.v = tmenu } }, 
  { MODKEY,                       XK_x,    spawn,          {.v = lockcmd } },
  { 0,                            XF86MonBrightnessDown, spawn,  {.v = brdown } },
  { 0,                            XF86MonBrightnessUp, spawn,  {.v = brup } },
  { 0,                            XF86AudioLowerVolume, spawn, {.v = voldown } },
  { 0,                            XF86AudioRaiseVolume, spawn, {.v = volup } },
  { 0,                            XF86AudioMute, spawn,      {.v = voltoggle } },
  { 0,                            Insert,    spawn,          {.v = clippaste } },
  { 0,                            XF86LaunchA, spawn, {.v = music } },
  { 0,                            XF86Forward, spawn, {.v = seekforward } },
  { 0,                            XF86Back, spawn,  {.v = seekbackward } },
  { 0,                            XF86Mail, spawn,  {.v = mail } },
	{ MODKEY,                       XK_e,      spawn,       {.v = clipshow } },
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
static Button buttons[] = {
	/* click                event mask      button          function        argument */
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

