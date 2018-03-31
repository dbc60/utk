/* ========================================================================
 * (C) Copyright 2018 Douglas B. Cuthbertson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 * ========================================================================
 */

/**
 * @file
 * @brief An atom module.
 */

#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include "platform.h"
#include "ehm.h"
#include "mem.h"
#include "atom.h"


#define ANYSIZE 1
#define ATOM_BUCKET_COUNT_DEFAULT 2048

#if defined(ATOM_BUCKETS)
#define ATOM_BUCKET_COUNT ATOM_BUCKETS
#else
#define ATOM_BUCKET_COUNT ATOM_BUCKET_COUNT_DEFAULT
#endif

/// @brief an atom is some data, its size, and a link to another atom.
struct atom {
    struct atom *link;
    size_t len;
    u08 *data;
};

/// @brief a hash table of atom objects with its own hash function.
struct atoms {
    atom_hash hash;
    size_t count;   // the number of buckets
    struct atom *buckets[ANYSIZE];
};


/// @brief the default hash function - forward declaration.
static size_t hash_default(const void *data, size_t bytes);


struct atoms_default {
    atom_hash hash;
    size_t count;
    struct atom *buckets[ATOM_BUCKET_COUNT];
};

static struct atoms_default default_table = {
    hash_default,
    ATOM_BUCKET_COUNT,
    {0}
};


/** @note: These are too small. I should generate a new set of 256 "random"
 * numbers, but make them 64-bit unsigned values. These are really 32-bit
 * unsigned numbers generated for the "C Interfaces and Implementations"
 * book by David R. Hanson
 */
static u64 scatter[] = {
2078917053, 143302914, 1027100827, 1953210302, 755253631, 2002600785,
1405390230, 45248011, 1099951567, 433832350, 2018585307, 438263339,
813528929, 1703199216, 618906479, 573714703, 766270699, 275680090,
1510320440, 1583583926, 1723401032, 1965443329, 1098183682, 1636505764,
980071615, 1011597961, 643279273, 1315461275, 157584038, 1069844923,
471560540, 89017443, 1213147837, 1498661368, 2042227746, 1968401469,
1353778505, 1300134328, 2013649480, 306246424, 1733966678, 1884751139,
744509763, 400011959, 1440466707, 1363416242, 973726663, 59253759,
1639096332, 336563455, 1642837685, 1215013716, 154523136, 593537720,
704035832, 1134594751, 1605135681, 1347315106, 302572379, 1762719719,
269676381, 774132919, 1851737163, 1482824219, 125310639, 1746481261,
1303742040, 1479089144, 899131941, 1169907872, 1785335569, 485614972,
907175364, 382361684, 885626931, 200158423, 1745777927, 1859353594,
259412182, 1237390611, 48433401, 1902249868, 304920680, 202956538,
348303940, 1008956512, 1337551289, 1953439621, 208787970, 1640123668,
1568675693, 478464352, 266772940, 1272929208, 1961288571, 392083579,
871926821, 1117546963, 1871172724, 1771058762, 139971187, 1509024645,
109190086, 1047146551, 1891386329, 994817018, 1247304975, 1489680608,
706686964, 1506717157, 579587572, 755120366, 1261483377, 884508252,
958076904, 1609787317, 1893464764, 148144545, 1415743291, 2102252735,
1788268214, 836935336, 433233439, 2055041154, 2109864544, 247038362,
299641085, 834307717, 1364585325, 23330161, 457882831, 1504556512,
1532354806, 567072918, 404219416, 1276257488, 1561889936, 1651524391,
618454448, 121093252, 1010757900, 1198042020, 876213618, 124757630,
2082550272, 1834290522, 1734544947, 1828531389, 1982435068, 1002804590,
1783300476, 1623219634, 1839739926, 69050267, 1530777140, 1802120822,
316088629, 1830418225, 488944891, 1680673954, 1853748387, 946827723,
1037746818, 1238619545, 1513900641, 1441966234, 367393385, 928306929,
946006977, 985847834, 1049400181, 1956764878, 36406206, 1925613800,
2081522508, 2118956479, 1612420674, 1668583807, 1800004220, 1447372094,
523904750, 1435821048, 923108080, 216161028, 1504871315, 306401572,
2018281851, 1820959944, 2136819798, 359743094, 1354150250, 1843084537,
1306570817, 244413420, 934220434, 672987810, 1686379655, 1301613820,
1601294739, 484902984, 139978006, 503211273, 294184214, 176384212,
281341425, 228223074, 147857043, 1893762099, 1896806882, 1947861263,
1193650546, 273227984, 1236198663, 2116758626, 489389012, 593586330,
275676551, 360187215, 267062626, 265012701, 719930310, 1621212876,
2108097238, 2026501127, 1865626297, 894834024, 552005290, 1404522304,
48964196, 5816381, 1889425288, 188942202, 509027654, 36125855,
365326415, 790369079, 264348929, 513183458, 536647531, 13672163,
313561074, 1730298077, 286900147, 1549759737, 1699573055, 776289160,
2143346068, 1975249606, 1136476375, 262925046, 92778659, 1856406685,
1884137923, 53392249, 1735424165, 1602280572
};

