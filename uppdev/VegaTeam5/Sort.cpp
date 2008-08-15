#include "VegaMain.h"
#include "Tournament.h"

/*
** this file contains all the sort function used in the program
*/

int  CanPlayWith(int player1, int player2, int round);
int  CanPlayWithThem(int player, int players[], int nplayers, int round);
int  comppair(int j1, int j2, int round);
void SortPairs(int round);
int  CompareRatingTitleName( const void *pid1, const void *pid2);
int  CompareBLACK(const void *pid1, const void *pid2);
int  CompareWHITE(const void *pid1, const void *pid2);
int  CompareScoreName(const void *pid1, const void *pid2);
int  CompareCategoryScoreName(const void *pid1, const void *pid2);
int  CompareName(const void *pid1, const void *pid2);
int  CompareCountry(const void *pid1, const void *pid2);
void SortUpfloater(int colorflag, int n, int array[]);
int  CompareUpfDuecRatAroNome0(int id1, int id2);
int  CompareUpfDuecRatAroNome1(int id1, int id2);
int  CompareUpfDuecRatAroNome2(int id1, int id2);
void SortDownfloater(int colorflag, int n, int array[]);
int CompareDuecRatAroNome0(int id1, int id2);
int CompareDuecRatAroNome1(int id1, int id2);
int CompareDuecRatAroNome2(int id1, int id2);

int  cmpdata(int d1, int d2);
void DecodeData(int code, int *dd, int *mm, int *yy);

int  cmpSB(int id1, int id2);
int  cmpBUC1(int id1, int id2);
int  cmpBUCM(int id1, int id2);
int  cmpBUCT(int id1, int id2);
int  cmpCUMUL(int id1, int id2);
int  cmpARO(int id1, int id2);
int  cmpMB(int id1, int id2);
int  cmpMWINS(int id1, int id2);
int  cmpAPRO(int id1, int id2);
int  cmpMPSS(int id1, int id2); // missing point score system
int  (*cmpfunc[FUNC_CMP+1])(int, int)={NULL, cmpBUC1, cmpBUCT, cmpBUCM, cmpSB, cmpCUMUL,
                                      cmpARO, cmpMB, cmpMWINS, cmpAPRO, cmpMPSS}; // array of pointer to function

/********************** test player compatibility ***********************/
int CanPlayWith(int player1, int player2, int round)
//
// PURPOSE: check if player1 and player2 are compatible,
//          i.e. if they: 1) have not yet played before, and 2) are color compatible
//
// INPUT VARIABLE
//	player1 = ID of the player 1
//	player2 = ID of the player 2
//	round   = current round
//
// OUTPUT VARIABLE
//	1 if yes, 0 otherwise
//
{ int i;
  if (player1 == player2) return 0; 	// a player cannot play with himself!
  if ((player1 == -1) || (player2 == -1)) return 1; // -1 is a fictious player compatible with everybody

  //  if (AVOID_SAME_COUNTRY) if ( strcmp(player[player1].name, player[player2].name )==0 ) return 0;
  if ( (player1 == 0) && (player[player2].canGetBYE == 1) ) return 1; 	// the bye can be assigned
  else if ( (player2 == 0) && (player[player1].canGetBYE == 1) ) return 1; 	// the bye can be assigned

  for (i=1; i<round; i++) if ( TD.playerOpponent[player2][i] == player1 ) return 0;
  // check one round less because round was previously incremented by 1

  // further check about colors although they didn't meet each other
  if (player[player1].duecolor != player[player2].duecolor) return 1;
  else if ( player[player1].canChangeDuecolor ||
            player[player2].canChangeDuecolor ) return 1;
     //  else if ( !player[player1].canChangeDuecolor && !player[player2].canChangeDuecolor
     //             && PERMIT3COLOR ) return 1;  // permit the same colors in a row

  return 0;
}


