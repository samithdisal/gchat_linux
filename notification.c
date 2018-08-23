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


#include "notification.h"

gboolean show_notification(WebKitWebView *web_view, WebKitNotification *notification, gpointer user_data) {
    GNotification *gn;
    gn = g_notification_new(webkit_notification_get_title(notification));
    g_notification_set_body(gn, webkit_notification_get_body(notification));
    g_notification_set_default_action (gn, "app.focus-window");
    g_notification_set_priority(gn, G_NOTIFICATION_PRIORITY_URGENT);
    g_application_send_notification(G_APPLICATION(user_data), webkit_notification_get_title(notification), gn);
    g_object_unref(gn);
    return FALSE;
}