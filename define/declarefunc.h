//
// Created by Yvann on 21/02/2024.
//

#ifndef DECLARE_FUNC_H
#define DECLARE_FUNC_H

#include "confinc.h"
void add_ip_callback(GtkWidget *widget, gpointer entry);
void delete_ip_callback(GtkWidget *widget, gpointer entry);
void search_ip_callback(GtkWidget *widget, gpointer entry);
void convert_ip_to_binary_callback(GtkWidget *widget, gpointer entry);
void open_add_ip_view(GtkWidget *widget, gpointer entry);
void open_delete_ip_view(GtkWidget *widget, gpointer entry);
void open_list_all_ip_view(GtkWidget *widget, gpointer entry);
void open_search_ip_view(GtkWidget *widget, gpointer entry);
void open_convert_ip_to_binary_view(GtkWidget *widget, gpointer entry);
int init_db(const char *db_path);

typedef struct {
    GtkWidget *entry_ip;
    GtkWidget *entry_mask;
} IpInputData;

typedef struct {
    GtkWidget *window;
    GtkWidget *main_container;
} AppWidgets;

AppWidgets app_widgets;
IpInputData *global_input_data;
#endif //DECLARE_FUNC_H
