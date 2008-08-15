#include "VegaMain.h"
#include "Tournament.h"

int roundfloat( double per)
{
    String str, str2;
    int ldig;
    double p;
    str = NFormat("%6.1f", per); // trasforma in stringa per prenderne la prima cifra decimale
    str2 = str.Mid(5,1); // ("%c", str[5]);
    ldig = StrIntValue(str2);
    str = str.Mid(0,4); //    str[5] = '\0'; //remove last digitltima cifra
    p = StrDblValue(str);  // calcola tot senza la cifra dei decimi
    if (ldig>=5 ) p += 1.; // arrotonda tot
    return (int) p;
}

int rat_flor[] = {2250, 2100, 2050, 1900};
int rat_norm[] = {2601, 2451, 2401, 2251};

int p_dp[] =
{-735,
-677,
-589,
-538,
-501,
-470,
-444,
-422,
-401,
-383,
-366,
-351,
-336,
-322,
-309,
-296,
-284,
-273,
-262,
-251,
-240,
-230,
-220,
-211,
-202,
-193,
-184,
-175,
-166,
-158,
-149,
-141,
-133,
-125,
-117,
-110,
-102,
-95,
-87,
-80,
-72,
-65,
-57,
-50,
-43,
-36,
-29,
-21,
-14,
-7,
0,
7,
14,
21,
29,
36,
43,
50,
57,
65,
72,
80,
87,
95,
102,
110,
117,
125,
133,
141,
149,
158,
166,
175,
184,
193,
202,
211,
220,
230,
240,
251,
262,
273,
284,
296,
309,
322,
336,
351,
366,
383,
401,
422,
444,
470,
501,
538,
589,
677,
735};


