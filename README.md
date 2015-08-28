# lua-sentry

kqueue/epoll event sentry module.


## Installation

```sh
luarocks install sentry --from=http://mah0x211.github.io/rocks/
```

## Creating a Sentry

### s, err = sentry.new( [bufsize:int] );

returns the sentry object.

**Parameters**

- `bufsize:int`: event buffer size. (`default 128`)

**Returns**

- `s:userdata`: sentry object on success, or nil on failure.
- `err:string`: error string.

**NOTE: bufsize will be automatically resized to larger than specified size if need more buffer allocation.**


## Sentry Methods

### ev, err = s:timer( timeout:number [, ctx:any [, oneshot:boolean]] )

creating a timer event object and register that event.

**Parameters**

- `timeout:number`: timeout seconds.
- `ctx:any`: context object.
- `oneshot:boolean`: automatically unregister this event when event occurred.

**Returns**

- `ev:userdata`: event object on success, or nil on failure.
- `err:string`: error string.


### ev, err = s:signal( signo:int [, ctx:any [, oneshot:boolean]] )

creating a signal event object and register that event.

**Parameters**

- `signo:int`: signal number.
- `ctx:any`: context object.
- `oneshot:boolean`: automatically unregister this event when event occurred.

**Returns**

- `ev:userdata`: event object on success, or nil on failure.
- `err:string`: error string.


### ev, err = s:readable( fd:int [, ctx:any [, oneshot:boolean [, edge:boolean]]] )

creating a readable event object and register that event.

**Parameters**

- `fd:int`: descriptor.
- `ctx:any`: context object.
- `oneshot:boolean`: automatically unregister this event when event occurred.
- `edge:boolean`: using an edge-trigger if specified a true. `default: level-trigger`.


**Returns**

- `ev:userdata`: event object on success, or nil on failure.
- `err:string`: error string.


### ev, err = s:writable( fd:int [, ctx:any [, oneshot:boolean [, edge:boolean]]] )

creating a writable event object and register that event.

**Parameters**

- `fd:int`: descriptor.
- `ctx:any`: context object.
- `oneshot:boolean`: automatically unregister this event when event occurred.
- `edge:boolean`: using an edge-trigger if specified a true. `default: level-trigger`.


**Returns**

- `ev:userdata`: event object on success, or nil on failure.
- `err:string`: error string.


### nevt, err = s:wait( [timeout:int] )

wait until event occurring.

**Parameters**

- `timeout:int`: wait until specified timeout seconds. `default: 1 second`


**Returns**

- `nevt:int`: number of the occurred events.
- `err:string`: error string.


### nevt, isdel, ev, ctx = s:getevent()

returns an event object.

**Returns**

- `nevt:int`: the remaining number of events.
- `isdel:boolean`: true on event unregister.
- `ev:userdata`: event object or nil.
- `ctx:any`: context object.


## Event Methods

### id = ev:ident()

returns an event ident.

**Returns**

- `id`
    - `timeout:number` if timer event.
    - `signo:int` if signal event.
    - `fd:int` if readable or writable event.


### ctx = ev:context()

returns a context object.


### ev:unwatch()

unregister this event.


### ok, err = ev:watch()

register this event.


**Returns**

- `ok:boolean`: true on success, or false on failure.
- `err:string`: error string.

