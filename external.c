//
//    gchat_linux An un-official Google Chat client for linux
//
//    Copyright (C) 2018  Samith Sandanayake <samithdisal@gmail.com>
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
#include "external.h"

gboolean open_in_external_browser(const gchar* url) {
    GString *str = g_string_new(NULL);
    g_string_printf(str, "xdg-open %s", url);
    system(str->str);
    g_string_free(str, TRUE);
    return TRUE;
}

