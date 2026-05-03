#ifndef UTILS_H
#define UTILS_H

#include <time.h>

/* Tarih/Saat fonksiyonları */
char* utils_format_date(time_t timestamp, const char *format);
time_t utils_string_to_time(const char *date_string);
int utils_get_current_month();
int utils_get_current_year();

/* String işlemleri */
char* utils_trim(char *str);
int utils_is_empty(const char *str);
char* utils_to_uppercase(char *str);
char* utils_to_lowercase(char *str);

/* Dosya işlemleri */
int utils_file_exists(const char *filename);
int utils_backup_database(const char *source, const char *dest);

/* Doğrulama */
int utils_is_valid_email(const char *email);
int utils_is_valid_phone(const char *phone);
int utils_is_valid_number(const char *str);

/* Bellek yönetimi */
void utils_free_daire_array(void *arr, int count);
void utils_free_aidat_array(void *arr, int count);
void utils_free_odeme_array(void *arr, int count);

#endif /* UTILS_H */
