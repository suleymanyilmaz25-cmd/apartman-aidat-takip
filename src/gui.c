#include "gui.h"
#include "database.h"
#include "payment.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static GtkWidget *main_window = NULL;

/* GUI başlat */
int gui_init(int *argc, char **argv) {
    gtk_init(argc, argv);
    return 0;
}

/* GTK+ ana döngüsünü çalıştır */
void gui_run() {
    gtk_main();
}

/* GTK+ uygulamasını kapat */
void gui_quit() {
    gtk_main_quit();
}

/* Ana pencereyi oluştur */
GtkWidget* gui_create_main_window() {
    /* Ana pencere */
    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "🏢 Apartman Aidat Takip Sistemi");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 600);
    gtk_container_set_border_width(GTK_CONTAINER(main_window), 10);
    
    /* Ana pencere kapatıldığında uygulamayı kapat */
    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    /* Ana kutu (dikey) */
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(main_window), main_box);
    
    /* Başlık */
    GtkWidget *header = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(header), 
        "<big><b>Apartman Aidat Takip Sistemi</b></big>\n<small>v1.0 - GTK+ ve SQLite3</small>");
    gtk_box_pack_start(GTK_BOX(main_box), header, FALSE, FALSE, 0);
    
    /* Ayırıcı */
    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(main_box), separator, FALSE, FALSE, 0);
    
    /* Buton kutusu (yatay) */
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_set_homogeneous(GTK_BOX(button_box), TRUE);
    gtk_box_pack_start(GTK_BOX(main_box), button_box, FALSE, FALSE, 0);
    
    /* Daire Yönetimi Butonu */
    GtkWidget *btn_daire = gtk_button_new_with_label("👥 Daire Yönetimi");
    gtk_box_pack_start(GTK_BOX(button_box), btn_daire, TRUE, TRUE, 0);
    
    /* Aidat Takibi Butonu */
    GtkWidget *btn_aidat = gtk_button_new_with_label("💰 Aidat Takibi");
    gtk_box_pack_start(GTK_BOX(button_box), btn_aidat, TRUE, TRUE, 0);
    
    /* Ödeme Kaydı Butonu */
    GtkWidget *btn_odeme = gtk_button_new_with_label("💳 Ödeme Kaydı");
    gtk_box_pack_start(GTK_BOX(button_box), btn_odeme, TRUE, TRUE, 0);
    
    /* Raporlar Butonu */
    GtkWidget *btn_rapor = gtk_button_new_with_label("📊 Raporlar");
    gtk_box_pack_start(GTK_BOX(button_box), btn_rapor, TRUE, TRUE, 0);
    
    /* Bilgi kutusu */
    GtkWidget *info_frame = gtk_frame_new("ℹ️  Hızlı Bilgiler");
    gtk_box_pack_start(GTK_BOX(main_box), info_frame, TRUE, TRUE, 0);
    
    GtkWidget *info_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(info_frame), info_box);
    
    /* İstatistikler etiketi */
    GtkWidget *stats_label = gtk_label_new(
        "📌 Toplam Daire: 0\n"
        "💵 Toplam Ödenen: 0 TL\n"
        "⚠️  Toplam Borç: 0 TL\n"
        "🔴 Gecikeli Aidatlar: 0"
    );
    gtk_label_set_xalign(GTK_LABEL(stats_label), 0.0);
    gtk_box_pack_start(GTK_BOX(info_box), stats_label, FALSE, FALSE, 0);
    
    /* Alt buton kutusu */
    GtkWidget *bottom_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_set_homogeneous(GTK_BOX(bottom_box), TRUE);
    gtk_box_pack_end(GTK_BOX(main_box), bottom_box, FALSE, FALSE, 0);
    
    /* Yedekle Butonu */
    GtkWidget *btn_backup = gtk_button_new_with_label("💾 Yedekle");
    gtk_box_pack_start(GTK_BOX(bottom_box), btn_backup, TRUE, TRUE, 0);
    
    /* Çıkış Butonu */
    GtkWidget *btn_exit = gtk_button_new_with_label("❌ Çıkış");
    gtk_box_pack_start(GTK_BOX(bottom_box), btn_exit, TRUE, TRUE, 0);
    g_signal_connect(btn_exit, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    
    /* Buton sinyalleri bağla (şimdilik placeholder) */
    g_signal_connect(btn_daire, "clicked", G_CALLBACK(gui_show_success), 
                    (gpointer)"Daire Yönetimi penceresine yönlendiriliyorsunuz...");
    g_signal_connect(btn_aidat, "clicked", G_CALLBACK(gui_show_success),
                    (gpointer)"Aidat Takibi penceresine yönlendiriliyorsunuz...");
    g_signal_connect(btn_odeme, "clicked", G_CALLBACK(gui_show_success),
                    (gpointer)"Ödeme Kaydı penceresine yönlendiriliyorsunuz...");
    g_signal_connect(btn_rapor, "clicked", G_CALLBACK(gui_show_success),
                    (gpointer)"Raporlar penceresine yönlendiriliyorsunuz...");
    
    return main_window;
}

