/* SelEvent.c
 *
 * This file contains Events stuffs
 *
 * 24/04/2017 LF : First version
 */

#include "selene.h"
#include "SelEvent.h"
#include "SelShared.h"

#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include <linux/input.h>

/* grep KEY_ /usr/include/linux/input-event-codes.h |  awk '$2 !~ /KEY_F\(n\)/ { gsub("KEY_","",$2); print "\t{\""$2"\", KEY_"$2"}," }' */
static const struct ConstTranscode _evtKeys[] = {
	{"RESERVED", KEY_RESERVED},
	{"ESC", KEY_ESC},
	{"1", KEY_1},
	{"2", KEY_2},
	{"3", KEY_3},
	{"4", KEY_4},
	{"5", KEY_5},
	{"6", KEY_6},
	{"7", KEY_7},
	{"8", KEY_8},
	{"9", KEY_9},
	{"0", KEY_0},
	{"MINUS", KEY_MINUS},
	{"EQUAL", KEY_EQUAL},
	{"BACKSPACE", KEY_BACKSPACE},
	{"TAB", KEY_TAB},
	{"Q", KEY_Q},
	{"W", KEY_W},
	{"E", KEY_E},
	{"R", KEY_R},
	{"T", KEY_T},
	{"Y", KEY_Y},
	{"U", KEY_U},
	{"I", KEY_I},
	{"O", KEY_O},
	{"P", KEY_P},
	{"LEFTBRACE", KEY_LEFTBRACE},
	{"RIGHTBRACE", KEY_RIGHTBRACE},
	{"ENTER", KEY_ENTER},
	{"LEFTCTRL", KEY_LEFTCTRL},
	{"A", KEY_A},
	{"S", KEY_S},
	{"D", KEY_D},
	{"F", KEY_F},
	{"G", KEY_G},
	{"H", KEY_H},
	{"J", KEY_J},
	{"K", KEY_K},
	{"L", KEY_L},
	{"SEMICOLON", KEY_SEMICOLON},
	{"APOSTROPHE", KEY_APOSTROPHE},
	{"GRAVE", KEY_GRAVE},
	{"LEFTSHIFT", KEY_LEFTSHIFT},
	{"BACKSLASH", KEY_BACKSLASH},
	{"Z", KEY_Z},
	{"X", KEY_X},
	{"C", KEY_C},
	{"V", KEY_V},
	{"B", KEY_B},
	{"N", KEY_N},
	{"M", KEY_M},
	{"COMMA", KEY_COMMA},
	{"DOT", KEY_DOT},
	{"SLASH", KEY_SLASH},
	{"RIGHTSHIFT", KEY_RIGHTSHIFT},
	{"KPASTERISK", KEY_KPASTERISK},
	{"LEFTALT", KEY_LEFTALT},
	{"SPACE", KEY_SPACE},
	{"CAPSLOCK", KEY_CAPSLOCK},
	{"F1", KEY_F1},
	{"F2", KEY_F2},
	{"F3", KEY_F3},
	{"F4", KEY_F4},
	{"F5", KEY_F5},
	{"F6", KEY_F6},
	{"F7", KEY_F7},
	{"F8", KEY_F8},
	{"F9", KEY_F9},
	{"F10", KEY_F10},
	{"NUMLOCK", KEY_NUMLOCK},
	{"SCROLLLOCK", KEY_SCROLLLOCK},
	{"KP7", KEY_KP7},
	{"KP8", KEY_KP8},
	{"KP9", KEY_KP9},
	{"KPMINUS", KEY_KPMINUS},
	{"KP4", KEY_KP4},
	{"KP5", KEY_KP5},
	{"KP6", KEY_KP6},
	{"KPPLUS", KEY_KPPLUS},
	{"KP1", KEY_KP1},
	{"KP2", KEY_KP2},
	{"KP3", KEY_KP3},
	{"KP0", KEY_KP0},
	{"KPDOT", KEY_KPDOT},
	{"ZENKAKUHANKAKU", KEY_ZENKAKUHANKAKU},
	{"102ND", KEY_102ND},
	{"F11", KEY_F11},
	{"F12", KEY_F12},
	{"RO", KEY_RO},
	{"KATAKANA", KEY_KATAKANA},
	{"HIRAGANA", KEY_HIRAGANA},
	{"HENKAN", KEY_HENKAN},
	{"KATAKANAHIRAGANA", KEY_KATAKANAHIRAGANA},
	{"MUHENKAN", KEY_MUHENKAN},
	{"KPJPCOMMA", KEY_KPJPCOMMA},
	{"KPENTER", KEY_KPENTER},
	{"RIGHTCTRL", KEY_RIGHTCTRL},
	{"KPSLASH", KEY_KPSLASH},
	{"SYSRQ", KEY_SYSRQ},
	{"RIGHTALT", KEY_RIGHTALT},
	{"LINEFEED", KEY_LINEFEED},
	{"HOME", KEY_HOME},
	{"UP", KEY_UP},
	{"PAGEUP", KEY_PAGEUP},
	{"LEFT", KEY_LEFT},
	{"RIGHT", KEY_RIGHT},
	{"END", KEY_END},
	{"DOWN", KEY_DOWN},
	{"PAGEDOWN", KEY_PAGEDOWN},
	{"INSERT", KEY_INSERT},
	{"DELETE", KEY_DELETE},
	{"MACRO", KEY_MACRO},
	{"MUTE", KEY_MUTE},
	{"VOLUMEDOWN", KEY_VOLUMEDOWN},
	{"VOLUMEUP", KEY_VOLUMEUP},
	{"POWER", KEY_POWER},
	{"KPEQUAL", KEY_KPEQUAL},
	{"KPPLUSMINUS", KEY_KPPLUSMINUS},
	{"PAUSE", KEY_PAUSE},
	{"SCALE", KEY_SCALE},
	{"KPCOMMA", KEY_KPCOMMA},
	{"HANGEUL", KEY_HANGEUL},
	{"HANGUEL", KEY_HANGUEL},
	{"HANJA", KEY_HANJA},
	{"YEN", KEY_YEN},
	{"LEFTMETA", KEY_LEFTMETA},
	{"RIGHTMETA", KEY_RIGHTMETA},
	{"COMPOSE", KEY_COMPOSE},
	{"STOP", KEY_STOP},
	{"AGAIN", KEY_AGAIN},
	{"PROPS", KEY_PROPS},
	{"UNDO", KEY_UNDO},
	{"FRONT", KEY_FRONT},
	{"COPY", KEY_COPY},
	{"OPEN", KEY_OPEN},
	{"PASTE", KEY_PASTE},
	{"FIND", KEY_FIND},
	{"CUT", KEY_CUT},
	{"HELP", KEY_HELP},
	{"MENU", KEY_MENU},
	{"CALC", KEY_CALC},
	{"SETUP", KEY_SETUP},
	{"SLEEP", KEY_SLEEP},
	{"WAKEUP", KEY_WAKEUP},
	{"FILE", KEY_FILE},
	{"SENDFILE", KEY_SENDFILE},
	{"DELETEFILE", KEY_DELETEFILE},
	{"XFER", KEY_XFER},
	{"PROG1", KEY_PROG1},
	{"PROG2", KEY_PROG2},
	{"WWW", KEY_WWW},
	{"MSDOS", KEY_MSDOS},
	{"COFFEE", KEY_COFFEE},
	{"SCREENLOCK", KEY_SCREENLOCK},
/*	{"ROTATE_DISPLAY", KEY_ROTATE_DISPLAY}, */
	{"DIRECTION", KEY_DIRECTION},
	{"CYCLEWINDOWS", KEY_CYCLEWINDOWS},
	{"MAIL", KEY_MAIL},
	{"BOOKMARKS", KEY_BOOKMARKS},
	{"COMPUTER", KEY_COMPUTER},
	{"BACK", KEY_BACK},
	{"FORWARD", KEY_FORWARD},
	{"CLOSECD", KEY_CLOSECD},
	{"EJECTCD", KEY_EJECTCD},
	{"EJECTCLOSECD", KEY_EJECTCLOSECD},
	{"NEXTSONG", KEY_NEXTSONG},
	{"PLAYPAUSE", KEY_PLAYPAUSE},
	{"PREVIOUSSONG", KEY_PREVIOUSSONG},
	{"STOPCD", KEY_STOPCD},
	{"RECORD", KEY_RECORD},
	{"REWIND", KEY_REWIND},
	{"PHONE", KEY_PHONE},
	{"ISO", KEY_ISO},
	{"CONFIG", KEY_CONFIG},
	{"HOMEPAGE", KEY_HOMEPAGE},
	{"REFRESH", KEY_REFRESH},
	{"EXIT", KEY_EXIT},
	{"MOVE", KEY_MOVE},
	{"EDIT", KEY_EDIT},
	{"SCROLLUP", KEY_SCROLLUP},
	{"SCROLLDOWN", KEY_SCROLLDOWN},
	{"KPLEFTPAREN", KEY_KPLEFTPAREN},
	{"KPRIGHTPAREN", KEY_KPRIGHTPAREN},
	{"NEW", KEY_NEW},
	{"REDO", KEY_REDO},
	{"F13", KEY_F13},
	{"F14", KEY_F14},
	{"F15", KEY_F15},
	{"F16", KEY_F16},
	{"F17", KEY_F17},
	{"F18", KEY_F18},
	{"F19", KEY_F19},
	{"F20", KEY_F20},
	{"F21", KEY_F21},
	{"F22", KEY_F22},
	{"F23", KEY_F23},
	{"F24", KEY_F24},
	{"PLAYCD", KEY_PLAYCD},
	{"PAUSECD", KEY_PAUSECD},
	{"PROG3", KEY_PROG3},
	{"PROG4", KEY_PROG4},
	{"DASHBOARD", KEY_DASHBOARD},
	{"SUSPEND", KEY_SUSPEND},
	{"CLOSE", KEY_CLOSE},
	{"PLAY", KEY_PLAY},
	{"FASTFORWARD", KEY_FASTFORWARD},
	{"BASSBOOST", KEY_BASSBOOST},
	{"PRINT", KEY_PRINT},
	{"HP", KEY_HP},
	{"CAMERA", KEY_CAMERA},
	{"SOUND", KEY_SOUND},
	{"QUESTION", KEY_QUESTION},
	{"EMAIL", KEY_EMAIL},
	{"CHAT", KEY_CHAT},
	{"SEARCH", KEY_SEARCH},
	{"CONNECT", KEY_CONNECT},
	{"FINANCE", KEY_FINANCE},
	{"SPORT", KEY_SPORT},
	{"SHOP", KEY_SHOP},
	{"ALTERASE", KEY_ALTERASE},
	{"CANCEL", KEY_CANCEL},
	{"BRIGHTNESSDOWN", KEY_BRIGHTNESSDOWN},
	{"BRIGHTNESSUP", KEY_BRIGHTNESSUP},
	{"MEDIA", KEY_MEDIA},
	{"SWITCHVIDEOMODE", KEY_SWITCHVIDEOMODE},
	{"KBDILLUMTOGGLE", KEY_KBDILLUMTOGGLE},
	{"KBDILLUMDOWN", KEY_KBDILLUMDOWN},
	{"KBDILLUMUP", KEY_KBDILLUMUP},
	{"SEND", KEY_SEND},
	{"REPLY", KEY_REPLY},
	{"FORWARDMAIL", KEY_FORWARDMAIL},
	{"SAVE", KEY_SAVE},
	{"DOCUMENTS", KEY_DOCUMENTS},
	{"BATTERY", KEY_BATTERY},
	{"BLUETOOTH", KEY_BLUETOOTH},
	{"WLAN", KEY_WLAN},
	{"UWB", KEY_UWB},
	{"UNKNOWN", KEY_UNKNOWN},
	{"VIDEO_NEXT", KEY_VIDEO_NEXT},
	{"VIDEO_PREV", KEY_VIDEO_PREV},
	{"BRIGHTNESS_CYCLE", KEY_BRIGHTNESS_CYCLE},
	{"BRIGHTNESS_AUTO", KEY_BRIGHTNESS_AUTO},
	{"BRIGHTNESS_ZERO", KEY_BRIGHTNESS_ZERO},
	{"DISPLAY_OFF", KEY_DISPLAY_OFF},
	{"WWAN", KEY_WWAN},
	{"WIMAX", KEY_WIMAX},
	{"RFKILL", KEY_RFKILL},
	{"MICMUTE", KEY_MICMUTE},
	{"OK", KEY_OK},
	{"SELECT", KEY_SELECT},
	{"GOTO", KEY_GOTO},
	{"CLEAR", KEY_CLEAR},
	{"POWER2", KEY_POWER2},
	{"OPTION", KEY_OPTION},
	{"INFO", KEY_INFO},
	{"TIME", KEY_TIME},
	{"VENDOR", KEY_VENDOR},
	{"ARCHIVE", KEY_ARCHIVE},
	{"PROGRAM", KEY_PROGRAM},
	{"CHANNEL", KEY_CHANNEL},
	{"FAVORITES", KEY_FAVORITES},
	{"EPG", KEY_EPG},
	{"PVR", KEY_PVR},
	{"MHP", KEY_MHP},
	{"LANGUAGE", KEY_LANGUAGE},
	{"TITLE", KEY_TITLE},
	{"SUBTITLE", KEY_SUBTITLE},
	{"ANGLE", KEY_ANGLE},
	{"ZOOM", KEY_ZOOM},
	{"MODE", KEY_MODE},
	{"KEYBOARD", KEY_KEYBOARD},
	{"SCREEN", KEY_SCREEN},
	{"PC", KEY_PC},
	{"TV", KEY_TV},
	{"TV2", KEY_TV2},
	{"VCR", KEY_VCR},
	{"VCR2", KEY_VCR2},
	{"SAT", KEY_SAT},
	{"SAT2", KEY_SAT2},
	{"CD", KEY_CD},
	{"TAPE", KEY_TAPE},
	{"RADIO", KEY_RADIO},
	{"TUNER", KEY_TUNER},
	{"PLAYER", KEY_PLAYER},
	{"TEXT", KEY_TEXT},
	{"DVD", KEY_DVD},
	{"AUX", KEY_AUX},
	{"MP3", KEY_MP3},
	{"AUDIO", KEY_AUDIO},
	{"VIDEO", KEY_VIDEO},
	{"DIRECTORY", KEY_DIRECTORY},
	{"LIST", KEY_LIST},
	{"MEMO", KEY_MEMO},
	{"CALENDAR", KEY_CALENDAR},
	{"RED", KEY_RED},
	{"GREEN", KEY_GREEN},
	{"YELLOW", KEY_YELLOW},
	{"BLUE", KEY_BLUE},
	{"CHANNELUP", KEY_CHANNELUP},
	{"CHANNELDOWN", KEY_CHANNELDOWN},
	{"FIRST", KEY_FIRST},
	{"LAST", KEY_LAST},
	{"AB", KEY_AB},
	{"NEXT", KEY_NEXT},
	{"RESTART", KEY_RESTART},
	{"SLOW", KEY_SLOW},
	{"SHUFFLE", KEY_SHUFFLE},
	{"BREAK", KEY_BREAK},
	{"PREVIOUS", KEY_PREVIOUS},
	{"DIGITS", KEY_DIGITS},
	{"TEEN", KEY_TEEN},
	{"TWEN", KEY_TWEN},
	{"VIDEOPHONE", KEY_VIDEOPHONE},
	{"GAMES", KEY_GAMES},
	{"ZOOMIN", KEY_ZOOMIN},
	{"ZOOMOUT", KEY_ZOOMOUT},
	{"ZOOMRESET", KEY_ZOOMRESET},
	{"WORDPROCESSOR", KEY_WORDPROCESSOR},
	{"EDITOR", KEY_EDITOR},
	{"SPREADSHEET", KEY_SPREADSHEET},
	{"GRAPHICSEDITOR", KEY_GRAPHICSEDITOR},
	{"PRESENTATION", KEY_PRESENTATION},
	{"DATABASE", KEY_DATABASE},
	{"NEWS", KEY_NEWS},
	{"VOICEMAIL", KEY_VOICEMAIL},
	{"ADDRESSBOOK", KEY_ADDRESSBOOK},
	{"MESSENGER", KEY_MESSENGER},
	{"DISPLAYTOGGLE", KEY_DISPLAYTOGGLE},
	{"BRIGHTNESS_TOGGLE", KEY_BRIGHTNESS_TOGGLE},
	{"SPELLCHECK", KEY_SPELLCHECK},
	{"LOGOFF", KEY_LOGOFF},
	{"DOLLAR", KEY_DOLLAR},
	{"EURO", KEY_EURO},
	{"FRAMEBACK", KEY_FRAMEBACK},
	{"FRAMEFORWARD", KEY_FRAMEFORWARD},
	{"CONTEXT_MENU", KEY_CONTEXT_MENU},
	{"MEDIA_REPEAT", KEY_MEDIA_REPEAT},
	{"10CHANNELSUP", KEY_10CHANNELSUP},
	{"10CHANNELSDOWN", KEY_10CHANNELSDOWN},
	{"IMAGES", KEY_IMAGES},
	{"DEL_EOL", KEY_DEL_EOL},
	{"DEL_EOS", KEY_DEL_EOS},
	{"INS_LINE", KEY_INS_LINE},
	{"DEL_LINE", KEY_DEL_LINE},
	{"FN", KEY_FN},
	{"FN_ESC", KEY_FN_ESC},
	{"FN_F1", KEY_FN_F1},
	{"FN_F2", KEY_FN_F2},
	{"FN_F3", KEY_FN_F3},
	{"FN_F4", KEY_FN_F4},
	{"FN_F5", KEY_FN_F5},
	{"FN_F6", KEY_FN_F6},
	{"FN_F7", KEY_FN_F7},
	{"FN_F8", KEY_FN_F8},
	{"FN_F9", KEY_FN_F9},
	{"FN_F10", KEY_FN_F10},
	{"FN_F11", KEY_FN_F11},
	{"FN_F12", KEY_FN_F12},
	{"FN_1", KEY_FN_1},
	{"FN_2", KEY_FN_2},
	{"FN_D", KEY_FN_D},
	{"FN_E", KEY_FN_E},
	{"FN_F", KEY_FN_F},
	{"FN_S", KEY_FN_S},
	{"FN_B", KEY_FN_B},
	{"BRL_DOT1", KEY_BRL_DOT1},
	{"BRL_DOT2", KEY_BRL_DOT2},
	{"BRL_DOT3", KEY_BRL_DOT3},
	{"BRL_DOT4", KEY_BRL_DOT4},
	{"BRL_DOT5", KEY_BRL_DOT5},
	{"BRL_DOT6", KEY_BRL_DOT6},
	{"BRL_DOT7", KEY_BRL_DOT7},
	{"BRL_DOT8", KEY_BRL_DOT8},
	{"BRL_DOT9", KEY_BRL_DOT9},
	{"BRL_DOT10", KEY_BRL_DOT10},
	{"NUMERIC_0", KEY_NUMERIC_0},
	{"NUMERIC_1", KEY_NUMERIC_1},
	{"NUMERIC_2", KEY_NUMERIC_2},
	{"NUMERIC_3", KEY_NUMERIC_3},
	{"NUMERIC_4", KEY_NUMERIC_4},
	{"NUMERIC_5", KEY_NUMERIC_5},
	{"NUMERIC_6", KEY_NUMERIC_6},
	{"NUMERIC_7", KEY_NUMERIC_7},
	{"NUMERIC_8", KEY_NUMERIC_8},
	{"NUMERIC_9", KEY_NUMERIC_9},
	{"NUMERIC_STAR", KEY_NUMERIC_STAR},
	{"NUMERIC_POUND", KEY_NUMERIC_POUND},
/*	{"NUMERIC_A", KEY_NUMERIC_A},
	{"NUMERIC_B", KEY_NUMERIC_B},
	{"NUMERIC_C", KEY_NUMERIC_C},
	{"NUMERIC_D", KEY_NUMERIC_D}, */
	{"CAMERA_FOCUS", KEY_CAMERA_FOCUS},
	{"WPS_BUTTON", KEY_WPS_BUTTON},
	{"TOUCHPAD_TOGGLE", KEY_TOUCHPAD_TOGGLE},
	{"TOUCHPAD_ON", KEY_TOUCHPAD_ON},
	{"TOUCHPAD_OFF", KEY_TOUCHPAD_OFF},
	{"CAMERA_ZOOMIN", KEY_CAMERA_ZOOMIN},
	{"CAMERA_ZOOMOUT", KEY_CAMERA_ZOOMOUT},
	{"CAMERA_UP", KEY_CAMERA_UP},
	{"CAMERA_DOWN", KEY_CAMERA_DOWN},
	{"CAMERA_LEFT", KEY_CAMERA_LEFT},
	{"CAMERA_RIGHT", KEY_CAMERA_RIGHT},
	{"ATTENDANT_ON", KEY_ATTENDANT_ON},
	{"ATTENDANT_OFF", KEY_ATTENDANT_OFF},
	{"ATTENDANT_TOGGLE", KEY_ATTENDANT_TOGGLE},
	{"LIGHTS_TOGGLE", KEY_LIGHTS_TOGGLE},
	{"ALS_TOGGLE", KEY_ALS_TOGGLE},
	{"BUTTONCONFIG", KEY_BUTTONCONFIG},
	{"TASKMANAGER", KEY_TASKMANAGER},
	{"JOURNAL", KEY_JOURNAL},
	{"CONTROLPANEL", KEY_CONTROLPANEL},
	{"APPSELECT", KEY_APPSELECT},
	{"SCREENSAVER", KEY_SCREENSAVER},
	{"VOICECOMMAND", KEY_VOICECOMMAND},
	{"BRIGHTNESS_MIN", KEY_BRIGHTNESS_MIN},
	{"BRIGHTNESS_MAX", KEY_BRIGHTNESS_MAX},
	{"KBDINPUTASSIST_PREV", KEY_KBDINPUTASSIST_PREV},
	{"KBDINPUTASSIST_NEXT", KEY_KBDINPUTASSIST_NEXT},
	{"KBDINPUTASSIST_PREVGROUP", KEY_KBDINPUTASSIST_PREVGROUP},
	{"KBDINPUTASSIST_NEXTGROUP", KEY_KBDINPUTASSIST_NEXTGROUP},
	{"KBDINPUTASSIST_ACCEPT", KEY_KBDINPUTASSIST_ACCEPT},
	{"KBDINPUTASSIST_CANCEL", KEY_KBDINPUTASSIST_CANCEL},
	{"MIN_INTERESTING", KEY_MIN_INTERESTING}
};