int CanPlayWithThem(int idplayer, int players[], int nplayers, int round)
//
// PURPOSE: check if player is compatible with respect to the opponent in players[],
//          i.e. if he can play with at least one player in the group hold in the
//          array 'players'. The color compatibility is even considered
//
// INPUT VARIABLE
//	player  = ID of the player
//	players = ID of the players to be checked
//	nplayer = how many players are in the group to be checked
//	round   = check up to round 'round'
//
// OUTPUT VARIABLE
//	1 if yes, 0 otherwise
//
{
  int i, j, res;
  for (j=1; j<=nplayers; j++) {
	  res = true;
	  for (i=1; i<round; i++)
     	      { if ( TD.playerOpponent[ idplayer ][i] == players[j] ) {
		        res = false;
		        break;
	            }
	          }
  	  // further check about colors although they didn't meet each other
	  if (res) {
//	     if (AVOID_SAME_COUNTRY) {
//              if ( strcmp(player[idplayer].name, player[ players[j] ].name )==0 )
//              return false;
//         }
         if (player[idplayer].duecolor != player[players[j]].duecolor) return true;
	     else if ( player[idplayer].canChangeDuecolor ||
	            player[players[j]].canChangeDuecolor ) return true;
 //        else if ( !player[idplayer].canChangeDuecolor && !player[players[j]].canChangeDuecolor
 //           && PERMIT3COLOR ) return true;  // permit the same colors 3 times in a row
      }
  }
  return false;
}
/*
void SortPairs(int round)
//
// PURPOSE: sort the pairs of the pairing with respect some criteria defined in comppair
//
{ int i, j, t, N;
  N = pairing[0][round];
  for(i=1; i<=N-1; i++)
     for (j=i+1; j<=N; j++)
          if ( comppair(i, j, round)<0 )
             { t = pairing[j][round];
               pairing[j][round] = pairing[i][round];
               pairing[i][round] = t;
             }

}
*/

void SortPairs(int round)
//
// PURPOSE: sort the pairs of the pairing with respect some criteria defined in comppair
//
{ int i, j, t, N;
  N = TD.pairing[0][round];
  for (i=N; i>=1; i--)
      for (j=2; j<=i; j++)
          if ( comppair(j-1, j, round)<0 )
             { t = TD.pairing[j-1][round];
               TD.pairing[j-1][round] = TD.pairing[j][round];
               TD.pairing[j][round] = t;
             }

}


int comppair(int j1, int j2, int round)
//
// compare two pairs
//
{ int code1, code2;
  int idw1, idw2, idb1, idb2, res;
  int m1, m2;
  float max1, max2, s1, s2;

  code1 = TD.pairing[j1][round];
  TD.DecodeResult(code1, &idw1, &idb1, &res);
  if (idw1==0 || idb1==0) return 0;

  max1 = player[idw1].tiebreak[SCORE];
  if (player[idb1].tiebreak[SCORE]>max1) max1 = player[idb1].tiebreak[SCORE];

  code2 = TD.pairing[j2][round];
  TD.DecodeResult(code2, &idw2, &idb2, &res);
  if (idw2==0 || idb2==0) return 0;

  max2 = player[idw2].tiebreak[SCORE];
  if (player[idb2].tiebreak[SCORE]>max2) max2 = player[idb2].tiebreak[SCORE];

  if (max2>max1) return -1;
  else if (max2<max1) return 1;

  s1 = player[idw1].tiebreak[SCORE] + player[idb1].tiebreak[SCORE];
  s2 = player[idw2].tiebreak[SCORE] + player[idb2].tiebreak[SCORE];
  if (max2==max1 && s2>s1) return -1;
  else if (max2==max1 && s2<s1) return 1;

  m1 = player[idw1].rating;
  if (player[idb1].rating>m1) m1 = player[idb1].rating;
  m2 = player[idw2].rating;
  if (player[idb2].rating>m2) m2 = player[idb2].rating;

  if (max2==max1 && s2==s1 && m2>m1) return -1;
  return 1; // silly return with no effect to avoid complain from compiler
}


/************* compare routine during pairing for colored subgroup **************/
int CompareRatingTitleName(const void *pid1, const void *pid2)
{ // id1>id2 return -1, else return 1
 int *id1, *id2;
 id1 = (int *) pid1;
 id2 = (int *) pid2;

 if (player[*id1].RATING > player[*id2].RATING) return -1;
 else if ( (player[*id1].RATING == player[*id2].RATING)
            && (player[*id1].title > player[*id2].title)) return -1;
 else if ( (player[*id1].RATING == player[*id2].RATING)
            && (player[*id1].title == player[*id2].title)
	        && (strcmp(player[*id1].name, player[*id2].name)<0)) return -1;
 else return 1;
}

