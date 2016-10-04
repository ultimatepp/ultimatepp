#include <CtrlLib/CtrlLib.h>
#include "Tournament.h"
#include "pairing88.h"


//extern ofstream verbosefile;

/* Generazione di abbinamenti per un torneo di scacchi
   a sistema Svizzero.

   - Contiene dei controlli per evitare loop: ogni giocatore non pu�
     avere pi di n-1 incompatibilit�in un gruppo di n giocatori

   - Ricorda i giocatori scelti in occasione di ogni accoppiamento

   - Tiene traccia del miglior accoppiamento parziale qualora l'accoppiamento
     complessivo non possa essere realizzato
*/

extern int  CanPlayWith(int player1, int player2, int round);

/*
 * -------- class implementation ---------------
 */

//
// This is the engine that produce the pairing. I am proude of it!!! :-)
// It Realizes the pairing of 'np' players put on 2 columns as S1 and S2 in
// the FIDE handbook .
// It return the numebr of pair done, 'nmade' on a maximum np/2.
// Se l'abbinamento e' riuscito vi saranno n_partial=n/2 coppie fatte.
// The numbers of floaters will be n_floaters =  np-2*nmade
//
int TPairingDubov::pairing(void)
{ int i, j, l, n_couple, sum, found;
  int pairing_possible = 1, max_pair;
  int count, i_player, j_player, checkCompatibility=0;
  int ll, opponent, i_player_last, j_player_last;
  int n_partial=0; //pair at the moment
  int avoid[LIMIT+1];

  resetplayerFlag(IsPlayerFree, np);
  resetchallengerFlag(challengerFlag, np);

 if (np_s1==0) {
  // build the opponent list for each player in the case np_s1=0
  for (i=1; i<=np2; i++) {  // the last player is not considered
      count = 0; // counter of the compatible players
      // verbosefile <<  "player " << idpl[i] << ":";
      for (j=1; j<=np2; j++) { // prima meta' del vettore
           opponent = challengerFlag[j]; // opponent list
           if (CanPlayWith(idpl[i], idpl[ opponent ], nround)) {
	          count++;
	          remind[count] = opponent;
	       }
      }
      for (j=np2+i; j<=np; j++) {  // seconda meta'
           opponent = challengerFlag[j]; // opponent list
           if (CanPlayWith(idpl[i], idpl[ opponent ], nround)) {
	           count++;
	           remind[count] = opponent;
	       }
      }
      remind[0] = count;

      /*  for (l=1; l<=count; l++) {
        verbosefile << idpl[ remind[l] ] <<"; ";
      }
      verbosefile << endl;
      */
      // allocate space for a vector that store 'remind', that is the opponent list
      sss[i] = (unsigned int *) calloc(count+1, sizeof(unsigned int) );

      // verbosefile <<  "opps= " << count << " -> ";
      for (l=0; l<=count; l++) {
         sss[i][l] = remind[l]; //store remind
	     //if (l>0) verbosefile << idpl[ sss[i][l] ] <<", ";
      }
       //verbosefile << endl;
  }
  for (i=np2+1; i<=np-1; i++) {  // the last player is not considered
      count = 0; // counter of the compatible players
//      verbosefile <<  "player " << idpl[i] << ":";
      for (j=np2; j>i-np2; j--) {
           opponent = challengerFlag[j]; // opponent list
           if (CanPlayWith(idpl[i], idpl[ opponent ], nround)) {
	           count++;
	           remind[count] = opponent;
	       }
      }
      remind[0] = count;

      //for (l=1; l<=count; l++) {
      //    verbosefile << idpl[ remind[l] ] <<"; ";
      //}
      //verbosefile << endl;

      // allocate space for a vector that store 'remind', that is the opponent list
      sss[i] = (unsigned int *) calloc(count+1, sizeof(unsigned int) );

      // verbosefile <<  "opps= " << count << " -> ";
      for (l=0; l<=count; l++) {
         sss[i][l] = remind[l]; //store remind
	     //if (l>0) verbosefile <<  idpl[ sss[i][l] ] <<", ";
      }
      //verbosefile << endl;
  }
  // last element need a special treatment because he has no opponent
  sss[np] = (unsigned int *) calloc(1, sizeof(unsigned int) );
  sss[np][0] = 0;
 }
 else {  // np_s1>0
  // build the opponent list for each player in the case np_s1=0
  for (i=1; i<=np_s1; i++) {
      count = 0; // counter of the compatible players
      //verbosefile <<  "player " << idpl[i] << ":";
      for (j=1; j<=np2; j++) {
           opponent = challengerFlag[j]; // opponent list
           if (CanPlayWith(idpl[i], idpl[ opponent ], nround)) {
	       count++;
	       remind[count] = opponent;
	   }
      }
      remind[0] = count;

      /* for (l=1; l<=count; l++) {
         verbosefile << idpl[ sss[i][l] ] <<"; ";
      }
      verbosefile << endl;
      */
      // allocate space for a vector that store 'remind', that is the opponent list
      sss[i] = (unsigned int *) calloc(count+1, sizeof(unsigned int) );

      //verbosefile <<  "opps= " << count << " -> ";
      for (l=0; l<=count; l++) {
         sss[i][l] = remind[l]; //store remind
	     //if (l>0) verbosefile<<  idpl[ sss[i][l] ] <<",";
      }
      //verbosefile <<endl;
  }
  for (i=np_s1+1; i<=np; i++) {
      //verbosefile <<  "player " << idpl[i] << ":";
      // allocate space for a vector that store 'remind', that is the opponent list
      sss[i] = (unsigned int *) calloc(1, sizeof(unsigned int) );
      sss[i][0] = 0;
      //verbosefile<<  "opps= " << idpl[sss[i][0]] << " -> " << endl;
  }
 }


  // reset the cursor of each vector_challenger
  for (i=1; i<=np; i++) curs_position[i] = 0;

  // reset the array that remind the destroyed pairing
  for (i=0; i<=LIMIT; i++) avoid[i] = 0;

  // reset the pair matrix
  for (i=1; i<=np2; i++)
      for (j=0; j<=1; j++) {
           pair[i][j] = 0;
      }

  max_pair = np2; // massimo numero di coppie possibile: sono np/2 se S1=S2
                  // altrimenti np_s1 se S1<S2
  if (np_s1) max_pair = np_s1;
  n_couple = 1;
  while ( (n_couple <= max_pair) && pairing_possible)
    { //verbosefile <<"n_couple = " <<n_couple<< " "<< endl;

      if (n_couple>maximum_possible_pair)
		   {
		     for (l=1; l<=maximum_possible_pair; l++) {
			     partial_pair[l][0] = pair[l][0];
			     partial_pair[l][1] = pair[l][1];
			 }
		     return maximum_possible_pair;
		   }
      //if (np_s1) i_player = findPlayer(IsPlayerFree, np_s1+1);
      //else
      i_player = findPlayer(IsPlayerFree, np);
      //verbosefile <<"Found " <<i_player << ": "<<idpl[i_player] <<endl;
      // if (i_player>=np)  { verbosefile <<i_player<< endl; }

LABEL100:
      if (i_player<np) j_player = findChallenger( i_player, np);
      else  j_player = 0;  // destroy the last good couple
      //verbosefile <<"  Found opponent" <<j_player<< ": "<<idpl[j_player]<<endl;
      found=0;
      if (j_player) { // j_player>0  imply  i_player<np
                 if (checkCompatibility) { // check if this new pair appeared before
                        // codify the pairing
			            sum = 0;
			            for (ll=1; ll<=n_couple-1; ll++) sum += pair[ll][0] * pair[ll][1];
                        sum += i_player * j_player;
			            // found if present
			            found = 0;
			            j=1;
			            while (!found && j<=avoid[0]) {
			              if(sum==avoid[j] ) {
			                 found = 1;
			                 //verbosefile <<"  Found removed pairing! " <<sum<< endl;
                          }
		                  else j++;
                        }
                     }
                  if(!found) {
                      //checkCompatibility = 0;  ??
		              IsPlayerFree[i_player] = 0; // player to be skipped in future
	                  IsPlayerFree[j_player] = 0; // player to be skipped in future
 	                  pair[n_couple][0] = i_player;
		              pair[n_couple][1] = j_player;
	                  n_couple++;
	              }
		          else goto LABEL100;
       }
      else { if (n_partial<n_couple-1)
		     { // remind best partial couples
		       n_partial = n_couple-1;
		       for (l=1; l<=n_partial; l++) {
			     partial_pair[l][0] = pair[l][0];
			     partial_pair[l][1] = pair[l][1];
			   }
		     }

           if (n_couple<=1) pairing_possible = 0; // go to finish
	       else  { //  go back one step destroying the last pair

		           // store in code the pairing destroyed to avoid it in future
		           avoid[0]++; // count the number of destroyed pair at this point
		           if(avoid[0] >= LIMIT) {
                             //if (IS_SET_VERBOSE) verbosefile <<"  LIMIT REACHED... going out...\n";
                             Exclamation(" The iteration limit has been reached.&I am going out...");
                             flag_limit = 1;
                             return n_partial;
                   }
		           for (ll=1; ll<=n_couple-1; ll++)
		                avoid[ avoid[0] ] += pair[ll][0] * pair[ll][1];
		           checkCompatibility = 1;  // flag to force the check on the next pair after the restart

		           n_couple = n_couple - 2;

		           // restore the flags of IsPlayerFree[]
		           resetplayerFlag(IsPlayerFree, np);

		           // restore IsPlayerFree[] of incompatibility
		           for (ll=1; ll<=n_couple; ll++)
		   	       { i_player_last = pair[ll][0];
			         IsPlayerFree[i_player_last] = 0;
			         //curs_position[i_player_last] = 0;

			         j_player_last = pair[ll][1];
			         IsPlayerFree[j_player_last] = 0;
			         //curs_position[j_player_last] = 0;
                   }
	               n_couple++; //restart the pairing from N-1
	       }
	   }
     }
  if (pairing_possible) return max_pair;
  else return n_partial;
}