double diff[] =
{0.50,
0.50,
0.50,
0.50,
0.51,
0.51,
0.51,
0.51,
0.51,
0.51,
0.51,
0.52,
0.52,
0.52,
0.52,
0.52,
0.52,
0.52,
0.53,
0.53,
0.53,
0.53,
0.53,
0.53,
0.53,
0.53,
0.54,
0.54,
0.54,
0.54,
0.54,
0.54,
0.54,
0.55,
0.55,
0.55,
0.55,
0.55,
0.55,
0.55,
0.56,
0.56,
0.56,
0.56,
0.56,
0.56,
0.56,
0.57,
0.57,
0.57,
0.57,
0.57,
0.57,
0.57,
0.58,
0.58,
0.58,
0.58,
0.58,
0.58,
0.58,
0.58,
0.59,
0.59,
0.59,
0.59,
0.59,
0.59,
0.59,
0.60,
0.60,
0.60,
0.60,
0.60,
0.60,
0.60,
0.60,
0.61,
0.61,
0.61,
0.61,
0.61,
0.61,
0.61,
0.62,
0.62,
0.62,
0.62,
0.62,
0.62,
0.62,
0.62,
0.63,
0.63,
0.63,
0.63,
0.63,
0.63,
0.63,
0.64,
0.64,
0.64,
0.64,
0.64,
0.64,
0.64,
0.64,
0.65,
0.65,
0.65,
0.65,
0.65,
0.65,
0.65,
0.66,
0.66,
0.66,
0.66,
0.66,
0.66,
0.66,
0.66,
0.67,
0.67,
0.67,
0.67,
0.67,
0.67,
0.67,
0.67,
0.68,
0.68,
0.68,
0.68,
0.68,
0.68,
0.68,
0.68,
0.69,
0.69,
0.69,
0.69,
0.69,
0.69,
0.69,
0.69,
0.70,
0.70,
0.70,
0.70,
0.70,
0.70,
0.70,
0.70,
0.71,
0.71,
0.71,
0.71,
0.71,
0.71,
0.71,
0.71,
0.71,
0.72,
0.72,
0.72,
0.72,
0.72,
0.72,
0.72,
0.72,
0.73,
0.73,
0.73,
0.73,
0.73,
0.73,
0.73,
0.73,
0.73,
0.74,
0.74,
0.74,
0.74,
0.74,
0.74,
0.74,
0.74,
0.74,
0.75,
0.75,
0.75,
0.75,
0.75,
0.75,
0.75,
0.75,
0.75,
0.76,
0.76,
0.76,
0.76,
0.76,
0.76,
0.76,
0.76,
0.76,
0.77,
0.77,
0.77,
0.77,
0.77,
0.77,
0.77,
0.77,
0.77,
0.78,
0.78,
0.78,
0.78,
0.78,
0.78,
0.78,
0.78,
0.78,
0.78,
0.79,
0.79,
0.79,
0.79,
0.79,
0.79,
0.79,
0.79,
0.79,
0.79,
0.80,
0.80,
0.80,
0.80,
0.80,
0.80,
0.80,
0.80,
0.80,
0.80,
0.81,
0.81,
0.81,
0.81,
0.81,
0.81,
0.81,
0.81,
0.81,
0.81,
0.81,
0.82,
0.82,
0.82,
0.82,
0.82,
0.82,
0.82,
0.82,
0.82,
0.82,
0.82,
0.83,
0.83,
0.83,
0.83,
0.83,
0.83,
0.83,
0.83,
0.83,
0.83,
0.83,
0.84,
0.84,
0.84,
0.84,
0.84,
0.84,
0.84,
0.84,
0.84,
0.84,
0.84,
0.84,
0.85,
0.85,
0.85,
0.85,
0.85,
0.85,
0.85,
0.85,
0.85,
0.85,
0.85,
0.85,
0.86,
0.86,
0.86,
0.86,
0.86,
0.86,
0.86,
0.86,
0.86,
0.86,
0.86,
0.86,
0.86,
0.87,
0.87,
0.87,
0.87,
0.87,
0.87,
0.87,
0.87,
0.87,
0.87,
0.87,
0.87,
0.87,
0.88,
0.88,
0.88,
0.88,
0.88,
0.88,
0.88,
0.88,
0.88,
0.88,
0.88,
0.88,
0.88,
0.88,
0.88,
0.88,
0.89,
0.89,
0.89,
0.89,
0.89,
0.89,
0.89,
0.89,
0.89,
0.89,
0.89,
0.89,
0.89,
0.90,
0.90,
0.90,
0.90,
0.90,
0.90,
0.90,
0.90,
0.90,
0.90,
0.90,
0.90,
0.90,
0.90,
0.90,
0.90,
0.90,
0.91,
0.91,
0.91,
0.91,
0.91,
0.91,
0.91,
0.91,
0.91,
0.91,
0.91,
0.91,
0.91,
0.91,
0.91,
0.91,
0.91,
0.92,
0.92,
0.92,
0.92,
0.92,
0.92,
0.92,
0.92,
0.92,
0.92,
0.92,
0.92,
0.92,
0.92,
0.92,
0.92,
0.92,
0.92,
0.92,
0.92,
0.93,
0.93,
0.93,
0.93,
0.93,
0.93,
0.93,
0.93,
0.93,
0.93,
0.93,
0.93,
0.93,
0.93,
0.93,
0.93,
0.93,
0.93,
0.93,
0.93,
0.93,
0.94,
0.94,
0.94,
0.94,
0.94,
0.94,
0.94,
0.94,
0.94,
0.94,
0.94,
0.94,
0.94,
0.94,
0.94,
0.94,
0.94,
0.94,
0.94,
0.94,
0.94,
0.94,
0.94,
0.94,
0.95,
0.95,
0.95,
0.95,
0.95,
0.95,
0.95,
0.95,
0.95,
0.95,
0.95,
0.95,
0.95,
0.95,
0.95,
0.95,
0.95,
0.95,
0.95,
0.95,
0.95,
0.95,
0.95,
0.95,
0.95,
0.95,
0.95,
0.95,
0.96,
0.96,
0.96,
0.96,
0.96,
0.96,
0.96,
0.96,
0.96,
0.96,
0.96,
0.96,
0.96,
0.96,
0.96,
0.96,
0.96,
0.96,
0.96,
0.96,
0.96,
0.96,
0.96,
0.96,
0.96,
0.96,
0.96,
0.96,
0.96,
0.96,
0.96,
0.96,
0.96,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.97,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.98,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
0.99,
1.00};

