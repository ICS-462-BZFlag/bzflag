#ifndef LUA_BZDB_H
#define LUA_BZDB_H

struct lua_State;

namespace BZDB {
  bool PushEntries(lua_State* L);
}

#endif // LUA_BZDB_H
