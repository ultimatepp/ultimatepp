// Solution to our pre-interview test.

#include <stdlib.h>
#include <stdio.h>

// Macros I can't do without anymore.

#define hope(x)     while ( ! (x) ) { onIllegalOperation() ; }
#define require(x)  typedef char require[ (x) ? 1 : -1 ] ;

// Error handlers.

#define onOutOfMemory()         do{ fprintf( stderr, "Out of memory!\n" ) ; exit( EXIT_FAILURE ) ; } while( 0 )
#define onIllegalOperation()    do{ fprintf( stderr, "Illegal Operation!\n" ) ; exit( EXIT_FAILURE ) ; } while( 0 )

// Typedefs.

typedef unsigned char byte ;
typedef unsigned int uint ;

struct Q {
    uint first ;
    uint last ;
} ;

// Constants.

const uint BLOCK_SIZE = 8 ;
const uint BLOCK_MASK = BLOCK_SIZE - 1 ;
const uint BLOCK_LAST = BLOCK_SIZE - 1 ;
const uint TOTAL_SIZE = 2048 ;
const uint BLOCK_COUNT = TOTAL_SIZE / BLOCK_SIZE ;

// The provided memory, and appropriate aliasing.
//
// Everything is split to 256 8byte blocks.
//
// Blocks 1-255 are used for both queues and data, block 0 for book-keeping of free blocks.
// data[ 0 ] maintains single linked list of free blocks, while
// data[ 1 ] keeps the index of the last block ever allocated.
// Both used and free blocks are linked via their last byte.
//
// Note that array requires alignment to uint on some architectures,
// which is simple to achieve with "aligned" attribute if necessary.
// The alternative would be to use aligned memory within it ourselves.

byte data[ TOTAL_SIZE ] ;

#define queues ((Q *)(data))

require( sizeof( Q ) == BLOCK_SIZE ) ;

// Block management.

static byte allocate_block( void )
{
    // Use one of the freed blocks, if possible.

    byte block = data[ 0 ] ;
    if ( block ) {
        data[ 0 ] = data[ block * BLOCK_SIZE + BLOCK_LAST ] ;
        return block ;
    }

    // Otherwise grab new block from the array, if possible.
    //
    // We can use an arguable trick that it will overflow in case we run out of blocks.
    // This may be considered either cool or entirely intorelable, depending on the point of view.

    require( BLOCK_COUNT == 256 ) ;

    block = ++data[ 1 ] ;
    if ( block ) {
        return block ;
    }

    // Well, bad luck.

    onOutOfMemory() ;
}

static void free_block( const byte block )
{
    hope( block > 0 ) ;
    hope( block < BLOCK_COUNT ) ;

    data[ block * BLOCK_SIZE + BLOCK_LAST ] = data[ 0 ] ;
    data[ 0 ] = block ;
}

// Queue methods themselves, each O(1).

Q * createQ( void )
{
    Q * const q = queues + allocate_block() ;
    q->first = q->last = allocate_block() * BLOCK_SIZE ;
    return q ;
}

void destroyQ( Q * q )
{
    hope( q ) ;

    data[ q->last | BLOCK_MASK ] = data[ 0 ] ;
    data[ 0 ] = q->first / BLOCK_SIZE ;

    free_block( q - queues ) ;
}

void enQ( Q * q, byte n )
{
    hope( q ) ;

    if ( q->last & BLOCK_MASK == BLOCK_LAST ) {
        const byte block = allocate_block() ;
        data[ q->last ] = block ;
        q->last = block * BLOCK_SIZE ;
    }
    data[ q->last++ ] = n ;
}

byte deQ( Q * q )
{
    hope( q ) ;

    if ( q->first == q->last ) {
        onIllegalOperation() ;
    }
    if ( q->first & BLOCK_MASK == BLOCK_LAST ) {
        const byte block = data[ q->first ] ;
        free_block( q->first / BLOCK_SIZE ) ;
        q->first = block * BLOCK_SIZE ;
    }
    return data[ q->first++ ] ;
}

void TestPatrik()
{
	Q *q0 = createQ();
	for(int i = 0; i < 3000; i++) {
		DDUMP(i);
		enQ(q0, 0);
	}
}