/************* compare routine during the chooce of an upfloater **************/
void SortUpfloater(int colorflag, int n, int array[])
//
// PURPOSE: sort the pairs of the pairing with respect some criteria defined in comppair
//
{ int i, j, t;
  for(i=1; i<=n-1; i++)
     { for (j=i+1; j<=n; j++)
       { switch (colorflag)
	       { case 0: if ( CompareUpfDuecRatAroNome0( array[i], array[j])<0)
                       { t = array[i]; array[i] = array[j]; array[j] = t;}
             break; // no color preference

             case 1: if ( CompareUpfDuecRatAroNome1( array[i], array[j])<0)
                       { t = array[i]; array[i] = array[j]; array[j] = t;}
             break; // white color

             case 2: if ( CompareUpfDuecRatAroNome2( array[i], array[j])<0)
                       { t = array[i]; array[i] = array[j]; array[j] = t;}
             break; // black color
	       }
       }
     }
}

int CompareUpfDuecRatAroNome0(int id1, int id2)
{ // black = white, group equal
  if (player[id1].canUpfloat < player[id2].canUpfloat) return -1;
  else if ( (player[id1].canUpfloat == player[id2].canUpfloat)
            && (player[id1].RATING > player[id2].RATING) ) return -1;
  else if ( (player[id1].canUpfloat == player[id2].canUpfloat)
            && (player[id1].RATING == player[id2].RATING)
            && (player[id1].aro < player[id2].aro) ) return -1;
  else if ( (player[id1].canUpfloat == player[id2].canUpfloat)
            && (player[id1].RATING == player[id2].RATING)
            && (player[id1].aro == player[id2].aro)
	        && (strcmp(player[id1].name, player[id2].name)>0)) return -1;
  else return 1;
}

int CompareUpfDuecRatAroNome1(int id1, int id2)
{ // get a white player
  if (player[id1].canUpfloat < player[id2].canUpfloat) return -1;
  else if ( (player[id1].canUpfloat == player[id2].canUpfloat)
          && (player[id1].duecolor < player[id2].duecolor) ) return -1;
  else if ( (player[id1].canUpfloat == player[id2].canUpfloat)
          && (player[id1].duecolor==player[id2].duecolor)
          && (player[id1].RATING < player[id2].RATING) ) return -1;
  else if ( (player[id1].canUpfloat == player[id2].canUpfloat)
          && (player[id1].duecolor==player[id2].duecolor)
          && (player[id1].RATING == player[id2].RATING)
          && (player[id1].aro < player[id2].aro) ) return -1;
  else if ( (player[id1].canUpfloat == player[id2].canUpfloat)
          && (player[id1].duecolor==player[id2].duecolor)
          && (player[id1].RATING == player[id2].RATING)
          && (player[id1].aro == player[id2].aro)
	      && (strcmp(player[id1].name, player[id2].name)>0)) return -1;
  else return 1;
}

int CompareUpfDuecRatAroNome2(int id1, int id2)
{ if (player[id1].canUpfloat < player[id2].canUpfloat) return -1;
  else if ( (player[id1].canUpfloat == player[id2].canUpfloat)
          && (player[id1].duecolor > player[id2].duecolor) ) return -1;
  else if ( (player[id1].canUpfloat == player[id2].canUpfloat)
          && (player[id1].duecolor==player[id2].duecolor)
          && (player[id1].RATING < player[id2].RATING) ) return -1;
  else if ( (player[id1].canUpfloat == player[id2].canUpfloat)
          && (player[id1].duecolor==player[id2].duecolor)
          && (player[id1].RATING == player[id2].RATING)
          && (player[id1].aro < player[id2].aro) ) return -1;
  else if ( (player[id1].canUpfloat == player[id2].canUpfloat)
          && (player[id1].duecolor==player[id2].duecolor)
          && (player[id1].RATING == player[id2].RATING)
          && (player[id1].aro == player[id2].aro)
	      && (strcmp(player[id1].name, player[id2].name)>0)) return -1;
  else return 1;
}


