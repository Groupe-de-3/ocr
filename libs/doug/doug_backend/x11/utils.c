#include "utils.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

xcb_atom_t doug_natom(xcb_connection_t *conn, const char *at) {
    xcb_generic_error_t     *err;
    xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(
        conn, xcb_intern_atom(conn, false, (uint16_t)strlen(at), at), &err
    );
    if (err != NULL)
        errx(1, "No non ono");
    xcb_atom_t atom = reply->atom;
    free(reply);
    return atom;
}

