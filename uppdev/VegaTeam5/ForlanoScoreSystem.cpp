#include "VegaMain.h"
#include "Tournament.h"

/* Find shortest paths between nodes of a graph
  Input  l - adjecency matrix
              l[i,j] is the cost of a link between node i and node j.
              l[i,j] should be positive, except for
              -1 if no direct link, 0 for l[i,i] (no cost).
          n - number of nodes
  Output l - matrix of minimum distances between nodes
              l[i,j] is the length of the shortest path between i and j
          d - auxiliary matrix for shortest path reconstruction
*/

void RoundData::Floyd(int l[MAXDIM+1][MAXDIM+1], int d[MAXDIM+1][MAXDIM+1], int n)
{ int i,j,k;
  for (i=1; i<=n; i++)
    for (j=1; j<=n; j++)
      if ( l[i][j]<0 )  d[i][j]=0;
      else d[i][j]=-1;
  for (k=1; k<=n; k++)
    for (i=1; i<=n; i++)
    for (j=1; j<=n; j++)
        if ( (l[i][k]>=0) && (l[k][j]>=0) ) {
          if ( (l[i][j]<0) || (l[i][j]>l[i][k]+l[k][j]) ) {
            l[i][j]=l[i][k]+l[k][j];
            d[i][j]=k;
          }
	}
}

/* Helper procedure, do not call directly */
void RoundData::Path1(int d[MAXDIM+1][MAXDIM+1], int n, int i, int j)
{
    if (d[i][j]>0) {
      Path1(d,n,i,d[i][j]);
      countnodes++;
      nodes[countnodes] = d[i][j]; //stores the node
      Path1(d,n,d[i][j],j);
    }
}

/* Print shortest path from node i to node j.
  Input d - auxiliary matrix returned from Floyd
         n - number of nodes
         i - start of path
         j - end of path
  Note This procedure is somewhat differs from the actual
    Floyd-Warshall procedure. Theirs is a little faster and
    non recursive, but long to type in and very prone to
    typing errors.
*/

void RoundData::Path(int d[MAXDIM+1][MAXDIM+1], int n, int i, int j)
{
  if (d[i][j]==0) ; //printf("No path.\n");
  else if (i==j) ; // printf("Same node.\n");
  else {
   // printf("%d \n", i);
    Path1(d,n,i,j);
   // printf("%d \n", j);
  }
}

