/*
 * Copyright (c) 2008 Antoine Kaufmann
 *
 * This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/projects/COPYING.WTFPL for more details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __LOST__
#undef __LOST__

#ifdef __LOST__
    #include "cdi.h"
    #include "cdi/misc.h"
    #include "cdi/cache.h"
#else
    #include "cdi_cache.h"
#endif


#define INVBLKNUM (~0L)
#define READBUF_SZ 128

static uint64_t cache_time = 0;
#ifdef __LOST__
    static cdi_list_t cache_list = NULL;
#endif

/**
 * Hints fuer die Suche im Cache
 */
struct hint {
    /** Index */
    size_t      index;

    /** Block der dort gefunden werden kann */
    uint64_t    block;
};

/**
 * Block im Cache
 */
struct block {
    /** CDI-Interface-Block */
    struct cdi_cache_block  cdi;

    /** Anzahl der Eintraege, die diesen Block beinhalten */
    uint16_t                ref_count;

    /** Wird auf 1 gesetzt, wenn die Daten veraendert wurden */
    uint16_t                dirty;

    /** Zeit der letzten Benutzung */
    uint64_t                access_time;
};

/**
 * Cache-Handle mit OS-spezifischen Daten
 */
struct cache {
    /** Eigentliches Cache-Handle */
    struct cdi_cache    cache;

    /** Anzahl der Blocks */
    size_t              block_count;

    /** Anzahl der benutzten Blocks */
    size_t              blocks_used;

    /** Blockliste (sortiert nach offset) */
    struct block*       blocks;


    /** Callback zum Lesen eines Blocks */
    cdi_cache_read_block_t* read_block;

    /** Callback zum Schreiben eines Blocks */
    cdi_cache_write_block_t* write_block;

    /** Letzter Parameter fuer die Callbacks */
    void*               prv_data;


    /** Lesepuffer */
    uint8_t*            read_buffer;

    /** Erster Block im Lesepuffer oder INVBLKNUM wenn keiner */
    uint64_t            read_buffer_block;

    /** Anzahl Blocks die im Puffer sind */
    size_t              read_buffer_cnt;


    /** Hints fuer Positionen */
    struct hint         hints[8];

    /** Zuletzt geschriebener hint-index */
    uint16_t            prev_hint;
};

/**
 * Cache-eintrag
 */
struct entry {
    /** Eigentlicher CDI-Cache-Eintrag */
    struct cdi_cache_entry  entry;

    /** Offset */
    uint64_t                offset;

    /** Groesse des Bereichs */
    size_t                  size;

    /** Anzahl der Blocks */
    size_t                  block_count;

    /** Pointer auf ein Array mit Blockpointern */
    struct block**          blocks;

    /** Gesperrt */
    int                     locked;
};



/**
 * Cache erstellen
 *
 * @param block_size    Groesse der Blocks die der Cache verwalten soll
 * @param blkpriv_len   Groesse der privaten Daten die fuer jeden Block
 *                      alloziert werden und danach vom aurfrufer frei benutzt
 *                      werden duerfen
 * @param read_block    Funktionspointer auf eine Funktion zum einlesen eines
 *                      Blocks.
 * @param write_block   Funktionspointer auf eine Funktion zum schreiben einses
 *                      Blocks.
 * @param prv_data      Wird den Callbacks als letzter Parameter uebergeben
 *
 * @return Pointer auf das Cache-Handle
 */
