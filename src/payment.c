#include "payment.h"
#include <stdio.h>
#include <time.h>

/* Ödeme durumunu getir */
PaymentStatus payment_get_status(int aidat_id) {
    int sayac = 0;
    Odeme *odemeler = db_aidatinOdemeleri(aidat_id, &sayac);
    Aidat *aidat = NULL;
    
    /* Aidatı getir */
    Aidat *tumAidatlar = db_tumAidatlariGetir(&sayac);
    if (tumAidatlar) {
        for (int i = 0; i < sayac; i++) {
            if (tumAidatlar[i].id == aidat_id) {
                aidat = &tumAidatlar[i];
                break;
            }
        }
    }
    
    if (!aidat) {
        return PAYMENT_UNPAID;
    }
    
    /* Toplam ödenen tutarı hesapla */
    double toplam_odenen = 0.0;
    if (odemeler) {
        for (int i = 0; i < sayac; i++) {
            toplam_odenen += odemeler[i].tutar;
        }
    }
    
    /* Durumu belirle */
    if (toplam_odenen <= 0) {
        return PAYMENT_UNPAID;
    } else if (toplam_odenen >= aidat->tutar) {
        return PAYMENT_PAID;
    } else {
        return PAYMENT_PARTIAL;
    }
}

/* Kalan tutarı getir */
double payment_get_remaining(int aidat_id) {
    int sayac = 0;
    Odeme *odemeler = db_aidatinOdemeleri(aidat_id, &sayac);
    
    /* Aidatı getir */
    Aidat *tumAidatlar = db_tumAidatlariGetir(&sayac);
    Aidat *aidat = NULL;
    
    if (tumAidatlar) {
        for (int i = 0; i < sayac; i++) {
            if (tumAidatlar[i].id == aidat_id) {
                aidat = &tumAidatlar[i];
                break;
            }
        }
    }
    
    if (!aidat) {
        return 0.0;
    }
    
    /* Toplam ödenen tutarı hesapla */
    double toplam_odenen = 0.0;
    if (odemeler) {
        for (int i = 0; i < sayac; i++) {
            toplam_odenen += odemeler[i].tutar;
        }
    }
    
    return aidat->tutar - toplam_odenen > 0 ? aidat->tutar - toplam_odenen : 0.0;
}

/* Gecikeli mi kontrol et */
int payment_is_overdue(int aidat_id) {
    /* Bu basit bir kontrol - gerçek uygulamada daha karmaşık tarih mantığı gerekebilir */
    PaymentStatus status = payment_get_status(aidat_id);
    
    if (status == PAYMENT_UNPAID || status == PAYMENT_PARTIAL) {
        /* Eğer borç varsa ve belli bir tarih geçtiyse gecikeli sayılır */
        return 1;
    }
    
    return 0;
}

/* Kaç gün gecikmeli mi */
int payment_days_overdue(int aidat_id) {
    /* Basit hesaplama - gerçek tarih mantığı uygulanmalı */
    if (payment_is_overdue(aidat_id)) {
        return 30;  /* Örnek: 30 gün gecikmeli */
    }
    return 0;
}

/* Durum string'ini getir */
char* payment_get_status_string(PaymentStatus status) {
    switch (status) {
        case PAYMENT_UNPAID:
            return "❌ Ödenmemiş";
        case PAYMENT_PARTIAL:
            return "⚠️  Kısmen Ödendi";
        case PAYMENT_PAID:
            return "✅ Ödendi";
        case PAYMENT_OVERDUE:
            return "🔴 GECİKELİ";
        default:
            return "? Bilinmiyor";
    }
}

/* Dairenin ödeme özetini yazdır */
void payment_print_summary(int daire_id) {
    double toplam_odenen = db_toplam_odenen(daire_id);
    double toplam_borcu = db_toplam_borcu(daire_id);
    int gecikeli = db_gecikeli_aidat_sayisi(daire_id);
    
    printf("\n=== Daire #%d Ödeme Özeti ===\n", daire_id);
    printf("💰 Toplam Ödenen: %.2f TL\n", toplam_odenen);
    printf("⚠️  Toplam Borç: %.2f TL\n", toplam_borcu);
    printf("🔴 Gecikeli Aidatlar: %d\n", gecikeli);
    printf("=============================\n\n");
}
