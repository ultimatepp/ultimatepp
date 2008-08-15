#include "VegaMain.h"
#include "Tournament.h"


void RoundData::ShowTableStat()
//
// PURPOSE: show statistics information about tournament
//
{
  int j;
  float sum=0., sumfide=0.;
  String stringa, str, countrylist[N_PLAYER_MAX+1];
  int idopp, ctitle[6], titopp=0, ratopp=0;
  int playernothostfed=0, titoppnothost=0;
  int playerhostfed=0, countcountry=0, l, found;
  extern int roundfloat( double per);

  FileOut save (PATHDIR + "tourstat.txt");
  save<< "\n    Tournament Name: " << NAMETORNEO << "\n";
  save<< "    Place: " << PLACETORNEO << "\n";
  save<< "    Federation: " << HOSTFEDERATION << "\n";
  save<< "    Date: " << DATATORNEO_B <<", "<<DATATORNEO_E  << "\n";
  save<< "    Arbiter: " << ARBITER << "\n";

  if (PLAY_SYSTEM== DUBOV) str = "swiss FIDE (Dubov)\n";
  else if (PLAY_SYSTEM== ROUND_ROBIN) str = "Single Round Robin\n";
  else if (PLAY_SYSTEM == ROUND_ROBIN2) str = "Double Round Robin\n";
  else if (PLAY_SYSTEM== VEGA) str = "swiss vega\n";
  else if (PLAY_SYSTEM== USCF) str = "swiss USCF\n";
  else if (PLAY_SYSTEM== LIM) str = "swiss FIDE (Lim)\n";
  save<< "    Play System: " << str;
  save<< "    Number of rounds: " << NRounds << "\n";
  save<< "    Score game: 0-" << DRAW << "-" << WON << "\n";

  save<< "    Tie break: ";
  j=1;
  while (tyebreakorder[j]) {
          switch ( tyebreakorder[j] )
          { case 1: save <<  tyebreakname[ tyebreakorder[j] ];
            break;
            case 2: save <<  tyebreakname[ tyebreakorder[j] ];
            break;
            case 3: save <<  tyebreakname[ tyebreakorder[j] ];
            break;
            case 4: save <<  tyebreakname[ tyebreakorder[j] ];
            break;
            case 5: save <<  tyebreakname[ tyebreakorder[j] ];
            break;
            case 6: save <<  tyebreakname[ tyebreakorder[j] ];
            break;
            case 7: save <<  tyebreakname[ tyebreakorder[j] ];
            break;
            case 8: save <<  tyebreakname[ tyebreakorder[j] ];
            break;
            case 9: save <<  tyebreakname[ tyebreakorder[j] ];
            break;
          }
          j++;
  }
  save<< "\n";
  // count titled and rated player and calculate their aro
  for(j=0; j<=5; j++) ctitle[j]=0;
  for (j=1; j<=NPlayer; j++)
      {  idopp = j;
         sum += player[j].ratingnat; //BUG fixed (not yet in 4.2.7

	     if ( player[idopp].country == HOSTFEDERATION ||
	          player[idopp].country.GetLength() < 3 // hanno inserito una provincia o uno stato
	         ) playerhostfed++;
	     else playernothostfed++;
         if (player[j].ratingfide>0)  {
             ratopp++; //numfideplayers
             sumfide += player[j].ratingfide;  //BUG fixed (not yet in 4.2.7
	     }
         if (player[idopp].title>=10) titopp++;
	     if (player[idopp].title>=10 && player[idopp].country != HOSTFEDERATION) titoppnothost++;
         switch (player[idopp].title)
	       { case 20: ctitle[0]++;
                      break;
             case 16: ctitle[1]++;
                      break;
             case 18: ctitle[2]++;
                      break;
             case 14: ctitle[3]++;
                      break;
             case 12: ctitle[4]++;
                      break;
             case 11: ctitle[5]++;
                      break;
	       }
	       // count federations
	       l=0; found=0;
	       while (l<=countcountry && !found) {
	          if ( player[idopp].country == countrylist[l]) {
                   found = 1;
                   break;
              }
              else l++;
	       }
           if (!found) {
               countcountry++;
               countrylist[countcountry] = player[idopp].country;
           }
     }
  save<< "\n";
  save<< "    Registered Players: " << NPlayer << "\n";
  save<< "    Number of Federations: " << countcountry << "\n";
  save<< "    Average Rating (all): " << roundfloat(sum/NPlayer) << "\n";
  if (ratopp>0) save<< "    Average Rating (only FIDE rated): " << roundfloat(sumfide/ratopp) << "\n";
  else          save<< "    Average Rating (only FIDE rated): " << 0 << "\n";
  save<< "    Players from "<<HOSTFEDERATION << " Federation: " << playerhostfed << "\n";
  save<< "    Players NOT from "<<HOSTFEDERATION << " Federation " << playernothostfed << "\n";
  save<< "    FIDE rated players: " << ratopp << "\n";
  save<< "    FIDE unrated players: " << NPlayer-ratopp << "\n";
  save<< "    FIDE titled players : " << titopp << " of which:\n";
  save<< "        GM players: " << ctitle[0] << "\n";
  save<< "       WGM players: " << ctitle[2] << "\n";
  save<< "        IM players: " << ctitle[1] << "\n";
  save<< "       WIM players: " << ctitle[3] << "\n";
  save<< "        FM players: " << ctitle[4] << "\n";
  save<< "       WFM players: " << ctitle[5] << "\n";
  save<< "    FIDE titled players from " <<HOSTFEDERATION<<" Federation: " << titopp-titoppnothost << "\n";
  save<< "    FIDE titled players NOT from " <<HOSTFEDERATION<<" Federation: " << titoppnothost << "\n";
  save<<"\n Generated by Vega - www.vegachess.com\n";
  save.Close();
  ShowTableStatHTML();
}

