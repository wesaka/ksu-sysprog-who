/* who4.c - who with buffered reads
*  Identity crisis: The manual page for who lists who   am   i as an acceptable *usage of
*   the command. It also lists whoami. Modify who2.c so it supports the who am i *usage.
*   Experiment with the command whom and read the manage. How does it differ from who    
*   am i ? write a program that works like whoami.
*/

/*
 * Edited by Alvacir Wesley Kalatai Alberti
 */

/* who2.c  - read /etc/utmp and list info therein
 *         - suppresses empty records
 *         - formats time nicely
 */
#include        <stdio.h>
#include        <stdlib.h>
#include        <unistd.h>
#include        <utmp.h>
#include        <fcntl.h>
#include        <time.h>
#include <pwd.h>
#include <string.h>

/* #define      SHOWHOST */

void showtime(long);
void show_info(struct utmp *);

int main()
{
    struct utmp     utbuf;          /* read info into here */
    int             utmpfd;         /* read from this descriptor */

    if ( (utmpfd = open(UTMP_FILE, O_RDONLY)) == -1 ){
        perror(UTMP_FILE);
        exit(1);
    }

    while( read(utmpfd, &utbuf, sizeof(utbuf)) == sizeof(utbuf) )
        show_info( &utbuf );

    close(utmpfd);
    return 0;
}
/*
 *      show info()
 *                      displays the contents of the utmp struct
 *                      in human readable form
 *                      * displays nothing if record has no user name
 */
void show_info( struct utmp *utbufp )
{
    uid_t uid;
    uid = getuid();

    if ( utbufp->ut_type != USER_PROCESS )
        return;

    if (strcmp(getpwuid(uid)->pw_name, utbufp->ut_name) == 0) {
        printf("%-8.8s", utbufp->ut_name);      /* the logname  */
        printf(" ");                            /* a space      */
        printf("%-8.8s", utbufp->ut_line);      /* the tty      */
        printf(" ");                            /* a space      */
        showtime(utbufp->ut_time);            /* display time */



#ifdef SHOWHOST
        if ( utbufp->ut_host[0] != '\0' )
                    printf(" (%s)", utbufp->ut_host);/* the host    */
#endif
        printf("\n");                          /* newline      */
    }
}

void showtime( long timeval )
/*
 *      displays time in a format fit for human consumption
 *      uses ctime to build a string then picks parts out of it
 *      Note: %12.12s prints a string 12 chars wide and LIMITS
 *      it to 12chars.
 */
{
    char    *cp;                    /* to hold address of time      */

    cp = ctime(&timeval);           /* convert time to string       */
    printf("%12.12s", cp+4 );       /* pick 12 chars from pos 4     */
}
