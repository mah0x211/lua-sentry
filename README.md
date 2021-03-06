lua-sentry
===

kqueue/epoll event sentry module.

**NOTE: Do not use this module. this module is under heavy development.**


## Installation

```sh
luarocks install sentry --from=http://mah0x211.github.io/rocks/
```

## Constants


## Creating a Sentry Object

### s, err = sentry.new( [bufsize:int] );

returns the sentry object.


**Parameters**

- `bufsize:int`: event buffer size. (`default 128`)


**Returns**

- `s:userdata`: sentry object on success, or nil on failure.
- `err:string`: error string.


**NOTE: bufsize will be automatically resized to larger than specified size if need more buffer allocation.**


### s, err = sentry.default( [bufsize:int] );

returns the default sentry object.


**Parameters** and **Returns** are same as sentry.new function.


## Sentry Methods


### ok, err = s:renew()

renew(recreate) the internal event descriptor.


**Returns**

- `ok:boolean`: true on success, or false on failure.
- `err:string`: error string.


### ev = s:newevent();

returns the new [empty event object](#empty-event-object-methods).

**Returns**

- `ev:userdata`: event object.


### evs, err = s:newevents( [nevt:int] );

returns the table that has new [empty event object(s)](#empty-event-object-methods).

**Parameters**

- `nevt:int`: number of new event. (`default 1`)

**Returns**

- `evs:table`: table on success, or nil on failure.
- `err:string`: error string.



### nevt, err = s:wait( [timeout:lua_Integer] )

wait until event occurring.


**Parameters**

- `timeout:lua_Integer`: wait until specified timeout milliseconds. `default: -1(never-timeout)`


**Returns**

- `nevt:int`: number of the occurred events.
- `err:string`: error string.


### ev, ctx, disabled = s:getevent()

returns an event object.


**Returns**

- `ev:userdata`: event object or nil.
- `ctx:any`: context object.
- `disabled:boolean`: if true, event is disabled.


## Empty Event Object Methods

empty event object can be use as following event object;

### err = ev:astimer( timeout:number [, ctx:any [, oneshot:boolean]] )

to use the event object as a timer event.

**Parameters**

- `timeout:number`: timeout milliseconds.
- `ctx:any`: context object.
- `oneshot:boolean`: automatically unregister this event when event occurred.


**Returns**

- `err:string`: nil on success, or error string on failure.


### err = ev:assignal( signo:int [, ctx:any [, oneshot:boolean]] )

to use as a signal event.

**Parameters**

- `signo:int`: signal number.
- `ctx:any`: context object.
- `oneshot:boolean`: automatically unregister this event when event occurred.


**Returns**

- `err:string`: nil on success, or error string on failure.


### err = ev:asreadable( fd:int [, ctx:any [, oneshot:boolean [, edge:boolean]]] )

to use as a readable event.

**Parameters**

- `fd:int`: descriptor.
- `ctx:any`: context object.
- `oneshot:boolean`: automatically unregister this event when event occurred.
- `edge:boolean`: using an edge-trigger if specified a true. `default: level-trigger`.


**Returns**

- `err:string`: nil on success, or error string on failure.


### err = ev:aswritable( fd:int [, ctx:any [, oneshot:boolean [, edge:boolean]]] )

to use as a writable event.

**Parameters**

- `fd:int`: descriptor.
- `ctx:any`: context object.
- `oneshot:boolean`: automatically unregister this event when event occurred.
- `edge:boolean`: using an edge-trigger if specified a true. `default: level-trigger`.


**Returns**

- `err:string`: nil on success, or error string on failure.


## Common Methods Of Non-Empty Event Object.


### ev = ev:revert()

revert to an empty event object.

**Returns**

- `ev:userdata`: empty event object.


### ok, err = ev:renew( [s] )

renew(rewatch) an event object.

**Parameters**

- `s:userdata`: sentry object.

**Returns**

- `ok:boolean`: true on success, or false on failure.
- `err:string`: error string.


### id = ev:ident()

returns an event ident.


**Returns**

- `id`
    - `timeout:number` if timer event.
    - `signo:int` if signal event.
    - `fd:int` if readable or writable event.


### asa = ev:asa()

returns an event type.


**Returns**

- `asa:string`: `astimer`, `assignal`, `asreadable` or `aswritable`.


### ctx = ev:context( [ctx:any] )

returns a current context object, and if argument passed then replace that object to passed argument.

**Parameters**

- `ctx:any`: context object.

**Returns**

- `ctx:any`: current context object.


### ev:unwatch()

unregister this event.


### ok, err = ev:watch()

register this event.


**Returns**

- `ok:boolean`: true on success, or false on failure.
- `err:string`: error string.