// set to 1 all the elements of the vector
void TPairingDubov::resetplayerFlag(int vector[], int n)
{ int i;
  for (i=1; i<=n; i++) vector[i] = 1; // 1 = not busy
  // if ( idpl[ vector[np2] ] == -1 && np>2) vector[np2+1] = 0; // ?????????
  if ( idpl[ vector[np2] ] == -1 ) vector[np2] = 0; // ?????????
}

void TPairingDubov::resetchallengerFlag(int vector[], int n)
{ int i;

  if (np_s1) { // ci sono meno giocatori in S1 che in S2
     for (i=1; i<=np2; i++) vector[i] = np2+i;
     //    for (i=n; i>np2; i--)  vector[i] = n-i+1;
     for (i=np2+1; i<=n; i++)  vector[i] = i - np2; // fixed bug CONTROLLARE

     // rimuove np_s1 giocatori dal challengerflag per evitare incontri tra giocatori di S1
     for (i=n; i > np2; i--) vector[i] = 0;

  }
  else
  {  for (i=1; i<=np2; i++) vector[i] = np2+i;
     //     for (i=n; i>np2; i--)  vector[i] = n-i+1;
     for (i=np2+1; i<=n; i++)  vector[i] = i - np2; // fixed bug CONTROLLARE
  }
  //for (l=1; l<=n; l++) verbosefile << idpl[ vector[l] ] <<", "; verbosefile << endl;
}