void RoundData::ShowTableStatHTML()
//
// PURPOSE: show statistics information about tournament
//
{
  int j;
  float sum=0., sumfide=0.;
  String stringa, str, countrylist[N_PLAYER_MAX+1];
  int idopp, ctitle[6], titopp=0, ratopp=0;
  int playernothostfed=0, titoppnothost=0;
  int playerhostfed=0, countcountry=0, l, found;
  extern int roundfloat( double per);

  FileOut save (PATHDIRWWW + "tourstat.html");
  save << HTMLCommonHeader();
  save << HTMLNavBar();
  save << "<h2>" << "Tournament summary" << "</h2>\n";

  save << "<table bgcolor=#efefef cellpadding=4 cellspacing=0 border=1> \n";
  save << "<tbody>\n";

  save<< "<tr> <td bgcolor=#e2e2f8> Tournament Name </td> <td> " << NAMETORNEO << "</td> </tr>\n";
  save<< "<tr><td bgcolor=#e2e2f8>  Place </td> <td> " << PLACETORNEO << "</td> </tr>\n";
  save<< "<tr><td bgcolor=#e2e2f8> Federation  </td> <td> " << HOSTFEDERATION << "</td> </tr>\n";
  save<< "<tr><td bgcolor=#e2e2f8> Date Begin  </td> <td> " << DATATORNEO_B << "</td> </tr>\n";
  save<< "<tr><td bgcolor=#e2e2f8> Date End  </td> <td> " << DATATORNEO_E << "</td> </tr>\n";
  save<< "<tr><td bgcolor=#e2e2f8> Arbiter  </td> <td> " << ARBITER << "</td> </tr>\n";

  if (PLAY_SYSTEM== DUBOV) str = "swiss FIDE (Dubov)\n";
  else if (PLAY_SYSTEM== ROUND_ROBIN) str = "Single Round Robin\n";
  else if (PLAY_SYSTEM == ROUND_ROBIN2) str = "Double Round Robin\n";
  else if (PLAY_SYSTEM== VEGA) str = "swiss vega\n";
  else if (PLAY_SYSTEM== USCF) str = "swiss USCF\n";
  else if (PLAY_SYSTEM== LIM) str = "swiss FIDE (Lim)\n";
  save<< "<tr><td bgcolor=#e2e2f8> Play System  </td> <td> " << str << "</td> </tr>\n";
  save<< "<tr><td bgcolor=#e2e2f8> Number of rounds  </td> <td> " << NRounds << "</td> </tr>\n";
  save<< "<tr><td bgcolor=#e2e2f8> Score game  </td> <td> 0-" << DRAW << "-" << WON  << "</td> </tr>\n";

  save<< "<tr><td bgcolor=#e2e2f8> Tie break </td> <td> ";

  j=1;
  while (tyebreakorder[j]) {
          switch ( tyebreakorder[j] )
          { case 1: save <<  tyebreakname[ tyebreakorder[j] ];
            break;
            case 2: save <<  tyebreakname[ tyebreakorder[j] ];
            break;
            case 3: save <<  tyebreakname[ tyebreakorder[j] ];
            break;
            case 4: save <<  tyebreakname[ tyebreakorder[j] ];
            break;
            case 5: save <<  tyebreakname[ tyebreakorder[j] ];
            break;
            case 6: save <<  tyebreakname[ tyebreakorder[j] ];
            break;
            case 7: save <<  tyebreakname[ tyebreakorder[j] ];
            break;
            case 8: save <<  tyebreakname[ tyebreakorder[j] ];
            break;
            case 9: save <<  tyebreakname[ tyebreakorder[j] ];
            break;
          }
          j++;
  }
  save << "</td> </tr>\n";
  // count titled and rated player and calculate their aro
  for(j=0; j<=5; j++) ctitle[j]=0;
  for (j=1; j<=NPlayer; j++)
      {  idopp = j;
         sum += player[j].ratingnat; //BUG fixed (not yet in 4.2.7

	     if ( player[idopp].country == HOSTFEDERATION ||
	          player[idopp].country.GetLength() < 3 // hanno inserito una provincia o uno stato
	         ) playerhostfed++;
//	     if ( player[idopp].country == HOSTFEDERATION) playerhostfed++;
	     else playernothostfed++;
         if (player[j].ratingfide>0)  {
             ratopp++;
             sumfide += player[j].ratingfide; //BUG fixed (not yet in 4.2.7
	     }
         if (player[idopp].title>=10) titopp++;
	     if (player[idopp].title>=10 && player[idopp].country != HOSTFEDERATION) titoppnothost++;
         switch (player[idopp].title)
	       { case 20: ctitle[0]++;
                      break;
             case 16: ctitle[1]++;
                      break;
             case 18: ctitle[2]++;
                      break;
             case 14: ctitle[3]++;
                      break;
             case 12: ctitle[4]++;
                      break;
             case 11: ctitle[5]++;
                      break;
	       }
	       // count federations
	       l=0; found=0;
	       while (l<=countcountry && !found) {
	          if ( player[idopp].country == countrylist[l]) {
                   found = 1;
                   break;
              }
              else l++;
	       }
           if (!found) {
               countcountry++;
               countrylist[countcountry] = player[idopp].country;
           }
     }

  save<< "<tr><td bgcolor=#e2e2f8> Registered Players </td> <td> " << NPlayer << "</td> </tr>\n";
  save<< "<tr><td bgcolor=#e2e2f8> Number of Federations </td> <td> " << countcountry << "</td> </tr>\n";
  save<< "<tr><td bgcolor=#e2e2f8> Average Rating (all) </td> <td> " << roundfloat(sum/NPlayer) << "</td> </tr>\n";
  if (ratopp>0) save<< "<tr><td bgcolor=#e2e2f8> Average Rating (only FIDE rated) </td> <td> " << roundfloat(sumfide/ratopp) << "</td> </tr>\n";
  else          save<< "<tr><td bgcolor=#e2e2f8> Average Rating (only FIDE rated) </td> <td> " << 0 << "</td> </tr>\n";
  save<< "<tr><td bgcolor=#e2e2f8> Players from "<<HOSTFEDERATION << " Federation </td> <td> " << playerhostfed << "</td> </tr>\n";
  save<< "<tr><td bgcolor=#e2e2f8> Players NOT from "<<HOSTFEDERATION << " Federation </td> <td> " << playernothostfed << "</td> </tr>\n";
  save<< "<tr><td bgcolor=#e2e2f8> FIDE rated players </td> <td> " << ratopp << "</td> </tr>\n";
  save<< "<tr><td bgcolor=#e2e2f8> FIDE unrated players </td> <td> " << NPlayer-ratopp << "</td> </tr>\n";
  save<< "<tr><td bgcolor=#e2e2f8>  FIDE titled players </td> <td> " << titopp  << "</td> </tr>\n";
  save<< "<tr><td bgcolor=#e2e2f8> GM players </td> <td> " << ctitle[0]  << "</td> </tr>\n";
  save<< "<tr><td bgcolor=#e2e2f8> WGM players </td> <td> " << ctitle[2] << "</td> </tr>\n";
  save<< "<tr><td bgcolor=#e2e2f8>  IM players </td> <td> " << ctitle[1] << "</td> </tr>\n";

  save<< "<tr><td bgcolor=#e2e2f8> WIM players</td> <td> " << ctitle[3] << "</td> </tr>\n";
  save<< "<tr><td bgcolor=#e2e2f8> FM players </td> <td> " << ctitle[4] << "</td> </tr>\n";
  save<< "<tr><td bgcolor=#e2e2f8> WFM players </td> <td> " << ctitle[5] << "</td> </tr>\n";
  save<< "<tr><td bgcolor=#e2e2f8> FIDE titled player from " <<HOSTFEDERATION<<" Federation </td> <td> " << titopp-titoppnothost << "</td> </tr>\n";
  save<< "<tr><td bgcolor=#e2e2f8> FIDE titled player NOT from " <<HOSTFEDERATION<<" Federation </td> <td> " << titoppnothost << "</td> </tr>\n";
  save<< "<tr><td bgcolor=#e2e2f8>  IM players </td> <td> " << ctitle[1] << "</td> </tr>\n";

  save << "</tbody>\n";
  save << "</table>\n";
  save << "<br> <p align=\"center\"><a href=\"http://www.vegachess.com\"> Generated by Vega </a></p> \n";
  save << "</DIV></body>\n</html>\n";
  save.Close();
}

void RoundData::SortedCross(int nplayers, int flag)
//
// PURPOSE: sort the cross table according to the ranking
//
// INPUT VARIABLE
//	n = number of players to be sorted
//
// flag = 1 for score ID regenation, 0 according rating, title and name
//
{ int i, k, j, id, index;
  int arr[N_PLAYER_MAX+1];
  int id_to_pos[N_PLAYER_MAX+1];
  String stringa, str;
  FileOut save (PATHDIR + "sortcros.txt");
  int *roundRes;
  int *playerCol;
  int *playerOpp;

  extern int  CompareScoreName(const void *pid1, const void *pid2);
  extern int  CompareRatingTitleName( const void *pid1, const void *pid2);

  for (i=0; i<nplayers; i++)
      { arr[i] = i+1; // set the id of each player in the array to be sort
      }
  if (PLAY_SYSTEM != ROUND_ROBIN && PLAY_SYSTEM != ROUND_ROBIN2) {
     playerOpp = &playerOpponent[0][0];
     roundRes = &roundResult[0][0];
     playerCol = &playerColor[0][0];
  }
  else {
     playerOpp = &playerOpponentRR[0][0];
     roundRes = &roundResultRR[0][0];
     playerCol = &playerColorRR[0][0];
  }
  if (flag) qsort(arr, nplayers, sizeof(int), CompareScoreName);
  else  qsort(arr, nplayers, sizeof(int), CompareRatingTitleName);

  for (i=0; i<nplayers; i++) // 'i' is the ranking position after we performed the sort
      { id = arr[i];
        id_to_pos[id] = i;  // new id after the sort
      }
  save<< NAMETORNEO << "\n";
  save<< PLACETORNEO << " - " <<  DATATORNEO_B <<", "<<DATATORNEO_E  << "\n\n";
  save<< t_(" Cross Table at round ")<< currentRound <<"\n\n";

  save << " ID NAME                 Rtg  Tit Fed  Scr |";
  for(j=1; j<=currentRound; j++) save << NFormat(" %3d  ", j);
  save << "\n";
  save << "--------------------------------------------";
  for(j=1; j<=currentRound; j++) save << "------";
  save << "\n";

  for (k=0; k<nplayers; k++)
      { i=arr[k];
        if (player[i].isAvailable != "0") {
           save << NFormat("%3d %-20.20s %4d %3.3s %3.3s %4.1f | ",
	           k+1, // k is the ranking position now
		   player[i].name,
           player[i].RATING,
		   player[i].codetitle,
		   player[i].country,
		   player[i].tiebreak[SCORE]);
        }
        else
        {
          save << NFormat("%3d %-17.17s(W) %4d %3.3s %3.3s %4.1f | ",
    	       k+1,
	       player[i].name,
           player[i].RATING,
	       player[i].codetitle,
	       player[i].country,
           player[i].tiebreak[SCORE]);
        }
	for(j=1; j<=currentRound; j++)
	   { if (IS_SWISS) index = i*(N_ROUND_MAX+1) + j;
	     else index = i*(N_ROUND_MAX_RR+1) + j;

	     if (playerOpp[index]==0 && roundRes[index]==0) { //for late-comer - not played
		     save<< " --   ";
	     }
	     else if (playerOpp[index]==0 && roundRes[index]==3) { //for BYE: 1F - 0
		     save << "+BYE  ";
	     }
	     else if (playerOpp[index]==0 && roundRes[index]==4) { //for BYE: 1F - 0
	         save << "-BYE  ";
	     }
	     else if (playerOpp[index]==0 && (roundRes[index]==6 || roundRes[index]==8)) { //for BYE: XF
		     save << "=BYE  ";
	     }
	     else {
	       if (roundRes[index]==0 || roundRes[index]==4) save<<"-";
	       else if (roundRes[index]==1 || roundRes[index]==3) save<<"+";
	       else if (roundRes[index]==2 || roundRes[index]==6) save<<"=";
	       if (playerCol[index]==-1) save<<"B";
	       else if (playerCol[index]==1) save<<"W";
	       else if (playerCol[index]==0) save<<"N";
	       save << NFormat("%-3d ", id_to_pos[ playerOpp[index] ] + 1);
         }
	   }
	save<<"\n";
      }
  save<<"\n Generated by Vega - www.vegachess.com\n";
  save.Close();
}

