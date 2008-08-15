#include "VegaMain.h"
#include "Tournament.h"

extern int  CompareScoreName(const void *pid1, const void *pid2);

void RoundData::PrepareRRPairing(int nplayer)
//
// fill the pairingRR[][] array
//
{ int i, round, npairs, nr, code;
  int idwhite, idblack, result;

  //  GenerateRR( nplayer, pairingRR);
  if (PLAY_SYSTEM == ROUND_ROBIN) {
     GenerateRRCalendar( nplayer, pairingRR);
  }
  else if (PLAY_SYSTEM == ROUND_ROBIN2) {
    nr = GenerateRRCalendar( nplayer, pairingRR);
    for (round=nr+1; round<=2*nr; round++)  // adesso ripete scambiando i colori
      { npairs = pairingRR[0][round] = pairingRR[0][round-nr] ;
        for (i=1; i<=npairs; i++)
            { code = pairingRR[i][round - nr];
              DecodeResult(code, &idwhite, &idblack, &result);
              pairingRR[i][round] = CodifyResult( idblack, idwhite, result);
            }
      }
  }
}


int RoundData::GenerateRRCalendar( int nplayer, int pairs[][N_ROUND_MAX_RR+1])
//
// purpose: generate the round robin calendar
//
// nplayer = number of players
// pairs[round][] = array containing the calendar
//
{ int arr[N_ROUND_MAX_RR+1];          // contains 0 (busy) and 1 (free) relative to each player
  int ID_last_black=1;  // who played with black in the previous round
  int i, j, np2, rounds;
  int count, color;
  int pairing[2][N_ROUND_MAX_RR/2 + 2 ];
  int odd=0, bye;

  if (nplayer&1) {
     odd = 1;
     nplayer++;
     np2=nplayer/2;
     rounds=nplayer-1;
  }
  else {
     np2=nplayer/2;
     rounds=nplayer-1;
  }
  arr[nplayer] = 0; // player "nplayer" will be always busy
  for (i=1; i<=rounds; i++)
  { for (j=1; j<=nplayer-1; j++) arr[j] = 1;  //free all the player except "nplayer"
    color = i & 1;
    pairing[color][1] = nplayer;      //set the first couple with their color
    pairing[ !color ][1] = ID_last_black;
    arr[ID_last_black] = 0;   // ID_last_black is set to busy

	j = ID_last_black;
	count = 1;

	while (count<np2) {  // look for white free player: from left to right
	   if (j>=nplayer) j=1; // restart the cycle
	   if (arr[j]) {
	       count++;
	       pairing[0][count] = j;  // set the white
               arr[j] = 0;      // set to busy
	       j++;
	   } else j++;
	}
	count = np2+1;
	while (count>2) {  // look for black free player: from right to left
	   if (j>=nplayer) j=1; // restart the cycle
	   if (arr[j]) {
	       count--;
	       pairing[1][count] = j;  // set the white
           arr[j] = 0;      // set to busy
	       j++;
	   } else j++;
	}
	ID_last_black = pairing[1][np2];

	pairs[0][i] = np2;
	count = 1;
	for (j=1; j<=np2; j++) {
	  if ( !odd || (odd && pairing[0][j]!=nplayer && pairing[1][j]!=nplayer) ) {
	       pairs[count][i] = pairing[0][j]*10000 + pairing[1][j]*10 + 9;
	       count++;
	     }
	  else if ( odd && pairing[0][j]==nplayer )
	     //bye = pairing[1][j]*10000 + 0 + 2; // set the BYE  //NEW
	     bye = pairing[1][j]*10000 + 0 + 8; // set the BYE
	  else if ( odd && pairing[1][j]==nplayer )
	     //bye = pairing[0][j]*10000 + 0 + 2; // set the BYE
	     bye = pairing[0][j]*10000 + 0 + 8; // set the BYE	//NEW
	}
	if ( odd ) pairs[np2][i] = bye;
  }
  return rounds;
}