// get the next free row in which find the player i_th
int TPairingDubov::findPlayer(int index[], int n)
{ int r=1;
  while ( ( index[r]==0 || sss[r][0]==0 ) && r<n ) r++;
  return r;
}

// find the challenger of the i_th in his challenger list
int TPairingDubov::findChallenger(int i, int n)
{ unsigned int j;

  j = curs_position[i] + 1;
  while ( j<=sss[i][0] && !IsPlayerFree[sss[i][j]] ) j++;
  if (j>sss[i][0] ) { // overcame the limit
     curs_position[i] = 0; // restart the cursor from zero;
     return 0; // no player found => player i_th not compatible with anybody
  }
  else {
     curs_position[i] = j; // adjourn the cursor
     return sss[i][j];
  }
  return 0; // fictitious return;
}


int TPairingDubov::dopairingMax(int nplayer, int npair_max, int idplayers[], int S1_S2[][2],
		     int arrfloat[], int arrpartial[][2], int round)
//
// PURPOSE: This is the driver of the routine pairing. It looks for the floaters
// The players enter in two columns in idplayers. The final pairing is put
// in S1_S2
//
// INPUT
// 	nplayer         = total players to be paired
//  npair_max       = maximum pairs that it is expected to obtain ( less or equal to nplayer/2 )
//	idplayers[]     = vector containg the ID of the nplayers players
//	S1_S2[][2]      = will contain the final pairs on success
//  arrfloat[]      = will contain the eventual floaters
//	arrpartial[][2] = will contain the pairs if the floaters are met
//	round           = current round
//
// OUTPUT
// 	return the number of pairs done: they are nplayers/2 if there are no
//      floaters, otherwise they are less than nplayers/2.
//
{ int i;
  int count_floaters = 0;
  int id;

  //-------------
  np = nplayer;     // e' globale
  np2 = (int) np/2;   // e' globale
  np_s1 = 0;    // e' globale e se zero significa S1 = S2
  maximum_possible_pair = npair_max;  // e' globale
  for (i=1; i<=np; i++)  idpl[i] = idplayers[i];   // now is made global
  nround = round;                                  // now is global
  //-------------

  nmade = pairing();
  // look up pair
  if ( nmade == np2 ) // no floaters: reached the maximum pairs possible
     { n_floaters = 0;
       for (i=1; i<=np2; i++)
           { id = pair[i][0];
	         S1_S2[i][0] = idplayers[id];

	         id = pair[i][1];
	         S1_S2[i][1] = idplayers[id];
           }
       arrfloat[0] = 0;  // no floaters
     }
  else // floaters treatment
     {
       n_floaters = 2*(np2-nmade);
       for (i=1; i<=np; i++) mask[i] = 1;
       for (i=1; i<=nmade; i++)
           { // let emerge the the floaters, they aren't in partial and in mask are denoted with 1
             mask[ partial_pair[i][0] ] = 0;
             mask[ partial_pair[i][1] ] = 0;
           }
       for (i=1; i<=np; i++)
           { if (mask[i] == 1) {
	             count_floaters++;
                 arrfloat[count_floaters] = i;
 	     }
	   }
     }

  if (n_floaters) {
      // set partial_pair array
	  arrfloat[0] = n_floaters;
	  for (i=1; i<=nmade; i++)
	      {
	        id = partial_pair[i][0];
	        arrpartial[i][0] = idplayers[id];

		    id = partial_pair[i][1];
		    arrpartial[i][1]= idplayers[id];
	      }

	  // set floaters looking up their real id
	  for (i=1; i<=n_floaters; i++)
	      {
	        id = arrfloat[i];
	        arrfloat[i] = idplayers[id];
	      }
  }
  for (i=1; i<=np; i++) if (sss[i] != NULL) free(sss[i]);
  if (flag_limit) return -nmade;
  else return nmade;
}