void RoundData::FideEloVariation(int nplayers)
{ int i, j, idopp, noppfide, index, rp;
  String stringa;
  double tot, totscore, totscorefide, we_w, result, arofide, totexp;
  double arrp[N_ROUND_MAX_RR+2][2];
  int *roundRes;
  int *playerOpp;
  extern int p_dp[];
  extern int roundfloat( double per);
  FileOut save (PATHDIR + "felovar.txt");

  if (IS_SWISS) {
     playerOpp = &playerOpponent[0][0];
     roundRes = &roundResult[0][0];
  }
  else {
     playerOpp = &playerOpponentRR[0][0];
     roundRes = &roundResultRR[0][0];
  }
  save<< NAMETORNEO << ", " << PLACETORNEO << "\n";
  save<< DATATORNEO_B <<", "<<DATATORNEO_E  << "\n\n";
  save<< t_(" FIDE rating variation at round ")<< currentRound << "\n\n";

  save <<" -----------------------------------------------------------------------------------\n";
  save <<"   ID  Cat  Name                   FIDE ID  Fed   Rtg  Ng  Score  ARO   W-We   Rp\n";
  save <<" -----------------------------------------------------------------------------------\n";
  for (i=1; i<=nplayers; i++)
  { tot = totexp = 0.;
    arofide = 0.;
    noppfide = 0;
    totscore = totscorefide = 0.; //score without to consider the forfait

    // preliminary test to calculate nopp (total)  and noppfide (only fide opponent)
    for(j=1; j<=currentRound; j++)
       { if (PLAY_SYSTEM != ROUND_ROBIN && PLAY_SYSTEM != ROUND_ROBIN2) index = i*(N_ROUND_MAX+1) + j;
	     else index = i*(N_ROUND_MAX_RR+1) + j;
	     idopp = playerOpp[index];
         if ( idopp!=0 && (roundRes[index]<3) ) { // not forfait
             if (player[idopp].ratingfide > 0 )  noppfide++;
             //nopp++;
         }
       }

    // elaboration
    if ( ((player[i].ratingfide>0) && (noppfide>0)) || ((player[i].ratingfide==0) && (noppfide>=3)) )
    {
       calcelo(i, arrp, currentRound);
       for(j=1; j<=currentRound; j++)
	   { if (IS_SWISS) index = i*(N_ROUND_MAX+1) + j;
	     else index = i*(N_ROUND_MAX_RR+1) + j;

	     idopp = playerOpp[index];
         if ( idopp!=0 && (player[idopp].ratingfide>0) && (roundRes[index]<3)  ) { // not forfait
	            totexp += arrp[j][1];
                //if (player[idopp].ratingfide>0) arofide += player[idopp].ratingfide;
                arofide += player[idopp].ratingfide;
	            if (roundRes[index]==1)
                     { //if (player[idopp].ratingfide>0)  {
                           totscorefide += 1.0;
                       //}
                       totscore += 1.0; result = 1.;
                     }
	            else if (roundRes[index]==2)
	                 { //if (player[idopp].ratingfide>0) {
                          totscorefide += 0.5;
                       //}
                       totscore += 0.5; result = 0.5;
                     //
                     }
	            else result = 0.;
    	  }
	   }
       if (noppfide>0) arofide = arofide/noppfide; else arofide = 0.;
       if (player[i].ratingfide>0) we_w = totscorefide - totexp; else we_w = 0.;
       if (player[i].ratingfide==0 && totscorefide/noppfide<=0.5) rp = roundfloat (arofide + p_dp[ roundfloat(100.* totscorefide /noppfide) ] );
       else if (player[i].ratingfide==0 && totscorefide/noppfide>0.5) rp = roundfloat ( arofide + 25*(totscorefide-noppfide/2.) );
       else rp = roundfloat(arofide + p_dp[ roundfloat(100.* totscorefide /noppfide) ] );
       save << NFormat("  %3d  %3s  %-20.20s  %8d  %3s  %4d  ",
	                i, player[i].codetitle, player[i].name,
		            player[i].idfide, player[i].country, player[i].ratingfide);
       save << NFormat("%2d  %4.1f  %4d  %5.2f  %4d\n",
		            noppfide, totscorefide, roundfloat(arofide), we_w, rp);
	}
  }
  save<<"\n Generated by Vega - www.vegachess.com\n";
  save.Close();
}


void RoundData::FideEloVariationHTML()
{ int i, j, idopp, noppfide, index, rp;
  String stringa;
  double tot, totscore, totscorefide, we_w, result, arofide, totexp;
  double arrp[N_ROUND_MAX_RR+2][2];
  int *roundRes;
  int *playerOpp;
  extern int p_dp[];
  extern int roundfloat( double per);
  FileOut savehtml(PATHDIRWWW + "felovar.html");
  savehtml << TD.HTMLCommonHeader();
  savehtml << HTMLNavBar();

  savehtml << "<h2>" << NAMETORNEO << "</h2>\n";
  savehtml << "<h2>" << PLACETORNEO << " - " <<  DATATORNEO_B <<", "<<DATATORNEO_E  << "</h2>\n";
  savehtml << "<h3> Rating variation at round "<< currentRound<< " </h3>\n";

  savehtml << "<table bgcolor=#efefef cellpadding=4 cellspacing=0 border=0> \n";
  savehtml << "<tbody>\n";
  savehtml << "<tr> <th>ID</th> <th>Cat</th> <th width=\"200\">NAME</th> <th>FIDE ID</th> <th>Fed</th> <th>Rat</th>  <th>Ng</th> <th>Score</th> <th>ARO</th> <th>W-We</th> <th>Rp</th> </tr>";

  if (IS_SWISS) {
     playerOpp = &playerOpponent[0][0];
     roundRes = &roundResult[0][0];
  }
  else {
     playerOpp = &playerOpponentRR[0][0];
     roundRes = &roundResultRR[0][0];
  }

  for (i=1; i<=NPlayer; i++)
  { tot = totexp = 0.;
    arofide = 0.;
    noppfide = 0;
    totscore = totscorefide = 0.; //score without to consider the forfait

    // preliminary test to calculate noppfide (only fide opponent)
    for(j=1; j<=currentRound; j++)
       { if (PLAY_SYSTEM != ROUND_ROBIN && PLAY_SYSTEM != ROUND_ROBIN2) index = i*(N_ROUND_MAX+1) + j;
	     else index = i*(N_ROUND_MAX_RR+1) + j;
	     idopp = playerOpp[index];
         if ( idopp!=0 && (roundRes[index]<3) ) { // not forfait
             if (player[idopp].ratingfide > 0 )  noppfide++;
             //nopp++;
         }
       }

    // elaboration
    if ( ((player[i].ratingfide>0) && (noppfide>0)) || ((player[i].ratingfide==0) && (noppfide>=3)) )
    {
       calcelo(i, arrp, currentRound);
       for(j=1; j<=currentRound; j++)
	   { if (IS_SWISS) index = i*(N_ROUND_MAX+1) + j;
	     else index = i*(N_ROUND_MAX_RR+1) + j;

	     idopp = playerOpp[index];
         if (   idopp!=0 && (player[idopp].ratingfide>0) && (roundRes[index]<3)  ) { // not forfait
	            totexp += arrp[j][1];
                arofide += player[idopp].ratingfide;
	            if (roundRes[index]==1)
                     { totscorefide += 1.0;
                       totscore += 1.0; result = 1.;
                     }
	            else if (roundRes[index]==2)
	                 { totscorefide += 0.5;
                       totscore += 0.5; result = 0.5;
                     }
	            else result = 0.;
    	  }
	   }
       if (noppfide>0) arofide = arofide/noppfide; else arofide = 0.;
       if (player[i].ratingfide>0) we_w = totscorefide - totexp; else we_w = 0.;
       if (player[i].ratingfide==0 && totscorefide/noppfide<=0.5) rp = roundfloat (arofide + p_dp[ roundfloat(100.* totscorefide /noppfide) ] );
       else if (player[i].ratingfide==0 && totscorefide/noppfide>0.5) rp = roundfloat ( arofide + 25*(totscorefide-noppfide/2.) );
       else rp = roundfloat(arofide + p_dp[ roundfloat(100.* totscorefide /noppfide) ] );
       stringa = NFormat("<td>%3d</td> <td>%3s</td> <td>%-20.20s</td> <td>%8d</td> <td>%3s</td> <td>%4d</td>",
	                i, player[i].codetitle, player[i].name,
		            player[i].idfide, player[i].country, player[i].ratingfide);
       stringa << NFormat("<td>%2d</td> <td align=center>%4.1f</td> <td>%4d</td> <td align=center>%5.2f</td> <td>%4d</td>",
	                noppfide, totscorefide, roundfloat(arofide), we_w, rp);
        if (i&1) savehtml <<"<tr bgcolor=#e2e2f8> ";
        else savehtml << "<tr bgcolor=#ffffff> ";
        savehtml << stringa;
        savehtml << "</tr>\n";
	}
  }
  savehtml << "</tbody>\n";
  savehtml << "</table>\n";
  savehtml << "<br> <a href=\"http://www.vegachess.com\"> Generated by Vega </a>\n";
  savehtml << "</DIV></body>\n</html>\n";
  savehtml.Close();
}


