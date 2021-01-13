/*
 * Copyright 2017 Google, Inc
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <gpiod.h>

#ifndef CONSUMER
#define CONSUMER   "Consumer"
#endif

int led_test( void )
{
    char *chipname = "gpiochip0";
    unsigned int line_num = 0;	// GPIO Pin #32
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    int i, ret;

    chip = gpiod_chip_open_by_name( chipname );
    if ( !chip ) {
	perror("Open chip failed\n");
	return 0;
    }

    line = gpiod_chip_get_line( chip, line_num );
    if ( !line ) {
	perror("Get line failed\n");
	gpiod_chip_close( chip );
        return 0;
    }

    ret = gpiod_line_request_output( line, CONSUMER, 0 );
    if ( ret < 0 ) {
	perror("Request line as output failed\n");
	goto release_line;
    }
    /* Blink 5 times */
    for ( i = 5; i > 0; i-- ) {
	gpiod_line_set_value( line, 1 );
	usleep( 500000 );
	gpiod_line_set_value( line, 0 );
	usleep( 500000 );
    }

release_line:
    gpiod_line_release( line );
    gpiod_chip_close( chip );
    return 0;
}

int button_test( void )
{
    char *chipname = "gpiochip0";
    unsigned int line_num0 = 0;	// GPIO Pin #32
    unsigned int line_num2 = 2; // GPIO Pin #22
    struct gpiod_chip *chip;
    struct gpiod_line *line0;
    struct gpiod_line *line2;
    int i, ret;
    int old_state;
    int current_state;

    chip = gpiod_chip_open_by_name( chipname );
    if ( !chip ) {
	perror("Open chip failed\n");
	return 0;
    }

    line0 = gpiod_chip_get_line( chip, line_num0 );
    if ( !line0 ) {
	perror("Get line0 failed\n");
	gpiod_chip_close( chip );
        return 0;
    }

    ret = gpiod_line_request_output( line0, CONSUMER, 0 );
    if ( ret < 0 ) {
	perror("Request line0 as output failed\n");
	gpiod_line_release( line0 );
        gpiod_chip_close( chip );
        return 0;
    }

    line2 = gpiod_chip_get_line( chip, line_num2 );
    if ( !line2 ) {
	perror("Get line2 failed\n");
	gpiod_chip_close( chip );
        return 0;
    }

    ret = gpiod_line_request_input( line2, CONSUMER );
    if ( ret < 0 ) {
	perror("Request line2 as input failed\n");
	gpiod_line_release( line2 );
        gpiod_chip_close( chip );
        return 0;
    }
    
    printf("Push button 10 times\n");

    old_state = 0;
    for ( i = 0; i < 10; ) {
        current_state = gpiod_line_get_value( line2 );
        if ( old_state == 0 && current_state == 1 ) {
	    gpiod_line_set_value( line0, 1 );           // on
            old_state = current_state;
        } else if ( old_state == 1 && current_state == 0 ) {
	    gpiod_line_set_value( line0, 0 );           // off
            old_state = current_state;
            i++;
        }
	usleep( 50000 );
    }

    gpiod_line_release( line0 );
    gpiod_line_release( line2 );
    gpiod_chip_close( chip );
    return 0;
}