int TPairingDubov::dopairingFixed(int nplayer, int nplayer_s1, int idplayers[],
                         int S1_S2[][2], int round)
//
// PURPOSE: This is the driver of the routine pairing. It looks for the floaters
// The players enter in two columns in idplayers. The final pairing is put
// in S1_S2
//
// INPUT
// 	nplayer         = total players to be paired
//  player_s1       = players to pair in S1
//	idplayers[]     = vector containg the ID of the nplayers players
//	S1_S2[][2]      = will contain the final pairs on success
//	round           = current round
//
// OUTPUT
// 	return the number of pairs done: they are nplayers/2 if there are no
//      floaters, otherwise they are less than nplayers/2.
//
{ int i;
  int id;

  //-------------
  np = nplayer; // e' globale
  np2 = np/2;   // e' globale
  np_s1 = maximum_possible_pair = nplayer_s1; // e' globale S1 < S2 => np_s1 < np_s2
  for (i=1; i<=np; i++)  idpl[i] = idplayers[i];   // now is made global
  nround = round;                                 // now is global
  //-------------

  nmade = pairing();

  // look up pair
  if ( nmade == np_s1 )
     {
       for (i=1; i<=np_s1; i++)
           { id = pair[i][0];
             S1_S2[i][0] = idplayers[id];

	         id = pair[i][1];
	         S1_S2[i][1] = idplayers[id];
           }
     }
  else  nmade = 0;  // l'accoppiamento e' stato impossibile
  for (i=1; i<=np_s1; i++) delete sss[i];
  return nmade;
}

