#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>

void *handle;
void (*addr_curl_easy_cleanup)();
void (*addr_curl_easy_duphandle)();
void (*addr_curl_easy_escape)();
void (*addr_curl_easy_getinfo)();
void (*addr_curl_easy_init)();
void (*addr_curl_easy_option_by_id)();
void (*addr_curl_easy_option_by_name)();
void (*addr_curl_easy_option_next)();
void (*addr_curl_easy_pause)();
void (*addr_curl_easy_perform)();
void (*addr_curl_easy_recv)();
void (*addr_curl_easy_reset)();
void (*addr_curl_easy_send)();
void (*addr_curl_easy_setopt)();
void (*addr_curl_easy_strerror)();
void (*addr_curl_easy_unescape)();
void (*addr_curl_easy_upkeep)();
void (*addr_curl_escape)();
void (*addr_curl_formadd)();
void (*addr_curl_formfree)();
void (*addr_curl_formget)();
void (*addr_curl_free)();
void (*addr_curl_getdate)();
void (*addr_curl_getenv)();
void (*addr_curl_global_cleanup)();
void (*addr_curl_global_init)();
void (*addr_curl_global_init_mem)();
void (*addr_curl_global_sslset)();
void (*addr_curl_maprintf)();
void (*addr_curl_mfprintf)();
void (*addr_curl_mime_addpart)();
void (*addr_curl_mime_data)();
void (*addr_curl_mime_data_cb)();
void (*addr_curl_mime_encoder)();
void (*addr_curl_mime_filedata)();
void (*addr_curl_mime_filename)();
void (*addr_curl_mime_free)();
void (*addr_curl_mime_headers)();
void (*addr_curl_mime_init)();
void (*addr_curl_mime_name)();
void (*addr_curl_mime_subparts)();
void (*addr_curl_mime_type)();
void (*addr_curl_mprintf)();
void (*addr_curl_msnprintf)();
void (*addr_curl_msprintf)();
void (*addr_curl_multi_add_handle)();
void (*addr_curl_multi_assign)();
void (*addr_curl_multi_cleanup)();
void (*addr_curl_multi_fdset)();
void (*addr_curl_multi_info_read)();
void (*addr_curl_multi_init)();
void (*addr_curl_multi_perform)();
void (*addr_curl_multi_poll)();
void (*addr_curl_multi_remove_handle)();
void (*addr_curl_multi_setopt)();
void (*addr_curl_multi_socket)();
void (*addr_curl_multi_socket_action)();
void (*addr_curl_multi_socket_all)();
void (*addr_curl_multi_strerror)();
void (*addr_curl_multi_timeout)();
void (*addr_curl_multi_wait)();
void (*addr_curl_multi_wakeup)();
void (*addr_curl_mvaprintf)();
void (*addr_curl_mvfprintf)();
void (*addr_curl_mvprintf)();
void (*addr_curl_mvsnprintf)();
void (*addr_curl_mvsprintf)();
void (*addr_curl_pushheader_byname)();
void (*addr_curl_pushheader_bynum)();
void (*addr_curl_share_cleanup)();
void (*addr_curl_share_init)();
void (*addr_curl_share_setopt)();
void (*addr_curl_share_strerror)();
void (*addr_curl_slist_append)();
void (*addr_curl_slist_free_all)();
void (*addr_curl_strequal)();
void (*addr_curl_strnequal)();
void (*addr_curl_unescape)();
void (*addr_curl_url)();
void (*addr_curl_url_cleanup)();
void (*addr_curl_url_dup)();
void (*addr_curl_url_get)();
void (*addr_curl_url_set)();
void (*addr_curl_url_strerror)();
void (*addr_curl_version)();
void (*addr_curl_version_info)();
void *curl_jmpenv;

void *fakelib_payload() {
	return 0;
}

// Caller frees
char* get_home_path() {
    char* env_home = getenv("HOME");
    char* path = malloc(PATH_MAX);
    strncpy(path, env_home, PATH_MAX-1);
    return path;
}

