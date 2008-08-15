#include "VegaMain.h"
#include "Tournament.h"

int RoundData::MakeNextPairing()
{ extern void SortPairs(int round);
  return 10;
}


void RoundData::SaveRound(int flag)
//
// save the round file
// flag = 0, save the file at the current round
// flag = 1, save a backup file at current round
//
{ int i, j, npair, numPlayers = NPlayer;
  int round = currentRound;
  String filename, s;
  if (flag==0) filename = PATHDIR + FILETORNEO + ".rnd";
  else filename = PATHDIR + FILETORNEO + AsString(TD.currentRound) + ".rnd";
  FileOut out(filename);

  out.PutLine( NFormat("%2d  # current round", currentRound ) );
  out.PutLine( " 1  # not used" );
  out.PutLine( " 1  # not used" );
  out.PutLine( " 1  # not used" );
  out.PutLine( NFormat("%2d  # STATUS_ROUND", STATUS_ROUND) );
  out.PutLine( " 1  # not used" );
  out.PutLine( " 1  # not used" );

  out.PutLine( "# players color" );
  for (i=1; i<=NPlayer; i++)
      { String s = NFormat("%3d  ", i);
        for (j=1; j<=round; j++)
            {  s << NFormat("%3d ", playerColor[i][j]);
	        }
        out.PutLine( s );
      }

  out.PutLine("# players opponent" );
  for (i=1; i<=NPlayer; i++)
      { String s = NFormat("%3d  ", i);
        for (j=1; j<=round; j++) s << NFormat("%3d ", playerOpponent[i][j]);
        out.PutLine( s );
      }

  out.PutLine( "# players floater");
  for (i=1; i<=NPlayer; i++)
      { String s = NFormat("%3d  ", i);
        for (j=1; j<=round; j++) s << NFormat("%d ", playerFloater[i][j]);
        out.PutLine( s );
      }

  out.PutLine( "# round results");
  for (i=1; i<=NPlayer; i++)
      { String s = NFormat("%3d  ", i);
        for (j=1; j<=round; j++) s << NFormat("%d ", roundResult[i][j]);
        out.PutLine( s );
      }

  out.PutLine( "# code pairing");
  if (NPlayer & 1) npair = (NPlayer-1)/2 + 1;
  else npair = NPlayer/2;
  for (i=0; i<=npair; i++)
      { String s = "";
	    for (j=1; j<=round; j++) s << NFormat("%7d ", pairing[i][j]);
        out.PutLine( s );
      }
  if (out.IsError()) Exclamation(t_("Error while writing .rnd file"));
  out.Close();
}


void RoundData::LoadRound(String filename)
{ FileIn in(filename);
  int i, j, npair, numPlayers = NPlayer;
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

  in.GetLine(); //skip this line // "# players color\n"
  for (i=1; i<=numPlayers; i++)
      { Vector<String> field = Split(in.GetLine(), ' ');
        //field.GetCount() coincide with currentRound
        for (j=1; j<field.GetCount(); j++)  playerColor[i][j] = StrIntValue( field[j] );
      }

  in.GetLine(); //skip this line // "# players opponent\n"
  for (i=1; i<=numPlayers; i++)
      { Vector<String> field = Split(in.GetLine(), ' ');
        //field.GetCount() coincide with currentRound
        for (j=1; j<field.GetCount(); j++)  playerOpponent[i][j] = StrIntValue( field[j] );
      }

  in.GetLine(); //skip this line // "# players opponent\n"
  for (i=1; i<=numPlayers; i++)
      { Vector<String> field = Split(in.GetLine(), ' ');
        //field.GetCount() coincide with currentRound
        for (j=1; j<field.GetCount(); j++)  playerFloater[i][j] = StrIntValue( field[j] );
      }

  in.GetLine(); //skip this line // "# players result\n"
  for (i=1; i<=numPlayers; i++)
      { Vector<String> field = Split(in.GetLine(), ' ');
        //field.GetCount() coincide with currentRound
        for (j=1; j<field.GetCount(); j++)  roundResult[i][j] = StrIntValue( field[j] );
      }

  // now reads the pairing
  in.GetLine(); //skip this line // "# code pairing\n"
  if (numPlayers & 1) npair = (numPlayers-1)/2 + 1;
  else npair = numPlayers/2;
  for (i=0; i<=npair; i++)
      { Vector<String> field = Split(in.GetLine(), ' ');
        for (j=1; j<=field.GetCount(); j++) pairing[i][j] = StrIntValue( field[j-1] );
      }
  in.Close();
}