void RoundData::calcelo (int idp, double arrp[][2], int round)
{ int d, k;
  int idopp;

  for(k=1; k<=round; k++)
     {  //the rating is not calculated for game finished with forfait, code 3, 4, 2, and 6
        if (IS_SWISS) {
	       idopp = playerOpponent[idp][k];
           if ( idopp>0 && roundResult[idp][k]<3 )  // no forfeit
             {
               d = abs(player[idp].RATING - player[idopp].RATING);
	           if ( d > 350) d = 350;
	           arrp[k][0]= (float) d;
	           if (player[idp].RATING >= player[idopp].RATING) arrp[k][1] = diff[ d ];
	           else arrp[k][1] = 1.0 - diff[ d ];
	         }
           else {
            arrp[k][0] = 0.; // rating difference
            arrp[k][1] = 0.; // expected result
           }
	   }
	else {
	   idopp = playerOpponentRR[idp][k];
       if ( idopp>0 && roundResultRR[idp][k]<3 )
          {
           d = abs(player[idp].RATING - player[idopp].RATING);
	       if ( d > 350) d = 350;
	       arrp[k][0]= (float) d;
	       if (player[idp].RATING >= player[idopp].RATING) arrp[k][1] = diff[ d ];
	       else arrp[k][1] = 1.0 - diff[ d ];
	      }
           else {
            arrp[k][0] = 0.; // rating difference
            arrp[k][1] = 0.; // expected result
           }
    }
  }
}




void RoundData::EloForesee(int nplayers, int round)
{ int i, j, index, idopp, idopp_old, var, npair;
  int code, idw, idb, res;
  int plopp[N_PLAYER_MAX+1];
  String str, str2, stringa;
  double arrp[N_ROUND_MAX_RR+2][2];
  double totscore, kpoint, expected, tot2, previousExp;
  int ldig, d;
  int *roundRes;
  int *playerOpp;

  if (IS_SWISS) {
     playerOpp = &playerOpponent[0][0];
     roundRes = &roundResult[0][0];
  }
  else {
     playerOpp = &playerOpponentRR[0][0];
     roundRes = &roundResultRR[0][0];
   }
  FileOut saveelo(PATHDIR + "eloprev.txt");
  saveelo<<NAMETORNEO << " - "  << PLACETORNEO << ", " << "\n" << "\n";
  saveelo<<"Elo preview at round: " << round << "\n";

  saveelo << "\n  ID  NAME                   Elo    K    if -   if =   if +\n";
  saveelo <<"-----------------------------------------------------------\n";

  if (IS_SWISS) npair = pairing[0][round];
  else npair = pairingRR[0][round];

  for (i=1; i<=npair; i++)
      { if (IS_SWISS) code = pairing[i][round];
        else code = pairingRR[i][round];
        DecodeResult(code, &idw, &idb, &res);
        plopp[idw] = idb;
        plopp[idb] = idw;
      }

  for (i=1; i<=nplayers; i++)
  { idopp = plopp[i];
    if ( idopp!=0 && player[i].isAvailable=="1" && player[i].kcoeff>0) {   // not forfait
         previousExp = totscore = 0.;
	     calcelo(i, arrp, round-1);
         for(j=1; j<=round-1; j++)
	     { if (IS_SWISS) index = i*(N_ROUND_MAX+1) + j;
	       else index = i*(N_ROUND_MAX_RR+1) + j;

	       idopp_old = playerOpp[index];
           if ( idopp_old!=0 && roundRes[index]<3  ) {   // not forfait
	          previousExp += arrp[j][1];
              if (roundRes[index]==1 || roundRes[index]==3) totscore += 1.0;
	          else if (roundRes[index]==2 || roundRes[index]==6) totscore += 0.5;
 	      }
	     }

         d = abs(player[i].RATING - player[idopp].RATING);
         if ( d > 350) d = 350;
         if (player[i].RATING >= player[idopp].RATING) expected = diff[ d ];
         else expected = 1.0 - diff[ d ];

         saveelo<<NFormat( "%4d  %-20.20s  %4d  %3d  ", i, player[i].name, player[i].RATING, player[i].kcoeff);
         kpoint = 0.;
	     while (kpoint<=1.)
         {
         str = NFormat("%5.2f", previousExp + expected); // trasforma tot in stringa per prenderne l'ultima cifra
         str2 = str.Mid(4,1); //sprintf(str2, "%c", str[4]);
         ldig = StrIntValue(str2);
	     str = str.Mid(0,4); //toglie l'ultima cifra
         tot2 = StrDblValue(str);  // calcola tot senza la cifra dei centisimi
         if (ldig>5 && ldig<=9) tot2 += 0.1; // arrotonda tot
          //  var = (int) rintf (player[i].kcoeff * (kpoint + totscore - tot2)) ;
           var = fround (player[i].kcoeff * (kpoint + totscore - tot2)) ;
           saveelo << NFormat(" %5d ", var);
           kpoint += 0.5;
	     }
         saveelo<<"\n";
    }
  }
  saveelo <<"\n Generated by Vega - www.vegachess.com\n";
  saveelo.Close();
}

