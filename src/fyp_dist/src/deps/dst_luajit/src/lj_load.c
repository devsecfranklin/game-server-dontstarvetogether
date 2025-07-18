/*
** Load and dump code.
** Copyright (C) 2005-2016 Mike Pall. See Copyright Notice in luajit.h
*/

#include <errno.h>
#include <stdio.h>

#define lj_load_c
#define LUA_CORE

#include "lua.h"
#include "lauxlib.h"

#include "lj_obj.h"
#include "lj_gc.h"
#include "lj_err.h"
#include "lj_buf.h"
#include "lj_func.h"
#include "lj_frame.h"
#include "lj_vm.h"
#include "lj_lex.h"
#include "lj_bcdump.h"
#include "lj_parse.h"

/* -- Load Lua source code and bytecode ----------------------------------- */

static TValue *cpparser(lua_State *L, lua_CFunction dummy, void *ud)
{
  LexState *ls = (LexState *)ud;
  GCproto *pt;
  GCfunc *fn;
  int bc;
  UNUSED(dummy);
  cframe_errfunc(L->cframe) = -1;  /* Inherit error function. */
  bc = lj_lex_setup(L, ls);
  if (ls->mode && !strchr(ls->mode, bc ? 'b' : 't')) {
    setstrV(L, L->top++, lj_err_str(L, LJ_ERR_XMODE));
    lj_err_throw(L, LUA_ERRSYNTAX);
  }
  pt = bc ? lj_bcread(ls) : lj_parse(ls);
  fn = lj_func_newL_empty(L, pt, tabref(L->env));
  /* Don't combine above/below into one statement. */
  setfuncV(L, L->top++, fn);
  return NULL;
}

LUA_API int lua_loadx(lua_State *L, lua_Reader reader, void *data,
		      const char *chunkname, const char *mode)
{
// HAX  printf("lua_loadx called..\n");
  LexState ls;
  int status;
  ls.rfunc = reader;
  ls.rdata = data;
  ls.chunkarg = chunkname ? chunkname : "?";
  ls.mode = mode;
  lj_buf_init(L, &ls.sb);
  status = lj_vm_cpcall(L, NULL, &ls, cpparser);
  lj_lex_cleanup(L, &ls);
  lj_gc_check(L);
// HAX  printf("lua_loadx returned..\n");
  return status;
}

LUA_API int lua_load(lua_State *L, lua_Reader reader, void *data,
		     const char *chunkname)
{
// HAX  printf("lua_load called\n");
  return lua_loadx(L, reader, data, chunkname, NULL);
}

typedef struct FileReaderCtx {
  FILE *fp;
  char buf[LUAL_BUFFERSIZE];
} FileReaderCtx;

static const char *reader_file(lua_State *L, void *ud, size_t *size)
{
  FileReaderCtx *ctx = (FileReaderCtx *)ud;
  UNUSED(L);
  if (feof(ctx->fp)) return NULL;
  *size = fread(ctx->buf, 1, sizeof(ctx->buf), ctx->fp);
  return *size > 0 ? ctx->buf : NULL;
}

LUALIB_API int luaL_loadfilex(lua_State *L, const char *filename,
			      const char *mode)
{
// HAX  printf("luaL_loadfilex called..\n");
  FileReaderCtx ctx;
  int status;
  const char *chunkname;
  if (filename) {
    ctx.fp = fopen(filename, "rb");
    if (ctx.fp == NULL) {
      lua_pushfstring(L, "cannot open %s: %s", filename, strerror(errno));
      return LUA_ERRFILE;
    }
    chunkname = lua_pushfstring(L, "@%s", filename);
  } else {
    ctx.fp = stdin;
    chunkname = "=stdin";
  }
  status = lua_loadx(L, reader_file, &ctx, chunkname, mode);
  if (ferror(ctx.fp)) {
    L->top -= filename ? 2 : 1;
    lua_pushfstring(L, "cannot read %s: %s", chunkname+1, strerror(errno));
    if (filename)
      fclose(ctx.fp);
    return LUA_ERRFILE;
  }
  if (filename) {
    L->top--;
    copyTV(L, L->top-1, L->top);
    fclose(ctx.fp);
  }
  return status;
}

LUALIB_API int luaL_loadfile(lua_State *L, const char *filename)
{
  return luaL_loadfilex(L, filename, NULL);
}

typedef struct StringReaderCtx {
  const char *str;
  size_t size;
} StringReaderCtx;

