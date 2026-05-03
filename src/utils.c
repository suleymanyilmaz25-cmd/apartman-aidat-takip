#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/* Tarihi formatlı string'e dönüştür */
char* utils_format_date(time_t timestamp, const char *format) {
    static char buffer[100];
    struct tm *timeinfo = localtime(&timestamp);
    strftime(buffer, sizeof(buffer), format, timeinfo);
    return buffer;
}

/* String'i time_t'ye dönüştür */
time_t utils_string_to_time(const char *date_string) {
    struct tm tm_info = {0};
    sscanf(date_string, "%d-%d-%d", &tm_info.tm_year, &tm_info.tm_mon, &tm_info.tm_mday);
    tm_info.tm_year -= 1900;
    tm_info.tm_mon -= 1;
    return mktime(&tm_info);
}

/* Geçerli ayı getir */
int utils_get_current_month() {
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    return timeinfo->tm_mon + 1;
}

/* Geçerli yılı getir */
int utils_get_current_year() {
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    return timeinfo->tm_year + 1900;
}

/* String'in başındaki ve sonundaki boşlukları sil */
char* utils_trim(char *str) {
    if (!str) return str;
    
    char *start = str;
    while (isspace((unsigned char)*start)) start++;
    
    char *end = str + strlen(str) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;
    
    *(end + 1) = '\0';
    return start;
}

/* String boş mu */
int utils_is_empty(const char *str) {
    if (!str) return 1;
    while (*str && isspace((unsigned char)*str)) str++;
    return *str == '\0';
}

/* String'i büyük harfe dönüştür */
char* utils_to_uppercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
    return str;
}

/* String'i küçük harfe dönüştür */
char* utils_to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
    return str;
}

/* Dosya var mı */
int utils_file_exists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

/* Veritabanını yedekle */
int utils_backup_database(const char *source, const char *dest) {
    FILE *src = fopen(source, "rb");
    if (!src) return 0;
    
    FILE *dst = fopen(dest, "wb");
    if (!dst) {
        fclose(src);
        return 0;
    }
    
    char buffer[4096];
    size_t n;
    while ((n = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, n, dst);
    }
    
    fclose(src);
    fclose(dst);
    return 1;
}

/* Email doğrula */
int utils_is_valid_email(const char *email) {
    if (!email || utils_is_empty(email)) return 0;
    
    const char *at = strchr(email, '@');
    if (!at || at == email) return 0;
    
    const char *dot = strchr(at, '.');
    if (!dot || dot == at + 1) return 0;
    
    return 1;
}

/* Telefon doğrula */
int utils_is_valid_phone(const char *phone) {
    if (!phone || utils_is_empty(phone)) return 0;
    
    int len = strlen(phone);
    if (len < 10) return 0;
    
    for (int i = 0; i < len; i++) {
        if (!isdigit(phone[i]) && phone[i] != '+' && phone[i] != '-' && 
            phone[i] != ' ' && phone[i] != '(') {
            return 0;
        }
    }
    
    return 1;
}

/* Sayı doğrula */
int utils_is_valid_number(const char *str) {
    if (!str || utils_is_empty(str)) return 0;
    
    char *endptr;
    strtod(str, &endptr);
    return *endptr == '\0';
}

/* Daire dizisini serbest bırak */
void utils_free_daire_array(void *arr, int count) {
    if (arr) free(arr);
}

/* Aidat dizisini serbest bırak */
void utils_free_aidat_array(void *arr, int count) {
    if (arr) free(arr);
}

/* Ödeme dizisini serbest bırak */
void utils_free_odeme_array(void *arr, int count) {
    if (arr) free(arr);
}