/* Daire yönetimi penceresi */
GtkWidget* gui_create_daire_window() {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Daire Yönetimi");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), main_box);
    
    /* Başlık */
    GtkWidget *title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title), "<big><b>Daire Yönetimi</b></big>");
    gtk_box_pack_start(GTK_BOX(main_box), title, FALSE, FALSE, 0);
    
    /* Form kutusu */
    GtkWidget *form_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(main_box), form_box, FALSE, FALSE, 0);
    
    /* Daire No */
    GtkWidget *label1 = gtk_label_new("Daire No:");
    gtk_label_set_xalign(GTK_LABEL(label1), 0.0);
    gtk_box_pack_start(GTK_BOX(form_box), label1, FALSE, FALSE, 0);
    
    GtkWidget *entry_daire = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(form_box), entry_daire, FALSE, FALSE, 0);
    
    /* Sakin Adı */
    GtkWidget *label2 = gtk_label_new("Sakin Adı:");
    gtk_label_set_xalign(GTK_LABEL(label2), 0.0);
    gtk_box_pack_start(GTK_BOX(form_box), label2, FALSE, FALSE, 0);
    
    GtkWidget *entry_saki = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(form_box), entry_saki, FALSE, FALSE, 0);
    
    /* Telefon */
    GtkWidget *label3 = gtk_label_new("Telefon:");
    gtk_label_set_xalign(GTK_LABEL(label3), 0.0);
    gtk_box_pack_start(GTK_BOX(form_box), label3, FALSE, FALSE, 0);
    
    GtkWidget *entry_telefon = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(form_box), entry_telefon, FALSE, FALSE, 0);
    
    /* Email */
    GtkWidget *label4 = gtk_label_new("Email:");
    gtk_label_set_xalign(GTK_LABEL(label4), 0.0);
    gtk_box_pack_start(GTK_BOX(form_box), label4, FALSE, FALSE, 0);
    
    GtkWidget *entry_email = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(form_box), entry_email, FALSE, FALSE, 0);
    
    /* Buton kutusu */
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_set_homogeneous(GTK_BOX(button_box), TRUE);
    gtk_box_pack_start(GTK_BOX(main_box), button_box, FALSE, FALSE, 0);
    
    GtkWidget *btn_ekle = gtk_button_new_with_label("Ekle");
    gtk_box_pack_start(GTK_BOX(button_box), btn_ekle, TRUE, TRUE, 0);
    
    GtkWidget *btn_guncelle = gtk_button_new_with_label("Güncelle");
    gtk_box_pack_start(GTK_BOX(button_box), btn_guncelle, TRUE, TRUE, 0);
    
    GtkWidget *btn_sil = gtk_button_new_with_label("Sil");
    gtk_box_pack_start(GTK_BOX(button_box), btn_sil, TRUE, TRUE, 0);
    
    GtkWidget *btn_kapat = gtk_button_new_with_label("Kapat");
    gtk_box_pack_start(GTK_BOX(button_box), btn_kapat, TRUE, TRUE, 0);
    g_signal_connect(btn_kapat, "clicked", G_CALLBACK(gtk_widget_destroy), window);
    
    return window;
}

