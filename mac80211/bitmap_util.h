#ifndef BITMAP_H
#define BITMAP_H


#include "ts-util.h"

#define BITMAP_MAX_SIZE 7

//#define bitmap_get_bit (b) ((b)->bitmap >> (b)->curr_pos) & 0x01;)




struct bitmap_t{
	unsigned char curr_pos;
	unsigned char bitmap;
	unsigned char chunk_length;
};


static inline  unsigned int
bitmap_has_next(struct bitmap_t *b)
{
	return b->curr_pos != BITMAP_MAX_SIZE;
}

static inline  unsigned char
bitmap_get_bit(struct bitmap_t *b)
{
	return (b->bitmap << b->curr_pos) & 0x80;
}



void bitmap_init(struct bitmap_t *b, unsigned char vbitmap){
	b->curr_pos = 0;
	b->bitmap = vbitmap;
	b->chunk_length = 0;
}




void bitmap_get_next(struct bitmap_t *b){
	unsigned char prev_bit, curr_bit;
	prev_bit = bitmap_get_bit(b);
	b->chunk_length = 0;
	while(b->curr_pos < BITMAP_MAX_SIZE){
		b->curr_pos++;
		curr_bit = bitmap_get_bit(b);
		if (curr_bit != prev_bit){
			b->chunk_length++;
			break;
		}
		else{
			b->chunk_length++;
		}
	}
}


#endif