void RoundData::SaveRoundRR(int flag)
// flag = 0, save the file .rnd
// flag > 0, save the backup file at round "round"
{ int i, j, npair, nr;
  String filename;
  if (flag==0) filename = PATHDIR + FILETORNEO + ".rnd";
  else filename = TD.PATHDIR + FILETORNEO + AsString(TD.currentRound) + ".rnd";
  FileOut out(filename);
  String s;
  int round = currentRound;

  out.PutLine( NFormat("%2d  # current round", currentRound ) );
  out.PutLine( " 1  # not used" );
  out.PutLine( " 1  # not used" );
  out.PutLine( " 1  # not used" );
//  out.PutLine( NFormat("%2d  # IS_GET_RESULT_DONE", IS_GET_RESULT_DONE) );
  out.PutLine( NFormat("%2d  # STATUS_ROUND", STATUS_ROUND) );
  out.PutLine( " 1  # not used" );
  out.PutLine( " 1  # not used" );

  if (currentRound>0) {
    out.PutLine( "# players color" );
    for (i=1; i<=NPlayer; i++)
      { String s = NFormat("%3d  ", i);
        for (j=1; j<=round; j++)
            {  s << NFormat("%3d ", playerColorRR[i][j]);
	        }
        out.PutLine( s );
      }

    out.PutLine("# players opponent" );
    for (i=1; i<=NPlayer; i++)
      { String s = NFormat("%3d  ", i);
        for (j=1; j<=round; j++) s << NFormat("%3d ", playerOpponentRR[i][j]);
        out.PutLine( s );
      }

    out.PutLine( "# round results");
    for (i=1; i<=NPlayer; i++)
      { String s = NFormat("%3d  ", i);
        for (j=1; j<=round; j++) s << NFormat("%d ", roundResultRR[i][j]);
        out.PutLine( s );
      }
  }
  out.PutLine( "# code pairing");
  if (NPlayer & 1) { npair = (NPlayer-1)/2 + 1; nr = NPlayer;}
  else { npair = NPlayer/2; nr = NPlayer-1;}
  if (PLAY_SYSTEM == ROUND_ROBIN2) nr = 2*nr;
  for (i=0; i<=npair; i++)
      { String s = "";
	    for (j=1; j<=nr; j++) s << NFormat("%7d ", pairingRR[i][j]);
        out.PutLine( s );
      }
  out.Close();
}

void  RoundData::LoadRoundRR(String filename)
{ FileIn in(filename);
  int i, j, nr, npair, numPlayers = TD.NPlayer;// NPlayer is known from the *.veg file
 /*  if ((in=fopen(filename, "r"))==NULL)
     {  WarningMessage(sss[S_NOREADR], sss[S_CLOSE]);
        return;
     }
*/
  TD.currentRound = StrIntValue( in.GetLine() );
  in.GetLine(); //skip this line
  in.GetLine(); //skip this line
  in.GetLine(); //skip this line
  STATUS_ROUND = StrIntValue( in.GetLine() );
  in.GetLine(); //skip this line
  in.GetLine(); //skip this line

  if (currentRound>0) {
     in.GetLine(); //skip this line // "# players color\n"
     for (i=1; i<=numPlayers; i++)
      { Vector<String> field = Split(in.GetLine(), ' ');
        //field.GetCount() coincide with currentRound
        for (j=1; j<field.GetCount(); j++)  playerColorRR[i][j] = StrIntValue( field[j] );
      }

     in.GetLine(); //skip this line // "# players opponent\n"
     for (i=1; i<=numPlayers; i++)
      { Vector<String> field = Split(in.GetLine(), ' ');
        //field.GetCount() coincide with currentRound
        for (j=1; j<field.GetCount(); j++)  playerOpponentRR[i][j] = StrIntValue( field[j] );
      }

     in.GetLine(); //skip this line // "# players result\n"
     for (i=1; i<=numPlayers; i++)
      { Vector<String> field = Split(in.GetLine(), ' ');
        //field.GetCount() coincide with currentRound
        for (j=1; j<field.GetCount(); j++)  roundResultRR[i][j] = StrIntValue( field[j] );
      }
  }
  // now reads the pairing
  in.GetLine(); //skip this line // "# code pairing\n"
  if (numPlayers & 1) { npair = (numPlayers-1)/2 + 1; nr = numPlayers;}
  else { npair = numPlayers/2;  nr = numPlayers-1;}
  if (PLAY_SYSTEM == ROUND_ROBIN2) nr = 2*nr;
  for (i=0; i<=npair; i++)
      { Vector<String> field = Split(in.GetLine(), ' ');
        for (j=1; j<=field.GetCount(); j++) pairingRR[i][j] = StrIntValue( field[j-1] );
      }
  in.Close();
}


