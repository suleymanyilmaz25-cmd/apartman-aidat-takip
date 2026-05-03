#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static sqlite3 *db = NULL;
static const char *db_file = "data/apartman.db";

/* Veritabanını başlat */
int db_init() {
    int rc;
    char *errmsg = 0;
    
    /* Veritabanını aç */
    rc = sqlite3_open(db_file, &db);
    if (rc) {
        fprintf(stderr, "❌ Veritabanı açılamadı: %s\n", sqlite3_errmsg(db));
        return 1;
    }
    
    /* Tabloları oluştur */
    const char *sql = 
        "CREATE TABLE IF NOT EXISTS daireler ("
        "  id INTEGER PRIMARY KEY,"
        "  daire_no TEXT UNIQUE,"
        "  saki_adi TEXT,"
        "  telefon TEXT,"
        "  email TEXT,"
        "  durum TEXT,"
        "  olusturma_tarihi DATETIME"
        ");"
        
        "CREATE TABLE IF NOT EXISTS aidatlar ("
        "  id INTEGER PRIMARY KEY,"
        "  daire_id INTEGER,"
        "  ay TEXT,"
        "  yil INTEGER,"
        "  tutar REAL,"
        "  olusturma_tarihi DATETIME,"
        "  FOREIGN KEY(daire_id) REFERENCES daireler(id)"
        ");"
        
        "CREATE TABLE IF NOT EXISTS odemeler ("
        "  id INTEGER PRIMARY KEY,"
        "  aidat_id INTEGER,"
        "  tutar REAL,"
        "  odeme_tarihi DATETIME,"
        "  odeme_yontemi TEXT,"
        "  aciklama TEXT,"
        "  FOREIGN KEY(aidat_id) REFERENCES aidatlar(id)"
        ")";
    
    rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "❌ SQL hatası: %s\n", errmsg);
        sqlite3_free(errmsg);
        return 1;
    }
    
    printf("✅ Veritabanı tabloları oluşturuldu/kontrol edildi.\n");
    return 0;
}

/* Veritabanını kapat */
int db_close() {
    if (db) {
        sqlite3_close(db);
        printf("✅ Veritabanı kapatıldı.\n");
        return 0;
    }
    return 1;
}

/* Daire ekle */
int db_daire_ekle(const char *daire_no, const char *saki_adi,
                  const char *telefon, const char *email) {
    sqlite3_stmt *stmt;
    const char *sql = 
        "INSERT INTO daireler (daire_no, saki_adi, telefon, email, durum, olusturma_tarihi) "
        "VALUES (?, ?, ?, ?, ?, datetime('now'))";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "❌ SQL hazırlanması başarısız: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    
    sqlite3_bind_text(stmt, 1, daire_no, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, saki_adi, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, telefon, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, email, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, "Aktif", -1, SQLITE_STATIC);
    
    int result = sqlite3_step(stmt) == SQLITE_DONE ? 1 : 0;
    sqlite3_finalize(stmt);
    
    if (result) {
        printf("✅ Daire eklendi: %s\n", daire_no);
    } else {
        fprintf(stderr, "❌ Daire eklenirken hata: %s\n", sqlite3_errmsg(db));
    }
    
    return result;
}

/* Daire sil */
int db_daire_sil(int daire_id) {
    const char *sql = "DELETE FROM daireler WHERE id = ?";
    sqlite3_stmt *stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return 0;
    }
    
    sqlite3_bind_int(stmt, 1, daire_id);
    int result = sqlite3_step(stmt) == SQLITE_DONE ? 1 : 0;
    sqlite3_finalize(stmt);
    
    return result;
}

/* Daire güncelle */
int db_daire_guncelle(int daire_id, const char *saki_adi,
                      const char *telefon, const char *email) {
    const char *sql = 
        "UPDATE daireler SET saki_adi = ?, telefon = ?, email = ? WHERE id = ?";
    sqlite3_stmt *stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return 0;
    }
    
    sqlite3_bind_text(stmt, 1, saki_adi, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, telefon, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, email, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, daire_id);
    
    int result = sqlite3_step(stmt) == SQLITE_DONE ? 1 : 0;
    sqlite3_finalize(stmt);
    
    return result;
}

/* Daire getir */
Daire* db_daire_getir(int daire_id) {
    const char *sql = "SELECT id, daire_no, saki_adi, telefon, email, durum, olusturma_tarihi "
                      "FROM daireler WHERE id = ?";
    sqlite3_stmt *stmt;
    Daire *daire = NULL;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, daire_id);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            daire = (Daire *)malloc(sizeof(Daire));
            daire->id = sqlite3_column_int(stmt, 0);
            strcpy(daire->daire_no, (char *)sqlite3_column_text(stmt, 1));
            strcpy(daire->saki_adi, (char *)sqlite3_column_text(stmt, 2));
            strcpy(daire->telefon, (char *)sqlite3_column_text(stmt, 3));
            strcpy(daire->email, (char *)sqlite3_column_text(stmt, 4));
            strcpy(daire->durum, (char *)sqlite3_column_text(stmt, 5));
            daire->olusturma_tarihi = sqlite3_column_int64(stmt, 6);
        }
        
        sqlite3_finalize(stmt);
    }
    
    return daire;
}

