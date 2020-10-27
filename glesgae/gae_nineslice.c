#include "gae.h"

gae_nineslice_t* gae_nineslice_init(gae_nineslice_t* nineslice, gae_nineslice_dims_t* const dims, gae_graphics_texture_t* const texture)
{
	unsigned int i;
	unsigned int pX = dims->x[GAE_NINESLICE_U];
	unsigned int pL = dims->x[GAE_NINESLICE_L];
	unsigned int pR = dims->x[GAE_NINESLICE_R];
	unsigned int pW = dims->x[GAE_NINESLICE_W];
	
	unsigned int wX = pL - pX;
	unsigned int wL = pR - wX;
	unsigned int wR = pW - (pR - pX);
	
	unsigned int pY = dims->y[GAE_NINESLICE_V];
	unsigned int pT = dims->y[GAE_NINESLICE_T];
	unsigned int pB = dims->y[GAE_NINESLICE_B];
	unsigned int pH = dims->y[GAE_NINESLICE_H];
	
	unsigned int hY = pT - pY;
	unsigned int hT = pB - hY;
	unsigned int hB = pH - (pB - pY);
	
	nineslice->texture = texture;
	
	for (i = 0; i < gae_nineslice_count; ++i) {
		gae_frame_t* frame = &(nineslice->frames[i]);
		gae_rect_t* rect = &(nineslice->dimensions[i]);
		
		frame->texture = nineslice->texture;
		switch (i) {
			case gae_nineslice_top_left: {
				frame->src.x = pX; 	frame->src.y = pY;
				frame->src.w = wX; 	frame->src.h = hY;
				rect->x = pX - pX; 	rect->y = pY - pY;
				rect->w = wX; 		rect->h = hY;
			};
			break;
			case gae_nineslice_top: {
				frame->src.x = pL; 	frame->src.y = pY;
				frame->src.w = wL; 	frame->src.h = hY;
				rect->x = pL - pX; 	rect->y = pY - pY;
				rect->w = wL; 		rect->h = hY;
			};
			break;
			case gae_nineslice_top_right: {
				frame->src.x = pR; 	frame->src.y = pY;
				frame->src.w = wR; 	frame->src.h = hY;
				rect->x = pR - pX; 	rect->y = pY - pY;
				rect->w = wR; 		rect->h = hY;
			};
			break;
			
			case gae_nineslice_centre_left: {
				frame->src.x = pX; 	frame->src.y = pT;
				frame->src.w = wX; 	frame->src.h = hT - pY;
				rect->x = pX - pX; 	rect->y = pT - pY;
				rect->w = wX; 		rect->h = hT - pY;
			};
			break;
			case gae_nineslice_centre: {
				frame->src.x = pL; 	frame->src.y = pT;
				frame->src.w = wL; 	frame->src.h = hT - pY;
				rect->x = pL - pX; 	rect->y = pT - pY;
				rect->w = wL; 		rect->h = hT - pY;
			};
			break;
			case gae_nineslice_centre_right: {
				frame->src.x = pR; 	frame->src.y = pT;
				frame->src.w = wR; 	frame->src.h = hT - pY;
				rect->x = pR - pX; 	rect->y = pT - pY;
				rect->w = wR; 		rect->h = hT - pY;
			};
			break;
			
			case gae_nineslice_bottom_left: {
				frame->src.x = pX; 	frame->src.y = pB;
				frame->src.w = wX; 	frame->src.h = hB - pY;
				rect->x = pX - pX; 	rect->y = pB - pY;
				rect->w = wX; 		rect->h = hB - pY;
			};
			break;
			case gae_nineslice_bottom: {
				frame->src.x = pL; 	frame->src.y = pB;
				frame->src.w = wL; 	frame->src.h = hB - pY;
				rect->x = pL - pX; 	rect->y = pB - pY;
				rect->w = wL; 		rect->h = hB - pY;
			};
			break;
			case gae_nineslice_bottom_right: {
				frame->src.x = pR; 	frame->src.y = pB;
				frame->src.w = wR; 	frame->src.h = hB - pY;
				rect->x = pR - pX; 	rect->y = pB - pY;
				rect->w = wR; 		rect->h = hB - pY;
			};
			break;
			default:
			break;
		};
	}
	
	nineslice->dst.x = 0; nineslice->dst.y = 0;
	nineslice->dst.w = wX + wL + wR; nineslice->dst.h = hY + hT + hB;
	nineslice->scale.x = 1.0F;
	nineslice->scale.y = 1.0F;
	
	return nineslice;
}

gae_nineslice_t* gae_nineslice_render(gae_nineslice_t* const nineslice)
{
	unsigned int i;
	
	for (i = 0; i < gae_nineslice_count; ++i) {
		gae_rect_t pos = nineslice->dst;
		pos.x += (int)(nineslice->dimensions[i].x * nineslice->scale.x);
		pos.y += (int)(nineslice->dimensions[i].y * nineslice->scale.y);
		pos.w = (int)(nineslice->dimensions[i].w * nineslice->scale.x);
		pos.h = (int)(nineslice->dimensions[i].h * nineslice->scale.y);
		gae_graphics_context_blit_texture(gae_system.graphics.context, nineslice->texture, &(nineslice->frames[i].src), &pos);
	}
	
	return nineslice;
}

gae_nineslice_t* gae_nineslice_rescale(gae_nineslice_t* nineslice, unsigned int width, unsigned int height)
{
	nineslice->scale.x = (float)(width / nineslice->dst.w);
	nineslice->scale.y = (float)(height / nineslice->dst.h);
	
	return nineslice;
}

gae_nineslice_t* gae_nineslice_resize(gae_nineslice_t* nineslice, unsigned int width, unsigned int height)
{
	gae_rect_t* topleft = &(nineslice->dimensions[gae_nineslice_top_left]);
	gae_rect_t* top = &(nineslice->dimensions[gae_nineslice_top]);
	gae_rect_t* topright = &(nineslice->dimensions[gae_nineslice_top_right]);
	gae_rect_t* centreleft = &(nineslice->dimensions[gae_nineslice_centre_left]);
	gae_rect_t* centre = &(nineslice->dimensions[gae_nineslice_centre]);
	gae_rect_t* centreright = &(nineslice->dimensions[gae_nineslice_centre_right]);
	gae_rect_t* bottomleft = &(nineslice->dimensions[gae_nineslice_bottom_left]);
	gae_rect_t* bottom = &(nineslice->dimensions[gae_nineslice_bottom]);
	gae_rect_t* bottomright = &(nineslice->dimensions[gae_nineslice_bottom_right]);
	
	int diffX = nineslice->dst.w - width;
	int diffY = nineslice->dst.h - height;
	
	top->w -= diffX;
	topright->x -= diffX;
	
	centreleft->h -= diffY;
	centre->w -= diffX;
	centre->h -= diffY;
	centreright->x -= diffX;
	centreright->h -= diffY;
	
	bottomleft->y -= diffY;
	bottom->y -= diffY;
	bottom->w -= diffX;
	bottomright->x -= diffX;
	bottomright->y -= diffY;
	
	nineslice->dst.w = topleft->w + top->w + topright->w;
	nineslice->dst.h = topright->h + centreright->h + bottomright->h;
	
	return nineslice;
}

gae_nineslice_t* gae_nineslice_destroy(gae_nineslice_t* nineslice)
{
	return nineslice;
}
