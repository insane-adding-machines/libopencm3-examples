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
    gfx_puts3(gfx_width()-10, 14, "Bouncing balls\nflying around in späce!", GFX_ALIGNMENT_RIGHT);
    
	/* init/draw bezier */
	gfx_set_surface_visible_area(1,41, gfx_width()-1,gfx_height()-1);
	init_bezier();
	gfx_set_surface_visible_area_max();

    /* define ball movement walls */
    balls_t ball_simulation;
    ball_t balls[100];
    ball_setup(
    		&ball_simulation,
			mtime(),
			(walls_t) {
    			.x1=201,
    			.y1=61,
				.x2=gfx_width()-4,
				.y2=179,
    	        .bg_color=GFX_COLOR_GREY2, .fg_color=GFX_COLOR_WHITE
    		},
			balls, sizeof(balls)/sizeof(ball_t)
		);
	/* draw the stage (draw_plane) */
    ball_draw_walls(&ball_simulation);

    ili9341_flip_layer1_buffer();

    /* update display.. */
    ltdc_reload(LTDC_SRCR_RELOAD_VBR);
    
    /* create some balls */
    ball_create(&ball_simulation,  5, 5, (point2d_t){ 100, 56 }, (point2d_t){  50,-20 }, GFX_COLOR_BLUE);
    ball_create(&ball_simulation,  6, 6, (point2d_t){ 130, 60 }, (point2d_t){  20,-70 }, GFX_COLOR_BLUE2);
    ball_create(&ball_simulation,  7, 7, (point2d_t){ 190,140 }, (point2d_t){ -30, 20 }, GFX_COLOR_ORANGE);
    ball_create(&ball_simulation,  4, 4, (point2d_t){ 100,190 }, (point2d_t){  10,-50 }, GFX_COLOR_YELLOW);
    ball_create(&ball_simulation, 10,10, (point2d_t){  20,200 }, (point2d_t){  20, 60 }, GFX_COLOR_GREEN2);
    ball_create(&ball_simulation, 15,15, (point2d_t){  70,120 }, (point2d_t){ -20, 30 }, GFX_COLOR_RED);
    ball_create(&ball_simulation,  5, 1, (point2d_t){ 180, 90 }, (point2d_t){  40, 80 }, GFX_COLOR_CYAN);
    ball_create(&ball_simulation,  5, 1, (point2d_t){  75,200 }, (point2d_t){ -20, 60 }, GFX_COLOR_MAGENTA);
    ball_create(&ball_simulation,  5, 1, (point2d_t){ 250,210 }, (point2d_t){  20,-30 }, GFX_COLOR_GREEN);
    ball_create(&ball_simulation,  6,30, (point2d_t){ 212,205 }, (point2d_t){  50, 30 }, GFX_COLOR_BROWN);

    ball_create(&ball_simulation,  3, .1, (point2d_t){ 100, 56 }, (point2d_t){  50,-20 }, GFX_COLOR_BLACK);
    ball_create(&ball_simulation,  3, .1, (point2d_t){ 100, 56 }, (point2d_t){  50,-20 }, GFX_COLOR_DARKGREY);
    ball_create(&ball_simulation,  3, .1, (point2d_t){ 100, 56 }, (point2d_t){  50,-20 }, GFX_COLOR_GREY);
    ball_create(&ball_simulation,  3, .1, (point2d_t){ 100, 56 }, (point2d_t){  50,-20 }, GFX_COLOR_BLUE);
    ball_create(&ball_simulation,  3, .1, (point2d_t){ 100, 56 }, (point2d_t){  50,-20 }, GFX_COLOR_BLUE2);
    ball_create(&ball_simulation,  3, .1, (point2d_t){ 100, 56 }, (point2d_t){  50,-20 }, GFX_COLOR_RED);
    ball_create(&ball_simulation,  3, .1, (point2d_t){ 100, 56 }, (point2d_t){  50,-20 }, GFX_COLOR_MAGENTA);
    ball_create(&ball_simulation,  3, .1, (point2d_t){ 100, 56 }, (point2d_t){  50,-20 }, GFX_COLOR_GREEN);
    ball_create(&ball_simulation,  3, .1, (point2d_t){ 100, 56 }, (point2d_t){  50,-20 }, GFX_COLOR_GREEN2);
    ball_create(&ball_simulation,  3, .1, (point2d_t){ 100, 56 }, (point2d_t){  50,-20 }, GFX_COLOR_CYAN);
    ball_create(&ball_simulation,  3, .1, (point2d_t){ 100, 56 }, (point2d_t){  50,-20 }, GFX_COLOR_YELLOW);
    ball_create(&ball_simulation,  3, .1, (point2d_t){ 100, 56 }, (point2d_t){  50,-20 }, GFX_COLOR_ORANGE);
    ball_create(&ball_simulation,  3, .1, (point2d_t){ 100, 56 }, (point2d_t){  50,-20 }, GFX_COLOR_BROWN);

