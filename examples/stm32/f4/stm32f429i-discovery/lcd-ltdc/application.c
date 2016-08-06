/*
 * This file is part of the libopencm3 project.
 * 
 * Copyright (C) 2016 Oliver Meier <h2obrain@gmail.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <libopencm3/cm3/nvic.h>
#include <stdbool.h>
#include "clock.h"
#include "sdram.h"
#include "lcd_ili9341.h"
#include "Tamsyn5x9r_9.h"
#include "Tamsyn5x9b_9.h"

#include "application_balls.h"


#include "h2_bezier.h"

uint16_t color = GFX_COLOR_BLACK;
void draw_segment(point2d_t p1, point2d_t p2);
void draw_segment(point2d_t p1, point2d_t p2) {
	gfx_draw_line((int16_t)p1.x,(int16_t)p1.y, (int16_t)p2.x,(int16_t)p2.y, color);
}
point2d_t p[] = {
//		{ 161, 191 },
//		{ 132, 101 },
//		{ 209, 156 },
//		{ 113, 156 },
//		{ 190, 101 },
//
//		{ 161, 191 },
//		{ 132, 101 },
//		{ 209, 156 },
//		{ 113, 156 },
//		{ 190, 101 },
//
//		{ 161, 191 },
//		{ 132, 101 },
//		{ 209, 156 },
//		{ 113, 156 },
//		{ 190, 101 },
//
//		{ 161, 191 },


		{ 161, 166 },
		{ 146, 121 },
		{ 185, 149 },
		{ 137, 149 },
		{ 176, 121 },

		{ 161, 166 },
		{ 146, 121 },
		{ 185, 149 },
		{ 137, 149 },
		{ 176, 121 },

		{ 161, 166 },
		{ 146, 121 },
		{ 185, 149 },
		{ 137, 149 },
		{ 176, 121 },

		{ 161, 166 },

};
uint32_t num_points;
uint32_t num_ipoints;
static inline void init_bezier(void) {
	uint32_t i;

	num_points  = sizeof(p)/sizeof(p[0]);
	num_ipoints = h2_bezier_calculate_int_points_length(num_points);

	color = GFX_COLOR_GREEN;
	for (i=0; i<num_points-1; i++) {
		draw_segment(p[i],p[i+1]);
	}


//	point2d_t pi1[num_ipoints];
//	color = GFX_COLOR_BLUE;
//	h2_bezier_cubic(pi1, p, num_points, 1.0f, 1.0f);
//	for (i=0; i<num_ipoints-1; i+=3) {
//		h2_bezier_draw_cubic2(draw_segment, 10, pi1[i], pi1[i+1], pi1[i+2], pi1[i+3]);
//	}
//	color = GFX_COLOR_RED;
//	h2_bezier_cubic(pi1, p, num_points, 1.0f, 1.0f);
//	for (i=0; i<num_ipoints-1; i+=3) {
//		h2_bezier_draw_cubic(draw_segment, 10, pi1[i], pi1[i+1], pi1[i+2], pi1[i+3]);
//	}

//	color = GFX_COLOR_BLUE;
//	point2d_t pi1[num_ipoints];
//	float tension = 1.0f;
//	for (uint32_t j=0; j<20; j++) {
//		h2_bezier_cubic(pi1, p, num_points, 0.0001f, tension);
//		for (i=0; i<num_ipoints-1; i+=3) {
//			h2_bezier_draw_cubic(draw_segment, 20, pi1[i], pi1[i+1], pi1[i+2], pi1[i+3]);
//		}
//		color += 100;
//		tension /= 1.1;
//	}

//	h2_bezier_cubic(pi1, p, num_points, 1.0f, 1.0f);
//	for (i=0; i<num_ipoints-1; i+=3) {
//		h2_bezier_draw_cubic(draw_segment, 10, pi1[i], pi1[i+1], pi1[i+2], pi1[i+3]);
//		//draw_segment(pi1[i],pi1[i+3]);
//	}

//	color = GFX_COLOR_RED;
//	point2d_t pi2[num_ipoints];
//	h2_bezier_cubic_symmetric(pi2, p, num_points, 1.0f, 3.0f);
//	for (i=0; i<num_ipoints-1; i+=3) {
//		h2_bezier_draw_cubic(draw_segment, 20, pi2[i], pi2[i+1], pi2[i+2], pi2[i+3]);
//	}

}

void spi5_isr() {
	if (ILI9341_SPI_IS_SELECTED()) {
		ili9341_spi5_isr();
	} else {
		spi_disable_tx_buffer_empty_interrupt(SPI5);
		(void)SPI_DR(SPI5);
	}
}

#define MIN1(x) (x<1?1:x)

#define DISPLAY_TIMEOUT   33 // ~30fps
#define BALL_MOVE_TIMEOUT MIN1(DISPLAY_TIMEOUT/10)
int main(void) {
	/* init timers. */
	clock_setup();
	
	/* set up SDRAM. */
	sdram_init();
	
	/* init LTDC (gfx
	 * cm3 and spi are also initialized by this function call) */
    ili9341_init(
        (uint16_t*[]){
    		(uint16_t*)(SDRAM_BASE_ADDRESS + 0*ILI9341_SURFACE_SIZE),
    		(uint16_t*)(SDRAM_BASE_ADDRESS + 1*ILI9341_SURFACE_SIZE)
    	},
    	(uint16_t*[]){
    		(uint16_t*)(SDRAM_BASE_ADDRESS + 2*ILI9341_SURFACE_SIZE),
    		(uint16_t*)(SDRAM_BASE_ADDRESS + 3*ILI9341_SURFACE_SIZE)
    	}

	);
	

	/*
	 * Application start..
	 */
	
	/* rotate LCD for 90 degrees */
    gfx_rotate(GFX_ROTATION_270_DEGREES);
	
	/* set background color */
	ltdc_set_background_color(0,0,0);
	
    //ltdc_reload(LTDC_SRCR_RELOAD_VBR);
    //while (LTDC_SRCR_IS_RELOADING());

	/* clear unused layers */
    ili9341_set_layer1();
    gfx_fill_screen(GFX_COLOR_BLACK);
    ili9341_flip_layer1_buffer();
    
    ili9341_set_layer2();
    
    gfx_fill_screen(ILI9341_LAYER2_COLOR_KEY); // color key sets alpha to 0
    ili9341_flip_layer2_buffer();
    gfx_fill_screen(ILI9341_LAYER2_COLOR_KEY); // color key sets alpha to 0
    ili9341_flip_layer2_buffer();
    
	ltdc_reload(LTDC_SRCR_RELOAD_VBR);
	while (LTDC_SRCR_IS_RELOADING());
	
	/* draw background */
    ili9341_set_layer1();
    gfx_fill_screen(GFX_COLOR_BLACK);

    gfx_set_font_scale(3);
    gfx_puts2(10, 10, "LTDC Demo", &font_Tamsyn5x9b_9 , GFX_COLOR_WHITE);
    gfx_set_font_scale(1);
    gfx_set_font(&font_Tamsyn5x9r_9);
    gfx_set_text_color(GFX_COLOR_BLUE2);
    gfx_puts3(gfx_width()-10, 14, "Bouncing rectangular balls\nflying around in späce!", GFX_ALIGNMENT_RIGHT);
    
    /* define ball movement plane */
    draw_plane = (plane_t) { .x = 1, .y = 40, .w = gfx_width()-2, .h = gfx_height()-40-2, .color=GFX_COLOR_DARKGREY };
	/* draw the stage (draw_plane) */
	gfx_draw_rect(0,39,gfx_width(),gfx_height()-40, GFX_COLOR_WHITE);
	gfx_fill_rect(draw_plane.x, draw_plane.y, draw_plane.w, draw_plane.h, draw_plane.color);

	/* draw bezier */
	gfx_set_surface_visible_area(1,41, 319,239);
	init_bezier();
	gfx_set_surface_visible_area_max();

    ili9341_flip_layer1_buffer();

    /* update dislay.. */
    ltdc_reload(LTDC_SRCR_RELOAD_VBR);
    
    /* create some balls */
    uint32_t ball_count = 0;
    ball_t *bp = balls;
    ball_create(bp++, 100, 10,20,20,  .5,-.2, GFX_COLOR_BLUE); ball_count++;
    ball_create(bp++, 130, 20,20,20,  .2,-.7, GFX_COLOR_BLUE2); ball_count++;
    ball_create(bp++, 190,100,20,20, -.3, .2, GFX_COLOR_ORANGE); ball_count++;
    ball_create(bp++, 100,150,20,20,  .1,-.5, GFX_COLOR_YELLOW); ball_count++;
    ball_create(bp++,  20,160,20,20,  .2, .6, GFX_COLOR_GREEN2); ball_count++;
    ball_create(bp++,  70, 80,20,20, -.2, .3, GFX_COLOR_RED); ball_count++;
    ball_create(bp++, 180, 50,20,20,  .4, .8, GFX_COLOR_CYAN); ball_count++;
    ball_create(bp++,  70,160,20,20, -.2, .6, GFX_COLOR_MAGENTA); ball_count++;
    ball_create(bp++, 250,170,20,20,  .2,-.3, GFX_COLOR_GREEN); ball_count++;
    ball_create(bp++, 212,165,20,20,  .5, .3, GFX_COLOR_BROWN); ball_count++;