void RoundData::FidePlayerCard()
{ int i, j, idopp, noppfide, index;
  String stringa, color;
  double tot, totscore, totscorefide, result, arofide, totexp;
  double arrp[N_ROUND_MAX_RR+2][2];
  int *roundRes;
  int *playerCol;
  int *playerOpp;
  extern int p_dp[];
  extern int roundfloat( double per);

  FileOut save( PATHDIR + "fidecard.txt");

  if (IS_SWISS) {
     playerOpp = &playerOpponent[0][0];
     roundRes = &roundResult[0][0];
     playerCol = &playerColor[0][0];
  }
  else {
     playerOpp = &playerOpponentRR[0][0];
     roundRes = &roundResultRR[0][0];
     playerCol = &playerColorRR[0][0];
  }

  for (i=1; i<=NPlayer; i++)
  { tot = totexp = 0.;
    arofide = 0.;
    noppfide = 0;
    totscore = totscorefide = 0.; //score without to consider the forfait

    // preliminary test to calculate and noppfide (only fide opponent)
    for(j=1; j<=currentRound; j++)
       { if (IS_SWISS) index = i*(N_ROUND_MAX+1) + j;
	     else index = i*(N_ROUND_MAX_RR+1) + j;
	     idopp = playerOpp[index];
         if ( idopp!=0 && (roundRes[index]<3) ) { // not forfait
             if (player[idopp].ratingfide > 0 )  noppfide++;
         }
       }

    // elaboration
    if ( ((player[i].ratingfide>0) && (noppfide>0)) || ((player[i].ratingfide==0) && (noppfide>=3)) )
    {  save <<"\n  Name: " << player[i].name << ",   Startingrank-Number = " <<  i <<"\n";
       save <<"  Fide ID: " << player[i].idfide << ", Rating: " << player[i].ratingfide << ", Title: " << player[i].codetitle;
       save<< ", Birthdate: " << player[i].data << ",  Fed: "<<player[i].country << ",  Sex: " << player[i].sex<<"\n";
       save<< "  Event: " << NAMETORNEO << ", Place: "<<PLACETORNEO<<" ("<<HOSTFEDERATION;
       save<< "), Start:" <<  DATATORNEO_B <<", Finish: "<<DATATORNEO_E << "\n";
	   save <<" -------------------------------------------------------------------------------------------\n";
	   save<<"  Rnd  Col N_Opp   Opponent Name          FIDE ID   Fed  Rtg  Title  Diff exp.  res  total\n";
       save <<" -------------------------------------------------------------------------------------------\n";

       calcelo(i, arrp, currentRound);
       for(j=1; j<=currentRound; j++)
	   { if (PLAY_SYSTEM != ROUND_ROBIN && PLAY_SYSTEM != ROUND_ROBIN2) index = i*(N_ROUND_MAX+1) + j;
	     else index = i*(N_ROUND_MAX_RR+1) + j;

	     idopp = playerOpp[index];
         if (   idopp!=0 && (player[idopp].ratingfide>0) && (roundRes[index]<3) ) { // not forfait and fide player
	            totexp += arrp[j][1];
                // if (player[idopp].ratingfide>0) arofide += player[idopp].ratingfide;
                arofide += player[idopp].ratingfide;
	            if (roundRes[index]==1)
                     { //if (player[idopp].ratingfide>0)  {
                       totscorefide += 1.0;
                       //}
                       totscore += 1.0; result = 1.;
                     }
	            else if (roundRes[index]==2)
	                 { //if (player[idopp].ratingfide>0) {
                       totscorefide += 0.5;
                       //}
                       totscore += 0.5; result = 0.5;
                     }
	            else result = 0.;

	            if (playerCol[index] == WHITE_COLOR) color = "W ";
                else  if (playerCol[index] == BLACK_COLOR) color = " B";
                stringa = NFormat("  %2d   %2s   %3d    %-20.20s  %8d   %3s  %4d  ",
	                j, color, idopp,
		            player[idopp].name,
		            player[idopp].idfide,
		            player[idopp].country,
		            player[idopp].ratingfide);
               stringa << NFormat("%3s  %4d  %4.2f  %3.1f  %4.1f\n",
		            player[idopp].codetitle,
		            (int) arrp[j][0],   // diff
		            arrp[j][1],         // expected score
		            result, totscore);
           if ( !(player[i].ratingfide==0 && player[idopp].ratingfide==0) ) {save<<stringa;}
    	  }
	   }
	   save <<" -------------------------------------------------------------------------------------------\n";
	   save <<"  Number of rated opponents: "<<noppfide<<",  Score vs. rated opponents: "<<totscorefide<<"\n";
       if (noppfide>0) arofide = arofide/noppfide;
	   save <<"  Average rating of rated opponents: "<< roundfloat(arofide);//<<"\n";
	   save <<" , Performance rating: "<< roundfloat(arofide + p_dp[ roundfloat(100.* totscorefide /noppfide) ] )<<"\n";
	   if (player[i].ratingfide>0) stringa = NFormat("  We = %5.2f,   W-We: %5.2f,    Chief Arbiter: %s", totexp, totscorefide - totexp, ARBITER);
       else if (player[i].ratingfide==0 && totscorefide/noppfide<=0.5) stringa = NFormat ("  Rp = %4d,    Chief Arbiter: %s",
             roundfloat (arofide + p_dp[ roundfloat(100.* totscorefide /noppfide) ] ) , ARBITER);
       else if (player[i].ratingfide==0 && totscorefide/noppfide>0.5) stringa = NFormat("  Rp = %4d,    Chief Arbiter: %s",
             roundfloat ( arofide + 25*(totscorefide-noppfide/2.) ) , ARBITER);
       save << stringa;
	   save <<"\n ================================================================================================\n";
	}
  }
  save<<"\n Generated by Vega - www.vegachess.com\n";
  save.Close();
}


