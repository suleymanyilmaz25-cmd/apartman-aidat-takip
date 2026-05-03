#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>

/* GUI başlatma */
int gui_init(int *argc, char **argv);
void gui_run();
void gui_quit();

/* Pencereler */
GtkWidget* gui_create_main_window();
GtkWidget* gui_create_daire_window();
GtkWidget* gui_create_aidat_window();
GtkWidget* gui_create_odeme_window();
GtkWidget* gui_create_rapor_window();

/* Yardımcı fonksiyonlar */
void gui_show_message(GtkWidget *parent, const char *title, 
                      const char *message, GtkMessageType type);
void gui_show_error(GtkWidget *parent, const char *title, 
                    const char *message);
void gui_show_success(GtkWidget *parent, const char *message);

#endif /* GUI_H */
