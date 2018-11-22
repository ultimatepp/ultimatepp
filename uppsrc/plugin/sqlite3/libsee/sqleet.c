#include "config.c"
//#include "sqlite3.c"
#include "rekeyvacuum.c"
#include "crypto.c"

/*
 * SQLite3 codec implementation.
 */
typedef struct codec { 
    struct codec *reader, *writer;
    unsigned char key[32], salt[16];
    void *pagebuf;
    int pagesize;
    const void *zKey;
    int nKey;
} Codec;

Codec *codec_new(const char *zKey, int nKey)
{
    Codec *codec;
    if ((codec = sqlite3_malloc(sizeof(Codec)))) {
        codec->reader = codec->writer = codec;
        memset(codec->key, 0, sizeof(codec->key));
        memset(codec->salt, 0, sizeof(codec->salt));
        codec->pagebuf = NULL;
        codec->pagesize = 0;
        codec->zKey = zKey;
        codec->nKey = nKey;
    }
    return codec;
}

Codec *codec_dup(Codec *src)
{
    Codec *codec;
    if ((codec = codec_new(src->zKey, src->nKey))) {
        codec->reader = (src->reader == src) ? codec : src->reader;
        codec->writer = (src->writer == src) ? codec : src->writer;
        memcpy(codec->salt, src->salt, 16);
        memcpy(codec->key, src->key, 32);
    }
    return codec;
}

void codec_kdf(Codec *codec)
{
    pbkdf2_hmac_sha256(codec->zKey, codec->nKey, codec->salt, 16, 12345,
                       codec->key, 32);
    codec->zKey = NULL;
    codec->nKey = 0;
}

void codec_free(void *pcodec)
{
    if (pcodec) {
        int i;
        volatile char *p;
        Codec *codec = pcodec;
        if (codec->pagebuf) {
            p = codec->pagebuf;
            for (i = 0; i < codec->pagesize; p[i++] = '\0');
            sqlite3_free(codec->pagebuf);
        }
        p = pcodec;
        for (i = 0; i < sizeof(Codec); p[i++] = '\0');
        sqlite3_free(codec);
    }
}

/*
 * The encrypted database page format.
 *
 * +----------------------------------------+----------------+----------------+
 * | Encrypted data                         | 16-byte nonce  | 16-byte tag    |
 * +----------------------------------------+----------------+----------------+
 *
 * As the only exception, the first page (page_no=1) starts with a plaintext
 * salt contained in the first 16 bytes of the database file. The "master" key
 * is derived from a user-given password with the salt and 12345 iterations of
 * PBKDF-HMAC-SHA256. Future plans include switching to BLAKE2 and Argon2.
 *
 * - The data is encrypted by XORing with the ChaCha20 keystream produced from
 *   the 16-byte nonce and a 32-byte encryption key derived from the master key.
 *   - OK, I lied a little: ChaCha20 uses only the first 12 bytes as the nonce.
 *     However, ChaCha20 also requires an initial value for a counter of 4 bytes
 *     that encodes a block position in the output stream. We derive the counter
 *     value from the last 4 bytes, effectively extending the nonce to 16 bytes.
 *   - Specifically, counter = LOAD32_LE(nonce[12..15])^page_no is first applied
 *     to generate a single 64-byte block from nonce[0..11] and the master key.
 *     The block consists of two 32-byte one-time keys, the former is a Poly1305
 *     key for the authentication tag, and the latter is a ChaCha20 key for the
 *     data encryption. The encryption with the one-time key uses nonce[0..11]
 *     and the initial counter value of counter+1.
 *   - The XOR with page_no prevents malicious reordering of the pages.
 *
 * - The nonce consists of 128 randomly generated bits, which should be enough
 *   to guarantee uniqueness with a reasonable pseudorandom number generator.
 *   - Given a perfect RNG, the adversary needs to observe at least 2^61 nonces
 *     to break Poly1305 with the birthday attack at a success rate of 1%.
 *   - If a nonce is reused, we lose confidentiality of the associated messages.
 *     Moreover, the compromised nonce can also be used to forge valid tags for
 *     new messages having the same nonce (basically, the one-time Poly1305 key
 *     can be recovered from distinct messages with identical nonces).
 *
 * - The tag is a Poly1305 MAC calculated over the encrypted data and the nonce
 *   with the one-time key generated from the master key and the nonce.
 */

#define PAGE_NONCE_LEN 16
#define PAGE_TAG_LEN 16
#define PAGE_RESERVED_LEN (PAGE_NONCE_LEN + PAGE_TAG_LEN)