/* Aidat takibi penceresi */
GtkWidget* gui_create_aidat_window() {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Aidat Takibi");
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 500);
    
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), main_box);
    
    /* Başlık */
    GtkWidget *title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title), "<big><b>Aidat Takibi</b></big>");
    gtk_box_pack_start(GTK_BOX(main_box), title, FALSE, FALSE, 0);
    
    /* Tablo (Tree View) */
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), 
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(main_box), scrolled, TRUE, TRUE, 0);
    
    GtkListStore *store = gtk_list_store_new(5, G_TYPE_INT, G_TYPE_STRING, 
                                              G_TYPE_STRING, G_TYPE_DOUBLE, G_TYPE_STRING);
    GtkWidget *tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    gtk_container_add(GTK_CONTAINER(scrolled), tree);
    
    /* Sütunlar */
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tree), -1,
                                                 "Daire", renderer, "text", 1, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tree), -1,
                                                 "Ay/Yıl", renderer, "text", 2, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tree), -1,
                                                 "Tutar (TL)", renderer, "text", 3, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tree), -1,
                                                 "Durum", renderer, "text", 4, NULL);
    
    /* Buton kutusu */
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_set_homogeneous(GTK_BOX(button_box), TRUE);
    gtk_box_pack_start(GTK_BOX(main_box), button_box, FALSE, FALSE, 0);
    
    GtkWidget *btn_yenile = gtk_button_new_with_label("🔄 Yenile");
    gtk_box_pack_start(GTK_BOX(button_box), btn_yenile, TRUE, TRUE, 0);
    
    GtkWidget *btn_kapat = gtk_button_new_with_label("Kapat");
    gtk_box_pack_start(GTK_BOX(button_box), btn_kapat, TRUE, TRUE, 0);
    g_signal_connect(btn_kapat, "clicked", G_CALLBACK(gtk_widget_destroy), window);
    
    return window;
}

/* Ödeme kaydı penceresi */
GtkWidget* gui_create_odeme_window() {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Ödeme Kaydı");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), main_box);
    
    /* Başlık */
    GtkWidget *title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title), "<big><b>Ödeme Kaydı</b></big>");
    gtk_box_pack_start(GTK_BOX(main_box), title, FALSE, FALSE, 0);
    
    /* Form kutusu */
    GtkWidget *form_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(main_box), form_box, FALSE, FALSE, 0);
    
    /* Daire Seçimi */
    GtkWidget *label1 = gtk_label_new("Daire Seçin:");
    gtk_label_set_xalign(GTK_LABEL(label1), 0.0);
    gtk_box_pack_start(GTK_BOX(form_box), label1, FALSE, FALSE, 0);
    
    GtkWidget *combo_daire = gtk_combo_box_text_new();
    gtk_box_pack_start(GTK_BOX(form_box), combo_daire, FALSE, FALSE, 0);
    
    /* Aidat Seçimi */
    GtkWidget *label2 = gtk_label_new("Aidat Seçin:");
    gtk_label_set_xalign(GTK_LABEL(label2), 0.0);
    gtk_box_pack_start(GTK_BOX(form_box), label2, FALSE, FALSE, 0);
    
    GtkWidget *combo_aidat = gtk_combo_box_text_new();
    gtk_box_pack_start(GTK_BOX(form_box), combo_aidat, FALSE, FALSE, 0);
    
    /* Ödeme Tutarı */
    GtkWidget *label3 = gtk_label_new("Ödeme Tutarı (TL):");
    gtk_label_set_xalign(GTK_LABEL(label3), 0.0);
    gtk_box_pack_start(GTK_BOX(form_box), label3, FALSE, FALSE, 0);
    
    GtkWidget *entry_tutar = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(form_box), entry_tutar, FALSE, FALSE, 0);
    
    /* Ödeme Yöntemi */
    GtkWidget *label4 = gtk_label_new("Ödeme Yöntemi:");
    gtk_label_set_xalign(GTK_LABEL(label4), 0.0);
    gtk_box_pack_start(GTK_BOX(form_box), label4, FALSE, FALSE, 0);
    
    GtkWidget *combo_yontem = gtk_combo_box_text_new();
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_yontem), NULL, "Nakit");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_yontem), NULL, "Çek");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_yontem), NULL, "EFT");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_yontem), NULL, "Kredi Kartı");
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_yontem), 0);
    gtk_box_pack_start(GTK_BOX(form_box), combo_yontem, FALSE, FALSE, 0);
    
    /* Buton kutusu */
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_set_homogeneous(GTK_BOX(button_box), TRUE);
    gtk_box_pack_start(GTK_BOX(main_box), button_box, FALSE, FALSE, 0);
    
    GtkWidget *btn_kaydet = gtk_button_new_with_label("💾 Kaydet");
    gtk_box_pack_start(GTK_BOX(button_box), btn_kaydet, TRUE, TRUE, 0);
    
    GtkWidget *btn_kapat = gtk_button_new_with_label("Kapat");
    gtk_box_pack_start(GTK_BOX(button_box), btn_kapat, TRUE, TRUE, 0);
    g_signal_connect(btn_kapat, "clicked", G_CALLBACK(gtk_widget_destroy), window);
    
    return window;
}