struct cdi_cache* cdi_cache_create(size_t block_size, size_t blkpriv_len,
    cdi_cache_read_block_t* read_block,
    cdi_cache_write_block_t* write_block,
    void* prv_data)
{
    struct cache* cache = malloc(sizeof(*cache));
    size_t i;

    cache->read_block = read_block;
    cache->write_block = write_block;
    cache->prv_data = prv_data;

    // FIXME
    cache->cache.block_size = block_size;
    cache->block_count = 1 * 1024 * 1024 / cache->cache.block_size;
    cache->blocks_used = 0;

    cache->read_buffer = malloc(cache->cache.block_size * READBUF_SZ);
    cache->read_buffer_block = INVBLKNUM;
    cache->read_buffer_cnt = 0;

    memset(cache->hints, 0, sizeof(cache->hints));
    cache->prev_hint = 0;


    cache->blocks = malloc(sizeof(struct block) * cache->block_count);
    for (i = 0; i < cache->block_count; i++) {
        struct block* block = cache->blocks + i;

        block->cdi.number = INVBLKNUM;
        block->cdi.data = malloc(cache->cache.block_size);
        block->cdi.private = malloc(blkpriv_len);

        block->ref_count = 0;
        block->dirty = 0;
    }

    // Cache in die Liste eintragen damit er Synchronisiert wird
#ifdef __LOST__
    if (cache_list == NULL) {
        cache_list = cdi_list_create();
    }
    cdi_list_push(cache_list, cache);
#endif

    return (struct cdi_cache*) cache;
}

/**
 * Cache zerstoeren
 */
void cdi_cache_destroy(struct cdi_cache* cache)
{
    struct cache* c = (struct cache*) cache;
    struct block* b;
    int i;

    if (!cdi_cache_sync(cache)) {
        puts("cdi_cache: Sync fehlgeschlagen vor dem Zerstoeren!");
    }

    // Einzelne Blocks freigeben
    for (i = 0; i < c->block_count; i++) {
        b = c->blocks + i;

        if (b->ref_count) {
            printf("cdi_cache: Beim Zerstoeren des Caches wurde ein Block "
                "gefunden, der einen Referenzzaehler != 0 hat (%lld)\n",
                (unsigned long long) b->cdi.number);
        }

        if (b->dirty) {
            printf("cdi_cache: Beim Zerstoeren des Caches wurde ein Block "
                "gefunden, der als veraendert markiert ist (%lld)\n",
                (unsigned long long) b->cdi.number);
        }

        free(b->cdi.data);
        free(b->cdi.private);
    }

    free(c->blocks);
    free(c->read_buffer);
    free(c);
}


/**
 * Einzelnen Block auf das Geraet schreiben. Das wird nur durchgefuehrt, wenn er
 * veraendert wurde.
 *
 * @param c Cache-Handle
 * @param b Block-Pointer
 *
 * @return 1 bei Erfolg, 0 im Fehlerfall
 */
static inline int cache_sync_block(struct cache* c, struct block* b)
{
    if (!b->dirty) {
        return 1;
    }

    // Lesepuffer aktualisieren, wenn noetig
    if ((c->read_buffer_block != INVBLKNUM) &&
        (c->read_buffer_block <= b->cdi.number) &&
        (b->cdi.number < c->read_buffer_block + c->read_buffer_cnt))
    {
        memcpy(c->read_buffer +
            (b->cdi.number - c->read_buffer_block) * c->cache.block_size,
            b->cdi.data, c->cache.block_size);
    }

    if (!c->write_block((struct cdi_cache*) c, b->cdi.number, 1, b->cdi.data,
        c->prv_data))
    {
        puts("cdi_cache: Schreiben beim Cache-Sync fehlgeschlagen.");
        return 0;
    }
    b->dirty = 0;

    return 1;
}


/**
 * Veraenderte Blocks aus dem Cache auf das Geraet schreiben
 *
 * @param cache Handle
 *
 * @return 1 bei Erfolg, 0 im Fehlerfall
 */
int cdi_cache_sync(struct cdi_cache* cache)
{
    struct cache* c = (struct cache*) cache;;
    struct block* b;
    size_t i;

    for (i = 0; i < c->block_count; i++) {
        b = c->blocks + i;

        // Sobald ein Eintrag mit INVBLKNUM erreicht wird, ist das Durchsuchen
        // beendet
        if (b->cdi.number == INVBLKNUM) {
            return 1;
        }

        cache_sync_block(c, b);
    }

    return 1;
}