/* Tüm daireleri getir */
Daire* db_tumDaireleriGetir(int *sayac) {
    const char *sql = "SELECT id, daire_no, saki_adi, telefon, email, durum, olusturma_tarihi "
                      "FROM daireler ORDER BY daire_no";
    sqlite3_stmt *stmt;
    Daire *daireler = NULL;
    *sayac = 0;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        /* Önce satır sayısını bul */
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            (*sayac)++;
        }
        
        sqlite3_reset(stmt);
        
        if (*sayac > 0) {
            daireler = (Daire *)malloc(sizeof(Daire) * (*sayac));
            int index = 0;
            
            while (sqlite3_step(stmt) == SQLITE_ROW && index < *sayac) {
                daireler[index].id = sqlite3_column_int(stmt, 0);
                strcpy(daireler[index].daire_no, (char *)sqlite3_column_text(stmt, 1));
                strcpy(daireler[index].saki_adi, (char *)sqlite3_column_text(stmt, 2));
                strcpy(daireler[index].telefon, (char *)sqlite3_column_text(stmt, 3));
                strcpy(daireler[index].email, (char *)sqlite3_column_text(stmt, 4));
                strcpy(daireler[index].durum, (char *)sqlite3_column_text(stmt, 5));
                daireler[index].olusturma_tarihi = sqlite3_column_int64(stmt, 6);
                index++;
            }
        }
        
        sqlite3_finalize(stmt);
    }
    
    return daireler;
}

/* Aidat ekle */
int db_aidat_ekle(int daire_id, const char *ay, int yil, double tutar) {
    const char *sql = 
        "INSERT INTO aidatlar (daire_id, ay, yil, tutar, olusturma_tarihi) "
        "VALUES (?, ?, ?, ?, datetime('now'))";
    sqlite3_stmt *stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return 0;
    }
    
    sqlite3_bind_int(stmt, 1, daire_id);
    sqlite3_bind_text(stmt, 2, ay, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, yil);
    sqlite3_bind_double(stmt, 4, tutar);
    
    int result = sqlite3_step(stmt) == SQLITE_DONE ? 1 : 0;
    sqlite3_finalize(stmt);
    
    return result;
}

/* Aidat sil */
int db_aidat_sil(int aidat_id) {
    const char *sql = "DELETE FROM aidatlar WHERE id = ?";
    sqlite3_stmt *stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return 0;
    }
    
    sqlite3_bind_int(stmt, 1, aidat_id);
    int result = sqlite3_step(stmt) == SQLITE_DONE ? 1 : 0;
    sqlite3_finalize(stmt);
    
    return result;
}

/* Tüm aidatları getir */
Aidat* db_tumAidatlariGetir(int *sayac) {
    const char *sql = "SELECT id, daire_id, ay, yil, tutar, olusturma_tarihi "
                      "FROM aidatlar ORDER BY yil DESC, ay DESC";
    sqlite3_stmt *stmt;
    Aidat *aidatlar = NULL;
    *sayac = 0;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            (*sayac)++;
        }
        
        sqlite3_reset(stmt);
        
        if (*sayac > 0) {
            aidatlar = (Aidat *)malloc(sizeof(Aidat) * (*sayac));
            int index = 0;
            
            while (sqlite3_step(stmt) == SQLITE_ROW && index < *sayac) {
                aidatlar[index].id = sqlite3_column_int(stmt, 0);
                aidatlar[index].daire_id = sqlite3_column_int(stmt, 1);
                strcpy(aidatlar[index].ay, (char *)sqlite3_column_text(stmt, 2));
                aidatlar[index].yil = sqlite3_column_int(stmt, 3);
                aidatlar[index].tutar = sqlite3_column_double(stmt, 4);
                aidatlar[index].olusturma_tarihi = sqlite3_column_int64(stmt, 5);
                index++;
            }
        }
        
        sqlite3_finalize(stmt);
    }
    
    return aidatlar;
}

