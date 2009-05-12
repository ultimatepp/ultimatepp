#ifndef _UltimatePlayer_UltimatePlayer_h
#define _UltimatePlayer_UltimatePlayer_h

using namespace Upp;

#define SCHEMADIALECT <plugin/Sqlite3/Sqlite3Schema.h>
#define MODEL <UltimatePlayer/UltimatePlayer.sch>
#include "Sql/sch_header.h"

#define LAYOUTFILE <UltimatePlayer/UltimatePlayer.lay>
#include <CtrlCore/lay.h>

#define IMAGEFILE <UltimatePlayer/UltimatePlayer.iml>
#define IMAGECLASS Images
#include <Draw/iml_header.h>
#include "FMod.h"

enum 
{
	ORDER_DSC = 0,
	ORDER_ASC = 1,
	ORDER_SHUFLE = 2,
	REPEAT_ONE = 0,
	REPEAT_PLAYLIST = 1,
	REPEAT_ALL = 2,
	REPEAT_NONE = 3
};

struct ConvMs : Convert
{
    virtual Value Format(const Value& q) const;
};

class FileAssoc : public WithFilesLayout<TopWindow>
{
	public:
		typedef FileAssoc CLASSNAME;
		
		FileAssoc();
		
};

class Player : public WithPlayerLayout<TopWindow>
{
	public:
		typedef Player CLASSNAME;
	
		SoundSystem sndsys;
		SoundStream mpx_stream, cd_stream, stream;
		
		FileSel filesel;	
		FileSel foldersel;
		MenuBar menu;	
		int lang, curlang;
		int prevpos;
		int track;
		
		bool isplaying;
		int playid;
		int prev_playid;
		int playrow;
		
		int order_mode;
		int repeat_mode;
		bool rebuild_songs_list;
		bool stop_play;
		
		EditString name;
		//NOTIFYICONDATA tray;   
		HICON trayicon;
		TaskbarButton tray;
		//TrayIcon tray;
		
		Array<GridCtrl> pls;
		Array<GridCtrl> sys;
		GridCtrl * cpl, * opl;
		GridCtrl list;
		GridCtrl system;
		
		bool register_startup;
		bool follow_song;
		bool play_last_song;
		bool goto_last_song;
		bool bring_to_front;
		
		VectorMap<String, int> register_exts;		
		Vector<int> songs; 
		
		Splitter left;
						
		Player();
		~Player();
		
		void Initialize();
				
		void AddPlaylist(bool append);
		void ReadPlaylists();
		GridCtrl& AddPlaylistControl(Array<GridCtrl> &pls, int n = -1, bool cd = false);
		
		bool MakeDefaultPlaylistName(String &name);
		
		void SystemChangeRow();

		void ListInsertRow();
		void ListUpdateRow();
		void ListNewRow();
		void ListRemoveRow();
		void ListChangeRow();

		void ListAdd()    { AddPlaylist(1); }
		void ListInsert() { AddPlaylist(0); }
		
		void ListAppendFolders(bool append);
		void ListAddFolders();
		void ListInsertFolders();
		
		bool IsMusicFile(String &path);
		bool IsCDFile(String &path);
		String GetCDDrive(String &path);

		void AddFiles(bool append, String path);		
		void AddFile(bool append, const String &file);

		void AddFiles(bool append);
		void AddFolders(bool append);
		
		void PLAddFiles()       { AddFiles(1);   }
		void PLAddFolders()     { AddFolders(1); }
		void PLInsertFiles()    { AddFiles(0);   }
		void PLInsertFolders()  { AddFolders(0); }
		
		void PLRemoveRow();
				
		void ShowAddPlaylistMenu();
		void ShowAddMusicMenu();
		void AddMusicBar(Bar &bar);
		
		int GetPlayRow();
		int FindPlaylist(GridCtrl &pl);
		int FindPlaylistId(GridCtrl &pl);
		
		void PopupMenuBar(Bar &bar);		
		void SystemMenuBar(Bar &bar);
		
		void StorePositions();
		
		void LoadCD();
		
		GridCtrl& GetPlaylist();
		
		void SetShellPlaylist();
		bool IsShellPlaylist(GridCtrl &pl);
		bool IsSystemPlaylist(GridCtrl &pl, bool noexplorator = false);
		bool IsCDPlaylist(GridCtrl &pl);
		
		int  GetCDTrackNum(const String& filename);
		
		void StopPlay();
				
		void Play();
		void Stop(bool stopstream = false, bool resetcpl = true);
		void RealStop() { Stop(true); }
		
		void TrayPlay();
		void TrayStop();

		bool PlayDir(bool forward);
		bool PlayNext() { return PlayDir(true);  }
		bool PlayPrev() { return PlayDir(false); }
		
		void OnPlayNext() { PlayNext(); }
		void OnPlayPrev() { PlayPrev(); }
		
		bool IsPlaying();
		
		void OnSetPos();
		void OnSetVol();
		
		void SetOrder();
		void OnChangeOrder();
		void SetRepeat();
		void OnChangeRepeat();
		
		void UpdateProgress();
		void UpdatePlayButton();

		void ChangePlaylistName();
		void AddPlaylistBar(Bar &bar);
		void PopupListMenuBar(Bar &bar);
		
		void MenuBar(Bar &bar);
		void FilesBar(Bar &bar);
		void LanguagesBar(Bar &bar);
		void OptionsBar(Bar &bar);
		void Nothing() {};
				
		void Serialize(Stream &s);
		
		void SetLang(int l);
				
		void RegisterExt(const char * ext);
		void RegisterExts(bool force);
		void FileAssociations();
		void Test();
		void GoToSong();
		void GoTo(bool gothere);
		//void GoTo(int);
		void FollowSong();
		void PlayLastSong();
		void GotoLastSong();
		void BringToFront();
		
		void BuildSongsList();
		int  GetSongRow0(GridCtrl &pl, int played_col);
		int  GetSongRow();

		void ToTray();
		void ShowPlayer();
		bool AddCmd(const Vector<String> &cmd, bool play = true);
		
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		virtual void State(int reason);
};

#endif

