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

#define line  0
#define line2 2

void initpin( int pin, char *mode )
{
    FILE *fp;
    bool isdir;
    char path[100] = "";
    char direction[100] = "";
    sprintf( path, "/sys/class/gpio/gpio%d", pin );
    sprintf( direction, "/sys/class/gpio/gpio%d/direction", pin );
    struct stat st = {0};
    if ( !stat(path, &st) ) {
        isdir = S_ISDIR( st.st_mode );
    }
    if ( !isdir ) {
        fp = fopen( "/sys/class/gpio/export", "w" );
        fprintf( fp, "%d", pin );
        fclose( fp );
        fp = fopen( direction, "w" );
        fprintf( fp, "%s", mode );
        fclose( fp );
    }
}

void setpin( int pin, int value )
{
    FILE *fp;
    char pinvalue[100] = "";
    sprintf( pinvalue, "/sys/class/gpio/gpio%d/value", pin );
    fp = fopen( pinvalue, "w" );
    fprintf( fp, "%d", value );
    fclose( fp );
}

int getpin( int pin )
{
    FILE *fp;
    int value;
    char pinvalue[100] = "";
    sprintf( pinvalue, "/sys/class/gpio/gpio%d/value", pin );
    fp = fopen( pinvalue, "r" );
    fscanf( fp, "%d", &value );
    fclose( fp );
    return ( value );
}

void closepin( int pin )
{
    FILE *fp;
    fp = fopen( "/sys/class/gpio/unexport", "w" );
    fprintf( fp, "%d", pin );
    fclose( fp );
}

int led_test( void )
{
    int i;
    initpin( line, "out" );

    /* Blink 5 times */
    for ( i = 5; i > 0; i-- ) {
	setpin( line, 1 );
	usleep( 500000 );
	setpin( line, 0 );
	usleep( 500000 );
    }

    closepin( line );
    return 0;
}

int button_test( void )
{
    int i;
    int old_state;
    int current_state;
    initpin( line, "out" );
    initpin( line2, "in" );

    printf("Push button 10 times\n");

    old_state = 0;
    for ( i = 0; i < 10; ) {
        current_state = getpin( line2 );
        if ( old_state == 0 && current_state == 1 ) {
	    setpin( line, 1 );           // on
            old_state = current_state;
        } else if ( old_state == 1 && current_state == 0 ) {
	    setpin( line, 0 );           // off
            old_state = current_state;
            i++;
        }
	usleep( 50000 );
    }

    closepin( line );
    closepin( line2 );
    return 0;
}