void *codec_handle(void *codec, void *pdata, Pgno page, int mode)
{
    uint32_t counter;
    unsigned char otk[64], tag[16], *data = pdata;
    Codec *reader = ((Codec *)codec)->reader;
    Codec *writer = ((Codec *)codec)->writer;
    const int skip = (page == 1) ? SKIP_HEADER_BYTES : 0;

    switch (mode) {
    case 0: /* Journal decryption */
    case 2: /* Reload a page */
    case 3: /* Load a page */
        if (reader) {
            int n = reader->pagesize - PAGE_RESERVED_LEN;
            if (page == 1 && reader->zKey) {
                memcpy(reader->salt, data, 16);
                codec_kdf(reader);
            }

            /* Generate one-time keys */
            memset(otk, 0, 64);
            counter = LOAD32_LE(data + n + PAGE_NONCE_LEN-4) ^ page;
            chacha20_xor(otk, 64, reader->key, data + n, counter);

            /* Verify the MAC */
            poly1305(data, n + PAGE_NONCE_LEN, otk, tag);
            if (poly1305_tagcmp(data + n + PAGE_NONCE_LEN, tag) != 0)
                return NULL;

            /* Decrypt */
            chacha20_xor(data + skip, n - skip, otk+32, data + n, counter+1);
            if (page == 1) memcpy(data, "SQLite format 3", 16);
        }
        break;

    case 7: /* Encrypt a journal page (with the reader key) */
        writer = reader;
        /* fall-through */
    case 6: /* Encrypt a main database page */
        if (writer) {
            int n = writer->pagesize - PAGE_RESERVED_LEN;
            data = memcpy(writer->pagebuf, data, writer->pagesize);

            /* Generate one-time keys */
            memset(otk, 0, 64);
            chacha20_rng(data + n, 16);
            counter = LOAD32_LE(data + n + PAGE_NONCE_LEN-4) ^ page;
            chacha20_xor(otk, 64, writer->key, data + n, counter);

            /* Encrypt and authenticate */
            chacha20_xor(data + skip, n - skip, otk+32, data + n, counter+1);
            if (page == 1) memcpy(data, writer->salt, 16);
            poly1305(data, n + PAGE_NONCE_LEN, otk, data + n + PAGE_NONCE_LEN);
        }
        break;
    }

    return data;
}

/* Reads page1 to trigger codec_kdf and verify the encryption key */
static int codec_verify_page1(Codec *codec, Btree *pBt)
{
    int count, rc = SQLITE_OK;
    Pager *pager = sqlite3BtreePager(pBt);
    sqlite3PagerSharedLock(pager);
    sqlite3PagerPagecount(pager, &count);
    if (count > 0) {
        DbPage *page;
        rc = SQLITE_NOTADB;
        sqlite3PcacheTruncate(pager->pPCache, 0);
        if (sqlite3PagerGet(pager, 1, &page, 0) == SQLITE_OK) {
            if (!memcmp(page->pData, "SQLite format 3", 16))
                rc = SQLITE_OK;
            sqlite3PagerUnref(page);
        } else {
            sqlite3PagerSetCodec(pager, NULL, NULL, NULL, NULL);
        }
    } else if (codec && codec->zKey) {
        /* Generate a salt and derive an encryption key for an empty database */
        chacha20_rng(codec->salt, 16);
        codec_kdf(codec);
    }
    pager_unlock(pager);
    return rc;
}

/*
 * Set (or unset) a codec for the pager of the specified Btree.
 *
 * The caller must hold the database mutex when calling this function.
 * Note that the function consumes the passed-in codec structure.
 */
static int codec_set_to(Codec *codec, Btree *pBt)
{
    int pagesize;
    Pager *pager = sqlite3BtreePager(pBt);

    if (!codec) {
        /* Unset a codec */
        sqlite3PagerSetCodec(pager, NULL, NULL, NULL, NULL);
        return SQLITE_OK;
    }

    /* Allocate page buffer */
    pagesize = sqlite3BtreeGetPageSize(pBt);
    if (!codec->pagebuf || codec->pagesize != pagesize) {
        void *new = sqlite3_malloc(pagesize);
        if (!new) {
            codec_free(codec);
            return SQLITE_NOMEM;
        }
        if (codec->pagebuf) {
            int i = 0;
            while (i < codec->pagesize)
                ((volatile char *)codec->pagebuf)[i++] = '\0';
            sqlite3_free(codec->pagebuf);
        }
        codec->pagebuf = new;
        codec->pagesize = pagesize;
    }

    /* Force secure delete */
    sqlite3BtreeSecureDelete(pBt, 1);

    /* Adjust the page size and the reserved area */
    if (pager->nReserve != PAGE_RESERVED_LEN) {
        pBt->pBt->btsFlags &= ~BTS_PAGESIZE_FIXED;
        sqlite3BtreeSetPageSize(pBt, pagesize, PAGE_RESERVED_LEN, 0);
    }

    /* Set pager codec and try to read page1 */
    sqlite3PagerSetCodec(pager, codec_handle, NULL, codec_free, codec);
    return codec_verify_page1(codec, pBt);
}

void sqlite3CodecGetKey(sqlite3 *db, int nDb, void **zKey, int *nKey)
{
    /*
     * sqlite3.c calls this function to decide if a database attached without a
     * password should use the encryption scheme of the main database. Returns
     * *nKey == 1 to indicate that the main database encryption is available.
     */
    *zKey = NULL;
    *nKey = !!sqlite3PagerGetCodec(sqlite3BtreePager(db->aDb[nDb].pBt));
}