static const char *reader_string(lua_State *L, void *ud, size_t *size)
{
  StringReaderCtx *ctx = (StringReaderCtx *)ud;
  UNUSED(L);
  if (ctx->size == 0) return NULL;
  *size = ctx->size;
  ctx->size = 0;
  return ctx->str;
}

char* hack_old_variadic(const char* base, size_t size) {
    char* bc = (char*)base; // base cursor
    char* patch = "table.getn(arg) ";

    if (strstr(bc, "local arg = {n=select"))
        return NULL;

    if (NULL == strstr(bc, "arg.n "))
        return NULL;

    char* modified_code = malloc(size * 2); // mods won't fit in-place
    memset(modified_code, 0, size*2);

    char* mc = modified_code;

    int seen = 0;

    while(1) {

        char* patch_ptr = strstr(bc, "arg.n ");
        if (NULL == patch_ptr)
            break;

        seen = 1;

        size_t block_size = patch_ptr - bc;
        // copy in pre-patch code
        memcpy(mc, bc, block_size);

        mc += block_size; // update mod cursor
        memcpy(mc, patch, strlen(patch));
        mc += strlen(patch); // update mod cursor


        bc = patch_ptr + strlen("arg.n "); // update base cursor

    }

    memcpy(mc, bc, size - (bc - base));

    printf("%s\n", modified_code);

//    if (seen) {
//        FILE* fp = fopen("/tmp/seen.txt", "wb");
//        fwrite(modified_code, 1, strlen(modified_code), fp);
//        fclose(fp);
//    }
      // fwrite(target, 1, (q - target) + (base - t) + size, fp);
      // fclose(fp);


    return modified_code;
}

char* hack_gemcore(const char* base, size_t size)
{
	char* target;
    char* t = base;
    char* s = NULL, *p = NULL, *q = NULL;

    if (strstr(base, "return _debug_getinfo") == NULL) return NULL;

    target = (char*)malloc(size * 2 + 32);
    memset(target, 0, size * 2 + 32);
    q = target;

    while (
      ((p = strstr(t, "return _debug_")) != NULL) ||
      ((p = strstr(t, "return _getfenv")) != NULL) ||
      ((p = strstr(t, "return _setfenv")) != NULL)
      ) {
      memcpy(q, t, p - t);
      q += p - t;
      if (memcmp(p, "return _debug_getupvalue", 24) == 0) {
        memcpy(q, p, 24);
        t = p + 24;
        q += 24;
        continue;
      }

      s = strstr(p, "end");
      if (s != NULL) {
        memcpy(q, p, s - p); q += s - p;
        memcpy(q, ", nil end", 9); q += 9;
        t = s + 3;
      } else {
        break;
      }
    }

    if (s != NULL)
    {
      // FILE* fp = fopen("hahaha.txt", "wb");
      memcpy(q, t, base + size - t);
      // fwrite(target, 1, (q - target) + (base - t) + size, fp);
      // fclose(fp);
      return target;
    }
    
    free(target);
    return NULL;
}

LUALIB_API int luaL_loadbufferx(lua_State *L, const char *buf, size_t size,
				const char *name, const char *mode)
{
  StringReaderCtx ctx;
  int ret;
  char* mod_buff = hack_old_variadic(buf,size);

  char* target = NULL;

  if (NULL != mod_buff) {
      size = strlen(mod_buff);
      buf = mod_buff;

  }

  
  target = hack_gemcore(buf, size);

  ctx.str = target == NULL ? buf : target;
  ctx.size = target == NULL ? size : strlen(target);
  ret = lua_loadx(L, reader_string, &ctx, name, mode);
  if (target != NULL) free(target);
  if (mod_buff != NULL) free(mod_buff); // free on null pointer is a noop anyway
  return ret;
}

LUALIB_API int luaL_loadbuffer(lua_State *L, const char *buf, size_t size,
			       const char *name)
{
  return luaL_loadbufferx(L, buf, size, name, NULL);
}

LUALIB_API int luaL_loadstring(lua_State *L, const char *s)
{
  return luaL_loadbuffer(L, s, strlen(s), s);
}

/* -- Dump bytecode ------------------------------------------------------- */

LUA_API int lua_dump(lua_State *L, lua_Writer writer, void *data)
{
  cTValue *o = L->top-1;
  api_check(L, L->top > L->base);
  if (tvisfunc(o) && isluafunc(funcV(o)))
    return lj_bcwrite(L, funcproto(funcV(o)), writer, data, 0);
  else
    return 1;
}

