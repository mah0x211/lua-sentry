/*
 *  Copyright (C) 2015 Masatoshi Teruya
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 *
 *
 *  kevent/event.c
 *  lua-sentry
 *  Created by Masatoshi Teruya on 2015/08/23.
 *
 */

#include "sentry.h"
#include "sentry_event.h"


static int unwatch_lua( lua_State *L )
{
    sentry_ev_t *e = luaL_checkudata( L, 1, SENTRY_EVENT_MT );
    
    if( lstate_isref( e->ref ) )
    {
        struct kevent evt = e->reg;
        
        // check filter type
        switch( e->reg.filter ){
            case EVFILT_READ:
                fddelset( &e->s->fds, e->reg.ident, FDSET_READ );
            break;
            
            case EVFILT_WRITE:
                fddelset( &e->s->fds, e->reg.ident, FDSET_WRITE );
            break;
            
            case EVFILT_SIGNAL:
                sigdelset( &e->s->signals, e->reg.ident );
            break;
        }
        
        // unregister event
        evt.flags = EV_DELETE;
        kevent( e->s->fd, &evt, 1, NULL, 0, NULL );
        e->s->nreg--;
        e->ref = lstate_unref( L, e->ref );
    }

    lua_pushboolean( L, 1 );
    
    return 1;
}


static int watch_lua( lua_State *L )
{
    sentry_ev_t *e = luaL_checkudata( L, 1, SENTRY_EVENT_MT );
    
    if( !lstate_isref( e->ref ) )
    {
        // register event
        if( sentry_register( e->s, e ) != 0 ){
            // got error
            lua_pushboolean( L, 0 );
            lua_pushstring( L, strerror( errno ) );
            return 2;
        }
        
        // check filter type
        switch( e->reg.filter ){
            case EVFILT_READ:
                fdaddset( &e->s->fds, e->reg.ident, FDSET_READ );
            break;
            
            case EVFILT_WRITE:
                fdaddset( &e->s->fds, e->reg.ident, FDSET_WRITE );
            break;
            
            case EVFILT_SIGNAL:
                sigaddset( &e->s->signals, e->reg.ident );
            break;
        }

        // retain event
        e->ref = lstate_ref( L );
    }
    
    lua_pushboolean( L, 1 );
    
    return 1;
}


static int context_lua( lua_State *L )
{
    sentry_ev_t *e = luaL_checkudata( L, 1, SENTRY_EVENT_MT );

    if( lstate_isref( e->ctx ) ){
        lstate_pushref( L, e->ctx );
        return 1;
    }
    
    return 0;
}


static int typeof_lua( lua_State *L )
{
    sentry_ev_t *e = luaL_checkudata( L, 1, SENTRY_EVENT_MT );

    lua_pushinteger( L, sev_type( e ) );
    
    return 1;
}


static int ident_lua( lua_State *L )
{
    sentry_ev_t *e = luaL_checkudata( L, 1, SENTRY_EVENT_MT );

    if( e->reg.filter == EVFILT_TIMER ){
        lua_pushnumber( L, e->reg.data / 1000000000.0 );
    }
    else{
        lua_pushinteger( L, e->reg.ident );
    }
    
    return 1;
}


static int tostring_lua( lua_State *L )
{
    return TOSTRING_MT( L, SENTRY_EVENT_MT );
}


static int gc_lua( lua_State *L )
{
    sentry_ev_t *e = lua_touserdata( L, 1 );
    
    // release context
    lstate_unref( L, e->ctx );
    
    return 0;
}


LUALIB_API int luaopen_sentry_event( lua_State *L )
{
    struct luaL_Reg mmethod[] = {
        { "__gc", gc_lua },
        { "__tostring", tostring_lua },
        { NULL, NULL }
    };
    struct luaL_Reg method[] = {
        { "ident", ident_lua },
        { "typeof", typeof_lua },
        { "context", context_lua },
        { "watch", watch_lua },
        { "unwatch", unwatch_lua },
        { NULL, NULL }
    };
    
    sentry_define_mt( L, SENTRY_EVENT_MT, mmethod, method );
    
    return 0;
}