void RoundData::CalcVarElo(int nplayers)
{ int i, j, idopp, index;
  double arrp[N_ROUND_MAX_RR+2][2];
  double tot, tot2, totscore;
  int *roundRes;
  int *playerOpp;
  int  ldig;
  String str, str2;

  if (IS_SWISS) {
     playerOpp = &playerOpponent[0][0];
     roundRes = &roundResult[0][0];
    // playerCol = &playerColor[0][0];
  }
  else {
     playerOpp = &playerOpponentRR[0][0];
     roundRes = &roundResultRR[0][0];
    // playerCol = &playerColorRR[0][0];
  }

  for (i=1; i<=nplayers; i++)
  { tot=0.;
    totscore=0.; //score without to consider the forfait
	calcelo(i, arrp, currentRound);
	for(j=1; j<=currentRound; j++)
	   { if (IS_SWISS) index = i*(N_ROUND_MAX+1) + j;
	     else index = i*(N_ROUND_MAX_RR+1) + j;

	     idopp = playerOpp[index];
         if ( idopp!=0 && roundRes[index]<3  ) {   // not forfait
	         tot += arrp[j][1];
             if (roundRes[index]==1 || roundRes[index]==3) totscore += 1.0;
	         else if (roundRes[index]==2 || roundRes[index]==6) totscore += 0.5;
 	      }
	   }
    str = NFormat("%5.2f", tot); // trasforma tot in stringa per prenderne l'ultima cifra
// Exclamation(str);
    str2=str.Mid(4,1);
// Exclamation(str2);
    ldig = StrIntValue(str2);
    str = str.Mid(0,4); //toglie l'ultima cifra
// Exclamation(str);
    tot2 = StrDblValue(str);  // calcola tot senza la cifra dei centisimi
    if (ldig>5 && ldig<=9) tot2 += 0.1; // arrotonda tot
    // player[i].tiebreak[ELO_VAR] = (float) rintf(player[i].kcoeff * (totscore - tot2)) ;
    player[i].tiebreak[ELO_VAR] = (float) fround(player[i].kcoeff * (totscore - tot2)) ;
  }
}