void RoundData::AdjournPlayerState(int round)
//
// NOTE: it is called by MakeNextPairing and determine the state
// of each player at round 'round' necessary to calculate the next pairing
//
{ int id, j, np, maxcol;
  int diffcolor, jopp, jump, maxjump;
  int countblack, countwhite;
  double score, aro, buc;

  maxjump = 3; if (NRounds>9) maxjump=4; //set the maximum number of upfloating
  maxcol = 2 + PERMIT3COLOR;   // PERMIT3COLOR = 0, 1
  if (round==NRounds-1 && PLAY_SYSTEM==LIM) maxcol = 3; // last round exception

  for (id=1; id<=NPlayer; id++)
  {
//determine the due color
    diffcolor = 0;
    for (j=1; j<=round; j++) diffcolor += playerColor[id][j];
    player[id].colorDiff = diffcolor; // useful to assign color in case of equalization

    if (diffcolor>0) player[id].duecolor = BLACK;
   	else if (diffcolor<0) player[id].duecolor = WHITE;
    else { //equal black & white games
	       j=round;
	       while (playerColor[id][j] == NO_COLOR) j--;  // go back in history
	       if (playerColor[id][j] == WHITE) player[id].duecolor = BLACK;
	       else player[id].duecolor = WHITE;
	       // useful for player that got the BYE
          if (round==1 && playerColor[id][round]==NO_COLOR) player[id].duecolor = -1;
          // WARNING!!!! the previous player should be in the color group with less players
        }
//set the currentDuecolor equal to the duecolor
    player[id].currentDuecolor = player[id].duecolor;

//set flag canChangeDuecolor to met 2.3 and 2.4
	player[id].canChangeDuecolor = true;  //default
	if ( (diffcolor == maxcol) || (diffcolor == -maxcol)) player[id].canChangeDuecolor=false;
	else player[id].canChangeDuecolor = true;
	countblack = countwhite = 0;
	if (player[id].duecolor==WHITE) {
	   for (j=round; j>=1; j--) {//check for consecutive black to met 2.4
	       if (playerColor[id][j]==BLACK_COLOR) countblack++;
	       else if (playerColor[id][j]==WHITE_COLOR) break;
	   }
	   if (countblack==maxcol) player[id].canChangeDuecolor = false;
       if ( countblack==maxcol && currentRound==NRounds && !PERMIT3COLOR //permit3color is gia' contato in maxcol o si rischia 4 color
          && PLAY_SYSTEM==USCF) player[id].canChangeDuecolor = true;
	}
	if (player[id].duecolor==BLACK) {
	   for (j=round; j>=1; j--) {//check for consecutive white to met 2.4
	       if (playerColor[id][j]==WHITE_COLOR) countwhite++;
	       else if (playerColor[id][j]==BLACK_COLOR) break;
	   }
	   if (countwhite==maxcol) player[id].canChangeDuecolor=false;
       else if (countwhite==maxcol && currentRound==NRounds && !PERMIT3COLOR //permit3color is gia' contato in maxcol o si rischia 4 color
          && PLAY_SYSTEM==USCF) player[id].canChangeDuecolor = true;
	}

    if (PLAY_SYSTEM==USCF) {
	   countblack = countwhite = 0;
	   for (j=round; j>=1; j--) {//check for consecutive black
	       if (playerColor[id][j]==BLACK_COLOR) countblack++;
	       else if (playerColor[id][j]==WHITE_COLOR) break;
	   }
	   for (j=round; j>=1; j--) {//check for consecutive white
	       if (playerColor[id][j]==WHITE_COLOR) countwhite++;
	       else if (playerColor[id][j]==BLACK_COLOR) break;
	   }
	   // duecolor = 1 for white
	   // duecolor = 0 for black
	   // duecolor = -1 for neutral
       if ( countblack == 0 && countwhite == 0) player[id].duecolor = -1;
       else if ( countblack > 0 ) player[id].colorCons = countblack;
       else if ( countwhite > 0 ) player[id].colorCons = countwhite;
    }

//set ARO
	aro = 0.;
	np = 0;
	for (j=1; j<=round; j++) {
	      jopp = playerOpponent[id][j];
	      if (jopp != 0 &&  (roundResult[id][j] < 3)) // no BYE o game won for forfait
	         { aro += (double) player[jopp].RATING; np++;}
    }
    if (np != 0) player[id].tiebreak[ARO] = player[id].aro = (float) (aro/np);
    else player[id].tiebreak[ARO] = player[id].aro = (float) 0.;

//set score
	score = 0.;
	for (j=1; j<=round; j++) {
        if (roundResult[id][j]==1 || roundResult[id][j]==3) score += WON;
        else if (roundResult[id][j]==2 || roundResult[id][j]==6 || roundResult[id][j]==8) score += DRAW;
	}
    player[id].tiebreak[SCORE] = (float) score;
    if (NAccRound && NAccRound>currentRound && id<=NPlayer/2)
           player[id].tiebreak[SCORE] += WON; //accelerated rounds

//set flag canUpfloat
	jump = 0;
    if (PLAY_SYSTEM == DUBOV) {
        for (j=1; j<=round; j++)  if (playerFloater[id][j]) jump++;
	    if (jump >= maxjump) player[id].canUpfloat = false;
	    else if ( (jump < maxjump) && (playerFloater[id][round]) && (round<NRounds-1))
		    player[id].canUpfloat = false;
	    else if ( (jump < maxjump) && (playerFloater[id][round]) && (round==NRounds-1))
	        player[id].canUpfloat = true;
	    else player[id].canUpfloat = true;
    }
    if (PLAY_SYSTEM == LIM) {	// downfloat
       // if ( playerFloater[id][round]==1 ) playerState[id].canDownfloat = NO;
	   // else playerState[id].canDownfloat = YES;
        if ( playerFloater[id][round]==1 ) player[id].canUpfloat = false;
	    else player[id].canUpfloat = true;
    }

//set flag canGetBYE: who got 1 point without to play will not get the bye
// who got 1/2 point can get the bye if there are not player with flag 1
	player[id].canGetBYE = 1;
	for (j=1; j<=round; j++)
	    { if (roundResult[id][j]==3) // won for forfeit
	         { player[id].canGetBYE = 0;
               break;
	         }
          else if (roundResult[id][j]==6 || roundResult[id][j]==8)
               player[id].canGetBYE = 2;
	    }
//set remaining flags
	player[id].isAlreadyTested = false;

//set bucholz for pairing
  if (PLAY_SYSTEM == VEGA) {
    //calculate Buccholz
	buc = 0.;
	for (j=1; j<=round; j++)
	    { jopp = playerOpponent[id][j];
          if ( (jopp != 0) && (roundResult[id][j]<3) ) // no BYE, no forfait
	         { buc += player[jopp].tiebreak[SCORE];
             }
          else if (roundResult[id][j]>2 && (roundResult[id][j] != 8)) // BYE, forfait but NOT draw without bucholz
	         buc += player[id].tiebreak[SCORE];  // sum his score to the bucholz
	    }

    player[id].rating = (int) (buc*10);
    player[id].aro = buc*10.;
  }
/*  else if (PLAY_SYSTEM == BURST) {
    //calculate Extended Buccholz
	buc = 0.;
	for (j=1; j<=round; j++)
	    { jopp = playerOpponent[id][j];
          if ( (jopp != 0) && (roundResult[id][j]<3) ) // no BYE, no forfait
	         { buc += playerResult[jopp][SCORE];
             }
          else if (roundResult[id][j]>2 && (roundResult[id][j] != 8)) // BYE, forfait but NOT draw without bucholz
	         buc += playerResult[id][SCORE];  // sum his score to the bucholz
	    }
    player[id].aro = buc+playerResult[id][SCORE]; // Extende Buccholz
    player[id].rating = player[id].RATING;
   }
   */
   else player[id].rating = player[id].RATING;
  }

  // set the _BYE_
  player[0].duecolor = -1; // nessun colore
  player[0].canChangeDuecolor = true;
}

