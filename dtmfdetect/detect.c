/* Stuff for goertzel algorithm */

#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define CHANNEL             0
#define MAX_BINS         8
#define GOERTZEL_N       92

int flag_got = FALSE; //TRUE: get press; FALSE: nothing got
int         sample_count;
double      q1[ MAX_BINS ];
double      q2[ MAX_BINS ];
double      r[ MAX_BINS ];
double      coefs[ MAX_BINS ] = {
1.7088388,
1.6339398,
1.5514226,
1.4616719,

1.1533606,
1.0391679,
0.7968022,
0.5395935,
};


int f_tone[MAX_BINS] = { 697, 770, 852, 941, 1209, 1336, 1477, 1633 };	// frequencies of rows & columns 

void goertzel( int sample );

void main(int argc, char **argv)
{
    for (int i = 0; i < MAX_BINS; i++)
    {
        coefs[i] = 2 * cos(2 * M_PI * (f_tone[i] / 8000.0));
        printf("coefs[%d]=%f\n", i, coefs[i]);
    }

    FILE *pcm_fd = fopen(argv[1], "rb");
    int16_t sample;

    int drop_sample_cnt = 0;
    while (fread(&sample, sizeof(sample), 1, pcm_fd) > 0)
    {
        if (flag_got == TRUE && drop_sample_cnt < 1120)
        {
            drop_sample_cnt++;
            continue;
        }
        else
        {
            //restart new processing
            drop_sample_cnt = 0;
            flag_got = FALSE;
        }
        goertzel( (int) sample );
    }
    fclose(pcm_fd);

    return;
}

/*----------------------------------------------------------------------------
 *  post_testing
 *----------------------------------------------------------------------------
 * This is where we look at the bins and decide if we have a valid signal.
 */
void
post_testing()
{
int         row, col, see_digit;
int         peak_count, max_index;
double      maxval, t;
int         i;
char  row_col_ascii_codes[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}};


    /* Find the largest in the row group. */
    row = 0;
    maxval = 0.0;
    for ( i=0; i<4; i++ )
    {
        if ( r[i] > maxval )
        {
            maxval = r[i];
            row = i;
        }
    }

    /* Find the largest in the column group. */
    col = 4;
    maxval = 0.0;
    for ( i=4; i<8; i++ )
    {
        if ( r[i] > maxval )
        {
            maxval = r[i];
            col = i;
        }
    }


    /* Check for minimum energy */

    if ( r[row] < 4.0e5 )   /* 2.0e5 ... 1.0e8 no change */
    {
        /* energy not high enough */
    }
    else if ( r[col] < 4.0e5 )
    {
        /* energy not high enough */
    }
    else
    {
        see_digit = TRUE;

        /* Twist check
         * CEPT => twist < 6dB
         * AT&T => forward twist < 4dB and reverse twist < 8dB
         *  -ndB < 10 log10( v1 / v2 ), where v1 < v2
         *  -4dB < 10 log10( v1 / v2 )
         *  -0.4  < log10( v1 / v2 )
         *  0.398 < v1 / v2
         *  0.398 * v2 < v1
         */
        if ( r[col] > r[row] )
        {
            /* Normal twist */
            max_index = col;
            if ( r[row] < (r[col] * 0.398) )    /* twist > 4dB, error */
                see_digit = FALSE;
        }
        else /* if ( r[row] > r[col] ) */
        {
            /* Reverse twist */
            max_index = row;
            if ( r[col] < (r[row] * 0.158) )    /* twist > 8db, error */
                see_digit = FALSE;
        }

        /* Signal to noise test
         * AT&T states that the noise must be 16dB down from the signal.
         * Here we count the number of signals above the threshold and
         * there ought to be only two.
         */
        if ( r[max_index] > 1.0e9 )
            t = r[max_index] * 0.158;
        else
            t = r[max_index] * 0.010;

        peak_count = 0;
        for ( i=0; i<8; i++ )
        {
            if ( r[i] > t )
                peak_count++;
        }
        if ( peak_count > 2 )
            see_digit = FALSE;

        if ( see_digit )
        {
            printf( "%c\n", row_col_ascii_codes[row][col-4] );
            fflush(stdout);
            flag_got = TRUE;
        }
    }
}



/*----------------------------------------------------------------------------
 *  goertzel
 *----------------------------------------------------------------------------
 */
void
goertzel( int sample )
{
double      q0;
int        i;

    if ( sample_count < GOERTZEL_N )
    {
        sample_count++;
        for ( i=0; i<MAX_BINS; i++ )
        {
            q0 = coefs[i] * q1[i] - q2[i] + sample;
            q2[i] = q1[i];
            q1[i] = q0;
        }
    }
    else
    {
        for ( i=0; i<MAX_BINS; i++ )
        {
            r[i] = (q1[i] * q1[i]) + (q2[i] * q2[i]) - (coefs[i] * q1[i] * q2[i]);
            q1[i] = 0.0;
            q2[i] = 0.0;
        }
        post_testing();
        sample_count = 0;
    }
}
