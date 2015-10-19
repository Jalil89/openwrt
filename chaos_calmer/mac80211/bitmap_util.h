#ifndef BITMAP_H
#define BITMAP_H


#include "ts-util.h"

#define BITMAP_MAX_SIZE 7

//#define bitmap_get_bit (b) ((b)->bitmap >> (b)->curr_pos) & 0x01;)




struct bitmap_t{
	unsigned char curr_pos;
	unsigned char priority;
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

static inline  int
bitmap_is_high_priority(struct bitmap_t *b)
{
	/*
	if (b->priority == 1){
		return 1;
	}
	else if (b->priority == 0){
		return 0;
	}
	else{
		printk(KERN_ALERT "priority bit is %u", (unsigned int)b->priority);
	}
	*/

	//return b->priority != 0;
	return b->bitmap != 0;
}



void bitmap_init(struct bitmap_t *b, unsigned char vbitmap){
	b->curr_pos = 0;
	b->priority = 0;
	b->bitmap = vbitmap;
	b->chunk_length = 0;
}




void bitmap_get_next(struct bitmap_t *b, unsigned int opt_size){
	unsigned char prev_bit, curr_bit;
	prev_bit = bitmap_get_bit(b);
	b->chunk_length = 0;
	while(b->curr_pos < BITMAP_MAX_SIZE){
		b->chunk_length++;
		b->priority = prev_bit;

		// advance to the next one
		b->curr_pos++;
		curr_bit = bitmap_get_bit(b);
		//if (curr_bit != prev_bit || b->chunk_length*188 > opt_size){
		if (b->chunk_length*188 > opt_size){
			break;
		}

	}
}


#endif
