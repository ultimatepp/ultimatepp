// here we have specific tournament data
#ifndef _Tournament_h_
#define _Tournament_h_

#define VERSION  "VegaTeam 5.0.1 U++ version"
#define N_ROUND_MAX 21
#define N_PLAYER_MAX 999
#define N_ROUND_MAX_RR 46
#define N_PLAYER_MAX_RR 24
#define N_COL 13
#define FUNC_CMP 12
#define MAXDIM  200  //  Max number of nodes in a graph
#define MAX_BOARDS 8 //  Max number of boards, i.e. players per team

#define N_PLAYER_MAX_TRUE 960  // N_PLAYER_MAX*8
#define N_PLAYER_MAX_TRUE_RR 192 // N_PLAYER_MAX_RR*8

enum{ DUBOV=1, ROUND_ROBIN, VEGA, USCF, ROUND_ROBIN2, BURST, LIM}; // avalaible play system
enum{ SCORE=0, ARO, SONN_BERG, BUC_CUT1, BUC_MED, BUC_TOT, CUMUL, ELO_VAR, MOST_BLACKS, MOST_WINS, PER_RAT, APRO, MPSS};
enum{ BLACK=0, WHITE};                     //useful in list operation
enum{ BLACK_COLOR=-1, NO_COLOR, WHITE_COLOR}; //useful in color allocation
enum {GM=0, IM, WGM, WIM};
enum {READY_FOR_PAIRING, READY_FOR_INSERTION}; //STATUSROUND values

////// class Tournament ////////////////
class TournamentStatus {
public:
 	String NAMETORNEO; // max 40 chars
 	String FILETORNEO; // max 8 chars
 	String PLACETORNEO; // max 40 chars
 	String HOSTFEDERATION; // max 3 chars
 	String DATATORNEO_B; // max 10 chars
 	String DATATORNEO_E; // max 10 chars
 	String ARBITER;  // max 40 chars
 	String PATHDIR, PATHDIRWWW, PATHVEGAPRG, PATHDIRDB, USER_REGISTERED;
 	String LANGUAGE; //useful for menu in different language
 	double WON, DRAW;
 	int tyebreakorder[FUNC_CMP+1];
 	String tyebreakname[FUNC_CMP+1];
 	int NPlayer;   // player partecipating to the tournament
 	int NAval;
 	int NRounds, NBoards, SCORE_TYPE;   // rounds of the tournament
 	int NAccRound;
 	int currentRound;
 	int MAX_PLAYERS;
 	int PLAY_SYSTEM, STATUS_ROUND;
 	bool IS_DONE_MANUAL, IS_TORNEO_STARTED, IS_VEG_SAVED, NEW_TORNEO, DROP_PLAYER,
         IS_CLOSED_REGISTRATION, IS_SAVED_ROUND, AVOID_SAME_COUNTRY,
         IS_SET_VERBOSE, IS_FIDE_TOURNAMENT, PERMIT3COLOR, LIMIT_FLOATER, IS_SWISS;
 	TournamentStatus();
	~TournamentStatus(){;}
};


////// class RoundRobinTournament derived from class Tournament ////////////////
class RoundData : public TournamentStatus {
public:
    int   countnodes, nodes[MAXDIM+1]; // useful for ForlanoScoreSystem
 	// ROUND ROBIN
 	// pairing and result. pairing[0][r] is the number of pairs at the round r
 	int   pairingRR[N_PLAYER_MAX_RR/2+2][N_ROUND_MAX_RR+1];
	int   pairingTRUE[ N_PLAYER_MAX_TRUE/2+2][N_ROUND_MAX+1];
	int   roundResultRR[N_PLAYER_MAX_RR+1][N_ROUND_MAX_RR+1];
	int   roundTRUEResult[N_PLAYER_MAX_TRUE+1][N_ROUND_MAX+1];   //remind the players result
    int   playerColorRR[N_PLAYER_MAX_RR+1][N_ROUND_MAX_RR+1];
    int   playerOpponentRR[N_PLAYER_MAX_RR+1][N_ROUND_MAX_RR+1];
	int   playerTRUEOpponent[N_PLAYER_MAX_TRUE+1][N_ROUND_MAX+1];
	float teamScore[N_PLAYER_MAX+1][N_ROUND_MAX+1]; // remind the match score
    // SWISS SYSTEM
    // pairing and result. pairing[0][r] is the number of pairs at the round r
	float ScoreGroup[N_ROUND_MAX*4 + 1];
    int   playerFloater[N_PLAYER_MAX+1][N_ROUND_MAX+1];
 	int   pairing[N_PLAYER_MAX/2+1][N_ROUND_MAX+1];
	int   pairingTRUERR[N_PLAYER_MAX_TRUE_RR/2+2][N_ROUND_MAX_RR+1];
	int   roundResult[N_PLAYER_MAX+1][N_ROUND_MAX+1]; //remind the code result
	int   roundTRUEResultRR[N_PLAYER_MAX_TRUE_RR+1][N_ROUND_MAX_RR+1];  //remind the players result
	int   playerColor[N_PLAYER_MAX+1][N_ROUND_MAX+1];
    int   playerOpponent[N_PLAYER_MAX+1][N_ROUND_MAX+1];
	int   playerTRUEOpponentRR[N_PLAYER_MAX_TRUE_RR+1][N_ROUND_MAX_RR+1];
	float teamScoreRR[N_PLAYER_MAX_RR+1][N_ROUND_MAX_RR+1]; // remind the match score

