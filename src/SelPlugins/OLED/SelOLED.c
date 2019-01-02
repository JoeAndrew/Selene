/* SelOLED
 *
 * This file contains all stuffs related to SSD1306 I2C driven OLED display
 *
 * 26/12/2018 LF : First version
 */

#include "SelOLED.h"

#ifdef USE_OLED
static const struct ConstTranscode _Colors[] = {
	{ "BLACK", BLACK },
	{ "WHITE", WHITE },
	{ NULL, 0 }
};

static int OLEColorsConst( lua_State *L ){
	return findConst(L, _Colors);
}

static int OLEDot(lua_State *L){
/* "list" all known screens
 * <- known screen
 */
	int i;
	for(i=0; i<OLED_LAST_OLED; i++)
		lua_pushstring( L, oled_type_str[i] );

	return OLED_LAST_OLED;
}

static int OLEDinit(lua_State *L){
/* Open the display
 * -> 1: screen ID
 * -> 2: i2c device
 * <- success or not (boolean)
 */
	int v = luaL_checkinteger(L, 1);
	const char *arg = luaL_checkstring(L, 2);

	if(v < 1 || v > OLED_LAST_OLED){
		lua_pushnil(L);
		lua_pushstring(L, "Screen ident our of range");
		return 2;
	}

	lua_pushboolean(L, PiOLED_Init(--v, arg));
	return 1;
}

static int OLEDclose(lua_State *L){
/* Free all ressources used for OLED
 */
	PiOLED_Close();
	return 0;
}

static int OLEDdisplay(lua_State *L){
/* refresh the screen
 */
	PiOLED_Display();
	return 0;
}

static int OLEDclear(lua_State *L){
/* Clear the screen
 */
	PiOLED_ClearDisplay();
	return 0;
}

static int OLEDtextsize(lua_State *L){
/* Set the text size
 * -> 1: size of letters
 */
	int v = luaL_checkinteger(L, 1);
	PiOLED_SetTextSize(v);
	return 0;
}

static int OLEDtextcolor(lua_State *L){
/* Set the text color
 * -> 1: front
 * -> 2: background (optional)
 */
	int f = luaL_checkinteger(L, 1);
	if( lua_isnumber(L, 2) ){
		int b = lua_tointeger(L, 2);
		PiOLED_SetTextColor2(f,b);
	} else
		PiOLED_SetTextColor(f);
	return 0;
}

static int OLEDcursor(lua_State *L){
/* Set the text cursor position
 * -> 1 : x
 * -> 2 : y
 */
	int x = luaL_checkinteger(L, 1);
	int y = luaL_checkinteger(L, 2);
	
	PiOLED_SetCursor(x,y);

	return 0;
}

static int OLEDinvert(lua_State *L){
/* Invert the screen
 * -> true : screen is inverted
 * -> false : screen is normal
 */
	boolean v;
	if(!lua_isboolean(L,1)){
		lua_pushnil(L);
		lua_pushstring(L, "Boolean expected");
		return 2;
	}

	v = lua_toboolean(L, 1);
	PiOLED_Invert(!!v);
	return 0;
}

static int OLEDflip(lua_State *L){
/* Flip the screen upside-down
 * -> true : screen is flipped
 * -> false : screen is normal
 */
	boolean v;
	if(!lua_isboolean(L,1)){
		lua_pushnil(L);
		lua_pushstring(L, "Boolean expected");
		return 2;
	}

	v = lua_toboolean(L, 1);
	PiOLED_Flip(!!v);
	return 0;
}

static int OLEDOnOff(lua_State *L){
/* Turn the screen On or Off
 * -> true : screen is On
 * -> false : screen is Off
 */
	boolean v;
	if(!lua_isboolean(L,1)){
		lua_pushnil(L);
		lua_pushstring(L, "Boolean expected");
		return 2;
	}

	v = lua_toboolean(L, 1);
	PiOLED_OnOff(!!v);
	return 0;
}

static int OLEDdrawpixel(lua_State *L){
/* Draw a pixel
 * -> 1 : x
 * -> 2 : y
 * -> 3 : color (optional)
 */
	int x = luaL_checkinteger(L, 1);
	int y = luaL_checkinteger(L, 2);

	int color = WHITE;
	if( lua_isnumber(L, 3) )
		color = lua_tointeger(L, 3);
	
	PiOLED_DrawPixel(x,y, color);
	return 0;
}

static int OLEDdrawline(lua_State *L){
/* Draw a line
 * -> 1 : left upper x
 * -> 2 : left upper y
 * -> 3 : bottom right x
 * -> 4 : bottom right y
 * -> 5 : color (optional)
 */
	int x0 = luaL_checkinteger(L, 1);
	int y0 = luaL_checkinteger(L, 2);
	int x1 = luaL_checkinteger(L, 3);
	int y1 = luaL_checkinteger(L, 4);

	int color = WHITE;
	if( lua_isnumber(L, 5) )
		color = lua_tointeger(L, 5);

	PiOLED_DrawLine(x0,y0,x1,y1,color);
	return 0;
}