int sqlite3CodecAttach(sqlite3 *db, int nDb, const void *zKey, int nKey)
{
    int rc;
    Codec *codec;
    Btree *pBt = db->aDb[nDb].pBt;

    rc = SQLITE_NOMEM;
    sqlite3_mutex_enter(db->mutex);
    if (!nKey) {
        /* Attach with an empty key (no encryption) */
        codec_set_to(NULL, pBt);
        rc = codec_verify_page1(NULL, pBt);
    } else if (zKey) {
        /* Attach with the provided key */
        if ((codec = codec_new(zKey, nKey)))
            rc = codec_set_to(codec, pBt);
    } else if (nDb != 0) {
        /* Use the main database's encryption */
        codec = sqlite3PagerGetCodec(sqlite3BtreePager(db->aDb[0].pBt));
        if (codec && (codec = codec_dup(codec))) {
            rc = codec_set_to(codec, pBt);
        } else {
            /* Main database codec unavailable */
            rc = SQLITE_CANTOPEN;
        }
    }
    sqlite3_mutex_leave(db->mutex);

    return rc;
}

/* Returns the main database if there is no match */
static int db_index_of(sqlite3 *db, const char *zDbName)
{
    int i;
    if (zDbName) {
        for (i = 0; i < db->nDb; i++) {
            if (!strcmp(db->aDb[i].zDbSName, zDbName))
                return i;
        }
    }
    return 0;
}

int sqlite3_key_v2(sqlite3 *db, const char *zDbName, const void *zKey, int nKey)
{
    return sqlite3CodecAttach(db, db_index_of(db, zDbName), zKey, nKey);
}

int sqlite3_key(sqlite3 *db, const void *zKey, int nKey)
{
    return sqlite3_key_v2(db, "main", zKey, nKey);
}

int sqlite3_rekey_v2(sqlite3 *db, const char *zDbName,
                     const void *zKey, int nKey)
{
    char *err;
    int nDb, rc;
    Btree *pBt;

    if (!db || (!nKey && !zKey))
        return SQLITE_ERROR;

    rc = SQLITE_ERROR;
    sqlite3_mutex_enter(db->mutex);
    if ((pBt = db->aDb[(nDb = db_index_of(db, zDbName))].pBt)) {
        Pgno pgno;
        DbPage *page;
        Codec *reader, *codec;
        Pager *pager = sqlite3BtreePager(pBt);

        reader = sqlite3PagerGetCodec(pager);
        if (!nKey) {
            /* Decrypt */
            if (reader) {
                reader->writer = NULL;
                rc = sqlite3RunVacuumForRekey(&err, db, nDb, 0);
                if (rc == SQLITE_OK) {
                    rc = codec_set_to(NULL, pBt);
                } else {
                    reader->writer = reader->reader;
                }
            } else {
                rc = codec_verify_page1(NULL, pBt);
            }
            goto leave;
        }

        /* Create a codec for the given key */
        if ((codec = codec_new(zKey, nKey))) {
            codec->pagesize = sqlite3BtreeGetPageSize(pBt);
            if ((codec->pagebuf = sqlite3_malloc(codec->pagesize))) {
                chacha20_rng(codec->salt, 16);
                codec_kdf(codec);
            }
        }
        if (!codec || !codec->pagebuf) {
            codec_free(codec);
            rc = SQLITE_NOMEM;
            goto leave;
        }

        if (!reader) {
            /* Encrypt */
            codec->reader = NULL;
            if ((rc = codec_set_to(codec, pBt)) == SQLITE_OK) {
                rc = sqlite3RunVacuumForRekey(&err, db, nDb, PAGE_RESERVED_LEN);
                if (rc == SQLITE_OK) {
                    codec->reader = codec->writer;
                } else {
                    codec_set_to(NULL, pBt);
                }
            }
            goto leave;
        }

        /* Change key (re-encrypt) */
        reader->writer = codec;
        rc = sqlite3BtreeBeginTrans(pBt, 1, NULL);
        for (pgno = 1; rc == SQLITE_OK && pgno <= pager->dbSize; pgno++) {
            /* The DB page occupied by the PENDING_BYTE is never used */
            if (pgno == PENDING_BYTE_PAGE(pager))
                continue;
            if ((rc = sqlite3PagerGet(pager, pgno, &page, 0)) == SQLITE_OK) {
                rc = sqlite3PagerWrite(page);
                sqlite3PagerUnref(page);
            }
        }
        if (rc == SQLITE_OK) {
            sqlite3BtreeCommit(pBt);
            rc = codec_set_to(codec, pBt);
        } else {
            reader->writer = reader;
            sqlite3BtreeRollback(pBt, SQLITE_ABORT_ROLLBACK, 0);
        }
    }

leave:
    sqlite3_mutex_leave(db->mutex);
    return rc;
}

int sqlite3_rekey(sqlite3 *db, const void *zKey, int nKey)
{
    return sqlite3_rekey_v2(db, "main", zKey, nKey);
}

void sqlite3_activate_see(const char *info)
{
}
