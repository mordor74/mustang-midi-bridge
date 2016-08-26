#include <cstdlib>
#include <cstdio>
#include <unistd.h>
//#include <RtMidi.h>
#include <cerrno>

#include "mustang.h"

static Mustang mustang;

static int channel;
int cmdflg;
char g_p_names[100][33];

// viene richiamata quando riceve un messaggio dal midi. l'azione sta qui! (ma è inutile!)
//void message_action( double deltatime, std::vector< unsigned char > *message, void *userData ) {
//#if 0
  //nBytes dimensione del messaggio
//  unsigned int nBytes = message->size();
  // se il messaggio è 2 byte, minimo 2 carartteri per il primo più secondo 
//  if      ( nBytes == 2 ) fprintf( stdout, "%02x %d\n", (int)message->at(0), (int)message->at(1) );
  // se il messaggio è 3 byte, minimo 2 carartteri per il primo più secondo e terzo
//  else if ( nBytes == 3 ) fprintf( stdout, "%02x %d %d\n", (int)message->at(0), (int)message->at(1), (int)message->at(2) );
//#endif

  // Is this for us?
  // verifica il canale del messaggio (riceve solo quelli del canale impostato!) (inutile)
//  int msg_channel = (*message)[0] & 0x0f;
//  if ( msg_channel != channel ) return;
  // verifica il tipo di messaggio
//  int msg_type = (*message)[0] & 0xf0;

//  switch ( msg_type ) {

//  case 0xc0: {
    // Program change
//    int bank = (int)(*message)[1];
//    int rc = mustang.patchChange( bank );
//    if ( rc ) {
//      fprintf( stderr, "Error: PC#%d failed. RC = %d\n", bank, rc );
//    }
//  }
//  break;
    
//  case 0xb0: {
    // Control change
//    int rc = 0;
//    int cc = (*message)[1];
//    int value = (*message)[2];
    
    // Tuner toggle
//    if ( cc == 20 ) {
//      int rc = mustang.tunerMode( value );
//    }
    // All EFX toggle
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
    // Effects on/off
//    else if ( cc >= 23 && cc <= 26 ) {
//      rc = mustang.effectToggle( cc, value );
//    }
    // Set stomp model
//    else if ( cc == 28 ) {
//      rc = mustang.setStomp( value );
//    }
//    // Stomp CC handler
//    else if ( cc >= 29 && cc <= 33 ) {
//      rc = mustang.stompControl( cc, value );
//    }
    // Set mod model
//    else if ( cc == 38 ) {
//      rc = mustang.setMod( value );
//    }
//    // Mod CC handler
//    else if ( cc >= 39 && cc <= 43 ) {
//      rc = mustang.modControl( cc, value );
//    }
    // Set delay model
//    else if ( cc == 48 ) {
//      rc = mustang.setDelay( value );
//    }
    // Delay CC handler
//    else if ( cc >= 49 && cc <= 54 ) {
//      rc = mustang.delayControl( cc, value );
//    }
    // Set reverb model
//    else if ( cc == 58 ) {
//      rc = mustang.setReverb( value );
//    }
    // Reverb CC handler
//    else if ( cc >= 59 && cc <= 63 ) {
//      rc = mustang.reverbControl( cc, value );
//    }
    // Set amp model
//    else if ( cc == 68 ) {
//      rc = mustang.setAmp( value );
//    }
    // Amp CC Handler
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
    "Usage: mustangr tuneron\n"
    "       mustangr <virtual_port> <midi_channel#>\n\n"

    "       port = 0..n,  channel = 1..16\n";

  fprintf( stderr, msg );
  exit( 1 );
}

// controlla il numero di argomenti e eventualmente mostra l help
int main( int argc, const char **argv ) {
	cmdflg = 0 ;
  if ( argc < 2 ) usage();
// dichiara il midi in (inutile)
// RtMidiIn input_handler;
// dichiara endptr puntatore agli argomenti (inutile?)
  char *endptr;
// il primo argomeno diventa la porta (inutile)
//  int port = (int) strtol( argv[1], &endptr, 10 );
  
// se endptr punta all'agomento zero prova ad aprire la porta indicata dal secondo argomento (intuile)
//  if ( endptr == argv[0] ) {
//    try {
//      input_handler.openVirtualPort( argv[2] );
//    }
//    catch ( RtError &error ) {
//      exit( 1 );
//    }
//  }
//  else {
	  // se porta minore zero mostra l help altrimenti prova ad aprire la porta indicata da port (inutile)
//    if ( port < 0 ) usage();
//    try {
//      input_handler.openPort( port );
//    }
//    catch ( RtError &error ) {
//      exit( 1 );
//    }
//  }
// il secondo  argomeno diventa il canale (inutile)
//  channel = (int) strtol( argv[2], &endptr, 10 ) - 1;
 // se endptr punta all'agomento zero mostra l'help 
//  if ( endptr == argv[0] ) usage();
  // se il canale è maggiore di 15 o minore di 0 mostra l'help
//  if ( channel < 0 || channel > 15 ) usage();
  
  //richiama message_action (inutile)
//  input_handler.setCallback( &message_action );

  // Don't want sysex, timing, active sense(inutile)
//  input_handler.ignoreTypes( true, true, true );
 // connetti il mustang
  if ( 0 != mustang.initialize() ) {
    fprintf( stderr, "Cannot setup USB communication\n" );
    exit( 1 );
  }
  if ( 0 != mustang.commStart() ) {
    fprintf( stderr, "Thread setup and init failed\n" );
    exit( 1 );
  }
    fprintf( stderr, "Mustang Raider 0.2\n" );
  if ( strcmp(argv[1],"t") == 0 ) {
    int tuneron = 64;
    int rc = mustang.tunerMode( tuneron );
    fprintf( stderr, "TUNER ON\n" );
    cmdflg = 1 ;
  }
  //fprintf( stderr, "t for tuner ON\n" );
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
  return 0;

}
