#include "includes.h"
#include <gmp.h>
#include <xmalloc.h>

int rsa_verbose = 1;

#define MAX_PRIMES_IN_TABLE 1050 /* must be more than # primes */

static const unsigned int small_primes[MAX_PRIMES_IN_TABLE + 1] =
{   /* 2 is eliminated by trying only odd numbers. */
  3, 5, 7, 11, 13, 17, 19,
  23, 29, 31, 37, 41, 43, 47, 53,
  59, 61, 67, 71, 73, 79, 83, 89,
  97, 101, 103, 107, 109, 113, 127, 131,
  137, 139, 149, 151, 157, 163, 167, 173,
  179, 181, 191, 193, 197, 199, 211, 223,
  227, 229, 233, 239, 241, 251, 257, 263,
  269, 271, 277, 281, 283, 293, 307, 311,
  313, 317, 331, 337, 347, 349, 353, 359,
  367, 373, 379, 383, 389, 397, 401, 409,
  419, 421, 431, 433, 439, 443, 449, 457,
  461, 463, 467, 479, 487, 491, 499, 503,
  509, 521, 523, 541, 547, 557, 563, 569,
  571, 577, 587, 593, 599, 601, 607, 613,
  617, 619, 631, 641, 643, 647, 653, 659,
  661, 673, 677, 683, 691, 701, 709, 719,
  727, 733, 739, 743, 751, 757, 761, 769,
  773, 787, 797, 809, 811, 821, 823, 827,
  829, 839, 853, 857, 859, 863, 877, 881,

  883, 887, 907, 911, 919, 929, 937, 941,

  947, 953, 967, 971, 977, 983, 991, 997,

  1009, 1013, 1019, 1021, 1031, 1033, 1039, 1049,

  1051, 1061, 1063, 1069, 1087, 1091, 1093, 1097,

  1103, 1109, 1117, 1123, 1129, 1151, 1153, 1163,

  1171, 1181, 1187, 1193, 1201, 1213, 1217, 1223,

  1229, 1231, 1237, 1249, 1259, 1277, 1279, 1283,

  1289, 1291, 1297, 1301, 1303, 1307, 1319, 1321,

  1327, 1361, 1367, 1373, 1381, 1399, 1409, 1423,

  1427, 1429, 1433, 1439, 1447, 1451, 1453, 1459,

  1471, 1481, 1483, 1487, 1489, 1493, 1499, 1511,

  1523, 1531, 1543, 1549, 1553, 1559, 1567, 1571,

  1579, 1583, 1597, 1601, 1607, 1609, 1613, 1619,

  1621, 1627, 1637, 1657, 1663, 1667, 1669, 1693,

  1697, 1699, 1709, 1721, 1723, 1733, 1741, 1747,

  1753, 1759, 1777, 1783, 1787, 1789, 1801, 1811,

  1823, 1831, 1847, 1861, 1867, 1871, 1873, 1877,

  1879, 1889, 1901, 1907, 1913, 1931, 1933, 1949,

  1951, 1973, 1979, 1987, 1993, 1997, 1999, 2003,

  2011, 2017, 2027, 2029, 2039, 2053, 2063, 2069,

  2081, 2083, 2087, 2089, 2099, 2111, 2113, 2129,

  2131, 2137, 2141, 2143, 2153, 2161, 2179, 2203,

  2207, 2213, 2221, 2237, 2239, 2243, 2251, 2267,

  2269, 2273, 2281, 2287, 2293, 2297, 2309, 2311,

  2333, 2339, 2341, 2347, 2351, 2357, 2371, 2377,

  2381, 2383, 2389, 2393, 2399, 2411, 2417, 2423,

  2437, 2441, 2447, 2459, 2467, 2473, 2477, 2503,

  2521, 2531, 2539, 2543, 2549, 2551, 2557, 2579,

  2591, 2593, 2609, 2617, 2621, 2633, 2647, 2657,

  2659, 2663, 2671, 2677, 2683, 2687, 2689, 2693,

  2699, 2707, 2711, 2713, 2719, 2729, 2731, 2741,

  2749, 2753, 2767, 2777, 2789, 2791, 2797, 2801,

  2803, 2819, 2833, 2837, 2843, 2851, 2857, 2861,

  2879, 2887, 2897, 2903, 2909, 2917, 2927, 2939,

  2953, 2957, 2963, 2969, 2971, 2999, 3001, 3011,

  3019, 3023, 3037, 3041, 3049, 3061, 3067, 3079,

  3083, 3089, 3109, 3119, 3121, 3137, 3163, 3167,

  3169, 3181, 3187, 3191, 3203, 3209, 3217, 3221,

  3229, 3251, 3253, 3257, 3259, 3271, 3299, 3301,

  3307, 3313, 3319, 3323, 3329, 3331, 3343, 3347,

  3359, 3361, 3371, 3373, 3389, 3391, 3407, 3413,

  3433, 3449, 3457, 3461, 3463, 3467, 3469, 3491,

  3499, 3511, 3517, 3527, 3529, 3533, 3539, 3541,

  3547, 3557, 3559, 3571, 3581, 3583, 3593, 3607,

  3613, 3617, 3623, 3631, 3637, 3643, 3659, 3671,

  3673, 3677, 3691, 3697, 3701, 3709, 3719, 3727,

  3733, 3739, 3761, 3767, 3769, 3779, 3793, 3797,

  3803, 3821, 3823, 3833, 3847, 3851, 3853, 3863,

  3877, 3881, 3889, 3907, 3911, 3917, 3919, 3923,

  3929, 3931, 3943, 3947, 3967, 3989, 4001, 4003,

  4007, 4013, 4019, 4021, 4027, 4049, 4051, 4057,

  4073, 4079, 4091, 4093, 4099, 4111, 4127, 4129,

  4133, 4139, 4153, 4157, 4159, 4177, 4201, 4211,

  4217, 4219, 4229, 4231, 4241, 4243, 4253, 4259,

  4261, 4271, 4273, 4283, 4289, 4297, 4327, 4337,

  4339, 4349, 4357, 4363, 4373, 4391, 4397, 4409,

  4421, 4423, 4441, 4447, 4451, 4457, 4463, 4481,

  4483, 4493, 4507, 4513, 4517, 4519, 4523, 4547,

  4549, 4561, 4567, 4583, 4591, 4597, 4603, 4621,

  4637, 4639, 4643, 4649, 4651, 4657, 4663, 4673,

  4679, 4691, 4703, 4721, 4723, 4729, 4733, 4751,

  4759, 4783, 4787, 4789, 4793, 4799, 4801, 4813,

  4817, 4831, 4861, 4871, 4877, 4889, 4903, 4909,

  4919, 4931, 4933, 4937, 4943, 4951, 4957, 4967,

  4969, 4973, 4987, 4993, 4999, 5003, 5009, 5011,

  5021, 5023, 5039, 5051, 5059, 5077, 5081, 5087,

  5099, 5101, 5107, 5113, 5119, 5147, 5153, 5167,

  5171, 5179, 5189, 5197, 5209, 5227, 5231, 5233,

  5237, 5261, 5273, 5279, 5281, 5297, 5303, 5309,

  5323, 5333, 5347, 5351, 5381, 5387, 5393, 5399,

  5407, 5413, 5417, 5419, 5431, 5437, 5441, 5443,

  5449, 5471, 5477, 5479, 5483, 5501, 5503, 5507,

  5519, 5521, 5527, 5531, 5557, 5563, 5569, 5573,

  5581, 5591, 5623, 5639, 5641, 5647, 5651, 5653,

  5657, 5659, 5669, 5683, 5689, 5693, 5701, 5711,

  5717, 5737, 5741, 5743, 5749, 5779, 5783, 5791,

  5801, 5807, 5813, 5821, 5827, 5839, 5843, 5849,

  5851, 5857, 5861, 5867, 5869, 5879, 5881, 5897,

  5903, 5923, 5927, 5939, 5953, 5981, 5987, 6007,

  6011, 6029, 6037, 6043, 6047, 6053, 6067, 6073,

  6079, 6089, 6091, 6101, 6113, 6121, 6131, 6133,

  6143, 6151, 6163, 6173, 6197, 6199, 6203, 6211,

  6217, 6221, 6229, 6247, 6257, 6263, 6269, 6271,

  6277, 6287, 6299, 6301, 6311, 6317, 6323, 6329,

  6337, 6343, 6353, 6359, 6361, 6367, 6373, 6379,

  6389, 6397, 6421, 6427, 6449, 6451, 6469, 6473,

  6481, 6491, 6521, 6529, 6547, 6551, 6553, 6563,

  6569, 6571, 6577, 6581, 6599, 6607, 6619, 6637,

  6653, 6659, 6661, 6673, 6679, 6689, 6691, 6701,

  6703, 6709, 6719, 6733, 6737, 6761, 6763, 6779,

  6781, 6791, 6793, 6803, 6823, 6827, 6829, 6833,

  6841, 6857, 6863, 6869, 6871, 6883, 6899, 6907,

  6911, 6917, 6947, 6949, 6959, 6961, 6967, 6971,

  6977, 6983, 6991, 6997, 7001, 7013, 7019, 7027,

  7039, 7043, 7057, 7069, 7079, 7103, 7109, 7121,

  7127, 7129, 7151, 7159, 7177, 7187, 7193, 7207,

  7211, 7213, 7219, 7229, 7237, 7243, 7247, 7253,

  7283, 7297, 7307, 7309, 7321, 7331, 7333, 7349,

  7351, 7369, 7393, 7411, 7417, 7433, 7451, 7457,

  7459, 7477, 7481, 7487, 7489, 7499, 7507, 7517,

  7523, 7529, 7537, 7541, 7547, 7549, 7559, 7561,

  7573, 7577, 7583, 7589, 7591, 7603, 7607, 7621,

  7639, 7643, 7649, 7669, 7673, 7681, 7687, 7691,

  7699, 7703, 7717, 7723, 7727, 7741, 7753, 7757,

  7759, 7789, 7793, 7817, 7823, 7829, 7841, 7853,

  7867, 7873, 7877, 7879, 7883, 7901, 7907, 7919,

  7927, 7933, 7937, 7949, 7951, 7963, 7993, 8009,

  8011, 8017, 8039, 8053, 8059, 8069, 8081, 8087,

  8089, 8093, 8101, 8111, 8117, 8123, 8147, 8161,

  8167, 8171, 8179, 8191,

  0};



