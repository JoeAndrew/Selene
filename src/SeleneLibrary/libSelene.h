/*	libSelene.h
 *
 */

#ifndef SEL_LIBRARY_H
#define SEL_LIBRARY_H

/*
 *	Options agnostics libraries contents
 */

extern char *SelL_strdup( const char * );	/* May missing on some system */
extern int hash( const char * );	/* calculates hash code of a string */

/*
 * Lua startup functions management
 *
 *	These function will be called at every thread creation
 *	(Generally, for libraries initialisation)
 */
#include <lua.h>

	/* Add a function to a startup list
	 * -> func : startup function to call
	 * -> list : current list (returned from a previous libSel_AddStartupFunc()
	 *  call. Null for the 1st one.
	 * <- return an opaque list object (NULL in case of fatal error)
	 *
	 * Notez-bien : they will be called in reverse order
	 */
extern void *libSel_AddStartupFunc( void (*func)( lua_State * ), void *list );

	/* Apply startup function to a Lua's State
	 * -> L : Lua state
	 * -> list : pointer returned by last libSel_AddStartupFunc() call
	 */
extern void libSel_ApplyStartupFunc( lua_State *L, void *list );

#endif