/************* compare routine to chooce the opponent of the upfloater **************/
void SortDownfloater(int colorflag, int n, int array[])
//
// PURPOSE: sort the pairs of the pairing with respect some criteria defined in comppair
//
{ int i, j, t;
  for(i=1; i<=n-1; i++)
     { for (j=i+1; j<=n; j++)
       { switch (colorflag)
	       { case 0: if ( CompareDuecRatAroNome0( array[i], array[j])<0)
                       { t = array[i]; array[i] = array[j]; array[j] = t;}
             break; // no color preference

             case 1: if ( CompareDuecRatAroNome1( array[i], array[j])<0)
                       { t = array[i]; array[i] = array[j]; array[j] = t;}
             break; // white color

             case 2: if ( CompareDuecRatAroNome2( array[i], array[j])<0)
                       { t = array[i]; array[i] = array[j]; array[j] = t;}
             break; // black color
	       }
       }
     }
}

int CompareDuecRatAroNome0(int id1, int id2)
{ // black = white, group equal
  if (  (player[id1].RATING > player[id2].RATING) ) return -1;
  else if ( (player[id1].RATING == player[id2].RATING)
            && (player[id1].aro < player[id2].aro) ) return -1;
  else if ( (player[id1].RATING == player[id2].RATING)
            && (player[id1].aro == player[id2].aro)
	        && (strcmp(player[id1].name, player[id2].name)>0)) return -1;
  else return 1;
}

int CompareDuecRatAroNome1(int id1, int id2)
{ // get a white player
  if (  (player[id1].duecolor < player[id2].duecolor) ) return -1;
  else if ( (player[id1].duecolor==player[id2].duecolor)
          && (player[id1].RATING < player[id2].RATING) ) return -1;
  else if (  (player[id1].duecolor==player[id2].duecolor)
          && (player[id1].RATING == player[id2].RATING)
          && (player[id1].aro < player[id2].aro) ) return -1;
  else if (  (player[id1].duecolor==player[id2].duecolor)
          && (player[id1].RATING == player[id2].RATING)
          && (player[id1].aro == player[id2].aro)
	      && (strcmp(player[id1].name, player[id2].name)>0)) return -1;
  else return 1;
}

int CompareDuecRatAroNome2(int id1, int id2)
{ if ( (player[id1].duecolor > player[id2].duecolor) ) return -1;
  else if (  (player[id1].duecolor==player[id2].duecolor)
          && (player[id1].RATING < player[id2].RATING) ) return -1;
  else if (  (player[id1].duecolor==player[id2].duecolor)
          && (player[id1].RATING == player[id2].RATING)
          && (player[id1].aro < player[id2].aro) ) return -1;
  else if (  (player[id1].duecolor==player[id2].duecolor)
          && (player[id1].RATING == player[id2].RATING)
          && (player[id1].aro == player[id2].aro)
	      && (strcmp(player[id1].name, player[id2].name)>0)) return -1;
  else return 1;
}


/************* compare routine during pairing for colored subgroup **************/



