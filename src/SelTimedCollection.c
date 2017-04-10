/*	SelTimedCollection.c
 *
 *	Timed values collection
 *
 *	10/04/2017	LF : First version
 */

#include "SelTimedCollection.h"

#include <assert.h>
#include <stdlib.h>

static struct SelTimedCollection *checkSelTimedCollection(lua_State *L){
	void *r = luaL_checkudata(L, 1, "SelTimedCollection");
	luaL_argcheck(L, r != NULL, 1, "'SelTimedCollection' expected");
	return (struct SelTimedCollection *)r;
}

static int stcol_push(lua_State *L){
	struct SelTimedCollection *col = checkSelTimedCollection(L);

	col->data[ col->last % col->size].data = luaL_checknumber( L, 2 );
	col->data[ col->last++ % col->size].t = (lua_type( L, 3 ) == LUA_TNUMBER) ? lua_tonumber( L, 3 ) : time(NULL);

	if(col->last > col->size)
		col->full = 1;
	return 0;
}

static int stcol_dump(lua_State *L){
	struct SelTimedCollection *col = checkSelTimedCollection(L);

	printf("SelTimedCollection's Dump (size : %d, last : %d)\n", col->size, col->last);
	if(col->full)
		for(unsigned int i = col->last - col->size; i < col->last; i++)
			printf("\t%f @ %s", col->data[i % col->size].data, ctime( &col->data[i % col->size].t ) );
	else
		for(unsigned int i = 0; i < col->last; i++)
			printf("\t%f @ %s", col->data[i].data, ctime( &col->data[i].t ) );
	return 0;
}

static int stcol_create(lua_State *L){
	struct SelTimedCollection *col = (struct SelTimedCollection *)lua_newuserdata(L, sizeof(struct SelTimedCollection));
	luaL_getmetatable(L, "SelTimedCollection");
	lua_setmetatable(L, -2);
	if(!(col->size = luaL_checkint( L, 1 ))){
		fputs("*E* SelTimedCollection's size can't be null\n", stderr);
		exit(EXIT_FAILURE);
	}
	assert( col->data = calloc(col->size, sizeof(struct timeddata)) );
	col->last = 0;
	col->full = 0;

	return 1;
}

static const struct luaL_reg SelTimedColLib [] = {
	{"create", stcol_create}, 
	{NULL, NULL}
};

static const struct luaL_reg SelTimedColM [] = {
	{"Push", stcol_push},
/*	{"MinMax", scol_minmax},
	{"Data", scol_data},
	{"iData", scol_idata},
	{"GetSize", scol_getsize},
	{"HowMany", scol_HowMany}, */
	{"dump", stcol_dump},
	{NULL, NULL}
};


void init_SelTimedCollection( lua_State *L ){
	luaL_newmetatable(L, "SelTimedCollection");
	lua_pushstring(L, "__index");
	lua_pushvalue(L, -2);
	lua_settable(L, -3);	/* metatable.__index = metatable */
	luaL_register(L, NULL, SelTimedColM);
	luaL_register(L,"SelTimedCollection", SelTimedColLib);
}
