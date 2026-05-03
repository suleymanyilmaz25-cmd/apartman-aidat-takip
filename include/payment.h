#ifndef PAYMENT_H
#define PAYMENT_H

#include "database.h"

/* Ödeme durumu */
typedef enum {
    PAYMENT_UNPAID,
    PAYMENT_PARTIAL,
    PAYMENT_PAID,
    PAYMENT_OVERDUE
} PaymentStatus;

/* Ödeme yönetimi fonksiyonları */
PaymentStatus payment_get_status(int aidat_id);
double payment_get_remaining(int aidat_id);
int payment_is_overdue(int aidat_id);
int payment_days_overdue(int aidat_id);

/* Ödeme raporları */
char* payment_get_status_string(PaymentStatus status);
void payment_print_summary(int daire_id);

#endif /* PAYMENT_H */