int CompareScoreName(const void *pid1, const void *pid2)
{ // id1>id2 return -1, else return 1
  int *id1, *id2,  tb[FUNC_CMP+1], s[FUNC_CMP+1];
  id1 = (int *) pid1;
  id2 = (int *) pid2;
  int s1, s2;

  s1 = (int) (10 * player[*id1].tiebreak[SCORE]);
  s2 = (int) (10 * player[*id2].tiebreak[SCORE]);
  if (s1 > s2) return -1;

  s[1] = TD.tyebreakorder[1];
  if (s[1]) {
    tb[1] = (*cmpfunc[ s[1] ])(*id1, *id2);
    if ( ( s1 == s2 ) && (tb[1]<0) ) return -1;
  } else return 1;

  s[2] = TD.tyebreakorder[2];
  if (s[2]) {
     tb[2] = (*cmpfunc[ s[2] ])(*id1, *id2);
     if ( ( s1 == s2 ) && (tb[1]==0) && (tb[2]<0) ) return -1;
  } else return 1;

  s[3] = TD.tyebreakorder[3];
  if (s[3]) {
     tb[3] = (*cmpfunc[ s[3] ])(*id1, *id2);
     if ( ( s1 == s2 ) && (tb[1]==0) && (tb[2]==0) && (tb[3]<0)) return -1;
  } else return 1;

  s[4] = TD.tyebreakorder[4];
  if (s[4]) {
     tb[4] = (*cmpfunc[ s[4] ])(*id1, *id2);
     if ( ( s1 == s2 ) && (tb[1]==0) && (tb[2]==0) && (tb[3]==0) && (tb[4]<0) ) return -1;
  } else return 1;

  s[5] = TD.tyebreakorder[5];
  if (s[5]) {
     tb[5] = (*cmpfunc[ s[5] ])(*id1, *id2);
     if ( ( s1 == s2 ) && (tb[1]==0) && (tb[2]==0)
          && (tb[3]==0) && (tb[4]==0) && (tb[5] < 0)) return -1;
  } else return 1;

  s[6] = TD.tyebreakorder[6];
  if (s[6]) {
     tb[6] = (*cmpfunc[ s[6] ])(*id1, *id2);
     if ( ( s1 == s2 ) && (tb[1]==0) && (tb[2]==0)
          && (tb[3]==0) && (tb[4]==0) && (tb[5]==0) && (tb[6] < 0)) return -1;
  } else return 1;

  s[7] = TD.tyebreakorder[7];
  if (s[7]) {
     tb[7] = (*cmpfunc[ s[7] ])(*id1, *id2);
     if ( ( s1 == s2 ) && (tb[1]==0) && (tb[2]==0)
          && (tb[3]==0) && (tb[4]==0) && (tb[5]==0) && (tb[6] == 0) && (tb[7] < 0)) return -1;
  } else return 1;

  s[8] = TD.tyebreakorder[8];
  if (s[8]) {
     tb[8] = (*cmpfunc[ s[8] ])(*id1, *id2);
     if ( ( s1 == s2 ) && (tb[1]==0) && (tb[2]==0)
          && (tb[3]==0) && (tb[4]==0) && (tb[5]==0) &&
          (tb[6] == 0) && (tb[7]==0) &&  (tb[8] < 0)) return -1;
  } else return 1;

  s[9] = TD.tyebreakorder[9];
  if (s[9]) {
     tb[9] = (*cmpfunc[ s[9] ])(*id1, *id2);
     if ( ( s1 == s2 ) && (tb[1]==0) && (tb[2]==0)
          && (tb[3]==0) && (tb[4]==0) && (tb[5]==0) &&
          (tb[6] == 0) && (tb[7]==0) &&
          (tb[8]==0) &&  (tb[9] < 0)) return -1;
  } else return 1;

  return 1;
}