void RoundData::AdjournCrossTableSwiss(int round)
//
// NOTE: it is called just after the iserting the result of the round
// so at round N, the state refer to the previous N-1 rounds
//
{ int id, j, np;
  int  jopp;
  double score, aro, buc, min, max, sb;
  double totscore, sumtotscore,  modifiedscore[N_PLAYER_MAX+1];

  for (id=1; id<=NPlayer; id++)
  {
    //set ARO
	aro = 0.;
	np = 0;
	for (j=1; j<=round; j++)
	    { jopp = playerOpponent[id][j];
	      if (jopp != 0 &&  roundResult[id][j] < 3) // no BYE o game won for forfait
	         { aro += (double) player[jopp].RATING;
               np++;
             }
	    }
	if (np != 0) player[id].tiebreak[ARO] = (float) aro/np;
	else player[id].tiebreak[ARO] = (float) 0.;

	score = 0.; //set score, cumulative points and most blacks
    totscore=0.; //progressive score : 1, 2, 2.5, ...
    sumtotscore = 0.;  //cumulative points 1, 3, 5.5, ...
    player[id].tiebreak[MOST_BLACKS] = modifiedscore[id] = 0.;
    player[id].tiebreak[MOST_WINS] = 0.;

    // calculate modified score useful for bucholz
    for (j=1; j<=round; j++) {
        switch (roundResult[id][j])
        {  case 1:
             totscore += WON; sumtotscore += totscore;
             score += WON; player[id].tiebreak[MOST_WINS] += 1.;
             modifiedscore[id] += WON;
             break;
           case 3: // win to forfeit
             totscore += DRAW; sumtotscore += totscore;
             score += WON; // playerResult[id][MOST_WINS] += 1; // forfeit count as draw
             modifiedscore[id] += DRAW; // forfeit count as draw
             break;
           case 2: // draw
             totscore += DRAW; sumtotscore += totscore;
             score += DRAW;
             modifiedscore[id] += DRAW; // forfeit count as draw
             break;
           case 6:  // draw to forfeit
             totscore += DRAW; sumtotscore += totscore;
             score += DRAW;
             modifiedscore[id] += DRAW; // forfeit count as draw
             break;

           case 0:
             if (playerOpponent[id][j] !=0 ) sumtotscore += totscore;
             else {
                 sumtotscore += totscore;
                 modifiedscore[id] += DRAW; // forfeit count as draw
             }
             break;
	       case 4: // lost to forfeit
             sumtotscore += totscore + DRAW;
             modifiedscore[id] += DRAW; // forfeit count as draw
             break;
           case 8:
             sumtotscore += totscore;
             score += DRAW;
             modifiedscore[id] += DRAW;
             break;
        }
	    if (roundResult[id][j]==1 && playerColor[id][j]==-1)
	        player[id].tiebreak[MOST_BLACKS] += (float) 1.;
    }
    player[id].tiebreak[SCORE] = score;
    if (NAccRound && NAccRound>currentRound && id<=NPlayer/2)
                player[id].tiebreak[SCORE] += (float) WON; //accelerated rounds
    player[id].tiebreak[CUMUL] = (float) sumtotscore;
 }

  for (id=1; id<=NPlayer; id++)
  {	//set Buccholz
	buc = 0.;
	min = 10000.;
	max = 0.;
	for (j=1; j<=round; j++)
	    { jopp = playerOpponent[id][j];
          if ( (jopp != 0) && (roundResult[id][j]<3) ) // no BYE, no forfait
	         { // buc += playerResult[jopp][SCORE];
	           buc += modifiedscore[jopp];

	           if (modifiedscore[jopp]<min) min = modifiedscore[jopp];
	           if (modifiedscore[jopp]>max) max = modifiedscore[jopp];
	         }
          else if ( (roundResult[id][j]>2 && (roundResult[id][j] < 8)) || playerOpponent[id][j] ==0 ) // BYE, forfait but NOT draw without bucholz
	         { buc += modifiedscore[id];  // sum his score to the bucholz

	           if (modifiedscore[id]<min) min = modifiedscore[id];
	           if (modifiedscore[id]>max) max = modifiedscore[id];
	         }
          else if ( roundResult[id][j] == 8 ) // without bucholz
	         {
	           min = 0;
	         }
	    }

	if (buc > min) player[id].tiebreak[BUC_CUT1] = (float) buc-min;
	else player[id].tiebreak[BUC_CUT1] = 0.;

    if (buc - min > max ) player[id].tiebreak[BUC_MED] = (float) (buc - min - max);
    else player[id].tiebreak[BUC_MED] = 0.;

	player[id].tiebreak[BUC_TOT] = (float) buc;
  }

  for (id=1; id<=NPlayer; id++)
  {	//set Sonneborg-Berger
	sb = 0.;
	for (j=1; j<=round; j++)
	    { jopp = playerOpponent[id][j];
              if ( jopp != 0 && roundResult[id][j]==1  ) // win, no forfait
	          sb += modifiedscore[jopp];

              else if ( (jopp != 0) && (roundResult[id][j]==2)  ) // draw, no forfait
	          sb += modifiedscore[jopp]/2.;

              else if ( ((jopp == 0) || (roundResult[id][j]>2)) && (roundResult[id][j] < 8) ) // draw with himself (forfait)
              sb += modifiedscore[id]/2.;  // sum his half score to the bucholz
	    }
        player[id].tiebreak[SONN_BERG] = (float) sb;
  }
  // adjourn the rest of cross table
  CalcVarElo(NPlayer);
  //ShowClassificationByname(numberPlayer);
//  PrepareHTMLindex();
  NationalPlayerCard(NPlayer);  // here is calculated the performance rating
  setAPRO(round, NPlayer);
  ShowTableScore(NPlayer);
  CrossTableSwiss();
  ShowTableCategoryScore(NPlayer);
}