/* Dairenin aidatlarını getir */
Aidat* db_dairinAidatlari(int daire_id, int *sayac) {
    const char *sql = "SELECT id, daire_id, ay, yil, tutar, olusturma_tarihi "
                      "FROM aidatlar WHERE daire_id = ? ORDER BY yil DESC, ay DESC";
    sqlite3_stmt *stmt;
    Aidat *aidatlar = NULL;
    *sayac = 0;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, daire_id);
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            (*sayac)++;
        }
        
        sqlite3_reset(stmt);
        sqlite3_bind_int(stmt, 1, daire_id);
        
        if (*sayac > 0) {
            aidatlar = (Aidat *)malloc(sizeof(Aidat) * (*sayac));
            int index = 0;
            
            while (sqlite3_step(stmt) == SQLITE_ROW && index < *sayac) {
                aidatlar[index].id = sqlite3_column_int(stmt, 0);
                aidatlar[index].daire_id = sqlite3_column_int(stmt, 1);
                strcpy(aidatlar[index].ay, (char *)sqlite3_column_text(stmt, 2));
                aidatlar[index].yil = sqlite3_column_int(stmt, 3);
                aidatlar[index].tutar = sqlite3_column_double(stmt, 4);
                aidatlar[index].olusturma_tarihi = sqlite3_column_int64(stmt, 5);
                index++;
            }
        }
        
        sqlite3_finalize(stmt);
    }
    
    return aidatlar;
}

/* Ödeme ekle */
int db_odeme_ekle(int aidat_id, double tutar, const char *odeme_yontemi,
                  const char *aciklama) {
    const char *sql = 
        "INSERT INTO odemeler (aidat_id, tutar, odeme_tarihi, odeme_yontemi, aciklama) "
        "VALUES (?, ?, datetime('now'), ?, ?)";
    sqlite3_stmt *stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return 0;
    }
    
    sqlite3_bind_int(stmt, 1, aidat_id);
    sqlite3_bind_double(stmt, 2, tutar);
    sqlite3_bind_text(stmt, 3, odeme_yontemi, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, aciklama, -1, SQLITE_STATIC);
    
    int result = sqlite3_step(stmt) == SQLITE_DONE ? 1 : 0;
    sqlite3_finalize(stmt);
    
    return result;
}

/* Aidatın ödemelerini getir */
Odeme* db_aidatinOdemeleri(int aidat_id, int *sayac) {
    const char *sql = "SELECT id, aidat_id, tutar, odeme_tarihi, odeme_yontemi, aciklama "
                      "FROM odemeler WHERE aidat_id = ? ORDER BY odeme_tarihi DESC";
    sqlite3_stmt *stmt;
    Odeme *odemeler = NULL;
    *sayac = 0;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, aidat_id);
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            (*sayac)++;
        }
        
        sqlite3_reset(stmt);
        sqlite3_bind_int(stmt, 1, aidat_id);
        
        if (*sayac > 0) {
            odemeler = (Odeme *)malloc(sizeof(Odeme) * (*sayac));
            int index = 0;
            
            while (sqlite3_step(stmt) == SQLITE_ROW && index < *sayac) {
                odemeler[index].id = sqlite3_column_int(stmt, 0);
                odemeler[index].aidat_id = sqlite3_column_int(stmt, 1);
                odemeler[index].tutar = sqlite3_column_double(stmt, 2);
                odemeler[index].odeme_tarihi = sqlite3_column_int64(stmt, 3);
                strcpy(odemeler[index].odeme_yontemi, (char *)sqlite3_column_text(stmt, 4));
                strcpy(odemeler[index].aciklama, (char *)sqlite3_column_text(stmt, 5));
                index++;
            }
        }
        
        sqlite3_finalize(stmt);
    }
    
    return odemeler;
}

/* Dairenin toplam ödediği tutarı getir */
double db_toplam_odenen(int daire_id) {
    const char *sql = 
        "SELECT SUM(o.tutar) FROM odemeler o "
        "JOIN aidatlar a ON o.aidat_id = a.id "
        "WHERE a.daire_id = ?";
    sqlite3_stmt *stmt;
    double total = 0.0;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, daire_id);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            total = sqlite3_column_double(stmt, 0);
        }
        
        sqlite3_finalize(stmt);
    }
    
    return total;
}

/* Dairenin toplam borcu getir */
double db_toplam_borcu(int daire_id) {
    const char *sql = 
        "SELECT SUM(a.tutar) - SUM(COALESCE(("
        "  SELECT SUM(tutar) FROM odemeler WHERE aidat_id = a.id"
        "), 0)) FROM aidatlar a WHERE a.daire_id = ?";
    sqlite3_stmt *stmt;
    double total = 0.0;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, daire_id);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            total = sqlite3_column_double(stmt, 0);
        }
        
        sqlite3_finalize(stmt);
    }
    
    return total > 0 ? total : 0.0;
}

/* Gecikeli aidat sayısı */
int db_gecikeli_aidat_sayisi(int daire_id) {
    /* Bu fonksiyon, ödemenin yapılmadığı veya kısmen yapıldığı aidatları sayar */
    const char *sql = 
        "SELECT COUNT(*) FROM aidatlar a "
        "WHERE a.daire_id = ? AND ("
        "  SELECT SUM(tutar) FROM odemeler WHERE aidat_id = a.id"
        ") IS NULL OR ("
        "  SELECT SUM(tutar) FROM odemeler WHERE aidat_id = a.id"
        ") < a.tutar";
    sqlite3_stmt *stmt;
    int count = 0;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, daire_id);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }
        
        sqlite3_finalize(stmt);
    }
    
    return count;
}