void RoundData::SaveFSIRatingReport()
//
// This file contains part of the *.veg and part of *.rnd file
//
{ int i, j, k, index, id, code[2];
  int var, aro, nopp, navv;
  int arr[N_PLAYER_MAX+1], id_to_pos[N_PLAYER_MAX+1];
  String stringa, str, col;
  String master = "Norma Maestro FSI: & ";
  float score, totscore, puntiplayerfide;
  int *roundRes;
  int *playerOpp, *playerCol, idopp;
  extern int  CompareScoreName(const void *pid1, const void *pid2);

  if(NPlayer<1) return;

  if (TD.IS_SWISS) {
     playerOpp = &playerOpponent[0][0];
     roundRes = &roundResult[0][0];
     playerCol = &playerColor[0][0];
  }
  else {
     playerOpp = &playerOpponentRR[0][0];
     roundRes = &roundResultRR[0][0];
     playerCol = &playerColorRR[0][0];
  }

  // avoid negative rating variation to the winners
  for (i=0; i<NPlayer; i++)
      { arr[i] = i+1; // set the id of each player in the array to be sort
      }
  qsort(arr, NPlayer, sizeof(int), CompareScoreName);

  for (i=0; i<TD.NPlayer; i++) // 'i' is the ranking position after we performed the sort
      { id = arr[i];
        id_to_pos[id] = i;  // new id after the sort
      }

  k = 0;
  score = player[ arr[k] ].tiebreak[SCORE];
  if (player[ arr[k] ].tiebreak[ELO_VAR] < 0.) player[ arr[k] ].tiebreak[ELO_VAR] = 0.;

  k=1;
  while (player[ arr[0] ].tiebreak[SCORE] == player[ arr[k] ].tiebreak[SCORE])
      { if (player[ arr[k] ].tiebreak[ELO_VAR] < 0.) player[ arr[k] ].tiebreak[ELO_VAR] = 0.;
        k++;
      }
  FileOut out(PATHDIR + FILETORNEO + ".elo");
  out << NFormat( "%-30.30s\n", NAMETORNEO);
  out << NFormat( "%-30.30s\n", PLACETORNEO);
  out << NFormat( "%s,%s\n", DATATORNEO_B, DATATORNEO_E);
  out << NFormat( "%-30.30s\n", ARBITER);
  out << NFormat( "%d\n", currentRound);
  out << NFormat( "%d\n", NPlayer);

  for (i=1; i<=NPlayer; i++)
      { // headers
        out << NFormat( "%7d,", player[i].idnat);
        out << NFormat( "%8d,", player[i].idfide);
        out << NFormat(  "%-25.25s,", player[i].name);
        out << NFormat(  "%-3.3s,", player[i].codetitle);
        out << NFormat(  "%-3.3s,", player[i].country);
        out << NFormat(  "%-8.8s,", player[i].data);
        out << NFormat(  "%4d,", player[i].ratingnat);
        out << NFormat( "%2d,", player[i].kcoeff);
        out << NFormat( "%4d,", player[i].ratingfide); // elo FIDE
        stringa = "";
        //tot=0.;
        aro = 0;
        nopp = 0;
        navv = 0;
        totscore=0.; // score without to consider the forfeit
        puntiplayerfide = 0.;

	    //calcelo(i, arrp, currentRound);
        // table players
        for (j=1; j<=currentRound; j++)
            { if (TD.IS_SWISS) index = i*(N_ROUND_MAX+1) + j;
	          else index = i*(N_ROUND_MAX_RR+1) + j;
              if (playerCol[index]==1) col = "b";
              else if (playerCol[index]==-1) col = "n";
              else col = "-";

              TestResult(roundRes[index], code);
              // string result
              idopp = playerOpp[index];
              str = NFormat("%d%03d%s%d", code[0], idopp, col, code[1]);
              stringa << str;

              if (code[0] && idopp!=0) { // game valid
                 //tot += arrp[j][1];
                 navv++;
                 if (player[idopp].ratingfide>0) {
                     aro += player[idopp].RATING;
                     nopp++;
                     if (roundRes[index]==1) puntiplayerfide += 1.0;
	                 else if (roundRes[index]==2) puntiplayerfide += 0.5;
                 }
                 if (roundRes[index]==1) totscore += 1.0;
	             else if (roundRes[index]==2) totscore += 0.5;
               }
            }
        if (nopp!=0) aro = aro/nopp; else aro = 0;
        var = (int) player[i].tiebreak[ELO_VAR];
        out << NFormat("%4d,", var);
        out << NFormat("%2d,", nopp);
        if (player[i].kcoeff==0) out << NFormat("%4d,", aro);
        else if (player[i].kcoeff>0 && nopp>=3) out << NFormat("%4d,", aro);
        else out << "   0,";
        out << NFormat("%4.1f,", puntiplayerfide);
        out << NFormat("%4d,", id_to_pos[ i ] + 1); // posizione
        out << NFormat("%s\n", stringa);

        //Norma maestro FSI
        if ( currentRound>=9 && navv>=8 && player[i].title<=9 && player[i].tiebreak[PER_RAT]>2250 )
           master<<NFormat("%3d %s", i, player[i].name)<<" & ";
      }
  out.Close();
  if (master.GetLength()>=25) Exclamation(master);
}

void RoundData::TestResult(int res, int code[])
{ // code[0] = valid, 1 yes, 0 not
  // code[1] = result, 1 win. 2 lost, 3 draw
  switch (res)
  { // lost
    case 0: code[0] = 1;
            code[1] = 2;
            break;
    // win
    case 1: code[0] = 1;
            code[1] = 1;
            break;
    // draw
    case 2: code[0] = 1;
            code[1] = 3;
            break;
    // lost forfeit
    case 4: code[0] = 0;
            code[1] = 2;
            break;
    // win forfeit
    case 3: code[0] = 0;
            code[1] = 1;
            break;
    // draw forfeit
    case 6: code[0] = 0;
            code[1] = 3;
            break;
  }
}