#ifdef __LOST__
/**
 * Caches Synchronisieren
 */
void caches_sync_all()
{
    size_t i;
    struct cache* c;

    if (cache_list == NULL) {
        return;
    }

    for (i = 0; (c = cdi_list_get(cache_list, i)); i++) {
        if (!cdi_cache_sync((struct cdi_cache*) c)) {
            puts("cdi_cache: Fehler beim Syncen der Caches.");
        }
    }
}
#endif

/**
 * Sucht das Element mit dem Enstprechenden Index, oder falls keines mit
 * diesem Index existiert, wird die Stelle angegeben an der es stehen muesste
 *
 * @param block Blocknummer des gesuchten Elements
 * @param dest  Pointer auf die Variable, in der der gefundene Index abgelegt
 *              werden soll.
 *
 * @return 1 bei Erfolg, 0 sonst
 */
static size_t search_position(struct cache* cache, uint64_t block,
    size_t* dest)
{
    size_t top = cache->blocks_used - 1;
    size_t bottom = 0;
    size_t pos;

    // Pruefen ob offset ueberhaupt im Array enthalten ist
    if (block < cache->blocks[0].cdi.number) {
        *dest = 0;
        return 0;
    } else if (block > cache->blocks[top].cdi.number) {
        // Wenn noch freie Blocks im Cache existieren wird der Index auf den
        // naechst freien gelegt
        if (top < cache->block_count - 1) {
            top++;
        }
        *dest = top;
        return 0;
    }

    // Binaere Suche nach dem Element
    do {
        pos = bottom + (top - bottom) / 2;

        if (cache->blocks[pos].cdi.number == block) {
            break;
        } else if (cache->blocks[pos].cdi.number > block) {
            top = pos - 1;
        } else {
            bottom = pos + 1;
            // Falls hier gleich abgebrochen wird, muss pos auf ein Element
            // zeigen, das nicht groesser ist, als das Gesuchte
            pos = bottom;
        }
    } while (top >= bottom);

    if (pos >= cache->block_count) {
        pos = cache->block_count - 1;
    }

    *dest = pos;

    return (cache->blocks[pos].cdi.number == block);
}


/**
 * Hint suchen fuer eine Blocknummer
 *
 * @param block Blocknummer
 *
 * @return Index oder (size_t) -1 im Fehlerfall
 */
static inline size_t get_hint(struct cache* cache, uint64_t block)
{
    int i;

    for (i = 0; i < sizeof(cache->hints) / sizeof(struct hint); i++) {
        if (cache->hints[i].block == block) {
            return cache->hints[i].index;
        }
    }

    return -1;
}

/**
 * Hint setzen
 *
 * @param block Blocknummer
 * @param index Index an dem der Block gefunden werden kann
 */
static inline void put_hint(struct cache* cache, uint64_t block, size_t index)
{
    cache->prev_hint++;
    cache->prev_hint %= sizeof(cache->hints) / sizeof(struct hint);
    cache->hints[cache->prev_hint].block = block;
    cache->hints[cache->prev_hint].index = index;
}

/**
 * Block in der Liste finden
 *
 * @param cache Handle
 * @param block Blocknummer
 *
 * @return Pointer auf den Block oder NULL wenn er nicht in der Liste ist
 */
static inline struct block* block_find(struct cache* cache, uint64_t block)
{
    size_t i;

    i = get_hint(cache, block);
    if ((i != -1) && (cache->blocks[i].cdi.number == block)) {
        return cache->blocks + i;
    }

    if (!search_position(cache, block, &i)) {
        return NULL;
    }

    put_hint(cache, block, i);
    return cache->blocks + i;
}


/**
 * Cache-Element zum ersetzen finden
 *
 * @param blocknum Blocknummer des neuen Elements
 *
 * @return Array-Index des Elements das erstezt werden kann oder block_count
 * wenn keines gefunden wurde.
 */