static int OLEDrect(lua_State *L){
/* Draw a rectangle
 * -> 1 : left upper x
 * -> 2 : left upper y
 * -> 3 : width
 * -> 4 : height
 * -> 5 : color (optional)
 */
	int x = luaL_checkinteger(L, 1);
	int y = luaL_checkinteger(L, 2);
	int w = luaL_checkinteger(L, 3);
	int h = luaL_checkinteger(L, 4);

	int color = WHITE;
	if( lua_isnumber(L, 5) )
		color = lua_tointeger(L, 5);

	PiOLED_DrawRect(x,y,w,h,color);
	return 0;
}

static int OLEDrectf(lua_State *L){
/* Draw a filled rectangle
 * -> 1 : left upper x
 * -> 2 : left upper y
 * -> 3 : width
 * -> 4 : height
 * -> 5 : color (optional)
 */
	int x = luaL_checkinteger(L, 1);
	int y = luaL_checkinteger(L, 2);
	int w = luaL_checkinteger(L, 3);
	int h = luaL_checkinteger(L, 4);

	int color = WHITE;
	if( lua_isnumber(L, 5) )
		color = lua_tointeger(L, 5);

	PiOLED_FillRect(x,y,w,h,color);
	return 0;
}

static int OLEDcircle(lua_State *L){
/* Draw a circle
 * -> 1 : center x
 * -> 2 : center y
 * -> 3 : radius
 * -> 4 : color (optional)
 */
	int x = luaL_checkinteger(L, 1);
	int y = luaL_checkinteger(L, 2);
	int r = luaL_checkinteger(L, 3);

	int color = WHITE;
	if( lua_isnumber(L, 4) )
		color = lua_tointeger(L, 4);

	PiOLED_DrawCircle(x,y,r,color);
	return 0;
}

static int OLEDcirclef(lua_State *L){
/* Draw a filled circle
 * -> 1 : center x
 * -> 2 : center y
 * -> 3 : radius
 * -> 4 : color (optional)
 */
	int x = luaL_checkinteger(L, 1);
	int y = luaL_checkinteger(L, 2);
	int r = luaL_checkinteger(L, 3);

	int color = WHITE;
	if( lua_isnumber(L, 4) )
		color = lua_tointeger(L, 4);

	PiOLED_FillCircle(x,y,r,color);
	return 0;
}

static int OLEDprint(lua_State *L){
/* Print a message to the screen
 * -> 1: message to print (string)
 */
	const char *arg = luaL_checkstring(L, 1);

	PiOLED_Print(arg);
	return 0;
}

static int OLEDwidth(lua_State *L){
/* Return the width of the screen 
 * <- the width
 */
	lua_pushinteger(L, PiOLED_DisplayWidth());
	return 1;
}

static int OLEDHeight(lua_State *L){
/* Return the width of the screen 
 * <- the width
 */
	lua_pushinteger(L, PiOLED_DisplayHeight());
	return 1;
}

static const struct luaL_Reg OLEDLib[] = {
	{"oled_type", OLEDot},
	{"ColorsConst", OLEColorsConst},
	{"Init", OLEDinit},
	{"Close", OLEDclose},
	{"Display", OLEDdisplay},
	{"Refresh", OLEDdisplay},	/* Alias */
	{"Clear", OLEDclear},
	{"SetTextSize", OLEDtextsize},
	{"SetTextColor", OLEDtextcolor},
	{"SetTextColor", OLEDtextcolor},
	{"SetCursor", OLEDcursor},
	{"Invert", OLEDinvert},
	{"OnOff", OLEDOnOff},
	{"Flip", OLEDflip},
	{"DrawPixel", OLEDdrawpixel},
	{"Pset", OLEDdrawpixel},	/* Alias */
	{"DrawLine", OLEDdrawline},
	{"Line", OLEDdrawline},		/* Alias */
	{"DrawRect", OLEDrect},
	{"Box", OLEDrect},			/* Alias */
	{"FillRect", OLEDrectf},
	{"BoxF", OLEDrectf},		/* Alias */
	{"DrawCircle", OLEDcircle},
	{"Circle", OLEDcircle},		/* Alias */
	{"FillCircle", OLEDcirclef},
	{"CircleF", OLEDcirclef},		/* Alias */
	{"Print", OLEDprint},
	{"Width", OLEDwidth},
	{"Height", OLEDHeight},
	{NULL, NULL}    /* End of definition */
};

void initSelOLED(lua_State *L){
	libSel_libFuncs( L, "SelOLED", OLEDLib );
}
#endif
