//
// Created by samiths on 8/23/18.
//

#include "preferences.h"

void show_preferences_cb(GSimpleAction* action, GVariant* variant, gpointer user_data) {
    GtkWidget* pref_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(pref_window), "Preferences");
    gtk_window_set_modal(GTK_WINDOW(pref_window), TRUE);
    gtk_window_set_default_size(GTK_WINDOW(pref_window), 600, 400);
    gtk_widget_show_all(pref_window);
}