void RoundData::AdjournCrossTableRR(int round)
//
// NOTE: it is called just after the iserting of the result of the round,
// so at round N, the state refer to the previous N-1 rounds
//
{ int id, j, np;
  int  jopp;
  double score, aro, buc, min, max;
  double sb, modifiedscore[N_PLAYER_MAX+1];

  for (id=1; id<=NPlayer; id++)
  {	//set ARO
	aro = modifiedscore[id] = 0.;
	np = 0;
	for (j=1; j<=round; j++)
	    { jopp = TD.playerOpponentRR[id][j];
	      if (jopp != 0 &&  (TD.roundResultRR[id][j]<3)) // no BYE o game won for forfait
	         { aro += (float) player[jopp].RATING;
               np++;
             }
        }
	if (np != 0) player[id].tiebreak[ARO] = (float) (aro/np);
	else player[id].tiebreak[ARO] = 0.;

    //set score
	score = 0.;
	for (j=1; j<=round; j++) {
        if (TD.roundResultRR[id][j]==1)
            { score += WON;
              modifiedscore[id] += WON;
            }
        else if (TD.roundResultRR[id][j]==3) // win forfeit
            { score += WON;
              modifiedscore[id] += DRAW;
            }
        else if (TD.roundResultRR[id][j]==4) // lost forfeit
            {
              modifiedscore[id] += DRAW;
            }
       else if (TD.roundResultRR[id][j]==5) // null game
            {
              modifiedscore[id] += 0.; //do nothing
            }
        else if (TD.roundResultRR[id][j]==2 || TD.roundResultRR[id][j]==6)
            { score += DRAW;
              modifiedscore[id] += DRAW;
            }
	}
    player[id].tiebreak[SCORE] = (float) score;

    //Exclamation(AsString(id) + " " + AsString(player[id].tiebreak[SCORE]) + " "+ AsString(modifiedscore[id]) );
  }

  for (id=1; id<=NPlayer; id++)
  {	//set Buccholz
	buc = 0.;
	min = 10000.;
	max = 0.;
	for (j=1; j<=round; j++)
	    { jopp = TD.playerOpponentRR[id][j];  //BUG! was TD.playerOpponent
          if ( (jopp != 0) && (TD.roundResultRR[id][j]<3) ) // no BYE, no forfait
	         { buc += modifiedscore[jopp];

	           if (modifiedscore[jopp]<min) min = modifiedscore[jopp];
	           if (modifiedscore[jopp]>max) max = modifiedscore[jopp];
	         }
          else if ( (jopp == 0) && (TD.roundResultRR[id][j]==5) ) // odd number
	         { buc += 0.; //do nothing
	         }
          else // BYE or forfait
	         { buc += modifiedscore[id];  // sum his score to the bucholz

	           if (modifiedscore[id]<min) min = modifiedscore[id];
	           if (modifiedscore[id]>max) max = modifiedscore[id];
	         }
	    }
	if (buc > min) player[id].tiebreak[BUC_CUT1] = (float) (buc-min);
	else player[id].tiebreak[BUC_CUT1] = 0.;

    if (buc - min > max ) player[id].tiebreak[BUC_MED] = (float) (buc - min - max);
    else player[id].tiebreak[BUC_MED] = 0.;

	player[id].tiebreak[BUC_TOT] = (float) buc;
  }

  for (id=1; id<=NPlayer; id++)
  {	//set Sonneborg-Berger
	sb = 0.;
	for (j=1; j<=round; j++)
	    { jopp = TD.playerOpponentRR[id][j];
		  if ( jopp != 0 && TD.roundResultRR[id][j]==1  ) // win, no forfait
	          sb += modifiedscore[jopp];
          else if ( (jopp != 0) && (TD.roundResultRR[id][j]==2)  ) // draw, no forfait
	          sb += modifiedscore[jopp]/2.;
          else if ( (jopp == 0) || (TD.roundResultRR[id][j]==5) )
              sb += 0.;  // do nothing
          else if ( ((jopp == 0) || (TD.roundResultRR[id][j]>2)) && (TD.roundResult[id][j] != 8) )
              sb += modifiedscore[id]/2.;  // sum his half score to the bucholz
	    }
        player[id].tiebreak[SONN_BERG] = (float) sb;
  }
// adjourn the rest of cross table
    CalcVarElo(NPlayer);
//    PrepareHTMLindex();
    NationalPlayerCard(NPlayer);  // here is calculated the performance rating
    setAPRO(round, NPlayer);
    ShowTableScore(NPlayer);
    CrossTableRoundRobin(NPlayer);
    ShowTableCategoryScore(NPlayer);
}