//
// find international title norm
//
void RoundData::FindNorm(void)
{ int i, arr[N_PLAYER_MAX][4], usedrating[N_ROUND_MAX_RR+1];
  String stringa;
  extern int rat_flor[];
  extern int rat_norm[];

  if (currentRound<3) return; //at least 3 round

  FileOut savenorm(PATHDIR + "fidenorm.txt");
  savenorm << "\n\n   Performance Rating at round " << currentRound << " for norm achievement\n";
  savenorm << "   (current FIDE limits: GM=2601, IM=2451, WGM=2401, WIM=2251)\n\n";
  savenorm<<"    ID  Name                  Cat  Fed  Rtg   |   GM    IM  |  WGM    WIM\n";
  savenorm <<"   ------------------------------------------------------------------------------\n";

  for (i=1; i<=NPlayer; i++) { // he's male
      arr[i][GM] = arr[i][IM] = arr[i][WGM] = arr[i][WIM] = 0;
      if (player[i].sex == "m" || player[i].sex == "M") {
         if (player[i].title < 16) { //he is less than IM
             arr[i][GM] = CalculatePlayerNorm( i, GM,usedrating);
             arr[i][IM] = CalculatePlayerNorm( i, IM,usedrating);
         }
         else if (player[i].title==16) { //he's IM
             arr[i][GM] = CalculatePlayerNorm( i, GM,usedrating);
             }
      }
      else { // female
         if (player[i].title < 14) { //she is less than WIM
             arr[i][GM] = CalculatePlayerNorm( i, GM,usedrating);
             arr[i][IM] = CalculatePlayerNorm( i, IM,usedrating);
             arr[i][WGM] = CalculatePlayerNorm( i, WGM,usedrating);
             arr[i][WIM] = CalculatePlayerNorm( i, WIM,usedrating);
             }
         else if (player[i].title==14) { //she's WIM
             arr[i][WGM] = CalculatePlayerNorm( i, WGM,usedrating);
             arr[i][GM] = CalculatePlayerNorm( i, GM,usedrating);
             arr[i][IM] = CalculatePlayerNorm( i, IM,usedrating);
             }
         else if (player[i].title==16) { //she's IM
             arr[i][GM] = CalculatePlayerNorm( i, GM,usedrating);
             }
         else if (player[i].title==18) { //she's wGM
             arr[i][GM] = CalculatePlayerNorm( i, GM,usedrating);
             arr[i][IM] = CalculatePlayerNorm( i, WGM,usedrating);
             }

      }
      if (player[i].title < 20) { // not GM
        savenorm << NFormat("   %3d  %-20.20s  %3.3s  %3.3s  %4d  |  %4d  %4d |  %4d  %4d\n",
           i, player[i].name, player[i].codetitle, player[i].country, player[i].ratingfide,
           arr[i][GM], arr[i][IM], arr[i][WGM], arr[i][WIM]);
      }
  }

  savenorm << "\n    The following players are performing international norms:\n";
  savenorm << "   ----------------------------------------------------------\n";
  for (i=1; i<=NPlayer; i++) { // he's male
      if (player[i].sex == "m" || player[i].sex == "M") {
         if (player[i].title < 16) { //he is less than IM
             if (arr[i][GM]>=rat_norm[GM] && arr[i][IM] >=rat_norm[IM]) {
                  savenorm << NFormat("   %3d  %-20.20s  %3.3s  %3.3s: GM and IM norms\n",
                          i, player[i].name, player[i].codetitle, player[i].country);
             }
             else if (arr[i][GM]>=rat_norm[GM]) {
                  savenorm << NFormat("   %3d  %-20.20s  %3.3s  %3.3s: GM norm\n",
                          i, player[i].name, player[i].codetitle, player[i].country);
             }
             else if (arr[i][IM]>=rat_norm[IM]) {
                  savenorm << NFormat("   %3d  %-20.20s  %3.3s  %3.3s: IM norm\n",
                          i, player[i].name, player[i].codetitle, player[i].country);
             }

         }
         else if (player[i].title==16) { //he's IM
             if (arr[i][GM]>=rat_norm[GM]) {
                  savenorm << NFormat("   %3d  %-20.20s  %3.3s  %3.3s: GM norm\n",
                          i, player[i].name, player[i].codetitle, player[i].country);
             }
        }
      }
      else { // female
        if (player[i].title < 14) { //she is less than WIM
             if (arr[i][WGM]>=rat_norm[WGM] && arr[i][WIM] >=rat_norm[WIM]) {
                  savenorm << NFormat("   %3d  %-20.20s  %3.3s  %3.3s: WGM and WIM norms\n",
                          i, player[i].name, player[i].codetitle, player[i].country);
             }
             else if (arr[i][WGM]>=rat_norm[WGM]) {
                  savenorm << NFormat("   %3d  %-20.20s  %3.3s  %3.3s: WGM norm\n",
                          i, player[i].name, player[i].codetitle, player[i].country);
             }
             else if (arr[i][WIM]>=rat_norm[WIM]) {
                  savenorm << NFormat("   %3d  %-20.20s  %3.3s  %3.3s: WIM norm\n",
                          i, player[i].name, player[i].codetitle, player[i].country);
             }
         }
         else if (player[i].title==14) { //she's WIM
             if (arr[i][WGM]>=rat_norm[WGM]) {
                  savenorm << NFormat("   %3d  %-20.20s  %3.3s  %3.3s: WGM norm\n",
                          i, player[i].name, player[i].codetitle, player[i].country);
             }
         }
      }
  }
  savenorm <<"\n Generated by Vega - www.vegachess.com\n";
  savenorm.Close();
}


int RoundData::CalculatePlayerNorm(int i, int typenorm, int arrrating[])
{ int j, idopp, nopp, noppfide, index;
  int id_min[2], rat, rating;
  float totscore, aro, per;
  int *roundRes;
  int *playerOpp;
  extern int p_dp[];
  extern int roundfloat( double per);
  extern int rat_flor[];
  extern int rat_norm[];

  if (IS_SWISS) {
     playerOpp = &playerOpponent[0][0];
     roundRes = &roundResult[0][0];
  }
  else {
     playerOpp = &playerOpponentRR[0][0];
     roundRes = &roundResultRR[0][0];
  }
    aro = 0.;
    nopp = noppfide = 0;
    totscore = 0.; //score without to consider the forfait
    // preliminary calculation to calculate nopp (total)  and noppfide (only fide opponent)
    for(j=1; j<=currentRound; j++)
       { if (IS_SWISS) index = i*(N_ROUND_MAX+1) + j;
	     else index = i*(N_ROUND_MAX_RR+1) + j;
	     idopp = playerOpp[index];
         if ( idopp!=0 && (roundRes[index]<3) ) { // not forfait
             if (player[idopp].ratingfide > 0 )  noppfide++;
             nopp++;
         }
       }
    // we have to calculate the % of unrated player: if < 33% return 0
    // calculate the first lowest rating
      id_min[0] = 0;
      rat = 10000;
      for(j=1; j<=currentRound; j++)
       { if (IS_SWISS) index = i*(N_ROUND_MAX+1) + j;
	     else index = i*(N_ROUND_MAX_RR+1) + j;
	     idopp = playerOpp[index];
         if ( idopp!=0 && (roundRes[index]<3) ) { // not forfait
             if (player[idopp].ratingfide < rat_flor[typenorm] && player[idopp].ratingfide < rat) {
                 rat = player[idopp].ratingfide;
                 id_min[0] = idopp;
             }
         }
       }
    // calculate the second lowest rating
      id_min[1] = 0;
      rat = 10000;
      for(j=1; j<=currentRound; j++)
       { if (IS_SWISS) index = i*(N_ROUND_MAX+1) + j;
	     else index = i*(N_ROUND_MAX_RR+1) + j;
	     idopp = playerOpp[index];
         if ( idopp!=0 && (roundRes[index]<3) ) { // not forfait
             if (player[idopp].ratingfide < rat_flor[typenorm] && player[idopp].ratingfide < rat
                 && idopp!=id_min[0] ) {
                 rat = player[idopp].ratingfide;
                 id_min[1] = idopp;
             }
         }
       }

    // elaboration
    if ( ((player[i].ratingfide>0) && (nopp>0)) || ((player[i].ratingfide==0) && (noppfide>=3)) )
    {  for(j=1; j<=currentRound; j++)
	   { if (IS_SWISS) index = i*(N_ROUND_MAX+1) + j;
	     else index = i*(N_ROUND_MAX_RR+1) + j;

	     idopp = playerOpp[index];
         if ( idopp!=0 && (roundRes[index]<3) ) { // not forfait
                rating = player[idopp].ratingfide;
                if (rating == 0) rating = 1600; // arise to rating floor
                if (idopp==id_min[0]) { rating = rat_flor[typenorm]; }
                else if (idopp==id_min[1]) { rating = rat_flor[typenorm]; }
                aro += rating;
                arrrating[j] = rating;
	            if (roundRes[index]==1 || roundRes[index]==3) totscore += 1.0;
	            else if (roundRes[index]==2 || roundRes[index]==6) totscore += 0.5;
    	  }
	   }
       per = aro/nopp + p_dp[ roundfloat(100.*totscore/nopp) ];
       if (per<=1000.) per=0.;
       if (per>4000.) per=9999.;
	}
  arrrating[0] = roundfloat(per); // return the performance
  return roundfloat(per);
}