void RoundData::CrossTableSwiss()
{ int i, j, index;
  String stringa, str, strname;
  int *roundRes;
  int *playerCol;
  int *playerOpp;

  if (IS_SWISS) {
     playerOpp = &playerOpponent[0][0];
     roundRes = &roundResult[0][0];
     playerCol = &playerColor[0][0];
  }

  FileOut save(PATHDIR + "crosstbl.txt");
  FileOut savehtml (PATHDIRWWW + "crosstbl.html");

  save << NAMETORNEO << "\n";
  save << PLACETORNEO << " - " <<  DATATORNEO_B <<", "<<DATATORNEO_E  << "\n\n";
  save << t_(" Cross Table at round ")<< currentRound << "\n\n";
  save << " ID NAME                 Rtg  Tit Fed  Scr |";

  // produce HTML files
  savehtml << TD.HTMLCommonHeader();
  savehtml << HTMLNavBar();
  savehtml << "<h2>" << NAMETORNEO << "</h2>\n";
  savehtml << "<h2>" << PLACETORNEO << " - " <<  DATATORNEO_B <<", "<<DATATORNEO_E  << "</h2>\n";
  savehtml << "<h3> " << t_(" Cross Table at round ") << currentRound<< "</h3>\n";
  savehtml << "<table bgcolor=#efefef cellpadding=4 cellspacing=0 border=0> \n";
  savehtml << "<tbody>\n";
  savehtml << "<tr>";
  savehtml << "<th>ID</th> <th >NAME</th> <th>Rat</th> <th>PRat</th> <th>Nat</th> <th>Scr</th>\n";

  for(j=1; j<=currentRound; j++) {
      str = NFormat( " %3d  ", j);
      save << str;
      savehtml << "<th>" << str << "</th>";
  }
  save << "\n";
  savehtml << "</tr> \n";

  save << "--------------------------------------------";
  for(j=1; j<=currentRound; j++) save << "------";
  save << "\n";
  for (i=1; i<=NPlayer; i++)
      { if (player[i].isAvailable != "0") strname = NFormat("%-20.20s", player[i].name);
        else strname = NFormat("%-17.17s(W)", player[i].name);

        stringa = NFormat("%3d %-20.20s %4d %3.3s %3.3s %4.1f | ",
	       i, strname, player[i].RATING, player[i].codetitle, player[i].country, player[i].tiebreak[SCORE]);
           save << stringa;

        stringa = NFormat("<td> %3d </td> <td> <a href=\"playercard.html#%d\"> %s %s </a> </td> <td>%4d</td> <td align=center bgcolor=#EEEEEE>%4d</td> <td align=center>%3.3s</td> <td>%4.1f</td>",
	       i, i, player[i].codetitle, strname, player[i].RATING,
		   (int) player[i].tiebreak[PER_RAT], player[i].country, player[i].tiebreak[SCORE]);
           if (i&1) savehtml <<"<tr bgcolor=#e2e2f8> ";
           else savehtml << "<tr bgcolor=#ffffff> ";
           savehtml << stringa;

	  for(j=1; j<=currentRound; j++)
	   { if (PLAY_SYSTEM != ROUND_ROBIN && PLAY_SYSTEM != ROUND_ROBIN2) index = i*(N_ROUND_MAX+1) + j;
	     else index = i*(N_ROUND_MAX_RR+1) + j;

	     if (playerOpp[index]==0 && roundRes[index]==0) { //for late-comer - not played
	         stringa = " --   ";
		     save<< stringa;
		     savehtml << "<td>" << stringa << "</td>";
	     }
	     else if (playerOpp[index]==0 && roundRes[index]==3) { //for BYE: 1F - 0
	         stringa = "+BYE  ";
		     save << stringa;
		     savehtml << "<td>" << stringa << "</td>";
	     }
	     else if (playerOpp[index]==0 && roundRes[index]==4) { //for BYE: 1F - 0
	         stringa = "-BYE  ";
		     save << stringa;
		     savehtml << "<td>" << stringa << "</td>";
	     }
	     else if (playerOpp[index]==0 && (roundRes[index]==6 || roundRes[index]==8)) { //for BYE: XF
	         stringa = "=BYE  ";
		     save<< stringa;
		     savehtml << "<td>" << stringa << "</td>";
	     }
	     else
	       { savehtml << "<td>";

		     if (roundRes[index]==0 || roundRes[index]==4) {save<<"-"; savehtml << "-";}
             else if (roundRes[index]==1 || roundRes[index]==3) {save<<"+"; savehtml << "+";}
	         else if (roundRes[index]==2 || roundRes[index]==6 || roundRes[index]==8) {save<<"="; savehtml << "=";}

             if (playerCol[index]==-1) {save<<"B";savehtml << "B";}
	         else if (playerCol[index]==1) {save<<"W";savehtml << "W";}
	         else if (playerCol[index]==0) {save<<"N";savehtml << "N";}

             stringa = NFormat("%-3d ", playerOpp[index]);
	         save << stringa;

		     savehtml << stringa << "</td>";
	       }
	    }
	    save << "\n";
	    savehtml << "</tr>\n";
      }
  save.Close();

  savehtml << "</tbody>\n";
  savehtml << "</table>\n";
  savehtml << "<br> <a href=\"http://www.vegachess.com\"> Generated by Vega </a> \n";
  savehtml << "</DIV></body>\n</html>\n";
  savehtml.Close();
}