/* Generate a random number of the desired number of bits.  */



void rsa_random_integer(MP_INT *ret, RandomState *state, unsigned int bits)

{

  unsigned int bytes = (bits + 7) / 8;

  char *str = xmalloc(bytes * 2 + 1);

  unsigned int i;



  /* We first create a random hex number of the desired size, and then

     convert it to a mp-int. */

  for (i = 0; i < bytes; i++)

    sprintf(str + 2 * i, "%02x", random_get_byte(state));



  /* Convert it to the internal representation. */

  if (mpz_set_str(ret, str, 16) < 0)

    fatal("Intenal error, mpz_set_str returned error");



  /* Clear extra data. */

  memset(str, 0, 2 * bytes);

  xfree(str);



  /* Reduce it to the desired number of bits. */

  mpz_mod_2exp(ret, ret, bits);

}



/* Returns a prime number of the specified number of bits.  The number

   will have the highest bit set and two lowest bits set. */



void rsa_random_prime(MP_INT *ret, RandomState *state, unsigned int bits)

{

  MP_INT start, aux;

  unsigned int num_primes;

  int *moduli;

  long difference;



  mpz_init(&start);

  mpz_init(&aux);



 retry:



  /* Pick a random integer of the appropriate size. */

  rsa_random_integer(&start, state, bits);



  /* Set the two highest bits. */

  mpz_set_ui(&aux, 3);

  mpz_mul_2exp(&aux, &aux, bits - 2);

  mpz_ior(&start, &start, &aux);

  /* Set the lowest bit to make it odd. */

  mpz_set_ui(&aux, 1);

  mpz_ior(&start, &start, &aux);



  /* Initialize moduli of the small primes with respect to the given

     random number. */

  moduli = xmalloc(MAX_PRIMES_IN_TABLE * sizeof(moduli[0]));

  if (bits < 16)

    num_primes = 0; /* Don\'t use the table for very small numbers. */

  else

    {

      for (num_primes = 0; small_primes[num_primes] != 0; num_primes++)

       {

         mpz_mod_ui(&aux, &start, small_primes[num_primes]);

         moduli[num_primes] = mpz_get_ui(&aux);

       }

    }



  /* Look for numbers that are not evenly divisible by any of the small

     primes. */

  for (difference = 0; ; difference += 2)

    {

      unsigned int i;



      if (difference > 0x70000000)

       { /* Should never happen, I think... */

         if (rsa_verbose)

           fprintf(stderr,

                  "rsa_random_prime: failed to find a prime, retrying.\n");

         xfree(moduli);

         goto retry;

       }



      /* Check if it is a multiple of any small prime.  Note that this

        updates the moduli into negative values as difference grows. */

      for (i = 0; i < num_primes; i++)

       {

         while (moduli[i] + difference >= small_primes[i])

           moduli[i] -= small_primes[i];

         if (moduli[i] + difference == 0)

           break;

       }

      if (i < num_primes)

       continue; /* Multiple of a known prime. */



      /* It passed the small prime test (not divisible by any of them). */

      if (rsa_verbose)

       {

         fprintf(stderr, ".");

       }



      /* Compute the number in question. */

      mpz_add_ui(ret, &start, difference);



      /* Perform the fermat test for witness 2.  This means:

        it is not prime if 2^n mod n != 2. */

      mpz_set_ui(&aux, 2);

      mpz_powm(&aux, &aux, ret, ret);

      if (mpz_cmp_ui(&aux, 2) == 0)

       {

         /* Passed the fermat test for witness 2. */

         if (rsa_verbose)

           {

             fprintf(stderr, "+");

           }

         /* Perform a more tests.  These are probably unnecessary. */

         if (mpz_probab_prime_p(ret, 20))

           break; /* It is a prime with probability 1 - 2^-40. */

       }

    }



  /* Found a (probable) prime.  It is in ret. */

  if (rsa_verbose)

    {

      fprintf(stderr, "+ (distance %ld)\n", difference);

    }



  /* Free the small prime moduli; they are no longer needed. */

  xfree(moduli);



  /* Sanity check: does it still have the high bit set (we might have

     wrapped around)? */

  mpz_div_2exp(&aux, ret, bits - 1);

  if (mpz_get_ui(&aux) != 1)

    {

      if (rsa_verbose)

       fprintf(stderr, "rsa_random_prime: high bit not set, retrying.\n");

      goto retry;

    }

  mpz_clear(&start);

  mpz_clear(&aux);

  /* Return value already set in ret. */

}