void RoundData::calceloFIDE (int idp, double arrp[][2], int round)
{ int d, k;
  int idopp;
  extern double diff[];
  for(k=1; k<=round; k++)
     {  //the rating is not calculated for game finished with forfait, code 3, 4, 2, and 6
        if (IS_SWISS) {
	       idopp = playerOpponent[idp][k];
           if ( idopp>0 && roundResult[idp][k]<3 && player[idopp].ratingfide>0)  // no forfeit and rated FIDE opponent
             {
               d = abs(player[idp].ratingfide - player[idopp].ratingfide);
	           if ( d > 350) d = 350;
	           arrp[k][0]= (float) d;
	           if (player[idp].ratingfide >= player[idopp].ratingfide) arrp[k][1] = diff[ d ];
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
           d = abs(player[idp].ratingfide - player[idopp].ratingfide);
	       if ( d > 350) d = 350;
	       arrp[k][0]= (float) d;
	       if (player[idp].ratingfide >= player[idopp].ratingfide) arrp[k][1] = diff[ d ];
	       else arrp[k][1] = 1.0 - diff[ d ];
	      }
           else {
            arrp[k][0] = 0.; // rating difference
            arrp[k][1] = 0.; // expected result
           }
    }
  }
}




//////////////////////////////// FIDE rating report ////////////////////////////////////

FIDERatingReport::FIDERatingReport()
{   int i, ratedplayers=0;
	String str;
	CtrlLayout(*this, "");

	edit0 <<= TD.NAMETORNEO;
	edit1 <<= TD.PLACETORNEO;
	edit2 <<= TD.HOSTFEDERATION;
	edit3 <<= TD.DATATORNEO_B;
	edit4 <<= TD.DATATORNEO_E;   // MObjectSetPopupHelp ( _edit000[4], "dd.mm.yyyy", 0, 10000);
	edit5 <<= AsString(TD.NPlayer);
       edit5.Disable();

    for (i=1; i<=TD.NPlayer; i++) if (player[i].ratingfide>0) ratedplayers++;
	edit6 <<= AsString(ratedplayers);
      edit6.Disable();

	if (TD.PLAY_SYSTEM==ROUND_ROBIN) str = "Individual: Round Robin";
	else if (TD.PLAY_SYSTEM==ROUND_ROBIN2) str = "Individual: Double Round Robin";
	else if (TD.PLAY_SYSTEM==USCF) str = "Individual: Swiss-System (USCF)";
	else if (TD.PLAY_SYSTEM==DUBOV) str = "Individual: Swiss-System (Dubov)";
	else if (TD.PLAY_SYSTEM==LIM) str = "Individual: Swiss-System (Lim)";
	else str = "Individual: Swiss-System";
	edit7 <<= str;	//MObjectSetPopupHelp ( _edit000[7], "specify the pairing system", 0, 10000);

	edit10 <<= "moves/time, increment";
    //MObjectSetPopupHelp ( _edit000[10], "specify the rate of play", 0, 10000);
    edit8 <<= TD.ARBITER;
    edit9 <<= "";

	btnClose <<= THISBACK(Close);
	btnDone <<= THISBACK(DoneCB);
}


void FIDERatingReport::DoneCB()
{ String stringa, string, color;
  int *roundRes;
  int *playerCol;
  int *playerOpp;
  int arr[N_PLAYER_MAX+1];
  int i, j, id, index;
  int id_to_pos[N_PLAYER_MAX+1];
  extern int  CompareScoreName(const void *pid1, const void *pid2);
  FileOut out( TD.PATHDIR + "fideelo.txt");

  for (i=0; i<TD.NPlayer; i++)
      { arr[i] = i+1; // set the id of each player in the array to be sort
      }
  qsort(arr, TD.NPlayer, sizeof(int), CompareScoreName);
  for (i=0; i<TD.NPlayer; i++) // 'i' is the ranking position after we performed the sort
      { id = arr[i];
        id_to_pos[id] = i;  // new id after the sort
      }

  out << NFormat("%02d2 %s\n", 1, AsString(~edit0));
  out << NFormat("%02d2 %s\n", 2, AsString(~edit1));
  out << NFormat("%02d2 %s\n", 3, AsString(~edit2));
  out << NFormat("%02d2 %s\n", 4, AsString(~edit3));
  out << NFormat("%02d2 %s\n", 5, AsString(~edit4));
  out << NFormat("%02d2 %s\n", 6, AsString(~edit5));
  out << NFormat("%02d2 %s\n", 7, AsString(~edit6));

  out << "082 0\n";  //team data = 0 for this version of the program

  out << NFormat("%02d2 %s\n",  9, AsString(~edit7));
  out << NFormat("%02d2 %s\n", 10, AsString(~edit8));
  out << NFormat("%02d2 %s\n", 11, AsString(~edit9));
  out << NFormat("%02d2 %s\n", 12, AsString(~edit10));

  if (TD.IS_SWISS) {
     playerOpp = &TD.playerOpponent[0][0];
     roundRes = &TD.roundResult[0][0];
     playerCol = &TD.playerColor[0][0];
  }
  else { playerOpp = &TD.playerOpponentRR[0][0];
         roundRes = &TD.roundResultRR[0][0];
         playerCol = &TD.playerColorRR[0][0];
       }

  for (i=1; i<=TD.NPlayer; i++)
      { String sex = "w", title = " ";
        if (player[i].sex == "m" || player[i].sex == "M") sex = "m";
        if (player[i].title == 20 || player[i].title == 18) title = "g";
        else if (player[i].title == 16 || player[i].title == 14) title = "m";
        else if (player[i].title == 12 || player[i].title == 10) title = "f";
        out << NFormat("001 %4d %s  %s", i, sex, title);
        out << NFormat(" %-33.33s %4d %3.3s %11d %-10.10s %4.1f %4d",
	        player[i].name,
            player[i].ratingfide,
	        player[i].country,
            player[i].idfide,
            player[i].data,
	        player[i].tiebreak[SCORE],
	        id_to_pos[ i ] + 1);  // rank position

	  for(j=1; j<=TD.currentRound; j++)
	   { if (TD.IS_SWISS) index = i*(N_ROUND_MAX+1) + j;
	     else index = i*(N_ROUND_MAX_RR+1) + j;

         if (playerCol[index] == WHITE_COLOR) color = "w";
         else  if (playerCol[index] == BLACK_COLOR) color = "b";
         else color = "-";

         if (playerOpp[index]==0 && roundRes[index]==3) { //BYE: 1F - 0
        	                              out << "  0000 - +";
	     }
	     else if (playerOpp[index]==0 && roundRes[index]==4) { //BYE: 0 - 1F
        	                              out << "  0000 - -";
	     }
	     else if (playerOpp[index]==0 && (roundRes[index]==6 || roundRes[index]==8)) { //for BYE: draw
        	                              out << "  0000 - =";
	     }
	     else if (playerOpp[index]!=0 ) {
		     if (roundRes[index]==0)      out << NFormat("  %4d %s 0", playerOpp[index], color);
             else if (roundRes[index]==1) out << NFormat("  %4d %s 1", playerOpp[index], color);
	         else if (roundRes[index]==2) out << NFormat("  %4d %s =", playerOpp[index], color);
		     else if (roundRes[index]==4) out << NFormat("  %4d %s +", playerOpp[index], color);
             else if (roundRes[index]==3) out << NFormat("  %4d %s -", playerOpp[index], color);
	      }
          else    out << "          "; //default case: unplayed
	    }
        out << "\n";
      }
  out <<"\n Generated by Vega - www.vegachess.com\n";
  out.Close();
  Close(); //close the dialog
}