void RoundData::CrossTableRoundRobin(int nplayers)
//
// PURPOSE: sort the cross table according the ranking
//
// INPUT VARIABLE
//	nplayers = number of players to be sorted
//
{ int i, k, j, nr, iw, ib, code, res, npairs, id1, id2, jround;
  int arr[N_PLAYER_MAX_RR+2];
  String stringa, mm[N_PLAYER_MAX_RR+1][N_PLAYER_MAX_RR+1];
  String str1, str2, str3, remark = "\nRemark:\n";

  FileOut save(PATHDIR + "crosstbl.txt");
  nr = currentRound;
  if (PLAY_SYSTEM == ROUND_ROBIN2)
       { str1 = "-- ";
         str2 = "- ";
         str3 = " ";
       }
  else { str1 = "-  ";
         str2 = "  ";
         str3 = "  ";
       }

  for (i=0; i<nplayers; i++)
      { arr[i] = i+1; // set the id of each player in the array to be sort
      }
  qsort(arr, nplayers, sizeof(int), CompareScoreName);

  for (i=0; i<nplayers; i++)
      for (j=0; j<nplayers; j++) mm[i][j] = " ";

  for (k=0; k<nplayers; k++)
      { id1=arr[k];
        for(j=0; j<nplayers; j++)
           { id2=arr[j];
             for(jround=1; jround<=nr; jround++)
 	            { if (id2 == playerOpponentRR[id1][jround])
 	                 { if (roundResultRR[id1][jround]==0 || roundResultRR[id1][jround]==4)
                          { mm[k][j] << "0";
                            // mm[j][k] << "1";
                          }
                       else if (roundResultRR[id1][jround]==1 || roundResultRR[id1][jround]==3)
                          { mm[k][j] << "1";
                            //mm[j][k] << "0";
                          }
                       else if (roundResultRR[id1][jround]==2 || roundResultRR[id1][jround]==6 || roundResultRR[id1][jround]==8)
                          { mm[k][j] << "½";
                            //mm[j][k] << "½";
                          }
                     }
                }
         }
      }
  save<< NAMETORNEO << "\n";
  save<< PLACETORNEO << " - " <<  DATATORNEO_B <<", "<<DATATORNEO_E  << "\n\n";
  save<< t_(" Cross Table");
  save<< "\n\n N  NAME                 Elo  Tit Fed  score |";
  for(j=1; j<=nplayers; j++)  save << NFormat("%3d ", j);
  save << "\n----------------------------------------------";
  for(j=1; j<=nplayers; j++) save << "----";
  save << "\n";

  for (k=0; k<nplayers; k++)
      { i=arr[k];
        save << NFormat( "%3d %-20.20s %4d %3.3s %3.3s %5.1f  | ",
	       k+1, // k is the ranking position now
		   player[i].name,
           player[i].RATING,
		   player[i].codetitle,
		   player[i].country,
		   player[i].tiebreak[SCORE]);
        for(j=0; j<nplayers; j++)
           { if (j==k)  save<< " *  ";
             else if (strlen(mm[k][j]) == 1) save<<mm[k][j]<<str1;
             else if (strlen(mm[k][j]) == 2) save<<mm[k][j]<<str2;
             else save<<mm[k][j]<<str3;
           }
        save<<"\n";
      }

  //add notes to the cross table
  for(jround=1; jround<=currentRound; jround++) {
    npairs = pairingRR[0][jround];
    for (i=1; i<=npairs; i++)
      { code = pairingRR[i][jround];
        DecodeResult(code, &iw, &ib, &res);
        if ( (res!=0 && res!=1 && res!=5) && iw!=0 && ib!=0 )
          remark << NFormat("round %2d: %-20.20s - %-20.20s = %s\n",
                        jround, player[iw].name, player[ib].name,  LabelResult(res, code, jround) );
      }
  }
  save << remark;

  save<<"\n Generated by Vega - www.vegachess.com\n";
  save.Close();


  // produce HTML files
  FileOut savehtml(PATHDIRWWW + "crosstbl.html");
  savehtml << TD.HTMLCommonHeader();
  savehtml << HTMLNavBar();
  savehtml << "<h2>" << NAMETORNEO << "</h2>\n";
  savehtml << "<h2>" << PLACETORNEO << " - " <<  DATATORNEO_B <<", "<<DATATORNEO_E  << "</h2>\n";
  savehtml << "<h3> " << t_(" Cross Table at round ")<< currentRound<< "</h3>\n";
  savehtml << "<table bgcolor=#efefef cellpadding=4 cellspacing=0 border=0> \n";
  savehtml << "<tbody>\n";
  savehtml << "<tr>";
  savehtml << "<th> N </th> <th width=\"180\">NAME</th> <th>Rat</th> <th>PRat</th> <th>Fed</th> <th>Scr</th>\n";

  for(j=1; j<=nplayers; j++) savehtml << "<th> " << j << " </th>";
  savehtml << "</tr> \n";

  for (k=0; k<nplayers; k++)
      { i=arr[k];
        stringa = NFormat("<td>%3d</td> <td> <a href=\"playercard.html#%d\"> %s %s </a> </td> <td>%4d</td> <td bgcolor=\"#EEEEEE\">%4d</td> <td>%3.3s</td> <td>%4.1f</td>",
	       k+1, // k is the ranking position now
		   i, player[i].codetitle, player[i].name,
           player[i].RATING,
		   (int) player[i].tiebreak[PER_RAT],
		   player[i].country,
		   player[i].tiebreak[SCORE]);
        if ((k+1)&1) savehtml << "<tr bgcolor=#e2e2f8> ";
        else savehtml << "<tr bgcolor=#ffffff> ";
        savehtml << stringa;
        for(j=0; j<nplayers; j++)
           { if (j==k)  savehtml<< "<td bgcolor=\"#000000\" width=\"27\"> &nbsp; </td>";
             else if (strlen(mm[k][j]) == 1) savehtml<< "<td align=\"center\">" << mm[k][j]<<str1<< "</td>";
             else if (strlen(mm[k][j]) == 2) savehtml<<"<td align=\"center\">" << mm[k][j]<<str2<< "</td>";
             else savehtml<<"<td align=\"center\">" << mm[k][j]<<str3<< "</td>";
           }
        savehtml<<"\n";
        savehtml << "</tr>\n";
      }
  savehtml << "</tbody>\n";
  savehtml << "</table>\n";
  savehtml << "<br> <p align=\"center\"><a href=\"http://www.vegachess.com\"> Generated by Vega </a></p> \n";
  savehtml << "</DIV></body>\n</html>\n";
  savehtml.Close();
}

void RoundData::RoundRobinCalendar()
//
// PURPOSE: show all the tournament calendar
//
{ int i, j, code, iw, ib, res, npairs, nround;
  String name, stringa;
  FileOut savepair(PATHDIR + "pairsRR.txt");
  if (NPlayer & 1) nround = NPlayer;
  else nround = NPlayer-1;

  if (PLAY_SYSTEM==ROUND_ROBIN2) nround = 2*nround;
      for (j=1; j<=nround; j++)
          { savepair << NAMETORNEO << t_(" : Round ") << j << "\n";
            savepair << "======================================================================\n";
            npairs = pairingRR[0][j];
            for (i=1; i<=npairs; i++)
                { code = pairingRR[i][j];
                  DecodeResult(code, &iw, &ib, &res);
                  savepair << NFormat("%3d = %3d %-20.20s - %3d %-20.20s  = ...-...\n",
                                       i, iw, player[iw].name, ib, player[ib].name);
                  savepair << "----------------------------------------------------------------------\n";
                }
              savepair << "\n\n";
          }
  savepair<<"\n Generated by Vega - www.vegachess.com\n";
  savepair.Close();
}