#define TENSION_MIN 0.1f
#define TENSION_MAX 3.0f
    h2bez_float_t tension = 1.0f;
    h2bez_float_t tension_change = 1.1f;

	color = GFX_COLOR_BLUE;

    while (1) {
		uint64_t ctime   = mtime();
		static uint64_t draw_timeout = 0, ball_move_timeout = 0;
		static char fps_s[7] = "   fps";
		
		if (ball_move_timeout <= ctime) {
			ball_move_timeout  = ctime+BALL_MOVE_TIMEOUT;
			/* move balls */
			ball_move(balls, ball_count);
		}
		
		if (!LTDC_SRCR_IS_RELOADING() && (draw_timeout <= ctime)) {
			/* calculate fps */
			uint32_t fps = 1000/(ctime-draw_timeout+DISPLAY_TIMEOUT);
			fps_s[2] = 48+fps%10;
			fps /= 10;
			if (fps) {
				fps_s[1] = 48+fps%10;
				fps /= 10;
				if (fps)
					fps_s[0] = 48+fps%10;
			}
			/* set next timeout */
			draw_timeout = ctime+DISPLAY_TIMEOUT;
			
			/* select layer to draw on */ 
			ili9341_set_layer2();
			/* clear the whole screen */
			gfx_fill_screen(ILI9341_LAYER2_COLOR_KEY);

			/* Flood fill test */
			gfx_draw_circle(60,100,50, GFX_COLOR_GREEN);
			gfx_draw_circle(80, 80, 6, GFX_COLOR_GREEN);
			gfx_draw_circle(40, 80, 6, GFX_COLOR_GREEN);
			gfx_draw_line ( 40,130, 50,120, GFX_COLOR_GREEN);
			gfx_draw_line ( 50,120, 60,130, GFX_COLOR_GREEN);
			gfx_draw_line ( 60,130, 70,120, GFX_COLOR_GREEN);
			gfx_draw_line ( 70,120, 80,130, GFX_COLOR_GREEN);
			gfx_draw_vline( 60,130, 10,     GFX_COLOR_GREEN);

			gfx_draw_vline( 80, 51,10, GFX_COLOR_GREEN);
			gfx_draw_vline( 80,140,10, GFX_COLOR_GREEN);
			gfx_draw_vline( 40, 51,10, GFX_COLOR_GREEN);
			gfx_draw_vline( 40,140,10, GFX_COLOR_GREEN);

			uint8_t fill_segment_buf[8*50];
			int ns = gfx_flood_fill4(60,100, ILI9341_LAYER2_COLOR_KEY, GFX_COLOR_RED, fill_segment_buf, sizeof(fill_segment_buf));
			char buf[64];
			sprintf(buf, "%d stored", ns);
			gfx_puts2(50, 180, buf, &font_Tamsyn5x9b_9 , GFX_COLOR_WHITE);


			/* redraw bezier curves (only the color changes..) */
			tension *= tension_change;
			if (tension <= TENSION_MIN) {
				tension  = TENSION_MIN;
				tension_change = 1/tension_change;
			} else
			if (tension >= TENSION_MAX) {
				tension  = TENSION_MAX;
				tension_change = 1/tension_change;
			}
			gfx_set_surface_visible_area(1,41, 319,239);
			point2d_t pi1[num_ipoints];
			h2_bezier_cubic(pi1, p, num_points, 0.0001f, tension);
			for (uint32_t i=0; i<num_ipoints-1; i+=3) {
				h2_bezier_draw_cubic(draw_segment, 30, pi1[i], pi1[i+1], pi1[i+2], pi1[i+3]);
//				h2_bezier_draw_cubic2(draw_segment, 10, pi1[i], pi1[i+1], pi1[i+2], pi1[i+3]);
			}
			gfx_set_surface_visible_area_max();


//			float tension = 1.0f;
//			for (uint32_t j=0; j<10; j++) {
//				h2_bezier_cubic(pi1, p, num_points, 0.0001f, tension);
//				for (uint32_t i=0; i<num_ipoints-1; i+=3) {
//					h2_bezier_draw_cubic(draw_segment, 10, pi1[i], pi1[i+1], pi1[i+2], pi1[i+3]);
////					h2_bezier_draw_cubic2(draw_segment, 10, pi1[i], pi1[i+1], pi1[i+2], pi1[i+3]);
//				}
//				color += 2113;
//				tension /= 1.2;
//			}

			/* draw balls */
			ball_draw(balls, ball_count);
			
			/* draw fps */
			gfx_set_font_scale(1);
		    gfx_puts2(
				gfx_width() -6*font_Tamsyn5x9b_9.charwidth *gfx_get_font_scale()-3,
				gfx_height()-font_Tamsyn5x9b_9.lineheight*gfx_get_font_scale()-3,
				fps_s, &font_Tamsyn5x9b_9 , GFX_COLOR_WHITE);

			/* swap the double buffer */
			ili9341_flip_layer2_buffer();
			
			/* update dislay */
			ltdc_reload(LTDC_SRCR_RELOAD_VBR);
		}
	}
}