/* Computes the multiplicative inverse of a number using Euclids algorithm.

   Computes x such that a * x mod n = 1, where 0 < a < n. */



static void mpz_mod_inverse(MP_INT *x, MP_INT *a, MP_INT *n)

{

  MP_INT g0, g1, v0, v1, div, mod, aux;

  mpz_init_set(&g0, n);

  mpz_init_set(&g1, a);

  mpz_init_set_ui(&v0, 0);

  mpz_init_set_ui(&v1, 1);

  mpz_init(&div);

  mpz_init(&mod);

  mpz_init(&aux);

  while (mpz_cmp_ui(&g1, 0) != 0)

    {

      mpz_divmod(&div, &mod, &g0, &g1);

      mpz_mul(&aux, &div, &v1);

      mpz_sub(&aux, &v0, &aux);

      mpz_set(&v0, &v1);

      mpz_set(&v1, &aux);

      mpz_set(&g0, &g1);

      mpz_set(&g1, &mod);

    }

  if (mpz_cmp_ui(&v0, 0) < 0)

    mpz_add(x, &v0, n);

  else

    mpz_set(x, &v0);



  mpz_clear(&g0);

  mpz_clear(&g1);

  mpz_clear(&v0);

  mpz_clear(&v1);

  mpz_clear(&div);

  mpz_clear(&mod);

  mpz_clear(&aux);

}



