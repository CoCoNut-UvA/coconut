#pragma once

typedef struct imap_entry_t {
    void *key;
    void *value;
    struct imap_entry_t *next;
} imap_entry_t;

typedef struct imap_t {
    int size;
    struct imap_entry_t **slots;
} imap_t;

imap_t *imap_init(int);
void imap_free(imap_t *);
unsigned int imap_hash_fun(void *);
unsigned int imap_hash(imap_t *, void *);
imap_entry_t *imap_entry_init(void *, void *);
void imap_insert(imap_t *, void *, void *);
void *imap_retrieve(imap_t *, void *);
void imap_map(imap_t *, void *(f)(void *, void *));
