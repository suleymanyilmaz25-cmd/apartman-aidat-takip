#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "database.h"
#include "gui.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    printf("🏢 Apartman Aidat Takip Sistemi başlatılıyor...\n");
    
    /* Veritabanını başlat */
    if (db_init() != 0) {
        fprintf(stderr, "❌ Veritabanı başlatılamadı!\n");
        return 1;
    }
    printf("✅ Veritabanı başlatıldı.\n");
    
    /* GTK+ arayüzünü başlat */
    if (gui_init(&argc, argv) != 0) {
        fprintf(stderr, "❌ GTK+ arayüzü başlatılamadı!\n");
        return 1;
    }
    printf("✅ GTK+ arayüzü başlatıldı.\n");
    
    /* Ana pencereyi oluştur ve göster */
    GtkWidget *window = gui_create_main_window();
    gtk_widget_show_all(window);
    
    printf("🎯 Uygulama çalışıyor...\n\n");
    
    /* GTK+ ana döngüsü */
    gtk_main();
    
    /* Temizlik */
    gui_quit();
    db_close();
    
    printf("👋 Uygulama kapatıldı.\n");
    return 0;
}