int CompareCategoryScoreName(const void *pid1, const void *pid2)
{ // id1>id2 return -1, else return 1
  int *id1, *id2, tb[FUNC_CMP+1], s[FUNC_CMP+1];

  id1 = (int *) pid1;
  id2 = (int *) pid2;
  int s1, s2;

  if (player[*id1].title > player[*id2].title) return -1;

  s1 = (int) (10 * player[*id1].tiebreak[SCORE]);
  s2 = (int) (10 * player[*id2].tiebreak[SCORE]);
  if ( (player[*id1].title == player[*id2].title) && (s1 > s2)) return -1;

  s[1] = TD.tyebreakorder[1];
  if (s[1]) {
     tb[1] = (*cmpfunc[ s[1] ])(*id1, *id2);
     if ( (player[*id1].title == player[*id2].title) && ( s1 == s2 ) && (tb[1]<0) ) return -1;
  } else return 1;

  s[2] = TD.tyebreakorder[2];
  if (s[2]) {
     tb[2] = (*cmpfunc[ s[2] ])(*id1, *id2);
     if ( (player[*id1].title == player[*id2].title) && ( s1 == s2 ) && (tb[1]==0) && (tb[2]<0) ) return -1;
  } else return 1;

  s[3] = TD.tyebreakorder[3];
  if (s[3]) {
     tb[3] = (*cmpfunc[ s[3] ])(*id1, *id2);
     if ( (player[*id1].title == player[*id2].title) && ( s1 == s2 ) && (tb[1]==0) && (tb[2]==0) && (tb[3]<0)) return -1;
  } else return 1;

  s[4] = TD.tyebreakorder[4];
  if (s[4]) {
     tb[4] = (*cmpfunc[ s[4] ])(*id1, *id2);
     if ( (player[*id1].title == player[*id2].title) && ( s1 == s2 ) && (tb[1]==0) && (tb[2]==0) && (tb[3]==0) && (tb[4]<0) ) return -1;
  } else return 1;

  s[5] = TD.tyebreakorder[5];
  if (s[5]) {
     tb[5] = (*cmpfunc[ s[5] ])(*id1, *id2);
     if ( (player[*id1].title == player[*id2].title) && ( s1 == s2 ) && (tb[1]==0) && (tb[2]==0)
          && (tb[3]==0) && (tb[4]==0) && (tb[5] < 0)) return -1;
  } else return 1;

  s[6] = TD.tyebreakorder[6];
  if (s[6]) {
     tb[6] = (*cmpfunc[ s[6] ])(*id1, *id2);
     if ( (player[*id1].title == player[*id2].title) && ( s1 == s2 ) && (tb[1]==0) && (tb[2]==0)
          && (tb[3]==0) && (tb[4]==0) && (tb[5]==0) && (tb[6] < 0)) return -1;
  } else return 1;

  s[7] = TD.tyebreakorder[7];
  if (s[7]) {
     tb[7] = (*cmpfunc[ s[7] ])(*id1, *id2);
     if ( (player[*id1].title == player[*id2].title) && ( s1 == s2 ) && (tb[1]==0) && (tb[2]==0)
          && (tb[3]==0) && (tb[4]==0) && (tb[5]==0) && (tb[6] == 0) && (tb[7] < 0)) return -1;
  } else return 1;

  s[8] = TD.tyebreakorder[8];
  if (s[8]) {
     tb[8] = (*cmpfunc[ s[8] ])(*id1, *id2);
     if ( ( s1 == s2 ) && (tb[1]==0) && (tb[2]==0)
          && (tb[3]==0) && (tb[4]==0) && (tb[5]==0) &&
          (tb[6] == 0) && (tb[7]==0) &&  (tb[8] < 0)) return -1;
  } else return 1;

  s[9] = TD.tyebreakorder[9];
  if (s[9]) {
     tb[9] = (*cmpfunc[ s[9] ])(*id1, *id2);
     if ( ( s1 == s2 ) && (tb[1]==0) && (tb[2]==0)
          && (tb[3]==0) && (tb[4]==0) && (tb[5]==0) &&
          (tb[6] == 0) && (tb[7]==0) &&
          (tb[8]==0) &&  (tb[9] < 0)) return -1;
  } else return 1;

  return 1;
}


int CompareName(const void *pid1, const void *pid2)
{ // id1>id2 return 1, else return -1
 int *id1, *id2;
 id1 = (int *) pid1;
 id2 = (int *) pid2;
 if (strcmp(player[*id1].name, player[*id2].name)<0) return -1;
  else return 1;
}

int  cmpSB(int id1, int id2)
{
  if (player[id1].tiebreak[SONN_BERG] > player[id2].tiebreak[SONN_BERG] ) return -1;
  else if (player[id1].tiebreak[SONN_BERG] < player[id2].tiebreak[SONN_BERG] ) return 1;
  else return 0;
}

int  cmpBUC1(int id1, int id2)
{
  if (player[id1].tiebreak[BUC_CUT1] > player[id2].tiebreak[BUC_CUT1] ) return -1;
  else if (player[id1].tiebreak[BUC_CUT1] < player[id2].tiebreak[BUC_CUT1] ) return 1;
  else return 0;
}

int  cmpBUCM(int id1, int id2)
{
  if (player[id1].tiebreak[BUC_MED] > player[id2].tiebreak[BUC_MED] ) return -1;
  else if (player[id1].tiebreak[BUC_MED] < player[id2].tiebreak[BUC_MED] ) return 1;
  else return 0;
}

int  cmpBUCT(int id1, int id2)
{
  if (player[id1].tiebreak[BUC_TOT] > player[id2].tiebreak[BUC_TOT] ) return -1;
  else if (player[id1].tiebreak[BUC_TOT] < player[id2].tiebreak[BUC_TOT] ) return 1;
  else return 0;
}