static size_t
hash_default(const void *data, size_t bytes) {
    size_t i = 0;
    size_t h = 0;

    EHM_ASSERT(data);

    while (i < bytes) {
        h = (h << 1) + scatter[((u08*)data)[i]];
        ++i;
    }

    return h;
}


/** @brief return the default hash table */
atoms
atoms_default() {
    return (atoms)&default_table;
}


/** @brief make a new hash table.
 * 
 * If atom_hash is null, use the default hash function.
 */
atoms
atoms_new(size_t buckets, atom_hash hash) {
    atoms result;
    size_t bytes = sizeof(struct atoms)
                   + sizeof(struct atom) * (buckets - ANYSIZE);

    EHM_ASSERT(buckets > 0);
    result = calloc(bytes, 1);

    if (result) {
        if (hash != NULL) {
            result->hash = hash;
        } else {
            result->hash = hash_default;
        }
        result->count = buckets;
    }

    return result;
}

void
atoms_free(atoms *at) {
    EHM_ASSERT(at && *at);

    if (*at != (atoms)&default_table) {
        free(*at);
        *at = NULL;
    }
}

size_t atom_length(atoms at, const char *str);


const char *atom_data(atoms at, const void *data, size_t bytes);
/*
 * This implementation of atoms will null-terminate new values, so when they
 * are retrieved they can be treated like nul-terminated strings.
 */

const char *
atom_data(atoms at, const void *data, size_t bytes) {
    const u08 *str = data;
    u64 h;
    size_t i;
    struct atom *p;
    const char *result = 0;

    EHM_ASSERT(data);

    // hash the byte array
    h = at->hash(data, bytes);
    h %= at->count;

    for (p = at->buckets[h]; p != NULL; p = p->link) {
        if (bytes == p->len) {
            for (i = 0; i < bytes && p->data[i] == str[i]; ++i);
            if (i == bytes) {
                result = (const char*)p->data;
            }
        }
    }

    if (NULL == result) {
        // create a new atom.
        p = ALLOC(sizeof(*p) + bytes + 1);
        p->len = bytes;
        p->data = (u08*)(p  + 1);
        result = (const char *)p->data;

        if (bytes > 0) {
            memcpy(p->data, data, bytes);
        }
        p->data[bytes] = '\0';

        p->link = at->buckets[h];
        at->buckets[h] = p;
    }

    return result;
}


const char *
atom_string(atoms at, const char *str) {
    EHM_ASSERT(str);

    return atom_data(at, str, strlen(str));
}


const char *
atom_i64(atoms at, i64 n) {
    /*
        20 bytes are enough for the decimal representation of any 64-bit value.
        Note: there is no null-terminator, nor is there room for one.
    */
    u08 str[20];
    u08 *s = str + sizeof str;
    u64 m;

    if (n == I64_MIN) {
        m = ((u64)I64_MAX) + 1UL;
    } else if (n < 0) {
        m = (u64)-n;
    } else {
        m = (u64)n;
    }

    do {
        *--s = m % 10 + '0';
    } while ((m /= 10) > 0);

    if (n < 0) {
        *--s = '-';
    }

    return atom_data(at, s, (str + sizeof str) - s);
}

const char *
atom_i32(atoms at, i32 n) {
    /*
        11 bytes are enough for the decimal representation of any 32-bit value.
        Note: there is no null-terminator, nor is there room for one.
    */
    u08 str[11];
    u08 *s = str + sizeof str;
    u32 m;

    if (n == I32_MIN) {
        m = ((u32)I32_MAX) + 1UL;
    } else if (n < 0) {
        m = (u32)-n;
    } else {
        m = (u32)n;
    }

    do {
        *--s = m % 10 + '0';
    } while ((m /= 10) > 0);

    if (n < 0) {
        *--s = '-';
    }

    return atom_data(at, s, (str + sizeof str) - s);
}

const char *
atom_u64(atoms at, u64 n) {
    /*
        20 bytes are enough for the decimal representation of any 64-bit value.
        Note: there is no null-terminator, nor is there room for one.
    */
    u08 str[20];
    u08 *s = str + sizeof str;

    do {
        *--s = n % 10 + '0';
    } while ((n /= 10) > 0);

    return atom_data(at, s, (str + sizeof str) - s);
}

const char *
atom_u32(atoms at, u32 n) {
    /*
        10 bytes are enough for the decimal representation of any unsigned
        32-bit value.
        Note: there is no null-terminator, nor is there room for one.
    */
    u08 str[10];
    u08 *s = str + sizeof str;

    do {
        *--s = n % 10 + '0';
    } while ((n /= 10) > 0);

    return atom_data(at, s, (str + sizeof str) - s);
}
