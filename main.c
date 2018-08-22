
#include <stdlib.h>
#include <gtk/gtk.h>
#include <webkit2/webkit2.h>


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

static void activate(GApplication *application, gpointer userdata) {

    create_menu(application);

    /* Create the widgets */
    GtkWidget *main_window = gtk_application_window_new(GTK_APPLICATION(application));
    gtk_window_set_deletable(GTK_WINDOW(main_window), FALSE);
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    GtkWidget *web_view = webkit_web_view_new();

    /* Setup notification area */

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