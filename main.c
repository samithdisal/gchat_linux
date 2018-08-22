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

#include <stdlib.h>
#include <gtk/gtk.h>
#include <webkit2/webkit2.h>


#define COOKIE_FILE ".gchat_ckg"

static void quit_cb(GSimpleAction *action, GVariant *parameter, gpointer user_data) {
    g_application_quit(G_APPLICATION(user_data));
}


const GActionEntry application_actions[] = {
        { "quit", quit_cb },
};

void create_menu(GApplication* application) {
    GMenu *menu;
    g_action_map_add_action_entries(G_ACTION_MAP(application), application_actions,
                                    G_N_ELEMENTS(application_actions), application);
    menu = g_menu_new();
    g_menu_append(menu, "Quit", "app.quit");


    gtk_application_set_app_menu(GTK_APPLICATION(application),
                                 G_MENU_MODEL(menu));
    g_object_unref(menu);
}

static gboolean show_notification(WebKitWebView *web_view, WebKitNotification *notification, gpointer user_data) {
    GNotification *gn;
    gn = g_notification_new(webkit_notification_get_title(notification));
    g_notification_set_body(gn, webkit_notification_get_body(notification));
    g_application_send_notification(G_APPLICATION(user_data), webkit_notification_get_title(notification), gn);
    g_object_unref(gn);
    return FALSE;
}

static gboolean permission_request(WebKitWebView *web_view, WebKitPermissionRequest *request, gpointer user_data) {

    if(WEBKIT_IS_NOTIFICATION_PERMISSION_REQUEST(request)) {
        webkit_permission_request_allow(request);
        return TRUE;
    }
    gchar* permissionRequestType;
    if(WEBKIT_IS_GEOLOCATION_PERMISSION_REQUEST(request)) {
        permissionRequestType = "share geo location";
    } else if(WEBKIT_IS_USER_MEDIA_PERMISSION_REQUEST(request)) {
        permissionRequestType = "access user media";
    } else {
        permissionRequestType = "non classified action";
    }

    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(user_data),
                                                GTK_DIALOG_MODAL,
                                                GTK_MESSAGE_QUESTION,
                                                GTK_BUTTONS_YES_NO,
                                                "Allow permission to %s?", permissionRequestType);
    gtk_widget_show(dialog);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));

    switch (result) {
        case GTK_RESPONSE_YES:
            webkit_permission_request_allow(request);
            break;
        default:
            webkit_permission_request_deny(request);
            break;
    }
    gtk_widget_destroy (dialog);
    return TRUE;
}

static void activate(GApplication *application, gpointer userdata) {

    create_menu(application);

    /* Create the widgets */
    GtkWidget *main_window = gtk_application_window_new(GTK_APPLICATION(application));
    gtk_window_set_deletable(GTK_WINDOW(main_window), FALSE);
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    GtkWidget *web_view = webkit_web_view_new();

    webkit_cookie_manager_set_accept_policy(webkit_web_context_get_cookie_manager(
            webkit_web_view_get_context(WEBKIT_WEB_VIEW(web_view))
            ), WEBKIT_COOKIE_POLICY_ACCEPT_ALWAYS);

    webkit_cookie_manager_set_persistent_storage(
            webkit_web_context_get_cookie_manager(webkit_web_view_get_context(WEBKIT_WEB_VIEW(web_view))),
            COOKIE_FILE,
            WEBKIT_COOKIE_PERSISTENT_STORAGE_SQLITE);

    /* Setup notification area */
    g_signal_connect(web_view, "show-notification", G_CALLBACK(show_notification), application);
    g_signal_connect(web_view, "permission-request", G_CALLBACK(permission_request), main_window);

    /* Place the WebKitWebView in the GtkScrolledWindow */
    gtk_container_add(GTK_CONTAINER(scrolled_window), web_view);
    gtk_container_add(GTK_CONTAINER(main_window), scrolled_window);

    /* Open a webpage */
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(web_view), "https://chat.google.com");

    /* Show */
    gtk_window_set_default_size(GTK_WINDOW(main_window), 1280, 740);
    gtk_window_set_resizable(GTK_WINDOW(main_window), FALSE);
    gtk_window_set_title(GTK_WINDOW(main_window), "Google Chat");
    gtk_widget_show_all(main_window);
}

int main(int argc, char **argv) {
    GtkApplication *application;
    int status;

    application = gtk_application_new("com.github.samithdisal.gchat-linux", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(application, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(application), argc, argv);
    g_object_unref(application);

    return status;
}