	RoundData();
	~RoundData(){;}
    //general function
    void SaveVegFile(bool flag);
    void Reset();
    void StartSystem(int play_system);
	void DecodeResult(int const code, int *idwhite, int *idblack, int *res);
    int  CodifyResult(int idwhite, int idblack, int res);
	void SetPlayersResult(int round);
//	void PrepareHTMLindex(void);
    void ShowTableScore(int nplayers);
    void ShowTableCategoryScore(int nplayers);
    void ShowRoundCard2(int round);
    void ShowRoundCard3(int round);
	void SavePairsResultFile(int round);
	void SavePairsFile(int round);
    void DeleteRound(int fromround, int toround);
	void setAPRO(int round, int numberPlayer);
    String LabelResult(int result, int code, int round);
    String LabelResultNonStandard(int idw, int idb, int round);
    String HTMLCommonHeader();
    String HTMLNavBar();
    void ShowTableStatHTML();
    //Round Robin specific methods
  	void PrepareRRPairing(int nplayer);
	int  GenerateRRCalendar(int nplayer, int pairs[][N_ROUND_MAX_RR+1]);
	void SaveRoundRR(int round);
	void LoadRoundRR(String filename);
	void AdjournCrossTableRR(int round);
    void CrossTableRoundRobin(int nplayers);
    void RoundRobinCalendar();
//Swiss specific methods
	void SaveRound(int round);
	void LoadRound(String filename);
    void AdjournCrossTableSwiss(int round);
    void CrossTableSwiss();
    int  MakeNextPairing();
    void DoFirstRound();
    void AdjournPlayerState(int round);
   	int  AssignColor(int id1, int id2, int round);
   	int  AssignColorLIM(int id1, int id2, int round);
	void VerifyColor(int round);
	void Floyd(int l[MAXDIM+1][MAXDIM+1], int d[MAXDIM+1][MAXDIM+1], int n);
	void Path1(int d[MAXDIM+1][MAXDIM+1], int n, int i, int j);
	void Path(int d[MAXDIM+1][MAXDIM+1], int n, int i, int j);
	void ForlanoScoreSystem(int m);
//National stuff
    void NationalPlayerCard(int nplayers);
    void NationalPlayerCardHTML(int nplayers);
 	void calcelo (int idp, double arrp[][2], int round);
	void SaveFSIRatingReport();
	void TestResult(int res, int code[]);
	void ConvertUSCFDB();
//FIDE stuff
    void ShowTableStat();
    void FidePlayerCard();
	void SortedCross(int nplayers, int flag);
	void FideEloVariation(int nplayers);
	void FideEloVariationHTML();
    void FindNorm();
	int  CalculatePlayerNorm(int i, int typenorm, int arrrating[]);
    void calceloFIDE(int idp, double arrp[][2], int round);
	void EloForesee(int nplayers, int round);
	void CalcVarElo(int nplayers);
};

///// class Team ///////////////////////
class PlayerArchive : Moveable<PlayerArchive>  {
public:
// anagraphic
     String name; //25 chars max
	 String country; //3 chars max
	 String data;  //10 chars max
	 String sex;    //1 chars max

     int  idfide;   //8 chars max
     int  ratingfide; //4 chars max

	 int  idnat;  //8 chars max
     int  ratingnat;  //4 chars max
	 int  kcoeff;  //3 chars max

     int  title;        // int corrispondent to codetitle used during pairing
     String codetitle; // used during pairing, max 3 chars (GM, WGM, IM, ecc...
// status
	 String  isAvailable; //"0"=out, Null=get the bye - only for swiss, "1"=play

	 int  RATING;       // real rating used for pairing and printed in the output
                        // it depends by the FIDE flag

     float tiebreak[N_COL]; //contains the score for each tiebreak

    //int TitleToCode(); // set title corrispondent to codetitle - used during pairing

    PlayerArchive();
    ~PlayerArchive(){;}
};

///// class Team ///////////////////////
class TeamArchive : Moveable<TeamArchive>  {
public:
// anagraphic
     String name; //25 chars max
	 String country; //15 chars max
	 String data;  //10 chars max
	 String origin;  //15 chars max
	 String sex;    //1 chars max

     int  idfide;   //8 chars max
     int  ratingfide; //4 chars max

	 int  idnat;  //8 chars max
     int  ratingnat;  //4 chars max
	 int  kcoeff;  //3 chars max

     int  title;        // int corrispondent to codetitle used during pairing
     String codetitle; // used during pairing, max 3 chars (GM, WGM, IM, ecc...
// status
	 String  isAvailable; //"0"=out, Null=get the bye - only for swiss, "1"=play

// pairing data
	 int  rating;       // for DUBOV SYSTEM : rating = RATING,
                        // for VEGA system :  rating = BUCHOLZ
     float aro;         // for DUBOV system : aro = ARO
                        // for VEGA system : aro = BUCHOLZ
	 int  RATING;       // real rating used for pairing and printed in the output
	                    // (it is useful when .rating is used for something else)
                        // it depends by the FIDE flag

    // player state useful for swiss system
    int   duecolor;
	bool  canChangeDuecolor;
	int   currentDuecolor;
	int   colorDiff;
	int   colorCons;
	int   scoreGroup;
	int   currentGroup;
	bool  canUpfloat;  // 0 if cannot upfloat, 1 otherwise //int  canDownfloat;  // 0 if cannot upfloat, 1 otherwise
	int   canGetBYE;
 	bool  isAlreadyTested;
    float tiebreak[N_COL]; //contains the score for each tiebreak

    int TitleToCode(); // set title corrispondent to codetitle - used during pairing
	PlayerArchive plr[MAX_BOARDS];

    TeamArchive();
    ~TeamArchive(){;}
};


extern RoundData TD;
extern TeamArchive player[N_PLAYER_MAX+1];
extern VectorMap<String, TeamArchive> db;
#endif