int RoundData::AssignColorLIM(int id1, int id2, int round)
//
// PURPOSE: determine the color for the players id1 and id2
//
// INPUT VARIABLE
//	id1   = player1 with his duecolor
//	id2   = player2 with his duecolor
//	round = current round
//
// OUTPUT VARIABLE
//	 1 to confirm the duecolor of id1 and change that of id2
//	 0 to confirm both duecolor
//	-1 to confirm the duecolor of id2 and change that of id1
//	 >1 flag of special operation when color history is the same
//
{ int j=0;
  int id1due, id2due, diffcolor1, diffcolor2;
  double maxscore = TD.WON*(round-1)/2.;
//char str[30];

//  sprintf(str, "median = %4.1f", maxscore);
//  WarningMessage(str, sss[S_CLOSE]);

  if (id2==0) return TD.CodifyResult(id1, id2, 3); //return the BYE and the result
  if (id1==0) return TD.CodifyResult(id1, id2, 4); //return the BYE and the result

  id1due = player[id1].duecolor;
  id2due = player[id2].duecolor;

  if (id1due != id2due) return 0;  // confirm their duecolor

  // here both have the same duecolor one must change... who?

  // 1. let's see if only one can change duecolor
  else if ((player[id1].canChangeDuecolor) &&
           (!player[id2].canChangeDuecolor) ) return -1;
  else if ((!player[id1].canChangeDuecolor) &&
           (player[id2].canChangeDuecolor) ) return 1;

  // 2. both have same duecolor and can change it,
  // let's see the biggest diffcolor to equalize the color
  diffcolor1 = 0;
  for (j=1; j<=round; j++) diffcolor1 += playerColor[id1][j];
  diffcolor2 = 0;
  for (j=1; j<=round; j++) diffcolor2 += playerColor[id2][j];
  if ( abs(diffcolor1) > abs(diffcolor2) ) return 1;
  else if ( abs(diffcolor1) < abs(diffcolor2) ) return -1;

  // 3. both have same duecolor and can change it and same diff: see history
  else {
     j = round;
     while (playerColor[id1][j]==playerColor[id2][j] && j>0) j--;
	 if (j==0) {

        // median group or above: duecolor to the higher ranked
	    if (player[id1].tiebreak[SCORE] > player[id2].tiebreak[SCORE]
            && player[id1].tiebreak[SCORE] >= maxscore) {
	       return 1;
	    }
	    else if (player[id2].tiebreak[SCORE] > player[id1].tiebreak[SCORE]
            && player[id2].tiebreak[SCORE] >= maxscore) {
	       return -1;
	    }
        // below the median group: duecolor to the lower ranked
	    else if (player[id1].tiebreak[SCORE] > player[id2].tiebreak[SCORE]
            && player[id1].tiebreak[SCORE] < maxscore) {
	       return -1;
	    }
	    else if (player[id2].tiebreak[SCORE] > player[id1].tiebreak[SCORE]
            && player[id2].tiebreak[SCORE] < maxscore) {
	       return 1;
	    }

        //same score: will decide the rating as before
	    // above or median group
        if (player[id1].rating > player[id2].rating
             && player[id1].tiebreak[SCORE] >= maxscore ) {
	       return 1;
	    }
	    else if (player[id2].rating > player[id1].rating
             && player [id2].tiebreak[SCORE] >= maxscore ) {
	       return -1;
	    }
        //same score: below median group
	    else if (player[id1].rating > player[id2].rating
             && player[id1].tiebreak[SCORE] < maxscore ) {
	       return -1;
	    }
	    else if (player[id2].rating > player[id1].rating
             && player[id2].tiebreak[SCORE] < maxscore ) {
	       return 1;
	    }


	    // same rating, see alphabetical order!
	    // above or median group
	    if ( strcmp(player[id1].name,player[id2].name) < 0
             && player[id1].tiebreak[SCORE] >= maxscore ) {
	       return 1;
	    }
	    else if ( strcmp(player[id2].name,player[id1].name) < 0
             && player[id2].tiebreak[SCORE] >= maxscore ) {
	       return -1;
	    }
	    // below median group
	    else if ( strcmp(player[id1].name,player[id2].name) < 0
             && player[id1].tiebreak[SCORE] < maxscore ) {
	       return -1;
	    }
	    else if ( strcmp(player[id2].name,player[id1].name) < 0
             && player[id2].tiebreak[SCORE] < maxscore ) {
	       return 1;
	    }
     }

     // il primo a cambiare gli viene confermato il proprio duecolor
	 else if ( player[id1].duecolor && (playerColor[id1][j] == BLACK_COLOR)) return 1;
	 else if ( (!player[id1].duecolor) && (playerColor[id1][j] == WHITE_COLOR)) return 1;
	 else return -1;
 }
 return 0; // fictitious return to remove a silly warning message
}