int  cmpCUMUL(int id1, int id2)
{
  if (player[id1].tiebreak[CUMUL] > player[id2].tiebreak[CUMUL] ) return -1;
  else if (player[id1].tiebreak[CUMUL] < player[id2].tiebreak[CUMUL] ) return 1;
  else return 0;
}

int  cmpARO(int id1, int id2)
{
  if (player[id1].tiebreak[ARO] > player[id2].tiebreak[ARO] ) return -1;
  else if (player[id1].tiebreak[ARO] < player[id2].tiebreak[ARO] ) return 1;
  else return 0;
}

int  cmpMB(int id1, int id2)
{
  if (player[id1].tiebreak[MOST_BLACKS] > player[id2].tiebreak[MOST_BLACKS] ) return -1;
  else if (player[id1].tiebreak[MOST_BLACKS] < player[id2].tiebreak[MOST_BLACKS] ) return 1;
  else return 0;
}

int  cmpMWINS(int id1, int id2)
{
  if (player[id1].tiebreak[MOST_WINS] > player[id2].tiebreak[MOST_WINS] ) return -1;
  else if (player[id1].tiebreak[MOST_WINS] < player[id2].tiebreak[MOST_WINS] ) return 1;
  else return 0;
}

int  cmpAPRO(int id1, int id2)
{
  if (player[id1].tiebreak[APRO] > player[id2].tiebreak[APRO] ) return -1;
  else if (player[id1].tiebreak[APRO] < player[id2].tiebreak[APRO] ) return 1;
  else return 0;
}

int  cmpMPSS(int id1, int id2) // missing point score system
{
  if (player[id1].tiebreak[MPSS] > player[id2].tiebreak[MPSS] ) return -1;
  else if (player[id1].tiebreak[MPSS] < player[id2].tiebreak[MPSS] ) return 1;

  if ( player[id1].tiebreak[SCORE] > player[id2].tiebreak[SCORE]) return -1;
  else if ( player[id1].tiebreak[SCORE] < player[id2].tiebreak[SCORE]) return 1;

  else return 0;
}

int CompareCountry(const void *pid1, const void *pid2)
{ // id1>id2 return -1, else return +1
 int *id1, *id2;
 int s1, s2;
 id1 = (int *) pid1;
 id2 = (int *) pid2;

 s1 = (int) (10 * player[*id1].tiebreak[SCORE]);
 s2 = (int) (10 * player[*id2].tiebreak[SCORE]);

 if (strcmp(player[*id1].country, player[*id2].country)<0) return -1;
 else if ( player[*id1].country == player[*id2].country && (s1 > s2)) return -1;
 else return 1;
}

int  cmpdata(int d1, int d2)
{ // d1<d2 return -1, else return 0 if d1=d2, else return +1
 int dd1, mm1, yy1;
 int dd2, mm2, yy2;

 DecodeData(d1, & dd1, & mm1, & yy1);
 if (yy1<15) yy1=2000+yy1;
 else yy1=1900+yy1;

 DecodeData(d2, & dd2, & mm2, & yy2);
 if (yy2<15) yy2=2000+yy2;
 else yy2=1900+yy2;

 if (yy1<yy2) return -1;
 else if ( (yy1==yy2) && (mm1<mm2) ) return -1;
 else if ( (yy1==yy2) && (mm1==mm2)  && (dd1<dd2) ) return -1;
 else if ( (yy1==yy2) && (mm1==mm2)  && (dd1==dd2) ) return 0;
 else return 1;
}

void DecodeData(int code, int *dd, int *mm, int *yy)
//
// PURPOSE: parse the 7 digit integer and estract the players' id and result
//
// INPUT VARIABLE
//	   code     = the number to be parsed
//	   *idwhite = pointer to int for white
//	   *idblack = pointer to int for black
//	   *res	    = pointer to int for result
//
{ int c;
  *yy = code % 100;
  c = (int) (code/100);         // remove last 2 digit

  *mm = (int) (c % 100); // the remainder is mm

  *dd = (int) (c / 100); // remove last 2 digits and obtain dd
}