/* Given mutual primes p and q, derives RSA key components n, e, d, and u.

   The exponent e will be at least ebits bits in size.

   p must be smaller than q. */



static void derive_rsa_keys(MP_INT *n, MP_INT *e, MP_INT *d, MP_INT *u,

                         MP_INT *p, MP_INT *q,

                         unsigned int ebits)

{

  MP_INT p_minus_1, q_minus_1, aux, phi, G, F;



  assert(mpz_cmp(p, q) < 0);



  mpz_init(&p_minus_1);

  mpz_init(&q_minus_1);

  mpz_init(&aux);

  mpz_init(&phi);

  mpz_init(&G);

  mpz_init(&F);



  /* Compute p-1 and q-1. */

  mpz_sub_ui(&p_minus_1, p, 1);

  mpz_sub_ui(&q_minus_1, q, 1);



  /* phi = (p - 1) * (q - 1); the number of positive integers less than p*q

     that are relatively prime to p*q. */

  mpz_mul(&phi, &p_minus_1, &q_minus_1);



  /* G is the number of "spare key sets" for a given modulus n.  The smaller

     G is, the better.  The smallest G can get is 2. */

  mpz_gcd(&G, &p_minus_1, &q_minus_1);



  if (rsa_verbose)

    {

      if (mpz_cmp_ui(&G, 100) >= 0)

       {

         fprintf(stderr, "Warning: G=");

         mpz_out_str(stdout, 10, &G);

         fprintf(stderr, " is large (many spare key sets); key may be bad!\n");

       }

    }



  /* F = phi / G; the number of relative prime numbers per spare key set. */

  mpz_div(&F, &phi, &G);



  /* Find a suitable e (the public exponent). */

  mpz_set_ui(e, 1);

  mpz_mul_2exp(e, e, ebits);

  mpz_sub_ui(e, e, 1); /* make lowest bit 1, and substract 2. */

  /* Keep adding 2 until it is relatively prime to (p-1)(q-1). */

  do

    {

      mpz_add_ui(e, e, 2);

      mpz_gcd(&aux, e, &phi);

    }

  while (mpz_cmp_ui(&aux, 1) != 0);



  /* d is the multiplicative inverse of e, mod F.  Could also be mod

     (p-1)(q-1); however, we try to choose the smallest possible d. */

  mpz_mod_inverse(d, e, &F);



  /* u is the multiplicative inverse of p, mod q, if p < q.  It is used

     when doing private key RSA operations using the chinese remainder

     theorem method. */

  mpz_mod_inverse(u, p, q);



  /* n = p * q (the public modulus). */

  mpz_mul(n, p, q);



  /* Clear auxiliary variables. */

  mpz_clear(&p_minus_1);

  mpz_clear(&q_minus_1);

  mpz_clear(&aux);

  mpz_clear(&phi);

  mpz_clear(&G);

  mpz_clear(&F);

}