///////////////////////////////////////////////////////////////////////////////

FIDECertificate::FIDECertificate()
{
	CtrlLayout(*this, "Enter Player Data");
	editPlayer.Tip(t_("Enter the tournament ID"));
	editNorm.Add("GM");
	editNorm.Add("WGM");
	editNorm.Add("IM");
	editNorm.Add("WIM");
	editNorm <<= "GM";
	btnClose <<= THISBACK(Close);
	btnDone <<= THISBACK(DoneCB);
}

void FIDECertificate::DoneCB()
{ String e1, stringa, rateplay, placebirth, countrylist[N_PLAYER_MAX+1], title, usedrat;
  int typenorm=0, index, idopp, aro=0, ctitle[6], titopp=0, ratopp=0;
  int playernotapplicant=0, playernothostfed=0, titoppnothost=0;
  int playerhostfed=0, countcountry=0, l, found, var, required;
  int totcountcountry=0, tottitoppnothost=0, totratoppnothost=0;
  int i, j, usedrating[N_ROUND_MAX_RR+1], countrnd=0;
  int *roundRes;
  int *playerOpp;
  double result, totscore=0., requiredpoints;
extern int roundfloat( double per);
extern int rat_flor[], rat_norm[], p_dp[];

  if (TD.IS_SWISS) {
     playerOpp = &TD.playerOpponent[0][0];
     roundRes = &TD.roundResult[0][0];
  }
  else {
     playerOpp = &TD.playerOpponentRR[0][0];
     roundRes = &TD.roundResultRR[0][0];
  }

  i = StrIntValue(AsString( ~editPlayer));
  if (i<=0 || i>TD.NPlayer) {
      Exclamation("Enter a valid ID for the Player");
      return;
  }

  e1 = AsString(~editNorm);
  if ( e1 == "GM") typenorm = 0;
  else if ( e1 == "IM") typenorm = 1;
  else if ( e1 == "WGM") typenorm = 2;
  else if ( e1 == "WIM") typenorm = 3;

  placebirth = AsString(~editBirth);
  rateplay = AsString(~editRateofplay);

  TD.CalculatePlayerNorm(i, typenorm, usedrating); // get the rating

  stringa = NFormat("%sfidenormID=%das%s.txt", TD.PATHDIR, i, e1);
 // FileOut savenorm(PATHDIR + "fidenormID=" + AsString(i) + e1 + ".txt");
  FileOut savenorm(stringa);

  // count titled and rated player
  for(j=0; j<=5; j++) ctitle[j]=0;
  for(j=1; j<=TD.currentRound; j++)
       { if (TD.IS_SWISS) index = i*(N_ROUND_MAX+1) + j;
	     else index = i*(N_ROUND_MAX_RR+1) + j;
	     idopp = playerOpp[index];
	     if ( player[idopp].country != player[i].country) playernotapplicant++;
	     if ( player[idopp].country != TD.HOSTFEDERATION) playerhostfed++;
	     if ( player[idopp].country != player[i].country) playernothostfed++;
         if ( player[idopp].ratingfide>0) ratopp++;
	     if ( player[idopp].title>=14 && player[idopp].country != TD.HOSTFEDERATION) titoppnothost++;
	     if (player[idopp].title>=14) titopp++;
         switch (player[idopp].title)
	       { case 20: ctitle[0]++;
                      break;
             case 16: ctitle[1]++;
                      break;
             case 18: ctitle[2]++;
                      break;
             case 14: ctitle[3]++;
                      break;
             case 12: ctitle[4]++;
                      break;
             case 10: ctitle[5]++;
                      break;
	       }
	       // count federations
	       l=0; found=0;
	       while (l<=countcountry && !found) {
	          if ( player[idopp].country == countrylist[l]) {
                   found = 1;
                   break;
              }
              else l++;
	       }
           if (!found) {
               countcountry++;
               countrylist[countcountry] = player[idopp].country;
           }
	   }

  for(idopp=1; idopp<=TD.NPlayer; idopp++)
       { if (player[idopp].ratingfide>0 && player[idopp].country != TD.HOSTFEDERATION) totratoppnothost++;
         if (player[idopp].title>=14 && player[idopp].country != TD.HOSTFEDERATION) tottitoppnothost++;
         // count federations
	     l=0; found=0;
	     while (l<=totcountcountry && !found) {
	          if ( player[idopp].country != countrylist[l] ) {
                   found = 1;
                   break;
              }
              else l++;
	       }
           if (!found) {
               totcountcountry++;
               countrylist[totcountcountry] != player[idopp].country;
           }
	   }


  savenorm << "   CERTIFICATE OF TITLE RESULT " << e1 << "                  IT1\n\n";
  savenorm << "   Name (last name, first name): " << player[i].name << ", Sex: "<<player[i].sex<<", FIDE code: " << player[i].idfide << "\n";
  savenorm << "   date of birth: "<< player[i].data << ", place of birth: "<< placebirth << ", Federation: " << player[i].country<<"\n\n" ;
  savenorm << "   Event: " << TD.NAMETORNEO << ", City:"<<TD.PLACETORNEO<<", Start: "<< TD.DATATORNEO_B << ",  Close: " << TD.DATATORNEO_E << "\n" ;
  savenorm << "   City: "<<TD.PLACETORNEO<<", Country: "<< TD.HOSTFEDERATION << "\n" ;
  savenorm << "   Chief or supervising arbiter: " << TD.ARBITER << ", Number of games: "<< TD.currentRound << "\n" ;
  savenorm << "   Rate of play: " << rateplay << ", Number of Federations: " << countcountry << "\n" ;
  savenorm << "   Number of players NOT from the applicant's Federation: " << playernotapplicant<<", Number rated opponents: "<<ratopp<<"\n" ;
  savenorm << "   Number of players from host Federation: " << playerhostfed << ", Total number titled opponents: "<<titopp<<"\n" ;
  savenorm << "   Number of GM " << ctitle[0] << ", IM " << ctitle[1] << ", WGM " << ctitle[2]<<", WIM " << ctitle[3] << ", FM "<<ctitle[4]<<", WFM "<<ctitle[5]<<"\n";

  savenorm<<"\n  ---------------------------------------------------------------------------------\n";
  savenorm << "   Where applying 1.43e         Number of federation: " << totcountcountry << "\n";
  savenorm << "   Number of rated players NOT from host federation "<<totratoppnothost<<"\n";
  savenorm << "   Number of players NOT from host federation holding GM, IM, WGM, WIM titles "<< tottitoppnothost <<"\n";
  savenorm<<"\n  ---------------------------------------------------------------------------------\n";

  savenorm<<"  Special remarks __________________________________________________________\n";
  savenorm<<"\n  -------------------------------------------------------------------------\n";
  savenorm<<"   Rnd  Name of Opponents      FIDE ID   Fed  actual  rating  Title  res\n";
  savenorm<<"                                              rating  by 1.46c\n";
  savenorm<<"  -------------------------------------------------------------------------\n";
  for(j=1; j<=TD.currentRound; j++)
	   { if (TD.IS_SWISS) index = i*(N_ROUND_MAX+1) + j;
	     else index = i*(N_ROUND_MAX_RR+1) + j;

	     idopp = playerOpp[index];
         if ( idopp!=0 && (roundRes[index]<3)  ) { // not forfait
              countrnd++;
              if (roundRes[index]==1 || roundRes[index]==3)
                     { totscore += 1.0; result = 1; }
	          else if (roundRes[index]==2 || roundRes[index]==6)
	                 { totscore += 0.5; result = 0.5;}
	          else result = 0.;

              if (player[idopp].title<10) title = "   ";
              else title = player[idopp].codetitle;

              if (player[idopp].ratingfide==usedrating[j]) usedrat = "    ";
              else usedrat = NFormat("%4d", usedrating[j]);

              savenorm << NFormat("   %2d   %-20.20s  %8d   %3s   %4d    %s    %3s   %3.1f\n",
	                j,
 		            player[idopp].name,
		            player[idopp].idfide,
		            player[idopp].country,
		            player[idopp].ratingfide,
		            usedrat,
		            title,
		            result);
              aro += usedrating[j];
    	  }
	   }
  savenorm<<"  -------------------------------------------------------------------------\n";
  savenorm << NFormat(" Number of games: %2d                                         total: %4.1f\n",
             countrnd, totscore);

  //find required score
  var = rat_norm[typenorm] - roundfloat(aro*1./countrnd);
  l=0; while (l<100 && p_dp[l]<var) l++;
  requiredpoints = l/100.*countrnd;
  required = int (requiredpoints*10.);
  if (required%5!=0) required = required + (5 - required%5);
  requiredpoints = required/10.;

  savenorm << "   Norm sought: " << e1 << ", Rating Average (1.46c): " << roundfloat(aro*1./countrnd) << ", Score required: "<<requiredpoints<<", Score Achieved " << totscore<<"\n";
  savenorm << "   Exceding norm by "<< totscore-requiredpoints <<" (points)  Arbiter's Signature _____________________________________\n";
  savenorm << "   Federation confirming the result: "<<TD.HOSTFEDERATION<<", Name of federation official __________________________\n";
  savenorm << "   Signature of federation officer ____________________ date "<<TD.DATATORNEO_E<<"\n";

  savenorm.Close();

  if (totscore-requiredpoints<0) {
      stringa = NFormat("The selected player: ID=%d, %s\nhas NOT achieved the %s norm!\nPlease try another norm or use 'Find Norm' option",
	                i, player[i].name, e1);
      Exclamation(stringa);
  }
  // save the certificate in QTF format
  stringa = NFormat("%sfidenormID=%das%s.qtf", TD.PATHDIR, i, e1);
  FileOut savehtml(stringa);
  savehtml << "[ $$0,0#00000000000000000000000000000000:Default]\n";
  savehtml << "[{_}%EN-US\n";
  savehtml << "[s0;=l256;b42;a42;*R+150 &]\n";
  savehtml << "[s0;=l256;b42;a42;*R+150 CERTIFICATE OF TITLE RESULT " << e1 << "&]\n";
  savehtml << "[s0;l256;b42;a42;R Name (last name, first name): [* "<< TrimRight(player[i].name) <<" ], \n";
  savehtml << "FIDE code: [* "<< player[i].idfide <<"] &]\n";
  savehtml << "[s0;l256;b42;a42;*R [* date of birth: ]"<< player[i].data << "[* , Sex:] "<<player[i].sex<<"[* , Place of birth:\n";
  savehtml << "]"<< placebirth << "[* , Federation: ]" << player[i].country<<"&]\n";
  savehtml << "[s0;l256;b42;a42;*R [* Event: ]"<< TD.NAMETORNEO<< "[* , Start: ]"<< TD.DATATORNEO_B << "[* , Close: ]" << TD.DATATORNEO_E<<"&]\n";
  savehtml << "[s0;l256;b42;a42;*R [* City: ]"<< TD.PLACETORNEO<< "[* , Country: ]"<< TD.HOSTFEDERATION << "&]\n";
  savehtml << "[s0;l256;b42;a42;*R [* Chief or supervising arbiter: ]"<< TD.ARBITER<< "[* , Number\n";
  savehtml << "of games: ]"<< TD.currentRound << "&\n]";
  savehtml << "[s0;l256;b42;a42;*R [* Rate of play: ]"<< rateplay<< "[* , Number of Federations: ]"<< countcountry << "&]\n";
  savehtml << "[s0;l256;b42;a42;*R [* Number of players NOT from the applicant`'s\n";
  savehtml << "Federation: ]"<< playernotapplicant<< "[* , Number rated opponents: ]"<< ratopp << "&]\n";
  savehtml << "[s0;l256;b42;a42;*R [* Number of players from host Federation: ]"<< playerhostfed<< "[* , \n";
  savehtml << "Total number titled opponents: ]"<< titopp << "&]\n";
  savehtml << "[s0;l256;b42;a42;*R [* Number of GM: ]" << ctitle[0] << "[* , IM: ]" << ctitle[1] << "[* , WGM: ]" << ctitle[2]<<"[* , WIM: ]" << ctitle[3] << "[* ,\n";
  savehtml << "FM: ]"<<ctitle[4]<<"[* , WFM: ]"<<ctitle[5]<<"&]\n";

  savehtml << "[s0;l256;b42;a42;R `-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-\n";
  savehtml << "`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-\n";
  savehtml << "`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-&]\n";
  savehtml << "[s0;l256;b42;a42;*R [*  ][*_Where applying 1.43e][*  Number of federation: ]" << totcountcountry << " &]\n";
  savehtml << "[s0;l256;b42;a42;R Number of rated players NOT from host federation: [* " << totratoppnothost << "] &]\n";
  savehtml << "[s0;l256;b42;a42;*R [* Number of players NOT from host federation holding GM, IM, WGM, WIM titles: ]"<<tottitoppnothost<<" &]\n";
  savehtml << "[s0;l256;b42;a42;R `-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-\n";
  savehtml << "`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-\n";
  savehtml << "`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`-`- &]\n";
  savehtml << "[s0;l256;b42;a42;R Special remarks ..................................................\n";
  savehtml << "............................&]\n";

  // table
  savehtml << "[ {{721:3439:1519:634:970:1173:731:813<256;t/25b/25 [s0;=b42;a42;*R Rnd]\n";
  savehtml << ":: [s0;=b42;a42;*R Name of Opponents]\n"
              ":: [s0;=b42;a42;*R FIDE ID]\n"
              ":: [s0;=b42;a42;*R Fed]\n"
              ":: [s0;=b42;a42;*R actual rating]\n"
              ":: [s0;=b42;a42;*R rating by 1.46c]\n"
              ":: [s0;=b42;a42;*R Title]\n"
              ":: [s0;=b42;a42;*R result]\n";


  for(j=1; j<=TD.currentRound; j++)
	   { if (TD.IS_SWISS) index = i*(N_ROUND_MAX+1) + j;
	     else index = i*(N_ROUND_MAX_RR+1) + j;

	     idopp = playerOpp[index];
         if ( idopp!=0 && (roundRes[index]<3)  ) { // not forfait
              if (roundRes[index]==1 || roundRes[index]==3)
                     {result = 1; }
	          else if (roundRes[index]==2 || roundRes[index]==6)
	                 { result = 0.5;}
	          else result = 0.;

              if (player[idopp].title<10) title = "-";
              else title = player[idopp].codetitle;

              if (player[idopp].ratingfide==usedrating[j]) usedrat = "-";
              else usedrat = NFormat("%4d", usedrating[j]);

		  savehtml << ":: [s0;b42;a42;R2 "<<j <<"]\n"
			":: [s0;b42;a42;R2 "<< player[idopp].name<<" ]\n"
			":: [s0;=b42;a42;R2 "<<player[idopp].idfide<<"]\n"
			":: [s0;=b42;a42;R2 "<< player[idopp].country<<"]\n"
			":: [s0;=b42;a42;R2 "<<player[idopp].ratingfide<<"]\n"
			":: [s0;=b42;a42;R2 "<< usedrat <<"]\n"
			":: [s0;=b42;a42;R2 "<<title<<"]\n"
			":: [s0;=b42;a42;R2 "<<result<<"]\n";
          }
        }
  savehtml << "::-1 [s0;b42;a42;*R2 [* Number of games: ]"<< countrnd<<" ]\n"
	"::t/15b/15 [s0; ]\n"
	"::t/25b/25-3 [s0;b42;a42;R2 ]\n"
	"::t/15b/15-2 [s0; ]\n"
	"::-1 [s0; ]\n"
	":: [s0; ]\n"
	"::t/25b/25-1 [s0;b42;a42;*R2 [*     Total  ]"<<totscore<<"]\n"
	"::t/15b/15 [s0; ]}}&]\n";

  savehtml << "[s0;i256;b42;a42;*R [* Norm sought: ]" << e1 << "[* , Rating Average (1.46c): ]" << roundfloat(aro*1./countrnd) << "[* ,\n";
  savehtml << " Score required: ]"<<requiredpoints<<"[* , Score Achieved ]" << totscore<<"&]\n";
  savehtml << "[s0;i256;b42;a42;R Exceding norm by [* "<< totscore-requiredpoints <<"] (points), Arbiter`'s \n";
  savehtml << "Signature ......................................................&]\n";
  savehtml << "[s0;i256;b42;a42;R Federation confirming the result: [* "<<TD.HOSTFEDERATION<<"], Name of \n";
  savehtml << "federation official .....................................&]\n";
  savehtml << "[s0;i256;*R [* Signature of federation officer ........................................\n";
  savehtml << "........................ , date ]"<<TD.DATATORNEO_E<<"]\n";
  savehtml.Close();

  Exclamation("The FIDE certificate has been saved");
  Close();
}
