#include "define/declarefunc.h"
static void init_main_menu_view(GtkApplication *app)
{
    app_widgets.window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(app_widgets.window), "IP CATALOGUE");
    gtk_window_set_default_size(GTK_WINDOW(app_widgets.window), 300, 300);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_window_set_child(GTK_WINDOW(app_widgets.window), vbox);

    GtkWidget *button_add_ip = gtk_button_new_with_label("Add IP");
    g_signal_connect(button_add_ip, "clicked", G_CALLBACK(open_add_ip_view), app);
    gtk_box_append(GTK_BOX(vbox), button_add_ip);

    GtkWidget *button_delete_ip = gtk_button_new_with_label("Delete IP");
    g_signal_connect(button_delete_ip, "clicked", G_CALLBACK(open_delete_ip_view), app);
    gtk_box_append(GTK_BOX(vbox), button_delete_ip);

    GtkWidget *button_list_all_ip = gtk_button_new_with_label("List All IP");
    g_signal_connect(button_list_all_ip, "clicked", G_CALLBACK(open_list_all_ip_view), app);
    gtk_box_append(GTK_BOX(vbox), button_list_all_ip);

    GtkWidget *button_search_ip = gtk_button_new_with_label("Search IP");
    g_signal_connect(button_search_ip, "clicked", G_CALLBACK(open_search_ip_view), app);
    gtk_box_append(GTK_BOX(vbox), button_search_ip);

    gtk_widget_show(GTK_WIDGET(app_widgets.window));

}
void open_add_ip_view(GtkWidget *widget, gpointer app) {
    GtkWidget *window = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(window), "Add IP");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_window_set_child(GTK_WINDOW(window), vbox);

    GtkWidget *entry_ip = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_ip), "IP Address");
    gtk_box_append(GTK_BOX(vbox), entry_ip);

    GtkWidget *entry_mask = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_mask), "Mask");
    gtk_box_append(GTK_BOX(vbox), entry_mask);

    GtkWidget *button_add = gtk_button_new_with_label("Add");

    // Création d'une structure pour stocker les données d'entrée
    IpInputData *input_data = g_new0(IpInputData, 1);
    input_data->entry_ip = entry_ip;
    input_data->entry_mask = entry_mask;

    g_signal_connect(button_add, "clicked", G_CALLBACK(add_ip_callback), input_data);
    gtk_box_append(GTK_BOX(vbox), button_add);

    gtk_widget_show(GTK_WIDGET(window));
}
void open_delete_ip_view(GtkWidget *widget, gpointer app)
{
    GtkWidget *window = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(window), "Delete IP");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_window_set_child(GTK_WINDOW(window), vbox);

    GtkWidget *entry_ip = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_ip), "IP Address");
    gtk_box_append(GTK_BOX(vbox), entry_ip);

    gtk_box_append(GTK_BOX(vbox), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL));

    GtkWidget *entry_mask = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_mask), "Mask");
    gtk_box_append(GTK_BOX(vbox), entry_mask);

    IpInputData *input_data = g_new0(IpInputData, 1);
    input_data->entry_ip = entry_ip;
    input_data->entry_mask = entry_mask;

    GtkWidget *button_delete = gtk_button_new_with_label("Delete");
    g_signal_connect(button_delete, "clicked", G_CALLBACK(delete_ip_callback), input_data);
    gtk_box_append(GTK_BOX(vbox), button_delete);

    gtk_widget_show(GTK_WIDGET(window));
}
void open_list_all_ip_view(GtkWidget *widget, gpointer app)
{
    GtkWidget *window = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(window), "List All IP");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_window_set_child(GTK_WINDOW(window), vbox);

    GtkWidget *text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_view), FALSE);
    gtk_box_append(GTK_BOX(vbox), text_view);

    sqlite3 *db;
    int rc = sqlite3_open("ip_catalogue.db", &db);
    char *err_msg = 0;
    char sql[200];

    sprintf(sql, "SELECT * FROM Ips");

    sqlite3_stmt *res;
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        return;
    }

    char buffer[200];
    while (sqlite3_step(res) == SQLITE_ROW)
    {
        sprintf(buffer, "\n[%s -- %s]\n", sqlite3_column_text(res, 1), sqlite3_column_text(res, 2));
        GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
        gtk_text_buffer_insert_at_cursor(text_buffer, buffer, -1);
    }

    gtk_widget_show(GTK_WIDGET(window));
}
void open_search_ip_view(GtkWidget *widget, gpointer app)
{
    GtkWidget *window = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(window), "Search IP");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_window_set_child(GTK_WINDOW(window), vbox);

    GtkWidget *entry_ip = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_ip), "IP Address");
    gtk_box_append(GTK_BOX(vbox), entry_ip);

    gtk_box_append(GTK_BOX(vbox), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL));

    GtkWidget *entry_mask = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_mask), "Mask");
    gtk_box_append(GTK_BOX(vbox), entry_mask);

    IpInputData *input_data = g_new0(IpInputData, 1);
    input_data->entry_ip = entry_ip;
    input_data->entry_mask = entry_mask;

    GtkWidget *button_search = gtk_button_new_with_label("Search");
    g_signal_connect(button_search, "clicked", G_CALLBACK(search_ip_callback), input_data);
    gtk_box_append(GTK_BOX(vbox), button_search);

    gtk_widget_show(GTK_WIDGET(window));
}
/*void open_convert_ip_to_binary_view(GtkWidget *widget, gpointer app)
{
    GtkWidget *window = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(window), "Convert IP to Binary");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_window_set_child(GTK_WINDOW(window), vbox);

    GtkWidget *entry_ip = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_ip), "IP Address");
    gtk_box_append(GTK_BOX(vbox), entry_ip);

    GtkWidget *button_convert = gtk_button_new_with_label("Convert");
    g_signal_connect(button_convert, "clicked", G_CALLBACK(convert_ip_to_binary_callback), entry_ip);
    gtk_box_append(GTK_BOX(vbox), button_convert);

    gtk_widget_show(GTK_WIDGET(window));
}*/
static void activate(GtkApplication *app, gpointer user_data)
{
    init_main_menu_view(app);
}
int main (int argc, char **argv) {
    //init database
    char *db_path = "ip_catalogue.db";
    int rc = init_db(db_path);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to initialize database\n");
        return EXIT_FAILURE;
    }

    GtkApplication *app = gtk_application_new("com.crampter.ipcatalogue", G_APPLICATION_DEFAULT_FLAGS);
    if (!app) {
        fprintf(stderr, "Failed to initialize GTK application\n");
        return EXIT_FAILURE;
    }

    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
