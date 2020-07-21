/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h> // for media keys

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10", "Font Awesome 5 Free:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_bg[]          = "#282A36";
static const char col_gray[]        = "#44475A";
static const char col_text[]        = "#F8F8F2";
static const char col_hl[]          = "#FF79C6";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm]     = { col_text, col_bg, col_gray },
	[SchemeSel]      = { col_gray, col_hl,  col_hl  },
    /* colorbar border unused but cannot be empty */
   	[SchemeStatus]   = { col_text, col_bg, "#000000" }, // Statusbar right
	[SchemeTagsSel]  = { col_bg, col_hl, "#000000" }, // Tagbar left selected
    [SchemeTagsNorm] = { col_text, col_bg, "#000000" }, // Tagbar left unselected
    [SchemeInfoSel]  = { col_text, col_bg, "#000000" }, // infobar middle selected
    [SchemeInfoNorm] = { col_text, col_bg, "#000000" }, // infobar middle unselected
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class          instance    title         tags mask     iscentered   isfloating   monitor */
	{ "Gimp",         NULL,       NULL,         0,            0,           1,           -1 },
	{ "firefox",      NULL,       NULL,         0,            0,           0,           -1 },
	{ "spotify",      NULL,       NULL,         1 << 8,       0,           0,           -1 },
	{ "Alacritty",    NULL,       "pulsemixer", 0,            1,           1,           -1 },
	{ "Lxappearance", NULL,       NULL,         0,            1,           1,           -1 },
	{ "org.remmina.Remmina", NULL, "Remmina Remote Desktop Client", 0, 1,  1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "[D]",      deck },
};

/* key definitions */
#define MODKEY Mod4Mask

#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "alacritty", "-t", scratchpadname, "-d", "120", "34", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
    // Hotkeys
	{ MODKEY,                       XK_p,         spawn,          {.v = dmenucmd} },
	{ MODKEY,                       XK_Return,    spawn,          {.v = termcmd} },
	{ MODKEY,                       XK_w,         spawn,          SHCMD("$BROWSER") },
	{ MODKEY,                       XK_x,         spawn,          SHCMD("lockscreen") },
	{ MODKEY|ShiftMask,             XK_x,         spawn,          SHCMD("sessionopt") },
	{ MODKEY,                       XK_grave,     spawn,          SHCMD("list-icons") },
	{ MODKEY|ShiftMask,             XK_grave,     spawn,          SHCMD("list-colors") },
    // Media Keys
	{ 0,         XF86XK_AudioMute,                spawn,          SHCMD("pulsemixer --toggle-mute; kill -36 $(pidof goblocks)") },
	{ 0,         XF86XK_AudioRaiseVolume,         spawn,          SHCMD("pulsemixer --change-volume +10; kill -36 $(pidof goblocks)") },
	{ ShiftMask, XF86XK_AudioRaiseVolume,         spawn,          SHCMD("pulsemixer --change-volume +1; kill -36 $(pidof goblocks)") },
	{ 0,         XF86XK_AudioLowerVolume,         spawn,          SHCMD("pulsemixer --change-volume -10; kill -36 $(pidof goblocks)") },
	{ ShiftMask, XF86XK_AudioLowerVolume,         spawn,          SHCMD("pulsemixer --change-volume -1; kill -36 $(pidof goblocks)") },
	{ 0,         XF86XK_AudioPlay,                spawn,          SHCMD("playerctl --player=spotify play-pause") },
	{ 0,         XF86XK_AudioPrev,                spawn,          SHCMD("playerctl --player=spotify previous") },
	{ 0,         XF86XK_AudioNext,                spawn,          SHCMD("playerctl --player=spotify next") },
    //{ 0,       XF86XK_Calculator,               spawn,          SHCMD() },
    //{ 0,       XF86XK_Mail,                     spawn,          SHCMD() },
    //{ 0,       XF86XK_HomePage,                 spawn,          SHCMD() },
    //{ 0,       XF86XK_Tools,                    spawn,          SHCMD() },
    //{ 0,       XF86XK_Search,                   spawn,          SHCMD() },
    // DWM commands
	{ MODKEY,                       XK_q,         killclient,     {0} },
	{ MODKEY,                       XK_s,         togglescratch,  {.v = scratchpadcmd} },
	{ MODKEY,                       XK_b,         togglebar,      {0} },
	{ MODKEY,                       XK_comma,     incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_period,    incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,         shiftview,      {.i = -1 } },
	{ MODKEY,                       XK_l,         shiftview,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_h,         setmfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_l,         setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_space,     zoom,           {0} }, // make master
	{ MODKEY,                       XK_Tab,       view,           {0} }, // last tag
	{ MODKEY,                       XK_t,         setlayout,      {.v = &layouts[0]} }, // tile
	{ MODKEY,                       XK_y,         setlayout,      {.v = &layouts[1]} }, // floating
	{ MODKEY,                       XK_m,         setlayout,      {.v = &layouts[2]} }, // monocle
	{ MODKEY,                       XK_u,         setlayout,      {.v = &layouts[3]} }, // deck
	{ MODKEY|ShiftMask,             XK_space,     setlayout,      {0} }, // last layout
	{ MODKEY|ShiftMask,             XK_y,         togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_m,         togglefullscr,  {0} },
	{ MODKEY,                       XK_0,         view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,         tag,            {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_comma,     focusmon,       {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,    focusmon,       {.i = +1 } },
	{ MODKEY|ControlMask|ShiftMask, XK_comma,     tagmon,         {.i = -1 } },
	{ MODKEY|ControlMask|ShiftMask, XK_period,    tagmon,         {.i = +1 } },
	STACKKEYS(                      MODKEY,                       focus)
	STACKKEYS(                      MODKEY|ShiftMask,             push)
	TAGKEYS(                        XK_1,                         0)
	TAGKEYS(                        XK_2,                         1)
	TAGKEYS(                        XK_3,                         2)
	TAGKEYS(                        XK_4,                         3)
	TAGKEYS(                        XK_5,                         4)
	TAGKEYS(                        XK_6,                         5)
	TAGKEYS(                        XK_7,                         6)
	TAGKEYS(                        XK_8,                         7)
	TAGKEYS(                        XK_9,                         8)
	{ MODKEY|ShiftMask,             XK_q,         quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_q,         quit,           {1} },
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

