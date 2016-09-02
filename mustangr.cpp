#include <cstdlib>
#include <cstdio>
#include <unistd.h>
//#include <RtMidi.h>
#include <cerrno>

#include <iostream>
#include <fstream>

//includes for footswitch input

#include <fcntl.h>
#include <dirent.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>
#include <signal.h>
#include <time.h>

#include "mustang.h"

static Mustang mustang;
static int channel;
int cmdflg;
char g_p_names[100][33];

std::fstream myfile;

//declaration for footswitch input
struct input_event ev[64];
char name[256] = "Unknown";
char *device = "/dev/input/event0";

int fd, rd, value, size = sizeof (struct input_event);

void handler (int sig)
{
  printf ("nexiting...(%d)n", sig);
  exit (0);
}
 
void perror_exit (char *error)
{
  perror (error);
  handler (9);
}
static void *foot_trd ( void *)
{
fprintf (stderr,"Setuping up FS\n");
while (1){
           //int rcu = usleep( 200 );
           if ((rd = read (fd, ev, size * 64)) < size)
               //perror_exit ("read()");
               value = ev[0].value;
               //fprintf( stderr,"L\n");
               if (value != ' ' && ev[1].value == 1 && ev[1].type == 1){ // Only read the key press event
                   printf ("Code[%d]\n", (ev[1].code));
                   int dpreset=mustang.curpreset()-8;
                   if (dpreset<0) {dpreset=dpreset+24;}
                   int rc = mustang.patchChange( dpreset );
                   fprintf( stderr, "SELECTED: " );
                   fprintf( stderr, "%d", mustang.curpreset());
                   fprintf( stderr, "\n%s", g_p_names[mustang.curpreset()]);
                   fprintf( stderr,"\n");
                   myfile << "SELECTED: ";
                   myfile << g_p_names[mustang.curpreset()];
                   myfile << "\n";
               }
       }
    pthread_exit(NULL);
}

// viene richiamata quando riceve un messaggio dal midi. l'azione sta qui! (ma è inutile!)
//void message_action( double deltatime, std::vector< unsigned char > *message, void *userData ) {
//#if 0
//  unsigned int nBytes = message->size();
//  if      ( nBytes == 2 ) fprintf( stdout, "%02x %d\n", (int)message->at(0), (int)message->at(1) );
//  else if ( nBytes == 3 ) fprintf( stdout, "%02x %d %d\n", (int)message->at(0), (int)message->at(1), (int)message->at(2) );
//#endif
//
// Is this for us?
//  int msg_channel = (*message)[0] & 0x0f;
//  if ( msg_channel != channel ) return;
//  int msg_type = (*message)[0] & 0xf0;
//
//  switch ( msg_type ) {
//
//  case 0xc0: {
//    int bank = (int)(*message)[1];
//    int rc = mustang.patchChange( bank );
//    if ( rc ) {
//      fprintf( stderr, "Error: PC#%d failed. RC = %d\n", bank, rc );
//    }
//  }
//  break;
//    
//  case 0xb0: {
//    int rc = 0;
//    int cc = (*message)[1];
//    int value = (*message)[2];
//    if ( cc == 20 ) {
//      int rc = mustang.tunerMode( value );
//    }
//    else if ( cc == 22 ) {
//      rc = mustang.effectToggle( 23, value );
//      if ( rc == 0 ) {
//        rc = mustang.effectToggle( 24, value );
//        if ( rc == 0 ) {
//          rc = mustang.effectToggle( 25, value );
//          if ( rc == 0 ) {
//            rc = mustang.effectToggle( 26, value );
//          }
//        }
//      }
//    }
//    else if ( cc >= 23 && cc <= 26 ) {
//      rc = mustang.effectToggle( cc, value );
//    }
//    else if ( cc == 28 ) {
//      rc = mustang.setStomp( value );
//    }
//    // Stomp CC handler
//    else if ( cc >= 29 && cc <= 33 ) {
//      rc = mustang.stompControl( cc, value );
//    }
//    else if ( cc == 38 ) {
//      rc = mustang.setMod( value );
//    }
//    // Mod CC handler
//    else if ( cc >= 39 && cc <= 43 ) {
//      rc = mustang.modControl( cc, value );
//    }
//    else if ( cc == 48 ) {
//      rc = mustang.setDelay( value );
//    }
//    else if ( cc >= 49 && cc <= 54 ) {
//      rc = mustang.delayControl( cc, value );
//    }
//    else if ( cc == 58 ) {
//      rc = mustang.setReverb( value );
//    }
//    else if ( cc >= 59 && cc <= 63 ) {
//      rc = mustang.reverbControl( cc, value );
//    }
//    else if ( cc == 68 ) {
//      rc = mustang.setAmp( value );
//    }
//    else if ( cc >= 69 && cc <= 79 ) {
//      rc = mustang.ampControl( cc, value );
//    }
//    if ( rc ) {
//      fprintf( stderr, "Error: CC#%d failed. RC = %d\n", cc, rc );
//    }
//  }
//  break;
//  default:
//    break;
//  }
//
//}