static inline size_t find_replaceable(struct cache* cache, uint64_t blocknum)
{
    struct block* b = NULL;
    size_t i;
    size_t la_index = cache->block_count;

    // Wenn noch unbenzte Blocks existieren, dann gehoert der letzte sicher dazu
    if (cache->blocks[cache->block_count - 1].cdi.number == INVBLKNUM) {
        return cache->block_count - 1;
    }

    for (i = 0; i < cache->blocks_used; i++) {
        b = cache->blocks + i;

        // Solte nicht passieren
        if (b->cdi.number == INVBLKNUM) {
            puts("cdi_cache: Hier ist ein Element mit ungueltiger Blocknummer "
                "mitten im Array!");
            printf("%u\n", (unsigned int) i);
            continue;
        }

        // Pruefen ob das Element prinzipiell ersetzt werden darf, wenn nicht,
        // wird zum naechsten gesprungen
        if (b->ref_count) {
            continue;
        }

        // Wenn bis jetzt noch kein passendes Element gefunden wurde, dann muss
        // es genommen werden. Andernfalls wird geprueft ob das element aelter
        // ist
        if ((la_index == cache->block_count) || (b->access_time <
            cache->blocks[la_index].access_time))
        {
            la_index = i;
        }
    }

    // Falls das Element dirty ist, muss es gespeichert werden
    if ((la_index != cache->block_count) && (cache->blocks[la_index].dirty)) {
        cache_sync_block(cache, cache->blocks + la_index);
    }
    return la_index;
}

/**
 * Block im Cache allozieren und initialisieren
 *
 * @param cache     Handle
 * @param blocknum  Blocknummer des neuen Blocks
 * @param index     Pointer auf Speicherstelle, an der der Listenindex
 *                  abgespeichert werden soll.
 *
 * @return 1 wenn ein Element gefunden wurde, 0 sonst
 */
static inline size_t block_allocate(struct cache* cache, uint64_t blocknum,
    size_t* index)
{
    struct block* b;
    void* data;
    void* private;

    // Index an dem der neue Block spaeter im Array liegt
    size_t dest;

    // Index des Blocks, der ersetzt werden soll
    size_t i;


    // Position suchen an der der neue Block spaeter im Array liegen soll
    if (search_position(cache, blocknum, &dest)) {
        puts("cdi_cache: Warnung: Block, der bereits im Cache ist, soll"
           " alloziert werden.");
        *index = dest;
        return 1;
    }

    // Element finden, das ersetzt werden soll
    i = find_replaceable(cache, blocknum);
    //printf("Replaceable: %d", i);
    if (i == cache->block_count) {
        return 0;
    }

    // Wenn der Block bisher nicht benutzt wurde, muss used Blocks erhoeht
    // werden
    if (cache->blocks[i].cdi.number == INVBLKNUM) {
        cache->blocks_used++;
    }

    // Pointer auf daten des zu ersetzenden Blocks speichern, da die
    // ueberschrieben werden, beim memmove
    data = cache->blocks[i].cdi.data;
    private = cache->blocks[i].cdi.private;

    // Jetzt muessen die Array-Elemente so verschoben werden, dass die stelle an
    // mit dem index dest frei wird und dafuer die mit index i ueberschrieben
    // wird
    if (i < dest) {
        // Wenn wir die Elemente nach unten verschieben, muss das zielelement
        // kleiner sein, als das aktuelle
        if ((cache->blocks[dest].cdi.number > blocknum) && (dest > 0)) {
            dest--;
        }

        memmove(cache->blocks + i, cache->blocks + i + 1, (dest - i) *
            sizeof(struct block));
    } else if (i > dest) {
        // Wenn wird die Elemente nach oben verschieben, muss das zielelement
        // groesser sein, als das aktuelle
        if ((cache->blocks[dest].cdi.number < blocknum) &&
            (dest < cache->block_count - 1))
        {
            dest++;
        }

        memmove(cache->blocks + dest + 1, cache->blocks + dest, (i - dest) *
            sizeof(struct block));
    }

    b = cache->blocks + dest;
    b->cdi.data = data;
    b->cdi.number = blocknum;
    b->cdi.private = private;
    b->access_time = ++cache_time;
    b->dirty = 0;
    b->ref_count = 0;
    *index = dest;
    return 1;
}