/* This is an usage example for the above procedures */
void RoundData::ForlanoScoreSystem(int m)
{ extern int  cmpMPSS(int id1, int id2); // missing point score system
  int a[MAXDIM+1][MAXDIM+1]; // the a matrix will be lost!
  int c[MAXDIM+1][MAXDIM+1]; // copy of a
  int arr[N_PLAYER_MAX+1];
  int points[MAXDIM+1][MAXDIM+1];
  int b[MAXDIM+1][MAXDIM+1];
  int x,y,r, idopp, NORM=0,  TOT=1, FORL=2, BYE=3, PENALTY=4;
  int k,i,j;
  float score[MAXDIM+1][5]; // contains 2 times the points made by the players

  if (!IS_CLOSED_REGISTRATION){
     Exclamation( t_("The tournament is not yet started!"));
     return; // no import player with closed registration
  }
  m = NPlayer;
  if (m>MAXDIM)  {
     Exclamation( t_("I'm sorry, but I can't process more than 200 players"));
     return; // no import player with closed registration
  }
  FileOut fp(PATHDIR + "mpscorelog.txt");

  /* reset the score matrix */
  for (y=0; y<=m; y++)
    for (x=0; x<=4; x++) {
      score[y][x] = 0.;
    }
  /* set all to no links, except that node i is connected to itself with cost 0*/
  for (y=0; y<=m; y++)
    for (x=0; x<=m; x++) {
      if (x==y) a[x][y] = b[x][y] = c[x][y] = 0;
      else a[x][y] = c[x][y] = -1;
      points[y][x] = 0;
  }

  // Now set the links according to the cross table
  for (y=1; y<=m; y++)
    for (r=1; r<=currentRound; r++) { // scan round
      idopp = playerOpponent[y][r];
      if (idopp!=0 && roundResult[y][r]==1) {
          a[y][idopp] = c[y][idopp] = 1; // direct link from y to idopp
          points[y][idopp] = 2;
      }
      else if (idopp!=0 && roundResult[y][r]==0) {
          a[y][idopp] = c[y][idopp] = -1; // no link direct link from y to idopp
          points[y][idopp] = 0;
      }
      else if (idopp!=0 && roundResult[y][r]==2) {
          a[y][idopp] = c[y][idopp] = 1; // draw
          points[y][idopp] = 1;
      }
      score[y][NORM] += points[y][idopp]; // sum all regular point
      // now consider the points made with the BYE
      if (idopp==0 && roundResult[y][r]==3) { // win to forfeit
          score[y][BYE] += 2.;
          score[y][PENALTY] += 2.;
      }
      else if (idopp==0 && roundResult[y][r]==6) { // draw to forfeit
          score[y][BYE] += 1.;
          score[y][PENALTY] += 2.;
      }
    }

 /* print the adiacensy matrix */
 fp << "adjacency matrix: -1=if no direct link between [i, j], 1=direct link [i,j], 0 for pair [i, i]\n";
 for (x=1; x<=m; x++) {
   for (y=1; y<=m; y++) fp << NFormat("%2d ", a[x][y]);
   fp << "\n";
  }
  fp << "\n";

  /* print the point matrix */
  fp <<"points matrix (cross table with real points: 2 for win and 1 for draw)";
  for (x=1; x<=m; x++) {
    for (y=1; y<=m; y++) fp << NFormat("%2d ", points[x][y]);
    fp << "\n";
  }
  fp << "\n";

  /* Find shortest paths */
  Floyd(a,b,m);

  /* Print shortest-path matrix */
/*  for (x=1; x<=m; x++)  {
    for (y=1; y<=m; y++) fprintf(fp,"%3d ", c[x][y]);
    fprintf(fp,"\n");
  }
  fprintf(fp,"\n");
*/

  /* calculates the shortest path for each pair not directly connected
     and assign a point to that match
  */
  for (x=1; x<=m; x++) {
   for (y=x+1; y<=m; y++) {
      if (x != y && c[x][y]==-1 && c[y][x]==-1) {
          int cnodes1, cnodes2;

          countnodes=0;
          Path(b,m,x,y);
          if (countnodes>0) {
             int iswinner = 0; // in the chain must be a win to get 2 points otherwise he gets 1 point (draw)
             nodes[0] = x;
             nodes[countnodes+1] = y;
             fp << NFormat(" match [%3d,%3d] via game(s): ", x, y);
             for (i=0; i<=countnodes; i++) {
                 fp << NFormat("(%3d -%3d), ", nodes[i], nodes[i+1]);
                 if (points[nodes[i]][nodes[i+1]] == 2) iswinner=1;
             }
             if (iswinner) { points[x][y] = 2; fp << "= 1-0\n"; }
             else { points[x][y] = 1; fp << "= 1/2-1/2\n"; }
             cnodes1 = countnodes;
          } else { fp << NFormat(" match [%3d,%3d] = no game\n", x, y);}

          countnodes=0;
          Path(b,m,y,x); // now check the reversed path and exchange x and y
          if (countnodes>0) {
             int iswinner = 0; // in the chain must be a win to get 2 points otherwise he gets 1 point (draw)
             nodes[0] = y;
             nodes[countnodes+1] = x;
             fp << NFormat(" match [%3d,%3d] via game(s): ", y, x);
             for (i=0; i<=countnodes; i++) {
                 fp << NFormat("(%3d -%3d), ", nodes[i], nodes[i+1]);
                 if (points[nodes[i]][nodes[i+1]] == 2) iswinner=1;
             }
             if (iswinner) { points[y][x] = 2; fp << "= 1-0\n";}
             else { points[y][x] = 1; fp << "= 1/2-1/2\n"; }
             cnodes2 = countnodes;
          } else { fp << NFormat(" match [%3d,%3d] = no game\n",y,x);}
          // check consistency: both i-j and j-i cannot have 2 point (prefer the smallest path
          //both have win
          fp << NFormat(" result [%3d,%3d] =", x, y);
          if (points[x][y]==2 && points[y][x]==2 && cnodes1==cnodes2) {
             points[x][y] = points[y][x] = 1;
             fp << " 1/2-1/2\n\n";
          }
          else if (points[x][y]==2 && points[y][x]==2 && cnodes1<cnodes2){
             points[x][y] = 2;
             points[y][x] = 0;
             fp << " 1-0\n\n";
          }
          else if (points[x][y]==2 && points[y][x]==2 && cnodes1>cnodes2){
             points[x][y] = 0;
             points[y][x] = 2;
             fp << " 0-1\n\n";
          }
          //x win and y draw
          else if (points[x][y]==2 && points[y][x]==1 && cnodes1<=cnodes2) {
             points[x][y] = 2;
             points[y][x] = 0;
             fp << " 1-0\n\n";
          }
          else if (points[x][y]==2 && points[y][x]==1 && cnodes1>cnodes2) {
             points[x][y] = 1;
             points[y][x] = 1;
             fp << " 1/2-1/2\n\n";
          }
          //x draw and y win
          else if (points[x][y]==1 && points[y][x]==2 && cnodes1>=cnodes2) {
             points[x][y] = 0;
             points[y][x] = 2;
             fp << " 0-1\n\n";
          }
          else if (points[x][y]==1 && points[y][x]==2 && cnodes1<=cnodes2) {
             points[x][y] = 1;
             points[y][x] = 1;
             fp << " 1/2-1/2\n\n";
          }
          //x win and y no game
          else if (points[x][y]==2 && points[y][x]==0) {
             fp << " 1-0\n\n";
          }
          //x no game and y win
          else if (points[x][y]==0 && points[y][x]==2) {
             fp << " 0-1\n\n";
          }
          else if (points[x][y]==1 && points[y][x]==1 && cnodes1==cnodes2) {
             fp << " 1/2-1/2\n\n";
          }
          else {
             fp << " not assigned\n\n";
             points[x][y] = 1;
             points[y][x] = 1;
             fp << " 1/2-1/2\n\n";
          }
     }
   }
  }

  /* calculate the new total and FORL score */
  for (x=1; x<=m; x++) {
    for (y=1; y<=m; y++) score[x][TOT] += points[x][y];
    score[x][TOT] += score[x][BYE];
    player[x].tiebreak[MPSS] = (float) (score[x][TOT]/2. - score[x][PENALTY]/2.);
    score[x][FORL] = score[x][TOT] - score[x][NORM] - score[x][BYE] - score[x][PENALTY];
  }

  /* print the new point matrix */
  fp << "new point matrix with 'missing point'\n";
  for (x=1; x<=m; x++) {
    fp << NFormat("%3d %-20.20s:", x, player[x].name);
    for (y=1; y<=m; y++) fp << NFormat("%2d", points[x][y]);
    fp << NFormat(": %4.1f + %4.1f + %4.1f - %4.1f = %4.1f\n",
            score[x][NORM]/2., score[x][BYE]/2., score[x][FORL]/2., score[x][PENALTY]/2., score[x][TOT]/2.);
  }
  fp << "\n\n";
  /* print a better summary */
  for (x=1; x<=m; x++) {
    fp << NFormat("%3d %-20.20s:", x, player[x].name);
    fp << NFormat(": %4.1f + %4.1f + %4.1f - %4.1f = %4.1f\n",
            score[x][NORM]/2., score[x][BYE]/2., score[x][FORL]/2., score[x][PENALTY]/2., score[x][TOT]/2.);
  }
  fp << "\n";
  fp.Close();



  fp.Open(PATHDIR + "rankmpscore.txt");

  fp << NFormat("Tournament: %s\n", NAMETORNEO);
  fp << NFormat("%s - %s, %s\n", PLACETORNEO, DATATORNEO_B, DATATORNEO_E);
  fp << NFormat("\nMissing Point Score System: position at round %d\n", currentRound);

  for (i=0; i<m; i++) arr[i] = i+1; // set the id of each player in the array to be sort
  //qsort(arr, m, sizeof(int), cmpMPSS);

  // sort the array
  for(i=0; i<=m-2; i++)
     { for (j=i+1; j<=m-1; j++)
       {  if ( cmpMPSS( arr[i], arr[j])>0)
             { k = arr[i]; arr[i] = arr[j]; arr[j] = k;}
       }
     }

  fp << "Pos  ID  NAME                    | Rtg  Prat Fed | Score |  Real   MP    BYE  Nogame\n";
  fp << "------------------------------------------------------------------------------------\n";
  for (k=0; k<m; k++)
      { i=arr[k];
        //if (player[i].isAvailable) fprintf(fp,"%-20.20s", player[i].name);
        //else fprintf(fp,"%-17.17s(W)", player[i].name);
        if (player[i].isAvailable != "0") {
         fp << NFormat("%3d %3d %3.3s %s | %4d %4d ",
	      k+1, //position
          i,
	      player[i].codetitle,
          player[i].name,
	      player[i].RATING,
	      (int) player[i].tiebreak[PER_RAT]);
         fp << NFormat("%3s | %4.1f  |  %4.1f  %4.1f  %4.1f  %4.1f\n",
          player[i].country,
          player[i].tiebreak[MPSS],
          player[i].tiebreak[SCORE],
          score[i][FORL]/2.,
          score[i][BYE]/2.,
          score[i][PENALTY]/2.);
       }
      }
  fp.Close();

  Exclamation(t_("Done Missing Point Score System"));

/*
  for (i=0; i<nplayers; i++) {
       arr[i] = i+1; // set the id of each player in the array to be sort
  }
  qsort(arr, nplayers, sizeof(int), CompareScoreName);

  for (i=0; i<nplayers; i++) // 'i' is the ranking position after we performed the sort
      { id = arr[i];
        id_to_pos[id] = i;  // new id after the sort
      }

  for (k=0; k<nplayers; k++) {
      { i=arr[k];
	       //    k+1, // k is the ranking position now
		   player[i].name,
      }
   for(j=1; j<=currentRound; j++)
	   { idopp = id_to_pos[ playerOpp[index] ] + 1;
	   }
*/
}