int RoundData::AssignColor(int id1, int id2, int round)
//
// PURPOSE: determine the color for the players id1 and id2
//
// INPUT VARIABLE
//	id1   = player1 with his duecolor
//	id2   = player2 with his duecolor
//	round = current round
//
// OUTPUT VARIABLE
//	 1 to confirm the duecolor of id1 and change that of id2
//	 0 to confirm both duecolor
//	-1 to confirm the duecolor of id2 and change that of id1
//	 >1 flag of special operation when color history is the same
//
{ int j=0;
  int id1due, id2due;

  if (id2==0) return TD.CodifyResult(id1, id2, 3); //return the BYE and the result
  if (id1==0) return TD.CodifyResult(id2, id1, 3); //return the BYE and the result

  id1due = player[id1].duecolor;
  id2due = player[id2].duecolor;

  if (id1due != id2due) return 0;  // confirm their duecolor

  // here both have the same duecolor one must change... who?

  // 1. let's see if only one can change duecolor
  else if ((player[id1].canChangeDuecolor) &&
           (!player[id2].canChangeDuecolor) ) return -1;
  else if ((!player[id1].canChangeDuecolor) &&
           (player[id2].canChangeDuecolor) ) return 1;

  // 2. both have same duecolor and can change it, let's see the diffcolor to equalize
/*  diffcolor1 = 0;
  for (j=1; j<=round; j++) diffcolor1 += playerColor[id1][j];
  diffcolor2 = 0;
  for (j=1; j<=round; j++) diffcolor2 += playerColor[id2][j];
  if ( abs(diffcolor1) > abs(diffcolor2) ) return 1;
  else if ( abs(diffcolor1) < abs(diffcolor2) ) return -1;
*/
  if ( abs(player[id1].colorDiff) > abs(player[id2].colorDiff) ) return 1;
  else if ( abs(player[id1].colorDiff) < abs(player[id2].colorDiff) ) return -1;

  // 3. same diffcolor, let's test the alternation
  else { //
     j = round;
     while (TD.playerColor[id1][j]==TD.playerColor[id2][j] && j>0) j--;
	 if (j==0) { // same history, let's see highest ARO, rating, etc

     if (TD.PLAY_SYSTEM==USCF) {
	    if (player[id1].tiebreak[SCORE] > player[id2].tiebreak[SCORE]) {
	       //return CodifyResult(id1, id2, 9);
	       return 1;
	    }
	    else if (player[id2].tiebreak[SCORE] > player[id1].tiebreak[SCORE]) {
	       //return CodifyResult(id2, id1, 9);
	       return -1;
	    }
	    // same SCORE, let's see max rating
	    else if (player[id2].rating > player[id1].rating) {
	       //return CodifyResult(id2, id1, 9);
	       return -1;
	    }
	    else if (player[id1].rating > player[id2].rating) {
	       //return CodifyResult(id1, id2, 9);
	       return 1;
	    }
	    // same rating, see alphabetical order!
	    else if ( strcmp(player[id1].name,player[id2].name) < 0 ) {
	       //return CodifyResult(id1, id2, 9);
	       return 1;
	    }
	    else if ( strcmp(player[id1].name,player[id2].name) > 0 ) {
	       //return CodifyResult(id2, id1, 9);
	       return -1;
	    }
	 }
     else {	// dubov, vega
	    if (player[id1].aro > player[id2].aro) {
	       return TD.CodifyResult(id1, id2, 9);
	    }
	    else if (player[id2].aro > player[id1].aro) {
	       return TD.CodifyResult(id2, id1, 9);
	    }
	       // same ARO, let's see min rating
	    else if (player[id1].rating < player[id2].rating) {
	       return TD.CodifyResult(id1, id2, 9);
	    }
	    else if (player[id2].rating < player[id1].rating) {
	       return TD.CodifyResult(id2, id1, 9);
	    }
	       // same rating, see alphabetical order!
	    else if ( strcmp(player[id1].name,player[id2].name) < 0 ) {
	       return TD.CodifyResult(id1, id2, 9);
	    }
	    else if ( strcmp(player[id1].name,player[id2].name) > 0 ) {
	       return TD.CodifyResult(id2, id1, 9);
	    }
	 }
   }

   // il primo a cambiare gli viene confermato il proprio duecolor
   else if ( player[id1].duecolor && (TD.playerColor[id1][j] == BLACK_COLOR)) return 1;
   else if ( (!player[id1].duecolor) && (TD.playerColor[id1][j] == WHITE_COLOR)) return 1;
   else return -1;
 }
 return 0; // fictitious return to remove a silly warning message
}