//------------------- end implementazione TPairingDubov -------------------


//------------------- Start implementazione TPairingLim -------------------

int TPairingLim::pairingLIM()
{ int i, j, l, n_couple, sum, found;
  int pairing_possible = 1, max_pair;
  int count, i_player, j_player, checkCompatibility=0;
  int ll, opponent, i_player_last, j_player_last;
  int n_partial=0; //pair at the moment
  int avoid[LIMIT+1];
  int opponentlist[N_PLAYER_MAX+1];
  int typefloater;

  resetplayerFlag(IsPlayerFree, np);

  // reset the sss matrix to null
  for (i=1; i<=np; i++) sss[i] = NULL;

  // reset the cursor of each vector_challenger
  for (i=1; i<=np; i++) curs_position[i] = 0;

  // reset the array that remind the destroyed pairing
  for (i=0; i<=LIMIT; i++) avoid[i] = 0;

  // reset the pair matrix
  for (i=1; i<=np2; i++)
      for (j=0; j<=1; j++) {
           pair[i][j] = 0;
      }
  //verbosefile <<"group with points " << scoregroup<<endl;
  // massimo numero di coppie possibili: sono np/2 se S1=S2
  max_pair = np2;
  n_couple = 1;
  while ( (n_couple <= max_pair ) && pairing_possible )
    {
      if (n_couple>maximum_possible_pair)
		   { // go to finish
		     for (l=1; l<=maximum_possible_pair; l++) {
			     partial_pair[l][0] = pair[l][0];
			     partial_pair[l][1] = pair[l][1];
			 }
		     return maximum_possible_pair;
		   }
      if (TD.IS_SET_VERBOSE) verbosefile <<" pair " <<n_couple<< " : ";

      // start to find the first seeker
      i_player = findPlayerLIM(IsPlayerFree, np, &typefloater);

      //retrieve the list with all opponents
      makevectorchallengers(i_player, IsPlayerFree, opponentlist, typefloater);
      //remove from the list the players not compatible
      count = 0;
      for (j=1; j<=opponentlist[0]; j++) {
           opponent = opponentlist[j]; // opponent list
           if (CanPlayWith(idpl[i_player], idpl[ opponent ], nround)) {
	          count++;
	          remind[count] = opponent;
	       }
      }
      remind[0] = count;
      //if ( sss[i_player]==NULL ) sss[i_player] = (unsigned int *) calloc(count+1, sizeof(unsigned int) );
      //else sss[i_player] = (unsigned int *) realloc( sss[i_player], count+1);
      if ( sss[i_player]==NULL ) { //fixed BUG
           sss[i_player] = (unsigned int *) calloc(count+1, sizeof(unsigned int) );
           //if (!sss[i_player]) Exclamation("Memory allocation error.&Report this problem to luigi.forlano@tin.it" );
      }
      else {
         free(sss[i_player]); //libera la memoria per riutilizzarla
         sss[i_player] = (unsigned int *) calloc(count+1, sizeof(unsigned int) );;
      }
      if (TD.IS_SET_VERBOSE) verbosefile << idpl[i_player];
      for (l=0; l<=count; l++) {
          sss[i_player][l] = remind[l]; //store remind
//---- if (IS_SET_VERBOSE)
//         if (l>0) verbosefile <<  idpl[ sss[i_player][l] ] <<", ";
      }

LABEL100:

      if (i_player<=np) j_player = findChallenger( i_player, np);
      else  j_player = 0;  // destroy the last good couple
      if (TD.IS_SET_VERBOSE) {
           if (j_player) verbosefile <<" try opponent " << idpl[j_player]<< "\n";
           else verbosefile <<" has no suitable opponent! restart... " << "\n";
      }

      found=0;
      if (j_player) { // j_player>0  imply  i_player<np
                 if (checkCompatibility) { // check if this new pair appeared before
                        // codify the pairing
			            sum = 0;
			            for (ll=1; ll<=n_couple-1; ll++) sum += pair[ll][0] * pair[ll][1];
                        sum += i_player * j_player;
			            // found if present
			            found = 0;
			            j=1;
			            while (!found && j<=avoid[0]) {
			              if(sum==avoid[j] ) {
			                 found = 1;
			                 //verbosefile <<"  Found removed pairing! " <<sum<< endl;
                          }
		                  else j++;
                        }
                     }
                  if(!found) { // the pairing never appeared so far
                      //checkCompatibility = 0;  ??
		              IsPlayerFree[i_player] = 0; // player to be skipped in future
	                  IsPlayerFree[j_player] = 0; // player to be skipped in future
 	                  pair[n_couple][0] = i_player;
		              pair[n_couple][1] = j_player;
	                  n_couple++;
	              }
		          else goto LABEL100;
       }
       else { if (n_partial<n_couple-1)
		     { // remind best partial couples
		       n_partial = n_couple-1;
		       for (l=1; l<=n_partial; l++) {
			     partial_pair[l][0] = pair[l][0];
			     partial_pair[l][1] = pair[l][1];
               }
		     }

           if (n_couple<=1) pairing_possible = 0; // go to finish
	       else  { //  go back one step destroying the last pair

		           // store in code the pairing destroyed to avoid it in future
		           avoid[0]++; // count the number of destroyed pair at this point
		           if(avoid[0] >= LIMIT) {
                             Exclamation(" The iteration limit has been reached.&I am going out...");
                             //----if (IS_SET_VERBOSE) verbosefile <<"  LIMIT REACHED... going out...\n";
                             flag_limit = 1;
                             return n_partial;
                   }
		           for (ll=1; ll<=n_couple-1; ll++)
		                avoid[ avoid[0] ] += pair[ll][0] * pair[ll][1];
		           checkCompatibility = 1;  // flag to force the check on the next pair after the restart

		           n_couple = n_couple - 2;

		           // restore the flags of IsPlayerFree[]
		           resetplayerFlag(IsPlayerFree, np);

		           // restore IsPlayerFree[] before of incompatibility
                   for (ll=1; ll<=n_couple; ll++)
		   	       { i_player_last = pair[ll][0];
			         IsPlayerFree[i_player_last] = 0;
			         //curs_position[i_player_last] = 0;

			         j_player_last = pair[ll][1];
			         IsPlayerFree[j_player_last] = 0;
			         //curs_position[j_player_last] = 0;
                   }
                   n_couple++; //restart the pairing from N-1
	       }
	   }
     }
  if (pairing_possible) return max_pair;
  else return n_partial;
}