/**
 * Neuen block laden, der noch nicht in der Liste ist
 *
 * @param cache Handle
 * @param block Blocknummer
 * @param read  Wenn 1 dann muss der Block eingelesen werden, bei 0 wird nur
 *              ein freier Block herausgesucht (Wenn er eh ganz ueberschrieben
 *              wird)
 *
 * @return Pointer auf den Block
 */
static inline struct block* block_load(struct cache* cache, uint64_t block,
    int read)
{
    size_t index;
    struct block* b;
    size_t block_size = cache->cache.block_size;

    if (!block_allocate(cache, block, &index)) {
        puts("cdi_cache Panic: Kein Unbenutztes Element gefunden");
        return NULL;
    }

    b = cache->blocks + index;

    if (read) {
        uint64_t bnum = cache->read_buffer_block;

        // Wenn der zu lesende Block nicht im Puffer ist, wird er in den Puffer
        // eingelesen
        if ((bnum == INVBLKNUM) || (bnum > block) || (block >= bnum +
            cache->read_buffer_cnt))
        {
            cache->read_buffer_block = bnum = block;

            cache->read_buffer_cnt = cache->read_block(
                (struct cdi_cache*) cache, block, READBUF_SZ,
                cache->read_buffer, cache->prv_data);

            if (cache->read_buffer_cnt == 0) {
                puts("cdi_cache Panic: Einlesen der Daten fehlgeschlagen");
                return NULL;
            }
            //puts("nomatch");
        } else {
            //puts("match");
        }

        memcpy(b->cdi.data, cache->read_buffer + (block - bnum) * block_size,
            block_size);
    }

    return b;
}

/**
 * Block holen; Falls er noch nicht in der Liste ist, wird er geladen
 *
 * @param cache Handle
 * @param block Blocknummer
 * @param read  Wenn 1 dann muss der Block eingelesen werden, bei 0 wird nur
 *              ein freier Block herausgesucht (Wenn er eh ganz ueberschrieben
 *              wird)
 *
 *
 * @return Pointer auf den Block
 */
static inline struct block* get_block(struct cache* cache, uint64_t block,
    int read)
{
    struct block* b;

    if (!(b = block_find(cache, block))) {
        b = block_load(cache, block, read);
    }

    if (b) {
        b->access_time = ++cache_time;
    }


    return b;
}



/**
 * Cache-Block holen. Dabei wird intern ein Referenzzaehler erhoeht, sodass der
 * Block nicht freigegeben wird, solange er benutzt wird. Das heisst aber auch,
 * dass der Block nach der Benutzung wieder freigegeben werden muss, da sonst
 * die freien Blocks ausgehen.
 *
 * @param cache     Cache-Handle
 * @param blocknum  Blocknummer
 *
 * @return Pointer auf das Block-Handle oder NULL im Fehlerfall
 */
struct cdi_cache_block* cdi_cache_block_get(struct cdi_cache* cache,
    uint64_t blocknum)
{
    struct block* b = get_block((struct cache*) cache, blocknum, 1);

    if (!b) {
        return NULL;
    }

    b->ref_count++;

    return (struct cdi_cache_block*) b;
}

/**
 * Cache-Block freigeben
 *
 * @param cache Cache-Handle
 * @param block Block-Handle
 */
void cdi_cache_block_release(struct cdi_cache* cache,
    struct cdi_cache_block* block)
{
    struct block* b = (struct block*) block;