void __attribute__ ((constructor)) fakelib_init(void);
void fakelib_init() {
	unsetenv("LD_PRELOAD");
	handle = dlopen("/usr/lib/x86_64-linux-gnu/libcurl-gnutls.so.4", RTLD_LAZY | RTLD_DEEPBIND );
	if (handle == 0)
	    handle = dlopen("/usr/lib/libcurl-gnutls.so.4", RTLD_LAZY | RTLD_DEEPBIND );
	
	// Ok so the lib is not in the expected location, maybe the user hasn't installed it
	// So it gets pulled from steam environment
	// So we gotta get at that environment, we assume its installed in home
	if (handle == 0) {
	    char* home_path = get_home_path();
	    char steam_path[PATH_MAX] = {0};
	    snprintf(steam_path, PATH_MAX, "%s/.local/share/Steam/ubuntu12_32/steam-runtime/usr/lib/x86_64-linux-gnu/libcurl-gnutls.so.4", home_path);
	    handle = dlopen(steam_path, RTLD_LAZY | RTLD_DEEPBIND );
	    free(home_path);
	}

	if (handle == 0) {
	    printf("[fyp:shim_loader] Could not find path to libcurl-gnutls.so.4, tell pancho\n");
	    //exit(1);
	}
	if (handle != 0) {
	    printf("[fyp:shim_loader] Loading second stage loader: fyp_loader.so\n");
	    dlopen("fyp_loader.so", RTLD_NOW);
	}
	addr_curl_easy_cleanup = dlsym(handle, "curl_easy_cleanup");
	addr_curl_easy_duphandle = dlsym(handle, "curl_easy_duphandle");
	addr_curl_easy_escape = dlsym(handle, "curl_easy_escape");
	addr_curl_easy_getinfo = dlsym(handle, "curl_easy_getinfo");
	addr_curl_easy_init = dlsym(handle, "curl_easy_init");
	addr_curl_easy_option_by_id = dlsym(handle, "curl_easy_option_by_id");
	addr_curl_easy_option_by_name = dlsym(handle, "curl_easy_option_by_name");
	addr_curl_easy_option_next = dlsym(handle, "curl_easy_option_next");
	addr_curl_easy_pause = dlsym(handle, "curl_easy_pause");
	addr_curl_easy_perform = dlsym(handle, "curl_easy_perform");
	addr_curl_easy_recv = dlsym(handle, "curl_easy_recv");
	addr_curl_easy_reset = dlsym(handle, "curl_easy_reset");
	addr_curl_easy_send = dlsym(handle, "curl_easy_send");
	addr_curl_easy_setopt = dlsym(handle, "curl_easy_setopt");
	addr_curl_easy_strerror = dlsym(handle, "curl_easy_strerror");
	addr_curl_easy_unescape = dlsym(handle, "curl_easy_unescape");
	addr_curl_easy_upkeep = dlsym(handle, "curl_easy_upkeep");
	addr_curl_escape = dlsym(handle, "curl_escape");
	addr_curl_formadd = dlsym(handle, "curl_formadd");
	addr_curl_formfree = dlsym(handle, "curl_formfree");
	addr_curl_formget = dlsym(handle, "curl_formget");
	addr_curl_free = dlsym(handle, "curl_free");
	addr_curl_getdate = dlsym(handle, "curl_getdate");
	addr_curl_getenv = dlsym(handle, "curl_getenv");
	addr_curl_global_cleanup = dlsym(handle, "curl_global_cleanup");
	addr_curl_global_init = dlsym(handle, "curl_global_init");
	addr_curl_global_init_mem = dlsym(handle, "curl_global_init_mem");
	addr_curl_global_sslset = dlsym(handle, "curl_global_sslset");
	addr_curl_maprintf = dlsym(handle, "curl_maprintf");
	addr_curl_mfprintf = dlsym(handle, "curl_mfprintf");
	addr_curl_mime_addpart = dlsym(handle, "curl_mime_addpart");
	addr_curl_mime_data = dlsym(handle, "curl_mime_data");
	addr_curl_mime_data_cb = dlsym(handle, "curl_mime_data_cb");
	addr_curl_mime_encoder = dlsym(handle, "curl_mime_encoder");
	addr_curl_mime_filedata = dlsym(handle, "curl_mime_filedata");
	addr_curl_mime_filename = dlsym(handle, "curl_mime_filename");
	addr_curl_mime_free = dlsym(handle, "curl_mime_free");
	addr_curl_mime_headers = dlsym(handle, "curl_mime_headers");
	addr_curl_mime_init = dlsym(handle, "curl_mime_init");
	addr_curl_mime_name = dlsym(handle, "curl_mime_name");
	addr_curl_mime_subparts = dlsym(handle, "curl_mime_subparts");
	addr_curl_mime_type = dlsym(handle, "curl_mime_type");
	addr_curl_mprintf = dlsym(handle, "curl_mprintf");
	addr_curl_msnprintf = dlsym(handle, "curl_msnprintf");
	addr_curl_msprintf = dlsym(handle, "curl_msprintf");
	addr_curl_multi_add_handle = dlsym(handle, "curl_multi_add_handle");
	addr_curl_multi_assign = dlsym(handle, "curl_multi_assign");
	addr_curl_multi_cleanup = dlsym(handle, "curl_multi_cleanup");
	addr_curl_multi_fdset = dlsym(handle, "curl_multi_fdset");
	addr_curl_multi_info_read = dlsym(handle, "curl_multi_info_read");
	addr_curl_multi_init = dlsym(handle, "curl_multi_init");
	addr_curl_multi_perform = dlsym(handle, "curl_multi_perform");
	addr_curl_multi_poll = dlsym(handle, "curl_multi_poll");
	addr_curl_multi_remove_handle = dlsym(handle, "curl_multi_remove_handle");
	addr_curl_multi_setopt = dlsym(handle, "curl_multi_setopt");
	addr_curl_multi_socket = dlsym(handle, "curl_multi_socket");
	addr_curl_multi_socket_action = dlsym(handle, "curl_multi_socket_action");
	addr_curl_multi_socket_all = dlsym(handle, "curl_multi_socket_all");
	addr_curl_multi_strerror = dlsym(handle, "curl_multi_strerror");
	addr_curl_multi_timeout = dlsym(handle, "curl_multi_timeout");
	addr_curl_multi_wait = dlsym(handle, "curl_multi_wait");
	addr_curl_multi_wakeup = dlsym(handle, "curl_multi_wakeup");
	addr_curl_mvaprintf = dlsym(handle, "curl_mvaprintf");
	addr_curl_mvfprintf = dlsym(handle, "curl_mvfprintf");
	addr_curl_mvprintf = dlsym(handle, "curl_mvprintf");
	addr_curl_mvsnprintf = dlsym(handle, "curl_mvsnprintf");
	addr_curl_mvsprintf = dlsym(handle, "curl_mvsprintf");
	addr_curl_pushheader_byname = dlsym(handle, "curl_pushheader_byname");
	addr_curl_pushheader_bynum = dlsym(handle, "curl_pushheader_bynum");
	addr_curl_share_cleanup = dlsym(handle, "curl_share_cleanup");
	addr_curl_share_init = dlsym(handle, "curl_share_init");
	addr_curl_share_setopt = dlsym(handle, "curl_share_setopt");
	addr_curl_share_strerror = dlsym(handle, "curl_share_strerror");
	addr_curl_slist_append = dlsym(handle, "curl_slist_append");
	addr_curl_slist_free_all = dlsym(handle, "curl_slist_free_all");
	addr_curl_strequal = dlsym(handle, "curl_strequal");
	addr_curl_strnequal = dlsym(handle, "curl_strnequal");
	addr_curl_unescape = dlsym(handle, "curl_unescape");
	addr_curl_url = dlsym(handle, "curl_url");
	addr_curl_url_cleanup = dlsym(handle, "curl_url_cleanup");
	addr_curl_url_dup = dlsym(handle, "curl_url_dup");
	addr_curl_url_get = dlsym(handle, "curl_url_get");
	addr_curl_url_set = dlsym(handle, "curl_url_set");
	addr_curl_url_strerror = dlsym(handle, "curl_url_strerror");
	addr_curl_version = dlsym(handle, "curl_version");
	addr_curl_version_info = dlsym(handle, "curl_version_info");
	curl_jmpenv = dlsym(handle, "curl_jmpenv");
	fakelib_payload();
};
void *curl_easy_cleanup() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_easy_cleanup));
};
void *curl_easy_duphandle() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_easy_duphandle));
};
void *curl_easy_escape() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_easy_escape));
};
void *curl_easy_getinfo() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_easy_getinfo));
};
void *curl_easy_init() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_easy_init));
};
void *curl_easy_option_by_id() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_easy_option_by_id));
};
void *curl_easy_option_by_name() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_easy_option_by_name));
};
void *curl_easy_option_next() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_easy_option_next));
};
void *curl_easy_pause() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_easy_pause));
};
void *curl_easy_perform() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_easy_perform));
};
void *curl_easy_recv() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_easy_recv));
};
void *curl_easy_reset() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_easy_reset));
};
void *curl_easy_send() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_easy_send));
};
void *curl_easy_setopt() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_easy_setopt));
};
void *curl_easy_strerror() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_easy_strerror));
};
void *curl_easy_unescape() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_easy_unescape));
};
void *curl_easy_upkeep() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_easy_upkeep));
};
void *curl_escape() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_escape));
};
void *curl_formadd() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_formadd));
};
void *curl_formfree() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_formfree));
};
void *curl_formget() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_formget));
};
void *curl_free() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_free));
};
void *curl_getdate() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_getdate));
};
void *curl_getenv() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_getenv));
};
void *curl_global_cleanup() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_global_cleanup));
};
void *curl_global_init() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_global_init));
};
void *curl_global_init_mem() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_global_init_mem));
};
void *curl_global_sslset() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_global_sslset));
};
void *curl_maprintf() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_maprintf));
};
void *curl_mfprintf() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_mfprintf));
};
void *curl_mime_addpart() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_mime_addpart));
};
void *curl_mime_data() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_mime_data));
};
void *curl_mime_data_cb() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_mime_data_cb));
};
void *curl_mime_encoder() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_mime_encoder));
};
void *curl_mime_filedata() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_mime_filedata));
};
void *curl_mime_filename() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_mime_filename));
};
void *curl_mime_free() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_mime_free));
};
void *curl_mime_headers() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_mime_headers));
};
void *curl_mime_init() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_mime_init));
};
void *curl_mime_name() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_mime_name));
};
void *curl_mime_subparts() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_mime_subparts));
};
void *curl_mime_type() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_mime_type));
};
void *curl_mprintf() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_mprintf));
};
void *curl_msnprintf() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_msnprintf));
};
void *curl_msprintf() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_msprintf));
};
void *curl_multi_add_handle() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_multi_add_handle));
};
void *curl_multi_assign() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_multi_assign));
};
void *curl_multi_cleanup() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_multi_cleanup));
};
void *curl_multi_fdset() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_multi_fdset));
};
void *curl_multi_info_read() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_multi_info_read));
};
void *curl_multi_init() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_multi_init));
};
void *curl_multi_perform() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_multi_perform));
};
void *curl_multi_poll() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_multi_poll));
};
void *curl_multi_remove_handle() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_multi_remove_handle));
};
void *curl_multi_setopt() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_multi_setopt));
};
void *curl_multi_socket() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_multi_socket));
};
void *curl_multi_socket_action() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_multi_socket_action));
};
void *curl_multi_socket_all() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_multi_socket_all));
};
void *curl_multi_strerror() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_multi_strerror));
};
void *curl_multi_timeout() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_multi_timeout));
};
void *curl_multi_wait() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_multi_wait));
};
void *curl_multi_wakeup() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_multi_wakeup));
};
void *curl_mvaprintf() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_mvaprintf));
};
void *curl_mvfprintf() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_mvfprintf));
};
void *curl_mvprintf() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_mvprintf));
};
void *curl_mvsnprintf() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_mvsnprintf));
};
void *curl_mvsprintf() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_mvsprintf));
};
void *curl_pushheader_byname() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_pushheader_byname));
};
void *curl_pushheader_bynum() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_pushheader_bynum));
};
void *curl_share_cleanup() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_share_cleanup));
};
void *curl_share_init() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_share_init));
};
void *curl_share_setopt() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_share_setopt));
};
void *curl_share_strerror() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_share_strerror));
};
void *curl_slist_append() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_slist_append));
};
void *curl_slist_free_all() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_slist_free_all));
};
void *curl_strequal() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_strequal));
};
void *curl_strnequal() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_strnequal));
};
void *curl_unescape() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_unescape));
};
void *curl_url() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_url));
};
void *curl_url_cleanup() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_url_cleanup));
};
void *curl_url_dup() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_url_dup));
};
void *curl_url_get() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_url_get));
};
void *curl_url_set() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_url_set));
};
void *curl_url_strerror() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_url_strerror));
};
void *curl_version() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_version));
};
void *curl_version_info() {
	asm volatile ("pop %%rbp\n\tjmp *%0" : : "r" (addr_curl_version_info));
};