void RoundData::VerifyColor(int round)
//
// PURPOSE: determine the color for each pair of the round 'round'
//
// INPUT VARIABLE
//	round = current round
//
{ int i, npairs, id1, id2, res, code, flag;
  npairs = TD.pairing[0][round];
  for (i=1; i<=npairs; i++)
      { code = TD.pairing[i][round];
        TD.DecodeResult(code, &id1, &id2, &res);
        // -----------BUG !!!! aggiungi LIM, fatto il 24.10.2006
        // il problema si presenta solo nella versione compilata con msc8
        if (TD.PLAY_SYSTEM==LIM) flag = AssignColorLIM(id1, id2, round);
        else flag = AssignColor(id1, id2, round);

	    if (flag==1) { // confirm id1 and change id2 color
	             if (player[id1].duecolor)
	                  { TD.pairing[i][round] = TD.CodifyResult(id1, id2, 9);
			     }
	   		     else { TD.pairing[i][round] = TD.CodifyResult(id2, id1, 9);
			     }
	    }
 	    else if (flag==0) { // confirm both
	        if (player[id1].duecolor)
	                     { TD.pairing[i][round] = TD.CodifyResult(id1, id2, 9);
			     }
	   		else { TD.pairing[i][round] = TD.CodifyResult(id2, id1, 9);
			     }
	    }
	    else if (flag==-1) { // confirm id2 and change id1 color
	    if (player[id2].duecolor)
                 { TD.pairing[i][round] = TD.CodifyResult(id2, id1, 9);
			     }
	   		else { TD.pairing[i][round] = TD.CodifyResult(id1, id2, 9);
			     }
	    }
        else if (flag>1) TD.pairing[i][round] = flag;
     }
}