    b->ref_count--;
}

/**
 * Cache-Block als veraendert markieren
 *
 * @param cache Cache-Handle
 * @param block Block-Handle
 */
void cdi_cache_block_dirty(struct cdi_cache* cache,
    struct cdi_cache_block* block)
{
    struct block* b = (struct block*) block;
    b->dirty = 1;
}




/**
 * Cache-Eintrag erstellen
 *
 * @param offset    Position auf dem Datentraeger
 * @param size      Groesse des Bereichs
 *
 * @return Handle
 */
struct cdi_cache_entry* cdi_cache_entry_new(struct cdi_cache* cache,
    uint64_t offset, size_t size)
{
    struct entry* entry = malloc(sizeof(struct entry));
    size_t block_size = cache->block_size;
    uint64_t start_block = offset / block_size;
    uint64_t end_block = (offset + size - 1) / block_size;

    entry->entry.cache = cache;
    entry->locked = 0;
    entry->offset = offset;
    entry->size = size;
    entry->block_count = end_block - start_block + 1;
    entry->blocks = calloc(sizeof(struct block*) * entry->block_count, 1);

    return (struct cdi_cache_entry*) entry;
}

/**
 * Cache-Eintrag freigeben
 *
 * @param entry Handle
 */
void    cdi_cache_entry_release(struct cdi_cache_entry* entry)
{
    struct entry* e = (struct entry*) entry;
    size_t i;

    if (e->locked) {
        cdi_cache_entry_unlock(entry);
    }

    for (i = 0; i < e->block_count; i++) {
        struct block* block = e->blocks[i];
        if (block) {
            block->ref_count--;
        }
    }

    free(e);
}

/**
 * Cache-Eintrag sperren
 *
 * @param entry Handle
 */
void    cdi_cache_entry_lock(struct cdi_cache_entry* entry)
{
    struct entry* e = (struct entry*) entry;
    e->locked = 1;
}

/**
 * Cache-Eintrag entsperren
 *
 * @param entry Handle
 */
void    cdi_cache_entry_unlock(struct cdi_cache_entry* entry)
{
    struct entry* e = (struct entry*) entry;
    e->locked = 0;
}

/**
 * Cache-Eintrag als veraendert markieren
 *
 * @param entry Handle
 */
void    cdi_cache_entry_dirty(struct cdi_cache_entry* entry)
{
    struct entry* e = (struct entry*) entry;
    size_t i;

    for (i = 0; i < e->block_count; i++) {
        struct block* block = e->blocks[i];
        block->dirty = 1;
    }
}



/**
 * Pointer auf einen Block im Cache-Eintrag holen
 *
 * @param entry Handle
 * @param block Blocknummer relativ zum Eintrag (der Offset innerhalb des
 *              Eintrags wird nicht beruecksichtigt)
 *
 * @return Pointer auf den Block
 */
void*   cdi_cache_entry_block(struct cdi_cache_entry* entry, size_t block)
{
    struct entry* e = (struct entry*) entry;
    size_t block_size = entry->cache->block_size;
    uint64_t dev_start_block = (e->offset) / block_size;

    if (block >= e->block_count) {
        return NULL;
    }

    if (e->blocks[block] == NULL) {
        e->blocks[block] = get_block((struct cache*) entry->cache,
            dev_start_block + block, 1);
        e->blocks[block]->ref_count++;
    }


    return e->blocks[block]->cdi.data;
}

/**
 * Daten aus dem Cache-Eintrag lesen
 *
 * @param entry     Handle
 * @param offset    Offset relativ zum Cache-Eintrag
 * @param size      Groesse des zu lesenden Bereichs
 * @param buffer    Puffer in dem die Daten abgelegt werden sollen
 */