void usage() {
  const char msg[] = 
    "Mustang Raider 0.9\n"
    "Usage: mustangr t\n"
    "                tuner on\n"
    "       mustangr u\n"
    "                tuner off\n"
    "       mustangr c [preset]\n"
    "                change to [preset] \n"
    "       mustangr d\n"
    "                daemon mode\n";

  fprintf( stderr, msg );
  exit( 1 );
}

// controlla il numero di argomenti e eventualmente mostra l help
int main( int argc, const char **argv ) {
  myfile.open ("/var/log/mustangr.log");
  if (myfile.is_open ()) {fprintf(stderr,"logging...\n"); }
  else { fprintf(stderr,"error opening file"); exit(1); }
  //string log_date;
  time_t rawtime;
  time (&rawtime);
  //log_date = ctime(&rawtime);
  myfile << ctime(&rawtime); //log_date;
  myfile << " Writing this to a file.\n";
  cmdflg = 0 ;
  if ( argc < 2 ) usage();

// RtMidiIn input_handler;
// dichiara endptr puntatore agli argomenti (inutile?)
//  char *endptr;
//  int port = (int) strtol( argv[1], &endptr, 10 );
//  if ( endptr == argv[0] ) {
//    try {
//      input_handler.openVirtualPort( argv[2] );
//    }
//    catch ( RtError &error ) {
//      exit( 1 );
//    }
//  }
//  else {
//    if ( port < 0 ) usage();
//    try {
//      input_handler.openPort( port );
//    }
//    catch ( RtError &error ) {
//      exit( 1 );
//    }
//  }
//  channel = (int) strtol( argv[2], &endptr, 10 ) - 1;
//  if ( endptr == argv[0] ) usage();
//  if ( channel < 0 || channel > 15 ) usage();
//  input_handler.setCallback( &message_action );
//  input_handler.ignoreTypes( true, true, true );

// connect Mustang
  if ( 0 != mustang.initialize() ) {
    fprintf( stderr, "Cannot setup USB communication\n" );
    exit( 1 );
  }
  
  if ( 0 != mustang.commStart() ) {
    fprintf( stderr, "Thread setup and init failed\n" );
    exit( 1 );
  }
  
  fprintf( stderr, "Mustang Raider 0.9\n" );
  
  if ( strcmp(argv[1],"t") == 0 ) {
    int tuneron = 64;
    int rc = mustang.tunerMode( tuneron );
    fprintf( stderr, "TUNER ON\n" );
    cmdflg = 1 ;
  }
  
  if ( strcmp(argv[1],"u") == 0 ) {
    int tuneroff = 0;
    int rc = mustang.tunerMode( tuneroff );
    fprintf( stderr, "TUNER OFF\n" );
    cmdflg = 1 ;
  }
  
  if ( strcmp(argv[1],"c") == 0 ) {
	fprintf( stderr, "PATCH " );
        fprintf( stderr, argv[2]);
        fprintf( stderr, "\n" );
        int rc = mustang.patchChange( atoi(argv[2]) );
	fprintf( stderr, "SELECTED: " );
	fprintf( stderr, "%d", mustang.curpreset());
	fprintf( stderr, "\n%s", g_p_names[mustang.curpreset()]);
	fprintf( stderr,"\n");
	cmdflg = 1 ;
  }
  
  if ( strcmp(argv[1],"d") == 0 ) {
        
        // the code here was originally written by Derek Hildreth from The Linux Daily
        if ((getuid ()) != 0)
            printf ("You are not root! This may not work...n");
        //device="/dev/input/event0"; //this will be placed in a cfg file in future
        
        //Open Device
        fprintf( stderr, "Open %s",device );
        fd = open (device, O_RDONLY|O_NONBLOCK);
        if (fd == -1)
            printf ("%s is not a vaild device\nn", device);
  
        //Print Device Name
        ioctl (fd, EVIOCGNAME (sizeof (name)), name);
        printf ("Reading From : %s (%s)\n", device, name);
        pthread_t fsthread;
        fprintf (stderr,"Starting\n");
        int rcf = pthread_create(&fsthread ,NULL,foot_trd,NULL);
        fprintf (stderr,"Setup OK\n");
        pause(); 
	cmdflg = 1 ;
  }
if ( cmdflg == 0) {
 fprintf( stderr, "No argoument!\n" );
usage();
}
  // Block and wait for signal 
//  pause();

  if ( 0 != mustang.commShutdown() ) {
    fprintf( stderr, "Thread shutdown failed\n" );
    exit( 1 );
  }
  if ( 0 != mustang.deinitialize() ) {
    fprintf( stderr, "USB shutdown failed\n" );
    exit( 1 );
  }
  
  // delete input_handler;
  myfile.close();
  return 0;

}
