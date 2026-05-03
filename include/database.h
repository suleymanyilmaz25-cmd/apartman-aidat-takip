#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <time.h>

/* Veritabanı yapıları */

typedef struct {
    int id;
    char daire_no[20];
    char saki_adi[100];
    char telefon[20];
    char email[100];
    char durum[20];
    time_t olusturma_tarihi;
} Daire;

typedef struct {
    int id;
    int daire_id;
    char ay[20];
    int yil;
    double tutar;
    time_t olusturma_tarihi;
} Aidat;

typedef struct {
    int id;
    int aidat_id;
    double tutar;
    time_t odeme_tarihi;
    char odeme_yontemi[50];
    char aciklama[200];
} Odeme;

/* Veritabanı fonksiyonları */

int db_init();
int db_close();

/* Daire işlemleri */
int db_daire_ekle(const char *daire_no, const char *saki_adi, 
                  const char *telefon, const char *email);
int db_daire_sil(int daire_id);
int db_daire_guncelle(int daire_id, const char *saki_adi, 
                      const char *telefon, const char *email);
Daire* db_daire_getir(int daire_id);
Daire* db_tumDaireleriGetir(int *sayac);

/* Aidat işlemleri */
int db_aidat_ekle(int daire_id, const char *ay, int yil, double tutar);
int db_aidat_sil(int aidat_id);
Aidat* db_tumAidatlariGetir(int *sayac);
Aidat* db_dairinAidatlari(int daire_id, int *sayac);

/* Ödeme işlemleri */
int db_odeme_ekle(int aidat_id, double tutar, const char *odeme_yontemi, 
                  const char *aciklama);
Odeme* db_aidatinOdemeleri(int aidat_id, int *sayac);

/* İstatistik ve Raporlar */
double db_toplam_odenen(int daire_id);
double db_toplam_borcu(int daire_id);
int db_gecikeli_aidat_sayisi(int daire_id);

#endif /* DATABASE_H */