// get the next free row in which find the player i_th
int TPairingLim::findPlayerLIM(int index[], int n, int *typeFloater)
{ int r;
  r=1;  // look for downfloater from left to right
  while ( index[r]==0 && r<n ) r++;
  if (player[ idpl[r] ].tiebreak[SCORE]>scoregroup) { // found downfloater
      *typeFloater = DOWNFLOATER;
      return r;
  }

  r=n;  // look for  upfloater from right to left
  while ( index[r]==0 && r>1 ) r--;
  if (player[ idpl[r] ].tiebreak[SCORE]<scoregroup) { // found upfloater
      *typeFloater = UPFLOATER;
      return r;
  }

  if (DOWNWARD) { // moving downward
     //if (IS_SET_VERBOSE) verbosefile <<" moving down: " ;
     r=1;  // look for normal player in the group from left to right for
     while ( index[r]==0 && r<n ) r++;
     *typeFloater = STANDARD;
     return r;
  }
  else { // moving upward
     //if (IS_SET_VERBOSE) verbosefile <<" moving   up: " ;
     r=n;  // look for normal player in the group from right to left
     while ( index[r]==0 && r>1 ) r--;
     *typeFloater = STANDARD;
     return r;
  }
}

void TPairingLim::makevectorchallengers(int id, int freeplayer[], int challengers[], int floaterType)
{ int i, count = 0, aux, m;
  int x, y, t, duec;

  // grab the remained unpaired players.
  switch (floaterType)
  { case STANDARD:   // STANDARD TREATMENT: player id is NOT a floater

     if (DOWNWARD) {
      // moving downward get all unpaired players from left to right
      // 1,2,3,4, 5,6,7,8
      for (i=1; i<=np; i++) {
         if (freeplayer[i]) {
            count++;
            challengers[count] = i;
         }
      }
      // Exchange the bottom half with the top half
      // 5,6,7,8  1,2,3,4,
      m = count/2;
      for (i=1; i<=m; i++) {
         aux = challengers[i];
         challengers[i] = challengers[i+m];
         challengers[i+m] = aux;
      }
      // revert the order of the bottom half
      // 5,6,7,8  3,4,1,2,
      for (i=1; i<=m/2; i++) {
         aux = challengers[i+m];
         challengers[i+m] = challengers[count + 1 -i];
         challengers[count + 1 -i] = aux;
      }
     }
     else {
      // moving upward get all unpaired players from right to left
      // 1,2,3,4,  5,6,7,8 it is the final wanted order!
      for (i=1; i<=np; i++) {
         if (freeplayer[i]) {
            count++;
            challengers[count] = i;
         }
      }
      // revert the order of the top half
      // 4,3,1,2,  5,6,7,8 it is the final wanted order!
      m = count/2;
      for (i=1; i<=m/2; i++) {
         aux = challengers[i];
         challengers[i] = challengers[m + 1 -i];
         challengers[m + 1 -i] = aux;
      }
     }
	break;

    case DOWNFLOATER:
      // DOWNFLOATER TREATMENT: player id is a downfloater
      // ..... no sort, just the actual rating order in descendent order
      if (TD.IS_SET_VERBOSE) verbosefile <<"(downfloater) " ;
      for (i=1; i<=np; i++) {
         if (freeplayer[i]){
            count++;
            challengers[count] = i;
         }
      }
    break;

    case UPFLOATER:
    // UPFLOATER TREATMENT: player id is an upfloater
    // ..... no sort, grab the remained player from the lowest score-rated to
    // the highest rated, i.e. the reverse order
      if (TD.IS_SET_VERBOSE) verbosefile <<"(upfloater) ";
      for (i=np; i>=1; i--) {
         if (freeplayer[i]) {
            count++;
            challengers[count] = i;
         }
      }
     break;
  }

  // sort the challengers depending on the due color of player id
  duec=player[ idpl[id] ].duecolor;;
  if (duec == -1) { // if duecolor neutral then do not sort the color
     challengers[0] = count;
     return;
  }
  y=2;
  while ( y<=count)
    { if ( player[ idpl[challengers[y]] ].duecolor == duec ) y++;
      else {
           // found a different duecolor that must be brought up
           // if this do not alter the ranking score
           x = y;
           while (x>1) {
              if (player[ idpl[challengers[x-1]] ].duecolor == duec
                 && player[ idpl[challengers[x-1]] ].tiebreak[SCORE]==player[ idpl[challengers[x]] ].tiebreak[SCORE])
              { t = challengers[x-1]; challengers[x-1] = challengers[x]; challengers[x] = t; x--;}
              else  break;
           }
           y++;
         }
    }
  challengers[0] = count;
}