void RoundData::DoFirstRound()
//
// PURPOSE: prepare the pairing for the first round
{ int nhalf, ans=1, n2, groupOdd = 0;
  int i, k, id, prating, kshift, ratbye=1000000;
  int idbye, id1, id2, playerAvailable[N_PLAYER_MAX+1], countAvailable=0;

  //if (!RANDOM_SORT) ans = QuestionMessage(sss[S_HPWHI], sss[S_NO], sss[S_YES], NULL);//who get the white?

  ans = PromptYesNo(t_("Do you want to assign the WHITE&to the higher rated players?"));

  for (i=1; i<=NPlayer; i++)
      { if (player[i].isAvailable == "1")   // get only available player
        { countAvailable++;
	      playerAvailable[countAvailable] = i;
	      idbye = i; // he is the last available as they are sorted by rating
          //playerState[i].canChangeDuecolor = true;
        }
      }
  if (countAvailable & 1) {     // if number player odd
      if (PLAY_SYSTEM != USCF) pairing[countAvailable/2+1][currentRound] = CodifyResult(idbye, 0, 3);
      else { // USCF treatmet
         for (k=countAvailable; k>=1; k--)
             { id = playerAvailable[k];
               prating = player[id].RATING;
               if (prating==0) prating = 10000; //unrated treatment
               if (prating < ratbye)
               { idbye = id;
                 ratbye = prating;
                 kshift = k;
               }
             }
         for (k=kshift; k<=countAvailable; k++) //remove the bye from the list
             { playerAvailable[k] = playerAvailable[k+1];
             }
         pairing[countAvailable/2+1][currentRound] = CodifyResult(idbye, 0, 3);
       }
       groupOdd = 1;
  }

  nhalf = countAvailable/2;
  // set pair and colors for first round; by default the first player has white
  if (NAccRound) {  // accelerated rounds
   n2 = nhalf/2;
   for (i=1; i<=n2; i++) // first group made by nhalf player, i.e. n2 pairs
      { id1 = playerAvailable[i];
        //if (PLAY_SYSTEM!=BURST) id2 = playerAvailable[i+n2];
        //else id2 = playerAvailable[2*n2+1-i];
        id2 = playerAvailable[i+n2];

        if (ans) { // white at highest rated
            if (i & 1) pairing[i][1] = CodifyResult(id1, id2, 9); // coppie dispari
	        else  pairing[i][1] = CodifyResult(id2, id1, 9);
        }
        else {
            if (i & 1) pairing[i][1] = CodifyResult(id2, id1, 9); // coppie dispari
	        else  pairing[i][1] = CodifyResult(id1, id2, 9);
        }
      }
   for (i=n2+1; i<=nhalf; i++) // second group made by nhalf player, i.e. n2 pairs
      { id1 = playerAvailable[i + n2];
        if (PLAY_SYSTEM!=BURST) id2 = playerAvailable[i + nhalf];
        else id2 = playerAvailable[2*nhalf+n2+1-i];
        if (ans) { // white at highest rated
            if (i & 1) pairing[i][1] = CodifyResult(id1, id2, 9); // coppie dispari
	        else  pairing[i][1] = CodifyResult(id2, id1, 9);
        }
        else {
            if (i & 1) pairing[i][1] = CodifyResult(id2, id1, 9); // coppie dispari
	        else  pairing[i][1] = CodifyResult(id1, id2, 9);
        }
      }
  }
  else {  // not accelerated rounds
    for (i=1; i<=nhalf; i++)
      { id1 = playerAvailable[i];
        //if (PLAY_SYSTEM!=BURST) id2 = playerAvailable[i+nhalf];
        //else id2 = playerAvailable[2*nhalf+1-i];
        id2 = playerAvailable[i+nhalf];

        if (ans) { // white at highest rated
            if (i & 1) pairing[i][1] = CodifyResult(id1, id2, 9); // coppie dispari
	        else  pairing[i][1] = CodifyResult(id2, id1, 9);
        }
        else {
            if (i & 1) pairing[i][1] = CodifyResult(id2, id1, 9); // coppie dispari
	        else  pairing[i][1] = CodifyResult(id1, id2, 9);
        }
      }
   }
   //----------- adesso aggiungere le coppie formate da chi ha avuto il BYE ---------!!!
   // save the number of pairs
   if (groupOdd) pairing[0][1] = nhalf+1;
   else pairing[0][1] = nhalf;

   //set the players that got a BYE
   for (i=1; i<=NPlayer; i++) {
      if ( IsNull(player[i].isAvailable) ) { // get byed player
         playerOpponent[i][currentRound] = 0;  // BYE assigned
         roundResult[i][currentRound] = 3;  // result 1F-0
         playerColor[i][currentRound] = 0;  // no-color
      }
   }

}