static int EventKeyConst(lua_State *L){
	return findConst(L, _evtKeys);
}

static int EventKeyName(lua_State *L){
	return rfindConst(L, _evtKeys);
}

/* grep EV_ /usr/include/linux/input-event-codes.h |  awk '{ gsub("EV_","",$2); print "\t{\""$2"\", EV_"$2"}," }' */
static const struct ConstTranscode _evtTypes[] = {
	{"SYN", EV_SYN},
	{"KEY", EV_KEY},
	{"REL", EV_REL},
	{"ABS", EV_ABS},
	{"MSC", EV_MSC},
	{"SW", EV_SW},
	{"LED", EV_LED},
	{"SND", EV_SND},
	{"REP", EV_REP},
	{"FF", EV_FF},
	{"PWR", EV_PWR},
	{"FF_STATUS", EV_FF_STATUS}
};

static int EventTypeConst(lua_State *L){
	return findConst(L, _evtTypes);
}

static int EventTypeName(lua_State *L){
	return rfindConst(L, _evtTypes);
}

static struct SelEvent *checkSelEvent(lua_State *L){
	void *r = luaL_checkudata(L, 1, "SelEvent");
	luaL_argcheck(L, r != NULL, 1, "'SelEvent' expected");
	return (struct SelEvent *)r;
}