int TPairingLim::dopairingMax(int nplayer, int npair_max, int idplayers[], int S1_S2[][2],
		     int arrfloat[], int arrpartial[][2], int round, float SCOREGROUP, String& verbose)
//
// PURPOSE: This is the driver of the routine pairing. It looks for the floaters
// The players enter in two columns in idplayers. The final pairing is put
// in S1_S2
//
// INPUT
// 	nplayer         = total players to be paired
//  npair_max       = maximum pairs that it is expected to obtain ( less or equal to nplayer/2 )
//	idplayers[]     = vector containg the ID of the nplayers players
//	S1_S2[][2]      = will contain the final pairs on success
//  arrfloat[]      = will contain the eventual floaters
//	arrpartial[][2] = will contain the pairs if the floaters are met
//	round           = current round
//
// OUTPUT
// 	return the number of pairs done: they are nplayers/2 if there are no
//      floaters, otherwise they are less than nplayers/2.
//
{ int i;
  int count_floaters = 0;
  int id;
  verbosefile = "";
  //-------------
  np = nplayer;     // e' globale
  np2 = (int) np/2;   // e' globale
  np_s1 = 0;    // e' globale e se zero significa S1 = S2
  maximum_possible_pair = npair_max;  // e' globale
  for (i=1; i<=np; i++)  idpl[i] = idplayers[i];   // now is made global
  nround = round;                                  // now is global
  //-------------

  // Lim treatment   : check scoregroup
  scoregroup = SCOREGROUP; //score of the group we are considering
  if ( SCOREGROUP >= (round-1)*TD.WON/2.) DOWNWARD=1;
  else DOWNWARD=0;
  nmade = pairingLIM();

  // look up pair
  if ( nmade == np2 ) // no floaters: reached the maximum pairs possible
     { n_floaters = 0;
       for (i=1; i<=np2; i++)
           { id = pair[i][0];
	         S1_S2[i][0] = idplayers[id];

	         id = pair[i][1];
	         S1_S2[i][1] = idplayers[id];
           }
       arrfloat[0] = 0;  // no floaters
     }
  else // floaters treatment
     {
       n_floaters = 2*(np2-nmade);
       for (i=1; i<=np; i++) mask[i] = 1;
       for (i=1; i<=nmade; i++)
           { // let emerge the the floaters, they aren't in partial and in mask are denoted with 1
             mask[ partial_pair[i][0] ] = 0;
             mask[ partial_pair[i][1] ] = 0;
           }
       for (i=1; i<=np; i++)
           { if (mask[i] == 1) {
	             count_floaters++;
                 arrfloat[count_floaters] = i;
 	     }
	   }
     }

  if (n_floaters) {
      // set partial_pair array
	  arrfloat[0] = n_floaters;
	  for (i=1; i<=nmade; i++)
	      {
	        id = partial_pair[i][0];
	        arrpartial[i][0] = idplayers[id];

		    id = partial_pair[i][1];
		    arrpartial[i][1]= idplayers[id];
	      }

	  // set floaters looking up their real id
	  for (i=1; i<=n_floaters; i++)
	      {
	        id = arrfloat[i];
	        arrfloat[i] = idplayers[id];
	      }
  }
  for (i=1; i<=np; i++) if (sss[i] != NULL) free(sss[i]);
  verbose = verbosefile;
  if (flag_limit) return -nmade;
  else return nmade;
}