/* Generates RSA public and private keys.  This initializes the data

   structures; they should be freed with rsa_clear_private_key and

   rsa_clear_public_key. */



void rsa_generate_key(RSAPrivateKey *prv, RSAPublicKey *pub,

                    RandomState *state, unsigned int bits)

{

  MP_INT test, aux;

  unsigned int pbits, qbits;

  int ret;



  mpz_init(&prv->q);

  mpz_init(&prv->p);

  mpz_init(&prv->e);

  mpz_init(&prv->d);

  mpz_init(&prv->u);

  mpz_init(&prv->n);

  mpz_init(&test);

  mpz_init(&aux);



  /* Compute the number of bits in each prime. */

  pbits = bits / 2;

  qbits = bits - pbits;



#ifndef RSAREF

 retry0:

#endif /* !RSAREF */



  if (rsa_verbose)

    {

      fprintf(stderr, "Generating p:  ");

    }



  /* Generate random number p. */

  rsa_random_prime(&prv->p, state, pbits);



 retry:



  if (rsa_verbose)

    {

      fprintf(stderr, "Generating q:  ");

    }



  /* Generate random number q. */

  rsa_random_prime(&prv->q, state, qbits);



  /* Sort them so that p < q. */

  ret = mpz_cmp(&prv->p, &prv->q);

  if (ret == 0)

    {

      if (rsa_verbose)

       fprintf(stderr, "Generated the same prime twice!\n");

      goto retry;

    }

  if (ret > 0)

    {

      mpz_set(&aux, &prv->p);

      mpz_set(&prv->p, &prv->q);

      mpz_set(&prv->q, &aux);

    }



  /* Make sure that p and q are not too close together (I am not sure if this

     is important). */

  mpz_sub(&aux, &prv->q, &prv->p);

  mpz_div_2exp(&test, &prv->q, 10);

  if (mpz_cmp(&aux, &test) < 0)

    {

      if (rsa_verbose)

       fprintf(stderr, "The primes are too close together.\n");

      goto retry;

    }



  /* Make certain p and q are relatively prime (in case one or both were false

     positives...  Though this is quite impossible). */

  mpz_gcd(&aux, &prv->p, &prv->q);

  if (mpz_cmp_ui(&aux, 1) != 0)

    {

      if (rsa_verbose)

       fprintf(stderr, "The primes are not relatively prime!\n");

      goto retry;

    }



  /* Derive the RSA private key from the primes. */

  if (rsa_verbose)

    fprintf(stderr, "Computing the keys...\n");

  derive_rsa_keys(&prv->n, &prv->e, &prv->d, &prv->u, &prv->p, &prv->q, 5);

  prv->bits = bits;



  /* Initialize the public key with public data from the private key. */

  pub->bits = bits;

  mpz_init_set(&pub->n, &prv->n);

  mpz_init_set(&pub->e, &prv->e);



#ifndef RSAREF /* I don't want to kludge these to work with RSAREF. */

  /* Test that the key really works.  This should never fail (I think). */

  if (rsa_verbose)

    fprintf(stderr, "Testing the keys...\n");

  rsa_random_integer(&test, state, bits);

  mpz_mod(&test, &test, &pub->n); /* must be less than n. */

  rsa_private(&aux, &test, prv);

  rsa_public(&aux, &aux, pub);

  if (mpz_cmp(&aux, &test) != 0)

    {

      if (rsa_verbose)

       fprintf(stderr, "**** private+public failed to decrypt.\n");

      goto retry0;

    }



  rsa_public(&aux, &test, pub);

  rsa_private(&aux, &aux, prv);

  if (mpz_cmp(&aux, &test) != 0)

    {

      if (rsa_verbose)

       fprintf(stderr, "**** public+private failed to decrypt.\n");

      goto retry0;

    }

#endif /* !RSAREF */



  mpz_clear(&aux);

  mpz_clear(&test);



  if (rsa_verbose)

    fprintf(stderr, "Key generation complete.\n");

}