/* Raporlar penceresi */
GtkWidget* gui_create_rapor_window() {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Raporlar");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    
    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), main_box);
    
    /* Başlık */
    GtkWidget *title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title), "<big><b>Raporlar ve İstatistikler</b></big>");
    gtk_box_pack_start(GTK_BOX(main_box), title, FALSE, FALSE, 0);
    
    /* Notebook (sekmeler) */
    GtkWidget *notebook = gtk_notebook_new();
    gtk_box_pack_start(GTK_BOX(main_box), notebook, TRUE, TRUE, 0);
    
    /* Sekme 1: Genel İstatistikler */
    GtkWidget *stats_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *stats_label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(stats_label),
        "<b>Genel İstatistikler</b>\n\n"
        "📌 Toplam Daire Sayısı: 0\n"
        "💰 Toplam Ödenen Tutar: 0 TL\n"
        "⚠️  Toplam Borç: 0 TL\n"
        "🔴 Gecikeli Aidat Sayısı: 0\n"
    );
    gtk_label_set_xalign(GTK_LABEL(stats_label), 0.0);
    gtk_box_pack_start(GTK_BOX(stats_box), stats_label, FALSE, FALSE, 10);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), stats_box, 
                             gtk_label_new("📊 İstatistikler"));
    
    /* Sekme 2: Ödenmemiş Aidatlar */
    GtkWidget *unpaid_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(unpaid_box), scrolled, TRUE, TRUE, 0);
    
    GtkListStore *store = gtk_list_store_new(3, G_TYPE_STRING, 
                                              G_TYPE_STRING, G_TYPE_DOUBLE);
    GtkWidget *tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    gtk_container_add(GTK_CONTAINER(scrolled), tree);
    
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tree), -1,
                                                 "Daire", renderer, "text", 0, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tree), -1,
                                                 "Ay/Yıl", renderer, "text", 1, NULL);
    gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(tree), -1,
                                                 "Kalan (TL)", renderer, "text", 2, NULL);
    
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), unpaid_box, 
                             gtk_label_new("⚠️  Ödenmemiş Aidatlar"));
    
    /* Kapat Butonu */
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_end(GTK_BOX(main_box), button_box, FALSE, FALSE, 0);
    
    GtkWidget *btn_yenile = gtk_button_new_with_label("🔄 Yenile");
    gtk_box_pack_start(GTK_BOX(button_box), btn_yenile, FALSE, FALSE, 0);
    
    GtkWidget *btn_kapat = gtk_button_new_with_label("Kapat");
    gtk_box_pack_end(GTK_BOX(button_box), btn_kapat, FALSE, FALSE, 0);
    g_signal_connect(btn_kapat, "clicked", G_CALLBACK(gtk_widget_destroy), window);
    
    return window;
}

/* Mesaj kutuları */
void gui_show_message(GtkWidget *parent, const char *title, 
                     const char *message, GtkMessageType type) {
    GtkWidget *dialog = gtk_message_dialog_new(parent ? GTK_WINDOW(parent) : NULL,
                                                GTK_DIALOG_MODAL,
                                                type,
                                                GTK_BUTTONS_OK,
                                                "%s", message);
    gtk_window_set_title(GTK_WINDOW(dialog), title);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void gui_show_error(GtkWidget *parent, const char *title, const char *message) {
    gui_show_message(parent, title, message, GTK_MESSAGE_ERROR);
}

void gui_show_success(GtkWidget *parent, const char *message) {
    gui_show_message(parent, "Başarılı", message, GTK_MESSAGE_INFO);
}