int     cdi_cache_entry_read(struct cdi_cache_entry* entry, size_t offset,
            size_t size, void* buffer)
{
    struct entry* e = (struct entry*) entry;
    size_t block_size = entry->cache->block_size;
    uint64_t dev_start_block = (e->offset + offset) / block_size;
    size_t start_block = (e->offset + offset) / block_size - e->offset /
        block_size;
    size_t end_block = (e->offset + offset + size -1) / block_size - e->offset /
        block_size;
    size_t block_count = end_block - start_block + 1;
    size_t i;
    size_t bytes_done = 0;

    // Nicht ueber den Eintragsrand hinauslesen
    if ((offset + size) > e->size) {
        return 0;
    }

    // Offset vom Block-Anfang im Cacheeintrag beruecksichtigen, falls vorhanden
    offset += e->offset % block_size;

    for (i = 0; i < block_count; i++) {
        void* block;
        size_t bytes = block_size;
        size_t off = 0;

        if (e->blocks[start_block + i] == NULL) {
            e->blocks[start_block + i] = get_block((struct cache*) entry->cache,
                dev_start_block + i, 1);
            e->blocks[start_block + i]->ref_count++;
        }

        block = e->blocks[start_block + i]->cdi.data;
        e->blocks[start_block + i]->access_time = ++cache_time;

        // Beim ersten Durchlauf muss der Anfangsoffset beruecksichtigt werden
        if (i == 0) {
            off = offset % block_size;
            bytes = block_size - off;
        }

        // Beim letzten Durchlauf muss unter umstaenden nicht mehr ein ganzer
        // Block gelesen werden.
        if (i == block_count - 1) {
            bytes = size - bytes_done;
        }

        memcpy((void*) ((uintptr_t) buffer + bytes_done), (void*)
            ((uintptr_t) block + off), bytes);
        bytes_done += bytes;
    }

    return 1;
}

/**
 * Daten in den Cache-Eintrag schreiben
 *
 * @param entry     Handle
 * @param offset    Offset relativ zum Cache-Eintrag
 * @param size      Groesse des zu schreibenden Bereichs
 * @param buffer    Puffer aus dem die Daten gelesen werden
 */
int     cdi_cache_entry_write(struct cdi_cache_entry* entry, size_t offset,
            size_t size, const void* buffer)
{
    struct entry* e = (struct entry*) entry;
    size_t block_size = entry->cache->block_size;
    uint64_t dev_start_block = (e->offset + offset) / block_size;
    size_t start_block = (e->offset + offset) / block_size - e->offset /
        block_size;
    size_t end_block = (e->offset + offset + size - 1) / block_size - e->offset /
        block_size;
    size_t block_count = end_block - start_block + 1;
    size_t i;
    size_t bytes_done = 0;

    // Nicht ueber den Eintragsrand hinausschreiben
    if ((offset + size) > e->size) {
        return 0;
    }

    // Offset vom Block-Anfang im Cacheeintrag beruecksichtigen, falls vorhanden
    offset += e->offset % block_size;

    for (i = 0; i < block_count; i++) {
        void* block;
        size_t bytes = block_size;
        size_t off = 0;

        // Beim ersten Durchlauf muss der Anfangsoffset beruecksichtigt werden
        if (i == 0) {
            off = offset % block_size;
            bytes = block_size - off;
        }

        if (i == block_count - 1) {
            bytes = size - bytes_done;
        }


        if (e->blocks[start_block + i] == NULL) {
            int read = 1;

            if ((off == 0) && (bytes == block_size)) {
                read = 0;
            }

            e->blocks[start_block + i] = get_block((struct cache*) entry->cache,
                dev_start_block + i, read);
            e->blocks[start_block + i]->ref_count++;
        }

        block = e->blocks[start_block + i]->cdi.data;
        e->blocks[start_block + i]->access_time = ++cache_time;
        e->blocks[start_block + i]->dirty = 1;


        memcpy((void*) ((uintptr_t) block + off), (void*) ((uintptr_t) buffer +
            bytes_done), bytes);
        bytes_done += bytes;
    }

    return 1;
}