#define TENSION_MIN 0.15f
#define TENSION_MAX 3.0f
    h2bez_float_t tension = 1.0f;
    h2bez_float_t tension_change = 1.1f;

	color = GFX_COLOR_BLUE;

    while (1) {
		uint64_t ctime   = mtime();
		static uint64_t draw_timeout = 0;
		static char fps_s[7] = "   fps";
		
		if (!LTDC_SRCR_IS_RELOADING() && (draw_timeout <= ctime)) {
			/* calculate fps */
			uint32_t fps = 1000/(ctime-draw_timeout+DISPLAY_TIMEOUT);
			/* set next timeout */
			draw_timeout = ctime+DISPLAY_TIMEOUT;
			
			/* select layer to draw on */ 
			ili9341_set_layer2();
			/* clear the whole screen */
			gfx_fill_screen(ILI9341_LAYER2_COLOR_KEY);

			/* Flood fill test */
//			gfx_draw_rect(10,50,100,100, GFX_COLOR_GREEN2);
			// Frowny
			gfx_draw_circle(60,100,50, GFX_COLOR_GREEN2);
			gfx_draw_circle(80, 80, 6, GFX_COLOR_GREEN2);
			gfx_draw_circle(40, 80, 6, GFX_COLOR_GREEN2);
			gfx_draw_line ( 40,130, 50,120, GFX_COLOR_GREEN2);
			gfx_draw_line ( 50,120, 60,130, GFX_COLOR_GREEN2);
			gfx_draw_line ( 60,130, 70,120, GFX_COLOR_GREEN2);
			gfx_draw_line ( 70,120, 80,130, GFX_COLOR_GREEN2);
			// Goatee
			gfx_draw_vline( 60,130, 20, GFX_COLOR_GREEN2);
			// 4 Markers
			gfx_draw_vline( 80, 51,15, GFX_COLOR_GREEN2);
			gfx_draw_vline( 80,140,15, GFX_COLOR_GREEN2);
			gfx_draw_vline( 40, 51,15, GFX_COLOR_GREEN2);
			gfx_draw_vline( 40,140,15, GFX_COLOR_GREEN2);
			// Scars
			gfx_draw_line ( 30,60,  80,120, GFX_COLOR_GREEN2);
			gfx_draw_line ( 20,125, 100,80, GFX_COLOR_GREEN2);
			// Fly :)
			gfx_draw_rect(  20,150, 80,20,  GFX_COLOR_GREEN2);
			gfx_draw_line ( 23,155, 97,167, GFX_COLOR_GREEN2);
			gfx_draw_line ( 30,167, 80,150, GFX_COLOR_GREEN2);
//			// Algorithm breaking dots
//			for (uint32_t px=11; px<110; px+=4) {
//				for (uint32_t py=51; py<170; py+=2) {
//					gfx_draw_pixel(px,py,GFX_COLOR_GREEN2);
//					gfx_draw_pixel(px+2,py+1,GFX_COLOR_GREEN2);
//				}
//			}
			// Head-Fly connection
			gfx_draw_vline( 60,149,  3, ILI9341_LAYER2_COLOR_KEY);

			// Flood fill
//			gfx_offscreen_rendering_start(NULL,0,0);
//			gfx_set_surface_visible_area(15,55, 105,145);
			uint8_t fill_segment_buf[8*2048];
			fill_segment_queue_statistics_t stats = gfx_flood_fill4(60,100, ILI9341_LAYER2_COLOR_KEY, GFX_COLOR_RED, fill_segment_buf, sizeof(fill_segment_buf));
//			gfx_set_surface_visible_area_max();
			// Print statistics
			char buf[1024];
			snprintf(buf,1023,
					"flood_fill4 test:\n"
					" max. %5d segments\n"
					" tot. %5d segments stored\n"
					"      %5d segment buffer overflows",
					stats.count_max,
					stats.count_total,
					stats.overflows);
			buf[1023]=0;
			gfx_puts2(10, 178, buf, &font_Tamsyn5x9b_9 , GFX_COLOR_WHITE);
//			gfx_offscreen_rendering_stop();


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
				h2_bezier_draw_cubic(draw_segment, 20, pi1[i], pi1[i+1], pi1[i+2], pi1[i+3]);
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

			/* move balls */
			ball_move(&ball_simulation, mtime());
			/* draw balls */
			ball_draw(&ball_simulation);
			
			/* draw fps */
			gfx_set_font_scale(1);
			sprintf(fps_s, "%lufps", fps);
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