static int EventCreate(lua_State *L){
/*	Create an events' handler
 *	-> 1: /dev/input/event's file
 *	-> 2: function to be called (must be as fast as possible)
 */
	struct SelEvent *event;
	const char *fn = luaL_checkstring(L, 1);	/* Event's file */
	int t,f;

	if( lua_type(L, 2) != LUA_TFUNCTION ){
		lua_pushstring(L, "Expecting function for argument #2 of SelEvent.create()");
		lua_error(L);
		exit(EXIT_FAILURE);
	} else
		f = findFuncRef(L,2);

	if((t = open( fn, O_RDONLY | O_NOCTTY )) == -1){
		lua_pushnil(L);
		lua_pushstring(L, strerror(errno));
		return 2;
	}

	event = (struct SelEvent *)lua_newuserdata(L, sizeof( struct SelEvent ));
	luaL_getmetatable(L, "SelEvent");
	lua_setmetatable(L, -2);
	event->fd = t;
	event->func = f;

	return 1;
}

static int EventRead(lua_State *L){
	struct SelEvent *event = checkSelEvent(L);
	struct input_event ev;
	int r;

	if((r=read(event->fd, &ev, sizeof( struct input_event ))) != sizeof( struct input_event )){
#ifdef DEBUG
		printf("*D* Read input_event : only %d bytes read\n", r);
#endif
		lua_pushnil(L);
		lua_pushstring(L, "Read input_event : unexpected read");
		return 2;
	}
	lua_Number t = ev.time.tv_sec + (lua_Number)ev.time.tv_usec/1000000.0;
	lua_pushnumber( L, t );
	lua_pushnumber( L, ev.type );
	lua_pushnumber( L, ev.code );
	lua_pushnumber( L, ev.value );
	return 4;
}

static const struct luaL_reg SelEventLib [] = {
	{"create", EventCreate},
	{"KeyConst", EventKeyConst},
	{"KeyName", EventKeyName},
	{"TypeConst", EventTypeConst},
	{"TypeName", EventTypeName},
	{NULL, NULL}
};

static const struct luaL_reg SelEventM [] = {
	{"read", EventRead},
	{NULL, NULL}
};

void init_SelEvent( lua_State *L ){
	luaL_newmetatable(L, "SelEvent");
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);	/* metatable.__index = metatable */
	luaL_register(L, NULL, SelEventM);
	luaL_register(L,"SelEvent", SelEventLib);
}