/* Frees any memory associated with the private key. */



void rsa_clear_private_key(RSAPrivateKey *prv)

{

  prv->bits = 0;

  mpz_clear(&prv->n);

  mpz_clear(&prv->e);

  mpz_clear(&prv->d);

  mpz_clear(&prv->u);

  mpz_clear(&prv->p);

  mpz_clear(&prv->q);

}



/* Frees any memory associated with the public key. */



void rsa_clear_public_key(RSAPublicKey *pub)

{

  pub->bits = 0;

  mpz_clear(&pub->e);

  mpz_clear(&pub->n);

}



#ifndef RSAREF



/* Performs a private-key RSA operation (encrypt/decrypt).  The computation

   is done using the Chinese Remainder Theorem, which is faster than

   direct modular exponentiation. */



void rsa_private(MP_INT *output, MP_INT *input, RSAPrivateKey *prv)

{

  MP_INT dp, dq, p2, q2, k;



  /* Initialize temporary variables. */

  mpz_init(&dp);

  mpz_init(&dq);

  mpz_init(&p2);

  mpz_init(&q2);

  mpz_init(&k);



  /* Compute dp = d mod p-1. */

  mpz_sub_ui(&dp, &prv->p, 1);

  mpz_mod(&dp, &prv->d, &dp);



  /* Compute dq = d mod q-1. */

  mpz_sub_ui(&dq, &prv->q, 1);

  mpz_mod(&dq, &prv->d, &dq);



  /* Compute p2 = (input mod p) ^ dp mod p. */

  mpz_mod(&p2, input, &prv->p);

  mpz_powm(&p2, &p2, &dp, &prv->p);



  /* Compute q2 = (input mod q) ^ dq mod q. */

  mpz_mod(&q2, input, &prv->q);

  mpz_powm(&q2, &q2, &dq, &prv->q);



  /* Compute k = ((q2 - p2) mod q) * u mod q. */

  mpz_sub(&k, &q2, &p2);

  mpz_mul(&k, &k, &prv->u);

  mpz_mmod(&k, &k, &prv->q);



  /* Compute output = p2 + p * k. */

  mpz_mul(output, &prv->p, &k);

  mpz_add(output, output, &p2);



  /* Clear temporary variables. */

  mpz_clear(&dp);

  mpz_clear(&dq);

  mpz_clear(&p2);

  mpz_clear(&q2);

  mpz_clear(&k);

}



/* Performs a public-key RSA operation (encrypt/decrypt). */



void rsa_public(MP_INT *output, MP_INT *input, RSAPublicKey *pub)

{

  mpz_powm(output, input, &pub->e, &pub->n);

}



#endif /* !RSAREF */



/* Special realloc that zeroes the old memory before freeing it. */



static void *rsa_realloc(void *ptr, size_t old_size, size_t new_size)

{

  int s;

  void *p = xmalloc(new_size);

  s = old_size;

  if (old_size > new_size)

    s = new_size;

  memcpy(p, ptr, s);

  memset(ptr, 0, old_size);

  xfree(ptr);

  return p;

}



/* Special free that zeroes the memory before freeing it. */



static void rsa_free(void *ptr, size_t size)

{

  memset(ptr, 0, size);

  xfree(ptr);

}



/* Sets MP_INT memory allocation routines to ones that clear any memory

   when freed. */



void rsa_set_mp_memory_allocation(void)

{

  mp_set_memory_functions(xmalloc, rsa_realloc, rsa_free);

}



/* Set whether to output verbose messages during key generation. */



void rsa_set_verbose(int verbose)

{

  rsa_verbose = verbose;

}

/* program ends    */