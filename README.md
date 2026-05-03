# Apartman Aidat Takip Sistemi

GTK+ ile yazılmış masaüstü uygulaması olarak apartman aidat yönetimi ve takibi.

## Özellikler

- ✅ Daire/Apartman sahibi yönetimi
- ✅ Aylık aidat takibi
- ✅ Ödeme takibi (ödenen/ödenmemiş)
- ✅ Ödeme tarihleri ve gecikmeler
- ✅ Gecikmeli ödemeler için hatırlatma
- ✅ Raporlar ve istatistikler
- ✅ Arama ve filtreleme
- ✅ Veri yedekleme

## Teknolojiler

- **Dil:** C
- **GUI:** GTK+ 3.0
- **Veritabanı:** SQLite3
- **Build:** Make/Makefile

## Kurulum

### Gerekli Paketler (Ubuntu/Debian)

```bash
sudo apt-get install build-essential
sudo apt-get install libgtk-3-dev
sudo apt-get install libsqlite3-dev
```

### MacOS

```bash
brew install gtk+3
brew install sqlite3
```

### Windows

GTK+ ve SQLite3 geliştirme kütüphanelerini indirin ve PATH'e ekleyin.

## Derleme ve Çalıştırma

```bash
make
./bin/apartman-aidat-takip
```

Temizleme:
```bash
make clean
```

## Proje Yapısı

```
apartman-aidat-takip/
├── src/
│   ├── main.c              # Ana program
│   ├── gui.c / gui.h       # GTK+ arayüzü
│   ├── database.c / database.h  # SQLite işlemleri
│   ├── payment.c / payment.h    # Ödeme yönetimi
│   └── utils.c / utils.h   # Yardımcı fonksiyonlar
├── include/                # Header dosyaları
├── data/                   # Veritabanı dosyaları
├── Makefile               # Build dosyası
└── README.md              # Bu dosya
```

## Kullanım

1. **Daire Ekle:** Yeni daire/apartman sahibi bilgilerini ekleyin
2. **Aidat Takibi:** Aylık aidatları girin
3. **Ödeme Kayıt:** Yapılan ödemeleri kaydedin
4. **Raporlar:** Ödeme istatistiklerini ve gecikmeli ödmeleri görüntüleyin
5. **Hatırlatmalar:** Ödenmemiş aidatlar için hatırlatma alın

## Veritabanı Şeması

### daireler tablosu
```sql
CREATE TABLE daireler (
    id INTEGER PRIMARY KEY,
    daire_no TEXT UNIQUE,
    saki_adi TEXT,
    telefon TEXT,
    email TEXT,
    durum TEXT,
    olusturma_tarihi DATETIME
);
```

### aidatlar tablosu
```sql
CREATE TABLE aidatlar (
    id INTEGER PRIMARY KEY,
    daire_id INTEGER,
    ay TEXT,
    yil INTEGER,
    tutar REAL,
    olusturma_tarihi DATETIME,
    FOREIGN KEY(daire_id) REFERENCES daireler(id)
);
```

### odemeler tablosu
```sql
CREATE TABLE odemeler (
    id INTEGER PRIMARY KEY,
    aidat_id INTEGER,
    tutar REAL,
    odeme_tarihi DATETIME,
    odeme_yontemi TEXT,
    aciklama TEXT,
    FOREIGN KEY(aidat_id) REFERENCES aidatlar(id)
);
```

## Lisans

MIT License

## Yazar

suleymanyilmaz25-cmd
